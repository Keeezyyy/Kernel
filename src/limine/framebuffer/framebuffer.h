
#pragma once

#include <stdint.h>
#define LIMINE_FRAMEBUFFER_RGB 1

typedef struct
{
    void *address;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
    uint16_t bpp; 
    uint8_t memory_model;
    uint8_t red_mask_size;
    uint8_t red_mask_shift;
    uint8_t green_mask_size;
    uint8_t green_mask_shift;
    uint8_t blue_mask_size;
    uint8_t blue_mask_shift;
    uint8_t unused[7];
    uint64_t edid_size;
    void *edid;

    uint64_t mode_count;
    struct limine_video_mode **modes;
} limine_framebuffer;

typedef struct
{
    uint64_t pitch;
    uint64_t width;
    uint64_t height;
    uint16_t bpp;
    uint8_t memory_model;
    uint8_t red_mask_size;
    uint8_t red_mask_shift;
    uint8_t green_mask_size;
    uint8_t green_mask_shift;
    uint8_t blue_mask_size;
    uint8_t blue_mask_shift;
} limine_video_mod;