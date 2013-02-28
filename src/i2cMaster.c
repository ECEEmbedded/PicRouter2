#include "i2cMaster.h"
#include "messages.h"
#include "my_i2c.h"

enum {
    I2C_FREE,
    I2C_SENDING,
    I2C_REQUESTING,
    I2C_RECEIVING,
    I2C_ACKNOWLEDGE
};

typedef struct __i2c_master_comm {
	unsigned char buffer[MSGLEN];
	unsigned char	buflen;
        unsigned char   buffind;
	unsigned char event_count;
	unsigned char status;
	unsigned char error_code;
	unsigned char error_count;
} i2c_master_comm;

static i2c_master_comm i2c_p;

// Sending in I2C Master mode [slave write]
// 		returns -1 if the i2c bus is busy
// 		return 0 otherwise
// Will start the sending of an i2c message -- interrupt handler will take care of
//   completing the message send.  When the i2c message is sent (or the send has failed)
//   the interrupt handler will send an internal_message of type MSGT_MASTER_SEND_COMPLETE if
//   the send was successful and an internal_message of type MSGT_MASTER_SEND_FAILED if the
//   send failed (e.g., if the slave did not acknowledge).  Both of these internal_messages
//   will have a length of 0.
// The subroutine must copy the msg to be sent from the "msg" parameter below into
//   the structure to which ic_ptr points [there is already a suitable buffer there].

unsigned char i2c_master_send(unsigned char adr, unsigned char length, unsigned char *msg) {
    if (i2c_p.status != I2C_FREE)
        return -1;

    for (i2c_p.buflen = 0; i2c_p.buflen < length; ++i2c_p.buflen) {
        i2c_p.buffer[i2c_p.buflen] = msg[i2c_p.buflen];
    }

    i2c_p.buffind = 0;
    SEN = 1;
    SSPIF = 1;
    SSPBUF = (adr << 1);
    i2c_p.status = I2C_SENDING;
    return(0);
}

// Receiving in I2C Master mode [slave read]
// 		returns -1 if the i2c bus is busy
// 		return 0 otherwise
// Will start the receiving of an i2c message -- interrupt handler will take care of
//   completing the i2c message receive.  When the receive is complete (or has failed)
//   the interrupt handler will send an internal_message of type MSGT_MASTER_RECV_COMPLETE if
//   the receive was successful and an internal_message of type MSGT_MASTER_RECV_FAILED if the
//   receive failed (e.g., if the slave did not acknowledge).  In the failure case
//   the internal_message will be of length 0.  In the successful case, the
//   internal_message will contain the message that was received [where the length
//   is determined by the parameter passed to i2c_master_recv()].
// The interrupt handler will be responsible for copying the message received into

unsigned char i2c_master_recv(unsigned char ID, unsigned char adr, unsigned char length) {
    if (i2c_p.status != I2C_FREE)
        return -1;
        
    i2c_p.buffind = 0;
    i2c_p.buflen = length;

    i2c_p.buffer[0] = (ID << 1)  | 0x01;
    
    i2c_p.status = I2C_REQUESTING;
    SEN = 1;

    return(0);
}

void i2c_master_int_handler() {
    switch (i2c_p.status) {
        case I2C_FREE: {
            // Should never get an interrupt in this case
            break;
        }
        case I2C_SENDING: {
            if (i2c_p.buffind < i2c_p.buflen/* && !SSPCON2bits.ACKSTAT*/) {
                    SSPBUF = i2c_p.buffer[i2c_p.buffind++];
                } else {    // we have nothing left to send
                    i2c_p.status = I2C_FREE;
                    PEN = 1;
                }
            break;
        }
        case I2C_REQUESTING: {
            SSPBUF = i2c_p.buffer[0];
            i2c_p.status = I2C_RECEIVING;
            break;
        }
        case I2C_RECEIVING: {
            if (i2c_p.buffind < i2c_p.buflen) {
                i2c_p.status = I2C_ACKNOWLEDGE;
                RCEN = 1;
                i2c_p.buffer[i2c_p.buffind++] = SSPBUF;
            } else {    // we have nothing left to send
                i2c_p.status = I2C_FREE;
                PEN = 1;

                ToMainHigh_sendmsg(i2c_p.buflen, MSGT_I2C_DATA, i2c_p.buffer);
            }
            break;
        }
        case I2C_ACKNOWLEDGE: {
            ACKEN = 1;
            if (i2c_p.buffind < i2c_p.buflen) {
                ACKDT = 0;
            } else {    // we have nothing left to send
                ACKDT = 1;
            }
            i2c_p.status = I2C_RECEIVING;
            break;
        }
    }
}

/*
Function: I2CInit
Return:
Arguments:
Description: Initialize I2C in master mode, Sets the required baudrate
*/
void I2CInit(void){
    TRISC3 = 1;      /* SDA and SCL as input pin */
    TRISC4 = 1;      /* these pins can be configured either i/p or o/p */
    SSPSTAT |= 0x80; /* Slew rate disabled */
    SSPCON1 = 0x28;   /* SSPEN = 1, I2C Master mode, clock = FOSC/(4 * (SSPADD + 1)) */
    SSPADD = 0x28;    /* 100Khz @ 4Mhz Fosc */
    
    i2c_p.status = I2C_FREE;
}

// Old functions (for not interrupt driven)
///*
//Function: I2CStart
//Return:
//Arguments:
//Description: Send a start condition on I2C Bus
//*/
//void I2CStart(){
//    SEN = 1;         /* Start condition enabled */
//    while(SEN);      /* automatically cleared by hardware */
//                     /* wait for start condition to finish */
//}
//
///*
//Function: I2CStop
//Return:
//Arguments:
//Description: Send a stop condition on I2C Bus
//*/
//void I2CStop(){
//    PEN = 1;         /* Stop condition enabled */
//    while(PEN);      /* Wait for stop condition to finish */
//                    /* PEN automatically cleared by hardware */
//}
//
///*
//Function: I2CRestart
//Return:
//Arguments:
//Description: Sends a repeated start condition on I2C Bus
//*/
//void I2CRestart(){
//    RSEN = 1;        /* Repeated start enabled */
//    while(RSEN);     /* wait for condition to finish */
//}
//
///*
//Function: I2CAck
//Return:
//Arguments:
//Description: Generates acknowledge for a transfer
//*/
//void I2CAck(){
//    ACKDT = 0;       /* Acknowledge data bit, 0 = ACK */
//    ACKEN = 1;       /* Ack data enabled */
//    while(ACKEN);    /* wait for ack data to send on bus */
//}
//
///*
//Function: I2CNck
//Return:
//Arguments:
//Description: Generates Not-acknowledge for a transfer
//*/
//void I2CNak(){
//    ACKDT = 1;       /* Acknowledge data bit, 1 = NAK */
//    ACKEN = 1;       /* Ack data enabled */
//    while(ACKEN);    /* wait for ack data to send on bus */
//}
//
///*
//Function: I2CWait
//Return:
//Arguments:
//Description: wait for transfer to finish
//*/
//void I2CWait(){
//    while ( ( SSPCON2 & 0x1F ) || ( SSPSTAT & 0x04 ) );
//    /* wait for any pending transfer */
//}
//
///*
//Function: I2CSend
//Return:
//Arguments: dat - 8-bit data to be sent on bus
//           data can be either address/data byte
//Description: Send 8-bit data on I2C bus
//*/
//void I2CSend(unsigned char dat){
//    SSPBUF = dat;    /* Move data to SSPBUF */
//    SSPIF=0;
//    while(BF);       /* wait till complete data is sent from buffer */
//    I2CWait();       /* wait for any pending transfer */
//}
//
///*
//Function: I2CRead
//Return:    8-bit data read from I2C bus
//Arguments:
//Description: read 8-bit data from I2C bus
//*/
//unsigned char I2CRead(void){
//    unsigned char temp;
///* Reception works if transfer is initiated in read mode */
//    RCEN = 1;        /* Enable data reception */
////        while(!BF);      /* wait for buffer full */
//
//
////        for (int i = 0; i < 100; ++i);  //nop
//    temp = SSPBUF;   /* Read serial buffer and store in temp register */
//    SSPIF=0;
//    I2CWait();       /* wait to check any pending transfer */
//    return temp;     /* Return the read data from bus */
//}
//
////Send full write request
//unsigned char I2CReadOneByte(unsigned char id, unsigned char registerAddress) {
//    I2CStart();
//    I2CSend(id << 1);
//    I2CSend(registerAddress);
//   // I2CStop();
//    I2CStart();
//    I2CSend(id << 1 | 1);
//    unsigned char out = I2CRead();
//    I2CNak();
//    I2CStop();
//
//    return out;
//}
//
//void I2CReadRequest(unsigned char id, unsigned char registerAddress, unsigned char *data, int N) {
//
//    I2CStart();
//    I2CSend(id << 1);
//
//    I2CSend(registerAddress);
//
//    I2CRestart();
//
//    I2CSend(id << 1 | 1);
//
//
//    for (int i = 0; i < N; ++i) {
//        data[i] = I2CRead();
//
//        if (i == N - 1)
//            I2CNak();
//        else
//            I2CAck();
//    }
//
//    I2CStop();
//
//
//}
//
//void I2CWriteRequest(unsigned char id, unsigned char registerAddress, unsigned char *data, int N) {
//    I2CStart();
//    I2CSend(id << 1);
//
//    I2CSend(registerAddress);
//
//    for (int i = 0; i < N; ++i)
//        I2CSend(data[i]);
//
//    I2CStop();
//}
