/*******************************************************************************
 *
 *  adcTemp.c - This program allows the user to use 3 different ADC
 *              configurations to measure the temperature and Vcc.
 *              1. Software based - software poll and software trigger
 *              2. Timer Delay - software poll and hardware (Timer B) trigger
 *              3. Full Auto - hardware poll and hardware trigger
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
#include "adcTemp.h"

#define   STATUS_PASS (1)
#define   STATUS_FAIL (0)
#define   TEMP_STRING_LEN  (19)
#define   VCC_STRING_LEN  (11)
#define   V_ones      (6)
#define   V_tenths        (8)
#define   C_tens      (7)                                   // Temp_string[3]  = tens of deg C
#define   C_ones      (8)                                   // Temp_string[4]  = ones of deg C
#define   F_hundreds  (12)                                  // Temp_string[9]  = hundreds of deg F
#define   F_tens      (13)                                  // Temp_string[10] = tens of deg F
#define   F_ones      (14)                                  // Temp_string[11] = ones of deg F

#define ADC_MENU_MAX 2

static const char ADCText[] = {
    "ADC12 Power Demo \0"
    "1. Flag Poll Mode\0"
    "2. Interrupt Mode\0"
};

char Temp_string[TEMP_STRING_LEN] = "Temp = xx^C xxx^F\0";  //e.g. - "26^C  072^F"
char Vcc_string[VCC_STRING_LEN] = "Vcc = x.xV\0";           //e.g. - "Vcc = 3.0V"

int CAL_ADC_20T30, CAL_ADC_20T85;
float m = 0, c = 0;

void ADCTemp(void)
{
    unsigned char ADCmenuPos = 0;
    unsigned char quit = 0;
    unsigned int i;
    unsigned char backlightIsOn = halLcdGetBackLight();

    CAL_ADC_20T30 = (*(int *)0x01A1E);                      //ADC temp calibration consts for 2.0V
                                                            // ref
    CAL_ADC_20T85 = (*(int *)0x01A20);
    m = (85 - 30) / (float)(CAL_ADC_20T85 - CAL_ADC_20T30); //calculate the slope
    c = 85 - m * CAL_ADC_20T85;

    halButtonsInterruptDisable(BUTTON_ALL);
    halButtonsInterruptEnable(BUTTON_SELECT + BUTTON_UP + BUTTON_DOWN);
    halAccelerometerShutDown();
    halBoardStopOutputSystemClock();
    halLcdClearScreen();

    if (backlightIsOn){
        halLcdShutDownBackLight();
        halLcdPrintLine("Backlight is off\0", 7, OVERWRITE_TEXT);
        halLcdPrintLine(" to save power.\0", 8, OVERWRITE_TEXT);
    }

    TB0CTL &= ~TBIE;                                        // disable timer interrupts
    TB0CCTL0 &= ~CCIE;

    halLcdPrintLine((char *)ADCText, 0, 0);                 //display menu
    for (i = 0; i < 3; i++)
    {
        halLcdPrintLine((char *)&ADCText[i * 18 + 18], i + 1,       \
                        OVERWRITE_TEXT);
    }
    halLcdPrintLine((char *)&ADCText[ADCmenuPos * 18 + 18], ADCmenuPos + 1,       \
                    INVERT_TEXT | OVERWRITE_TEXT);

    buttonsPressed = 0;
    RTCCTL0 = 0;

    /* Initialize LFXT1 */
    P7SEL |= BIT1 + BIT0;                                   // Enable crystal pins
    LFXT_Start(XT1DRIVE_0);

    while (!quit)
    {
        if (buttonsPressed & BUTTON_SELECT)
        {
            quit = 1;
            buttonsPressed = 0;
        }
        else
        if (buttonsPressed & BUTTON_DOWN)
        {
            halLcdPrintLine((char *)&ADCText[ADCmenuPos * 18 + 18], ADCmenuPos + 1,      \
                            OVERWRITE_TEXT);
            if (ADCmenuPos == 1)
                ADCmenuPos = 0;
            else
                ADCmenuPos = 1;
            halLcdPrintLine((char *)&ADCText[ADCmenuPos * 18 + 18], ADCmenuPos + 1,       \
                            INVERT_TEXT | OVERWRITE_TEXT);
            buttonsPressed = 0;
        }
        else
        if (buttonsPressed & BUTTON_UP)
        {
            halLcdPrintLine((char *)&ADCText[ADCmenuPos * 18 + 18], ADCmenuPos + 1,      \
                            OVERWRITE_TEXT);
            if (ADCmenuPos == 0)
                ADCmenuPos = 1;
            else
                ADCmenuPos = 0;
            halLcdPrintLine((char *)&ADCText[ADCmenuPos * 18 + 18], ADCmenuPos + 1,       \
                            INVERT_TEXT | OVERWRITE_TEXT);
            buttonsPressed = 0;
        }

        buttonsPressed = 0;

        if (!quit)
            switch (ADCmenuPos)
            {
                case 0: SWBased(); break;
                case 1: FullAuto(); break;
                default: break;
            }
    }
    halAdcShutDownVcc();
    halAdcInitVcc();
    adcTempISR = 0;

    SFRIE1 &= ~OFIE;      //disable oscillator fault before configuring FLL because of DCOFFG
    Init_FLL_Settle(16000, 488);
    SFRIE1 |= OFIE;

    halLcdClearScreen();
    if (backlightIsOn){
        halLcdBackLightInit();
    }
    halLcdSetBackLight(lcdBackLightLevelSettingLOCAL);

    RTCCTL0 |= RTCRDYIE + RTCTEVIE;
}

//Software Poll, Software Trigger
void SWBased(void)
{
    unsigned int i = 0;

    SFRIE1 &= ~OFIE;                                    //disable oscillator fault before
                                                        // configuring FLL because of DCOFFG
    Init_FLL_Settle(1000, 31);
    SFRIE1 |= OFIE;

    /* Initialize the shared reference module */
    REFCTL0 &= ~REFVSEL_3;
    REFCTL0 |= REFVSEL_1;                               // Configure internal 2.0V reference

    // 5.4MHz (MODOSC_max) * 30 us (t_sensor)= 162 cycles min sampling time
    ADC12CTL0 &= ~ADC12ENC;                             // Ensure ENC is clear
    ADC12CTL0 = ADC12SHT0_7 + ADC12ON + ADC12MSC;       // Set sample time to 192 cycles
    ADC12CTL1 = ADC12CONSEQ_1 + ADC12SHP;               // Enable sequence-of-channels & sample
                                                        // timer
    ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_10;            // ADC input ch A10 => temp sense
    ADC12MCTL1 = ADC12SREF_1 + ADC12INCH_11 + ADC12EOS; // ADC input ch A11 => Vcc
    ADC12IE = 0x00;                                     // Ensure all interrupts are disabled

    while (!buttonsPressed)
    {
        REFCTL0 |= REFON;                               // Enable internal reference
        __delay_cycles(85);                             // Settling time for reference
        ADC12CTL0 |= ADC12ENC + ADC12SC;                // (Re)enable & trigger conversion

        // Poll IFG until ADC12MEM1 loaded signifying sequence is complete
        while (!(ADC12IFG & BIT1)) ;
        ADC12CTL0 &= ~ADC12ENC;                         // Disable conversions to configure REF
        REFCTL0 &= ~REFON;                              // Disable internal reference
        temp_temp = ADC12MEM0;
        temp_vcc = ADC12MEM1;

        if (buttonsPressed) break;                      //user input immediately stops operation
        printResults();

        i = 0;
        while ((i < 220) && !buttonsPressed)            //delay 1 second, but break on user input
        {
            __delay_cycles(10000);
            i++;
        }
    }
    SFRIE1 &= ~OFIE;                                    //disable oscillator fault before
                                                        // configuring FLL because of DCOFFG
    Init_FLL_Settle(16000, 488);
    SFRIE1 |= OFIE;
}

//Hardware Poll, Hardware Trigger
void FullAuto(void)
{
    SFRIE1 &= ~OFIE;                                    //disable oscillator fault before
                                                        // configuring FLL because of DCOFFG
    Init_FLL_Settle(1000, 31);
    SFRIE1 |= OFIE;

    /* Initialize the shared reference module */
    REFCTL0 &= ~REFVSEL_3;
    REFCTL0 |= REFVSEL_1 + REFON;                       // Configure internal 2.0V reference

    // 5.4MHz (MODOSC_max) * 30 us (t_sensor)= 162 cycles min sampling time
    ADC12CTL0 &= ~ADC12ENC;                             // Ensure ENC is clear
    ADC12CTL0 = ADC12SHT0_7 + ADC12ON + ADC12MSC;       // Set sample time to 192 cycles
    // Enable TB0.OUT1 as ADC trigger (ADC12SHS_x), enable sequence-of-channels
    ADC12CTL1 = ADC12SHS_3 + ADC12CONSEQ_1 + ADC12SHP;  // Enable sequence-of-channels & sample
                                                        // timer
    ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_10;            // ADC input ch A10 => temp sense
    ADC12MCTL1 = ADC12SREF_1 + ADC12INCH_11 + ADC12EOS; // ADC input ch A11 => Vcc
    ADC12IE = BIT1;                                     // Enable ADC12MEM1IFG interrupts
    ADC12CTL0 |= ADC12ENC;                              // Enable conversions

    TB0CCR0   = 65535;
    // Added margin for REF settling time
    TB0CCR1   = 5;                                      // 5 x 1/32768kHz
    TB0CCTL1  = OUTMOD_3;                               // CCR1 output mode to TB0CCR1
    TB0CTL    = TBSSEL_1 + MC_1 + TBCLR;                // ACLK, continuous mode, clear TBR
    TBCCTL0 = CCIE;
    adcTempISR = 1;

    while (!buttonsPressed)
    {
        // Bit-set (LPM3_bits + GIE) in SR register to enter LPM3 mode
        __bis_SR_register(LPM3_bits + GIE);
        __no_operation();
        if (buttonsPressed) break;                      //user input immediately stops operation
        printResults();
    }
    TB0CTL = 0;
    SFRIE1 &= ~OFIE;                                    //disable oscillator fault before
                                                        // configuring FLL because of DCOFFG
    Init_FLL_Settle(16000, 488);
    SFRIE1 |= OFIE;                                     //disable oscillator fault before
                                                        // configuring FLL because of DCOFFG
}

void format_voltage_string(unsigned int voltage)
{
    unsigned char ones = '0';
    unsigned char tenths = '0';

    while (voltage >= 10)
    {
        ones++;
        voltage -= 10;
    }
    tenths += voltage;

    Vcc_string[V_ones] = ones;
    Vcc_string[V_tenths] = tenths;
}

unsigned char format_temperature_string(unsigned int degrees, unsigned char c_or_f)
{
    unsigned char hundreds = '0';           // Init to '0' to code in ASCII
    unsigned char tens = '0';
    unsigned char ones = '0';
    unsigned char status = STATUS_PASS;

    while (degrees >= 100){                 // Calculate hundreds' place
        hundreds++;
        degrees -= 100;
    }
    if (hundreds == '0') hundreds = ' ';    //eliminate preceding zero, if any
    while (degrees >= 10){                  // Calculate tens' place
        tens++;
        degrees -= 10;
    }
    ones += degrees;                        // Remainder is ones' place

    switch (c_or_f)
    {
        case 'c':
        case 'C':
            Temp_string[C_tens] = tens;
            Temp_string[C_ones] = ones;
            break;
        case 'f':
        case 'F':
            Temp_string[F_hundreds]  = hundreds;
            Temp_string[F_tens]      = tens;
            Temp_string[F_ones]      = ones;
            break;
        default:
            status = STATUS_FAIL;           // Invalid parameter
            break;
    }
    return status;
}

void printResults(void)
{
    volatile unsigned long IntDegC, IntDegF;
    volatile unsigned long batt_voltage;

    // Temperature in Celsius
    // m and c calculated from calibration constants
    // y = mx + c, where y = temperature, x  = value from ADC conversion,
    // m = slope and c  = offset for calibration
    IntDegC = (unsigned long) (temp_temp * m + c);
    format_temperature_string(IntDegC, 'C');

    // Temperature in Fahrenheit
    // Tf = (9/5)*Tc + 32
    IntDegF = (unsigned long) (IntDegC * ((float)9 / 5) + 32);
    format_temperature_string(IntDegF, 'F');
    __no_operation();                     // SET BREAKPOINT HERE

    // Calculate voltage
    // 10 * Batt voltage = 2.0V(A11/4096) * 2 * 10 = 40(A11)/4096
    batt_voltage = (temp_vcc * 40) >> 12;
    format_voltage_string(batt_voltage);
    __no_operation();

    /*Display results on Lcd screen */

    halLcdPrintLine(Temp_string, 4, OVERWRITE_TEXT);
    halLcdPrintLine(Vcc_string, 5, OVERWRITE_TEXT);
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_ISR(void)
{
    REFCTL0 |= REFON;                           // Enable internal reference
    ADC12CTL0 |= ADC12ENC;                      // (Re)enable conversions
    __bic_SR_register_on_exit(LPM3_bits);
}

// ADC12 interrupt service routine found in file hal_adc.c
