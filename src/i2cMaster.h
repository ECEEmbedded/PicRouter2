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

//Setup I2C
void I2CInit(void);

void I2CReadRequest(unsigned char id, unsigned char registerAddress, unsigned char *data, int N);

void I2CWriteRequest(unsigned char id, unsigned char registerAddress, unsigned char *data, int N);

#endif
