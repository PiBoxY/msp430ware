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
 * ULPMeter.c
 * User Experience Code for the MSP-EXP430FR5969
 * Battery-less ADC Application
 *
 * Program Execution: A menu is displayed previous to user interaction. If
 *   "Transfer Data" is chosen, then the DMA, triggered by the UCA0TXIFG, will
 *   transfer the previously logged data to the PC via UART. If "Run App"
 *   is chosen, then a warning screen is shown. Before entering LPM3.5, the device
 *   takes an ADC reading of the capacitor/battery charge remaining. This is then
 *   displayed (as a battery icon display) for 1s before the device enters LPM3.5.
 *   The device can then exit/enter LPM3.5 using the right button. If the device is
 *   awake for more than 10s before being put back into LPM3.5, it will timeout
 *   and reset the app to the menu screen.
 *
 * Directions for use: Upon entering the app, a menu will display two options,
 *   "Run App" and "Transfer Data." If "Transfer Data" is selected, then the UART
 *   will trigger the DMA to send the previously logged data to the PC via UART.
 *   If "Run App" is selected, a warning screen saying that the device is entering
 *   LPM3.5 is displayed. When the right button is pushed, the device takes an
 *   ADC measurement of the charge of 2/3*Vcc and compares it to the internal
 *   reference. This measurement is displayed for 1s then the device disables
 *   the menu button and goes into LPM3.5. The device will wake up every minute
 *   to take another ADC measurement. When the right button is pressed (while
 *   still in LPM3.5), the device will wake up and show the battery charge display
 *   based on the most recent ADC measurement. In this state, the right button
 *   will put the device back into LPM3.5 and the left button will take the user
 *   back to the main menu. 10s of idle time in this state will cause a timeout
 *   and the device to return to the app menu screen.
 *
 * February 2012
 *
 *************************************************************************//****
 * Rev 1.1 - RTCTEV bug fixed --> removed clearing of LOCKLPM5 bit from
 *           SYS_wakupLPM35 and placed it at the top of code
 * Rev 1.2 - driverlib integration
 * Rev 1.3 - UART sending of logged data added
 ******************************************************************************/

#include "driverlib.h"
#include "Sharp96x96.h"
#include "CTS/CTS_wolverine.h"

#include "ULPMeter.h"
#include "FR59xx_EXP.h"
#include "myTimer.h"
#include "UpdateDisplay.h"

#define FRAM_ADC_INDEX          0xF008          // FRAM location for ADC index
#define V_REF_VALUE             0xF00A          // VRef Value for ADC calculations
// The reserved space is large enough to hold 1,466 sample values. Since we are taking one sample
// per minute this equals a maximum time of ~24.5hours.
#define FRAM_ADC_RESULTS        0xF00C          // FRAM location for ADC results
#define FRAM_ADC_RESULTS_END    0xFB7F          // Address following the storage area

char clearStopwatch;
char takeADCReading;

unsigned int *FRAM_write_ptr;
unsigned int *FRAM_write_index;
unsigned int *vRef_value;

static unsigned char ULP_subMenu(void);         // Battery free menu
static void ULP_takeADCReading(void);           // Init ADC and REF, then take ADC reading
static void ULP_stopADCReading(void);           // stop ADC and REF, then stop ADC reading
static void ULP_setupRTC(void);                 // Set up RTC for 10s event interrupts
static void ULP_clearStopwatch(void);           // Clear RTC contents used for "stopwatch"
static signed int ADC_calculateBars(void);      // Find out how many "bars" of charge remain
static void ULP_sendData(void);                 // Send data via UART to PC

/**********************************************************************//**
 * @brief  ULP_ADC Battery-less application
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void ULP_meter(void)
{
    signed int bars;

    while (mode == APP_ULP_ADC) {
        switch (select) {
        case SEL_MENU_MODE:
            FRAM_write_index = (unsigned int*)FRAM_ADC_INDEX;               // Init FRAM index
            FRAM_write_ptr = (unsigned int*)(FRAM_ADC_RESULTS + *FRAM_write_index);
            vRef_value = (unsigned int*)V_REF_VALUE;
            *vRef_value = REF_A_VREF2_0V;
            select = ULP_subMenu();                        // Set select after interrupts are finished

            // Disable CapTouch sliders
            GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5);
            GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6);
            break;
        case SEL_TITLE_MODE:         // Display instructions, go to LPM3 till button pushed
            *FRAM_write_index = 0x0000;

            clearStopwatch = 1;
            takeADCReading = 1;

            LCD_introULP();                                     // Display Instructions

            __bis_SR_register(LPM3_bits + GIE);                 // Wait for P1.1 to be pushed to
            __no_operation();                                   // progress to ADC_LPM35_MODE
            break;
        case SEL_ADC_LPM35_MODE:                                // Take an ADC measurement, enter LPM3.5
            // Disable both buttons
            DisableRightSwitch();
            DisableLeftSwitch();

            TA0_turnOff();                                      // Turn off VCOM toggle timer
            LCD_turnOff();                                      // Turn off power to the LCD

            if (takeADCReading) {             // take ADC reading of Vcc
                // Init FRAM pointers
                FRAM_write_index = (unsigned int*)FRAM_ADC_INDEX;
                // TODO: It looks like the first word will always be zero (0xFFFF) then?? We should increment the index after the ADC conversion.
                *FRAM_write_index += 2;                 // Increment write index
                FRAM_write_ptr = (unsigned int*)(FRAM_ADC_RESULTS + *FRAM_write_index);
                vRef_value = (unsigned int*)V_REF_VALUE;

                // Make sure we are not exceeding the reserved memory region. If we do we are going
                // to cancel the ongoing operation and return to the main menu.
                if (FRAM_write_ptr > (unsigned int*)FRAM_ADC_RESULTS_END) {
                    RTC_B_disableInterrupt(__MSP430_BASEADDRESS_RTC_B__, RTC_B_TIME_EVENT_INTERRUPT);

                    Sharp96x96_initDisplay();
                    TA0_enableVCOMToggle();

                    select = SEL_MENU_MODE;                     // Return to app Menu

                    // Re-enable Main Menu Button (Left Button) and Select button (Right Button)
                    EnableLeftSwitch();
                    EnableRightSwitch();
                    break;
                }

                ULP_takeADCReading();                 // Start up ADC and REF and take reading
                takeADCReading = 0;
            }

            if (clearStopwatch) {             // clear stopwatch
                ULP_clearStopwatch();
                bars = ADC_calculateBars();
                LCD_drawBattery(bars);                  // Show initial charge
                TA1_sleep(4096);                        // for 1 second
                clearStopwatch = 0;
            }

            ULP_setupRTC();                 // Start RTC, 1 min interrupts

            // Re-enable select (right) button
            EnableRightSwitch();

            // If cap/battery charge is below 2.2 V then enter the low battery mode. Otherwise
            // enter low-power mode LPM3.5. Note that in this case the function will not return;
            // any LPMx.5 always results in a device RESET upon wakeup.
            if (*FRAM_write_ptr < (2.2 * 4095 / 4)) {
                select = SEL_BAT_LOW_MODE;
            }
            else {
                SYS_enterLPM35();
            }
            break;
        case SEL_DISPLAY_MODE:
            RTC_B_holdClock(__MSP430_BASEADDRESS_RTC_B__);

            Sharp96x96_initDisplay();                       // Perform a full initialization of the display
            TA0_enableVCOMToggle();                     // Enable VCOM toggle

            // Init FRAM pointers
            FRAM_write_index = (unsigned int*)FRAM_ADC_INDEX;
            FRAM_write_ptr = (unsigned int*)(FRAM_ADC_RESULTS + *FRAM_write_index);
            vRef_value = (unsigned int*)V_REF_VALUE;

            ULP_takeADCReading();                       // Start up ADC and REF and overwrite previous reading
            bars = ADC_calculateBars();                 // Calculate # of bars of charge remain
            LCD_drawBattery(bars);                      // Send image to LCD

            // Re-enable Main Menu Button (Left Button)
            EnableLeftSwitch();

            TA1_sleep(40960);
            break;
        case SEL_BAT_LOW_MODE:
            RTC_B_disableInterrupt(__MSP430_BASEADDRESS_RTC_B__, RTC_B_TIME_EVENT_INTERRUPT);

            Sharp96x96_initDisplay();                       // Perform a full initialization of the display
            TA0_enableVCOMToggle();                     // Enable VCOM toggle

            bars = ADC_calculateBars();             // Calculate # of bars of charge remain

            do {                                                // Stay in this mode until USB connected (or until Vcc > 3V)
                LCD_drawLowBattery(bars);                       // Send image to LCD

                __bis_SR_register(LPM3_bits + GIE);                     // Wait for P1.1 to be pushed to
                __no_operation();                                       // retest if USB is plugged in

                ULP_takeADCReading();                                   // Start up ADC and REF and overwrite previous reading
                bars = ADC_calculateBars();                             // Calculate # of bars of charge remain
            } while (*FRAM_write_ptr < (3.1 * 4095 / 4));               // Is Vcc < 3.1V?

            // Re-enable Main Menu Button (Left Button)
            EnableLeftSwitch();
            break;
        case SEL_UART_TRANSFER_MODE:
            LCD_displaySendingScreen();                         // Show "Sending - Please Wait" screen

            if (*FRAM_write_index == 0xFFFF) {                  // ensure true data is being sent
                *FRAM_write_index = 0;
            }

            if (*FRAM_write_index != 0) {
                ULP_sendData();                                                 // Send data
                LCD_showDataSent((*FRAM_write_index - 1));                      // Show "Data sent to PC" screen (1s)
            }
            else {
                LCD_showDataSent(0);
            }
            TA1_sleep(4096);

            select = SEL_MENU_MODE;                     // Return to app Menu
            break;
        default: break;
        }
    }
    TA1_turnOff();

    // STOP ADC and REF
    ULP_stopADCReading();

    // Stop RTC
    RTC_B_holdClock(__MSP430_BASEADDRESS_RTC_B__);

    // Reinit CapTouch sliders
    GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5);
    GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6);
}

/**********************************************************************//**
 * @brief  ADC12_ISR - Interrupt Service Routine for ADC12B
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
#if defined (__TI_COMPILER_VERSION__) || defined (__IAR_SYSTEMS_ICC__)
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
#elif defined(__GNUC__)
void ADC12_ISR(void) __attribute__((interrupt(ADC12_VECTOR)));
void ADC12_ISR(void)
#else
#error Compiler not supported!
#endif
{
    // Get the ADC result and store it in a local variable for more efficient access.
    unsigned int adcResult = ADC12MEM0;

    // Now, let's check if we need to update the reference voltage settings. We want to switch
    // between the 1.2V and 2.0V settings of the reference which will allow us to measure VCC
    // up to 2.4V respective 4.0V using the ADC-internal battery measurement channel.

    if (*vRef_value == REF_A_VREF1_2V) {
        // Did we exceed the range for the 1.2V reference, meaning we max'ed out the input
        // voltage range equal to an ADC result of 4095? That happens when the input voltage
        // reaches 2.4V. If that's the case enable the 2.0V reference for the next ADC run.
        if (adcResult >= 0x0FFF) {
            *vRef_value = REF_A_VREF2_0V;
        }

        // Scale the voltage as it would have been taken with the 2.0V reference instead of the
        // 1.2V reference it was actually taken with.
        adcResult = adcResult * 6 / 10;
    }
    else {
        // The Vref is currently set to 2.0V. See if our ADC result points to a battery voltage
        // that is equal to or smaller than 2.3V. If that's the case let's switch over to the 1.2V
        // voltage reference for the next ADC run.
        if (adcResult <= (2.3 * 4095 / 4)) {
            *vRef_value = REF_A_VREF1_2V;
        }
    }

    // Store our adjusted result for further processing.
    *FRAM_write_ptr = adcResult;

    // Disable reference voltage generator and the ADC12_B
    ULP_stopADCReading();

    // Exit any LPMx by clearing the LPM4 status bits from the top of the stack
    __bic_SR_register_on_exit(LPM4_bits);
}

/**********************************************************************//**
 * @brief  ULP_subMenu - menu to choose whether to run app or to send data
 *         via UART to PC
 *
 * @param  none
 *
 * @return unsigned char - choice of which way to go in app (either to run
 *         the app or to send app data to PC via UART)
 *************************************************************************/
static unsigned char ULP_subMenu(void)
{
    unsigned char choice = 0;

    while (select == SEL_MENU_MODE) {
        CTS_getReading();

        if (sliderPos[0] <= 30) {
            choice = SEL_UART_TRANSFER_MODE;
        }
        else {
            choice = SEL_TITLE_MODE;
        }
        LCD_displayULPSubMenu(choice);         // show menu, highlighting selected option
        LCD_display();
    }
    return choice;
}

/**********************************************************************//**
 * @brief  ULP_stopADCReading - close ADC and REF modules, also
 *         stop ADC reading of 2/3*Vcc as compared to 2.5V internal
 *         reference
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
static void ULP_stopADCReading(void)
{
    ADC12_B_disableConversions(__MSP430_BASEADDRESS_ADC12_B__, ADC12_B_COMPLETECONVERSION);

    // If ref generator busy, WAIT
    while (Ref_A_isRefGenBusy(__MSP430_BASEADDRESS_REF_A__)) {
    }

    Ref_A_disableReferenceVoltage(__MSP430_BASEADDRESS_REF_A__);          //Internal Reference OFF
    ADC12_B_disable(__MSP430_BASEADDRESS_ADC12_B__);
}

/**********************************************************************//**
 * @brief  ULP_takeADCReading - initialization ADC and REF modules, also
 *         take ADC reading of 2/3*Vcc as compared to 2.5V internal
 *         reference
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
static void ULP_takeADCReading(void)
{
    // If ref generator busy, WAIT
    while (Ref_A_isRefGenBusy(__MSP430_BASEADDRESS_REF_A__)) {
    }

    // Configure the reference voltage according to a global variable to either 1.2V or 2.0V to
    // enable covering the entire supply voltage range of the device using the device-internal
    // voltage divider of 2. After enabling the reference observe the required settling time as
    // per device D/S of 80us (max).
    Ref_A_setReferenceVoltage(__MSP430_BASEADDRESS_REF_A__ , *vRef_value);
    Ref_A_enableReferenceVoltage(__MSP430_BASEADDRESS_REF_A__);
    __delay_cycles(80 * MCLK_CYCLES_PER_US);

    // Setup ADC for reading "Battery" Voltage. The sample time for the internal voltage divider is
    // configured to approx. 16/f_MODOSC = 16/4.8MHz = ~3.3us
    ADC12_B_initParam initParam = {0};
    initParam.sampleHoldSignalSourceSelect = ADC12_B_SAMPLEHOLDSOURCE_SC;
    initParam.clockSourceSelect = ADC12_B_CLOCKSOURCE_ADC12OSC;
    initParam.clockSourceDivider = ADC12_B_CLOCKDIVIDER_1;
    initParam.clockSourcePredivider = ADC12_B_CLOCKPREDIVIDER__4;
    initParam.internalChannelMap = ADC12_B_BATTMAP;
    ADC12_B_init(ADC12_B_BASE, &initParam);

    ADC12_B_setupSamplingTimer(__MSP430_BASEADDRESS_ADC12_B__, ADC12_B_CYCLEHOLD_16_CYCLES,
                              0, ADC12_B_MULTIPLESAMPLESDISABLE);

    ADC12_B_configureMemoryParam configureMemoryParam = {0};
    configureMemoryParam.memoryBufferControlIndex = ADC12_B_MEMORY_0;
    configureMemoryParam.inputSourceSelect = ADC12_B_INPUT_BATMAP;
    configureMemoryParam.refVoltageSourceSelect =
    		ADC12_B_VREFPOS_INTBUF_VREFNEG_VSS;
    configureMemoryParam.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;
    configureMemoryParam.windowComparatorSelect =
    		ADC12_B_WINDOW_COMPARATOR_DISABLE;
    configureMemoryParam.differentialModeSelect =
    		ADC12_B_DIFFERENTIAL_MODE_DISABLE;
    ADC12_B_configureMemory(ADC12_B_BASE, &configureMemoryParam);

    ADC12_B_clearInterrupt(__MSP430_BASEADDRESS_ADC12_B__, ADC12_B_IE0, ADC12_B_IFG0);
    ADC12_B_enableInterrupt(__MSP430_BASEADDRESS_ADC12_B__, ADC12_B_IE0, 0, 0);

    // Start conversion for first result
    ADC12_B_enable(__MSP430_BASEADDRESS_ADC12_B__);
    ADC12_B_startConversion(__MSP430_BASEADDRESS_ADC12_B__, ADC12_B_START_AT_ADC12MEM0,
                           ADC12_B_SINGLECHANNEL);

    // Enter LPM3 and wait for the end of conversion
    __bis_SR_register(LPM3_bits | GIE);                 // LPM3 w/ interrupts
    __no_operation();                                   // For debugger
}

/**********************************************************************//**
 * @brief  ULP_setupRTC - set up the RTC for 1 min event interrupts
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
static void ULP_setupRTC(void)
{
    RTC_B_holdClock(__MSP430_BASEADDRESS_RTC_B__);
    RTC_B_enableInterrupt(__MSP430_BASEADDRESS_RTC_B__, RTC_B_TIME_EVENT_INTERRUPT);
    RTC_B_setCalendarEvent(__MSP430_BASEADDRESS_RTC_B__, RTC_B_CALENDAREVENT_MINUTECHANGE);
    RTC_B_startClock(__MSP430_BASEADDRESS_RTC_B__);
}

/**********************************************************************//**
 * @brief  ULP_clearStopwatch - clear RTC registers pertaining to time
 *         spent in LPM3.5
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
static void ULP_clearStopwatch(void)
{
    Calendar clearStopwatchCalendar;

    // Clear hours, minutes, and seconds, and set day of month for stopwatch
    clearStopwatchCalendar.Seconds = 0;
    clearStopwatchCalendar.Minutes = 0;
    clearStopwatchCalendar.Hours = 0;
    clearStopwatchCalendar.DayOfMonth = 1;

    RTC_B_initCalendar(__MSP430_BASEADDRESS_RTC_B__, &clearStopwatchCalendar, RTC_B_FORMAT_BINARY);
}

/**********************************************************************//**
 * @brief  ADC_calculateBars - take ADC reading and determine how many
 *         "bars" of charge should appear on LCD
 *
 * @param  none
 *
 * @return charge - bars of charge remaining (for drawing battery)
 *************************************************************************/
static signed int ADC_calculateBars(void)
{
    signed int charge = 0;
    unsigned int conversionResult = *FRAM_write_ptr;

    // Convert the adjusted ADC12 conversion result into a bar-graph value. This assumes all
    // conversion results relate to a reference voltage of 2.0V which equals to a hypothetical
    // full-scale supply voltage reading of 4.0V.
    if (conversionResult >= (3.5 * 4095 / 4)) {               // 3.5V
        charge = 8;
    }
    else if (conversionResult >= (3.3 * 4095 / 4)) {          // 3.3V
        charge = 7;
    }
    else if (conversionResult >= (3.1 * 4095 / 4)) {          // 3.1V
        charge = 6;
    }
    else if (conversionResult >= (2.9 * 4095 / 4)) {          // 2.9V
        charge = 5;
    }
    else if (conversionResult >= (2.7 * 4095 / 4)) {          // 2.7V
        charge = 4;
    }
    else if (conversionResult >= (2.5 * 4095 / 4)) {          // 2.5V
        charge = 3;
    }
    else if (conversionResult >= (2.3 * 4095 / 4)) {          // 2.3V
        charge = 2;
    }
    else if (conversionResult >= (2.1 * 4095 / 4)) {          // 2.1V
        charge = 1;
    }
    else {                                                    // < 2.1V
        charge = 0;
    }

    return charge;
}

/**********************************************************************//**
 * @brief  ULP_sendData - send all data that has been logged to the PC via
 *         UART
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
static void ULP_sendData(void)
{
    // Configure UART pins
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2,
            GPIO_PIN0 | GPIO_PIN1, GPIO_SECONDARY_MODULE_FUNCTION);

    // Configure UART - 9600 baud
    EUSCI_A_UART_initParam param = {0};
    param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_ACLK;
    param.clockPrescalar = 0x03;
    param.firstModReg = 0x00;
    param.secondModReg = 0x92;
    param.parity = EUSCI_A_UART_NO_PARITY;
    param.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    param.uartMode = EUSCI_A_UART_MODE;
    param.overSampling = EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;
    EUSCI_A_UART_init(EUSCI_A0_BASE, &param);

    EUSCI_A_UART_enable(__MSP430_BASEADDRESS_EUSCI_A0__);

    // Trigger DMA transfer by sending intro byte
    while (!(UCA0IFG & UCTXIFG)) {
    }

    // byte has no significance, just used as DMA trigger
    EUSCI_A_UART_transmitData(__MSP430_BASEADDRESS_EUSCI_A0__, 0x44);

    // Set up DMA0, Repeated single transfer, length = index, UART trigger, transmit bytes
    DMA_initParam dma_param = {0};
    dma_param.channelSelect = DMA_CHANNEL_0;
    dma_param.transferModeSelect = DMA_TRANSFER_REPEATED_SINGLE;
    dma_param.transferSize = *FRAM_write_index;
    dma_param.triggerSourceSelect = DMA_TRIGGERSOURCE_15;
    dma_param.transferUnitSelect = DMA_SIZE_SRCBYTE_DSTBYTE;
    dma_param.triggerTypeSelect = DMA_TRIGGER_RISINGEDGE;
    DMA_init(&dma_param);

    // Transfer from ADC_results in FRAM, increment
    DMA_setSrcAddress(DMA_CHANNEL_0, (unsigned long)FRAM_ADC_RESULTS + 2,
                      DMA_DIRECTION_INCREMENT);

    // Transfer to TX buffer for UART (UCA0), unchanged
    DMA_setDstAddress(DMA_CHANNEL_0, (unsigned long)&UCA0TXBUF,
                      DMA_DIRECTION_UNCHANGED);
    DMA_enableTransfers(DMA_CHANNEL_0);
    DMA_enableInterrupt(DMA_CHANNEL_0);

    // Go to sleep until transfer finishes
    __bis_SR_register(LPM0_bits + GIE);
    __no_operation();

    EUSCI_A_UART_disable(__MSP430_BASEADDRESS_EUSCI_A0__);                      // Stop UART (UCA0)
    DMA_disableTransfers(DMA_CHANNEL_0);                                        // disable DMA
}
