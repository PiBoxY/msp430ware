/*
 * uart_driver.c
 *
 *  Created on: Oct 15, 2014
 *      Author: a0221162
 */

#include "msp430.h"
#include "stdio.h"
#include "string.h"
#include "driverlib.h"
#include "uart_driver.h"

#define SMCLK_FREQUENCY_HZ			(8000000)

s_test test = {
	FALSE,
	"",
	""
};

/*
* \brief uartReceive
*  adds bytes to the rx buffer
*
* @return void
*/
void uartReceive(char data){
	static char rxInProgress = FALSE;
	static char pieceOfString[MAX_STR_LENGTH] = "";           // Holds the new addition to the string
	static char charCnt = 0;

	if( !rxInProgress){
		if ((data != '\n') ){
			pieceOfString[0] = '\0';
			rxInProgress = TRUE;
			pieceOfString[0] = data;
			charCnt = 1;
		}
	}else{ // in progress
		if((data == '\n')){
			rxInProgress = FALSE;
			if (test.newStringReceived == FALSE){ // don't mess with the string while main processes it.
				pieceOfString[charCnt]='\0';
				__no_operation();
				charCnt++;
				strncpy(test.rxString,pieceOfString,charCnt);
				__no_operation();
				test.newStringReceived = TRUE;
				__no_operation();
			}
		}else{
			if (charCnt >= MAX_STR_LENGTH){
				rxInProgress = FALSE;
			}else{
				pieceOfString[charCnt++] = data;
				//charCnt++;
			}
		}
	}
}


/*
* \brief uartInit
*
* @return void
*/
void uartInit()
{
    P2SEL0 &= ~(BIT0 | BIT1);
    P2SEL1 |= BIT0 | BIT1;                  // USCI_A0 UART operation

    UCA0CTLW0 |= 0x0001;
    UCA0CTLW0 &= ~(UCSSEL_3);
    UCA0CTLW0 |= UCSSEL__SMCLK;
    //UCA0BRW = 0x68;

    // Calculate settings for UART registers
	double Num = CS_getSMCLK()/*SMCLK_FREQUENCY_HZ*//BAUDRATE;

	// Different register settings due to UART calculation
	// Use UCBRFO and UC0S16 when (SMCLK / baudrate) >= 16
	if(Num >=16)
	{
		UCA0BRW = (unsigned int)(Num/ 16);
		#if (SMCLK_FREQUENCY_HZ == 1000000)
				UCA0MCTLW = 0xFF81;
		#else
				UCA0MCTLW |= UCOS16 |
							(UCBRF0 *
							((unsigned int)(0.5+(((Num/16)-
							((unsigned int)(Num/16)))*16))));
//				if((SMCLK_FREQUENCY_HZ == 12000000) && (BAUDRATE == 115200)) {
//					UCA0MCTLW &= 0x00FF;
//					UCA0MCTLW |= 0x1100;
//				}
		#endif
			}
	// Use only UCBRS0 when (SMCLK / baudrate) < 16
	else
	{
		UCA0BRW = (unsigned int)(Num);
		UCA0MCTLW |= (UCBRS0 *
					 ((unsigned int)(0.5+(((Num)-
					 ((unsigned int)(Num)))*8))));
	}

    UCA0CTLW0 &= ~(UCSWRST);

    UCA0IE |= UCRXIE;

	test.newStringReceived = FALSE;
}

/* EUSCI A0 UART ISR - Echos data back to PC host */
#pragma vector=EUSCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
//	int receiveByte = UCA0RXBUF;
//	UCA0TXBUF = receiveByte;

    switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
    {
        case USCI_NONE: break;
        case USCI_UART_UCRXIFG:
            RTS_CLEAR;
            char data = UCA0RXBUF;  //RXBUF0;

            //#define ECHO_ON
        #ifdef ECHO_ON
            while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
            TXBUF0 = data;                          // RXBUF0 to TXBUF0
        #endif

            uartReceive(data);
            if(test.newStringReceived == TRUE){
        //        _bic_SR_register_on_exit(LPM4_bits); /* Exit Low Power Mode 4 */
            }
            RTS_SET;            break;
        case USCI_UART_UCTXIFG: break;
        case USCI_UART_UCSTTIFG: break;
        case USCI_UART_UCTXCPTIFG: break;
        default: break;
    }

    __bic_SR_register_on_exit(LPM0_bits);   // Exit active CPU
}


/**
* \brief uartSend
*
* Send out message string
*
* @param char * buf			String of characters to send
* @param unsigned char len	Number of characters to send
*
* @return void
*/
void sendText(){
   unsigned int i;

	for (i = 0; i < MAX_STR_LENGTH; ++i)
    {
	   // wait until UART ready
	   while (!(UCA0IFG & UCTXIFG));             // USCI_A0 TX buffer ready?
		   if (test.txString[i] != 0)
		  {
			   UCA0TXBUF = test.txString[i];
		  }
		  else{
			  break;
		  }
    }
}

bool receiveText(char* data, int maxNumChars){
	bool result = false;
    if(test.newStringReceived == TRUE){
    	result = true;
    	strncpy(data,test.rxString,maxNumChars);
    	test.newStringReceived = FALSE;
    }
    return result;
}
