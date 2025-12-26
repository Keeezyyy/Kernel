#include "./ata.h"


static struct ata_device_t ata_devices[4];


void ata_init(){
  scan_devices();
}

void scan_devices(){
  const uint16_t DATA_PORT[] = {ATA_PRIMARY_IO, ATA_SECONDARY_IO};
  const uint16_t CONTROL_REGISTER[] = {ATA_PRIMARY_CTRL, ATA_SECONDARY_CTRL};
  const uint8_t REGISTER_VALS[] = {ATA_MASTER, ATA_SLAVE};

  log("\nATA_SCAN_DEVICE", "checking for devices");



  for(int channel_num = 0; channel_num <2; channel_num++){
    for(int drive_num = 0; drive_num <2; drive_num++){

      uint16_t io_base = DATA_PORT[channel_num];
      uint16_t port    = io_base + ATA_REG_DRIVE;

      uint8_t val = REGISTER_VALS[drive_num];

      outb(port, val);
      io_wait();

      bool timeout = true;
      for(int i=0; i < ATA_TIMEOUT; i++){
        if(!(inb(io_base + ATA_REG_STATUS) & ATA_SR_BSY)) {
          timeout = false;
          break;
        }
      }


      printf("0b%B\n",inb(io_base + ATA_REG_STATUS));
    }
  }
}
