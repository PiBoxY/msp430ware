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
 * Out of Box Demo for the MSP-EXP430FR5969
 * Main loop, initialization, and interrupt service routines
 *
 * June 2014
 * E. Chen
 *
 ******************************************************************************/

#include "main.h"
#include "LiveTempMode.h"
#include "FRAMLogMode.h"
#include "driverlib.h"

uint8_t RXData = 0;                               // UART Receive byte
int mode = 0;                                     // mode selection variable
int pingHost = 0;                                 // ping request from PC GUI
Calendar calendar;                                // Calendar used for RTC

#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = 0x9000
__no_init uint16_t dataArray[12289];
#endif

//-----------------------------------------------------------------------------
int _system_pre_init(void)
{
    // Stop Watchdog timer
    WDT_A_hold(__MSP430_BASEADDRESS_WDT_A__);     // Stop WDT

    /*==================================*/
    /* Choose if segment initialization */
    /* should be done or not. */
    /* Return: 0 to omit initialization */
    /* 1 to run initialization */
    /*==================================*/
    return 1;
}

/*
 * main.c
 */
int main(void) {

    // Check if a wakeup from LPMx.5
    if (SYSRSTIV == SYSRSTIV_LPM5WU) {
    	// Button S2 pressed
        if (P1IFG & BIT1)
        {
        	// Clear P1.1 interrupt flag
        	GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN0);

        	// Exit FRAM Log Mode
        	mode = '0';

        	// Light up LED1 to indicate Exit from FRAM mode
        	Init_GPIO();
        	GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN6);
        	__delay_cycles(600000);
        }
        else
        {
        	// Continue FRAM data logging
            mode = FRAM_LOG_MODE;
            Init_RTC();
            Init_GPIO();
            framLog();
        }
    }
    else
    {
    	Init_GPIO();
    	GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);

    	// Toggle LED1 and LED2 to indicate OutOfBox Demo start
    	int i;
    	for (i=0;i<10;i++)
    	{
            GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
            GPIO_toggleOutputOnPin(GPIO_PORT_P4, GPIO_PIN6);
            __delay_cycles(200000);
    	}
    }

    // Board initializations
    Init_GPIO();
    Init_Clock();
    Init_UART();

    // Main Loop
    while (1)
    {
    	// Acknowledge PC GUI's ping request
        if (pingHost)
            sendAckToPC();

        switch (mode)
        {
            case LIVE_TEMP_MODE:                  // Measures and transmits internal temperature data every 0.125 second
            	sendCalibrationConstants();
                liveTemp();
                break;
            case FRAM_LOG_MODE:                   // Logs internal temperature and battery voltage data to FRAM every 5 seconds
            	Init_RTC();
            	storeTimeStamp();
                dataArray[12288] = 0;

            	int i;
            	for (i=0;i<3;i++)
            	{
                    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN6);
                    __delay_cycles(300000);
                    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
                    __delay_cycles(2000000);
            	}

                enterLPM35();
                framLog();
                break;
            case TRANSMIT_DATA_MODE:              // Transmits Logged FRAM data through UART to PC
            	sendCalibrationConstants();
            	sendTimeStamp();
                transmitFRAMData();
                break;
        }

        __bis_SR_register(LPM3_bits | GIE);       // Enter LPM3 and wait for PC commands
        __no_operation();
    }
}

/*
 * GPIO Initialization
 */
void Init_GPIO()
{
    // Set all GPIO pins to output low for low power
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_PJ, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7|GPIO_PIN8|GPIO_PIN9|GPIO_PIN10|GPIO_PIN11|GPIO_PIN12|GPIO_PIN13|GPIO_PIN14|GPIO_PIN15);

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_PJ, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7|GPIO_PIN8|GPIO_PIN9|GPIO_PIN10|GPIO_PIN11|GPIO_PIN12|GPIO_PIN13|GPIO_PIN14|GPIO_PIN15);

	// Configure P2.0 - UCA0TXD and P2.1 - UCA0RXD
	GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
	GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2, GPIO_PIN1, GPIO_SECONDARY_MODULE_FUNCTION);

    // Set PJ.4 and PJ.5 as Primary Module Function Input, LFXT.
    GPIO_setAsPeripheralModuleFunctionInputPin(
           GPIO_PORT_PJ,
           GPIO_PIN4 + GPIO_PIN5,
           GPIO_PRIMARY_MODULE_FUNCTION
           );

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PMM_unlockLPM5();
}

/*
 * Clock System Initialization
 */
void Init_Clock()
{
    // Set DCO frequency to 8 MHz
    CS_setDCOFreq(CS_DCORSEL_0, CS_DCOFSEL_6);
    //Set external clock frequency to 32.768 KHz
    CS_setExternalClockSource(32768, 0);
    //Set ACLK=LFXT
    CS_initClockSignal(CS_ACLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
    // Set SMCLK = DCO with frequency divider of 1
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    // Set MCLK = DCO with frequency divider of 1
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    //Start XT1 with no time out
    CS_turnOnLFXT(CS_LFXT_DRIVE_0);
}

/*
 * UART Communication Initialization
 */
void Init_UART()
{
    // Configure UART
    EUSCI_A_UART_initParam param = {0};
    param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    param.clockPrescalar = 52;
    param.firstModReg = 1;
    param.secondModReg = 0x49;
    param.parity = EUSCI_A_UART_NO_PARITY;
    param.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    param.uartMode = EUSCI_A_UART_MODE;
    param.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;

    if(STATUS_FAIL == EUSCI_A_UART_init(EUSCI_A0_BASE, &param))
        return;

    EUSCI_A_UART_enable(EUSCI_A0_BASE);

    EUSCI_A_UART_clearInterrupt(EUSCI_A0_BASE,
                                EUSCI_A_UART_RECEIVE_INTERRUPT);

    // Enable USCI_A0 RX interrupt
    EUSCI_A_UART_enableInterrupt(EUSCI_A0_BASE,
                                 EUSCI_A_UART_RECEIVE_INTERRUPT); // Enable interrupt

    // Enable globale interrupt
    __enable_interrupt();
}

/*
 * Real Time Clock Initialization
 */
void Init_RTC()
{
    //Setup Current Time for Calendar
    calendar.Seconds    = 0x55;
    calendar.Minutes    = 0x30;
    calendar.Hours      = 0x04;
    calendar.DayOfWeek  = 0x01;
    calendar.DayOfMonth = 0x30;
    calendar.Month      = 0x04;
    calendar.Year       = 0x2014;

    // Initialize RTC with the specified Calendar above
    RTC_B_initCalendar(RTC_B_BASE,
                       &calendar,
                       RTC_B_FORMAT_BCD);

    RTC_B_setCalendarEvent(RTC_B_BASE,
    		               RTC_B_CALENDAREVENT_MINUTECHANGE
    		               );

    RTC_B_clearInterrupt(RTC_B_BASE,
                         RTC_B_TIME_EVENT_INTERRUPT
                         );

    RTC_B_enableInterrupt(RTC_B_BASE,
                          RTC_B_TIME_EVENT_INTERRUPT
                          );

    //Start RTC Clock
    RTC_B_startClock(RTC_B_BASE);
}

/*
 * Transmit Internal Temperature Sensor's Calibration constants through UART
 */
void sendCalibrationConstants()
{
	// Select UART TXD on P2.0
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN0, GPIO_SECONDARY_MODULE_FUNCTION);

    // Send Temp Sensor Calibration Data
    EUSCI_A_UART_transmitData(EUSCI_A0_BASE, CAL_ADC_12T30_H);
    EUSCI_A_UART_transmitData(EUSCI_A0_BASE, CAL_ADC_12T30_L);

    __delay_cycles(900000);

    EUSCI_A_UART_transmitData(EUSCI_A0_BASE, CAL_ADC_12T85_H);
    EUSCI_A_UART_transmitData(EUSCI_A0_BASE, CAL_ADC_12T85_L);

    while(EUSCI_A_UART_queryStatusFlags(EUSCI_A0_BASE, EUSCI_A_UART_BUSY));

    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
	GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
}

/*
 * Transmit FRAMLogMode starting TimeStamp through UART
 */
void sendTimeStamp()
{
	// Select UART TXD on P2.0
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN0, GPIO_SECONDARY_MODULE_FUNCTION);
    __delay_cycles(900000);
    int i;
    for (i=0;i<6;i++) {
        EUSCI_A_UART_transmitData(EUSCI_A0_BASE, timeStamp[i]);
        __delay_cycles(900000);
    }

    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
	GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
}

/*
 * Transmit 0xFF to acknowledge PC GUI's ping request
 */
void sendAckToPC()
{
	// Select UART TXD on P2.0
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN0, GPIO_SECONDARY_MODULE_FUNCTION);

    // Send Ackknowledgement to Host PC
    EUSCI_A_UART_transmitData(EUSCI_A0_BASE, 0xFF);
    EUSCI_A_UART_transmitData(EUSCI_A0_BASE, 0xFF);

    while(EUSCI_A_UART_queryStatusFlags(EUSCI_A0_BASE, EUSCI_A_UART_BUSY));
    pingHost = 0;

    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
}

/*
 * Enter Low Power Mode 3.5
 */
void enterLPM35()
{
	// Configure button S2 (P1.1) interrupt
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);

    // Request the disabling of the core voltage regulator when device enters
    // LPM3 (or LPM4) so that we can effectively enter LPM3.5 (or LPM4.5).
    PMM_turnOffRegulator();

    //Enter LPM3 mode with interrupts enabled
    __bis_SR_register(LPM4_bits + GIE);
    __no_operation();
}

/*
 * USCI_A0 Interrupt Service Routine that receives PC GUI's commands
 */
#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
	int i;
    switch (__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG)) {
        case USCI_NONE: break;
        case USCI_UART_UCRXIFG:
            i = EUSCI_A_UART_receiveData(EUSCI_A0_BASE);
        	if (i == '5')
                pingHost = 1;
            else
                mode = i;
            __bic_SR_register_on_exit(LPM3_bits); // Exit active CPU
            break;
        case USCI_UART_UCTXIFG: break;
        case USCI_UART_UCSTTIFG: break;
        case USCI_UART_UCTXCPTIFG: break;
    }
}

/*
 * ADC12 Interrupt Service Routine
 * Exits LPM3 when Temperature/Voltage data is ready
 */
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
  switch(__even_in_range(ADC12IV,76))
  {
    case  ADC12IV_NONE: break;                // Vector  0:  No interrupt
    case  ADC12IV_ADC12OVIFG: break;          // Vector  2:  ADC12MEMx Overflow
    case  ADC12IV_ADC12TOVIFG: break;         // Vector  4:  Conversion time overflow
    case  ADC12IV_ADC12HIIFG: break;          // Vector  6:  ADC12HI
    case  ADC12IV_ADC12LOIFG: break;          // Vector  8:  ADC12LO
    case ADC12IV_ADC12INIFG: break;           // Vector 10:  ADC12IN
    case ADC12IV_ADC12IFG0:                   // Vector 12:  ADC12MEM0
        ADC12IFGR0 &= ~ADC12IFG0;             // Clear interrupt flag
        __bic_SR_register_on_exit(LPM3_bits); // Exit active CPU
        break;
    case ADC12IV_ADC12IFG1:                   // Vector 14:  ADC12MEM1
        ADC12IFGR0 &= ~ADC12IFG1;             // Clear interrupt flag
        __bic_SR_register_on_exit(LPM3_bits); // Exit active CPU
        break;
    case ADC12IV_ADC12IFG2: break;            // Vector 16:  ADC12MEM2
    case ADC12IV_ADC12IFG3: break;            // Vector 18:  ADC12MEM3
    case ADC12IV_ADC12IFG4: break;            // Vector 20:  ADC12MEM4
    case ADC12IV_ADC12IFG5: break;            // Vector 22:  ADC12MEM5
    case ADC12IV_ADC12IFG6: break;            // Vector 24:  ADC12MEM6
    case ADC12IV_ADC12IFG7: break;            // Vector 26:  ADC12MEM7
    case ADC12IV_ADC12IFG8: break;            // Vector 28:  ADC12MEM8
    case ADC12IV_ADC12IFG9: break;            // Vector 30:  ADC12MEM9
    case ADC12IV_ADC12IFG10: break;           // Vector 32:  ADC12MEM10
    case ADC12IV_ADC12IFG11: break;           // Vector 34:  ADC12MEM11
    case ADC12IV_ADC12IFG12: break;           // Vector 36:  ADC12MEM12
    case ADC12IV_ADC12IFG13: break;           // Vector 38:  ADC12MEM13
    case ADC12IV_ADC12IFG14: break;           // Vector 40:  ADC12MEM14
    case ADC12IV_ADC12IFG15: break;           // Vector 42:  ADC12MEM15
    case ADC12IV_ADC12IFG16: break;           // Vector 44:  ADC12MEM16
    case ADC12IV_ADC12IFG17: break;           // Vector 46:  ADC12MEM17
    case ADC12IV_ADC12IFG18: break;           // Vector 48:  ADC12MEM18
    case ADC12IV_ADC12IFG19: break;           // Vector 50:  ADC12MEM19
    case ADC12IV_ADC12IFG20: break;           // Vector 52:  ADC12MEM20
    case ADC12IV_ADC12IFG21: break;           // Vector 54:  ADC12MEM21
    case ADC12IV_ADC12IFG22: break;           // Vector 56:  ADC12MEM22
    case ADC12IV_ADC12IFG23: break;           // Vector 58:  ADC12MEM23
    case ADC12IV_ADC12IFG24: break;           // Vector 60:  ADC12MEM24
    case ADC12IV_ADC12IFG25: break;           // Vector 62:  ADC12MEM25
    case ADC12IV_ADC12IFG26: break;           // Vector 64:  ADC12MEM26
    case ADC12IV_ADC12IFG27: break;           // Vector 66:  ADC12MEM27
    case ADC12IV_ADC12IFG28: break;           // Vector 68:  ADC12MEM28
    case ADC12IV_ADC12IFG29: break;           // Vector 70:  ADC12MEM29
    case ADC12IV_ADC12IFG30: break;           // Vector 72:  ADC12MEM30
    case ADC12IV_ADC12IFG31: break;           // Vector 74:  ADC12MEM31
    case ADC12IV_ADC12RDYIFG: break;          // Vector 76:  ADC12RDY
    default: break;
  }
}

/*
 * Timer0_A3 Interrupt Vector (TAIV) handler
 * Used to trigger ADC conversion every 0.125 seconds
 *
 */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
    __bic_SR_register_on_exit(LPM3_bits); // Exit active CPU
}

/*
 * Port 1 interrupt service routine to handle S2 button press
 *
 */
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  P1IFG &= ~BIT1;                           // Clear P1.1 IFG
  mode = FRAM_LOG_MODE;
  __bic_SR_register_on_exit(LPM3_bits);     // Exit LPM3
}
