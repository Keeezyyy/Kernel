#include "./fat32.h"
#include "../../x86-64/stdio/stdio.h"
#include <stdint.h>

int test_if_disk_is_fat32(const uint8_t const *buffer) {
  constexpr char sign[] = {0x46, 0x41, 0x54, 0x33, 0x32, 0x20, 0x20, 0x20};

  for (int i = 0x52; i < 0x5A; i++) {
    if (buffer[i] != sign[i - 0x52])
      return -1;
  }

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
  fat32_disc->ata_device = device;
  fat32_disc->bpb_lba_start = BPB_LBA_OFFSET;

  return fat32_disc;
}

uint64_t get_fat_start(struct FAT32_DISC *device) {
  return device->bpb_lba_start +
         ((device->bpb->BPB_RsvdSecCnt * device->bpb->BPB_BytsPerSec) /
          ATA_BYTES_PER_SECTOR);
}

void *get_fat_entry(struct FAT32_DISC *device, const char const *dir_name) {
  uint64_t fat_start = get_fat_start(device);
  printf("fat start : %p\n", fat_start);
  printf("bpb starbpb start%p\n", device->bpb_lba_start);
}

int change_directory(struct FAT32_DISC *device, const char const *dir_name) {
  printf("dir name : %s\n", dir_name);

  get_fat_entry(device, dir_name);
  return 1;
}

// TODO: fix it allocates one byte to much
int go_to_path(const char *path, char **file_name, struct FAT32_DISC *device) {
  const char *cur_char = path;

  int count_since_last_name = 0;
  for (int k = 0; k < 40; k++) {
    // printf("num : %p, char : %c\n", k, path[k]);
    if (*cur_char == '/' || *cur_char == '\0') {

      if (count_since_last_name == 0) {
        count_since_last_name++;
        cur_char++;
        continue;
      }

      void *buffer;

      if (!k_malloc(count_since_last_name + 1, &buffer)) {
        printf("Error malloc for paht node\n");
        return 0;
      }
      char *src = cur_char;
      memcpy(buffer, src - count_since_last_name + 1, count_since_last_name);

      ((uint8_t *)buffer)[count_since_last_name - 1] = '\0';
      if (*cur_char == '\0') {
        *file_name = (char *)buffer;
        return 1;
      }

      if (!change_directory(device, buffer)) {
        printf("Error changing driectory to : %s", buffer);
        k_free(buffer);
        return 0;
      }

      if (!k_free(buffer)) {
        printf("Error freeing in go_to_path\n");
        return 0;
      }
      count_since_last_name = 0;
    }

    count_since_last_name++;
    cur_char++;
  }
  return 0;
}

// TODO:
// Look at max posix nodes
int deallocate_path_node(struct PATH_NODE *node) {
  uint64_t buffer[256] = {0};
  struct PATH_NODE *curr_node = node;

  for (int k = 0; k < 256; k++) {
    buffer[k++] = (uint64_t)curr_node->dir;
    buffer[k] = (uint64_t)curr_node;

    curr_node = curr_node->next;
  }

  for (int k = 0; k < 256; k++) {
    if (buffer[k] != 0) {

      if (!k_free((void *)buffer[k])) {
        printf("Error freeing : %p\n", buffer[k]);
        return 0;
      }
    }
  }
  return 0;
}

void *get_file(const char *const path, struct FAT32_DISC *device, int *err,
               const void *opt_current_working_directory) {

  if (path[0] == '/' || path[0] == '.') {
    // absloute path
    struct PATH_NODE *path_linked_list;

    char *file_name;
    if (!go_to_path(path, &file_name, device)) {
      *err = -1;
      return 0x0;
    }

    printf("file name : %s\n", file_name);

    if (!k_free(file_name)) {
      printf("Error freeing in go_to_path\n");
      return 0;
    }

  } else {
    // relative path
    kernel_panic("Relative path finding not implemented");
  }
}
