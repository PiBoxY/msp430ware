/*=============================================================
 * @file:		TI_SMS_Motor_SPI_API.h
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

#ifndef TI_SMS_MOTOR_SPI_API_H_
#define TI_SMS_MOTOR_SPI_API_H_
#include "TI_SMS_Motor_Com_def.h"
#define SPI_DELAY 20//50 //100 // for FRAM-based device  // 12 for FLASH
#define SPI_BUSY_FLAG 0x01

inline void SPI_Init()
{
   // use to init port3 for spi setup
    P4DIR |= BIT3 | BIT4;         // Set SIMO, CLK, SRESET as outputs
    P4DIR &= 0xFB;                // SOMI, slave in master in defined as input P4.5
                                   // Hall sensor comes in on P3 Bit3:1. So it is set as input
    P4SEL = BIT2 | BIT4 | BIT3;    // Non-port driven (driven by USCI block)
    P3DIR |= 0x80;                 // SREST P3.7 
    P3OUT |= 0x80;                 // SRESET P3.7 high. This to clear out the SPI communication and have it ready to go     
    P3OUT &= 0x4F;    // SRESET low. This to clear out the SPI communication and have it ready to go                         

/* USCI Initialization
The USCI is initialized with the following setting
3-pin, 8-bit SPI master,Clock polarity high,MSB
*/
    UCB0CTL1 |= UCSWRST;    // **Put state machine in reset**
    UCB0CTL0 = 0xA9;                                                                                   
    UCB0CTL1 |= UCSSEL_3;    // MCLK
    UCB0BR0 = 0x00;          
    UCB0BR1 = 0;            
    UCB0CTL1 &= ~UCSWRST;   // **Initialize USCI state machine** 
    P3OUT |= BIT7;                 // SRESET P3.7 high. This to clear out the SPI communication and have it ready to go     
    P3OUT &= ~BIT7;    // SRESET low. This to clear out the SPI communication and have it ready to go   
}

inline void SPI_Send(UINT8 data)
{
    while(UCB0STAT & SPI_BUSY_FLAG);    // keep in loop until UCBUSY flag set
	UCB0TXBUF = data;    // Transmit address     
    while(UCB0STAT & SPI_BUSY_FLAG);    // keep in loop until UCBUSY flag set
}

inline void SPI_Write(UINT8 address,UINT8 data)
{
      SPI_Send((address << 1) | BIT0);    // send address
      SPI_Send(data);       // send data
}

inline UINT8 SPI_Read(UINT8 address)
{
    volatile UINT8 data;
    SPI_Send(address << 1);    // send address
    SPI_Send(0x00);    // send zeroes
    data=UCB0RXBUF;
    return(data);
}

/*!
  \fn static inline void drv91670SpiResetAfeSpi()
  \brief Holds the AFE SPI slave in reset
 */
inline void drv91670SpiResetAfeSpi() { P3OUT |= BIT7; }

/*!
  \fn static inline void drv91670SpiReleaseAfeSpi()
  \brief Releases the AFE SPI slave from reset, so that it can begin accepting characters
 */
inline void drv91670SpiReleaseAfeSpi() { P3OUT &= ~BIT7; }

#endif /*TI_SMS_MOTOR_SPI_API_H_*/
