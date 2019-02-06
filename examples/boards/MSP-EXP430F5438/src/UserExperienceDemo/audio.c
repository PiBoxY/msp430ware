/*******************************************************************************
 *
 *  audio.c
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

#include "hal_MSP-EXP430F5438.h"
#include "UserExperience_F5438A.h"
#include "audio.h"
#include "FFT.h"
#include "flashUtils.h"
#include "menuSetting.h"

static const unsigned long AUDIO_MEM_START[] = {0x10000, 0x20000, 0x30000, 0x40000, 0x46000};
unsigned long PlaybackPtr;
unsigned long lastAudioByte = 0;

static void setupRecord(void);
static void shutdownRecord(void);
static void record(void);

static const char audioText[] = {
    "Select Audio App\0"
    " 1. Voice Rec   \0"
    " 2. FFT         \0"
    " 3. Quit        \0"
};

// -------------- Audio Menu---------------------------------

/**********************************************************************//**
 * @brief  Executes the "Audio Apps" menu option in the User Experience
 *         example code. This menu option allows one to use the on-board
 *         microphone to record your voice or to view the FFT in real time.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void audioMenu(void)
{
    unsigned char AudioMenuPos = 0;
    unsigned char quit = 0;
    unsigned int i;

    halButtonsInterruptDisable(BUTTON_ALL);
    halButtonsInterruptEnable(BUTTON_SELECT + BUTTON_UP + BUTTON_DOWN);
    halLcdClearScreen();

    halLcdPrintLine((char *)audioText, 0, 0);

    buttonsPressed = 0;

    while (!quit)
    {
        for (i = 0; i < 3; i++)
        {
            halLcdPrintLine((char *)&audioText[i * 17 + 17], i + 1,       \
                            OVERWRITE_TEXT);
        }
        halLcdPrintLine((char *)&audioText[AudioMenuPos * 17 + 17], AudioMenuPos + 1,       \
                        INVERT_TEXT | OVERWRITE_TEXT);

        while (!buttonsPressed) ;

        if (buttonsPressed & BUTTON_DOWN)
        {
            halLcdPrintLine((char *)&audioText[AudioMenuPos * 17 + 17], AudioMenuPos + 1,      \
                            OVERWRITE_TEXT);
            if (++AudioMenuPos >= AUDIO_MENU_MAX)
            {
                AudioMenuPos = 0;
            }

            halLcdPrintLine((char *)&audioText[AudioMenuPos * 17 + 17], AudioMenuPos + 1,       \
                            INVERT_TEXT | OVERWRITE_TEXT);
            buttonsPressed = 0;
        }

        if (buttonsPressed & BUTTON_UP)
        {
            halLcdPrintLine((char *)&audioText[AudioMenuPos * 17 + 17], AudioMenuPos + 1,      \
                            OVERWRITE_TEXT);
            if (AudioMenuPos-- <= 0)
            {
                AudioMenuPos = 2;
            }

            halLcdPrintLine((char *)&audioText[AudioMenuPos * 17 + 17], AudioMenuPos + 1,       \
                            INVERT_TEXT | OVERWRITE_TEXT);
            buttonsPressed = 0;
        }

        if (buttonsPressed & BUTTON_SELECT)
        {
            switch (AudioMenuPos)
            {
                case 0: audio(); quit = 0; break;
                case 1: FFTrecorder(); quit = 0; break;
                case 2: quit = 1; break;
                default: break;
            }
            buttonsPressed = 0;

            if (!quit)
            {
                halLcdClearScreen();
                halLcdPrintLine((char *)audioText, 0, 0);
                halLcdPrintLine((char *)&audioText[AudioMenuPos * 17 + 17], AudioMenuPos + 1,       \
                                INVERT_TEXT | OVERWRITE_TEXT);
            }
        }
        buttonsPressed = 0;
    }
}

/**********************************************************************//**
 * @brief  Initializes the microphone, ADC, and DMA for recording.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

static void setupRecord(void)
{
    LED_PORT_OUT |= LED_1;                                                           // Turn on LED
    AUDIO_PORT_OUT |= MIC_POWER_PIN;
    AUDIO_PORT_OUT &= ~MIC_INPUT_PIN;
    AUDIO_PORT_SEL |= MIC_INPUT_PIN;

    lcdBackLightLevelSettingLOCAL = halLcdGetBackLight();
    if (lcdBackLightLevelSettingLOCAL < 5)
        halLcdSetBackLight(0);
    else
        halLcdSetBackLight(16);

    TBCTL = TBSSEL_2;                                                                // Use SMCLK as
                                                                                     // Timer_B
                                                                                     // source
    TBR = 0;
    TBCCR0 = 2047;                                                                   // Initialize
                                                                                     // TBCCR0
    TBCCR1 = 2047 - 100;
    TBCCTL1 = OUTMOD_7;

    UCSCTL8 |= MODOSCREQEN;
    ADC12CTL0 &= ~ADC12ENC;                                                          // Ensure ENC
                                                                                     // is clear
    ADC12CTL0 = ADC12ON + ADC12SHT02;
    ADC12CTL1 = ADC12SHP + ADC12CONSEQ_2 + ADC12SSEL_2 + ADC12SHS_3;
    ADC12CTL2 = ADC12RES_0;                                                          // Select 8-bit
                                                                                     // resolution
    //Sequence of channels, once
    ADC12MCTL0 = MIC_INPUT_CHAN | ADC12EOS;                                          //VeREF+ and
                                                                                     // VeREF-
    ADC12CTL0 |= ADC12ENC;                                                           //Enable
    ADC12CTL0 |= ADC12SC;                                                            //Start
                                                                                     // conversion

    DMACTL0 = DMA0TSEL_24;                                                           // ADC12IFGx
                                                                                     // triggers
                                                                                     // DMA0
    __data16_write_addr((unsigned long)&DMA0SA & 0xffff, (unsigned long)&ADC12MEM0); // Src address
                                                                                     // = ADC12
                                                                                     // module

    halButtonsInterruptDisable(BUTTON_S1);
}

/**********************************************************************//**
 * @brief  Disables the recording peripherals and the microphone.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

static void shutdownRecord(void)
{
    halLcdSetBackLight(lcdBackLightLevelSettingLOCAL);

    TBCTL &= ~MC0;

    FCTL1 = FWKEY;                                                                   // Disable
                                                                                     // Flash write
    FCTL3 = FWKEY + LOCK;                                                            // Lock Flash
                                                                                     // memory

    // Power-down MSP430 modules
    ADC12CTL1 &= ~ADC12CONSEQ_2;                                                     // Stop
                                                                                     // conversion
                                                                                     // immediately
    ADC12CTL0 &= ~ADC12ENC;                                                          // Disable
                                                                                     // ADC12
                                                                                     // conversion
    ADC12CTL0 = 0;                                                                   // Switch off
                                                                                     // ADC12 & ref
                                                                                     // voltage
    REFCTL0 &= ~REFON;

    TBCTL = 0;                                                                       // Disable
                                                                                     // Timer_B
    LED_PORT_OUT &= ~LED_1;                                                          // Turn off LED

    AUDIO_PORT_OUT &= ~MIC_POWER_PIN;                                                // Turn of MIC
    AUDIO_PORT_SEL &= ~MIC_INPUT_PIN;

    saveSettings();                                                                  // Store
                                                                                     // lastAudioByte
                                                                                     // to Flash

    halLcdPrintLine("    DONE    ",  6, INVERT_TEXT | OVERWRITE_TEXT);
    halLcdPrintLineCol("Record",  8, 1,  OVERWRITE_TEXT);
    halLcdPrintLineCol("Play",  8, 12,  OVERWRITE_TEXT);
    buttonsPressed = 0;

    halButtonsInterruptEnable(BUTTON_S1);
}

/**********************************************************************//**
 * @brief  Executes the record.
 *
 * - Unlocks the Flash and initialize to long-word write mode
 * - Initializes the Timer to trigger ADC12 samples
 * - When the operation is done, locks the flash, disables the DMA, and stops
 *   the timer
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

static void record(void)
{
    halLcdPrintLine("  Recording ", 6, INVERT_TEXT | OVERWRITE_TEXT);
    halLcdPrintLineCol("Stop", 8, 12,  OVERWRITE_TEXT);

    FCTL3 = FWKEY;                          // Unlock the flash for write
    FCTL1 = FWKEY + BLKWRT;

    DMA0CTL = DMADSTINCR_3 + DMAEN + DMADSTBYTE +  DMASRCBYTE + DMAIE;
    // Enable Long-Word write, all 32 bits will be written once
    // 4 bytes are loaded

    TBCCTL1 &= ~CCIFG;
    TBCTL |= MC0;

    __bis_SR_register(LPM0_bits + GIE);     // Enable interrupts, enter LPM0
    __no_operation();

    TBCTL &= ~MC0;
    DMA0CTL &= ~(DMAEN + DMAIE);

    FCTL3 = FWKEY + LOCK;                   // Lock the flash from write
}

/**********************************************************************//**
 * @brief  Global API call to record audio on the mike.
 *
 * - Initializes the record using setupRecord()
 * - Erases the memory reserved for audio recording
 * - Records until the memory is full using record()
 * - Shuts down the record using shutdownRecord()
 *
 * @param  mode The mode of audio record
 *
 * - AUDIO_TEST_MODE ...for production test
 * - AUDIO_DEMO_MODE ...called for user sample code
 *
 * @return none
 *************************************************************************/

void audioRecord(unsigned char mode)
{
    unsigned char i;

    setupRecord();

    halLcdPrintLine("   Erasing  ", 6, INVERT_TEXT | OVERWRITE_TEXT);
    halLcdPrintLineCol("----", 8, 12,  OVERWRITE_TEXT);

    // Not used in User Experience sample code
    if (mode == AUDIO_TEST_MODE)
    {
        flashErase(MemstartTest, AUDIO_MEMEND);
        __data16_write_addr((unsigned long)&DMA0DA & 0xffff, (unsigned long)MemstartTest);
        DMA0SZ = (long) (AUDIO_MEMEND - MemstartTest);

        record();

        if (DMA0SZ != ( long) (AUDIO_MEMEND - MemstartTest))
            lastAudioByte = AUDIO_MEMEND - DMA0SZ;
        else
            lastAudioByte = AUDIO_MEMEND;
    }
    // Always used in User Experience sample code
    else
    {
        flashEraseBank(AUDIO_MEM_START[0]);
        flashEraseBank(AUDIO_MEM_START[1]);
        flashEraseBank(AUDIO_MEM_START[2]);
        flashErase(AUDIO_MEM_START[3], AUDIO_MEM_START[4]);

        for (i = 0; i < 3; i++)
        {
            __data16_write_addr((unsigned long)&DMA0DA & 0xffff, (unsigned long)AUDIO_MEM_START[i]);
            DMA0SZ = AUDIO_MEM_START[i + 1] - AUDIO_MEM_START[i] - 1;

            record();

            if (DMA0SZ != AUDIO_MEM_START[i + 1] - AUDIO_MEM_START[i] - 1)
            {
                lastAudioByte = AUDIO_MEM_START[i + 1] - DMA0SZ;
                break;
            }
            else lastAudioByte = AUDIO_MEM_START[i + 1] - 1;
        }
    }

    shutdownRecord();
}

/**********************************************************************//**
 * @brief  Plays back the audio data stored in Flash memory using
 *         the integrated DMA controller and the DAC12 module.
 *
 * @param  mode The mode of audio record
 *
 * - AUDIO_TEST_MODE ...for production test
 * - AUDIO_DEMO_MODE ...called for user sample code
 *
 * @return none
 *************************************************************************/

void audioPlayBack(unsigned char mode)
{
    // Power-up external hardware
    AUDIO_PORT_DIR |= AUDIO_OUT_PWR_PIN;
    AUDIO_PORT_OUT &= ~AUDIO_OUT_PWR_PIN;
    AUDIO_OUT_SEL |= AUDIO_OUT_PIN;         // P4.4 = TB4
    LED_PORT_OUT |= LED_1;                  // Turn on LED

    if (mode == AUDIO_TEST_MODE)
        PlaybackPtr = (unsigned long)(MemstartTest);
    else
        PlaybackPtr = (unsigned long)(AUDIO_MEMSTART);

    PlaybackPtr++;

    /* Setup Timer0_A for playback */
    // Use SMCLK as Timer0_A source, enable overflow interrupt
    TBCTL = TBSSEL_2 + TBIE;
    // Set output resolution (8 bit. Add 10 counts of headroom for loading TBCCR1
    TBCCR0 = 255;
    TBCCR4 = 255 >> 1;                      // Default output ~Vcc/2
    // Reset OUT1 on EQU1, set on EQU0. Load TBCCR1 when TBR counts to 0.
    TBCCTL4 = OUTMOD_7 + CLLD_1;
    // Start Timer_B in UP mode (counts up to TBCCR0)
    TBCTL |= MC0;

    halLcdPrintLine("   Playing  ", 6, INVERT_TEXT | OVERWRITE_TEXT);
    halLcdPrintLineCol(" Stop ", 8, 1,  OVERWRITE_TEXT);
    halButtonsInterruptDisable(BUTTON_S2);

    // Activate LPM during DMA playback, wake-up when finished
    __bis_SR_register(LPM0_bits + GIE);     // Enable interrupts, enter LPM0
    __no_operation();

    halLcdPrintLine("    DONE    ", 6, INVERT_TEXT | OVERWRITE_TEXT);
    halButtonsInterruptEnable(BUTTON_S2);
    halLcdPrintLineCol("Record", 8, 1,  OVERWRITE_TEXT);
    halLcdPrintLineCol("Play", 8, 12,  OVERWRITE_TEXT);

    // Power-down MSP430 modules
    TBCTL = 0;                              // Disable Timer_B PWM generation
    AUDIO_OUT_SEL &= ~AUDIO_OUT_PIN;        // P4.4 = TB4
    LED_PORT_OUT &= ~LED_1;                 // Turn off LED
    buttonsPressed = 0;
}

/**********************************************************************//**
 * @brief  Calls the audioRecord function with the AUDIO_TEST_MODE as the
 *         parameter.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void audioRecordTest(void)
{
    audioRecord(AUDIO_TEST_MODE);
}

/**********************************************************************//**
 * @brief  Calls the audioPlayBack function with the AUDIO_TEST_MODE as the
 *         parameter.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void audioPlayBackTest(void)
{
    audioPlayBack(AUDIO_TEST_MODE);
}

/**********************************************************************//**
 * @brief  Executes the "Voice Rec" menu option in the User Experience
 *         example code. Calls the necessary functions according to the
 *         user selection of "Record", "Play", or Exit (center D-pad btn press)
 *
 * - Initializes the LCD to display the "Record" and "Play" selections
 * - Enters LPM3 to be awoken and continue code execution after a button press
 * - According to the user selection records audio, plays audio, or exits
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void audio(void)
{
    unsigned char quit = 0;

    audioISR = 1;
    halButtonsInterruptDisable(BUTTON_ALL);
    halButtonsInterruptEnable(BUTTON_SELECT + BUTTON_S1 + BUTTON_S2);
    halAccelerometerShutDown();
    halLcdClearScreen();

    while (!quit)
    {
        buttonsPressed = 0;
        halLcdPrintLineCol("Record", 8, 1,  OVERWRITE_TEXT);
        halLcdPrintLineCol("Play", 8, 12,  OVERWRITE_TEXT);

        __bis_SR_register(LPM3_bits + GIE); //Returns if button pressed
        __no_operation();

        if (buttonsPressed & BUTTON_S1)
        {
            halLcdPrintLineCol("Record", 8, 1, INVERT_TEXT | OVERWRITE_TEXT);
            audioRecord(AUDIO_DEMO_MODE);
            halLcdPrintLineCol("Record", 8, 1,  OVERWRITE_TEXT);
        }
        if (buttonsPressed & BUTTON_S2)
        {
            halLcdPrintLineCol("Play", 8, 12, INVERT_TEXT | OVERWRITE_TEXT);
            audioPlayBack(AUDIO_DEMO_MODE);
            halLcdPrintLineCol("Play", 8, 12,  OVERWRITE_TEXT);
        }
        if (buttonsPressed & BUTTON_SELECT)
            quit = 1;
    }
    audioISR = 0;
    halButtonsInterruptDisable(BUTTON_ALL);
    halButtonsInterruptEnable(BUTTON_SELECT + BUTTON_UP + BUTTON_DOWN);
}

/********************************************************************/
#pragma vector=DMA_VECTOR
__interrupt void DMA_ISR(void)
{
    DMA0CTL &= ~DMAIFG;
    __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0 on reti
}

#pragma vector=TIMERB1_VECTOR
__interrupt void TIMERB1_ISR(void)
{
    if (audioISR == 1)                    //recorder service routine
    {
        static int counter  = 0;
        switch (TBIV)
        {
            case 14:
                if (++counter == 8)
                {
                    counter  = 0;
                    TBCCR4 = (*((unsigned char*)PlaybackPtr));
                    PlaybackPtr++;
                    if ((unsigned long) PlaybackPtr >= (lastAudioByte))
                    {
                        TBCTL = 0;
                        __bic_SR_register_on_exit(LPM0_bits);
                    }
                }
            default: break;
        }
    }
    else if (fftISR == 1)              //fft service routine
    {
        switch (TBIV)
        {
            case 14:
                ADC12CTL0 &= ~ADC12SC; //Start conversion
                ADC12CTL0 |= ADC12SC;
            default: break;
        }
    }
}

