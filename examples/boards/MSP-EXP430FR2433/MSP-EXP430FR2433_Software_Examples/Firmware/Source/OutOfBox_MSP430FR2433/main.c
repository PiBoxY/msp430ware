/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
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
//  Out of box demo for the MSP-EXP430FR2433
//
//  This demo uses the MSP432FR2433's internal Temperature Sensor and demonstrates
//  how to setup a periodic temperature data logger, by utilizing a ring-buffer
//  inside the MSP430F2433 device's FRAM memory. In addition, the demo also implements
//  a real time temperature sensor
//
//
//  E. Chen
//  Texas Instruments Inc.
//  October 2017
//******************************************************************************

#include "FRAMLogMode.h"
#include "LiveTempMode.h"

// ADC data
adc_data_t adc_data;

// NVS ring handle
nvs_ring_handle nvsHandle;

// FRAM storage for ADC samples using NVS ring storage
#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(nvsStorage)
#elif defined(__IAR_SYSTEMS_ICC__)
__persistent
#endif
uint8_t nvsStorage[NVS_RING_STORAGE_SIZE(sizeof(adc_data_t), NVS_RING_SIZE)] = {0};

bool buttonS1Pressed;
bool buttonS2Pressed;
bool rtcWakeup;

// Application mode, selected between FRAM_LOG_MODE and LIVE_TEMP_MODE
char mode;

bool rxStringReady = false;
bool rxThreshold = false;
char rxString[MAX_STRBUF_SIZE];

#ifdef RECEIVE_JSON
#include <jsmn.h>

jsmn_parser p;
jsmntok_t t[5]; /* We expect no more than 5 tokens */
#endif

// UART Defines
#define UART_TXD_PORT        GPIO_PORT_P1
#define UART_TXD_PIN         GPIO_PIN4
#define UART_RXD_PORT        GPIO_PORT_P1
#define UART_RXD_PIN         GPIO_PIN5
#define UART_SELECT_FUNCTION GPIO_PRIMARY_MODULE_FUNCTION

// Function Definitions
void initGpio(void);
void initCs(void);
void initRtc(void);
void initAdc(void);
void initEusci(void);
void UART_receiveString(char);
#ifdef RECEIVE_JSON
static int jsoneq(const char *, jsmntok_t *, const char *);
#endif

int main(void)
{
    /* Halt the watchdog timer */
    WDT_A_hold(WDT_A_BASE);

    /* Initialize GPIO and RTC */
    initGpio();
    initCs();
    initRtc();
    initAdc();
    initEusci();

    // Check integrity of NVS container and initialize if required;
    nvsHandle = nvs_ring_init(nvsStorage, sizeof(adc_data_t), NVS_RING_SIZE);

    /* Toggle P1.0 LED to indicated device start up. */
    P1OUT ^= BIT1;
    __delay_cycles(16000000);
    P1OUT ^= BIT1;

    P2IFG &= ~(BIT3 | BIT7);

    mode = 0;
    buttonS1Pressed = false;
    buttonS2Pressed = false;
    rtcWakeup = false;

    /* Enable global interrupts. */
    __enable_interrupt();

    /*
     * Enter into LPM3.5 with restore on reset disabled. The RTC interrupt will
     * toggle P2.0 each time the device wakes up.
     */
    while (1) {
        switch (mode)
        {
            case 0:
                framLog();
                break;
            case 1:
                thresholdChanged = true;
                liveTemp();
                break;
            default:
                break;
        }
    }
}

void initGpio(void)
{
    /* Configure GPIO to Output LOW state */
    PADIR |= 0xFFFF;
    P3DIR |= 0xFF;
    PAOUT = 0x0000;
    P3OUT = 0x00;

    P2OUT |= BIT3 | BIT7;       // Configure P2.3 P2.7 pull-up
    P2DIR &= ~(BIT3 | BIT7);    // Configure P2.3 P2.7 as input
    P2REN |= BIT3 | BIT7;       // P2.3 P2.7 Enable pull-up resistors
    P2IES |= BIT3 | BIT7;       // P2.3 P2.7 Hi/Lo edge
    P2IE |= BIT3 | BIT7;        // P2.3 P2.7 interrupt enabled

    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings.
     */
    PM5CTL0 &= ~LOCKLPM5;

    P2IFG &= ~(BIT3 | BIT7);    // Clear P2.3 P2.7 IFG
}

void initCs(void)
{
    //Set DCO FLL reference = REFO
    CS_initClockSignal(
        CS_FLLREF,
        CS_REFOCLK_SELECT,
        CS_CLOCK_DIVIDER_1
        );

    //Set ACLK = REFO
    CS_initClockSignal(
        CS_ACLK,
        CS_REFOCLK_SELECT,
        CS_CLOCK_DIVIDER_1
        );

    //Create struct variable to store proper software trim values
    CS_initFLLParam param = {0};

    //Set Ratio/Desired MCLK Frequency, initialize DCO, save trim values
    CS_initFLLCalculateTrim(
        16000,
        488,
        &param
        );

    //Clear all OSC fault flag
    CS_clearAllOscFlagsWithTimeout(1000);

    //For demonstration purpose, change DCO clock freq to 16MHz
    CS_initFLLSettle(
        16000,
        488
        );
}

void initRtc(void)
{
    //Initialize RTC, interrupts roughly every 5 seconds
    RTCMOD = 50;
    RTCCTL = RTCSS__VLOCLK | RTCSR |RTCPS__1024;
    RTCCTL |= RTCIE;
}

void initAdc(void)
{
    //Initialize the ADC Module
    /*
     * Base Address for the ADC Module
     * Use internal ADC bit as sample/hold signal to start conversion
     * USE MODOSC 5MHZ Digital Oscillator as clock source
     * Use default clock divider of 1
     */
    ADC_init(ADC_BASE,
             ADC_SAMPLEHOLDSOURCE_SC,
             ADC_CLOCKSOURCE_ADCOSC,
             ADC_CLOCKDIVIDER_1);

    ADC_enable(ADC_BASE);

    /*
     * Base Address for the ADC Module
     * Sample/hold for 256 clock cycles
     * Do not enable Multiple Sampling
     */
    ADC_setupSamplingTimer(ADC_BASE,
                           ADC_CYCLEHOLD_1024_CYCLES,
                           ADC_MULTIPLESAMPLESDISABLE);

    //Configure the Memory Buffer
    /*
     * Base Address for the ADC Module
     * Use input A12
     * Use positive reference of AVcc
     * Use negative reference of AVss
     */
    ADC_configureMemory(ADC_BASE,
                        ADC_INPUT_TEMPSENSOR,
                        ADC_VREFPOS_INT,
                        ADC_VREFNEG_AVSS);

    ADC_clearInterrupt(ADC_BASE,
                       ADC_COMPLETED_INTERRUPT);

    //Enable the Memory Buffer Interrupt
    ADC_enableInterrupt(ADC_BASE,
                        ADC_COMPLETED_INTERRUPT);

    //Configure internal reference
    //If ref voltage no ready, WAIT
    while(PMM_REFGEN_NOTREADY ==
          PMM_getVariableReferenceVoltageStatus());

    PMM_enableTempSensor();

    //Internal Reference ON
    PMM_enableInternalReference();

//    __delay_cycles(500);
}

// Initialize EUSCI
void initEusci(void)
{
    // Configure UCA1TXD and UCA1RXD
    P1SEL0 |= BIT4 | BIT5;
    P1SEL1 &= ~(BIT4 | BIT5);

    // Configure UART
    // http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
    EUSCI_A_UART_initParam param = {0};
    param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    param.clockPrescalar = 8;
    param.firstModReg = 10;
    param.secondModReg = 247;
    param.parity = EUSCI_A_UART_NO_PARITY;
    param.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    param.uartMode = EUSCI_A_UART_MODE;
    param.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;

    if(STATUS_FAIL == EUSCI_A_UART_init(EUSCI_A0_BASE, &param))
    {
        return;
    }

    EUSCI_A_UART_enable(EUSCI_A0_BASE);

    EUSCI_A_UART_clearInterrupt(EUSCI_A0_BASE,
                                EUSCI_A_UART_RECEIVE_INTERRUPT);

    // Enable USCI_A0 RX interrupt
    EUSCI_A_UART_enableInterrupt(EUSCI_A0_BASE,
                                 EUSCI_A_UART_RECEIVE_INTERRUPT);      // Enable interrupt
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(PORT2_VECTOR)))
#endif
void Port2_ISR(void)
{
    // Both buttons pressed
    if (!(P2IN & BIT3) && !(P2IN & BIT7))
    {
        P2IES |= BIT3 | BIT7;       // P2.3 P2.7 Hi/Lo edge
        P2IFG &= ~(BIT3 | BIT7);    // Clear P2.3 P2.7 IFG

        buttonS1Pressed = false;
        buttonS2Pressed = false;

        // Switch mode
        if (mode == FRAM_LOG_MODE)
            mode = LIVE_TEMP_MODE;
        else
            mode = FRAM_LOG_MODE;

        return;
    }
    // Left button S1
    if (P2IFG & BIT3)
    {
        P2IFG &= ~BIT3;

        // Left button S1 released
        if (P2IN & BIT3)
        {
            P2IES |= BIT3;       // P2.3 Hi/Lo edge
            buttonS1Pressed = true;

            if (mode == LIVE_TEMP_MODE && threshold > _Q8(-127))
            {
                threshold = threshold - _Q8(0.5);
                thresholdChanged = true;
            }
        }
        else
            P2IES &= ~BIT3;       // P2.3 Lo/Hi edge
    }
    // Right button S2
    if (P2IFG & BIT7)
    {
        P2IFG &= ~BIT7;

        // Right button S2 released
        if (P2IN & BIT7)
        {
            P2IES |= BIT7;       // P2.7 Hi/Lo edge
            buttonS2Pressed = true;

            if (mode == LIVE_TEMP_MODE && threshold < _Q8(127))
            {
                threshold = threshold + _Q8(0.5);
                thresholdChanged = true;
            }
        }
        else
            P2IES &= ~BIT7;       // P2.7 Lo/Hi edge
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=RTC_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(RTC_VECTOR)))
#endif
void RTC_ISR(void)
{
    switch(__even_in_range(RTCIV, RTCIV_RTCIF)) {
        case RTCIV_NONE : break;
        case RTCIV_RTCIF:
            // Turn on LED1 P1.0
            P1OUT |= BIT0;
            rtcWakeup = true;

            break;
        default: break;
    }
}

//ADC interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(ADC_VECTOR)))
#endif
void ADC_ISR(void)
{
    switch(__even_in_range(ADCIV,12))
    {
        case ADCIV_NONE: break;       //No interrupt
        case ADCIV_ADCOVIFG: break;   //conversion result overflow
        case ADCIV_ADCTOVIFG: break;  //conversion time overflow
        case ADCIV_ADCHIIFG: break;   //ADC10HI
        case ADCIV_ADCLOIFG: break;   //ADC10LO
        case ADCIV_ADCINIFG: break;   //ADC10IN
        case ADCIV_ADCIFG:            //ADC10IFG0
            if (mode == FRAM_LOG_MODE)
                adc_data.value = ADC_getResults(ADC_BASE);
            else
                ADCIFG &= ~ADCIFG0;
            //Clear LPM3 bits from 0(SR)
            __bic_SR_register_on_exit(LPM3_bits);
            break;
        default: break;
    }
}

// EUSCI interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(UCA0IV,USCI_UART_UCTXCPTIFG))
    {
        case USCI_NONE: break;
        case USCI_UART_UCRXIFG:
            // Read buffer
            UART_receiveString(UCA0RXBUF);
#ifdef RECEIVE_JSON
            if (rxStringReady)
            {
                int i;
                int r;
                jsmn_init(&p);
                r = jsmn_parse(&p, rxString, strlen(rxString), t, sizeof(t)/sizeof(t[0]));

                // Loop over keys of JSON object
                for (i = 1; i < r; i++) {
                    if (jsoneq(rxString, &t[i], "tempThreshold") == 0) {
                        // New Temperature Threshold received from PC
                        char test[6];
                        strncpy(test, rxString+t[i+1].start, t[i+1].end-t[i+1].start);
                        threshold = _Q8(strtof(test, NULL));
                        i++;
                    }
                }

                rxStringReady = false;
            }
#else
            if (rxThreshold)
            {
                if (rxStringReady)
                {
                    threshold = _Q8(strtof(rxString, NULL));
                    rxThreshold = false;
                    rxStringReady = false;
                }
            }
            if (rxStringReady && !rxThreshold)
            {
                if (strcmp(rxString,"THRESH")==0)
                {
                    rxThreshold = true;
                }
                rxStringReady = false;
            }
#endif
            break;
        case USCI_UART_UCTXIFG: break;
        case USCI_UART_UCSTTIFG: break;
        case USCI_UART_UCTXCPTIFG: break;
        default: break;
    }
}

// Receives strings terminated with \n
void UART_receiveString(char data) {
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
            if (charCnt >= MAX_STRBUF_SIZE){
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

// Transmits string buffer through EUSCI UART
void transmitString(char *str)
{
    int i = 0;
    for(i = 0; i < strlen(str); i++)
    {
        if (str[i] != 0)
        {
            // Transmit Character
            while (EUSCI_A_UART_queryStatusFlags(EUSCI_A0_BASE, EUSCI_A_UART_BUSY));
            EUSCI_A_UART_transmitData(EUSCI_A0_BASE, str[i]);
        }
    }
}

#ifdef RECEIVE_JSON
// Compare JSON keys
static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
            strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}
#endif
