/* --COPYRIGHT--,BSD
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
 * --/COPYRIGHT--*/
//****************************************************************************
//
// HAL_LCD.h - Prototypes of hardware abstraction layer for LCD on FR6989 LaunchPad
//
//****************************************************************************

//***** Header Files **********************************************************
#include "stdint.h"
#include <msp430fr6989.h>

#ifndef HAL_LCD_H_
#define HAL_LCD_H_

//***** Prototypes ************************************************************
void LCD_init(void);
void LCD_showChar( char, int, int );
void LCD_displayNumber( long, int );
int  LCD_showSymbol( int, int, int );

// Prototypes for DriverLib alternative functions
void LCD_setMemoryBits   ( uint8_t, uint8_t );                             // Set byte-wide LCD Memory with 8-bit SegmentMask
void LCD_updateMemoryBits( uint8_t, uint8_t );                             // Update LCD Memory with 8-bit SegmentMask
void LCD_clearMemoryBits ( uint8_t, uint8_t );                             // Clear specified SegmentMask bits of LCD Memory
void LCD_toggleMemoryBits( uint8_t, uint8_t );                             // Toggle the bits specified by the 8-bit SegmentMask
uint8_t LCD_getMemoryBits   ( uint8_t );                                   // Get the byte-wide LCD Memory location

void LCD_setBlinkingMemoryBits   ( uint8_t, uint8_t );
void LCD_updateBlinkingMemoryBits( uint8_t, uint8_t );                     // Update LCD Blinking Memory with 8-bit SegmentMask
void LCD_clearBlinkingMemoryBits ( uint8_t, uint8_t );                     // Clear specified SegmentMask bits of LCD Blinking Memory
void LCD_toggleBlinkingMemoryBits( uint8_t, uint8_t );                     // Toggle the bits specified by the 8-bit SegmentMask
uint8_t LCD_getBlinkingMemoryBits   ( uint8_t );                           // Get the byte-wide LCD Blinking Memory location


//***** Defines ***************************************************************
#define LCD_NUM_CHAR                6                                             // Number of character positions in the display

/* LCD Symbol Bits */
#define LCD_HEART_IDX               3
#define LCD_HEART_SEG               LCD_C_SEGMENT_LINE_4
#define LCD_HEART_COM               BIT2

#define LCD_TIMER_IDX               3
#define LCD_TIMER_SEG               LCD_C_SEGMENT_LINE_4
#define LCD_TIMER_COM               BIT3

#define LCD_REC_IDX                 3
#define LCD_REC_COM                 BIT1

#define LCD_EXCLAMATION_IDX         3
#define LCD_EXCLAMATION_COM         BIT0

#define LCD_BRACKETS_IDX            18
#define LCD_BRACKETS_COM            BIT4

#define LCD_BATT_IDX                14
#define LCD_BATT_COM                BIT4

#define LCD_B1_IDX                  18
#define LCD_B1_COM                  BIT5

#define LCD_B2_IDX                  14
#define LCD_B2_COM                  BIT5

#define LCD_B3_IDX                  18
#define LCD_B3_COM                  BIT6

#define LCD_B4_IDX                  14
#define LCD_B4_COM                  BIT6

#define LCD_B5_IDX                  18
#define LCD_B5_COM                  BIT7

#define LCD_B6_IDX                  14
#define LCD_B6_COM                  BIT7

#define LCD_ANT_IDX                 5
#define LCD_ANT_COM                 BIT2

#define LCD_TX_IDX                  9
#define LCD_TX_COM                  BIT2

#define LCD_RX_IDX                  9
#define LCD_RX_COM                  BIT0

#define LCD_NEG_IDX                 11
#define LCD_NEG_COM                 BIT2

#define LCD_DEG_IDX                 16
#define LCD_DEG_COM                 BIT2

#define LCD_A1DP_IDX                11
#define LCD_A1DP_COM                BIT0

#define LCD_A2DP_IDX                7
#define LCD_A2DP_COM                BIT0

#define LCD_A3DP_IDX                5
#define LCD_A3DP_COM                BIT0

#define LCD_A4DP_IDX                20
#define LCD_A4DP_COM                BIT0

#define LCD_A5DP_IDX                16
#define LCD_A5DP_COM                BIT0

#define LCD_A2COL_IDX               7
#define LCD_A2COL_COM               BIT2

#define LCD_A4COL_IDX               20
#define LCD_A4COL_COM               BIT2

//***** LCD_showSymbol() function operations
#define LCD_UPDATE                  0
#define LCD_CLEAR                   1
#define LCD_TOGGLE                  2
#define LCD_GET                     3

//***** LCD_showSymbol() function memory enums
#define LCD_MEMORY_MAIN             0
#define LCD_MEMORY_BLINKING         1

//***** LCD_showSymbol() function icons (for 'FR6989 LaunchPad)
#define LCD_TMR                     0
#define LCD_HRT                     1
#define LCD_REC                     2
#define LCD_EXCLAMATION             3
#define LCD_BRACKETS                4
#define LCD_BATT                    5
#define LCD_B1                      6
#define LCD_B2                      7
#define LCD_B3                      8
#define LCD_B4                      9
#define LCD_B5                      10
#define LCD_B6                      11
#define LCD_ANT                     12
#define LCD_TX                      13
#define LCD_RX                      14
#define LCD_NEG                     15
#define LCD_DEG                     16
#define LCD_A1DP                    17
#define LCD_A2DP                    18
#define LCD_A3DP                    19
#define LCD_A4DP                    20
#define LCD_A5DP                    21
#define LCD_A2COL                   22
#define LCD_A4COL                   23

//***** Global Variables ******************************************************
extern const char digit[10][2];                                                 // Segment values for each number character
extern const char alphabetBig[26][2];                                           // Segment values for each alphabet character


#endif /* HAL_LCD_H_ */
