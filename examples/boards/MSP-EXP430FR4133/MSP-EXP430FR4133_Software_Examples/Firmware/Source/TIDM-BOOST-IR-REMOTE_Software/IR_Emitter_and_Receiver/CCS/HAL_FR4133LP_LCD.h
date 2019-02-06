/* --COPYRIGHT--,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
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
/*
 * HAL_FR4133LP_LCD.h
 *
 *  Created on: Jul 30, 2014
 *      Author: a0222631
 */

#ifndef HAL_FR4133LP_LCD_H_
#define HAL_FR4133LP_LCD_H_

//LCD display definition
#define pos1 4   // Digit A1 - L4
#define pos2 6   // Digit A2 - L6
#define pos3 8   // Digit A3 - L8
#define pos4 10  // Digit A4 - L10
#define pos5 2   // Digit A5 - L2
#define pos6 18  // Digit A6 - L18

extern void Init_LCD();
extern void LCD_Clear();
extern void LCD_Display_digit(unsigned char pos, unsigned char ch);
extern void LCD_Display_letter(unsigned char pos, unsigned char ch);
extern void LCD_Display_TX();
extern void LCD_Display_RX();
extern void LCD_Display_Buttons(unsigned char btn);

#endif /* HAL_FR4133LP_LCD_H_ */
