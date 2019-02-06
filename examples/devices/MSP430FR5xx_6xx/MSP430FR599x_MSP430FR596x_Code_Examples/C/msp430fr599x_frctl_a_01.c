/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2015, Texas Instruments Incorporated
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
//   MSP430FR5x9x Demo - Entire FRAM Write Protection Enabled
//
//   Description: Protect the entire FRAM memory from write access. Any MPU
//   configuration would be ignored. It triggers an interrupt SYSNMI when a
//   a forced write to FRAM memory at ptr occurs. It then blinks the LED in the
//   interrupt service routine and clears the WPIFG flag.
//
//   MCLK = SMCLK = default DCO
//
//           MSP430FR5994
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |               |
//        |          P1.0 |---> LED
//
//   William Goh
//   Texas Instruments Inc.
//   October 2015
//   Built with IAR Embedded Workbench V6.30 & Code Composer Studio V6.1
//******************************************************************************
#include <msp430.h>

unsigned int data;

#if defined (__TI_COMPILER_VERSION__)
#pragma PERSISTENT(ptr)
unsigned int ptr = 0;
#elif defined (__IAR_SYSTEMS_ICC__)
__persistent unsigned int ptr = 0;
#elif defined(__GNUC__)
unsigned int __attribute__((persistent)) ptr = 0;
#else
#error Compiler not supported!
#endif


#if defined(__TI_COMPILER_VERSION__)
int _system_pre_init(void)
#elif defined(__IAR_SYSTEMS_ICC__)
int __low_level_init(void)
#elif defined(__GNUC__)
int __attribute__((naked, section(".crt_0000init"), used)) _system_pre_init(void)
#endif
{
    /* Insert your low-level initializations here */

    /* Disable Watchdog timer to prevent reset during */
    /* long variable initialization sequences. */
    WDTCTL = WDTPW | WDTHOLD;

    // Enable global FRAM memory protection as early as possible
    FRCTL0 = FRCTLPW | WPROT;

    /*==================================*/
    /* Choose if segment initialization */
    /* should be done or not.           */
    /* Return: 0 to omit initialization */
    /* 1 to run initialization          */
    /*==================================*/
    return 1;
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Configure GPIO
    P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
    P1DIR |= BIT0;                          // Set P1.0 to output direction

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Interrupt enable for global FRAM memory protection
    GCCTL0 |= WPIE;

    // Enable global interrupt
    __enable_interrupt();

    // Initialize dummy data
    data = 0xABCD;

    while(1)
    {
        // The entire FRAM memory should be protected by now. Any
        // writes to FRAM would be ignored. This would trigger a SYSNMI
        // interrupt
        ptr = data;

        __delay_cycles(100000);
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=SYSNMI_VECTOR
__interrupt void SYSNMI_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(SYSNMI_VECTOR))) SYSNMI_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(SYSSNIV, SYSSNIV__LEACMD))
    {
        case SYSSNIV__NONE       : break;   // No interrupt pending
        case SYSSNIV__UBDIFG     : break;   // Uncorrectable FRAM bit error detection
        case SYSSNIV__ACCTEIFG   : break;   // FRAM Access Time Error
        case SYSSNIV__MPUSEGPIFG : break;   // MPUSEGPIFG encapsulated IP memory segment violation
        case SYSSNIV__MPUSEGIIFG : break;   // MPUSEGIIFG information memory segment violation
        case SYSSNIV__MPUSEG1IFG : break;   // MPUSEG1IFG segment 1 memory violation
        case SYSSNIV__MPUSEG2IFG : break;   // MPUSEG2IFG segment 2 memory violation
        case SYSSNIV__MPUSEG3IFG : break;   // MPUSEG3IFG segment 3 memory violation
        case SYSSNIV__VMAIFG     : break;   // VMAIFG Vacant memory access
        case SYSSNIV__JMBINIFG   : break;   // JMBINIFG JTAG mailbox input
        case SYSSNIV__JMBOUTIFG  : break;   // JMBOUTIFG JTAG mailbox output
        case SYSSNIV__CBDIFG     : break;   // Correctable FRAM bit error detection
        case SYSSNIV__WPROT      :          // FRAM write protection detection
            P1OUT ^= BIT0;                  // Set BREAKPOINT here and watch ptr 
            break;
        case SYSSNIV__LEATO    : break;   // LEA time-out fault
        case SYSSNIV__LEACMD   : break;   // LEA command fault
        default: break;
    }
}
