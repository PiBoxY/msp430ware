/* --COPYRIGHT--,BSD
 * Copyright (c) 2012, Texas Instruments Incorporated
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
/*******************************************************************************
 *
 * UpdateDisplay.h
 * User Experience Code for the MSP-EXP430FR5969
 * main LCD/graphics header file
 *
 * Created: Version 1.0:
 *
 ******************************************************************************/

#ifndef UPDATE_DISPLAY
#define UPDATE_DISPLAY

extern void LCD_turnOn(void);                                                                                       // Turn LCD on
extern void LCD_display(void);                                                                                      // Pull DISP high
extern void LCD_turnOff(void);                                                                                      // Turn LCD off
extern void LCD_introWrite(void);                                                                                   // Write "Wolverine User's Experience" as an intro
extern void LCD_displayMainMenu(unsigned char choice);                                                              // display the main menu
extern void LCD_displayULPSubMenu(unsigned char choice);                                                            // display battery free menu
extern void LCD_introULP(void);                                                                                     // Write intro for battery free
extern void LCD_drawBattery(signed int bars);                                                                       // Draw battery for battery free app
extern void LCD_drawLowBattery(signed int bars);                                                                    // Draw low battery warning
extern void LCD_displaySendingScreen(void);                                                                         // UART Sending screen
extern void LCD_showDataSent(unsigned int bytes);                                                                   // done sending screen
extern void LCD_displaySpeed(unsigned long speed, unsigned long total,
                             unsigned long decimal);                                                                // display FRAM write speed & stuff
extern void LCD_updateClock(unsigned char blink, unsigned int value, unsigned char choose, unsigned char LPM3ON );  // Update the clock contents in the LCD buffer
extern void LCD_displayFrequencyMenu(unsigned char choice);                                                         // Display frequency menu
extern void LCD_displayCacheRatioMenu(unsigned char choice);                                                        // Display cache hit ratio menu
extern void LCD_activePowerIntro(void);                                                                             // Display intro/instructions for active power app
extern void LCD_sliderBallMenu(unsigned char choice);                                                               // Display the menu choices for the SliderBall game
extern void LCD_drawSliderBall(unsigned char position_l, unsigned char position_r,
                               unsigned char x, unsigned char y);
extern void LCD_updateLives(unsigned char lives);           // Update how many lives the user has left
extern void LCD_updateScore(unsigned int score);            // Update User's score
extern void LCD_displayGameOver(unsigned int score);        // user lost game, show game over and score.
extern void LCD_displayNewHighScore(unsigned int score);    // user beat high score, show it.
extern void LCD_drawPicture(int choice);                    // Draw a picture (from .h file)
extern int UnsignedInt_To_ASCII(unsigned int hex, char *ASCII);
extern int UnsignedLong_To_ASCII(unsigned long hex, char *ASCII);

#endif
