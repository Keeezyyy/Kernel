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

// I/O Port Offsets for ATA Primary/Secondary Base Ports
#define DATA_REGISTER_IO_PORT_OFFSET_RW            0   // 16-bit R/W
#define ERROR_REGISTER_IO_PORT_OFFSET_R            1   // 8-bit Read
#define FEATURES_REGISTER_IO_PORT_OFFSET_W         1   // 8-bit Write
#define SECTOR_COUNT_REGISTER_IO_PORT_OFFSET_RW    2   // 8-bit R/W
#define SECTOR_NUMBER_REGISTER_IO_PORT_OFFSET_RW   3   // LBAlo
#define CYLINDER_LOW_REGISTER_IO_PORT_OFFSET_RW    4   // LBAmid
#define CYLINDER_HIGH_REGISTER_IO_PORT_OFFSET_RW   5   // LBAhi
#define DRIVE_HEAD_REGISTER_IO_PORT_OFFSET         6   // R/W
#define STATUS_REGISTER_IO_PORT_OFFSET_R           7   // Status Read
#define COMMAND_REGISTER_IO_PORT_OFFSET_W          7   // Command Write

//command register values
#define ATA_CMD_IDENTIFY                0xEC    
#define ATA_CMD_READ_SECTOR             0x20    // LBA28 Read
#define ATA_CMD_READ_SECTOR_EXT         0x24    // LBA48 Read
#define ATA_CMD_WRITE_SECTOR            0x30    // LBA28 Write
#define ATA_CMD_WRITE_SECTOR_EXT        0x34    // LBA48 Write
#define ATA_CMD_CACHE_FLUSH             0xE7    // Flush Cache (28-bit devices)
#define ATA_CMD_CACHE_FLUSH_EXT         0xEA    // Flush Cache (48-bit devices)
#define ATA_CMD_IDENTIFY_PACKET         0xA1    // Identify ATAPI
#define ATA_CMD_PACKET                  0xA0    // Packet command for CD/DVD
                                                //

//Status register values
#define ATA_SR_ERR   0x01    // Error
#define ATA_SR_DRQ   0x08    // Data ready to read
#define ATA_SR_DF    0x20    // Device fault
#define ATA_SR_DRDY  0x40    // Device ready
#define ATA_SR_BSY   0x80    // Busy




struct ata_device_t{
  bool present;
  bool atapi;
  bool is_master;
  uint16_t io_base_register_port;
  uint16_t device_control_register;
};


void ata_init();
void scan_devices();
bool read_disc_sector(struct ata_device_t *device ,uint64_t LBA, uint16_t* buffer);
struct ata_device_t *get_device(int idx);
bool flush_disk(struct ata_device_t *device);

