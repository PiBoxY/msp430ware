/*******************************************************************************
 *
 *  hal_adc.c
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

int SavedADC12MEM0 = 0, SavedADC12MEM1 = 0, SavedADC12MEM2 = 0;
int Acc_x = 0, Acc_y = 0, Acc_z = 0;
int Acc_x_offset = 0, Acc_y_offset = 0, Acc_z_offset = 0;
long int Vcc = 0;

unsigned char adcMode = ADC_OFF_MODE;
unsigned char exit_active_from_ADC12 = 0;

unsigned long temp_temp, temp_vcc;

unsigned int buffer[1024];                 // Used for FFT
unsigned int buf_index;                    // Used for FFT

/**********************************************************************//**
 * @brief  Turns on and initializes ADC12, accelerometer in order to
 *         sample x, y, z-axis inputs.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void halAccelerometerInit(void)
{
    //Set internal mode variable to accelerometer
    adcMode = ADC_ACC_MODE;

    //Configure GPIO
    ACC_PORT_SEL |= ACC_X_PIN + ACC_Y_PIN; //Enable A/D channel inputs
    ACC_PORT_DIR &= ~(ACC_X_PIN + ACC_Y_PIN + ACC_Z_PIN);
    ACC_PORT_DIR |= ACC_PWR_PIN;           //Enable ACC_POWER
    ACC_PORT_OUT |= ACC_PWR_PIN;

    //Sequence of channels, once, ADC12CLK=MODOSC
    UCSCTL8 |= MODOSCREQEN;
    ADC12CTL0 &= ~ADC12ENC;                // Ensure ENC is clear
    ADC12CTL0 = ADC12ON + ADC12SHT02 + ADC12MSC;
    ADC12CTL1 = ADC12SHP + ADC12CONSEQ_1 + ADC12SSEL_0;
    ADC12CTL2 = ADC12RES_2;
    ADC12MCTL0 = ACC_X_CHANNEL;
    ADC12MCTL1 = ACC_Y_CHANNEL;
    ADC12MCTL2 = ACC_Z_CHANNEL + ADC12EOS;
    ADC12IE = 0x00;

    // Allow the accelerometer to settle before sampling any data
    __delay_cycles(200000);
}

/**********************************************************************//**
 * @brief  Calibrates the offset values for x, y, and z axes.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void halAccelerometerCalibrate(void)
{
    unsigned char tempQuit;

    tempQuit = exit_active_from_ADC12;
    halAdcSetQuitFromISR(1);
    halAdcStartRead();

    __bis_SR_register(LPM3_bits + GIE);
    __no_operation();

    halAccelerometerReadWithOffset(&Acc_x_offset, &Acc_y_offset, &Acc_z_offset);
    halAdcSetQuitFromISR(tempQuit);
}

/**********************************************************************//**
 * @brief  Set function for the calibrated offsets for the x, y, and z axes.
 *
 * @param  x Calibrated offset for the x-axis
 *
 * @param  y Calibrated offset for the y-axis
 *
 * @param  z Calibrated offset for the z-axis
 *
 * @return none
 *************************************************************************/

void halAccelerometerSetCalibratedOffset(int x, int y, int z)
{
    Acc_x_offset = x;
    Acc_y_offset = y;
    Acc_z_offset = z;
}

/**********************************************************************//**
 * @brief  Get function for the x, y, and z axes calibrated offsets
 *
 * @param  x Pointer to the calibrated offset for the x-axis
 *
 * @param  y Pointer to the calibrated offset for the y-axis
 *
 * @param  z Pointer to the calibrated offset for the z-axis
 *
 * @return none
 *************************************************************************/

void halAccelerometerGetCalibratedOffset(int *x, int *y, int *z)
{
    *x = Acc_x_offset;
    *y = Acc_y_offset;
    *z = Acc_y_offset;
}

/**********************************************************************//**
 * @brief  Get function for the x, y, and z accelerometer samples,
 *         including the calibrated offsets.
 *
 * @param  x Pointer to the accelerometer reading (x-axis)
 *
 * @param  y Pointer to the accelerometer reading (y-axis)
 *
 * @param  z Pointer to the accelerometer reading (z-axis)
 *
 * @return none
 *************************************************************************/

void halAccelerometerRead(int *x, int *y, int *z)
{
    Acc_x = SavedADC12MEM0;
    Acc_y = SavedADC12MEM1;
    Acc_z = SavedADC12MEM2;

    *x = Acc_x - Acc_x_offset;
    *y = Acc_y - Acc_y_offset;
    *z = Acc_z - Acc_z_offset;
}

/**********************************************************************//**
 * @brief  Get function for the x, y, and z accelerometer samples,
 *         excluding the calibrated offsets.
 *
 * @param  x Pointer to the accelerometer reading (x-axis)
 *
 * @param  y Pointer to the accelerometer reading (y-axis)
 *
 * @param  z Pointer to the accelerometer reading (z-axis)
 *
 * @return none
 *************************************************************************/

void halAccelerometerReadWithOffset(int *x, int *y, int *z)
{
    *x = SavedADC12MEM0;
    *y = SavedADC12MEM1;
    *z = SavedADC12MEM2;
}

/**********************************************************************//**
 * @brief  Disables the ADC12, accelerometer that sampled x, y, z-axis inputs.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void halAccelerometerShutDown(void)
{
    //Turn off ADC module and power to the accelerometer
    ADC12CTL0 &= ~ADC12ENC;
    ADC12CTL0 = 0x00;

    ACC_PORT_OUT &= ~ACC_PWR_PIN;           //Disable ACC_POWER

    //Disable A/D channel inputs
    ACC_PORT_SEL &= ~(ACC_X_PIN + ACC_Y_PIN + ACC_Z_PIN);
    ACC_PORT_DIR |= (ACC_X_PIN + ACC_Y_PIN + ACC_Z_PIN + ACC_PWR_PIN);
    ACC_PORT_OUT &= ~(ACC_X_PIN + ACC_Y_PIN + ACC_Z_PIN + ACC_PWR_PIN);

    adcMode = ADC_OFF_MODE;
}

/*----------------------------------------------------------------------------*/

/**********************************************************************//**
 * @brief  Intializes the ADC12 to sample Vcc.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void halAdcInitVcc(void)
{
    //Set internal mode variable to temperature/VCC
    adcMode = ADC_TEMP_MODE;

    //Sequence of channels, once, ADC12CLK=MODOSC
    UCSCTL8 |= MODOSCREQEN;
    REFCTL0 &= ~REFVSEL_3;
    REFCTL0 |= REFVSEL_2 + REFON;                // Configure internal 2.5V reference
    __delay_cycles(1900);                        // delay for reference to settle

    ADC12CTL0 &= ~ADC12ENC;                      // Ensure ENC is clear
    ADC12CTL0 = ADC12ON + ADC12SHT0_15 + ADC12MSC;
    ADC12CTL1 = ADC12SHP + ADC12CONSEQ_1 + ADC12SSEL_0;
    ADC12CTL2 = ADC12RES_2;
    ADC12MCTL0 = ADC12SREF_1 + TEMP_CHANNEL;
    ADC12MCTL1 = ADC12SREF_1 + VCC_CHANNEL + ADC12EOS;
    ADC12IE = 0x00;
}

/**********************************************************************//**
 * @brief  Turns off / disable the ADC12.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void halAdcShutDownVcc(void)
{
    //Turn off ADC module and internal reference
    ADC12CTL0 &= ~ADC12ENC;
    ADC12CTL0 = 0x00;
    ADC12IE = 0x00;
    REFCTL0 &= ~REFON;

    adcMode = ADC_OFF_MODE;
}

/**********************************************************************//**
 * @brief  Get function for the current Vcc value.
 *
 * @param  none
 *
 * @return The current Vcc value.
 *************************************************************************/

int halAdcGetVcc(void)
{
    return Vcc;
}

/**********************************************************************//**
 * @brief  Converts the Vcc readings from the ADC to BCD format.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void halAdcConvertVccFromADC(void)
{
    // Convert Vcc
    Vcc = SavedADC12MEM1;
    Vcc = Vcc * 50 + 2048;                      // add .5*4096 so truncation rounds up
    Vcc = Vcc / 4096;
}

/**********************************************************************//**
 * @brief  Get function for the Vcc samples in "x.xV" format.
 *
 * @param  Vcc            The string that holds the Vcc reading
 *
 * @return none
 *************************************************************************/

void halAdcReadVcc(char *VccStr)
{
    long int dummyVcc;

    halAdcConvertVccFromADC();
    dummyVcc = Vcc;

    VccStr[0] = '0';
    VccStr[2] = '0';
    while (Vcc >= 10)
    {
        VccStr[0]++;
        Vcc -= 10;
    }
    VccStr[2] += Vcc;
    Vcc = dummyVcc;
}

/*----------------------------------------------------------------------------*/

/**********************************************************************//**
 * @brief  Starts the ADC conversion.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void halAdcStartRead(void)
{
    ADC12IFG = 0x00;                        // Clear all ADC12 channel int flags

    if (adcMode == ADC_ACC_MODE)
    {
        ADC12IE = BIT2;                     // Enable channel A2 interrupt
        ADC12CTL0 |= ADC12ENC | ADC12SC;    // Start conversion
    }
    else
    {
        REFCTL0 |= REFON;                   // Turn on ADC12 reference
        __delay_cycles(1900);               // delay for reference to settle

        ADC12IE = BIT1;                     // Enable channel A1 interrupt
        ADC12CTL0 |= ADC12ENC | ADC12SC;    // Start conversion
    }
}

/**********************************************************************//**
 * @brief  Sets the flag that causes an exit into active CPU mode from
 *         the ADC12 ISR.
 *
 * @param  quit
 *
 * - 1 - Exit active from ADC12 ISR
 * - 0 - Remain in LPMx on exit from ADC12ISR
 *
 * @return none
 *************************************************************************/

void halAdcSetQuitFromISR(unsigned char quit)
{
    exit_active_from_ADC12 = quit;
}

/*----------------------------------------------------------------------------*/
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    if (adcTempISR == 1)
    {
        switch (__even_in_range(ADC12IV, ADC12IV_ADC12IFG15))
        {
            case  ADC12IV_NONE: break;        // Vector  0:  No interrupt
            case  ADC12IV_ADC12OVIFG: break;  // Vector  2:  ADC overflow
            case  ADC12IV_ADC12TOVIFG: break; // Vector  4:  ADC timing overflow
            case  ADC12IV_ADC12IFG0: break;   // Vector  6:  ADC12IFG0
            case  ADC12IV_ADC12IFG1:          // Vector  8:  ADC12IFG1
                ADC12CTL0 &= ~ADC12ENC;       // Disable ADC12 conversions
                REFCTL0 &= ~REFON;            // Disable internal reference
                temp_temp = ADC12MEM0;        // Read ADC12 temp measurement, clear IFG
                temp_vcc  = ADC12MEM1;        // Read ADC12 vcc/2 measurement, clear IFG
                __bic_SR_register_on_exit(LPM3_bits);
                break;
            case ADC12IV_ADC12IFG2: break;    // Vector 10:  ADC12IFG2
            case ADC12IV_ADC12IFG3: break;    // Vector 12:  ADC12IFG3
            case ADC12IV_ADC12IFG4: break;    // Vector 14:  ADC12IFG4
            case ADC12IV_ADC12IFG5: break;    // Vector 16:  ADC12IFG5
            case ADC12IV_ADC12IFG6: break;    // Vector 18:  ADC12IFG6
            case ADC12IV_ADC12IFG7: break;    // Vector 20:  ADC12IFG7
            case ADC12IV_ADC12IFG8: break;    // Vector 22:  ADC12IFG8
            case ADC12IV_ADC12IFG9: break;    // Vector 24:  ADC12IFG9
            case ADC12IV_ADC12IFG10: break;   // Vector 26:  ADC12IFG10
            case ADC12IV_ADC12IFG11: break;   // Vector 28:  ADC12IFG11
            case ADC12IV_ADC12IFG12: break;   // Vector 30:  ADC12IFG12
            case ADC12IV_ADC12IFG13: break;   // Vector 32:  ADC12IFG13
            case ADC12IV_ADC12IFG14: break;   // Vector 34:  ADC12IFG14
            case ADC12IV_ADC12IFG15: break;   // Vector 34:  ADC12IFG14
        }
    }
    else if (fftISR == 1)                     // FFT service routine
    {
        buffer[buf_index++] = ADC12MEM0;
        buffer[buf_index++] = 0;

        if (buf_index == 1024){
            buf_index = 0;
            __bic_SR_register_on_exit(LPM0_bits);
        }
        else if (buf_index == 512){
            __bic_SR_register_on_exit(LPM0_bits);
        }
    }
    else
    {
        ADC12CTL0 &= ~ADC12ENC;               // Disable ADC12 conversions
        REFCTL0 &= ~REFON;                    // Disable reference (if enabled)

        SavedADC12MEM0 = ADC12MEM0;           // Store the sampled data
        SavedADC12MEM1 = ADC12MEM1;
        SavedADC12MEM2 = ADC12MEM2;

        if (exit_active_from_ADC12){
            __bic_SR_register_on_exit(LPM3_bits);
        }
    }
}

