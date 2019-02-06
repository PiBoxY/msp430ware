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
//   MSP430F665x Demo -  Error injection when MID protection is enabled
//
//
//   Description: MSP430F6659 demo enables MID protection for one MID block
//   The device has 512kB of flash, each MID block is 32kB. Note that for
//   device variants with lesser main memory the size of the MID block is scaled
//   proportionaly. Protection for the Info block is turned off using CW1
//   parameter inside MidEnable();
//   In this case 0x8000 to 0xFFFF the memory range that is protected.
//   A parity error is injected in a known location '0xA000' and the MID
//   function to check the memory is called.
//   This function flags the injected error and provides a UNMI.
//   In the UNMI, the code traps execution and blinks the LED only if the
//   returned error address is the same where the error was injected.
//
//   MCLK = default DCO = ~1MHz, ACLK = REFO = ~32kHZ
//
//           MSP430F665x
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |               |
//
//   Priya Thanigai / William Goh
//   Texas Instruments Inc.
//   January 2013
//   Built with IAR Embedded Workbench Version 5.50 & CCS V5.3
//******************************************************************************
#include <msp430.h>

void FlashSegmentFill(unsigned short *, unsigned short);
void SegmentErase(unsigned short*);
unsigned short usHParBefore;
unsigned short usHParAfter;
unsigned short *pErrorLocation;

// The test segment needs to be within the MID enabled space
// In this example between 0xA000 - 0xA1FF
#define MID_TEST_SEG_START  0xA000
#define MID_TEST_SEG_END    0xA1FF

int main(void)
{
    WDTCTL  = WDTPW | WDTHOLD;                // Setup Watchdog
    P1OUT = 0;
    P1DIR |= BIT0;
    SYSBSLC &= ~SYSBSLPE;

    // Fill memory block at 0xA000 with known pattern
    FlashSegmentFill((unsigned short*)MID_TEST_SEG_START,0xBEEF);

    // Clear error interrupt registers
    SYSBERRIV = 0;
    SYSUNIV = 0;

    __disable_interrupt();

    // Enable MID for main memory 0x8000 to 0xFFFF only
    //  CW0 = 1, to enable the first MID block
    //  CW1 = 0, MID is disabled for Info, BSL memory
    MidEnable(0x0001,0x0000);

    // Obtain the parity bit
    usHParBefore = MidGetParity(MID_TEST_SEG_START);

    // ******************
    //  Error Injection!
    // ******************
    // Comment this code out if you do not want to force inject errors.
    // Do not include this code in your final application.
    // It is used to simulate a User NMI and for NMI handling
    // in the rare case that a flash parity error occurs.
    // An error is injected in location 0xA000, the data word at
    // that location is 0xBEEF, the parity is "1" but is re-written as
    // '0' in order to trigger an MID error
    MidSetRaw(0xBEEF, 0, MID_TEST_SEG_START, FWKEY);

    // Obtain the parity bit again after force setting the parity bit
    // without changing the data. You'll observe a missmatch and
    // triggers a UNMI when a memory parity check is performed.
    usHParAfter = MidGetParity(MID_TEST_SEG_START);

    // Once the error is injected, do a memory check of the entire flash
    // segment from 0xA000 to 0xA1FF.
    // Running the check should return a UNMI
    MidCheckMem(MID_TEST_SEG_START, MID_TEST_SEG_END);

    // Code never gets here since it is trapped in the UNMI ISR
    // Set breakpoint here.
    while(1);
}

/*---------------------------------------------------------------------
 Fill the test segment with the value 0xBEEF
---------------------------------------------------------------------*/
void FlashSegmentFill(unsigned short * Flash_ptrD, unsigned short value)
{
    unsigned int counter = 0;
    // Erase Flash segment
    SegmentErase((unsigned short *) MID_TEST_SEG_START);
    FCTL3 = FWKEY;                          // Clear Lock bit
    FCTL1 = FWKEY | WRT;                    // Enable long-word write
    while (counter < 256)
    {
        *Flash_ptrD = value;                // Write to Flash
        Flash_ptrD ++;
        counter++;
    }

    FCTL1 = FWKEY;                          // Clear Erase bit
    FCTL3 = FWKEY | LOCK;                   // Set LOCK bit
}

void SegmentErase(unsigned short *Flash_ptrD)
{
    FCTL3 = FWKEY;                          // Clear Lock bit
    FCTL1 = FWKEY | ERASE;                  // Set Erase bit
    *Flash_ptrD = 0;                        // Dummy write to erase Flash seg
    while (FCTL3 & BUSY);                   // test busy
    FCTL1 = FWKEY;                          // Clear WRT bit
    FCTL3 = FWKEY | LOCK;                   // Set LOCK bit
}

/*---------------------------------------------------------------------*/

// User NMI vector
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = UNMI_VECTOR
__interrupt void unmi_isr(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(UNMI_VECTOR))) unmi_isr (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(SYSUNIV, SYSUNIV_BUSIFG))
    {
    case SYSUNIV_NONE:    break;
    case SYSUNIV_NMIIFG:  break;            // NMIIFG
    case SYSUNIV_OFIFG:   break;            // OFIFG
    case SYSUNIV_ACCVIFG: break;            // ACCVIFG
    case SYSUNIV_BUSIFG:                    // BUSIFG

        // Obtain the flash error location
        pErrorLocation = (unsigned short*) MidGetErrAdr();

        switch(__even_in_range(SYSBERRIV, 0x08))
        {
            case 0x00: break;
            case 0x02: break;
            case 0x04: break;
            case 0x06:                      // MID Handler

                // Disable MID controller
                MidDisable();

                // Check if the error location is at the right place
                // where we first injected the error.
                if (pErrorLocation == (unsigned short *)MID_TEST_SEG_START)
                {
                    // Returned error location is in the same location where
                    // error was injected.
                    while(1)
                    {
                        // Toggle the LED on P1.0
                        P1OUT ^= BIT0;
                        __delay_cycles(100000);
                    }
                }
                else
                {
                    // Should never get here
                    __bis_SR_register(LPM4_bits);
                }
                break;
            case 0x08: break;
            default: break;
        }
        break;
    default: break;
    }
}
