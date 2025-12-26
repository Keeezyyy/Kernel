#include "./ata.h"


static struct ata_device_t ata_devices[4];
//0 -> channe 0 maset 
//1 -> channe 0 slave 
//2 -> channe 1 maset 
//3 -> channe 1 slave 


void ata_init(){
  scan_devices();
}


void set_device_presents(uint8_t channel_num, uint8_t drive_num, bool set_to){
  ata_devices[(channel_num * 2) + drive_num].present = set_to;
}

void scan_devices(){
  const uint16_t DATA_PORT[] = {ATA_PRIMARY_IO, ATA_SECONDARY_IO};
  const uint16_t CONTROL_REGISTER[] = {ATA_PRIMARY_CTRL, ATA_SECONDARY_CTRL};
  const uint8_t REGISTER_VALS[] = {ATA_MASTER, ATA_SLAVE};


  for(int channel_num = 0; channel_num <2; channel_num++){
    for(int drive_num = 0; drive_num <2; drive_num++){
      uint16_t io_base = DATA_PORT[channel_num];
      uint16_t port    = io_base + ATA_REG_DRIVE;

      outb(io_base + ATA_REG_DRIVE, drive_num == 0 ? 0xA0 : 0xB0 | (drive_num << 4)); 

      outb(io_base + ATA_REG_SECCOUNT, 0);
      outb(io_base + ATA_REG_LBA_LO, 0);
      outb(io_base + ATA_REG_LBA_MID, 0);
      outb(io_base + ATA_REG_LBA_HI, 0);

      outb(io_base + ATA_REG_COMMAND, 0xEC);   

      uint8_t status = inb(io_base + ATA_REG_STATUS);

      if(status == 0x0){
        set_device_presents(channel_num, drive_num, false);
        continue;
      }

      bool timeout = true;
      for(int i=0; i < ATA_TIMEOUT; i++){
        if(!(inb(io_base + ATA_REG_STATUS) & ATA_SR_BSY)) {
          timeout = false;
          break;
        }
      }

      if(timeout){
        set_device_presents(channel_num, drive_num, false);
        continue;
      }

      status = inb(io_base + ATA_REG_STATUS);

      if(status & ATA_SR_ERR){
        set_device_presents(channel_num, drive_num, false);
        continue;
      }

      timeout = true;
      for(int i = 0; i < ATA_TIMEOUT; i++){
        status = inb(io_base + ATA_REG_STATUS);

        if(status & ATA_SR_ERR){
          set_device_presents(channel_num, drive_num, false);
          timeout = false;
          break;
        }

        if(status & ATA_SR_DRQ){
          timeout = false;
          break;
        }
      }

      if(timeout){
        set_device_presents(channel_num, drive_num, false);
        continue;
      }


      for (int i = 0; i < 256; i++) {
        uint16_t w = inw(io_base + ATA_REG_DATA);
        (void)w;
      }

      set_device_presents(channel_num, drive_num, true);

      printf("[%s %s] STATUS=0x%b -> %s\n",
             channel_num==0?"PRI":"SEC",
             drive_num==0?"MASTER":"SLAVE",
             status,
             ata_devices[(channel_num*2)+drive_num].present?"VALID":"NONE");

    }
  }
}
