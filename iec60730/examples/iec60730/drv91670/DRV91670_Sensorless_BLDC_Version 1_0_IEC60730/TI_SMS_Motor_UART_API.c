/*=============================================================
 * @file:		TI_SMS_Motor_UART_API.h
 *
 * @brief:	Error message definition.
 *
 * @vers:		1.0.0.0	09-2012
 *
 *=============================================================
 *
 * Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/
#include "TI_SMS_Motor_UART_API.h"
#ifndef TI_SMS_MOTOR_USERINPUT_API_H_
#include "TI_SMS_Motor_UserInPut_API.h"
#endif
#include "TI_SMS_Motor_General_Timer_API.h"

// Added for IEC60730 support
#include "TI_DRV_IEC60730.h"
extern uint16_t gInterruptCounter;

extern UART_PARAMS_S uartUCA0;
extern TIMERA_RARAM_S timerA4;

UINT8 gUARTA0RxBufferReady = FALSE;
UINT8 gUARTA0TxBufferReady = FALSE; 
UINT16 gUARTA0RxBufStartPosition = 0; 
UINT16 gUARTA0RxBufEndtPosition = 0; 
UINT8 gStartUARTOperation = FALSE;
UINT8 gSpeedFromMater = 0;

// Enable disable UART Rx interrupt
void TI_SMS_MOTOR_Enable_Disable_UART_Rx_Interrupt (UART_PARAMS_S *p, UINT8 flag)
{
	if (flag)
	{
		p->rxIntEnable = UCRXIE;
		UCA0IE |= UCRXIE;
	}
	else
	{
		p->rxIntEnable = FALSE;
		UCA0IE &= DISABLE_RX_UART_INTERRUPT;
	}
}

UINT32 TI_SMS_MOTOR_Process_Rx_Packet (UART_PARAMS_S *p)
{
    UINT32 errCode = 0;
    volatile UINT8 packetByteCount;
    volatile UINT16 totalCheckSum;
    volatile UINT8 errorRX = FALSE;

    switch (p->rx_receive_packet_state)
    {
    	case UART_RX_STATE_INIT_FIRST_MARKER:
    		if (p->rxBytesCount == 1)
    		{
    		    if (p->rxBuffer[p->rxBytesCount - 1] == FIRST_MARKER_FOR_RX_PACKET)
    		    {
    		    	// we received 1st valid byte form received
    		    	p->rx_receive_packet_state = UART_RX_SECOND_MARKER_RECEIVED;
                    // load timer value for timeout of 100 milliseconds
    		    	// if we don't received packet complete with 100 milliseconds we
    		    	// timeout and reject received data
    		    	TI_SMS_MOTOR_TIMER_Read(&timerA4);
                    p->rxTimeOutCount = timerA4.ISRCount;
    		    	p->rxPacketStarted = TRUE;
    		    }
    		    else
    		    {
    		    	errorRX = TRUE;
    		    }
    		}
    		break;

    	case UART_RX_SECOND_MARKER_RECEIVED:
    		if (p->rxBytesCount == 2)
    		{
    			if (p->rxBuffer[p->rxBytesCount - 1] == SECOND_MARKER_FOR_RX_PACKET)
    				p->rx_receive_packet_state = UART_RX_PACKET_LENGHT_RECEIVED;
    			else
    			{
    				errorRX = TRUE;
    			}
    		}
    		break;

    	case UART_RX_PACKET_LENGHT_RECEIVED:
            if (p->rxBuffer[p->rxBytesCount - 1] == RX_PACKET_MAX_LENGTH)
            {
            	p->rx_receive_packet_state = UART_RX_PROCESS_DATA;
            }
            else
            {
            	errorRX = TRUE;
            }
            break;

    	case UART_RX_PROCESS_DATA:
            if (p->rxBytesCount == (RX_PACKET_MAX_LENGTH - 2))
            {
            	p->rx_receive_packet_state = UART_RX_CHECK_CHECKSUM;
            }
            break;

    	case UART_RX_CHECK_CHECKSUM:
            if (p->rxBytesCount == RX_PACKET_MAX_LENGTH)
            {
            	totalCheckSum = 0;
            	// calculate checksum
                for (packetByteCount = 0; packetByteCount < (RX_PACKET_MAX_LENGTH - 2); packetByteCount++)
                	totalCheckSum = totalCheckSum + p->rxBuffer[packetByteCount];

            	// check packet checksum
                if ((p->rxBuffer[p->rxBytesCount - 2] + p->rxBuffer[p->rxBytesCount - 1] * 256)
                		== totalCheckSum)
                {
                	TI_SMS_MOTOR_Enable_Disable_UART_Rx_Interrupt(p, FALSE);
                	p->rxPacketComplete = TRUE;
                    // disable RX interrupt
            	}
                else
            	{
                	errorRX = TRUE;
            	}
            }
            break;

    	default:
			p->rx_receive_packet_state = UART_RX_STATE_INIT_FIRST_MARKER;
			p->rxBytesCount = 0;
			p->rxPacketStarted = FALSE;
    }

    if (errorRX)
    {
		p->rx_receive_packet_state = UART_RX_STATE_INIT_FIRST_MARKER;
		p->rxBytesCount = 0;
		p->rxPacketStarted = FALSE;
    }

    return errCode;
}
UINT32 TI_SMS_MOTOR_UART_Init(UART_PARAMS_S *p) //called in system init
{ 
    UINT32 errCode = 0;

    gStartUARTOperation = FALSE;
    P3SEL |= 0x60;                          // after masking set P3.5,6 = USCI_A0 TXD/RXD
    UCA0CTL0 = 0x00;
    UCA0CTL1 |= UCSWRST;                    // **Put state machine in reset**
    UCA0CTL1 |= p->uartClk;                 // SMCLK
    UCA0CTL0 |= p->dataBits + p->stopBits + p->parityEnable + p->paritySelect;
    UCA0BR0 = p->baudRateDivLSB;            // 1MHz 115200 (see User's Guide)
    UCA0BR1 = p->baudRateDivMSB;            // 1MHz 115200
    UCA0MCTL = p->modControl;               // Modulation
    UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
    UCA0IE = 0x00;                         
    UCA0IE = p->rxIntEnable + p->txIntEnable;
  __no_operation();                         // For debugger   
    gUARTA0TxBufferReady = FALSE;
    gUARTA0RxBufferReady = FALSE;
    uartUCA0.rxBytesCount = 0;
    gStartUARTOperation = TRUE;
    return (errCode); 
}

//use to send data to UART
UINT32 TI_SMS_MOTOR_UART_Send_Data (UART_PARAMS_S *p)
{
    UINT32 errCode = 0;
    UINT8 bufferSize = 0;
    
    gUARTA0TxBufferReady = FALSE;
    
    for (bufferSize = 0; bufferSize < p->txBytesCount; bufferSize++)
    {    
        UCA0TXBUF = p->txBuffer[bufferSize];
        if (p->txIntEnable)
            while(!gUARTA0TxBufferReady);    // USCI_A0 TX buffer ready?
         DelayCounts(UART_TX_SEND_FLAG_SET_DELAY);
    }
    // set transmission complete flag
    p->txComplete = TRUE;
    return errCode;
}

// 	use to read data from UART buffer
unsigned char uart_get_uart_data (void)
{
	return UCA0RXBUF;
}

UINT8 TI_SMS_MOTOR_UART_CheckCrcOk(UART_PARAMS_S *p)
{
	UINT8 i;
	UINT16 crcsum = 0;

	if(p->rxBytesCount <= 2)
	    return 0;

	for(i = 0; i < p->rxBytesCount - 2; i++)
		crcsum += p->rxBuffer[i];

	if(crcsum%0x100 == p->rxBuffer[p->rxBytesCount - 1] &&
	    crcsum/0x100 == p->rxBuffer[p->rxBytesCount - 2]) 
		return 1;

	return 0;
}

UINT32 TI_SMS_MOTOR_UART_Parse_Data (void)
{
    UINT32 errCode = 0;
       
    if (gUARTA0RxBufferReady)
    {
        if ((uartUCA0.rxBuffer[1] >= 0x30) && 
            (uartUCA0.rxBuffer[1] <= 0x39))
        {
            gSpeedFromMater = uartUCA0.rxBuffer[1] - 0x30;	
        }
        gUARTA0RxBufferReady = FALSE;
    }	
    return errCode;
}

UINT32 TI_SMS_MOTOR_UARRT_Send_Data_To_Master (UART_PARAMS_S *p)
{
    UINT32 errCode = 0;	

    p->txBuffer[SLAVE_ADDRESS_BYTE] = TI_SMS_MOTOR_UserInPut_Get_Motor_Controller_Address();
    p->txBuffer[PWM_FREQ_BYTE_1] = (UINT8)(TI_SMS_MOTOR_UserInPut_Get_Motor_PWM_Frequency() & 0xFF);
    p->txBuffer[PWM_FREQ_BYTE_2] = (UINT8)((TI_SMS_MOTOR_UserInPut_Get_Motor_PWM_Frequency() >> 8 ) & 0xFF);
    p->txBuffer[COMMUTATION_FREQ_BYTE_1] = (UINT8)(TI_SMS_MOTOR_UserInPut_Get_Motor_Commutation_Frequency() & 0xFF);  
    p->txBuffer[COMMUTATION_FREQ_BYTE_2] = (UINT8)((TI_SMS_MOTOR_UserInPut_Get_Motor_Commutation_Frequency() >> 8) & 0xFF);  
    p->txBuffer[CURRENT_BYTE] = TI_SMS_MOTOR_UserInPut_Get_Motor_Current();
    p->txBuffer[LEAD_ANGLE_BYTE] = TI_SMS_MOTOR_UserInPut_Get_Motor_LeadAngle();
    p->txBuffer[TEMP_MEASUREMENT_BYTE] = TI_SMS_MOTOR_UserInPut_Get_Board_Temperature();
    p->txBuffer[BAT_VOLATGE_MEASUREMENT_BYTE] = TI_SMS_MOTOR_UserInPut_Get_Battery_Voltage();
    p->txBuffer[MASTER_PACKET_CRC] = DEFAULT_CRC_VALUE;  
    p->txBytesCount = 10;
    TI_SMS_MOTOR_UART_Send_Data(&uartUCA0);
    return errCode;
}

// init UART packet for start data reception
UINT32 TI_SMS_MOTOR_UARRT_Init_For_Packet (UART_PARAMS_S *p)
{
    UINT32 errCode = 0;

    p->rxPacketComplete = FALSE;
    p->rxPacketStarted = FALSE;
    p->rxBytesCount = 0;
    p->rx_receive_packet_state = UART_RX_STATE_INIT_FIRST_MARKER;

    return errCode;
}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
	// Added for IEC60730 support
	// Check if interrupt was triggered by Interrupt Sweep Function in
	// POST routine.
	if(gInterruptCounter& BITF){
		gInterruptCounter += TEST_USCIA0_INTERRUPT;
		UCA0IE &= ~ UCTXIE;
	}else{
		UINT8 interruptFlag = UCA0IV;

		if (gStartUARTOperation)
		{
			switch(__even_in_range(interruptFlag,4))
			{
				case 0:                             // Vector 0 - no interrupt
					break;
				case 2:                             // Vector 2 - RXIFG
					uartUCA0.rxBuffer[uartUCA0.rxBytesCount++] = UCA0RXBUF;
					TI_SMS_MOTOR_Process_Rx_Packet(&uartUCA0);
					break;
				case 4:                             // Vector 4 - TXIFG
					gUARTA0TxBufferReady = TRUE;
					break;
				default:
					break;
			}
		}
	}
}
