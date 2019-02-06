/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
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
//  MSP430F22x4 Demo - Flash In-System Programming w/ EEI, Copy SegD to A/B/C
//
//  Description: This program first erases flash seg D, then it increments all
//  values in seg D, then it erases segs A,B,C and copies D to those locations
//  The EEI bit is set for the Flash Erase Cycles. This does allow the Timer_A
//  Interrupts to be handled also during the Segment erase time.
//  ACLK = n/a, MCLK = SMCLK = CALxxx_1MHZ = 1MHz
//  //* Set Breakpoint on NOP in the Mainloop to avoid Stressing Flash *//
//
//               MSP430F22x4
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//
//  A. Dannenberg
//  Texas Instruments Inc.
//  April 2006
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.41A
//******************************************************************************
#include <msp430.h>

char  value;                                // 8-bit value to write to seg D

// Function prototypes
void write_SegD(char value);
void copy_D2A(void);
void copy_D2B(void);
void copy_D2C(void);

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  if (CALBC1_1MHZ==0xFF)					// If calibration constant erased
  {											
    while(1);                               // do not load, trap CPU!!	
  }
  DCOCTL = 0;                               // Select lowest DCOx and MODx settings
  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO to 1MHz
  DCOCTL = CALDCO_1MHZ;
  FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator
  P1DIR |= 0x01;                            // P1.0 output
  TACCTL0 = CCIE;                           // TACCR0 interrupt enabled
  TACCR0 = 50000;
  TACTL = TASSEL_2 + MC_1;                  // SMCLK, upmode
  value = 0;                                // Initialize value
  __enable_interrupt();                     // Enable interrupts

  while(1)                                  // Repeat forever
  {
    write_SegD(value++);                    // Write segment D, increment value
    copy_D2A();                             // Copy segment D to A
    copy_D2B();                             // Copy segment D to B
    copy_D2C();                             // Copy segment D to C
    __no_operation();                       // Set breakpoint here
  }
}

void write_SegD(char value)
{
  char *Flash_ptr;                          // Flash pointer
  unsigned int i;

  Flash_ptr = (char *)0x1000;               // Initialize Flash pointer
  FCTL3 = FWKEY;                            // Clear Lock bit
  FCTL1 = FWKEY + ERASE + EEI;              // Set Erase bit, allow interrupts
  *Flash_ptr = 0;                           // Dummy write to erase Flash seg

  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

  for (i = 0; i < 64; i++)
  {
    *Flash_ptr++ = value;                   // Write value to flash
  }

  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}

// Warning: When this function is called, it MUST complete or
// DCO calibration constants will be lost
void copy_D2A(void)
{
  char *Flash_ptrA;                         // Segment A pointer
  char *Flash_ptrD;                         // Segment D pointer
  unsigned int i;
  char CAL_DATA[8];

  Flash_ptrA = (char *)0x10F8;              // Initialize Flash segment A ptr
  for (i = 0; i < 8; i++)
  {
    CAL_DATA[i] = *Flash_ptrA++;            // Save DCO constants
  }

  Flash_ptrA = (char *)0x10C0;              // Initialize Flash segment A ptr
  Flash_ptrD = (char *)0x1000;              // Initialize Flash segment D ptr
  FCTL3 = FWKEY + LOCKA;                    // Clear LOCK & LOCKA bits
  FCTL1 = FWKEY + ERASE + EEI;              // Set Erase bit, allow interrupts
  *Flash_ptrA = 0;                          // Dummy write to erase Flash seg A
  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

  for (i = 0; i < 56; i++)
  {
    *Flash_ptrA++ = *Flash_ptrD++;          // copy value segment D to seg A
  }

  for (i = 0; i < 8; i++)
  {
    *Flash_ptrA++ = CAL_DATA[i];            // re-flash DCO calibration data
  }

  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCKA + LOCK;             // Set LOCK & LOCKA bit
}

void copy_D2B(void)
{
  char *Flash_ptrB;                         // Segment B pointer
  char *Flash_ptrD;                         // Segment D pointer
  unsigned int i;

  Flash_ptrB = (char *)0x1080;              // Initialize Flash segment B ptr
  Flash_ptrD = (char *)0x1000;              // Initialize Flash segment D ptr
  FCTL3 = FWKEY;                            // Clear LOCK bit
  FCTL1 = FWKEY + ERASE + EEI;              // Set Erase bit, allow interrupts
  *Flash_ptrB = 0;                          // Dummy write to erase Flash seg B
  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

  for (i = 0; i < 64; i++)
  {
    *Flash_ptrB++ = *Flash_ptrD++;          // copy value segment D to seg B
  }

  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}

void copy_D2C(void)
{
  char *Flash_ptrC;                         // Segment C pointer
  char *Flash_ptrD;                         // Segment D pointer
  unsigned int i;

  Flash_ptrC = (char *)0x1040;              // Initialize Flash segment C ptr
  Flash_ptrD = (char *)0x1000;              // Initialize Flash segment D ptr
  FCTL3 = FWKEY;                            // Clear LOCK bit
  FCTL1 = FWKEY + ERASE + EEI;              // Set Erase bit, allow interrupts
  *Flash_ptrC = 0;                          // Dummy write to erase Flash seg C
  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

  for (i = 0; i < 64; i++)
  {
    *Flash_ptrC++ = *Flash_ptrD++;          // copy value segment D to seg C
  }

  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}

// Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERA0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
  P1OUT ^= 0x01;                            // Toggle P1.0
}
