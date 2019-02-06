/*******************************************************************************
 *
 *  flashUtils.c
 *
 *  Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
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
 ******************************************************************************/

#include "msp430.h"
#include "flashUtils.h"

/**********************************************************************//**
* @brief  Erases a bank in flash memory.
*
* @param  FarPtr The pointer to the location in memory within the bank to
*                be erased.
*
* @return none
**************************************************************************/

void flashEraseBank(unsigned long FarPtr)
{
    FCTL3 = FWKEY;
    while (FCTL3 & BUSY) ;
    FCTL1 = FWKEY + MERAS;

    __data20_write_char(FarPtr, 0x00);      // Dummy write to start erase

    while (FCTL3 & BUSY) ;
    FCTL1 = FWKEY;
    FCTL3 = FWKEY +  LOCK;
}

/**********************************************************************//**
* @brief  Erases a single segment of memory containing the address FarPtr.
*
* @param  FarPtr The address location within the segment of memory to be
*                erased.
*
* @return none
**************************************************************************/

void flashEraseSegment(unsigned long FarPtr)
{
    FCTL3 = FWKEY;
    FCTL1 = FWKEY + ERASE;

    __data20_write_char(FarPtr, 0x00);      // Dummy write to start erase

    while (FCTL3 & BUSY) ;
    FCTL1 = FWKEY;
    FCTL3 = FWKEY +  LOCK;
}

/**********************************************************************//**
* @brief  Erases the (multiple) segment(s) of memory including address
*         locations Memstart through Memend.
*
* @param  Memstart The starting address location identifying a segment of
*                  memory to be erased
*
* @param  Memend   The ending address location identifying a segment of
*                  memory to be erased
*
* @return none
**************************************************************************/

void flashErase(unsigned long Memstart, unsigned long Memend)
{
    unsigned long FarPtr = Memstart;        // Start of record memory array

    FCTL3 = FWKEY;                          // Unlock Flash memory for write
    do
    {
        if ((FarPtr & 0xFFFF) == 0x0000)    // Use bit 12 to toggle LED
            P1OUT ^= 0x01;

        FCTL1 = FWKEY + ERASE;

        __data20_write_char(FarPtr, 0x00);  // Dummy write to activate

        while (FCTL3 & BUSY) ;              // Segment erase
        FarPtr += 0x0200;                   // Point to next segment
    } while (FarPtr < Memend);

    FCTL1 = FWKEY;
    FCTL3 = FWKEY +  LOCK;
}

