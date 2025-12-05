

void kernel_main()
{
    for(;;);
}


/*

mkdir -p iso_root/boot/limine
mkdir -p iso_root/EFI/BOOT

cp path/to/kernel.elf iso_root/boot/
cp build/limine/limine-bios-cd.bin build/limine/limine-bios.sys build/limine/limine-uefi-cd.bin iso_root/boot/limine/
cp limine/BOOTX64.EFI iso_root/EFI/BOOT/
# falls vorhanden ggf. auch BOOTIA32.EFI, falls du 32-bit support willst

*/