/*=============================================================
 * @file:		TI_SMS_Motor_Fault_API.c
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

#include "TI_SMS_Motor_ADC_API.h"
#include "TI_SMS_Motor_SPI_API.h"
#include "TI_SMS_Motor_Fault_API.h"

UINT32 TI_SMS_MOTOR_Fault_Init(FAULT_CURRENT_PARAMS_S *p) //called in system init
{ 
	UINT32 errCode = 0;
	
    // Enable Hardware over-current interrupt (P1.1)
    if (p->overCurrentIntEnabled)
    {
        P1DIR &= 0xFD;                                      // P1.1 define as input 
        P1SEL &= 0xFD;                                      // SEL define as port function
        P1IES = 0x00;										// Low-to-high transition
        P1IFG &= ~BIT1;										// Clear interrupt flag for P1.1
        P1IE |= BIT1;										// P1.1 interrupt enabled (HW OC)   
    }
    else
    {
        P1DIR &= 0xFD;                                      // P1.1 define as input 
        P1SEL &= 0xFD;                                      // SEL define as port function
        P1IES = 0x00;										// Low-to-high transition
        P1IFG &= ~BIT1;										// Clear interrupt flag for P1.1
        P1IE &= 0xFD;		
    }  
    return (errCode); 
}
