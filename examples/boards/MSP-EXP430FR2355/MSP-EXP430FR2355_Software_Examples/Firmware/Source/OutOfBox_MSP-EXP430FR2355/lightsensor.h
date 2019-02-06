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
//  lightsensor.h
//
//  Uses SAC2 in generic Op-Amp mode along with onboard photodiode, capacitor, and 
//  resistor to implement a light sensor circuit. The resulting voltage is buffered
//  through SAC0, measured by the internal ADC, and used to control Timer PWMs
//  dutycycle to modulate the brightness of LED1 and LED2.
//
//  E. Chen
//  Texas Instruments Inc.
//  May 2018
//******************************************************************************

#ifndef LIGHTSENSOR_H_
#define LIGHTSENSOR_H_

#define LIGHTSENSOR_MODE 0

#define TIMER_PERIOD 650
#define DUTY_CYCLE  325

// Appliation mode
extern char mode;

/* FRAM Variable that stores lightsensor ADC results*/
extern unsigned int lightsensor_ADC_Result;          // ADC conversion result

void lightsensor(void);
void lightsensor_init_GPIO(void);
void lightsensor_init_LED_PWM(void);
void lightsensor_init_SACOA(void);
void lightsensor_init_ADC(void);


#endif /* LIGHTSENSOR_H_ */
