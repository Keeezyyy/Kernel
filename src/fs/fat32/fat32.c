#include "./fat32.h"
#include "../../x86-64/stdio/stdio.h"
#include <stdint.h>

int test_if_disk_is_fat32(const uint8_t const *buffer) {
  constexpr char sign[] = {0x46, 0x41, 0x54, 0x33, 0x32, 0x20, 0x20, 0x20};

  printHexDump(&buffer[0x52], 5);
  for (int i = 0x52; i < 0x5A; i++) {
    putc(buffer[i]);
    if (buffer[i] != sign[i - 0x52])
      return -1;
  }
  printf("\n");

  return 0;
}

struct FAT32_DISC *init_fat32_disc(struct ata_device_t *device, int *err) {
  uint8_t buffer[512] = {0};

  if (!read_disc_sector(device, BPB_LBA_OFFSET, (uint16_t *)buffer)) {
    *err = -1;
    printf("error disc reading \n");
    return (void *)0x0;
  }

  if (test_if_disk_is_fat32(buffer) != 0) {
    *err = -1;
    printf("error checking fat32 signature \n");
    return (void *)0x0;
  }

  void *bpb_buffer;
  if (!k_malloc(sizeof(struct FAT_32_BPB), &bpb_buffer)) {
    *err = -1;
    printf("FAT_32 dic init : Error allocating for FAT_32_BPB\n");
    return (void *)0x0;
  }

  memcpy(bpb_buffer, buffer, sizeof(struct FAT_32_BPB));

  struct FAT_32_BPB *bpb_object = (struct FAT_32_BPB *)bpb_buffer;

  struct FAT32_DISC *fat32_disc;
  if (!k_malloc(sizeof(struct FAT32_DISC), (void **)&fat32_disc)) {
    *err = -1;
    printf("FAT_32 dic init : Error allocating  fat32_disc\n");
    return (void *)0x0;
  }

  fat32_disc->bpb = bpb_object;

  return fat32_disc;
}

void *open_file(const char *const path, struct ata_device_t *device, int *err) {

}
