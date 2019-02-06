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
 * FRAMSpeedApp.c
 * User Experience Code for the MSP-EXP430FR5969
 * FRAM Write Speed/Endurance Application
 *
 * Program Execution: FRAM is written in 1KB blocks using DMA with MCLK = 8MHz.
 *
 * Directions for use: Right button is disabled. Left button returns user to
 *   main menu.
 *
 * February 2012
 *
 *************************************************************************//****
 * Rev 1.2 - driverlib integration
 ******************************************************************************/

#include "driverlib.h"
#include "Sharp96x96.h"

#include "FRAMSpeedApp.h"
#include "FR59xx_EXP.h"
#include "myTimer.h"
#include "UpdateDisplay.h"

// 1KB of FRAM to write to. Note that the linker command file needs to be used to
// reserve the 0x0400 bytes starting at this address. This memory must not be used
// by the C compiler.
#define FRAM_WRITE_START       0xFB80

//-----------------------------------------------------------------------------
// FRAM Speed Global Variables
#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = 0xF000
__no_init unsigned long decimalPlaces;
#elif defined(__TI_COMPILER_VERSION__)
#pragma location = 0xF000
#pragma NOINIT(decimalPlaces)
unsigned long decimalPlaces;            // for endurance, decimal places past 99.9
#elif defined(__GNUC__)
unsigned long decimalPlaces __attribute((section("APP_DATA_FRAM_SP")));
#endif

volatile unsigned int timerA3InterruptCounter;
volatile unsigned int count;            // count variable, counts KB of FRAM data written

unsigned int data;                      // data to be written
unsigned long kBytes;                   // variable to write in FRAM,holds KB written in last 1/4 second
unsigned long totKB;                    // total # of KB written to FRAM, written in FRAM

static void FRAM_speedInit(void);
static void FRAM_speedExit(void);

/**********************************************************************//**
 * @brief  FRAM Speed & Endurance Application
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void FRAM_speedApp(void)
{
    FRAM_speedInit();                   // Set up clock/GPIO pins

    while (mode == APP_FRAM_SPEED) {    // Continuously write data to FRAM, display stats every 1/4s
        while (timerA3InterruptCounter == 1) {
            DMA0CTL |= DMAREQ;          // Trigger write of 1KB to FRAM
            __bis_SR_register(LPM0_bits + GIE);
            __no_operation();
        }

        DMA_disableTransfers(DMA_CHANNEL_0);
        DMA_clearInterrupt(DMA_CHANNEL_0);
        DMA_disableInterrupt(DMA_CHANNEL_0);

        // Do necessary calculations for speed/endurance
        kBytes = count << 2;                    // 1KB block, 1/4 second - multiply count by 4 to get KBps
        totKB += count;
        decimalPlaces -= 2;                     // Update endurance decimal places

        // Display write speed (in KBps), total KB written, and FRAM endurance
        LCD_displaySpeed(kBytes, totKB, decimalPlaces);

        count = 0;                                          // Clear count
        Timer_A_clear(__MSP430_BASEADDRESS_T3A2__);         // and timer
        timerA3InterruptCounter = 1;

        DMA_enableTransfers(DMA_CHANNEL_0);
        DMA_enableInterrupt(DMA_CHANNEL_0);
    }

    FRAM_speedExit();                       // Set up clock/GPIO pins
}

/**********************************************************************//**
 * @brief  Initialize clock/timer for FRAM Speed/Endurance App
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
static void FRAM_speedInit(void)
{
    totKB = 0;
    kBytes = 0;
    data = 0x1234;                              // Initialize dummy data
    timerA3InterruptCounter = 1;                // Init as running

    // Setup DMA for 1KB block
    DMA_initParam dma_param = {0};
    dma_param.channelSelect = DMA_CHANNEL_0;
    dma_param.transferModeSelect = DMA_TRANSFER_REPEATED_BLOCK;
    dma_param.transferSize = 512;
    dma_param.triggerSourceSelect = DMA_TRIGGERSOURCE_0;
    dma_param.transferUnitSelect = DMA_SIZE_SRCWORD_DSTWORD;
    dma_param.triggerTypeSelect = DMA_TRIGGER_RISINGEDGE;
    DMA_init(&dma_param);

    DMA_setSrcAddress(DMA_CHANNEL_0, (unsigned long)&data,
                      DMA_DIRECTION_UNCHANGED);
    DMA_setDstAddress(DMA_CHANNEL_0, (unsigned long)FRAM_WRITE_START,
                      DMA_DIRECTION_INCREMENT);
    DMA_enableTransfers(DMA_CHANNEL_0);
    DMA_enableInterrupt(DMA_CHANNEL_0);

    // Timer3_A2 interrupt for 1/4s
    Timer_A_initUpModeParam timerA_param = {0};
    timerA_param.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    timerA_param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    timerA_param.timerPeriod = 8192;
    timerA_param.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    timerA_param.captureCompareInterruptEnable_CCR0_CCIE =
    		TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
    timerA_param.timerClear = TIMER_A_SKIP_CLEAR;
    timerA_param.startTimer = true;
    Timer_A_initUpMode(TIMER_A3_BASE, &timerA_param);
}

/**********************************************************************//**
 * @brief  Resets clock and turns off Timer for exiting FRAM speed app
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
static void FRAM_speedExit(void)
{
    Timer_A_disableCaptureCompareInterrupt(__MSP430_BASEADDRESS_T3A2__, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    Timer_A_stop(__MSP430_BASEADDRESS_T3A2__);

    DMA_disableTransfers(DMA_CHANNEL_0);
    DMA_clearInterrupt(DMA_CHANNEL_0);
    DMA_disableInterrupt(DMA_CHANNEL_0);
}

/**********************************************************************//**
 * @brief  Interrupt Service Routine for Timer A1
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
#if defined (__TI_COMPILER_VERSION__) || defined (__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER3_A0_VECTOR
__interrupt void TA3_ISR(void)
#elif defined(__GNUC__)
void TA3_ISR(void) __attribute__((interrupt(TIMER3_A0_VECTOR)));
void TA3_ISR(void)
#else
#error Compiler not supported!
#endif
{
    // Count entries into this ISR...
    timerA3InterruptCounter++;
}

/**********************************************************************//**
 * @brief  Interrupt Service Routine for DMA
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
#if defined (__TI_COMPILER_VERSION__) || defined (__IAR_SYSTEMS_ICC__)
#pragma vector = DMA_VECTOR
__interrupt void DMA_ISR(void)
#elif defined(__GNUC__)
void DMA_ISR(void) __attribute__((interrupt(DMA_VECTOR)));
void DMA_ISR(void)
#else
#error Compiler not supported!
#endif
{
    if (mode == APP_FRAM_SPEED) {       // FRAM Speed only - increase count
        count++;                        // Count blocks written
    }

    // FRAM Speed & Battery Free - clear IFG and wake up from LPM0
    DMA_clearInterrupt(DMA_CHANNEL_0);
    __bic_SR_register_on_exit(LPM0_bits);
}
