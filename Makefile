
# ======================================
#  Tools
# ======================================
CC64  = x86_64-elf-gcc
CXX64 = x86_64-elf-g++
LD64  = x86_64-elf-ld
AS    = nasm

CFLAGS64   = -g -O0 -ffreestanding -fno-pie -fno-stack-protector -m64 -mcmodel=large -O2 -Wall -Wextra -mno-red-zone
CXXFLAGS64 = $(CFLAGS64) -fno-exceptions -fno-rtti
ASFLAGS64  = -f elf64

# Limine (Homebrew)
LIMINE_PREFIX := $(shell brew --prefix limine)
LIMINE_BIN    := $(LIMINE_PREFIX)/bin/limine
LIMINE_SHARE  := $(LIMINE_PREFIX)/share/limine

# ======================================
#  Verzeichnisse / Dateien
# ======================================
SRC_KERNEL    = src
BUILD         = build
KERNEL_DIR    = $(BUILD)/kernel
LINKER_SCRIPT = $(SRC_KERNEL)/linker.ld

KERNEL_ELF = $(BUILD)/kernel.elf

IMG_DIR    = images
HDD_IMAGE  = $(IMG_DIR)/image.hdd
IMG_OFFSET = @@1M   # 2048 * 512 = 1 MiB

# Wichtig: Limine >= 9 nutzt limine.conf (nicht limine.cfg)
LIMINE_CONF = iso_root/boot/limine/limine.conf

# ======================================
#  Dateisuche (mit find)
# ======================================
KERNEL_C_SRC   = $(shell find $(SRC_KERNEL) -type f -name "*.c")
KERNEL_CPP_SRC = $(shell find $(SRC_KERNEL) -type f -name "*.cpp")
KERNEL_ASM_SRC = $(shell find $(SRC_KERNEL) -type f -name "*.asm")

KERNEL_C_OBJ   = $(patsubst $(SRC_KERNEL)/%.c,$(KERNEL_DIR)/%.o,$(KERNEL_C_SRC))
KERNEL_CPP_OBJ = $(patsubst $(SRC_KERNEL)/%.cpp,$(KERNEL_DIR)/%.o,$(KERNEL_CPP_SRC))
KERNEL_ASM_OBJ = $(patsubst $(SRC_KERNEL)/%.asm,$(KERNEL_DIR)/%.o,$(KERNEL_ASM_SRC))

KERNEL_OBJ = $(KERNEL_C_OBJ) $(KERNEL_CPP_OBJ) $(KERNEL_ASM_OBJ)

# ======================================
#  PHONY
# ======================================
.PHONY: all clean run debug hdd

all: $(KERNEL_ELF)

# ======================================
#  Compile-Regeln
# ======================================
$(KERNEL_DIR)/%.o: $(SRC_KERNEL)/%.c
	mkdir -p $(dir $@)
	$(CC64) $(CFLAGS64) -c $< -o $@

$(KERNEL_DIR)/%.o: $(SRC_KERNEL)/%.cpp
	mkdir -p $(dir $@)
	$(CXX64) $(CXXFLAGS64) -c $< -o $@

$(KERNEL_DIR)/%.o: $(SRC_KERNEL)/%.asm
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS64) $< -o $@

# ======================================
#  Linken
# ======================================
$(KERNEL_ELF): $(KERNEL_OBJ) $(LINKER_SCRIPT)
	mkdir -p $(dir $@)
	$(LD64) -T $(LINKER_SCRIPT) -o $@ $(KERNEL_OBJ)

# ======================================
#  FAT32 HDD Image (Limine BIOS + UEFI Datei)
# ======================================
hdd: $(HDD_IMAGE)

$(HDD_IMAGE): $(KERNEL_ELF) $(LIMINE_CONF)
	mkdir -p $(IMG_DIR)
	rm -f $(HDD_IMAGE)

	# 64MiB leeres Image
	dd if=/dev/zero bs=1M count=0 seek=64 of=$(HDD_IMAGE)

	# GPT + 1x ESP ab 1MiB (wie OSDev-Beispiel)
	sgdisk $(HDD_IMAGE) -n 1:2048 -t 1:ef00 -m 1

	# Limine BIOS Stages installieren (MBR + stage2)
	$(LIMINE_BIN) bios-install $(HDD_IMAGE)

	# FAT32 formatieren: -F = FAT32, und :: ist Pflicht bei mtools-image-drive
	mformat -i $(HDD_IMAGE)$(IMG_OFFSET) -F ::

	# Ordner anlegen
	mmd -i $(HDD_IMAGE)$(IMG_OFFSET) ::/EFI ::/EFI/BOOT ::/boot ::/boot/limine

	# Stage3 + Config + Kernel auf die FAT (sonst: "Stage 3 file not found")
	mcopy -i $(HDD_IMAGE)$(IMG_OFFSET) $(KERNEL_ELF) ::/boot/kernel.elf
	mcopy -i $(HDD_IMAGE)$(IMG_OFFSET) $(LIMINE_SHARE)/limine-bios.sys ::/boot/limine
	mcopy -i $(HDD_IMAGE)$(IMG_OFFSET) $(LIMINE_CONF) ::/boot/limine/limine.conf

	# UEFI Loader (optional, aber praktisch)
	mcopy -i $(HDD_IMAGE)$(IMG_OFFSET) $(LIMINE_SHARE)/BOOTX64.EFI ::/EFI/BOOT/BOOTX64.EFI

	# Mini-Sanity-Check: muss jetzt als FAT lesbar sein
	mdir -i $(HDD_IMAGE)$(IMG_OFFSET) ::

# ======================================
#  QEMU
# ======================================
run: hdd
	qemu-system-x86_64 \
	  -cpu max \
	  -monitor stdio \
	  -drive file=$(HDD_IMAGE),format=raw

debug: hdd
	qemu-system-x86_64 \
	  -cpu max \
	  -monitor stdio \
	  -s -S \
	  -drive file=$(HDD_IMAGE),format=raw

# ======================================
#  Aufräumen
# ======================================
clean:
	rm -rf $(BUILD) $(IMG_DIR)





















