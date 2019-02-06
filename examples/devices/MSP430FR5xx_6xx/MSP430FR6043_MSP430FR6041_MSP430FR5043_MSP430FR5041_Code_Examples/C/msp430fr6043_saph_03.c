/* --COPYRIGHT--,BSD_EX
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
//   MSP430FR60xx Demo - Pulse generation using the SAPH PPG and PHY.
//
//   Description: Configure the PPG and PHY in the SAPH module manually (register
//   mode instead of ASQ controlled) to generate 6(Can be more) sets of high frequency
//   pulses on the dedicated CH0_OUT pin in Multi Tone mode, approximately once per second.
//   6 sets of pulses(Can be defined by users):
//          Pulses number: 2-4-3-3-4-4
//          Pulses frequency(KHz): 625-1250-277-625-178-384
//   ***NOTE: For the limitation of the DMA transmission speed, the minimum duration
//            time of every set of pulses should be more than 4*(3+9) MCLK.***
//
//           MSP430FR6043
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |           P1.0|---> LED
//        |               |-USSXTIN
//        |               |-USSXTOUT
//        |        CH0_OUT|---> 6 sets of pulses ~1/sec
//
//   Gary Gao & Matt Calvo
//   Texas Instruments Inc.
//   February 2018
//   Built with IAR Embedded Workbench V7.10 & Code Composer Studio V7.3
//******************************************************************************
#include <msp430.h>

unsigned int RAM_XE[20]; // => 3 E-puls phases x 3 Register + 2 X-puls phases x 3 register+3 STOP
unsigned int RAM_DMA_CONFIG[60];    // => (3 E-puls +  2 X-puls + 1 STOP)*9
//The address of RAM_DMA_CONFIG[] should be within 0xFFFF to make the minimum number
//of transmit data by DMA3 which is 9
unsigned int epulse[3]={4,3,4};     // E-pulses number
unsigned int hper[3]  ={8,16,26};   // High phase
unsigned int lper[4]  ={8,16,26};   // Low phase

unsigned int xpulse[2]={3,4};       // X-pulses number
unsigned int xhper[2] ={36,56};     // High phasea
unsigned int xlper[2] ={36,56};     // Low phase

void HSPLL_init(void);
void setup_DMA();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // Stop WDT

    // Configure P1.0 as output for LED
    P1OUT &= ~BIT0;
    P1DIR |= BIT0;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;
    FRCTL0 = FRCTLPW | NWAITS_1;

    // Clock System Setup
    CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    // Per Device Errata(CS12) set divider to 4 before changing frequency to
    // prevent out of spec operation from overshoot transient

    // Set all corresponding clk sources to divide by 4 for errata(CS12)
    CSCTL3 = DIVA__4 | DIVS__4 | DIVM__4;
    CSCTL1 = DCOFSEL_4 | DCORSEL;           // Set DCO to 16MHz
    // Delay by ~10us to let DCO settle. 60 cycles = 20 cycles buffer + (10us / (1/4MHz))
    __delay_cycles(60);
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // Set all dividers to 1 for 16MHz operation
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // MCLK = SMCLK = 16MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL0_H = 0;                           // Lock CS registers

    HSPLL_init();                           // Initialize the HSPLL and wait for it to lock

    setup_DMA();                            // Initialize the DMA

    // Set up the PPG settings
    SAPHKEY = KEY;                          // Unlock the SAPH registers
    SAPHPGCTL = TRSEL_2 | PPGCHSEL_0 | PGSEL_0 ; // TA2.1 trigger, CH0 output, register mode
    // Set up the PHY to output PPG on dedicated CH0_OUT pin
    SAPHOSEL = PCH0SEL__PPGSE;              // Output PPG
    // Enable the PPG
    SAPHPGCTL |= PPGEN|PSCEN;

    // Configure TA2.1 for ~1/sec to trigger the pulse generation and toggle LED
    TA2CCR0 = 9400;
    TA2CCR1 = 4700;
    TA2CCTL1 = OUTMOD_7 | CCIE;             // Enable output signal to trigger PPG, enable Interrupt
    TA2CTL = TASSEL__ACLK | TACLR | MC__UP; // Timer sourced from ACLK (VLO), clear timer

    while(1)
    {
       __bis_SR_register(LPM0_bits | GIE);  // Enter LPM0 enable interrupt
       __no_operation();
    }
}

// Timer A2 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER2_A1_VECTOR
__interrupt void Timer2_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER2_A1_VECTOR))) Timer2_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(TA2IV, TAIV__TAIFG))
    {
        case TAIV__NONE:   break;           // No interrupt
        case TAIV__TACCR1:

            SAPH_AXPGCTL=0x7000 | 0x0002;   // Multi Tone mode, 2 Extra excitation pulses
            SAPH_AXPGLPER=16;               // Low phase
            SAPH_AXPGHPER=16;               // High phase
            // Repeated block transfer, increase source address, increase dst address
            DMA2CTL = 0x5000|DMASRCINCR|DMADSTINCR;
            __data20_write_long((unsigned long)(&DMA2SA),(unsigned long)(&RAM_XE));
            __data20_write_long((unsigned long)(&DMA2DA),(unsigned long)(&SAPHPGC));

            DMA2SZ = 0x0003;                // 3 registers per block
            DMA2CTL |= DMAEN;
            // Write new SA and DA of DMA2
            __data20_write_long((unsigned long)(&DMA2SA),(unsigned long)(RAM_XE+3));
            __data20_write_long((unsigned long)(&DMA2DA),(unsigned long)(&SAPHXPGCTL));


            // Repeated block transfer, increase source address, increase dst address
            DMA3CTL = 0x5000|DMASRCINCR|DMADSTINCR;
            __data20_write_long((unsigned long)(&DMA3SA),(unsigned long)(&RAM_DMA_CONFIG));
            __data20_write_long((unsigned long)(&DMA3DA),(unsigned long)(&DMA2SA));// DA is SA of DMA2
            DMA3SZ = 0x0009;                // 10 DMA registers
            DMA3CTL |= DMAEN;
            // Write new SA of DMA3
            __data20_write_long((unsigned long)(&DMA3SA),(unsigned long)(RAM_DMA_CONFIG + 9));

            P1OUT ^= BIT0;                  // Toggle LED to show new cycle
            break;
        case TAIV__TAIFG: break;            // overflow
        default: break;
    }
}

void HSPLL_init(void)
{
    // Configure USSXT Oscillator
    HSPLLUSSXTLCTL = OSCTYPE_0 |  USSXTEN;

    // Set up timer to wait in LPM for crystal stabilization time = 4096 clocks for crystal resonator.
    // For 8MHz XTAL, 4096 clocks = 512us. Using VLO = 9.4kHz, wait 5 timer clock cycles = 532us.
    TA4CCR0 = 5;
    TA4CCTL0 = CCIE;                        // Enable Interrupt
    TA4CTL = TASSEL__ACLK | TACLR | MC__UP; // Timer sourced from ACLK (VLO), clear timer
    __bis_SR_register(LPM3_bits | GIE);     // Enter LPM3 w/interrupt
    __no_operation();                       // For debug

    // Check if oscillator is stable
    while((HSPLLUSSXTLCTL & OSCSTATE) == 0);

    // Output oscillator on pin
    HSPLLUSSXTLCTL &= ~XTOUTOFF;

    // Init PLL
    // Use the PLLM setting to get 80MHz output from our 8MHz input
    // Equation: PLL output clock frequency x 2 = input clock frequency x (PLLM+1)
    // Input clock frequency = 8MHz
    // Desired PLL output clock frequency = 80MHz
    // PLLM = 19
    HSPLLCTL = PLLM4 | PLLM1 | PLLM0 | PLLINFREQ; //PLLM = 19, PLL input frequency > 6MHz

    // Power up the UUPS to start the PLL
    UUPSCTL |= USSPWRUP;

    // Wait for UUPS to power up
    while((UUPSCTL & UPSTATE_3) != UPSTATE_3);

    // Wait for PLL to lock
    while(!(HSPLLCTL & PLL_LOCK));
}

// Timer A4 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER4_A0_VECTOR
__interrupt void Timer4_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER4_A0_VECTOR))) Timer4_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    // Stop the timer and wake from LPM
    TA4CTL = MC__STOP;
    __bic_SR_register_on_exit(LPM3_bits | GIE);
    __no_operation();
}

void setup_DMA()
{
    int i;
    unsigned long SAPHPGC_addr,SAPHXPGCTL_addr;
    SAPHPGC_addr=(unsigned long)(&SAPHPGC);
    SAPHXPGCTL_addr=(unsigned long)(&SAPHXPGCTL);
       // DMA2 and DMA3 triggered by PPG, DMA2 has higher priority
       DMACTL1 = DMA2TSEL_27|DMA3TSEL_27;
       // Repeated block transfer, increase source address, increase dst address
       SAPH_AXPGCTL=0x7000 | 0x0002;        // Multi Tone mode, 2 Extra excitation pulses
       SAPH_AXPGLPER=16;                    // Low phase
       SAPH_AXPGHPER=16;                    // High phase
       // Repeated block transfer, increase source address, increase dst address
       DMA2CTL = 0x5000|DMASRCINCR|DMADSTINCR;
       __data20_write_long((unsigned long)(&DMA2SA),(unsigned long)(&RAM_XE));
       __data20_write_long((unsigned long)(&DMA2DA),(unsigned long)(&SAPHPGC));

       DMA2SZ = 0x0003;                     // 3 registers per block
       DMA2CTL |= DMAEN;
       // Write new SA and DA of DMA2
       __data20_write_long((unsigned long)(&DMA2SA),(unsigned long)(RAM_XE+3));
       __data20_write_long((unsigned long)(&DMA2DA),(unsigned long)(&SAPHXPGCTL));


       // Repeated block transfer, increase source address, increase dst address
       DMA3CTL = 0x5000|DMASRCINCR|DMADSTINCR;
       __data20_write_long((unsigned long)(&DMA3SA),(unsigned long)(&RAM_DMA_CONFIG));
       __data20_write_long((unsigned long)(&DMA3DA),(unsigned long)(&DMA2SA));// DA is SA of DMA2
       DMA3SZ = 0x0009;                     // 10 DMA registers
       DMA3CTL |= DMAEN;
       // Write new SA of DMA3
       __data20_write_long((unsigned long)(&DMA3SA),(unsigned long)(RAM_DMA_CONFIG + 9));


    // Setup RAM
        // RAM to be transfered to DMA2 and DMA3                              // DST Addresses
        for(i=0;i<5;i++){
           *(RAM_DMA_CONFIG+i*9)   = (unsigned int)((unsigned long)(RAM_XE+(i+2)*3));// DMA2SA
           *(RAM_DMA_CONFIG+1+i*9) = (unsigned int)(((unsigned long)(RAM_XE+(i+2)*3))>>16);// DMA2SAH
           *(RAM_DMA_CONFIG+2+i*9) = (unsigned int)(SAPHPGC_addr);            // DMA2DA
           *(RAM_DMA_CONFIG+3+i*9) = 0x0000;                                  // DMA2DAH
           *(RAM_DMA_CONFIG+4+i*9) = 0x0003;                                  // DMA2SZ
           *(RAM_DMA_CONFIG+5+i*9) = 0x0000;                                  // dummy
           *(RAM_DMA_CONFIG+6+i*9) = 0x0000;                                  // dummy
           *(RAM_DMA_CONFIG+7+i*9) = 0x5000|DMASRCINCR|DMADSTINCR|DMAEN;      // DMA3CTL
           *(RAM_DMA_CONFIG+8+i*9) = (unsigned int)(unsigned long)(RAM_DMA_CONFIG+(i+2)*9);  // DMA3SA
        }
        for(i=0;i<2;i++){
           *(RAM_DMA_CONFIG + 11+i*18) = (unsigned int)(SAPHXPGCTL_addr);      // DMA2DA
        }

        // RAM to be transfered to XCTL or PGC
        for(i=0;i<3;i++){
           *(RAM_XE+i*6) = epulse[i];       // SPULSE EPULSE SAPHPGC
           *(RAM_XE+1+i*6) = lper[i];       // SAPHLPGPER
           *(RAM_XE+2+i*6) = hper[i];       // SAPHHPGPER
        }
        for(i=0;i<2;i++){
           *(RAM_XE+3+i*6) =  0x7000|xpulse[i];// DMA_EN XMOD XPULSE SAPHXPGCTL
           *(RAM_XE+4+i*6) = xlper[i];      // SAPHAXLPGPER
           *(RAM_XE+5+i*6) = xhper[i];      // SAPHAXHPGPER
        }

        // Stop
        *(RAM_XE+15) = 0x6000;              // XMOD 2
        *(RAM_XE+15+1) = 0x0000;
        *(RAM_XE+15+2) = 0x0000;

}

