#ifndef DRIVER_H_
#define DRIVER_H_

    #include "debug.h"
    #include "i2cMaster.h"
    
    unsigned char DriverHeap[10];
    unsigned char *heapPointer = DriverHeap;
    unsigned char *driverMalloc(int size) {
      unsigned char *temp = heapPointer;

      heapPointer += size;

      return temp;
    }

    //Driver dispatch table
    //Id -> Driver Function?
    typedef struct DRIVER_T {
        unsigned char id;  //1 Byte
        unsigned char class; //1 Byte
        unsigned char data[10]; //Do whatever
    } Driver_t;

    typedef struct RRPACKET_T {
        unsigned char id; //1 Byte
        unsigned char class; //1 Byte
        unsigned char data[4]; //Payload
    } RRPacket_t;

    //Device ID Dispatch table (Convert a device ID to it's driver functions)
    typedef struct DRIVER_TABLE_ENT_T {
        unsigned char id;
        Driver_t *context;
        void (*respond)(Driver_t *, unsigned char *);
        void (*poll)(Driver_t *);
    } Driver_Table_Ent_t;

    #define MAX_DRIVERS 10
    Driver_Table_Ent_t DriverTable[MAX_DRIVERS];
    unsigned char NumberOfDrivers = 0;  //Current number of drivers
  #include "Color.h"
#include "IR.h"
#include "Motor.h"
#endif DRIVER_H_
