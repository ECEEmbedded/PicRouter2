#ifndef __IR_H_
#define __IR_H_

    typedef struct DRIVERIRMEMBERS {
        unsigned char id;
        unsigned char class;
  

    unsigned char a;
  
} DriverIRMembers;
void DriverIRInit(Driver_t *driver) {
DriverIRMembers *self = (DriverIRMembers *)driver;

    self->a = 0xE;
    DebugPrint(self->a);
  
}
void DriverIRRespond(Driver_t *driver, unsigned char *rcvData) {
DriverIRMembers *self = (DriverIRMembers *)driver;

  
}
void DriverIRPoll(Driver_t *driver) {
DriverIRMembers *self = (DriverIRMembers *)driver;

    //DebugPrint(self->id);
    LATAbits.LATA1 = !LATAbits.LATA1;
  
}

    void DriverIRAdd(unsigned char id) {
      Driver_t *context = (Driver_t *)driverMalloc(sizeof(Driver_t));
      context->id = id;
      context->class = 0x3A;
      DriverTable[NumberOfDrivers].context = context;
      DriverTable[NumberOfDrivers].respond = DriverIRRespond;
      DriverTable[NumberOfDrivers].poll = DriverIRPoll;
      ++NumberOfDrivers;

      DriverIRInit(context);
    }
  
#endif __IR_H_
