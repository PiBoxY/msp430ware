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
 * ULPMeter.c
 * User Experience Code for the MSP-EXP430FR5969
 * Battery-less ADC Application
 *
 * Program Execution: Objective is to keep the ball from passing either paddle
 *   on the sides of the screen by using the CapTouch sliders on each side.
 *   When the ball is hit by the paddle, points are awarded (respective of game
 *   difficulty). User plays until 5 lives are lost.
 *
 * Directions for use: Upon entry, a menu is displayed for choosing game
 *   difficulty. When select button is pressed, the game begins and user has 5
 *   lives to hit the ball as many times as possible. Game can be ended by
 *   pressing select button again. User will be returned to difficulty menu. If
 *   menu button is pressed, user exits game app entirely.
 *
 * February 2012
 *
 ******************************************************************************/

#include "grlib.h"
#include "Sharp96x96.h"
#include "CTS/CTS_wolverine.h"

#include "Game.h"
#include "FR59xx_EXP.h"
#include "myTimer.h"
#include "UpdateDisplay.h"

#define EASY_MODE    2
#define NORMAL_MODE  4
#define HARD_MODE    6
#define INSANE_MODE  8

#define SEL_MENU_MODE  2
#define SEL_GAME_MODE  4


// Game Global Variables
//#pragma DATA_SECTION(HighScore,".HIGHSCORE_DATA")
#pragma location = 0xF004
#if defined(__IAR_SYSTEMS_ICC__)
__no_init unsigned long HighScore;
#elif defined(__TI_COMPILER_VERSION__)
#pragma NOINIT(HighScore)
unsigned long HighScore;
#elif defined(__GNUC__)
unsigned long HighScore __attribute((section ("HIGHSCORE_DATA")));
#endif

//-----------------------------------------------------------------------------
// SliderBall Game Variables
volatile unsigned char MIN_Y = 14 + 3;          // Max and min y for ball, leave buffer for size
volatile unsigned char MAX_Y = 82 - 3;          // of ball itself
volatile unsigned char MIN_X = 6 + 3;           // Max and min x for ball, leave buffer for size
volatile unsigned char MAX_X = 89 - 3;          // of ball and paddle

volatile unsigned char x;                       // horizontal position of ball
volatile unsigned char y;                       // vertical position of ball
volatile unsigned char dx;                      // horiz direction/speed of ball, start positive
volatile unsigned char dy;                      // vert direction/speed of ball, start positive

volatile unsigned char halfstep = 1;            // flag to slow down horizontal speed
volatile unsigned char gameMode;
volatile unsigned char gameSpeedX;
volatile unsigned char gameSpeedY;

unsigned int lives;
unsigned int score;
unsigned int scoreMultiplier;
unsigned char missed;

static void collisionDetect(unsigned char position_l, unsigned char position_r);        // Collision detection algorithm
static unsigned char chooseGameMode(void);                                              // Choose speed of ball
static void roundingAlgorithm(void);                                                    // Better gameplay rounding algorithm


/**********************************************************************//**
 * @brief  SliderBall Game Application - Push button to start. Every time
 *         the ball hits a paddle, 5*scoreMultiplier points is added. User
 *         gets 5 lives. Every miss costs a life. Once all 5 lives are lost,
 *         game is over, the user's score is displayed, and the user will
 *         return to the game menu, to again choose the difficulty.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void SliderBall_app(void)
{
    while (mode == APP_SLIDERBALL) {
        select = SEL_MENU_MODE;
        gameMode = chooseGameMode();
        gameSpeedX = dx;
        gameSpeedY = dy;

        if (select) {                                                   // Only display if main menu button not pressed
            GrClearDisplay(&sContext);                                  // init buffer (get ready for game)
            GrContextFontSet(&sContext, &g_sFontCmss12);
            GrLineDrawH(&sContext, 0, 96, 13);                          // Draw Upper Bound Line
            GrLineDrawH(&sContext, 0, 96, 83);                          // Draw Lower Bound Line

            lives = 5; score = 0; dy = 3; missed = 0;                   // Reset game
            y = 44; x = 32;                                             // Initialize ball in the middle of the screen
            LCD_updateLives(lives);
            LCD_updateScore(score);
        }

        while ((select == SEL_GAME_MODE) && (lives != 0)) {
            CTS_getReading();
            roundingAlgorithm();

            LCD_drawSliderBall((unsigned char)sliderPos[0], (unsigned char)sliderPos[1], x, y);

            if (missed) {             // If ball was missed, allow for reset time
                LCD_updateLives(lives);
                TA1_sleep(150);
                missed--;
            }

            // For easier levels, sleep between ball movements to slow gameplay
            switch (gameMode) {
            case EASY_MODE:             // Slowest speed
                TA1_sleep(125);
                break;
            case NORMAL_MODE:             // Normal Speed
                TA1_sleep(50);
                break;
            case HARD_MODE:             // Fast
                TA1_sleep(6);
                break;
            case INSANE_MODE:             // Super fast
                __no_operation();
                break;
            default: break;
            }
            collisionDetect((unsigned char)sliderPos[0], (unsigned char)sliderPos[1]);
        }

        if (select) {                                   // Do not display game over if main menu button pressed
            if (score > HighScore) {                    // Show "NEW HIGH SCORE" if high score is beaten
                LCD_displayNewHighScore(score);
                TA1_sleep(8192);                        // 2s
            }
            LCD_displayGameOver(score);
            TA1_sleep(12288);             // 3s
        }
    }
}

/**********************************************************************//**
 * @brief  Collision detection for the SliderBall game. When the ball
 *         reaches the end of the screen, it checks if the ball has made
 *         contact with the paddle on that side, if so, the ball turns
 *         around and the player is awarded points (amount of points is
 *         determined by game mode). If not, the ball returns to the center
 *         and the player loses a life.
 *
 * @param  position_l and position_r - position variables passed in by the
 *         CapTouch functions
 *
 * @return none
 *************************************************************************/
static void collisionDetect(unsigned char position_l, unsigned char position_r)
{
    unsigned char leftPaddle, rightPaddle;

    leftPaddle = (unsigned char)(105 - (52 * position_l / 18));
    rightPaddle = (unsigned char)(105 - (52 * position_r / 18));

    x += dx;
    y += dy;

    // Check for collision with paddles
    if ((x > MAX_X) && (dx == gameSpeedX)) {     // Ball on right side, check right paddle
        if ( ((y > rightPaddle) && ((y - rightPaddle) < 11))
             || ((y < rightPaddle) && ((rightPaddle - y) < 11))
             || (y == rightPaddle)) {
            dx = -dx;             //paddle hit, turn ball around, add points
            x = MAX_X;
            score += 5 * scoreMultiplier;
            LCD_updateScore(score);
        }
        else {           // miss, reset ball, lose life
            x = 32;
            y = 44;
            dy = gameSpeedY;
            missed++;
            lives--;
        }
    }
    else if ((x < MIN_X) && (dx != gameSpeedX)) {      // Ball on left side, check left paddle
        if ( ((y > leftPaddle) && ((y - leftPaddle) < 12))
             || ((y < leftPaddle) && ((leftPaddle - y) < 12))
             || (y == leftPaddle)) {
            dx = -dx;             //paddle hit, turn ball around, add points
            x = MIN_X;
            score += 5 * scoreMultiplier;
            LCD_updateScore(score);
        }
        else {           // miss, reset ball, lose life
            x = 64;
            y = 44;
            dy = gameSpeedY;
            missed++;
            lives--;
        }
    }

    // Check for collision with upper/lower bounds
    if (y >= MAX_Y) {     // hit edge, change direction
        y = MAX_Y;
        dy = -dy;
    }
    else if (y < MIN_Y) {
        y = MIN_Y;
        dy = -dy;
    }

}

/**********************************************************************//**
 * @brief  Chooses the game mode. Valid choices are EASY_MODE, NORMAL_MODE,
 *         HARD_MODE, and INSANE_MODE.
 *
 * @param  none
 *
 * @return unsigned char - speed choice (1, 2, 3, or 4)
 *************************************************************************/
static unsigned char chooseGameMode(void)
{
    unsigned char choice = 0;

    while (select == SEL_MENU_MODE) {
        CTS_getReading();

        if (sliderPos[0] <= 18) {
            choice = INSANE_MODE;
            scoreMultiplier = 10;
            dx = 4;
            dy = 6;
        }
        else if (sliderPos[0] <= 24) {
            choice = HARD_MODE;
            scoreMultiplier = 4;
            dx = 2;
            dy = 3;
        }
        else if (sliderPos[0] <= 30) {
            choice = NORMAL_MODE;
            scoreMultiplier = 2;
            dx = 2;
            dy = 3;
        }
        else {
            choice = EASY_MODE;
            scoreMultiplier = 1;
            dx = 2;
            dy = 3;
        }
        LCD_sliderBallMenu(choice);
    }
    return choice;
}

/**********************************************************************//**
 * @brief  Round slider positions so gameplay is easier/better. Top and
 *         bottom of CapTouch sliders are cut off, as they are less responsive
 *         to touch and make gameplay more difficult
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
static void roundingAlgorithm(void)
{
    // Rounding algorithm to make gameplay better
    if (sliderPos[0] < 11) {
        sliderPos[0] = 11;
    }
    else if (sliderPos[0] > 29) {
        sliderPos[0] = 29;
    }
    if (sliderPos[1] < 11) {
        sliderPos[1] = 11;
    }
    else if (sliderPos[1] > 29) {
        sliderPos[1] = 29;
    }
}
