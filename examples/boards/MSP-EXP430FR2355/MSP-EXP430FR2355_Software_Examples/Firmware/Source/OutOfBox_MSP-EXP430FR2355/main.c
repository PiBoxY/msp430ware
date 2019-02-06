/* --COPYRIGHT--,BSD
 * Copyright (c) 2018, Texas Instruments Incorporated
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
//******************************************************************************
//
//  main.c
//
//  Out of box demo for the MSP-EXP430FR2355 LaunchPad
//
//  This demo uses the MSP432FR2355's four built-in Smart Analog Combo (SAC) peripherals
//  to demonstrate interconnections between SAC pairs and the flexibility for
//  various configurations.
//   - Two SACs are used to implement an ambient light sensor with LED feedback
//   - Two SACs are used to implement a simple function generator and a user controllable
//     Programmable Gain Amplifier (PGA)
//
//  E. Chen
//  Texas Instruments Inc.
//  May 2018
//******************************************************************************

#include <driverlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jsmn.h>

#include "lightsensor.h"
#include "functiongenerator.h"

/* Constants */
#define MAX_STR_LEN         256

/* Global Variables */
#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(mode)
#elif defined(__IAR_SYSTEMS_ICC__)
__persistent
#endif
char mode = LIGHTSENSOR_MODE;

/* UART variables */
jsmn_parser p;
jsmntok_t t[64]; /* We expect no more than 64 tokens */
bool rxStringReady = false;
char rxString[MAX_STR_LEN];
char txString[MAX_STR_LEN];

/* Function Declarations */
void init_GPIO(void);
void init_CS(void);
void init_EUSCI(void);

void transmitString(char *);
void receiveString(char);
static int jsoneq(const char *, jsmntok_t *, const char *);

/**
 * main.c
 */
int main(void)
{
    /* Stop watchdog timer */
    WDT_A_hold(WDT_A_BASE);

    init_GPIO();
    init_CS();
    init_EUSCI();
	
    while(1)
    {
        switch (mode)
        {
            case LIGHTSENSOR_MODE:
                // Lightsensor Demo
                lightsensor();
                break;
            case FUNCTIONGENERATOR_MODE:
                // DSP Demo
                functiongenerator();
                break;
            default:
                mode = 0;
                break;
        }
    }
}

void init_GPIO(void) {
    /* Initialize all GPIO to output low for minimal LPM power consumption */
    GPIO_setAsOutputPin(GPIO_PORT_PA, GPIO_PIN_ALL16);
    GPIO_setAsOutputPin(GPIO_PORT_PB, GPIO_PIN_ALL16);
    GPIO_setAsOutputPin(GPIO_PORT_PC, GPIO_PIN_ALL16);
    GPIO_setAsOutputPin(GPIO_PORT_PD, GPIO_PIN_ALL16);
    GPIO_setAsOutputPin(GPIO_PORT_PE, GPIO_PIN_ALL16);

    GPIO_setOutputLowOnPin(GPIO_PORT_PA, GPIO_PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_PB, GPIO_PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_PC, GPIO_PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_PD, GPIO_PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_PE, GPIO_PIN_ALL16);

    //Set P2.3 (S3) to input
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN3);
    GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN3, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN3);

    PMM_unlockLPM5();

    GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN3);
}

void init_CS(void) {
    // Configure two FRAM waitstate as required by the device datasheet for MCLK
    // operation at 24MHz(beyond 8MHz) _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_2 ;

    P2SEL1 |= BIT6 | BIT7;                       // P2.6~P2.7: crystal pins
    do
    {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);           // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);                   // Test oscillator fault flag

    __bis_SR_register(SCG0);                     // disable FLL
    CSCTL3 |= SELREF__XT1CLK;                    // Set XT1 as FLL reference source
    CSCTL0 = 0;                                  // clear DCO and MOD registers
    CSCTL1 = DCORSEL_7;                         // Set DCO = 24MHz
    CSCTL2 = FLLD_0 + 731;                       // DCOCLKDIV = 24MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                     // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));   // FLL locked

    CSCTL4 = SELMS__DCOCLKDIV | SELA__XT1CLK;   // set XT1 (~32768Hz) as ACLK source, ACLK = 32768Hz
                                                 // default DCOCLKDIV as MCLK and SMCLK source
    
    P3DIR |= BIT4;
    P3SEL0 |= BIT4;
    P3SEL1 &= ~BIT4;
}

// Initialize EUSCI
void init_EUSCI(void)
{
    // Configure UCA1TXD and UCA1RXD
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P4, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    // Configure UART
    // ClockSource = SMCLK = 24MHz, Baudrate = 115200bps
    // http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
    EUSCI_A_UART_initParam param = {0};
    param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    param.clockPrescalar = 13;
    param.firstModReg = 0;
    param.secondModReg = 37;
    param.parity = EUSCI_A_UART_NO_PARITY;
    param.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    param.uartMode = EUSCI_A_UART_MODE;
    param.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;

    if(STATUS_FAIL == EUSCI_A_UART_init(EUSCI_A1_BASE, &param))
    {
        return;
    }

    EUSCI_A_UART_enable(EUSCI_A1_BASE);

    EUSCI_A_UART_clearInterrupt(EUSCI_A1_BASE,
                                EUSCI_A_UART_RECEIVE_INTERRUPT);

    // Enable USCI_A0 RX interrupt
    EUSCI_A_UART_enableInterrupt(EUSCI_A1_BASE,
                                 EUSCI_A_UART_RECEIVE_INTERRUPT);      // Enable interrupt
}

// ADC interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC_VECTOR))) ADC_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG))
    {
        case ADCIV_NONE:
            break;
        case ADCIV_ADCOVIFG:
            break;
        case ADCIV_ADCTOVIFG:
            break;
        case ADCIV_ADCHIIFG:
            break;
        case ADCIV_ADCLOIFG:
            break;
        case ADCIV_ADCINIFG:
            break;
        case ADCIV_ADCIFG:
            if (mode == 0) {
                lightsensor_ADC_Result = ADCMEM0;
                __bic_SR_register_on_exit(LPM3_bits);              // Sleep Timer Exits LPM3
            }
            else {
                functiongenerator_ADC_Result = ADCMEM0;
                __bic_SR_register_on_exit(LPM0_bits);            // Clear CPUOFF bit from LPM0
            }
            break;
        default:
            break;
    }
}

// EUSCI interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A1_VECTOR))) USCI_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(UCA1IV,USCI_UART_UCTXCPTIFG))
    {
        case USCI_NONE: break;
        case USCI_UART_UCRXIFG:
            functiongenerator_disable();
            Timer_B_stop(TIMER_B0_BASE);
            // Read buffer
            receiveString(UCA1RXBUF);

            if (rxStringReady)
            {
                int i;
                int r;
                jsmn_init(&p);
                r = jsmn_parse(&p, rxString, strlen(rxString), t, sizeof(t)/sizeof(t[0]));

                // Loop over keys of JSON object
                for (i = 1; i < r; i++) {
                    if (jsoneq(rxString, &t[i], "inputSignalSource") == 0) {
                        // input signal source received from PC
                        inputSignalSource = strtol( rxString+t[i+1].start, NULL, 10);
                        if (inputSignalSource == 1)
                            opAmpMode = 0;
                        else if (inputSignalSource == 2)
                            opAmpMode = 1;
                        i++;
                    }
                    if (jsoneq(rxString, &t[i], "signalType") == 0) {
                        // New Signal Frequency received from PC
                        signalType = strtol( rxString+t[i+1].start, NULL, 10);
                        i++;
                    }
                    if (jsoneq(rxString, &t[i], "signalFreq") == 0) {
                        // New Signal Frequency received from PC
                        signalFreq = strtol( rxString+t[i+1].start, NULL, 10);
                        i++;
                    }
                    if (jsoneq(rxString, &t[i], "signalAmp") == 0) {
                        // New Signal Amplitude received from PC
                        signalAmp = strtol( rxString+t[i+1].start, NULL, 10);
                        i++;
                    }
                    if (jsoneq(rxString, &t[i], "dacFreq") == 0) {
                        // New DAC Sample Frequency received from PC
                        dacFreq = strtol( rxString+t[i+1].start, NULL, 10);
                        i++;
                    }
                    if (jsoneq(rxString, &t[i], "adcFreq") == 0) {
                        // New ADC Sample Frequency received from PC
                        adcFreq = strtol( rxString+t[i+1].start, NULL, 10);
                        i++;
                    }
                    if (jsoneq(rxString, &t[i], "opAmpMode") == 0) {
                        // New ADC Sample Frequency received from PC
                        opAmpMode = strtol( rxString+t[i+1].start, NULL, 10);
                        i++;
                    }
                    if (jsoneq(rxString, &t[i], "opAmpGain") == 0) {
                        // New ADC Sample Frequency received from PC
                        opAmpGain = strtol( rxString+t[i+1].start, NULL, 10);
                        i++;
                    }
                    if (jsoneq(rxString, &t[i], "mode") == 0) {
                        mode = strtol( rxString+t[i+1].start, NULL, 10);
                        __bic_SR_register_on_exit(LPM3_bits);
                        i++;
                    }
                }

                if (mode == FUNCTIONGENERATOR_MODE)
                    functiongenerator_enable();
                if (mode == LIGHTSENSOR_MODE) {
                    Timer_B_startCounter(TIMER_B0_BASE, TIMER_B_UP_MODE);
                    lightsensor_init_ADC();
                }

                rxStringReady = false;
            }
            break;
        case USCI_UART_UCTXIFG: break;
        case USCI_UART_UCSTTIFG: break;
        case USCI_UART_UCTXCPTIFG: break;
        default: break;
    }
}

// Transmits string buffer through EUSCI UART
void transmitString(char *str)
{
    unsigned int i = 0;
    for(i = 0; i < strlen(str); i++)
    {
        if (str[i] != 0)
        {
            // Transmit Character
            while (EUSCI_A_UART_queryStatusFlags(EUSCI_A1_BASE, EUSCI_A_UART_BUSY));
            EUSCI_A_UART_transmitData(EUSCI_A1_BASE, str[i]);
        }
    }
}

// Receives strings terminated with \n
void receiveString(char data) {
    static bool rxInProgress = false;
    static unsigned int charCnt = 0;

    if(!rxInProgress){
        if ((data != '\n') ){
            rxInProgress = true;
            charCnt = 0;
            rxString[charCnt] = data;
        }
    }else{ // in progress
        charCnt++;
        if((data != '\n')){
            if (charCnt >= MAX_STR_LEN){
                rxInProgress = false;
            }else{
                rxString[charCnt] = data;
            }
        }else{
            rxInProgress = false;
            rxString[charCnt] = '\0';
            // String receive complete
            rxStringReady = true;
        }
    }
}

// Compare JSON keys
static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
            strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) PORT2_ISR (void)
#else
#error Compiler not supported!
#endif
{
    // Clear P2.3 IFG
    GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN3);

    functiongenerator_disable();
    Timer_B_stop(TIMER_B0_BASE);

    mode ^= 1;

    if (mode == FUNCTIONGENERATOR_MODE)
        functiongenerator_enable();
    if (mode == LIGHTSENSOR_MODE) {
        Timer_B_startCounter(TIMER_B0_BASE, TIMER_B_UP_MODE);
        lightsensor_init_ADC();
    }

    __bic_SR_register_on_exit(LPM3_bits);
}
