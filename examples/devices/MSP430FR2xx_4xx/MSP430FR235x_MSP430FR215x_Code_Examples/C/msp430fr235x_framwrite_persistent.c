/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2016, Texas Instruments Incorporated
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
//   MSP430FR235x Demo - FRAM write, use #pragma PERSISTENT to initialize variable in FRAM.
//
//   Description: Program use #pragma PERSISTENT(in CCS) or __persistent(in IAR) 
//                to statically-initialize variable Port_event in FRAM.
//                Configure program FRAM write protection offset address to leave
//                1024 bytes memory for storing PERSISTENT variables. Linker command
//                file should be modified to change memory partitioning and specify
//                the location of PERSISTENT variables.
//                Download and run the program. Program automatically enters
//                LPM4.5 and measure the current. Use button S1 (or P1.2) on the
//                board to wake the device up from LPM4.5 and toggle the
//                LED (on P1.0). The interrupt times will be recorded in FRAM
//                and blink the LED (on P1.0) if more than 5 times.                
//
//   ACLK = REFO = 32kHz, MCLK = SMCLK = default DCODIV = ~1MHz.
//
//            MSP430FR2355
//         -----------------
//     /|\|                 |
//      | |                 |
//      | |                 |
//      --|RST              |
//        |                 |
//        |                 |
//        |             P1.0|-->LED
//        |                 |
//        |             P1.2|<--- S1 push-button
//
//   Darren Lu
//   Texas Instruments Inc.
//   Oct. 2016
//   Built with IAR Embedded Workbench v6.50 & Code Composer Studio v6.2  
//******************************************************************************
#include <msp430.h>

// Statically-initialized variable
#ifdef __TI_COMPILER_VERSION__
#pragma PERSISTENT(Port_event)
unsigned long Port_event = 0;
#elif __IAR_SYSTEMS_ICC__
__persistent unsigned long Port_event = 0;
#else
// Port the following variable to an equivalent persistent functionality for the specific compiler being used
unsigned long Port_event = 0;
#endif

void initGpio(void);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Configure GPIO
    initGpio();

    // Determine whether we are coming out of an LPMx.5 or a regular RESET.
    if (SYSRSTIV == SYSRSTIV_LPM5WU)        // MSP430 just woke up from LPMx.5
    {

        // Add the variable Port_event in FRAM to record the button event
        // Write protection starts from the beginning of Program FRAM + 1024 bytes
        // Code start address + 1024 bytes, Linker command file should be modified
        SYSCFG0 = FRWPPW | FRWPOA0 | DFWP | PFWP;// Configure 1024 bytes for FRAM write
        Port_event++;                       // Record the port event in FRAM
        SYSCFG0 = FRWPPW | DFWP | PFWP;     // Program FRAM write protected (not writable)

        do
        {

            P1OUT |= BIT0;                  // P1.0 = toggle
            __delay_cycles(100000);
            P1OUT &= ~BIT0;                 // P1.0 = toggle
            __delay_cycles(100000);

        }while (Port_event >= 5);           // Recorded 5 port interrupts?

    }

    // Device powered up from a cold start.
    // It configures the device and puts the device into LPM4.5

    P1DIR &= ~(BIT2);                       // Configure P1.2 as input direction pin
    P1OUT |= BIT2;                          // Configure P1.2 as pulled-up
    P1REN |= BIT2;                          // P1.2 pull-up register enable
    P1IES |= BIT2;                          // P1.2 Hi/Low edge
    P1IFG = 0;                              // Clear all P1 interrupt flags
    P1IE |= BIT2;                           // P1.2 interrupt enabled

    // Explicitly clear RTC control registers to disable it
    // just incase if the RTC was previously enabled
    RTCCTL = 0;

    PMMCTL0_H = PMMPW_H;                    // Open PMM Registers for write
    PMMCTL0_L |= PMMREGOFF;                 // and set PMMREGOFF
    PMMCTL0_H = 0;                          // Lock PMM Registers

    // Enter LPM4 Note that this operation does not return. The LPM4.5
    // will exit through a RESET event, resulting in a re-start
    // of the code.
    __bis_SR_register(LPM4_bits | GIE);
}

void initGpio()
{
    P1DIR = 0xFF; P2DIR = 0xFF; P3DIR = 0xFF; P4DIR = 0xFF; P5DIR = 0xFF; P6DIR = 0xFF;
    P1REN = 0xFF; P2REN = 0xFF; P3REN = 0xFF; P4REN = 0xFF; P5REN = 0xFF; P6REN = 0xFF;
    P1OUT = 0x00; P2OUT = 0x00; P3OUT = 0x00; P4OUT = 0x00; P5OUT = 0x00; P6OUT = 0x00;

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;
}
