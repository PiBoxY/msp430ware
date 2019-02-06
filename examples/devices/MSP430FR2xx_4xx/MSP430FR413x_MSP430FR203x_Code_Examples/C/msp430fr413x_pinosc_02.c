/* --COPYRIGHT--,BSD_EX
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
 *
 *******************************************************************************
 * 
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430FR413x Demo - Capacitive Touch, Pin Oscillator Method, 4-buttons
//
//  Description: Basic 4-button input using the built-in pin oscillation feature
//  on GPIO input structure. PinOsc signal feed into INCLK. WDT interval is used
//  to gate the measurements. Difference in measurements indicate button touch.
//  LED flash if input is touched.
//
//  Input 1: LED1 (LED2 off)
//  Input 2: LED2 (LED1 off)
//  Input 3: Both LEDs on
//  Input 4: Both LEDs flash on/off
//
//  ACLK = VLO = 32kHz, MCLK = SMCLK = 1MHz DCO
//
//               MSP430FR4133
//             -----------------
//         /|\|              XIN|-
//          | |                 | 
//          --|RST          XOUT|-
//            |                 |
//            |             P5.0|<--Capacitive Touch Input 1
//            |                 |
//  LED 2  <--|P1.2         P5.1|<--Capacitive Touch Input 2
//            |                 |
//  LED 1  <--|P1.0         P5.2|<--Capacitive Touch Input 3
//            |                 |
//            |             P5.3|<--Capacitive Touch Input 4
//
//  Zhu Ling
//  Texas Instruments Inc.
//  January 2014
//  Built with IAR Embedded Workbench v5.60 & Code Composer Studio v5.5
//******************************************************************************

#include <msp430.h>
/* Define User Configuration values */
/*----------------------------------*/
/* Defines WDT SMCLK interval for sensor measurements*/
#define WDT_meas_setting (DIV_SMCLK_512)
/* Defines WDT ACLK interval for delay between measurement cycles*/
#define WDT_delay_setting (DIV_ACLK_512)

/* Sensor settings*/
#define NUM_SEN     4                       // Defines number of sensors
#define KEY_LVL     220                     // Defines threshold for a key press
/*Set to ~ half the max delta expected*/

/* Definitions for use with the WDT settings*/
#define DIV_ACLK_32768  (WDT_ADLY_1000)     // ACLK/32768
#define DIV_ACLK_8192   (WDT_ADLY_250)      // ACLK/8192 
#define DIV_ACLK_512    (WDT_ADLY_16)       // ACLK/512 
#define DIV_ACLK_64     (WDT_ADLY_1_9)      // ACLK/64 
#define DIV_SMCLK_32768 (WDT_MDLY_32)       // SMCLK/32768
#define DIV_SMCLK_8192  (WDT_MDLY_8)        // SMCLK/8192 
#define DIV_SMCLK_512   (WDT_MDLY_0_5)      // SMCLK/512 
#define DIV_SMCLK_64    (WDT_MDLY_0_064)    // SMCLK/64 

#define LED_1   (BIT0)                      // P1.0 LED output
#define LED_2   (BIT2)                      // P1.2 LED output

// Global variables for sensing
unsigned int base_cnt[NUM_SEN];
unsigned int meas_cnt[NUM_SEN];
int delta_cnt[NUM_SEN];
unsigned char key_press[NUM_SEN];
char key_pressed;
const unsigned char electrode_bit[NUM_SEN]={BIT0, BIT1, BIT2, BIT3};
/* System Routines*/
void measure_count(void);                   // Measures each capacitive sensor
void pulse_LED(void);                       // LED gradient routine
void pinosc_channel_select(unsigned char PORT, unsigned char PIN);

int main( void )
{
    unsigned int i,j;
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    SFRIE1 |= WDTIE;                          // enable WDT interrupt
    P1DIR = LED_1 + LED_2;                    // P1.0 & P1.2 = LEDs
    P1OUT = LED_1 + LED_2;
    P1OUT = 0x00;                             

    __bis_SR_register(GIE);                   // Enable interrupts        

    measure_count();                          // Establish baseline capacitance
    for (i = 0; i<NUM_SEN; i++)
        base_cnt[i] = meas_cnt[i];
    
    for(i=15; i>0; i--)                       // Repeat and avg base measurement
    { 
        measure_count();
        for (j = 0; j<NUM_SEN; j++)
            base_cnt[j] = (meas_cnt[j]+base_cnt[j])/2;
    }
    
    /* Main loop starts here*/
    while (1)
    {
        j = KEY_LVL;
        key_pressed = 0;                        // Assume no keys are pressed
        
        measure_count();                        // Measure all sensors
        
        for (i = 0; i<NUM_SEN; i++)
        { 
            delta_cnt[i] = base_cnt[i] - meas_cnt[i];  // Calculate delta: c_change
            
            /* Handle baseline measurment for a base C decrease*/
            if (delta_cnt[i] < 0)                 // If negative: result increased
            {                                     // beyond baseline, i.e. cap dec
                base_cnt[i] = (base_cnt[i]+meas_cnt[i]) >> 1; // Re-average quickly
                delta_cnt[i] = 0;                 // Zero out for pos determination
            }
            if (delta_cnt[i] > j)                 // Determine if each key is pressed 
            {                                     // per a preset threshold
                key_press[i] = 1;                   // Specific key pressed
                j = delta_cnt[i];
                key_pressed = i+1;                  // key pressed
            }
            else
                key_press[i] = 0;
        }
        
        /* Delay to next sample */
        WDTCTL = WDT_delay_setting;             // WDT, ACLK, interval timer
        
        /* Handle baseline measurment for a base C increase*/
        if (!key_pressed)                       // Only adjust baseline down 
        {                                       // if no keys are touched
            for (i = 0; i<NUM_SEN; i++)
                base_cnt[i] = base_cnt[i] - 1;      // Adjust baseline down, should be 
        }                                       // slow to accomodate for genuine 
        pulse_LED();                           // changes in sensor C
        
        __bis_SR_register(LPM3_bits);
    }
}                                           // End Main

/* Measure count result (capacitance) of each sensor*/
/* Routine setup for four sensors, not dependent on NUM_SEN value!*/

void measure_count(void)
{ 
    unsigned char i;
    TA0CTL = TASSEL_3 + MC_2;                   // TACLK, cont mode
    TA0CCTL1 = CM_3 + CCIS_2 + CAP;             // Pos&Neg,GND,Cap

    for (i = 0; i<NUM_SEN; i++)
    {
        /*Configure Ports for relaxation oscillator*/
        pinosc_channel_select(5, electrode_bit[i]);
        
        /*Setup Gate Timer*/
        WDTCTL = WDT_meas_setting;              // WDT, ACLK, interval timer
        TA0CTL |= TACLR;                        // Clear Timer_A TAR
        __bis_SR_register(LPM0_bits+GIE);       // Wait for WDT interrupt
        TA0CCTL1 ^= CCIS0;                      // Create SW capture of CCR1
        meas_cnt[i] = TA0CCR1;                  // Save result
        WDTCTL = WDTPW + WDTHOLD;               // Stop watchdog timer
        CAPSIO0CTL = 0;
    }
}

void pinosc_channel_select(unsigned char PORT, unsigned char PIN)
{
    CAPSIO0CTL = 0;
    switch(PORT)
    {
    case 1: CAPSIO0CTL |= CAPSIOPOSEL0; break;
    case 2: CAPSIO0CTL |= CAPSIOPOSEL1; break;
    case 3: CAPSIO0CTL |= CAPSIOPOSEL0 + CAPSIOPOSEL1; break;
    case 4: CAPSIO0CTL |= CAPSIOPOSEL2; break;
    case 5: CAPSIO0CTL |= CAPSIOPOSEL0 + CAPSIOPOSEL2; break;
    case 6: CAPSIO0CTL |= CAPSIOPOSEL1 + CAPSIOPOSEL2; break;
    case 7: CAPSIO0CTL |= CAPSIOPOSEL0 + CAPSIOPOSEL1 + CAPSIOPOSEL2; break;
    case 8: CAPSIO0CTL |= CAPSIOPOSEL3; break;
    case 9: CAPSIO0CTL |= CAPSIOPOSEL0 + CAPSIOPOSEL3; break;
    case 10: CAPSIO0CTL |= CAPSIOPOSEL1 + CAPSIOPOSEL3; break;
    case 11: CAPSIO0CTL |= CAPSIOPOSEL0 + CAPSIOPOSEL1 + CAPSIOPOSEL3; break;
    case 12: CAPSIO0CTL |= CAPSIOPOSEL2 + CAPSIOPOSEL3; break;
    case 13: CAPSIO0CTL |= CAPSIOPOSEL0 + CAPSIOPOSEL2 + CAPSIOPOSEL3; break;
    case 14: CAPSIO0CTL |= CAPSIOPOSEL1 + CAPSIOPOSEL2 + CAPSIOPOSEL3; break;
    case 15: CAPSIO0CTL |= CAPSIOPOSEL0 +CAPSIOPOSEL1 + CAPSIOPOSEL2 + CAPSIOPOSEL3; break;
    default: break;
    }
    switch(PIN)
    {
    case BIT0: break;
    case BIT1: CAPSIO0CTL |= CAPSIOPISEL0; break;
    case BIT2: CAPSIO0CTL |= CAPSIOPISEL1; break;
    case BIT3: CAPSIO0CTL |= CAPSIOPISEL0 + CAPSIOPISEL1; break;
    case BIT4: CAPSIO0CTL |= CAPSIOPISEL2; break;
    case BIT5: CAPSIO0CTL |= CAPSIOPISEL0 + CAPSIOPISEL2; break;
    case BIT6: CAPSIO0CTL |= CAPSIOPISEL1 + CAPSIOPISEL2; break;
    case BIT7: CAPSIO0CTL |= CAPSIOPISEL0 + CAPSIOPISEL1 + CAPSIOPISEL2; break;
    default: break;
    }
    CAPSIO0CTL |= CAPTIOEN;
}

void pulse_LED(void)
{ 
    switch (key_pressed)
        {
    case 0: P1OUT &= ~(LED_1 + LED_2); break;
    case 1: P1OUT = LED_1; break;
    case 2: P1OUT = LED_2; break;
    case 3: P1OUT = LED_1 + LED_2; break;
    case 4: P1OUT ^= LED_1 + LED_2; break;
    }
}
/* Watchdog Timer interrupt service routine*/
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) watchdog_timer (void)
#else
#error Compiler not supported!
#endif
{
    TA0CCTL1 ^= CCIS0;                        // Create SW capture of CCR1
    __bic_SR_register_on_exit(LPM3_bits);     // Exit LPM3 on reti
}
