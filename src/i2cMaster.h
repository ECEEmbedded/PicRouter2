#ifndef I2C_MASTER_H_
#define I2C_MASTER_H_

#include "maindefs.h"
#include <stdio.h>
#ifndef __XC8
#include <usart.h>
#include <i2c.h>
#include <timers.h>
#else
#include <plib/usart.h>
#include <plib/i2c.h>
#include <plib/timers.h>
#endif

/*
Function: I2CInit
Return:
Arguments:
Description: Initialize I2C in master mode, Sets the required baudrate
*/
void I2CInit(void);

/*
Function: I2CStart
Return:
Arguments:
Description: Send a start condition on I2C Bus
*/
void I2CStart();

/*
Function: I2CStop
Return:
Arguments:
Description: Send a stop condition on I2C Bus
*/
void I2CStop();
/*
Function: I2CRestart
Return:
Arguments:
Description: Sends a repeated start condition on I2C Bus
*/
void I2CRestart();
/*
Function: I2CAck
Return:
Arguments:
Description: Generates acknowledge for a transfer
*/
void I2CAck();
/*
Function: I2CNck
Return:
Arguments:
Description: Generates Not-acknowledge for a transfer
*/
void I2CNak();
/*
Function: I2CWait
Return:
Arguments:
Description: wait for transfer to finish
*/
void I2CWait();

/*
Function: I2CSend
Return:
Arguments: dat - 8-bit data to be sent on bus
           data can be either address/data byte
Description: Send 8-bit data on I2C bus
*/
void I2CSend(unsigned char dat);
/*
Function: I2CRead
Return:    8-bit data read from I2C bus
Arguments:
Description: read 8-bit data from I2C bus
*/
unsigned char I2CRead(void);

//Send full write request
unsigned char I2CReadOneByte(unsigned char id, unsigned char registerAddress);

/*
 * Helper for a full I2C Read request
 */
void I2CReadNBytes(int n, unsigned char *data, unsigned char id, unsigned char registerAddress);

//Read one byte of data
unsigned char I2CWriteOneByte(unsigned char id, unsigned char registerAddress, unsigned char data);

#endif
