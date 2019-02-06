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
#include "inc/hw_memmap.h"

#include "adc10_a.h"
#include "adc12_a.h"
#include "aes.h"
#include "battbak.h"
#include "comp_b.h"
#include "crc.h"
#include "dac12_a.h"
#include "dma.h"
#include "eusci_a_spi.h"
#include "eusci_a_uart.h"
#include "eusci_b_i2c.h"
#include "eusci_b_spi.h"
#include "flashctl.h"
#include "gpio.h"
#include "ldopwr.h"
#include "mpy32.h"
#include "pmap.h"
#include "pmm.h"
#include "ram.h"
#include "ref.h"
#include "rtc_a.h"
#include "rtc_b.h"
#include "rtc_c.h"
#include "sd24_b.h"
#include "sfr.h"
#include "sysctl.h"
#include "tec.h"
#include "timer_a.h"
#include "timer_b.h"
#include "timer_d.h"
#include "tlv.h"
#include "ucs.h"
#include "usci_a_spi.h"
#include "usci_a_uart.h"
#include "usci_b_i2c.h"
#include "usci_b_spi.h"
#include "wdt_a.h"
