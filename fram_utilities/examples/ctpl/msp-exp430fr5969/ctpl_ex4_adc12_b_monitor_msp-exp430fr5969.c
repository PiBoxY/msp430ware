/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
//******************************************************************************
//  ctpl_ex4_adc12_b_monitor - ADC12_B Powerloss Monitor.
//
//  This example demonstrates how to use the ADC12_B battery monitor and window
//  comparator to actively monitor supply voltage and detect when power is lost.
//  The ADC12_B peripheral is configured with a 2.0V reference voltage and the
//  internal battery monitor channel provides Vcc/2. The ADC12_B low side window
//  comparator is configured to trigger the interrupt when Vcc reaches
//  ADC_MONITOR_THRESHOLD, 3.0V by default. The high side window comparator is
//  set to ADC_MONITOR_THRESHOLD + 0.1V to ensure the device has reached a
//  stable voltage before enabling the monitor. When the high side interrupt is
//  triggered it is disabled and the low side interrupt is enabled to begin
//  actively monitoring Vcc. When power loss is detected the device will invoke
//  the ctpl_enterShutdown API which saves the application and peripheral state
//  and waits for the device to enter BOR with a 64ms timeout. The device will
//  restore application and peripheral state when power is restored and continue
//  execution from the next line of code.
// 
//  The main application will blink LED2 with incremental counts, resetting
//  after four blinks. The power supply can be removed (by disconnecting the USB
//  cable or unplugging the jumpers connecting the on-board emulator to the
//  device) after a specific count of blink and then reapplied to verify that
//  context was saved.
// 
//  ACLK = VLOCLK, MCLK = SMCLK = DCO = ~1MHz
//
//           MSP-EXP430FR5969
//           ---------------
//       /|\|               |
//        | |               |
//        --|RST        P1.0|---> LED2
//          |               |
//          |               |
//
//  Brent Peterson
//  Texas Instruments Inc.
//  April 2015
//******************************************************************************
#include <msp430.h>

#include <ctpl.h>

#define ADC_MONITOR_THRESHOLD   3.0
#define ADC_MONITOR_FREQUENCY   1000

#define MCLK_FREQUENCY          1000000
#define SMCLK_FREQUENCY         1000000

extern void initGpio(void);
extern void initClocks(void);
extern void initAdcMonitor(void);

int main(void)
{
    uint16_t i, j;
    
    /* Halt the watchdog timer */
    WDTCTL = WDTPW | WDTHOLD;

    /* Initialize the GPIO, clock system and the ADC monitor. */
    initGpio();
    initClocks();
    initAdcMonitor();

    /* Enable global interrupts. */
    __enable_interrupt();

    /* Blink P1.0 between 1-4 counts every 1 second. */
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    while (1) {
        for (i = 1; i < 5; i++) {
            for (j = i; j > 0; j--) {
                /* Blink the LED for 0.1s and delay for 0.2s */
                P1OUT |= BIT0;
                __delay_cycles(MCLK_FREQUENCY/10);
                P1OUT &= ~BIT0;
                __delay_cycles(MCLK_FREQUENCY/10);
            }
            /* Delay 1 second between counts. */
            __delay_cycles(MCLK_FREQUENCY);
        }
    }
}

void initGpio(void)
{
    /* Configure GPIO to default state */
    P1OUT = 0; P1DIR = 0xFF;
    P2OUT = 0; P2DIR = 0xFF;
    P3OUT = 0; P3DIR = 0xFF;
    P4OUT = 0; P4DIR = 0xFF;
    PJOUT = 0; PJDIR = 0xFFFF;

    /* Disable the GPIO power-on default high-impedance mode. */
    PM5CTL0 &= ~LOCKLPM5;
}

void initClocks(void)
{
    /* Clock System Setup, MCLK = SMCLK = DCO (1MHz), ACLK = VLOCLK */
    CSCTL0_H = CSKEY >> 8;
    CSCTL1 = DCOFSEL_0;
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
    CSCTL0_H = 0;
}

void initAdcMonitor(void)
{
    /* Initialize timer for ADC trigger. */
    TA0CCR0 = (SMCLK_FREQUENCY/ADC_MONITOR_FREQUENCY);
    TA0CCR1 = TA0CCR0/2;
    TA0CCTL1 = OUTMOD_3;
    TA0CTL = TASSEL__SMCLK | MC__UP;

    /* Configure internal 2.0V reference. */
    while(REFCTL0 & REFGENBUSY);
    REFCTL0 |= REFVSEL_1 | REFON;
    while(!(REFCTL0 & REFGENRDY));

    /*
     * Initialize ADC12_B window comparator using the battery monitor.
     * The monitor will first enable the high side to the monitor voltage plus
     * 0.1v to make sure the voltage is sufficiently above the threshold. When
     * the high side is triggered the interrupt service routine will switch to
     * the low side and wait for the voltage to drop below the threshold. When
     * the voltage drops below the threshold voltage the device will invoke the
     * compute through power loss shutdown function to save the application
     * state and enter complete device shutdown.
     */
    ADC12CTL0 = ADC12SHT0_2 | ADC12ON;
    ADC12CTL1 = ADC12SHS_1 | ADC12SSEL_0 | ADC12CONSEQ_2 | ADC12SHP;
    ADC12CTL3 = ADC12BATMAP;
    ADC12MCTL0 = ADC12INCH_31 | ADC12VRSEL_1 | ADC12WINC;
    ADC12HI = (uint16_t)(4096*((ADC_MONITOR_THRESHOLD+0.1)/2)/(2.0));
    ADC12LO = (uint16_t)(4096*(ADC_MONITOR_THRESHOLD/2)/(2.0));
    ADC12IFGR2 &= ~(ADC12HIIFG | ADC12LOIFG);
    ADC12IER2 = ADC12HIIE;
    ADC12CTL0 |= ADC12ENC;
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    switch(__even_in_range(ADC12IV, ADC12IV_ADC12LOIFG)) {
        case ADC12IV_NONE:        break;        // Vector  0: No interrupt
        case ADC12IV_ADC12OVIFG:  break;        // Vector  2: ADC12MEMx Overflow
        case ADC12IV_ADC12TOVIFG: break;        // Vector  4: Conversion time overflow
        case ADC12IV_ADC12HIIFG:                // Vector  6: Window comparator high side
            /* Disable the high side and enable the low side interrupt. */
            ADC12IER2 &= ~ADC12HIIE;
            ADC12IER2 |= ADC12LOIE;
            ADC12IFGR2 &= ~ADC12LOIFG;
            break;
        case ADC12IV_ADC12LOIFG:                // Vector  8: Window comparator low side
            /* Enter device shutdown with 64ms timeout. */
            ctpl_enterShutdown(CTPL_SHUTDOWN_TIMEOUT_64_MS);

            /* Disable the low side and enable the high side interrupt. */
            ADC12IER2 &= ~ADC12LOIE;
            ADC12IER2 |= ADC12HIIE;
            ADC12IFGR2 &= ~ADC12HIIFG;
            break;
        default: break;
    }
}
