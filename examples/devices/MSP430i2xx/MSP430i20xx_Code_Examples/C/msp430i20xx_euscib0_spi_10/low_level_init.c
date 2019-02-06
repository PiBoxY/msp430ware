/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2013, Texas Instruments Incorporated
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
// MSP430i20xx Initialization Routines - low_level_init.c
//
// This function is called by the start-up code before "main" is called, and
// before data segment initialization is performed. The function affects the
// following modules:
//  - JTAG - JTAG is disabled
//  - TLV - A TLV checksum is performed
//  - PMM - The shared reference is calibrated to 1.16V
//  - Clock System - The DCO is calibrated to 16.384MHz
//  - SD24 - The SD24 reference voltage trimming is calibrated
//
// The return value of this function controls if data segment initialization
// should take place. If 0 is returned, it is bypassed.
//
// Any code example or application written for the MSP430i20xx which takes
// advantage of any of the affected modules is suggested to include this
// function to ensure full calibration of the application.
//
// Additionally, this initialization routine is suggested for small, short 
// code examples to ensure the JTAG will be unlocked. The i20xx series of 
// devices requires execution of 64 MCLK cycles before the JTAG can be unlocked.
// If a device does not execute 64 MCLK cycles, it cannot be accessed via JTAG.
//******************************************************************************
#include "msp430.h"

#ifdef __TI_COMPILER_VERSION__
int _system_pre_init(void)
#elif __IAR_SYSTEMS_ICC__
int __low_level_init(void)
#elif __GNUC__
extern int system_pre_init(void) __attribute__((constructor));
int system_pre_init(void)
#else
#error Compiler not supported!
#endif
{
    unsigned long *jtagPwd = (unsigned long *)JTAG_DIS_PWD1;

    /* Feed the watchdog timer */
    WDTCTL = WDTPW | WDTCNTCL;

    /* Check JTAG password locations and disable JTAG if passwords don't match.
     * Else the JTAG will be enabled in the 64th cycle after reset.
     */
    if ((*jtagPwd != 0x00000000) && (*jtagPwd != 0xFFFFFFFF))
    {
        /* Disable JTAG */
        SYSJTAGDIS = JTAGDISKEY;
    }

    /* Calibration section
     * Check for the BORIFG flag in IFG1. Execute calibration if this was a BORIFG.
     * Else skip calibration
     */
    if (IFG1 & BORIFG)
    {
        /* Perform 2's complement checksum on 62 bytes of TLV data */
        unsigned int checksum = 0;
        unsigned char *TLV_address_for_parse = ((unsigned char *)TLV_START);
        unsigned int *TLV_address_for_checksum = ((unsigned int *)TLV_START + 1);

        do
        {
            checksum ^= *TLV_address_for_checksum++;
        } while (TLV_address_for_checksum <= (unsigned int *)TLV_END);

        checksum ^= 0xFFFF;
        checksum++;

        /* If check sum is not correct go to LPM4 */
        if (*((unsigned int *)TLV_START) != checksum)
        {
            /* Enter LPM4 if checksum failed */
            __bis_SR_register(LPM4_bits);
        }

        /* Check sum matched, now set calibration values */

        /* Calibrate REF */
        REFCAL1 = *(TLV_address_for_parse + TLV_CAL_REFCAL1);
        REFCAL0 = *(TLV_address_for_parse + TLV_CAL_REFCAL0);

        /* Calibrate DCO */
        CSIRFCAL = *(TLV_address_for_parse + TLV_CAL_CSIRFCAL);
        CSIRTCAL = *(TLV_address_for_parse + TLV_CAL_CSIRTCAL);
        CSERFCAL = *(TLV_address_for_parse + TLV_CAL_CSERFCAL);
        CSERTCAL = *(TLV_address_for_parse + TLV_CAL_CSERTCAL);

        /* Calibrate SD24 */
        SD24TRIM = *(TLV_address_for_parse + TLV_CAL_SD24TRIM);

        /* Clear BORIFG */
        IFG1 &= ~(BORIFG);
    }

    /* Feed the watchdog timer */
    WDTCTL = WDTPW | WDTCNTCL;

    /* Return value:
     *  1 - Perform data segment initialization.
     *  0 - Skip data segment initialization.
     */
    return 1;
}

