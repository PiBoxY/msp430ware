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
/*******************************************************************************
 *
 * FRAMLogMode.c
 *
 * Wakes up every 5 seconds from LPM3 to measure and store its
 * internal temperature sensor & battery monitor data to FRAM
 *
 * October 2017
 * E. Chen
 *
 ******************************************************************************/

#include "FRAMLogMode.h"

void framLog()
{
    _q8 DegC;          // Q variables using global type
    uint16_t status;

    RTC_start(RTC_BASE, RTC_CLOCKSOURCE_VLOCLK);

    while (mode == FRAM_LOG_MODE)
    {
        if (buttonS1Pressed)
        {
            initEusci();

            // Add delay to ensure system clock stabilizes after waking up from LPM3.5
            __delay_cycles(100000);

            buttonS1Pressed = false;

            // Transmit JSON formatted string
            int i = 0;
            int length = nvs_ring_entries(nvsHandle);
            transmitString("{\"framTempData\":[");
            for (i = 0; i < length; i++)
            {
                adc_data_t data;
                nvs_ring_retrieve(nvsHandle, &data, i);

                DegC = _Q8(((int16_t)(data.value - CALADC_15V_30C)) * (85.0f-30.0f) / (CALADC_15V_85C - CALADC_15V_30C) + 30.0f);
                char str[MAX_STRBUF_SIZE];
                _Q8toa(str, "%2.2f", DegC);
                transmitString(str);
                if (i < length-1)
                    transmitString(", ");
            }
            transmitString("]}");
            while (EUSCI_A_UART_queryStatusFlags(EUSCI_A0_BASE, EUSCI_A_UART_BUSY));
            EUSCI_A_UART_transmitData(EUSCI_A0_BASE, '\n');
        }
        if (buttonS2Pressed)
        {
            buttonS2Pressed = false;
            nvs_ring_reset(nvsHandle);
        }
        if (rtcWakeup)
        {
            rtcWakeup = false;

            initAdc();

            //Enable and Start the conversion
            //in Single-Channel, Single Conversion Mode
            ADC_startConversion(ADC_BASE,
                                ADC_SINGLECHANNEL);

            __bis_SR_register(LPM3_bits | GIE);

            // Add adc_data to ring storage
            status = nvs_ring_add(nvsHandle, &adc_data);

            /*
             * Status should never be not NVS_OK but if it happens trap execution.
             * Potential reason for NVS_NOK:
             *     1. nvsStorage not initialized
             *     2. nvsStorage got corrupted by other task (buffer overflow?)
             */
            if (status != NVS_OK) {
                while (1);
            }

            P1OUT &= ~BIT0;
        }

        // Save peripheral, stack and cpu context and enter into LPM3.5
        ctpl_enterLpm35(CTPL_DISABLE_RESTORE_ON_RESET);
    }
}
