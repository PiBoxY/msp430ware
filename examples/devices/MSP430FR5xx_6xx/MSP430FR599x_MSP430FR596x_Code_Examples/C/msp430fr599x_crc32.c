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
//   MSP430FR5x9x Demo - CRC32, Compare CRC32 output with software-based
//                       algorithm
//
//   Description: An array of 16 random 16-bit values are moved through the
//   CRC32 module, as well as a software-based CRC32-ISO3309 algorithm. Due to
//   the fact that the software-based algorithm handles 8-bit inputs only,
//   the 16-bit words are broken into 2 8-bit words before being run through
//   (lower byte first). The outputs of both methods are then compared to ensure
//   that the operation of the CRC module is consistent with the expected
//   outcome. If the values of each output are equal, set P1.0, else reset.
//
//   MCLK = SMCLK = default DCO~1MHz
//
//                MSP430FR5994
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |             P1.0|--> LED
//
//   William Goh
//   Texas Instruments Inc.
//   April 2016
//   Built with IAR Embedded Workbench V6.40 & Code Composer Studio V6.1
//******************************************************************************
#include <msp430.h>

#define  POLYNOMIAL_32    0xEDB88320

// Holds a crc32 lookup table
unsigned long crc32Table[256];
// Global flag indicating that the crc32 lookup table has been initialized
unsigned int crc32TableInit = 0;

const unsigned long CRC_Init = 0xFFFFFFFF;

const unsigned short CRC_Input[] = {
    0xc00f, 0x9610, 0x5042, 0x0010,         // 16 random 16-bit numbers
    0x7ff7, 0xf86a, 0xb58e, 0x7651,         // these numbers can be
    0x8b88, 0x0679, 0x0123, 0x9599,         // modified if desired
    0xc58c, 0xd1e2, 0xe144, 0xb691
};

// Holds results obtained through the CRC32 module
unsigned long CRC32_Result;

// Holds results obtained through SW
unsigned long SW_CRC32_Results;

// Software CRC32 algorithm function declaration
void initSwCrc32Table(void);
unsigned long updateSwCrc32( unsigned long crc, char c );

int main(void)
{
    unsigned int i;

    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Configure GPIO
    P1OUT &= ~BIT0;                         // Clear LED to start
    P1DIR |= BIT0;                          // P1.0 Output

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // First, use the CRC32 hardware module to calculate the CRC...
    CRC32INIRESW0 = CRC_Init & 0x0000FFFF;      // Init CRC32 HW module
    CRC32INIRESW1 = CRC_Init >> 16;             // Init CRC32 HW module

    for(i = 0; i < (sizeof(CRC_Input) >> 1); i=i+2)
    {
        // Input values into CRC32 Hardware
        CRC32DIW0 = (unsigned int) CRC_Input[i + 0];
        CRC32DIW1 = (unsigned int) CRC_Input[i + 1];
    }

    // Save the CRC32 result
    CRC32_Result = ((unsigned long) CRC32RESRW0 << 16);
    CRC32_Result = ((unsigned long) CRC32RESRW1 & 0x0000FFFF) | CRC32_Result;

    // Now use a software routine to calculate the CRC32...

    // Init SW CRC32
    SW_CRC32_Results = CRC_Init;

    for(i = 0; i < (sizeof(CRC_Input) >> 1); i++)
    {
        // Calculate the CRC32 on the low-byte first
        SW_CRC32_Results = updateSwCrc32(SW_CRC32_Results, (CRC_Input[i] & 0xFF));

        // Calculate the CRC on the high-byte
        SW_CRC32_Results = updateSwCrc32(SW_CRC32_Results, (CRC_Input[i] >> 8));
    }

    // Compare data output results
    if(CRC32_Result == SW_CRC32_Results)    // if data is equal
        P1OUT |= BIT0;                      // set the LED
    else
        P1OUT &= ~BIT0;                     // if not, clear LED

    while(1);                               // infinite loop
}

// Calculate the SW CRC32 byte-by-byte
unsigned long updateSwCrc32( unsigned long crc, char c )
{
    unsigned long tmp, long_c;

    long_c = 0x000000ffL & (unsigned long) c;

    if (!crc32TableInit)
    {
        initSwCrc32Table();
    }

    tmp = crc ^ long_c;
    crc = (crc >> 8) ^ crc32Table[ tmp & 0xff ];

    return crc;
}

// Initializes the CRC32 table
void initSwCrc32Table(void)
{
    int i, j;
    unsigned long crc;

    for (i = 0; i < 256; i++)
    {
        crc = (unsigned long) i;

        for (j = 0; j < 8; j++)
        {
            if ( crc & 0x00000001L )
            {
                crc = ( crc >> 1 ) ^ POLYNOMIAL_32;
            }
            else
            {
                crc =   crc >> 1;
            }
        }
        crc32Table[i] = crc;
    }

    // Set flag that the CRC32 table is initialized
    crc32TableInit = 1;
}

