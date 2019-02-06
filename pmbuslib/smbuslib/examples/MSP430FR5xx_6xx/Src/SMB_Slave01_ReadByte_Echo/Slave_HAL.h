/* --COPYRIGHT--,BSD
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
 * --/COPYRIGHT--*/
#ifndef __SLAVE_HAL_H__
#define __SLAVE_HAL_H__

//
// Include files
//
#include <stdint.h>
#include <stdbool.h>
#include "msp430.h"

typedef enum
{
    LED0 = 0,                 /*! Access LED 0 */
    LED1,                   /*! Access LED1 */
}t_LED;

//
// Function prototypes
//
extern void HAL_MCU_Init(void);
extern void HAL_LED_SetHigh(t_LED LED);
extern void HAL_LED_SetLow(t_LED LED);
extern void HAL_LED_Toggle(t_LED LED);
extern void HAL_I2C_InitGPIO(void);
extern uint32_t HAL_I2C_GetBaseAddr(void);

//
// Events (called by HAL, declared in Application)
//
extern bool HAL_I2C_Event(void);
#ifdef __MSP430_HAS_USCI__
extern bool HAL_Timeout_Event(void);
#endif

#endif //__SLAVE_HAL_H__
