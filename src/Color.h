#ifndef __Color_H_
#define __Color_H_

    typedef struct DRIVERColorMEMBERS {
        unsigned char id;
        unsigned char class;
  

    unsigned char a;
  
} DriverColorMembers;
void DriverColorInit(Driver_t *driver) {
DriverColorMembers *self = (DriverColorMembers *)driver;

    
  
}
void DriverColorRespond(Driver_t *driver, unsigned char *rcvData) {
DriverColorMembers *self = (DriverColorMembers *)driver;

  
}
void DriverColorPoll(Driver_t *driver) {
DriverColorMembers *self = (DriverColorMembers *)driver;

    unsigned char data[3];
    I2CReadRequest(self->id, 0x10, data, 3);
    start_UART_send(3, &data[0]);
   
}

    void DriverColorAdd(unsigned char id) {
      Driver_t *context = (Driver_t *)driverMalloc(sizeof(Driver_t));
      context->id = id;
      context->class = 0x20;
      DriverTable[NumberOfDrivers].context = context;
      DriverTable[NumberOfDrivers].respond = DriverColorRespond;
      DriverTable[NumberOfDrivers].poll = DriverColorPoll;
      ++NumberOfDrivers;

      DriverColorInit(context);
    }
  
#endif __Color_H_
