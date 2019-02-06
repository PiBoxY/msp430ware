/*
 * uart_driver.h
 *
 *  Created on: Oct 21, 2014
 *      Author: a0221162
 */

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

// Choose baudrate
#define BAUDRATE                (115200)

// Global variables
#define MAX_STR_LENGTH 271

#define FALSE 0
#define TRUE  1

typedef struct{
	unsigned char newStringReceived;
	char          txString[MAX_STR_LENGTH];
	char          rxString[MAX_STR_LENGTH];
}s_test;

extern s_test test;
//extern char rxString[MAX_STR_LENGTH];     // The entire input string from the last 'return'
//extern char txString[MAX_STR_LENGTH];
//extern char partOfString[MAX_STR_LENGTH];
//extern unsigned char newStringReceived;

// Use handshake to secure UART communication
//#define USE_HANDSHAKE
#ifdef USE_HANDSHAKE
    #define RTS_INIT	            {P2DIR |= BIT3;}
    #define RTS_SET   	            {P2OUT |= BIT3;}
    #define RTS_CLEAR               {P2OUT &= ~BIT3;}
    #define CTS_INIT                {P2DIR &= ~BIT4;}
    #define CTS_IS_SET              ((P2IN & BIT4)==BIT4)
#else
    #define RTS_INIT	            { }
    #define RTS_SET   	            { }
    #define RTS_CLEAR               { }
    #define CTS_INIT                { }
    #define CTS_IS_SET              (1)
#endif

void uartReceive(char data);
void uartInit(void);
void uartSend(char * buf, unsigned char len);
void sendText(void);
bool receiveText(char* data, int maxNumChars);

#endif /* UART_DRIVER_H_ */
