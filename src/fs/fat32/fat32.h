#pragma once
#include "../../drivers/storage/ata.h"
#include "../../utils/containers.h"
#include "../../x86-64/memory/malloc.h"
#include "../../x86-64/stdio/stdio.h"
#include <stdint.h>

#define BPB_LBA_OFFSET 0x800
#define ATA_BYTES_PER_SECTOR 512

enum FILE_ACCESS {
  READ,
  WRITE,
  READ_WRITE,
};

struct __attribute__((packed)) FAT_32_BPB {
  uint8_t BS_jmpBoot[3];
  uint8_t BS_OEMName[8];

  uint16_t BPB_BytsPerSec;
  uint8_t BPB_SecPerClus;
  uint16_t BPB_RsvdSecCnt;
  uint8_t BPB_NumFATs;
  uint16_t BPB_RootEntCnt;
  uint16_t BPB_TotSec16;
  uint8_t BPB_Media;
  uint16_t BPB_FATSz16;
  uint16_t BPB_SecPerTrk;
  uint16_t BPB_NumHeads;
  uint32_t BPB_HiddSec;
  uint32_t BPB_TotSec32;

  uint32_t BPB_FATSz32;
  uint16_t BPB_ExtFlags;
  uint16_t BPB_FSVer;
  uint32_t BPB_RootClus;
  uint16_t BPB_FSInfo;
  uint16_t BPB_BkBootSec;
  uint8_t BPB_Reserved[12];

  uint8_t BS_DrvNum;
  uint8_t BS_Reserved1;
  uint8_t BS_BootSig;
  uint32_t BS_VolID;
  uint8_t BS_VolLab[11];
  uint8_t BS_FilSysType[8];
};

struct __attribute__((packed)) FAT32_DISC {
  uint64_t bpb_lba_start;
  struct FAT_32_BPB *bpb;
  struct ata_device_t *ata_device;
};

struct PATH_NODE {
  struct PATH_NODE *next;
  const char *dir;
};

void *get_file(const char *const path, struct FAT32_DISC *device, int *err,
               const void *opt_current_working_directory);
struct FAT32_DISC *init_fat32_disc(struct ata_device_t *device, int *err);

int go_to_path(const char *path, char **file_name, struct FAT32_DISC *device);
int change_directory(struct FAT32_DISC *device, const char const *dir_name);
