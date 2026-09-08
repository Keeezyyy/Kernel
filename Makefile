# ---------------------------------------------------------------------------
# x86_64 Kernel - BOOTBOOT Protokoll (https://gitlab.com/bztsrc/bootboot)
#
#   make            -> build/kernel.elf
#   make check      -> prueft den Kernel auf BOOTBOOT-Konformitaet
#   make img        -> build/disk.img (GPT + ESP + BOOTBOOT-Loader + initrd)
#   make run        -> QEMU, BIOS-Boot, COM1 auf stdio + build/serial.log
#   make run-efi    -> QEMU, UEFI-Boot (OVMF)
#   make debug      -> QEMU haelt an und wartet auf gdb (Port 1234)
#   make gdb        -> gdb starten und mit "make debug" verbinden
#   make debug-gdb  -> QEMU im Hintergrund + gdb im Vordergrund (ein Terminal)
#   make info       -> Toolchain, Quelldateien und Flags anzeigen
#   make clean      -> Build-Artefakte loeschen
#   make distclean  -> zusaetzlich generierte initrd-Dateien loeschen
#
#   DEBUG=1     haengt den gdb-Stub an run/run-efi an, Gast laeuft sofort los:
#                                                      make run DEBUG=1
#   DEBUG=wait  wie oben, aber QEMU wartet auf gdb:     make run DEBUG=wait
#   GDBPORT=... Port des gdb-Stubs (Vorgabe 1234):      make debug GDBPORT=9000
#
#   V=1     zeigt die vollstaendigen Kommandos:        make V=1
#   -j$(nproc) wird unterstuetzt:                      make -j8
#
#   Quellen unter src/ UND unter include/ werden rekursiv mitgebaut.
#
#   In der QEMU-Konsole: Ctrl-A C schaltet zum Monitor, Ctrl-A X beendet.
# ---------------------------------------------------------------------------

# --- Make-Verhalten --------------------------------------------------------
MAKEFLAGS    += --no-builtin-rules --no-builtin-variables
.SUFFIXES:
.DELETE_ON_ERROR:
.DEFAULT_GOAL := all

# --- Toolchain -------------------------------------------------------------
TARGET  := x86_64-elf
CC      := $(TARGET)-gcc
LD      := $(TARGET)-ld
OBJCOPY := $(TARGET)-objcopy
SIZE    := $(TARGET)-size
HOSTCC  := cc
# NASM uebersetzt die .asm-Dateien (Intel-Syntax), GAS die .s/.S.
NASM    := nasm

# Bevorzugt den Cross-gdb (kennt i386:x86-64 garantiert), faellt sonst auf
# den System-gdb zurueck. Ueberschreibbar: make gdb GDB=/pfad/zu/gdb
GDB     ?= $(firstword $(shell command -v $(TARGET)-gdb gdb 2>/dev/null) gdb)

# --- Verzeichnisse ---------------------------------------------------------
SRC_DIR   := src
INC_DIR   := include
BUILD_DIR := build
INITRD    := initrd

# Objekte aus include/ landen unter build/include/, damit sie nicht mit
# gleichnamigen Dateien aus src/ kollidieren.
INC_OBJ_DIR := $(BUILD_DIR)/$(INC_DIR)

LDSCRIPT := link.ld
KERNEL   := $(BUILD_DIR)/kernel.elf
CORE     := $(BUILD_DIR)/core          # Kernel ohne Debug-Sektionen, wandert ins initrd
IMG      := $(BUILD_DIR)/disk.img
GDBINIT  := $(BUILD_DIR)/gdbinit

# --- mkbootimg (aus tools/bootboot mitgeliefert, wird lokal gebaut) --------
MKBOOTIMG_DIR  := tools/bootboot/mkbootimg
MKBOOTIMG      := $(BUILD_DIR)/mkbootimg
MKBOOTIMG_SRCS := $(filter-out $(MKBOOTIMG_DIR)/bin2h.c,$(wildcard $(MKBOOTIMG_DIR)/*.c))

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
  MKBOOTIMG_DEFS := -DMACOSX
endif

# --- Serielle Schnittstelle (COM1) -----------------------------------------
# Diese Werte gehen als -D an den Compiler und werden in src/serial.c benutzt.
# Divisor 3 entspricht 115200/3 = 38400 Baud. QEMU ignoriert Divisor, Paritaet
# und Stoppbits, ein echter UART nicht - dort muessen beide Seiten passen.
SERIAL_PORT    := 0x3F8
SERIAL_DIVISOR := 3
SERIAL_LOG     := $(BUILD_DIR)/serial.log

# --- QEMU ------------------------------------------------------------------
QEMU    := qemu-system-x86_64
GDBPORT ?= 1234
GDBSTUB := -gdb tcp::$(GDBPORT)

QEMUBASE := -drive format=raw,file=$(IMG) -m 256 \
            -no-reboot -no-shutdown -d guest_errors

# COM1 haengt an stdio und wird zusaetzlich mitgeschrieben.
#   mux=on     -> Ctrl-A C wechselt zwischen Gastkonsole und QEMU-Monitor
#   signal=off -> Ctrl-C geht an den Gast statt QEMU zu beenden
QEMUCON := -chardev stdio,id=com1,mux=on,signal=on,logfile=$(SERIAL_LOG) \
           -serial chardev:com1 -mon chardev=com1

QEMUOPTS := $(QEMUBASE) $(QEMUCON)

# Fuer "make debug-gdb": stdio gehoert dem gdb, COM1 geht in die Logdatei.
QEMUOPTS_BG := $(QEMUBASE) -serial file:$(SERIAL_LOG) -display none

# DEBUG=1    -> gdb-Stub laeuft mit, Gast startet sofort (spaeter anhaengen)
# DEBUG=wait -> Stub laeuft mit, Gast haelt vor der ersten Instruktion
ifeq ($(DEBUG),1)
  QEMUOPTS += $(GDBSTUB)
endif
ifeq ($(DEBUG),wait)
  QEMUOPTS += $(GDBSTUB) -S
endif

# UEFI-Firmware (OVMF). Wird automatisch gesucht; bei Bedarf ueberschreiben:
#   make run-efi OVMF_DIR=/pfad/zu/share/qemu
OVMF_DIR  ?= $(firstword $(wildcard /opt/homebrew/share/qemu \
                                    /usr/local/share/qemu \
                                    /usr/share/qemu \
                                    /usr/share/OVMF \
                                    /usr/share/edk2/x64))
OVMF_CODE ?= $(firstword $(wildcard $(OVMF_DIR)/edk2-x86_64-code.fd \
                                    $(OVMF_DIR)/OVMF_CODE.4m.fd \
                                    $(OVMF_DIR)/OVMF_CODE.fd))
OVMF_VARS ?= $(firstword $(wildcard $(OVMF_DIR)/edk2-i386-vars.fd \
                                    $(OVMF_DIR)/OVMF_VARS.4m.fd \
                                    $(OVMF_DIR)/OVMF_VARS.fd))

# --- Flags -----------------------------------------------------------------
WARNINGS := -Wall -Wextra -Wshadow -Wundef -Wvla -Wstrict-prototypes \
            -Wmissing-prototypes -Wredundant-decls

# -fpic + ein einziges PT_LOAD-Segment sind BOOTBOOT-Vorgaben.
# -mno-{mmx,sse,sse2} verhindert, dass GCC SIMD-Register benutzt, die im
# Kernel-Kontext nicht initialisiert sind. -mno-red-zone ist fuer Interrupts
# zwingend.
CFLAGS := -std=gnu11 -ffreestanding -O2 -g $(WARNINGS) \
          -I$(INC_DIR) -I$(SRC_DIR) \
          -DSERIAL_PORT=$(SERIAL_PORT) -DSERIAL_DIVISOR=$(SERIAL_DIVISOR) \
          -fpic -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
          -fno-stack-protector -fno-omit-frame-pointer \
          -fno-common -ffunction-sections -fdata-sections

# Fremdcode unter include/ (z.B. printf) haelt sich nicht zwingend an die
# strengen Projektwarnungen. Gleiche Codegen-Flags, aber nur -Wall.
#
# PRINTF_DISABLE_SUPPORT_FLOAT ist zwingend: %f/%e wuerden ein double per
# va_arg holen, was die SysV-ABI ueber XMM-Register abwickelt - mit -mno-sse
# bricht der Compiler das mit "SSE register argument with SSE disabled" ab.
# Der Kernel initialisiert FPU/SSE ohnehin nicht, Fliesskomma hat hier nichts
# verloren. (Deaktiviert implizit auch %e/%g.)
EXT_CFLAGS := $(filter-out $(WARNINGS),$(CFLAGS)) -Wall \
              -DPRINTF_DISABLE_SUPPORT_FLOAT

ASFLAGS := $(CFLAGS)
# NASM erzeugt reine .text/.data-Sektionen; -g -F dwarf haelt die Zeilennummern
# fuer "make gdb" verfuegbar.
NASMFLAGS := -f elf64 -g -F dwarf
DEPFLAGS = -MMD -MP -MF $(@:.o=.d)

LDFLAGS := -nostdlib -n -T $(LDSCRIPT)
# --no-warn-rwx-segments gibt es erst ab binutils 2.39
ifneq ($(shell $(LD) --help 2>/dev/null | grep -c -- --no-warn-rwx-segments),0)
  LDFLAGS += --no-warn-rwx-segments
endif

# --- Quellen ---------------------------------------------------------------
# find laeuft rekursiv: jede neue .c/.s/.S unterhalb von src/ bzw. include/
# wird ohne weiteres Zutun mitgebaut. sort sorgt fuer eine reproduzierbare
# Linkreihenfolge.
C_SRCS := $(sort $(shell find $(SRC_DIR) -name '*.c' 2>/dev/null))
S_SRCS := $(sort $(shell find $(SRC_DIR) \( -name '*.s' -o -name '*.S' \) 2>/dev/null))
ASM_SRCS := $(sort $(shell find $(SRC_DIR) -name '*.asm' 2>/dev/null))

INC_C_SRCS := $(sort $(shell find $(INC_DIR) -name '*.c' 2>/dev/null))
INC_S_SRCS := $(sort $(shell find $(INC_DIR) \( -name '*.s' -o -name '*.S' \) 2>/dev/null))
INC_ASM_SRCS := $(sort $(shell find $(INC_DIR) -name '*.asm' 2>/dev/null))

SRC_OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.c.o,$(C_SRCS)) \
            $(patsubst $(SRC_DIR)/%.s,$(BUILD_DIR)/%.s.o,$(filter %.s,$(S_SRCS))) \
            $(patsubst $(SRC_DIR)/%.S,$(BUILD_DIR)/%.S.o,$(filter %.S,$(S_SRCS))) \
            $(patsubst $(SRC_DIR)/%.asm,$(BUILD_DIR)/%.asm.o,$(ASM_SRCS))

INC_OBJS := $(patsubst $(INC_DIR)/%.c,$(INC_OBJ_DIR)/%.c.o,$(INC_C_SRCS)) \
            $(patsubst $(INC_DIR)/%.s,$(INC_OBJ_DIR)/%.s.o,$(filter %.s,$(INC_S_SRCS))) \
            $(patsubst $(INC_DIR)/%.S,$(INC_OBJ_DIR)/%.S.o,$(filter %.S,$(INC_S_SRCS))) \
            $(patsubst $(INC_DIR)/%.asm,$(INC_OBJ_DIR)/%.asm.o,$(INC_ASM_SRCS))

OBJS := $(SRC_OBJS) $(INC_OBJS)
DEPS := $(OBJS:.o=.d)

# Dateien im initrd (ohne das generierte sys/core), damit das Image bei
# Aenderungen an der Konfiguration neu gebaut wird.
INITRD_FILES := $(shell find $(INITRD) -type f ! -name core 2>/dev/null)

# --- Ausgabe ---------------------------------------------------------------
V ?= 0
ifeq ($(V),0)
  Q := @
else
  Q :=
endif

# --- Toolchain-Pruefung (nur wenn wirklich gebaut wird) --------------------
GOALS := $(if $(MAKECMDGOALS),$(MAKECMDGOALS),all)
ifneq ($(filter all check img run run-efi debug debug-gdb,$(GOALS)),)
  ifeq ($(shell command -v $(CC) 2>/dev/null),)
    $(error Cross-Compiler "$(CC)" nicht gefunden. \
      Auf macOS z.B.: brew install x86_64-elf-gcc x86_64-elf-binutils)
  endif
  ifeq ($(strip $(C_SRCS)$(S_SRCS)$(ASM_SRCS)$(INC_C_SRCS)$(INC_S_SRCS)$(INC_ASM_SRCS)),)
    $(error Keine Quelldateien unter $(SRC_DIR)/ oder $(INC_DIR)/ gefunden)
  endif
  ifneq ($(strip $(ASM_SRCS)$(INC_ASM_SRCS)),)
    ifeq ($(shell command -v $(NASM) 2>/dev/null),)
      $(error "$(NASM)" nicht gefunden, wird aber fuer .asm gebraucht. \
        Auf macOS z.B.: brew install nasm)
    endif
  endif
endif

.PHONY: all check img run run-efi debug debug-gdb gdb info help clean distclean serial-log

all: $(KERNEL)

# --- Kompilieren: include/ -------------------------------------------------
# Steht bewusst vor den src/-Regeln, damit build/include/... eindeutig hier
# landet und nicht ueber den Umweg src/include/... gesucht wird.
$(INC_OBJ_DIR)/%.c.o: $(INC_DIR)/%.c $(MAKEFILE_LIST)
	@mkdir -p $(dir $@)
	@echo "  CC      $<"
	$(Q)$(CC) $(EXT_CFLAGS) $(DEPFLAGS) -c $< -o $@

$(INC_OBJ_DIR)/%.s.o: $(INC_DIR)/%.s $(MAKEFILE_LIST)
	@mkdir -p $(dir $@)
	@echo "  AS      $<"
	$(Q)$(CC) $(EXT_CFLAGS) $(DEPFLAGS) -c $< -o $@

$(INC_OBJ_DIR)/%.S.o: $(INC_DIR)/%.S $(MAKEFILE_LIST)
	@mkdir -p $(dir $@)
	@echo "  AS      $<"
	$(Q)$(CC) $(EXT_CFLAGS) $(DEPFLAGS) -c $< -o $@

$(INC_OBJ_DIR)/%.asm.o: $(INC_DIR)/%.asm $(MAKEFILE_LIST)
	@mkdir -p $(dir $@)
	@echo "  NASM    $<"
	$(Q)$(NASM) $(NASMFLAGS) -MD $(@:.o=.d) -MT $@ -o $@ $<

# --- Kompilieren: src/ -----------------------------------------------------
$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c $(MAKEFILE_LIST)
	@mkdir -p $(dir $@)
	@echo "  CC      $<"
	$(Q)$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(BUILD_DIR)/%.s.o: $(SRC_DIR)/%.s $(MAKEFILE_LIST)
	@mkdir -p $(dir $@)
	@echo "  AS      $<"
	$(Q)$(CC) $(ASFLAGS) $(DEPFLAGS) -c $< -o $@

# .S laeuft zusaetzlich durch den Praeprozessor
$(BUILD_DIR)/%.S.o: $(SRC_DIR)/%.S $(MAKEFILE_LIST)
	@mkdir -p $(dir $@)
	@echo "  AS      $<"
	$(Q)$(CC) $(ASFLAGS) $(DEPFLAGS) -c $< -o $@

# .asm laeuft ueber NASM (Intel-Syntax), nicht ueber GAS
$(BUILD_DIR)/%.asm.o: $(SRC_DIR)/%.asm $(MAKEFILE_LIST)
	@mkdir -p $(dir $@)
	@echo "  NASM    $<"
	$(Q)$(NASM) $(NASMFLAGS) -MD $(@:.o=.d) -MT $@ -o $@ $<

# --- Linken (higher half, ein einziges PT_LOAD-Segment) --------------------
$(KERNEL): $(OBJS) $(LDSCRIPT)
	@mkdir -p $(dir $@)
	@echo "  LD      $@"
	$(Q)$(LD) $(LDFLAGS) -o $@ $(OBJS)
	$(Q)$(SIZE) $@ 2>/dev/null || true

# Fuer das Image reicht der Kernel ohne DWARF-Sektionen; die Symbole bleiben
# in $(KERNEL) und stehen damit "make gdb" weiterhin zur Verfuegung.
$(CORE): $(KERNEL)
	@echo "  STRIP   $@"
	$(Q)$(OBJCOPY) --strip-debug $< $@

# --- Image-Tool ------------------------------------------------------------
$(MKBOOTIMG): $(MKBOOTIMG_SRCS)
	@mkdir -p $(dir $@)
	@echo "  HOSTCC  $@"
	$(Q)$(HOSTCC) -O2 -w $(MKBOOTIMG_DEFS) -o $@ $(MKBOOTIMG_SRCS)

check: $(KERNEL) $(MKBOOTIMG)
	$(Q)$(MKBOOTIMG) check $(KERNEL)

# --- initrd ----------------------------------------------------------------
# Nur ein Fallback: eine vorhandene Konfiguration wird nie ueberschrieben.
$(INITRD)/sys/config:
	@mkdir -p $(dir $@)
	@echo "  GEN     $@"
	$(Q)printf 'screen=1024x768\nkernel=sys/core\n' > $@

# --- Disk Image ------------------------------------------------------------
img: $(IMG)

$(IMG): $(CORE) $(MKBOOTIMG) disk.json $(INITRD)/sys/config $(INITRD_FILES)
	@mkdir -p $(INITRD)/sys
	$(Q)cp $(CORE) $(INITRD)/sys/core
	@echo "  IMG     $@"
	$(Q)$(MKBOOTIMG) disk.json $@

# --- Ausfuehren ------------------------------------------------------------
run: $(IMG)
	@mkdir -p $(dir $(SERIAL_LOG))
	$(QEMU) $(QEMUOPTS)

# OVMF braucht eine beschreibbare Kopie der Variablen
$(BUILD_DIR)/ovmf-vars.fd: $(OVMF_VARS)
	@mkdir -p $(dir $@)
	$(Q)cp $(OVMF_VARS) $@
	$(Q)chmod u+w $@

run-efi: $(IMG) $(BUILD_DIR)/ovmf-vars.fd
ifeq ($(strip $(OVMF_CODE)),)
	$(error OVMF nicht gefunden. Pfad angeben: make run-efi OVMF_DIR=/pfad/zu/share/qemu)
endif
	@mkdir -p $(dir $(SERIAL_LOG))
	$(QEMU) $(QEMUOPTS) \
	  -drive if=pflash,format=raw,unit=0,readonly=on,file=$(OVMF_CODE) \
	  -drive if=pflash,format=raw,unit=1,file=$(BUILD_DIR)/ovmf-vars.fd

# --- Debuggen --------------------------------------------------------------
# Kommandodatei fuer gdb. Wird aus den Makefile-Variablen erzeugt, damit
# GDBPORT und Symboldatei nicht doppelt gepflegt werden muessen.
$(GDBINIT): $(MAKEFILE_LIST)
	@mkdir -p $(dir $@)
	@echo "  GEN     $@"
	$(Q)printf '%s\n' \
	  'set confirm off' \
	  'set pagination off' \
	  'set disassembly-flavor intel' \
	  'set architecture i386:x86-64' \
	  'symbol-file $(KERNEL)' \
	  'target remote localhost:$(GDBPORT)' \
	  > $@

# QEMU haelt vor der ersten Instruktion an und wartet auf gdb (Terminal 1)
debug: $(IMG)
	@mkdir -p $(dir $(SERIAL_LOG))
	$(QEMU) $(QEMUBASE) $(QEMUCON) $(GDBSTUB) -S

# gdb starten und mit "make debug" verbinden (Terminal 2)
gdb: $(GDBINIT)
	$(GDB) -x $(GDBINIT)

# Alles in einem Terminal: QEMU im Hintergrund, gdb im Vordergrund.
# COM1 landet in $(SERIAL_LOG), "make serial-log" zeigt es live mit.
# Beim Verlassen von gdb wird QEMU mit beendet.
debug-gdb: $(IMG) $(GDBINIT)
	@mkdir -p $(dir $(SERIAL_LOG))
	@echo "  QEMU    Hintergrund, COM1 -> $(SERIAL_LOG), Stub auf Port $(GDBPORT)"
	$(Q)$(QEMU) $(QEMUOPTS_BG) $(GDBSTUB) -S & \
	  qpid=$$!; trap 'kill $$qpid 2>/dev/null' EXIT INT TERM; \
	  sleep 1; $(GDB) -x $(GDBINIT)

# Mitgeschriebene COM1-Ausgabe live verfolgen (drittes Terminal)
serial-log:
	$(Q)touch $(SERIAL_LOG)
	tail -f $(SERIAL_LOG)

# --- Hilfsziele ------------------------------------------------------------
info:
	@echo "Host        : $(UNAME_S)"
	@echo "CC          : $(CC) ($(shell $(CC) -dumpversion 2>/dev/null || echo 'nicht gefunden'))"
	@echo "LD          : $(LD)"
	@echo "GDB         : $(GDB) ($(shell $(GDB) --version 2>/dev/null | head -1 || echo 'nicht gefunden'))"
	@echo "GDB-Stub    : tcp::$(GDBPORT)   DEBUG=$(if $(DEBUG),$(DEBUG),<aus>)"
	@echo "OVMF_CODE   : $(if $(OVMF_CODE),$(OVMF_CODE),<nicht gefunden>)"
	@echo "COM1        : Port $(SERIAL_PORT), Divisor $(SERIAL_DIVISOR), Log $(SERIAL_LOG)"
	@echo "C-Quellen   : $(if $(C_SRCS),$(C_SRCS),<keine>)"
	@echo "NASM        : $(NASM) ($(shell $(NASM) -v 2>/dev/null || echo 'nicht gefunden'))"
	@echo "ASM-Quellen : $(if $(S_SRCS)$(ASM_SRCS),$(S_SRCS) $(ASM_SRCS),<keine>)"
	@echo "Quellen inc : $(if $(INC_C_SRCS)$(INC_S_SRCS)$(INC_ASM_SRCS),$(INC_C_SRCS) $(INC_S_SRCS) $(INC_ASM_SRCS),<keine>)"
	@echo "Objekte     : $(OBJS)"
	@echo "CFLAGS      : $(CFLAGS)"
	@echo "EXT_CFLAGS  : $(EXT_CFLAGS)"
	@echo "LDFLAGS     : $(LDFLAGS)"

help:
	@sed -n '2,26p' $(firstword $(MAKEFILE_LIST)) | sed 's/^# \{0,1\}//'

clean:
	$(Q)rm -rf $(BUILD_DIR)

distclean: clean
	$(Q)rm -f $(INITRD)/sys/core

-include $(DEPS)
