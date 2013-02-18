#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdio.h>
#include <plib/usart.h>
#include <plib/i2c.h>
#include <plib/timers.h>
#include "maindefs.h"

/*
 * ##################################################
 * DebugPrint
 *  Output lights to first four ports on PORT A
 * ##################################################
 */
void DebugPrint(unsigned char out);

/*
 * ##################################################
 * DebugSignal
 * Just display some lights, good for startup test
 * ##################################################
 */
void DebugSignal();


#endif
