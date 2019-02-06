/* --COPYRIGHT--,BSD
 * Copyright (c) 2010, Texas Instruments Incorporated
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
//****************************************************************************//
//  Inclination Game
//  Puzzle.c
//
//  Describtion:
//    Simple example to demonstrate the functionality of the Excel
//    state machine generation tool
//
//  Target MCU:   MSP430F55529
//
//  Implemented with Application report: SLAA402
//
//   Texas Instruments Freising
//   August 2009
//****************************************************************************//

#include <stdint.h>

#include "msp430.h"
#include "hal_UCS.h"
#include "HAL_Buttons.h"
#include "HAL_LIS3DH.h"
#include "HAL_Dogs102x6.h"
#include "HAL_Wheel.h"
#include "fsm_transition.h"
#include "fsm.h"
#include "Puzzle.h"

extern uint8_t ActState;
extern uint8_t GameData[FIELD_SIZE];

#define ACC_X_THRESHOLD   20
#define ACC_Y_THRESHOLD   20


//****************************************************************************//
// Start Puzzle Game
//****************************************************************************//
void StartPuzzle(void) {

    unsigned int quit = 0;

    buttonsPressed = 0;

    TimerA1Init();

    // Initialize accelerometer
    LIS3DH_init();

    // initialize the Game and set the right active State
    FSM_Init();
    FSM_InitGame();

    //wait for button press
    while (!buttonsPressed)
    {
        // Wait in low power mode 3 until a button is pressed
        __bis_SR_register(LPM3_bits + GIE);
    }

    while (!quit)
    {
        // quit the game
        if (buttonsPressed & BUTTON_S2)
        {
            Dogs102x6_clearScreen();
            TimerA1Stop();
            quit = 1;
        }
        // reset the game
        else if (buttonsPressed & BUTTON_S1){
            FSM_Button();
            buttonsPressed = 0;
        }
    }
    buttonsPressed = 0;

    // Disable accelerometer
    LIS3DH_disable();
}

//****************************************************************************//
// Leave Puzzle Game
//****************************************************************************//
void TimerA1Stop(void){
    //Turn off TimerA1
    TA1CTL = 0;
    TA1CCTL0 &= ~CCIE;
    TA1CCR0 = 0;
}

//****************************************************************************//
// TimerA1 initialisation
//****************************************************************************//
void TimerA1Init(void){
    // ACLK (32.768 kHz), Clear counter, Up mode
    TA1CTL = TASSEL_1 + TACLR + MC_1;

    //523;               // 16ms at ACLK
    TA1CCR0 = 350;

    // CCR0 - Interrupt enabled, clear interrupt flag
    TA1CCTL0 = CCIE;
}

//****************************************************************************//
// TimerA1 ISR
//****************************************************************************//
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer_A1_CCR0_ISR(void){

    unsigned int i;
    static unsigned char tilt = 0;

    // Disable TimerA1 interrupt
    TA1CCTL0 &= ~CCIE;

    // Get the acceleration in x- and y-direction and call event function
    // read values out of accelerometer with removed offset
    LIS3DH_readAccel_offset();

    if      (LIS3DH_xAccel < -ACC_X_THRESHOLD){
        if (tilt == 0) FSM_Right();
        tilt = 1;
    }
    else if (LIS3DH_xAccel >  ACC_X_THRESHOLD){
        if (tilt == 0) FSM_Left();
        tilt = 1;
    }
    else if (LIS3DH_yAccel < -ACC_Y_THRESHOLD){
        if (tilt == 0) FSM_Up();
        tilt = 1;
    }
    else if (LIS3DH_yAccel >  ACC_Y_THRESHOLD){
        if (tilt == 0) FSM_Down();
        tilt = 1;
    }
    else {
        if ((tilt == 1) &&
            (LIS3DH_xAccel > -ACC_X_THRESHOLD / 2) &&
            (LIS3DH_xAccel <  ACC_X_THRESHOLD / 2) &&
            (LIS3DH_yAccel > -ACC_Y_THRESHOLD / 2) &&
            (LIS3DH_yAccel <  ACC_Y_THRESHOLD / 2)
            )
            tilt = 0;
    }

    if (ActState == 9){ // Check for winning the game
        for (i = 0; GameData[i] == (i + 1); i++) ;
        if (i == 8) FSM_Win();
    }

    // Enable  TimerA1 interrupt
    TA1CCTL0 = CCIE;
}
