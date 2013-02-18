#ifndef __Motor_H_
#define __Motor_H_

    typedef struct DRIVERMotorMEMBERS {
        unsigned char id;
        unsigned char class;
  

    unsigned char a;
  
} DriverMotorMembers;
void DriverMotorInit(Driver_t *driver) {
DriverMotorMembers *self = (DriverMotorMembers *)driver;

    self->a = 0xE;
    DebugPrint(self->a);
  
}
void DriverMotorRespond(Driver_t *driver, unsigned char *rcvData) {
DriverMotorMembers *self = (DriverMotorMembers *)driver;

  
}
void DriverMotorPoll(Driver_t *driver) {
DriverMotorMembers *self = (DriverMotorMembers *)driver;

    //DebugPrint(self->id);
    LATAbits.LATA2 = !LATAbits.LATA2;
  
}

    void DriverMotorAdd(unsigned char id) {
      Driver_t *context = (Driver_t *)driverMalloc(sizeof(Driver_t));
      context->id = id;
      context->class = 0x4F;
      DriverTable[NumberOfDrivers].context = context;
      DriverTable[NumberOfDrivers].respond = DriverMotorRespond;
      DriverTable[NumberOfDrivers].poll = DriverMotorPoll;
      ++NumberOfDrivers;

      DriverMotorInit(context);
    }
  
#endif __Motor_H_
