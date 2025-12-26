#pragma once
#include <stdint.h>
#include "../../x86-64/stdio/stdio.h"
#include "../../hardware/hardware.h"

#define bool uint8_t
#define true 1
#define false 0

#define ATA_MASTER 0xE0  
#define ATA_SLAVE  0xF0  

#define ATA_PRIMARY_IO      0x1F0
#define ATA_PRIMARY_CTRL    0x3F6

#define ATA_SECONDARY_IO    0x170
#define ATA_SECONDARY_CTRL  0x376

#define ATA_REG_DRIVE      0x06
#define ATA_REG_STATUS     0x07

#define ATA_TIMEOUT 50000     

#define ATA_REG_DATA        0x00
#define ATA_REG_ERROR       0x01  
#define ATA_REG_FEATURES    0x01  
#define ATA_REG_SECCOUNT    0x02
#define ATA_REG_LBA_LO      0x03
#define ATA_REG_LBA_MID     0x04
#define ATA_REG_LBA_HI      0x05
#define ATA_REG_DRIVE       0x06
#define ATA_REG_STATUS      0x07  
#define ATA_REG_COMMAND     0x07 

#define ATA_SR_ERR   0x01 
#define ATA_SR_DRQ   0x08 
#define ATA_SR_SRV   0x10 
#define ATA_SR_DF    0x20 
#define ATA_SR_RDY   0x40 
#define ATA_SR_BSY   0x80 

struct ata_device_t{
    bool present;
    uint8_t atapi;
} ;


void ata_init();
void scan_devices();
