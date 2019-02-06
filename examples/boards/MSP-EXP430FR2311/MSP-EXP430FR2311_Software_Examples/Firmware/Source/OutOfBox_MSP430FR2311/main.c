/* --COPYRIGHT--,BSD
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
 * --/COPYRIGHT--*/
/*******************************************************************************
 *
 * main.c
 *
 * Out of Box Demo for the MSP-EXP430FR2311 Light sensing demo with FRAM write
 *
 * Main loop, initialization, and interrupt service routines
 *
 *   Description: Sample Op-Amp output with ADC10, PWM LED using Timer1 B1
 *   with ADC value, store current ADC value to FRAM and enter LPM3, wake up
 *   every ~50ms to start a new ADC conversion.
 *   NOTE: Running this example for extended periods will impact the FRAM
 *   endurance.
 *   ACLK = REFO= ~32768kHz, MCLK = SMCLK = ~1.0MHz
 *
 *   Tested On: MSP430FR2311
 *            -----------------
 *        /|\|                 |
 *         | |        P1.2/OA0-|<---External Pad
 *         --|RST     P1.3/OA0O|--->A3(Internal)
 *           |        P1.4/OA0+|<---External Pad
 *           |                 |
 *           |       P2.0/TB1.1|---> LED
 *           |                 |
 *
 * December 2015
 * J. Collins
 *
 ******************************************************************************/

#include "driverlib.h"

void initPWM(void);
void initSleepTimer(uint16_t);
void enable_SACOA(void);
void disable_SACOA(void);
void enable_ADC10(void);
void disable_ADC10(void);
void lightSample(void);
void initGPIO(void);

/* FRAM Variable that stores ADC results*/
#pragma PERSISTENT(ADC_Conversion_Result)
int ADC_Conversion_Result = 0;			// ADC conversion result

/*
 * main.c
 */
int main(void) {
    //Stop WDT
    WDT_A_hold(WDT_A_BASE);

/* Initialize peripherals */
    initGPIO();
    initPWM();

    //Toggle LED1 during start-up
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    __delay_cycles(10000);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    while(1){
    	lightSample();

		//Initialize sleep timer
		initSleepTimer(1638);// 1638/32768 = ~50ms and (65535/32768) = ~2sec

        //LPM3, SleepTimer will force exit
        __bis_SR_register(LPM3_bits + GIE);
	}

}

void initPWM(void){
	//Configure compare register latch load event
	Timer_B_initCompareLatchLoadEvent(TIMER_B1_BASE, TIMER_B_CAPTURECOMPARE_REGISTER_1, TIMER_B_LATCH_WHEN_COUNTER_COUNTS_TO_CURRENT_COMPARE_LATCH_VALUE);

	//Start timer
	Timer_B_initUpModeParam param = {0};
	param.clockSource = TIMER_B_CLOCKSOURCE_ACLK;
	param.clockSourceDivider = TIMER_B_CLOCKSOURCE_DIVIDER_1;
	param.timerPeriod = 256;
	param.timerInterruptEnable_TBIE = TIMER_B_TBIE_INTERRUPT_DISABLE;
	param.captureCompareInterruptEnable_CCR0_CCIE =
		TIMER_B_CCIE_CCR0_INTERRUPT_DISABLE;
	param.startTimer = true;
	Timer_B_initUpMode(TIMER_B1_BASE, &param);

	Timer_B_initCompareModeParam param1 = {0};
	param1.compareRegister = TIMER_B_CAPTURECOMPARE_REGISTER_1;
	param1.compareInterruptEnable = TIMER_B_CAPTURECOMPARE_INTERRUPT_DISABLE;
	param1.compareOutputMode = TIMER_B_OUTPUTMODE_RESET_SET;
	param1.compareValue = 0;
	Timer_B_initCompareMode(TIMER_B1_BASE, &param1);
}

void initSleepTimer(uint16_t period){
    //Start timer
    Timer_B_clearTimerInterrupt(TIMER_B0_BASE);

    Timer_B_initUpModeParam param = {0};
    param.clockSource = TIMER_B_CLOCKSOURCE_ACLK;
    param.clockSourceDivider = TIMER_B_CLOCKSOURCE_DIVIDER_1;
    param.timerPeriod = period;
    param.timerInterruptEnable_TBIE = TIMER_B_TBIE_INTERRUPT_DISABLE;
    param.captureCompareInterruptEnable_CCR0_CCIE =
        TIMER_B_CAPTURECOMPARE_INTERRUPT_ENABLE;
    param.timerClear = TIMER_B_DO_CLEAR;
    param.startTimer = true;
    Timer_B_initUpMode(TIMER_B0_BASE, &param);
}

void enable_ADC10(void){
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
	 * Sample/hold for 16 clock cycles
	 * Do not enable Multiple Sampling
	 */
	ADC_setupSamplingTimer(ADC_BASE,
						   ADC_CYCLEHOLD_16_CYCLES,
						   ADC_MULTIPLESAMPLESDISABLE);

	//Configure the Memory Buffer
	/*
	 * Base Address for the ADC Module
	 * Use input A2
	 * Use positive reference of AVcc
	 * Use negative reference of AVss
	 */
	ADC_configureMemory(ADC_BASE,
						ADC_INPUT_A3,
						ADC_VREFPOS_AVCC,
						ADC_VREFNEG_AVSS);

	ADC_clearInterrupt(ADC_BASE,
					   ADC_COMPLETED_INTERRUPT);

	//Enable the Memory Buffer Interrupt
	ADC_enableInterrupt(ADC_BASE,
						ADC_COMPLETED_INTERRUPT);
}

void disable_ADC10(void){
	ADC_disableConversions(ADC_BASE,0);
	ADC_disable(ADC_BASE);
}

void enable_SACOA(void){
    //Configure Op-Amp functionality
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
                                               GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4,
                                               GPIO_TERNARY_MODULE_FUNCTION);

    //Select external source for both positive and negative inputs
    SAC_OA_init(SAC0_BASE, SAC_OA_POSITIVE_INPUT_SOURCE_EXTERNAL,
                SAC_OA_NEGATIVE_INPUT_SOURCE_EXTERNAL);

    //Select low speed and low power mode
    SAC_OA_selectPowerMode(SAC0_BASE, SAC_OA_POWER_MODE_LOW_SPEED_LOW_POWER);
    SAC_OA_enable(SAC0_BASE);                  // Enable OA
    SAC_enable(SAC0_BASE);                     // Enable SAC
}

void disable_SACOA(void){
    //Turn off SAC to save power
	SAC_OA_disable(SAC0_BASE);
	SAC_disable(SAC0_BASE);

	//Configure Op-Amp pins as output
    GPIO_setAsOutputPin(GPIO_PORT_P1,
                        GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4);
}

void lightSample(void){

	enable_SACOA();
    enable_ADC10();

	__delay_cycles(15);

	//Enable and Start the conversion
    //in Single-Channel, Single Conversion Mode
    ADC_startConversion(ADC_BASE,
                        ADC_SINGLECHANNEL);

    //LPM3, ADC conversion complete will force exit
    __bis_SR_register(LPM3_bits + GIE);

	if(ADC_Conversion_Result >= 256)  //Normalize data for lower brightness
	{
		//Initialize compare mode to generate PWM
		Timer_B_setCompareValue(TIMER_B1_BASE, TIMER_B_CAPTURECOMPARE_REGISTER_1, 255);
	}
	else if (ADC_Conversion_Result <= 5)
	{
	    //Initialize compare mode to generate PWM
		Timer_B_setCompareValue(TIMER_B1_BASE, TIMER_B_CAPTURECOMPARE_REGISTER_1, 0);
	}
	else
	{
	    //Initialize compare mode to generate PWM
		Timer_B_setCompareValue(TIMER_B1_BASE, TIMER_B_CAPTURECOMPARE_REGISTER_1, ADC_Conversion_Result);
	}

	disable_ADC10();
	disable_SACOA();
}

void initGPIO(void){

    //Set Px.x to output direction
    P1DIR |= 0xFF;
    P1OUT = 0x00;
    P2DIR |= 0xFF;
    P2OUT = 0x00;

     // Select P2.0 as PWM Timer output function
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P2,
        GPIO_PIN0,
        GPIO_PRIMARY_MODULE_FUNCTION
        );

    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings
     */
    PMM_unlockLPM5();
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
    //Get previous write protection setting
    uint8_t state = HWREG8(SYS_BASE + OFS_SYSCFG0_L);
#ifdef DFWP
    uint8_t wp = DFWP | PFWP;
#else
    uint8_t wp = PFWP;
#endif

#ifdef FRWPPW
    HWREG16(SYS_BASE + OFS_SYSCFG0) = FWPW | (state & ~wp);
#else
    HWREG8(SYS_BASE + OFS_SYSCFG0_L) &= ~wp;
#endif

	ADC_Conversion_Result = (ADCMEM0 >> 1);		// Get conversion result and scale for lower light level situations (divide by 2)

    //Restore previous write protection setting
#ifdef FRWPPW
    HWREG16(SYS_BASE + OFS_SYSCFG0) = FWPW | state;
#else
    HWREG8(SYS_BASE + OFS_SYSCFG0_L) = state;
#endif

	__bic_SR_register_on_exit(LPM3_bits);              // Sleep Timer Exits LPM3
}

//******************************************************************************
//
//This is the Timer B0 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(TIMER0_B0_VECTOR)))
#endif
void TIMER0_B0_ISR(void)
{
	__bic_SR_register_on_exit(LPM3_bits);              // Sleep Timer Exits LPM3
}
