/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 * 
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430x09x Demo - Demonstration of calling various API functions in ROM
//
//  Description: This example calls the SWID, SpiReadByte, and ApiCall functions
//               available in the loader code the the MSP430x09x devices. For
//               more information regarding the usage of these APIs please refer
//               to SLAU324 (https://www.ti.com/lit/slau324)
//
//               This example was tested using the MSP-TS430L092 EVM with the
//               M95160R EEPROM on-board.
//
//  WARNING:     The GPIO used for SPI communication in this example are the same
//               used for the JTAG interface. Therefore, the code will not operate
//               properly if run in debug mode.
//
//  Caleb Overbay
//  Texas Instruments Inc.
//  April 2018
//****************************************************************************

#include <msp430.h>
#include <stdint.h>
#include "loader.h"

#define JTAG_SEL_PORT0      P2SEL0  // JTAG port select 0
#define JTAG_SEL_PORT1      P2SEL1  // JTAG port select 1

// SPI pin definitions
#define SPI_PORT_OUT        P2OUT
#define SPI_PORT_DIR        P2DIR
#define SPI_MOSI_PIN        BIT1
#define SPI_MISO_PIN        BIT3
#define SPI_CS_PIN          BIT0
#define SPI_CLK_PIN         BIT2

unsigned long swidVal = 0;       // Stores the value returned from SWID
unsigned char readByteVal = 0;   // Stores the value returned from SpiReadByte

// Struct defined in loader.h for ApiCall's to SWID
swid_api_args SWID_args = {.SWIDNum = 0};

//Struct defined in loader.h for ApiCall's to SpiReadByte
spireadbyte_api_args SpiReadByte_args = {.SpiReadByteNum = 7,
                                         .SpiAdrLow = 0x0000,
                                         .SpiAdrHigh = 0x0000};

void initAPI(void);              // Initializes peripherals necessary for API
void TimerA0_2_PWM_On(void);     // User defined function for LedOn API
void TimerA0_2_PWM_Off(void);    // User defined function for LedOff API

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;     // Stop watchdog timer

  // Setup P1.0 and P1.1 for debugging
  P1OUT &= ~(BIT0 | BIT1);     // P1.0 and P1.1 low
  P1DIR |= BIT0 | BIT1;        // Set P1.1 and P1.0 to output

  initAPI();                   // Initializes peripherals necessary for API

  SYSCNF &= ~(RAMLCK1 | RAMLCK0);   // Unlock API software registers

  // Example using the ApiCall to call SWID
  ApiCall(&SWID_args);
  if(SWID_args.IdLow != 0x2843)
  {
      // ApiCall to SWID failed, set P1.0 high
      P1OUT |= BIT0;
  }

  // Example using SWID
  swidVal = SWID();
  if((swidVal & 0x0000FFFF) != 0x00002843)
  {
      // SWID call failed, set P1.0 high
      P1OUT |= BIT0;
  }

  LedOn();  // Turn on power to the EEPROM

  // Example using the ApiCall to call SpiReadByte
  ApiCall(&SpiReadByte_args);
  if(SpiReadByte_args.SpiRxData != 0x55)
  {
      // ApiCall to SpiReadByte failed, set P1.1 high
      P1OUT |= BIT1;
  }

  // Example using SpiReadByte to read from address 0x0000 of EEPROM
  readByteVal = SpiReadByte(0x0000, 0x0000);
  if(readByteVal != 0x55)
  {
      // SpiReadByte call failed, set P1.1 high
      P1OUT |= BIT1;
  }

  LedOff();  // Turn off power to the EEPROM

  SYSCNF |= (RAMLCK1 | RAMLCK0); // Lock API software registers

  while (1);  // Loop forever
}

void initAPI(void)
{
    CCSCTL0 = CCSKEY;             // Unlock CS registers
    CCSCTL7 = 0x0000;             // Clear XOFFG and HOFFG faults
    SFRIFG1 = 0x0000;             // Clear any pending system interrupts
    CCSCTL4 = 0x0000;             // ACLK = DCO, SMCLK = DCO, MCLK = DCO
    CCSCTL5 = 0x0000;             // ACLK_DIV = 1, SMCLK_DIV = 1, MCLK_DIV =1

    JTAG_SEL_PORT0 = 0x00;        // Disable JTAG
    JTAG_SEL_PORT1 = 0x00;        // Disable JTAG

    SPI_PORT_OUT |= SPI_CS_PIN;                       // Set CS high
    SPI_PORT_OUT &= ~(SPI_MOSI_PIN | SPI_CLK_PIN);    // Set MOSI and clock low

    // Set CS, MOSI, and CLK to output, MISO to input
    SPI_PORT_DIR = (SPI_CS_PIN | SPI_MOSI_PIN | SPI_CLK_PIN);

    SYSCNF &= ~(RAMLCK1 | RAMLCK0);   // Unlock API software registers

    Status_Reg = 0x0000;              // status REG
    CurOvlSpiH = 0x0000;              // current Overlay SPIadr high word
    CurOvlSpiL = 0x0002;              // current Overlay SPIadr low word
    CurOvlAdrL = 0x0000;              // current Overlay prog address low
    LedOnPtr = *(int *)&TimerA0_2_PWM_On;     // Led On function pointer
    LedOffPtr = *(int *)&TimerA0_2_PWM_Off;   // Led Off function pointer

    SYSCNF |= (RAMLCK1 | RAMLCK0); // Lock API software registers
}

void TimerA0_2_PWM_On(void)
{
    P1SEL0 |= BIT2;              // P1.2 = TA0.2 output
    P1SEL1 &= ~BIT2;             // P1.2 = TA0.2 output
    P1DIR  |= BIT2;              // Set P1.2 to output

    TA0CCR0 = 4;                  // Period = 4 SMCLK ticks
    TA0CCR2 = 1;                  // Trigger when CCR2 counts to 1
    TA0CCTL2 = OUTMOD_4;          // Toggle mode output
    TA0CTL = TASSEL_2 | MC__UP | TACLR;  // Src = SMCLK, Up mode, clear timer
}

void TimerA0_2_PWM_Off(void)
{
    TA0CTL = 0;      // Stop timer
    P1SEL1 &= ~BIT2; // P1.2 = GPIO
    P1SEL0 &= ~BIT2; // P1.2 = GPIO
    P1OUT &= ~BIT2;  // Set P1.2 output low
}
