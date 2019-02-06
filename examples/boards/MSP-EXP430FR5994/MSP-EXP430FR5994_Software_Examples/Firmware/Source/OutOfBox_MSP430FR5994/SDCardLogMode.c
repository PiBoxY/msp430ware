/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
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
 * SDCardLogMode.c
 *
 * Wakes up every 5 seconds from LPM3 to measure and store its
 * internal temperature sensor & battery monitor data to SDCard
 *
 * March 2016
 * E. Chen
 *
 ******************************************************************************/

#include "stdlib.h"
#include "string.h"
#include "SDCardLogMode.h"
#include "driverlib.h"
#include "sdcard.h"
#include "HAL_SDCard.h"

// FRAM variable to keep track of number of SDCard logfiles
#if defined(__IAR_SYSTEMS_ICC__)
__persistent uint8_t numLogFiles = 0;
#elif defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(numLogFiles)
uint8_t numLogFiles = 0;
#endif

SDCardLib sdCardLib;
char dirs[10][MAX_DIR_LEN];
char files[10][MAX_FILE_LEN];
uint8_t dirNum = 0;
uint8_t fileNum = 0;

#define MAX_BUF_SIZE 32
char buffer[MAX_BUF_SIZE];

// FatFs Static Variables
static FIL fil;        /* File object */
static char filename[31];
static FRESULT rc;

void sdcardLog()
{
    //Initialize the ADC12B Module
    /*
     * Base address of ADC12B Module
     * Use internal ADC12B bit as sample/hold signal to start conversion
     * USE MODOSC 5MHZ Digital Oscillator as clock source
     * Use default clock divider/pre-divider of 1
     * Use Temperature Sensor and Battery Monitor internal channels
     */
    ADC12_B_initParam initParam = {0};
    initParam.sampleHoldSignalSourceSelect = ADC12_B_SAMPLEHOLDSOURCE_SC;
    initParam.clockSourceSelect = ADC12_B_CLOCKSOURCE_ACLK;
    initParam.clockSourceDivider = ADC12_B_CLOCKDIVIDER_1;
    initParam.clockSourcePredivider = ADC12_B_CLOCKPREDIVIDER__1;
    initParam.internalChannelMap = ADC12_B_TEMPSENSEMAP | ADC12_B_BATTMAP;
    ADC12_B_init(ADC12_B_BASE, &initParam);

    //Enable the ADC12B module
    ADC12_B_enable(ADC12_B_BASE);

    // Sets up the sampling timer pulse mode
    ADC12_B_setupSamplingTimer(ADC12_B_BASE,
                               ADC12_B_CYCLEHOLD_128_CYCLES,
                               ADC12_B_CYCLEHOLD_128_CYCLES,
                               ADC12_B_MULTIPLESAMPLESENABLE);

    // Maps Temperature Sensor input channel to Memory 0 and select voltage references
    /*
     * Base address of the ADC12B Module
     * Configure memory buffer 0
     * Map input A1 to memory buffer 0
     * Vref+ = IntBuffer
     * Vref- = AVss
     * Memory buffer 0 is not the end of a sequence
     */
    ADC12_B_configureMemoryParam configureMemoryParam = {0};
    configureMemoryParam.memoryBufferControlIndex = ADC12_B_MEMORY_0;
    configureMemoryParam.inputSourceSelect = ADC12_B_INPUT_TCMAP;
    configureMemoryParam.refVoltageSourceSelect =
        ADC12_B_VREFPOS_INTBUF_VREFNEG_VSS;
    configureMemoryParam.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;
    configureMemoryParam.windowComparatorSelect =
        ADC12_B_WINDOW_COMPARATOR_DISABLE;
    configureMemoryParam.differentialModeSelect =
        ADC12_B_DIFFERENTIAL_MODE_DISABLE;
    ADC12_B_configureMemory(ADC12_B_BASE, &configureMemoryParam);

    // Maps Battery Monitor input channel to Memory 0 and select voltage references
    /*
     * Base address of the ADC12B Module
     * Configure memory buffer 0
     * Map input A1 to memory buffer 0
     * Vref+ = IntBuffer
     * Vref- = AVss
     * Memory buffer 0 is not the end of a sequence
     */
    configureMemoryParam.memoryBufferControlIndex = ADC12_B_MEMORY_1;
    configureMemoryParam.inputSourceSelect = ADC12_B_INPUT_BATMAP;
    configureMemoryParam.refVoltageSourceSelect =
        ADC12_B_VREFPOS_INTBUF_VREFNEG_VSS;
    configureMemoryParam.endOfSequence = ADC12_B_ENDOFSEQUENCE;
    configureMemoryParam.windowComparatorSelect =
        ADC12_B_WINDOW_COMPARATOR_DISABLE;
    configureMemoryParam.differentialModeSelect =
        ADC12_B_DIFFERENTIAL_MODE_DISABLE;
    ADC12_B_configureMemory(ADC12_B_BASE, &configureMemoryParam);

    // Clear memory buffer 0 interrupt
    ADC12_B_clearInterrupt(ADC12_B_BASE,
                           0,
                           ADC12_B_IFG1
                           );

    // Enable memory buffer 0 interrupt
    ADC12_B_enableInterrupt(ADC12_B_BASE,
                            ADC12_B_IE1,
                            0,
                            0);

    // Configure internal reference
    while(Ref_A_isRefGenBusy(REF_A_BASE));              // If ref generator busy, WAIT
    Ref_A_enableTempSensor(REF_A_BASE);
    Ref_A_setReferenceVoltage(REF_A_BASE, REF_A_VREF2_0V);
    Ref_A_enableReferenceVoltage(REF_A_BASE);

    // Start timer
    Timer_A_initUpModeParam param = {0};
    param.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    param.timerPeriod = 13;
    param.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    param.captureCompareInterruptEnable_CCR0_CCIE =
    		TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
    param.timerClear = TIMER_A_DO_CLEAR;
    param.startTimer = true;
    Timer_A_initUpMode(TIMER_A0_BASE, &param);

    __bis_SR_register(LPM3_bits | GIE);       // Enter LPM3. Delay for Ref to settle.

    Timer_A_stop(TIMER_A0_BASE);

    P1OUT &= ~BIT1;
    P1DIR |= BIT1;
    P1OUT |= BIT1;

    //Enable/Start sampling and conversion
    /*
     * Base address of ADC12B Module
     * Start the conversion into memory buffer 0
     * Use the single-channel, single-conversion mode
     */
    ADC12_B_startConversion(ADC12_B_BASE,
                            ADC12_B_MEMORY_0,
                            ADC12_B_SEQOFCHANNELS);

    __bis_SR_register(LPM3_bits | GIE);   // Wait for conversion to complete
    __bic_SR_register(GIE);

    ADC12_B_disable(ADC12_B_BASE);

    P1OUT &= ~BIT1;

    char temperature[6];
    itoa(ADC12MEM0, temperature, 10);

    char voltage[6];
    itoa(ADC12MEM1, voltage, 10);

    //Plugin SDcard interface to SDCard lib
    SDCardLib_init(&sdCardLib, &sdIntf_MSP430FR5994LP);

    //Detect SD card
    SDCardLib_Status st = SDCardLib_detectCard(&sdCardLib);
    if (st == SDCARDLIB_STATUS_NOT_PRESENT) {
    	SDCardLib_unInit(&sdCardLib);
    	mode = '0';
    	noSDCard = 1;
    	return;
    }

    rc = f_open(&fil, filename, FA_WRITE | FA_OPEN_EXISTING);
    if (rc) {
    	f_close(&fil);
    	SDCardLib_unInit(&sdCardLib);
    	return;
    }

    rc = f_lseek(&fil, f_size(&fil));

    // Log Temperature ADC conversion results to SDCard
    f_puts(temperature, &fil);
    f_putc(' ', &fil);

    // Log Bat (SuperCap) Voltage ADC conversion results to SDCard
    f_puts(voltage, &fil);
    f_puts("\r\n", &fil);
    rc = f_close(&fil);

    SDCardLib_unInit(&sdCardLib);

    //Enable SPI module
    EUSCI_B_SPI_disable(EUSCI_B0_BASE);

    GPIO_setOutputLowOnPin(
        GPIO_PORT_P2,
        GPIO_PIN2
    );

    //Configure Pins for UCB0CLK
    GPIO_setAsOutputPin(
        GPIO_PORT_P2,
        GPIO_PIN2
    );

    //Configure Pins for UCB0TXD/UCB0SIMO, UCB0RXD/UCB0SOMI
    GPIO_setOutputLowOnPin(
        GPIO_PORT_P1,
        GPIO_PIN6 + GPIO_PIN7
    );

    //Set CS pin high
    GPIO_setOutputHighOnPin(
        GPIO_PORT_P4,
        GPIO_PIN0
        );

    //Configure Pins for UCB0TXD/UCB0SIMO, UCB0RXD/UCB0SOMI
    GPIO_setAsOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN6 + GPIO_PIN7
    );

    if (mode == SDCARD_LOG_MODE)
    {
        GPIO_selectInterruptEdge(GPIO_PORT_P5, GPIO_PIN5, GPIO_HIGH_TO_LOW_TRANSITION);
        GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN5);
        GPIO_clearInterrupt(GPIO_PORT_P5, GPIO_PIN5);
        GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN5);

        // disable USCI_A0 RX interrupt
        EUSCI_A_UART_disableInterrupt(EUSCI_A0_BASE,
                                     EUSCI_A_UART_RECEIVE_INTERRUPT);

        /* Request the disabling of the core voltage regulator when device enters
         LPM3 (or LPM4) so that we can effectively enter LPM3.5 (or LPM4.5) */
        // Commented out to stay in LPM3 due to device errata (RTC time event doesn't wake up from LPM3.5)
//        PMM_turnOffRegulator();

        //Enter LPM3 mode with interrupts enabled
        __bis_SR_register(LPM3_bits + GIE);
        __no_operation();
    }
}

/*
 * Send SDCardLogMode starting TimeStamp through UART
 */
void sendTimeStampSDCard()
{

    //Plugin SDcard interface to SDCard lib
    SDCardLib_init(&sdCardLib, &sdIntf_MSP430FR5994LP);

    //Detect SD card
    SDCardLib_Status st = SDCardLib_detectCard(&sdCardLib);
    if (st == SDCARDLIB_STATUS_NOT_PRESENT) {
    	SDCardLib_unInit(&sdCardLib);
    	mode = '0';
    	noSDCard = 1;
    	return;
    }

    // Construct log file's name
    if (numLogFiles == 0) numLogFiles++;
    strcpy(filename, "data_log/log_");
    char num[5];
    itoa(numLogFiles, num, 10);
    strcat(filename, num);
    strcat(filename, ".txt");

    rc = f_open(&fil, filename, FA_READ | FA_OPEN_EXISTING);
    if (rc) {
    	strcpy(buffer, "1234567890123");
    }
    else {
		rc = f_lseek(&fil, 28);

		// Reads TimeStamp from SDCard log file
		f_gets(buffer, MAX_BUF_SIZE, &fil);
    }
    rc = f_close(&fil);

    SDCardLib_unInit(&sdCardLib);

	// Select UART TXD on P2.0
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN0, GPIO_SECONDARY_MODULE_FUNCTION);
    __delay_cycles(900000);
    int i;
    for (i=0;i<13;i++) {
        EUSCI_A_UART_transmitData(EUSCI_A0_BASE, buffer[i]);
    }

    __delay_cycles(900000);

    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
	GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
}

/*
 * Send SDCard temperature and voltage data through UART
 */
void sendDataSDCard()
{
    //Plugin SDcard interface to SDCard lib
    SDCardLib_init(&sdCardLib, &sdIntf_MSP430FR5994LP);

    //Detect SD card
    SDCardLib_Status st = SDCardLib_detectCard(&sdCardLib);
    if (st == SDCARDLIB_STATUS_NOT_PRESENT) {
    	SDCardLib_unInit(&sdCardLib);
    	mode = '0';
    	noSDCard = 1;
    	return;
    }

    // Construct log file's name
    if (numLogFiles == 0) numLogFiles++;
    strcpy(filename, "data_log/log_");
    char num[5];
    itoa(numLogFiles, num, 10);
    strcat(filename, num);
    strcat(filename, ".txt");

    uint16_t numData = 0;

    rc = f_open(&fil, filename, FA_READ | FA_OPEN_EXISTING);
    if (rc) {
    	numData = 0;
    }
    else {
		f_gets(buffer, MAX_BUF_SIZE, &fil);
		f_gets(buffer, MAX_BUF_SIZE, &fil);
		f_gets(buffer, MAX_BUF_SIZE, &fil);
		f_gets(buffer, MAX_BUF_SIZE, &fil);
		f_gets(buffer, MAX_BUF_SIZE, &fil);

		while (f_gets(buffer, MAX_BUF_SIZE, &fil) != NULL)
		{
			numData++;
		}
    }

    rc = f_close(&fil);

	// Select UART TXD on P2.0
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN0, GPIO_SECONDARY_MODULE_FUNCTION);

    // Send FRAM Index
    EUSCI_A_UART_transmitData(EUSCI_A0_BASE, (uint8_t)((numData)>>8));
    EUSCI_A_UART_transmitData(EUSCI_A0_BASE, (uint8_t)(numData));

    __delay_cycles(900000);

    rc = f_open(&fil, filename, FA_READ | FA_OPEN_EXISTING);
    if (rc) {
    	f_close(&fil);
    	SDCardLib_unInit(&sdCardLib);
    	return;
    }

    f_gets(buffer, MAX_BUF_SIZE, &fil);
    f_gets(buffer, MAX_BUF_SIZE, &fil);
    f_gets(buffer, MAX_BUF_SIZE, &fil);
    f_gets(buffer, MAX_BUF_SIZE, &fil);
    f_gets(buffer, MAX_BUF_SIZE, &fil);

    uint16_t i;
    char *dataStr;
    int data;

    for (i=0;i<numData;i++)
    {
        // Reads TimeStamp from SDCard log file
        f_gets(buffer, MAX_BUF_SIZE, &fil);
        dataStr = strtok(buffer, " ");
        data = atoi(dataStr);
    	// Send logged Temperature Sensor ata
    	EUSCI_A_UART_transmitData(EUSCI_A0_BASE, (uint8_t)(data>>8));
    	EUSCI_A_UART_transmitData(EUSCI_A0_BASE, (uint8_t)(data));

        dataStr = strtok(NULL, " ");
        data = atoi(dataStr);
    	// Send logged Battery Monitor data
    	EUSCI_A_UART_transmitData(EUSCI_A0_BASE, (uint8_t)(data>>8));
    	EUSCI_A_UART_transmitData(EUSCI_A0_BASE, (uint8_t)(data));
    }

    rc = f_close(&fil);
    SDCardLib_unInit(&sdCardLib);

    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);

    mode = '0';
}

/*
 * Store TimeStamp from PC when logging starts to SDCard
 */
void storeTimeStampSDCard()
{
	int i = 0;
	uint16_t bw = 0;
//	FRESULT rc;

    // Increment log file number
    numLogFiles++;

    for(i = 0; i<13; i++)
    {
        __bis_SR_register(LPM3_bits | GIE);       // Enter LPM3. Delay for Ref to settle.
        __no_operation();

        buffer[i] = EUSCI_A_UART_receiveData(EUSCI_A0_BASE);
    }
    buffer[13] = 0;

    //Configure SDCard CS pin
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, BIT0);
    GPIO_setAsOutputPin(GPIO_PORT_P4, BIT0);

    //Plugin SDcard interface to SDCard lib
    SDCardLib_init(&sdCardLib, &sdIntf_MSP430FR5994LP);

    //Detect SD card
    SDCardLib_Status st = SDCardLib_detectCard(&sdCardLib);
    if (st == SDCARDLIB_STATUS_NOT_PRESENT) {
    	SDCardLib_unInit(&sdCardLib);
    	mode = '0';
    	return;
    }

    //Read directory and file
    rc = SDCardLib_getDirectory(&sdCardLib, "data_log", dirs, &dirNum, files, &fileNum);

    //Create the directory under the root directory
    rc = SDCardLib_createDirectory(&sdCardLib, "data_log");
    if (rc != FR_OK && rc != FR_EXIST) {
    	SDCardLib_unInit(&sdCardLib);
    	mode = '0';
    	return;
    }

    char * temp = "SDCard Logging Start Time:\r\n";

    // Construct log file's name
    strcpy(filename, "data_log/log_");
    char num[5];
    itoa(numLogFiles, num, 10);
    strcat(filename, num);
    strcat(filename, ".txt");

    // Write new log file
    rc = SDCardLib_writeFile(&sdCardLib, filename, temp, 28, &bw);
    if (rc != FR_OK) {
    	SDCardLib_unInit(&sdCardLib);
    	mode = '0';
    	return;
    }

    // Write time stamp to log file
    rc = f_open(&fil, filename, FA_WRITE | FA_OPEN_EXISTING);
    if (rc) {
    	f_close(&fil);
    	SDCardLib_unInit(&sdCardLib);
    	mode = '0';
    	return;
    }

    rc = f_lseek(&fil, f_size(&fil));
    f_puts(buffer, &fil);
    f_puts("\r\n\r\n", &fil);
    f_puts("Temperature and Voltage (12-bit ADC raw data):\r\n", &fil);
    rc = f_close(&fil);

	SDCardLib_unInit(&sdCardLib);
}

/*
 * Converts integer to string
 */
char *itoa(int num, char *str, int radix) {
    char sign = 0;
    char temp[17];  //an int can only be 16 bits long
                    //at radix 2 (binary) the string
                    //is at most 16 + 1 null long.
    int temp_loc = 0;
    int digit;
    int str_loc = 0;

    //save sign for radix 10 conversion
    if (radix == 10 && num < 0) {
        sign = 1;
        num = -num;
    }

    //construct a backward string of the number.
    do {
        digit = (unsigned int)num % radix;
        if (digit < 10)
            temp[temp_loc++] = digit + '0';
        else
            temp[temp_loc++] = digit - 10 + 'A';
        num = (((unsigned int)num) / radix);
    } while ((unsigned int)num > 0);

    //now add the sign for radix 10
    if (radix == 10 && sign) {
        temp[temp_loc] = '-';
    } else {
        temp_loc--;
    }


    //now reverse the string.
    while ( temp_loc >=0 ) {// while there are still chars
        str[str_loc++] = temp[temp_loc--];
    }
    str[str_loc] = 0; // add null termination.

    return str;
}
