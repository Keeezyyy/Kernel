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

      outb(io_base + ATA_REG_DRIVE, drive_num == 0 ? 0xE0 : 0xF0);

      outb(io_base + ATA_REG_SECCOUNT, 0);

      io_wait();

      uint8_t cl = inb(ATA_REG_LBA_MID);
      uint8_t ch = inb(ATA_REG_LBA_HI);

      if(cl == 0x14 && ch == 0xEB)
        ata_devices[(channel_num * 2) + drive_num].atapi = true;


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

      ata_devices[(channel_num*2)+drive_num].io_base_register_port = io_base;
      ata_devices[(channel_num*2)+drive_num].device_control_register = CONTROL_REGISTER[channel_num];
      ata_devices[(channel_num*2)+drive_num].is_master = drive_num == 0;


      flush_disk(&ata_devices[(channel_num*2)+drive_num]);

      printf("[%s %s] STATUS=0x%b -> %s \n",
             channel_num==0?"PRI":"SEC",
             drive_num==0?"MASTER":"SLAVE",
             status,
             ata_devices[(channel_num*2)+drive_num].present?"VALID":"NONE");

    }
  }
}


struct ata_device_t *get_device(int idx){
  return &ata_devices[idx];
}

bool read_disc_sector(struct ata_device_t *device ,uint64_t LBA, uint16_t* buffer){

  if(device->present != true){
    printf("invalid device for reading disc sector\n");
    return false;
  }

  uint8_t LBA1 = (LBA >> 0) & 0xFF;
  uint8_t LBA2 = (LBA >> 8) & 0xFF;
  uint8_t LBA3 = (LBA >> 16) & 0xFF;
  uint8_t LBA4 = (LBA >> 24) & 0xFF;
  uint8_t LBA5 = (LBA >> 32) & 0xFF;
  uint8_t LBA6 = (LBA >> 40) & 0xFF;


  //
  /*
    *
    *Bit	Bedeutung
      7	Immer 1
      6	LBA-Bit (1=LBA, 0=CHS)
      5	Immer 1
      4	Drive Select (DRV) → 0 = Master, 1 = Slave
      0–3	Head-Nummer (CHS) oder LBA-Bits 24-27
    *
    *
    *
    * */
  uint8_t drive_head = 0xE0;
  if (!device->is_master) 
    drive_head |= 0x10;

  drive_head |= (LBA4 & 0x0F);
  outb(device->io_base_register_port + DRIVE_HEAD_REGISTER_IO_PORT_OFFSET,drive_head);

  //setting sector count
  outb(device->io_base_register_port + SECTOR_COUNT_REGISTER_IO_PORT_OFFSET_RW, 0); //sector count high 
  outb(device->io_base_register_port + SECTOR_NUMBER_REGISTER_IO_PORT_OFFSET_RW, LBA4);
  outb(device->io_base_register_port + CYLINDER_LOW_REGISTER_IO_PORT_OFFSET_RW, LBA5);
  outb(device->io_base_register_port + CYLINDER_HIGH_REGISTER_IO_PORT_OFFSET_RW, LBA6);

  outb(device->io_base_register_port + SECTOR_COUNT_REGISTER_IO_PORT_OFFSET_RW, 1); // sector count low
  outb(device->io_base_register_port + SECTOR_NUMBER_REGISTER_IO_PORT_OFFSET_RW, LBA1);
  outb(device->io_base_register_port + CYLINDER_LOW_REGISTER_IO_PORT_OFFSET_RW, LBA2);
  outb(device->io_base_register_port + CYLINDER_HIGH_REGISTER_IO_PORT_OFFSET_RW, LBA3);

  outb(device->io_base_register_port + COMMAND_REGISTER_IO_PORT_OFFSET_W, ATA_CMD_READ_SECTOR_EXT);

  for(int i = 0; i< ATA_TIMEOUT * 10;i++){
    if(!(inb(device->io_base_register_port + STATUS_REGISTER_IO_PORT_OFFSET_R) & ATA_SR_BSY))
      break;
  }

  if(!(inb(device->io_base_register_port + STATUS_REGISTER_IO_PORT_OFFSET_R)& ATA_SR_DRQ))
    return false;

  for(int i = 0; i < 256; i++)
    buffer[i] = inw(device->io_base_register_port + DATA_REGISTER_IO_PORT_OFFSET_RW);



  if(!flush_disk(device))
    return false;

  return true;
}


bool flush_disk(struct ata_device_t *device){
  outb(device->io_base_register_port + COMMAND_REGISTER_IO_PORT_OFFSET_W, ATA_CMD_CACHE_FLUSH_EXT);

  while (inb(device->io_base_register_port + STATUS_REGISTER_IO_PORT_OFFSET_R) & ATA_SR_BSY) {}

  uint8_t status = inb(device->io_base_register_port + STATUS_REGISTER_IO_PORT_OFFSET_R);

  if(status & ATA_SR_ERR){
    printf("error flushen disk\n");
    return false;
  }

  return true;

}
