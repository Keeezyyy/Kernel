# ======================================
#  Tools
# ======================================
CC64      = x86_64-elf-gcc
LD64      = x86_64-elf-ld
AS        = nasm
OBJCOPY64 = x86_64-elf-objcopy

CFLAGS64  = -ffreestanding -fno-pie -fno-stack-protector -m64 -O2 -Wall -Wextra -mno-red-zone
ASFLAGS64 = -f elf64

# Limine Pfad automatisch via Homebrew finden
LIMINE_DIR := $(shell brew --prefix limine)/share/limine

# ======================================
#  Verzeichnisse
# ======================================
SRC_KERNEL = src
BUILD      = build
ISO_DIR    = iso
KERNEL_DIR = $(BUILD)/kernel
LINKER_SCRIPT = $(SRC_KERNEL)/linker.ld

KERNEL_ELF = iso_root/boot/kernel.elf
KERNEL_BIN = $(KERNEL_DIR)/kernel.bin
ISO_FILE   = iso/image.iso

# ======================================
#  Dateisuche (mit find)
# ======================================
KERNEL_C_SRC   = $(shell find $(SRC_KERNEL) -type f -name "*.c")
KERNEL_ASM_SRC = $(shell find $(SRC_KERNEL) -type f -name "*.asm")

KERNEL_C_OBJ   = $(patsubst $(SRC_KERNEL)/%.c,$(KERNEL_DIR)/%.o,$(KERNEL_C_SRC))
KERNEL_ASM_OBJ = $(patsubst $(SRC_KERNEL)/%.asm,$(KERNEL_DIR)/%.o,$(KERNEL_ASM_SRC))

KERNEL_OBJ = $(KERNEL_C_OBJ) $(KERNEL_ASM_OBJ)

# ======================================
#  PHONY
# ======================================
.PHONY: all clean run iso run-iso

all: $(KERNEL_ELF)

# ======================================
#  Compile-Regeln
# ======================================
$(KERNEL_DIR)/%.o: $(SRC_KERNEL)/%.c
	mkdir -p $(dir $@)
	$(CC64) $(CFLAGS64) -c $< -o $@

$(KERNEL_DIR)/%.o: $(SRC_KERNEL)/%.asm
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS64) $< -o $@

# ======================================
#  Linken
# ======================================
$(KERNEL_ELF): $(KERNEL_OBJ) $(LINKER_SCRIPT)
	mkdir -p $(KERNEL_DIR)
	$(LD64) -T $(LINKER_SCRIPT) -o $(KERNEL_ELF) $(KERNEL_OBJ)

	xorriso -as mkisofs -R -r -J \
		-b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
		-apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso_root -o iso/image.iso


# ======================================
#  ISO mit Limine erzeugen (macOS-kompatibel)
# ======================================
iso: $(ISO_FILE)
$(ISO_FILE): $(KERNEL_ELF)
	# ISO erzeugen
	xorriso -as mkisofs -R -r -J \
		-b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
		-apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso_root -o iso/image.iso

# ======================================
#  QEMU
# ======================================

run: iso
	qemu-system-x86_64 -cdrom $(ISO_FILE)

# ======================================
#  Aufräumen
# ======================================
clean:
	rm -rf $(BUILD)
	rm -rf $(ISO_DIR)
