/*******************************************************************************
 *
 *  FFT.c
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
#include "hal_MSP-EXP430F5438.h"
#include "UserExperience_F5438A.h"
#include "FFT.h"
#include "FFT_430.h"

int FBuff[512], outer;
unsigned short index1;

unsigned int voice_data[1024];
unsigned int index;
const int bit_rev_index[] = {0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50,
                             0xD0, 0x30, 0xB0, 0x70, 0xF0, 0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8,
                             0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 0x04,
                             0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4,
                             0x34, 0xB4, 0x74, 0xF4, 0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C,
                             0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 0x02, 0x82,
                             0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32,
                             0xB2, 0x72, 0xF2, 0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA,
                             0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA, 0x06, 0x86, 0x46,
                             0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6,
                             0x76, 0xF6, 0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E,
                             0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE, 0x01, 0x81, 0x41, 0xC1,
                             0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71,
                             0xF1, 0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99,
                             0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 0x05, 0x85, 0x45, 0xC5, 0x25,
                             0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
                             0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D,
                             0xDD, 0x3D, 0xBD, 0x7D, 0xFD, 0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3,
                             0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 0x0B,
                             0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB,
                             0x3B, 0xBB, 0x7B, 0xFB, 0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67,
                             0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 0x0F, 0x8F,
                             0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F,
                             0xBF, 0x7F, 0xFF};


int imag, real, real_abs, imag_abs, mag, max, min;
int FFT_data[512];

unsigned int FFT_Image[1344];

/**********************************************************************//**
 * @brief  Sets up board for recording from microphone
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void shutdownRecorder(void)
{
    halLcdSetBackLight(lcdBackLightLevelSettingLOCAL);

    // Power-down MSP430 modules
    TBCTL = 0;                        // Disable Timer_B
    ADC12CTL1 &= ~ADC12CONSEQ_2;      // Stop conversion immediately
    ADC12CTL0 &= ~ADC12ENC;           // Disable ADC12 conversion
    ADC12CTL0 = 0x00;                 // Switch off ADC12
    ADC12IE = 0x00;

    AUDIO_PORT_OUT &= ~MIC_POWER_PIN; // Turn of MIC
    AUDIO_PORT_SEL &= ~MIC_INPUT_PIN;
}

void setupRecorder()
{
    AUDIO_PORT_OUT |= MIC_POWER_PIN;
    AUDIO_PORT_OUT &= ~MIC_INPUT_PIN;
    AUDIO_PORT_SEL |= MIC_INPUT_PIN;

    UCSCTL8 |= MODOSCREQEN;
    ADC12CTL0 &= ~ADC12ENC;           // Disable conversions to configure ADC12
    ADC12CTL0 = ADC12ON + ADC12SHT02; // Configure ADC12 to sample a sequence of channels, once
    ADC12CTL1 = ADC12SHP + ADC12CONSEQ_2 + ADC12SSEL_2 + ADC12SHS_3;
    ADC12CTL2 = ADC12RES_0;           // Select 8-bit resolution
    ADC12MCTL0 = MIC_INPUT_CHAN | ADC12EOS;
    ADC12CTL0 |= ADC12ENC;            // Enable
    ADC12IE = BIT0;

    // Initialize Timer_B to be used as ADC12 trigger
    TBCTL = TBSSEL_2 + TBCLR;         // Use SMCLK as Timer_B source
    TBCCR0 = 2047;                    // Initialize TBCCR0 (period register)
    TBCCR1 = 2047 - 100;              // Initialize TBCCR1 (ADC12 trigger)
    TBCCTL0 = 0x0000;
    TBCCTL1 = OUTMOD_7;
    TBCCTL2 = 0x0000;
    TBCCTL3 = 0x0000;
    TBCCTL4 = 0x0000;
    TBCCTL5 = 0x0000;
    TBCCTL6 = 0x0000;
}

/**********************************************************************//**
 * @brief  Runs the FFT app. Performs the FFT and displays it to the screen.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void FFTrecorder(void)
{
    unsigned int i, j;
    unsigned int mask = 0x0000;

    fftISR = 1;

    // Initialize FFT variables
    buf_index = 0;
    for (i = 0; i < 1344; i++){
        FFT_Image[i] = 0x0000;
    }

    halLcdClearScreen();
    halLcdPrintLine("    FFT DEMO   ", 0, 0);
    halLcdPrintLine("0", 8, 0);
    halLcdPrintLineCol("Fs 2", 8, 13, 0);
    halLcdLine(120, 107, 126, 97, PIXEL_ON);
    halLcdHLine(0, 138, 96, PIXEL_ON);

    halButtonsInit(BUTTON_ALL);
    halButtonsInterruptEnable(BUTTON_ALL);
    buttonsPressed = 0;

    setupRecorder();

    while (!buttonsPressed)
    {
        TBCCTL1 &= ~CCIFG;
        TBCTL |= MC0;   //start timer B
        __bis_SR_register(LPM0_bits + GIE);
        __no_operation();
        TBCTL &= ~MC0;

        if (buf_index == 512)
            for (i = 0; i < 512; i++)
                voice_data[i] = buffer[i];
        else
            for (i = 0; i < 512; i++)
                voice_data[i] = buffer[i + 512];

        FFT_prog();     // Perform FFT and store

        for (i = 0; i < 256; i++)
        {
            index = bit_rev_index[i];
            real = FBuff[2 * index];
            imag = FBuff[2 * index + 1];
            if (real < 0)
                real_abs = ~real + 1;
            else
                real_abs = real;
            if (imag < 0)
                imag_abs = ~imag + 1;
            else
                imag_abs = imag;
            if (real_abs >= imag_abs)
            {
                max = real_abs;
                min = imag_abs;
            }
            else
            {
                max = imag_abs;
                min = real_abs;
            }
            mag = max + 3 * (min >> 3);
            FFT_data[i] = mag;
        }

        //prepare the image of the FFT for display
        for (i = 1; i < 128; i++)
        {
            switch (i % 8)
            {
                case 1: mask = 0x0003; break;
                case 2: mask = 0x000C; break;
                case 3: mask = 0x0030; break;
                case 4: mask = 0x00C0; break;
                case 5: mask = 0x0300; break;
                case 6: mask = 0x0C00; break;
                case 7: mask = 0x3000; break;
                case 0: mask = 0xC000; break;
                default: mask = 0x0000; break;
            }
            for (j = 0; j < 84; j++)
            {
                if (j < (84 - FFT_data[i] * 2))
                    FFT_Image[((i - 1) / 8) + j * 16] &= ~mask;
                else
                    FFT_Image[((i - 1) / 8) + j * 16] |= mask;
            }
        }
        halLcdImage(FFT_Image, 16, 84, 1, 12); //display the image
    }
    fftISR = 0;
    shutdownRecorder();
}

