// *************************************************************************************************
//
//      Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/
//
//
//        Redistribution and use in source and binary forms, with or without
//        modification, are permitted provided that the following conditions
//        are met:
//
//          Redistributions of source code must retain the above copyright
//          notice, this list of conditions and the following disclaimer.
//
//          Redistributions in binary form must reproduce the above copyright
//          notice, this list of conditions and the following disclaimer in the
//          documentation and/or other materials provided with the
//          distribution.
//
//          Neither the name of Texas Instruments Incorporated nor the names of
//          its contributors may be used to endorse or promote products derived
//          from this software without specific prior written permission.
//
//        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//        "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//        LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//        LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//        DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//        THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// *************************************************************************************************
// Data logger routines.
// *************************************************************************************************

// *************************************************************************************************
// Include section

// system
#include "project.h"

// logic
#include "datalog.h"

// *************************************************************************************************
// Prototypes section

// *************************************************************************************************
// Defines section

// *************************************************************************************************
// Implementation

void flash_erase_page(u8 page)
{
    // Convert page index to memory address
    u16 *wptr = (u16 *) (page * 512);

    // Range check
    if ((page < DATALOG_PAGE_START) || (page > DATALOG_PAGE_END))
        return;

    // Wait until not busy
    while ((FCTL3 & BUSY) != 0) ;

    __disable_interrupt();
    FCTL3 = FWKEY;              // Clear Lock bit
    FCTL1 = FWKEY + ERASE;      // Set Erase bit
    *wptr = 0;                  // Dummy write to erase Flash seg
    FCTL1 = FWKEY + WRT;        // Set WRT bit for write operation
    FCTL1 = FWKEY;              // Clear WRT bit
    FCTL3 = FWKEY + LOCK;       // Set LOCK bit
    __enable_interrupt();
}

void flash_write(u16 * wptr, u16 data)
{
    // Range check
    //if ((page < DATALOG_MEMORY_START) || (page > DATALOG_MEMORY_END)) return;

    // Wait until not busy
    while ((FCTL3 & BUSY) != 0) ;

    __disable_interrupt();
    FCTL3 = FWKEY;              // Clear Lock bit
    *wptr = 0;                  // Dummy write to erase Flash seg
    FCTL1 = FWKEY + WRT;        // Set WRT bit for write operation
    *wptr = data;
    FCTL1 = FWKEY;              // Clear WRT bit
    FCTL3 = FWKEY + LOCK;       // Set LOCK bit
    __enable_interrupt();
}

