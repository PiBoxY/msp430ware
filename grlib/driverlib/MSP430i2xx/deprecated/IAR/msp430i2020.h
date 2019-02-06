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
/********************************************************************
*
* Standard register and bit definitions for the Texas Instruments
* MSP430I204x microcontroller.
*
* This file supports assembler and C development for
* MSP430i2020 devices.
*
* Texas Instruments, Version 1.1
*
* Rev. 1.0, Setup
* Rev. 1.1, fixed TAxIV_TAIFG definition
*
********************************************************************/

#ifndef __MSP430i2020
#define __MSP430i2020

//#define __MSP430_HEADER_VERSION__ 1134      /* Beta-Build-Tag: #0006 */

#ifdef  __IAR_SYSTEMS_ICC__
#ifndef _SYSTEM_BUILD
#pragma system_include
#endif
#endif

#if (((__TID__ >> 8) & 0x7F) != 0x2b)     /* 0x2b = 43 dec */
#error msp430i2020.h file for use with ICC430/A430 only
#endif


#ifdef __IAR_SYSTEMS_ICC__
#include "in430.h"
#pragma language=extended

#define DEFC(name, address) __no_init volatile unsigned char name @ address;
#define DEFW(name, address) __no_init volatile unsigned short name @ address;

#define DEFCW(name, address) __no_init union \
{ \
  struct \
  { \
    volatile unsigned char  name##_L; \
    volatile unsigned char  name##_H; \
  }; \
  volatile unsigned short   name; \
} @ address;

#define READ_ONLY_DEFCW(name, address) __no_init union \
{ \
  struct \
  { \
    volatile READ_ONLY unsigned char  name##_L; \
    volatile READ_ONLY unsigned char  name##_H; \
  }; \
  volatile READ_ONLY unsigned short   name; \
} @ address;


#if __REGISTER_MODEL__ == __REGISTER_MODEL_REG20__
#define __ACCESS_20BIT_REG__  void __data20 * volatile
#else
#define __ACCESS_20BIT_REG__  volatile unsigned short  /* only short access from C is allowed in small memory model */
#endif

#define DEFA(name, address) __no_init union \
{ \
  struct \
  { \
    volatile unsigned char  name##_L; \
    volatile unsigned char  name##_H; \
  }; \
  struct \
  { \
    volatile unsigned short name##L; \
    volatile unsigned short name##H; \
  }; \
  __ACCESS_20BIT_REG__ name; \
} @ address;

#endif  /* __IAR_SYSTEMS_ICC__  */


#ifdef __IAR_SYSTEMS_ASM__
#define DEFC(name, address) sfrb name = address;
#define DEFW(name, address) sfrw name = address;

#define DEFCW(name, address) sfrbw name, name##_L, name##_H, address;
sfrbw macro name, name_L, name_H, address;
sfrb name_L = address;
sfrb name_H = address+1;
sfrw name   = address;
      endm

#define READ_ONLY_DEFCW(name, address) const_sfrbw name, name##_L, name##_H, address;
const_sfrbw macro name, name_L, name_H, address;
const sfrb name_L = address;
const sfrb name_H = address+1;
const sfrw name   = address;
      endm

#define DEFA(name, address) sfrba name, name##L, name##H, name##_L, name##_H, address;
sfrba macro name, nameL, nameH, name_L, name_H, address;
sfrb name_L = address;
sfrb name_H = address+1;
sfrw nameL  = address;
sfrw nameH  = address+2;
sfrl name   = address;
      endm

#endif /* __IAR_SYSTEMS_ASM__*/

#ifdef __cplusplus
#define READ_ONLY
#else
#define READ_ONLY const
#endif

/************************************************************
* STANDARD BITS
************************************************************/

#define BIT0                (0x0001u)
#define BIT1                (0x0002u)
#define BIT2                (0x0004u)
#define BIT3                (0x0008u)
#define BIT4                (0x0010u)
#define BIT5                (0x0020u)
#define BIT6                (0x0040u)
#define BIT7                (0x0080u)
#define BIT8                (0x0100u)
#define BIT9                (0x0200u)
#define BITA                (0x0400u)
#define BITB                (0x0800u)
#define BITC                (0x1000u)
#define BITD                (0x2000u)
#define BITE                (0x4000u)
#define BITF                (0x8000u)

/************************************************************
* STATUS REGISTER BITS
************************************************************/

#define C                   (0x0001u)
#define Z                   (0x0002u)
#define N                   (0x0004u)
#define V                   (0x0100u)
#define GIE                 (0x0008u)
#define CPUOFF              (0x0010u)
#define OSCOFF              (0x0020u)
#define SCG0                (0x0040u)
#define SCG1                (0x0080u)

/* Low Power Modes coded with Bits 4-7 in SR */

#ifndef __IAR_SYSTEMS_ICC__ /* Begin #defines for assembler */
#define LPM0                (CPUOFF)
#define LPM1                (SCG0+CPUOFF)
#define LPM2                (SCG1+CPUOFF)
#define LPM3                (SCG1+SCG0+CPUOFF)
#define LPM4                (SCG1+SCG0+OSCOFF+CPUOFF)
/* End #defines for assembler */

#else /* Begin #defines for C */
#define LPM0_bits           (CPUOFF)
#define LPM1_bits           (SCG0+CPUOFF)
#define LPM2_bits           (SCG1+CPUOFF)
#define LPM3_bits           (SCG1+SCG0+CPUOFF)
#define LPM4_bits           (SCG1+SCG0+OSCOFF+CPUOFF)

#include "in430.h"

#define LPM0      __bis_SR_register(LPM0_bits)         /* Enter Low Power Mode 0 */
#define LPM0_EXIT __bic_SR_register_on_exit(LPM0_bits) /* Exit Low Power Mode 0 */
#define LPM1      __bis_SR_register(LPM1_bits)         /* Enter Low Power Mode 1 */
#define LPM1_EXIT __bic_SR_register_on_exit(LPM1_bits) /* Exit Low Power Mode 1 */
#define LPM2      __bis_SR_register(LPM2_bits)         /* Enter Low Power Mode 2 */
#define LPM2_EXIT __bic_SR_register_on_exit(LPM2_bits) /* Exit Low Power Mode 2 */
#define LPM3      __bis_SR_register(LPM3_bits)         /* Enter Low Power Mode 3 */
#define LPM3_EXIT __bic_SR_register_on_exit(LPM3_bits) /* Exit Low Power Mode 3 */
#define LPM4      __bis_SR_register(LPM4_bits)         /* Enter Low Power Mode 4 */
#define LPM4_EXIT __bic_SR_register_on_exit(LPM4_bits) /* Exit Low Power Mode 4 */
#endif /* End #defines for C */

/************************************************************
* CPU
************************************************************/
#define __MSP430_HAS_MSP430_CPU__   /* Definition to show that it has MSP430 CPU */
#define __MSP430_HAS_MSP430I_CPU__  /* Definition to show that it has MSP430I CPU */

/************************************************************
* PERIPHERAL FILE MAP
************************************************************/

/************************************************************
* SPECIAL FUNCTION REGISTER ADDRESSES + CONTROL BITS
************************************************************/
#define __MSP430_HAS_SFR__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_SFR__ 0x0000
#define SFR_BASE __MSP430_BASEADDRESS_SFR__

#define IE1_                   (0x0000u)  /* Interrupt Enable 1 */
DEFC(   IE1                  , IE1_)
#define U0IE                IE1       /* UART0 Interrupt Enable Register */
#define WDTIE               (0x01)    /* Watchdog Interrupt Enable */
#define OFIE                (0x02)    /* Osc. Fault  Interrupt Enable */
#define NMIIE               (0x10)    /* NMI Interrupt Enable */
#define ACCVIE              (0x20)    /* Flash Access Violation Interrupt Enable */

#define IFG1_                  (0x0002u)  /* Interrupt Flag 1 */
DEFC(   IFG1                 , IFG1_)
#define WDTIFG              (0x01)    /* Watchdog Interrupt Flag */
#define OFIFG               (0x02)    /* Osc. Fault Interrupt Flag */
#define BORIFG              (0x04)    /* Brown Out Reset Interrupt Flag */
#define RSTIFG              (0x08)    /* Reset Interrupt Flag */
#define NMIIFG              (0x10)    /* NMI Interrupt Flag */

/* 5xx Compatibility defines */
#define SFRIE1_L            IE1       /* Interrupt Enable 1 */
#define SFRIFG1_L           IFG1      /* Interrupt Flag 1 */

#define SYSJTAGDIS_            (0x01FEu)  /* JTAG Disable Register */
DEFW(   SYSJTAGDIS           , SYSJTAGDIS_)
#define JTAGDISKEY          (0xA5A5u)  /* JTAG Lock Key */

/************************************************************
* CLOCK SYSTEM
************************************************************/
#define __MSP430_HAS_CS__              /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_CS__ 0x0050
#define CS_BASE __MSP430_BASEADDRESS_CS__

#define  CSCTL0_                (0x0050u)  /* CS Control Register 0 */
DEFC(    CSCTL0               , CSCTL0_)
#define  CSCTL1_                (0x0051u)  /* CS Control Register 1 */
DEFC(    CSCTL1               , CSCTL1_)
#define  CSIRFCAL_              (0x0052u)  /* CS Internal Resistor Frequency Calibration */
DEFC(    CSIRFCAL             , CSIRFCAL_)
#define  CSIRTCAL_              (0x0053u)  /* CS Internal Resistor Temperature Calibration */
DEFC(    CSIRTCAL             , CSIRTCAL_)
#define  CSERFCAL_              (0x0054u)  /* CS External Resistor Frequency Calibration */
DEFC(    CSERFCAL             , CSERFCAL_)
#define  CSERTCAL_              (0x0055u)  /* CS External Resistor Temperature Calibration */
DEFC(    CSERTCAL             , CSERTCAL_)

/* CSCTL0 Control Bits */
#define DCOR                 (0x0001u)    /* DCO resistor select */
#define DCOBYP               (0x0002u)    /* DCO bypass mode */
#define DCOF                 (0x0080u)    /* DCO fault flag */

/* CSCTL1 Control Bits */
#define DIVM0               (0x0001u)   /* MCLK Divider Bit: 0 */
#define DIVM1               (0x0002u)   /* MCLK Divider Bit: 1 */
#define DIVM2               (0x0004u)   /* MCLK Divider Bit: 2 */
#define DIVS0               (0x0010u)   /* SMCLK Divider Bit: 0 */
#define DIVS1               (0x0020u)   /* SMCLK Divider Bit: 1 */
#define DIVS2               (0x0040u)   /* SMCLK Divider Bit: 2 */

#define DIVM_0              (0x0000u)    /* MCLK Source Divider 0 */
#define DIVM_1              (0x0001u)    /* MCLK Source Divider 1 */
#define DIVM_2              (0x0002u)    /* MCLK Source Divider 2 */
#define DIVM_3              (0x0003u)    /* MCLK Source Divider 3 */
#define DIVM_4              (0x0004u)    /* MCLK Source Divider 5 */
#define DIVM_5              (0x0005u)    /* MCLK Source Divider 6 */
#define DIVM_6              (0x0006u)    /* MCLK Source Divider 7 */
#define DIVM_7              (0x0007u)    /* MCLK Source Divider 8 */
#define DIVM__1             (0x0000u)    /* MCLK Source Divider f(MCLK)/1 */
#define DIVM__2             (0x0001u)    /* MCLK Source Divider f(MCLK)/2 */
#define DIVM__4             (0x0002u)    /* MCLK Source Divider f(MCLK)/4 */
#define DIVM__8             (0x0003u)    /* MCLK Source Divider f(MCLK)/8 */
#define DIVM__16            (0x0004u)    /* MCLK Source Divider f(MCLK)/16 */

#define DIVS_0              (0x0000u)    /* SMCLK Source Divider 0 */
#define DIVS_1              (0x0010u)    /* SMCLK Source Divider 1 */
#define DIVS_2              (0x0020u)    /* SMCLK Source Divider 2 */
#define DIVS_3              (0x0030u)    /* SMCLK Source Divider 3 */
#define DIVS_4              (0x0040u)    /* SMCLK Source Divider 5 */
#define DIVS_5              (0x0050u)    /* SMCLK Source Divider 6 */
#define DIVS_6              (0x0060u)    /* SMCLK Source Divider 7 */
#define DIVS_7              (0x0070u)    /* SMCLK Source Divider 8 */
#define DIVS__1             (0x0000u)    /* SMCLK Source Divider f(SMCLK)/1 */
#define DIVS__2             (0x0010u)    /* SMCLK Source Divider f(SMCLK)/2 */
#define DIVS__4             (0x0020u)    /* SMCLK Source Divider f(SMCLK)/4 */
#define DIVS__8             (0x0030u)    /* SMCLK Source Divider f(SMCLK)/8 */
#define DIVS__16            (0x0040u)    /* SMCLK Source Divider f(SMCLK)/16 */

/* CSIRFCAL Control Bits */
#define IRFCAL0              (0x0001u)    /* DCO internal resistor frequency calibration value Bit 0 */
#define IRFCAL1              (0x0002u)    /* DCO internal resistor frequency calibration value Bit 1 */
#define IRFCAL2              (0x0004u)    /* DCO internal resistor frequency calibration value Bit 2 */
#define IRFCAL3              (0x0008u)    /* DCO internal resistor frequency calibration value Bit 3 */
#define IRFCAL4              (0x0010u)    /* DCO internal resistor frequency calibration value Bit 4 */
#define IRFCAL5              (0x0020u)    /* DCO internal resistor frequency calibration value Bit 5 */
#define IRFCAL6              (0x0040u)    /* DCO internal resistor frequency calibration value Bit 6 */
#define IRFCAL7              (0x0080u)    /* DCO internal resistor frequency calibration value Bit 7 */

/* CSIRTCAL Control Bits */
#define IRTCAL0              (0x0001u)    /* DCO internal resistor temperature calibration value Bit 0 */
#define IRTCAL1              (0x0002u)    /* DCO internal resistor temperature calibration value Bit 1 */
#define IRTCAL2              (0x0004u)    /* DCO internal resistor temperature calibration value Bit 2 */
#define IRTCAL3              (0x0008u)    /* DCO internal resistor temperature calibration value Bit 3 */
#define IRTCAL4              (0x0010u)    /* DCO internal resistor temperature calibration value Bit 4 */
#define IRTCAL5              (0x0020u)    /* DCO internal resistor temperature calibration value Bit 5 */
#define IRTCAL6              (0x0040u)    /* DCO internal resistor temperature calibration value Bit 6 */
#define IRTCAL7              (0x0080u)    /* DCO internal resistor temperature calibration value Bit 7 */

/* CSERFCAL Control Bits */
#define ERFCAL0              (0x0001u)    /* DCO external resistor frequency calibration value Bit 0 */
#define ERFCAL1              (0x0002u)    /* DCO external resistor frequency calibration value Bit 1 */
#define ERFCAL2              (0x0004u)    /* DCO external resistor frequency calibration value Bit 2 */
#define ERFCAL3              (0x0008u)    /* DCO external resistor frequency calibration value Bit 3 */
#define ERFCAL4              (0x0010u)    /* DCO external resistor frequency calibration value Bit 4 */
#define ERFCAL5              (0x0020u)    /* DCO external resistor frequency calibration value Bit 5 */
#define ERFCAL6              (0x0040u)    /* DCO external resistor frequency calibration value Bit 6 */
#define ERFCAL7              (0x0080u)    /* DCO external resistor frequency calibration value Bit 7 */

/* CSERTCAL Control Bits */
#define ERTCAL0              (0x0001u)    /* DCO external resistor temperature calibration value Bit 0 */
#define ERTCAL1              (0x0002u)    /* DCO external resistor temperature calibration value Bit 1 */
#define ERTCAL2              (0x0004u)    /* DCO external resistor temperature calibration value Bit 2 */
#define ERTCAL3              (0x0008u)    /* DCO external resistor temperature calibration value Bit 3 */
#define ERTCAL4              (0x0010u)    /* DCO external resistor temperature calibration value Bit 4 */
#define ERTCAL5              (0x0020u)    /* DCO external resistor temperature calibration value Bit 5 */
#define ERTCAL6              (0x0040u)    /* DCO external resistor temperature calibration value Bit 6 */
#define ERTCAL7              (0x0080u)    /* DCO external resistor temperature calibration value Bit 7 */

/*************************************************************
* Flash Memory
*************************************************************/
#define __MSP430_HAS_FLASH__          /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_FLASH__ 0x0128
#define FLASH_BASE __MSP430_BASEADDRESS_FLASH__

#define FCTL1_                 (0x0128u)  /* FLASH Control 1 */
DEFW(   FCTL1                , FCTL1_)
#define FCTL2_                 (0x012Au)  /* FLASH Control 2 */
DEFW(   FCTL2                , FCTL2_)
#define FCTL3_                 (0x012Cu)  /* FLASH Control 3 */
DEFW(   FCTL3                , FCTL3_)

#define FRKEY               (0x9600u)  /* Flash key returned by read */
#define FWKEY               (0xA500u)  /* Flash key for write */
#define FXKEY               (0x3300u)  /* for use with XOR instruction */

#define ERASE               (0x0002u)  /* Enable bit for Flash segment erase */
#define MERAS               (0x0004u)  /* Enable bit for Flash mass erase */
#define WRT                 (0x0040u)  /* Enable bit for Flash write */
#define BLKWRT              (0x0080u)  /* Enable bit for Flash segment write */
#define SEGWRT              (0x0080u)  /* old definition */ /* Enable bit for Flash segment write */

#define FN0                 (0x0001u)  /* Divide Flash clock by 1 to 64 using FN0 to FN5 according to: */
#define FN1                 (0x0002u)  /*  32*FN5 + 16*FN4 + 8*FN3 + 4*FN2 + 2*FN1 + FN0 + 1 */
#ifndef FN2
#define FN2                 (0x0004u)
#endif
#ifndef FN3
#define FN3                 (0x0008u)
#endif
#ifndef FN4
#define FN4                 (0x0010u)
#endif
#define FN5                 (0x0020u)
#define FSSEL0              (0x0040u)  /* Flash clock select 0 */        /* to distinguish from USART SSELx */
#define FSSEL1              (0x0080u)  /* Flash clock select 1 */

#define FSSEL_0             (0x0000u)  /* Flash clock select: 0 - ACLK */
#define FSSEL_1             (0x0040u)  /* Flash clock select: 1 - MCLK */
#define FSSEL_2             (0x0080u)  /* Flash clock select: 2 - SMCLK */
#define FSSEL_3             (0x00C0u)  /* Flash clock select: 3 - SMCLK */

#define BUSY                (0x0001u)  /* Flash busy: 1 */
#define KEYV                (0x0002u)  /* Flash Key violation flag */
#define ACCVIFG             (0x0004u)  /* Flash Access violation flag */
#define WAIT                (0x0008u)  /* Wait flag for segment write */
#define LOCK                (0x0010u)  /* Lock bit: 1 - Flash is locked (read only) */
#define EMEX                (0x0020u)  /* Flash Emergency Exit */
#define LOCKSEG             (0x0040u)  /* Flash Info Lock bit: read = 1 - Segment is locked (read only) */

/************************************************************
* HARDWARE MULTIPLIER
************************************************************/
#define __MSP430_HAS_MPY__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_MPY__ 0x0130
#define MPY_BASE __MSP430_BASEADDRESS_MPY__

#define MPY_                   (0x0130u)  /* Multiply Unsigned/Operand 1 */
DEFW(   MPY                  , MPY_)
#define MPYS_                  (0x0132u)  /* Multiply Signed/Operand 1 */
DEFW(   MPYS                 , MPYS_)
#define MAC_                   (0x0134u)  /* Multiply Unsigned and Accumulate/Operand 1 */
DEFW(   MAC                  , MAC_)
#define MACS_                  (0x0136u)  /* Multiply Signed and Accumulate/Operand 1 */
DEFW(   MACS                 , MACS_)
#define OP2_                   (0x0138u)  /* Operand 2 */
DEFW(   OP2                  , OP2_)
#define RESLO_                 (0x013Au)  /* Result Low Word */
DEFW(   RESLO                , RESLO_)
#define RESHI_                 (0x013Cu)  /* Result High Word */
DEFW(   RESHI                , RESHI_)
#define SUMEXT_                (0x013Eu)  /* Sum Extend */
READ_ONLY DEFW( SUMEXT            , SUMEXT_)

/************************************************************
* PMM - Power Management System
************************************************************/
#define __MSP430_HAS_PMM__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PMM__ 0x0060
#define PMM_BASE __MSP430_BASEADDRESS_PMM__


#define  LPM45CTL_             (0x0060u)  /* PMM Power Mode 4.5 Control Register */
DEFC(    LPM45CTL            , LPM45CTL_)
#define  VMONCTL_              (0x0061u)  /* PMM Voltage Monitor Control Register */
DEFC(    VMONCTL             , VMONCTL_)
#define  REFCAL0_              (0x0062u)  /* PMM Reference Calibration Register 0 */
DEFC(    REFCAL0             , REFCAL0_)
#define  REFCAL1_              (0x0063u)  /* PMM Reference Calibration Register 1 */
DEFC(    REFCAL1             , REFCAL1_)

#define VCMONCTL            VMONCTL   /* Legacy Define */

/* LPM45CTL Power Mode 5 Control Bits */
#define LOCKLPM45           (0x0001u)  /* Lock I/O pin configuration upon entry/exit to/from LPM4.5 */
#define LPM45IFG            (0x0002u)  /* LPM4.5 interrupt flag */
#define PMMREGOFF           (0x0010u)  /* PMM Turn Regulator off */

/* Voltage Monitor Control Bits */
#define VMONLVL0            (0x0001u)  /* Voltage monitor level selection Bit 0 */
#define VMONLVL1            (0x0002u)  /* Voltage monitor level selection Bit 1 */
#define VMONLVL2            (0x0004u)  /* Voltage monitor level selection Bit 2 */
#define VMONIE              (0x0010u)  /* Voltage monitor interrupt enable */
#define VMONIFG             (0x0020u)  /* Voltage monitor interrupt flag */

#define VMONLVL_0           (0x0000u)  /* Voltage monitor level selection: 0 */
#define VMONLVL_1           (0x0001u)  /* Voltage monitor level selection: 1 */
#define VMONLVL_2           (0x0002u)  /* Voltage monitor level selection: 2 */
#define VMONLVL_3           (0x0003u)  /* Voltage monitor level selection: 3 */
#define VMONLVL_4           (0x0004u)  /* Voltage monitor level selection: 4 */
#define VMONLVL_5           (0x0005u)  /* Voltage monitor level selection: 5 */
#define VMONLVL_6           (0x0006u)  /* Voltage monitor level selection: 6 */
#define VMONLVL_7           (0x0007u)  /* Voltage monitor level selection: 7 */

/* Reference Calibration 0 Bits */
#define BGFINE0             (0x0001u)  /* Bandgap voltage fine calibration Bit 0 */
#define BGFINE1             (0x0002u)  /* Bandgap voltage fine calibration Bit 1 */
#define BGFINE2             (0x0004u)  /* Bandgap voltage fine calibration Bit 2 */
#define BGFINE3             (0x0008u)  /* Bandgap voltage fine calibration Bit 3 */
#define BGCOARSE0           (0x0010u)  /* Bandgap voltage coarse calibration Bit 0 */
#define BGCOARSE1           (0x0020u)  /* Bandgap voltage coarse calibration Bit 1 */

/* Reference Calibration 1 Bits */
#define BGCURVE0            (0x0001u)  /* Curvature compensation Bit 0 */
#define BGCURVE1            (0x0002u)  /* Curvature compensation Bit 1 */
#define BGCURVE2            (0x0004u)  /* Curvature compensation Bit 2 */
#define BGCURVE3            (0x0008u)  /* Curvature compensation Bit 3 */
#define BIASCURRENT0        (0x0010u)  /* Bandgap bias current calibration Bit 0 */
#define BIASCURRENT1        (0x0020u)  /* Bandgap bias current calibration Bit 1 */
#define BIASCURRENT2        (0x0040u)  /* Bandgap bias current calibration Bit 2 */
#define BIASCURRENT3        (0x0080u)  /* Bandgap bias current calibration Bit 3 */

/************************************************************
* DIGITAL I/O Port1/2
************************************************************/
#define __MSP430_HAS_PORT1__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT1__ 0x0010
#define P1_BASE __MSP430_BASEADDRESS_PORT1__
#define __MSP430_HAS_PORT2__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT2__ 0x0010
#define P2_BASE __MSP430_BASEADDRESS_PORT2__
#define __MSP430_HAS_PORTA__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORTA__ 0x0010
#define PA_BASE __MSP430_BASEADDRESS_PORTA__
#define __MSP430_HAS_P1SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_P2SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_PASEL0__         /* Define for DriverLib */
#define __MSP430_HAS_P1SEL1__         /* Define for DriverLib */
#define __MSP430_HAS_P2SEL1__         /* Define for DriverLib */
#define __MSP430_HAS_PASEL1__         /* Define for DriverLib */

#define PAIN_                  (0x0010u)  /* Port A Input */
READ_ONLY_DEFCW( PAIN              , PAIN_)
#define  PAOUT_                 (0x0012u)  /* Port A Output */
DEFCW(   PAOUT                , PAOUT_)
#define  PADIR_                 (0x0014u)  /* Port A Direction */
DEFCW(   PADIR                , PADIR_)
#define  PASEL0_                (0x001Au)  /* Port A Selection 0 */
DEFCW(   PASEL0               , PASEL0_)
#define  PASEL1_                (0x001Cu)  /* Port A Selection 1 */
DEFCW(   PASEL1               , PASEL1_)
#define  PAIES_                 (0x0028u)  /* Port A Interrupt Edge Select */
DEFCW(   PAIES                , PAIES_)
#define  PAIE_                  (0x002Au)  /* Port A Interrupt Enable */
DEFCW(   PAIE                 , PAIE_)
#define  PAIFG_                 (0x002Cu)  /* Port A Interrupt Flag */
DEFCW(   PAIFG                , PAIFG_)


#define P1IV_                  (0x001Eu)  /* Port 1 Interrupt Vector Word */
DEFW(   P1IV                 , P1IV_)
#define P2IV_                  (0x002Eu)  /* Port 2 Interrupt Vector Word */
DEFW(   P2IV                 , P2IV_)
#define P1IN                (PAIN_L)  /* Port 1 Input */
#define P1OUT               (PAOUT_L) /* Port 1 Output */
#define P1DIR               (PADIR_L) /* Port 1 Direction */
#define P1SEL0              (PASEL0_L) /* Port 1 Selection 0 */
#define P1SEL1              (PASEL1_L) /* Port 1 Selection 1 */
#define P1IES               (PAIES_L) /* Port 1 Interrupt Edge Select */
#define P1IE                (PAIE_L)  /* Port 1 Interrupt Enable */
#define P1IFG               (PAIFG_L) /* Port 1 Interrupt Flag */

//Definitions for P1IV
#define P1IV_NONE            (0x0000u)    /* No Interrupt pending */
#define P1IV_P1IFG0          (0x0002u)    /* P1IV P1IFG.0 */
#define P1IV_P1IFG1          (0x0004u)    /* P1IV P1IFG.1 */
#define P1IV_P1IFG2          (0x0006u)    /* P1IV P1IFG.2 */
#define P1IV_P1IFG3          (0x0008u)    /* P1IV P1IFG.3 */
#define P1IV_P1IFG4          (0x000Au)    /* P1IV P1IFG.4 */
#define P1IV_P1IFG5          (0x000Cu)    /* P1IV P1IFG.5 */
#define P1IV_P1IFG6          (0x000Eu)    /* P1IV P1IFG.6 */
#define P1IV_P1IFG7          (0x0010u)    /* P1IV P1IFG.7 */

#define P2IN                (PAIN_H)  /* Port 2 Input */
#define P2OUT               (PAOUT_H) /* Port 2 Output */
#define P2DIR               (PADIR_H) /* Port 2 Direction */
#define P2SEL0              (PASEL0_H) /* Port 2 Selection 0 */
#define P2SEL1              (PASEL1_H) /* Port 2 Selection 1 */
#define P2IES               (PAIES_H) /* Port 2 Interrupt Edge Select */
#define P2IE                (PAIE_H)  /* Port 2 Interrupt Enable */
#define P2IFG               (PAIFG_H) /* Port 2 Interrupt Flag */

//Definitions for P2IV
#define P2IV_NONE            (0x0000u)    /* No Interrupt pending */
#define P2IV_P2IFG0          (0x0002u)    /* P2IV P2IFG.0 */
#define P2IV_P2IFG1          (0x0004u)    /* P2IV P2IFG.1 */
#define P2IV_P2IFG2          (0x0006u)    /* P2IV P2IFG.2 */
#define P2IV_P2IFG3          (0x0008u)    /* P2IV P2IFG.3 */
#define P2IV_P2IFG4          (0x000Au)    /* P2IV P2IFG.4 */
#define P2IV_P2IFG5          (0x000Cu)    /* P2IV P2IFG.5 */
#define P2IV_P2IFG6          (0x000Eu)    /* P2IV P2IFG.6 */
#define P2IV_P2IFG7          (0x0010u)    /* P2IV P2IFG.7 */


/************************************************************
* SD24_2 - Sigma Delta 24 Bit
************************************************************/
#define __MSP430_HAS_SD24__           /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_SD24__ 0x00B0
#define SD24_BASE __MSP430_BASEADDRESS_SD24__
#define __MSP430_HAS_SD24_2__        /* Definition to show that Module is available */

#define SD24INCTL0_            (0x00B0u)  /* SD24 Input Control Register Channel 0 */
DEFC(   SD24INCTL0           , SD24INCTL0_)
#define SD24INCTL1_            (0x00B1u)  /* SD24 Input Control Register Channel 1 */
DEFC(   SD24INCTL1           , SD24INCTL1_)
#define SD24PRE0_              (0x00B8u)  /* SD24 Preload Register Channel 0 */
DEFC(   SD24PRE0             , SD24PRE0_)
#define SD24PRE1_              (0x00B9u)  /* SD24 Preload Register Channel 1 */
DEFC(   SD24PRE1             , SD24PRE1_)
#define SD24TRIM_              (0x00BFu)  /* SD24 Trim Register 1 */
DEFC(   SD24TRIM             , SD24TRIM_)

#define SD24CTL_               (0x0100u)  /* Sigma Delta ADC 24 Control Register */
DEFW(   SD24CTL              , SD24CTL_)
#define SD24CCTL0_             (0x0102u)  /* SD24 Channel 0 Control Register */
DEFW(   SD24CCTL0            , SD24CCTL0_)
#define SD24CCTL1_             (0x0104u)  /* SD24 Channel 1 Control Register */
DEFW(   SD24CCTL1            , SD24CCTL1_)
#define SD24MEM0_              (0x0110u)  /* SD24 Channel 0 Conversion Memory */
DEFW(   SD24MEM0             , SD24MEM0_)
#define SD24MEM1_              (0x0112u)  /* SD24 Channel 1 Conversion Memory */
DEFW(   SD24MEM1             , SD24MEM1_)
#define SD24IV_                (0x01F0u)  /* SD24 Interrupt Vector Register */
DEFW(   SD24IV               , SD24IV_)

/* SD24INCTLx */
#define SD24INCH0           (0x0001u)  /* SD24 Input Channel select 0 */
#define SD24INCH1           (0x0002u)  /* SD24 Input Channel select 1 */
#define SD24INCH2           (0x0004u)  /* SD24 Input Channel select 2 */
#define SD24GAIN0           (0x0008u)  /* SD24 Input Pre-Amplifier Gain Select 0 */
#define SD24GAIN1           (0x0010u)  /* SD24 Input Pre-Amplifier Gain Select 1 */
#define SD24GAIN2           (0x0020u)  /* SD24 Input Pre-Amplifier Gain Select 2 */
#define SD24INTDLY          (0x0040u)  /* SD24 Interrupt Delay after 1.Conversion */

#define SD24GAIN_1          (0x0000u)  /* SD24 Input Pre-Amplifier Gain Select *1  */
#define SD24GAIN_2          (0x0008u)  /* SD24 Input Pre-Amplifier Gain Select *2  */
#define SD24GAIN_4          (0x0010u)  /* SD24 Input Pre-Amplifier Gain Select *4  */
#define SD24GAIN_8          (0x0018u)  /* SD24 Input Pre-Amplifier Gain Select *8  */
#define SD24GAIN_16         (0x0020u)  /* SD24 Input Pre-Amplifier Gain Select *16 */

#define SD24INCH_0          (0x0000u)  /* SD24 Input Channel select input */
#define SD24INCH_1          (0x0001u)  /* SD24 Input Channel select input */
#define SD24INCH_2          (0x0002u)  /* SD24 Input Channel select input */
#define SD24INCH_3          (0x0003u)  /* SD24 Input Channel select input */
#define SD24INCH_4          (0x0004u)  /* SD24 Input Channel select input */
#define SD24INCH_5          (0x0005u)  /* SD24 Input Channel select Vcc divider */
#define SD24INCH_6          (0x0006u)  /* SD24 Input Channel select Temp */
#define SD24INCH_7          (0x0007u)  /* SD24 Input Channel select Offset */

/* SD24CTL */
#define SD24OVIE            (0x0002u)  /* SD24 Overflow Interupt Enable */
#define SD24REFS            (0x0004u)  /* SD24 Reference Select */

/* SD24CCTLx */
#define SD24GRP             (0x0001u)  /* SD24 Grouping of Channels: 0:Off/1:On */
#define SD24SC              (0x0002u)  /* SD24 Start Conversion */
#define SD24IFG             (0x0004u)  /* SD24 Channel x Interrupt Flag */
#define SD24IE              (0x0008u)  /* SD24 Channel x Interrupt Enable */
#define SD24DF              (0x0010u)  /* SD24 Channel x Data Format: 0:Unipolar/1:Bipolar */
#define SD24OVIFG           (0x0020u)  /* SD24 Channel x Overflow Interrupt Flag */
#define SD24LSBACC          (0x0040u)  /* SD24 Channel x Access LSB of ADC */
#define SD24LSBTOG          (0x0080u)  /* SD24 Channel x Toggle LSB Output of ADC */
#define SD24OSR0            (0x0100u)  /* SD24 Channel x OverSampling Ratio 0 */
#define SD24OSR1            (0x0200u)  /* SD24 Channel x OverSampling Ratio 1 */
#define SD24SNGL            (0x0400u)  /* SD24 Channel x Single Conversion On/Off */

#define SD24OSR_256         (0x0000u)  /* SD24 Channel x OverSampling Ratio 256 */
#define SD24OSR_128         (0x0100u)  /* SD24 Channel x OverSampling Ratio 128 */
#define SD24OSR_64          (0x0200u)  /* SD24 Channel x OverSampling Ratio  64 */
#define SD24OSR_32          (0x0300u)  /* SD24 Channel x OverSampling Ratio  32 */

/* SD24TRIM */
#define SD24IBTRIM0         (0x0001u)  /* SD24 Trimming of bias current Bit: 0 */
#define SD24IBTRIM1         (0x0002u)  /* SD24 Trimming of bias current Bit: 1 */
#define SD24IBTRIM2         (0x0004u)  /* SD24 Trimming of bias current Bit: 2 */
#define SD24REFDLYTRIM0     (0x0008u)  /* SD24 Trimming of reference voltage buffer start up delay Bit: 0 */
#define SD24REFDLYTRIM1     (0x0010u)  /* SD24 Trimming of reference voltage buffer start up delay Bit: 1 */

/* SD24IV Definitions */
#define SD24IV_NONE         (0x0000u)    /* No Interrupt pending */
#define SD24IV_SD24OVIFG    (0x0002u)    /* SD24OVIFG */
#define SD24IV_SD24MEM0     (0x0004u)    /* SD24MEM0 SD24IFG */
#define SD24IV_SD24MEM1     (0x0006u)    /* SD24MEM1 SD24IFG */

/************************************************************
* Timer0_A3
************************************************************/
#define __MSP430_HAS_T0A3__           /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_T0A3__ 0x0160
#define TIMER_A0_BASE __MSP430_BASEADDRESS_T0A3__

#define TA0IV_                 (0x012Eu)  /* Timer0_A3 Interrupt Vector Word */
READ_ONLY DEFW( TA0IV             , TA0IV_)
#define TA0CTL_                (0x0160u)  /* Timer0_A3 Control */
DEFW(   TA0CTL               , TA0CTL_)
#define TA0CCTL0_              (0x0162u)  /* Timer0_A3 Capture/Compare Control 0 */
DEFW(   TA0CCTL0             , TA0CCTL0_)
#define TA0CCTL1_              (0x0164u)  /* Timer0_A3 Capture/Compare Control 1 */
DEFW(   TA0CCTL1             , TA0CCTL1_)
#define TA0CCTL2_              (0x0166u)  /* Timer0_A3 Capture/Compare Control 2 */
DEFW(   TA0CCTL2             , TA0CCTL2_)
#define TA0R_                  (0x0170u)  /* Timer0_A3 Counter Register */
DEFW(   TA0R                 , TA0R_)
#define TA0CCR0_               (0x0172u)  /* Timer0_A3 Capture/Compare 0 */
DEFW(   TA0CCR0              , TA0CCR0_)
#define TA0CCR1_               (0x0174u)  /* Timer0_A3 Capture/Compare 1 */
DEFW(   TA0CCR1              , TA0CCR1_)
#define TA0CCR2_               (0x0176u)  /* Timer0_A3 Capture/Compare 2 */
DEFW(   TA0CCR2              , TA0CCR2_)

/* Alternate register names */
#define TAIV                TA0IV     /* Timer A Interrupt Vector Word */
#define TACTL               TA0CTL    /* Timer A Control */
#define TACCTL0             TA0CCTL0  /* Timer A Capture/Compare Control 0 */
#define TACCTL1             TA0CCTL1  /* Timer A Capture/Compare Control 1 */
#define TACCTL2             TA0CCTL2  /* Timer A Capture/Compare Control 2 */
#define TAR                 TA0R      /* Timer A Counter Register */
#define TACCR0              TA0CCR0   /* Timer A Capture/Compare 0 */
#define TACCR1              TA0CCR1   /* Timer A Capture/Compare 1 */
#define TACCR2              TA0CCR2   /* Timer A Capture/Compare 2 */
#define TAIV_               TA0IV_    /* Timer A Interrupt Vector Word */
#define TACTL_              TA0CTL_   /* Timer A Control */
#define TACCTL0_            TA0CCTL0_ /* Timer A Capture/Compare Control 0 */
#define TACCTL1_            TA0CCTL1_ /* Timer A Capture/Compare Control 1 */
#define TACCTL2_            TA0CCTL2_ /* Timer A Capture/Compare Control 2 */
#define TAR_                TA0R_     /* Timer A Counter Register */
#define TACCR0_             TA0CCR0_  /* Timer A Capture/Compare 0 */
#define TACCR1_             TA0CCR1_  /* Timer A Capture/Compare 1 */
#define TACCR2_             TA0CCR2_  /* Timer A Capture/Compare 2 */

/* Alternate register names 2 */
#define CCTL0               TACCTL0   /* Timer A Capture/Compare Control 0 */
#define CCTL1               TACCTL1   /* Timer A Capture/Compare Control 1 */
#define CCTL2               TACCTL2   /* Timer A Capture/Compare Control 2 */
#define CCR0                TACCR0    /* Timer A Capture/Compare 0 */
#define CCR1                TACCR1    /* Timer A Capture/Compare 1 */
#define CCR2                TACCR2    /* Timer A Capture/Compare 2 */
#define CCTL0_              TACCTL0_  /* Timer A Capture/Compare Control 0 */
#define CCTL1_              TACCTL1_  /* Timer A Capture/Compare Control 1 */
#define CCTL2_              TACCTL2_  /* Timer A Capture/Compare Control 2 */
#define CCR0_               TACCR0_   /* Timer A Capture/Compare 0 */
#define CCR1_               TACCR1_   /* Timer A Capture/Compare 1 */
#define CCR2_               TACCR2_   /* Timer A Capture/Compare 2 */

#define TASSEL1             (0x0200u)  /* Timer A clock source select 1 */
#define TASSEL0             (0x0100u)  /* Timer A clock source select 0 */
#define ID1                 (0x0080u)  /* Timer A clock input divider 1 */
#define ID0                 (0x0040u)  /* Timer A clock input divider 0 */
#define MC1                 (0x0020u)  /* Timer A mode control 1 */
#define MC0                 (0x0010u)  /* Timer A mode control 0 */
#define TACLR               (0x0004u)  /* Timer A counter clear */
#define TAIE                (0x0002u)  /* Timer A counter interrupt enable */
#define TAIFG               (0x0001u)  /* Timer A counter interrupt flag */

#define MC_0                (0*0x10u)  /* Timer A mode control: 0 - Stop */
#define MC_1                (1*0x10u)  /* Timer A mode control: 1 - Up to CCR0 */
#define MC_2                (2*0x10u)  /* Timer A mode control: 2 - Continuous up */
#define MC_3                (3*0x10u)  /* Timer A mode control: 3 - Up/Down */
#define ID_0                (0*0x40u)  /* Timer A input divider: 0 - /1 */
#define ID_1                (1*0x40u)  /* Timer A input divider: 1 - /2 */
#define ID_2                (2*0x40u)  /* Timer A input divider: 2 - /4 */
#define ID_3                (3*0x40u)  /* Timer A input divider: 3 - /8 */
#define TASSEL_0            (0*0x100u) /* Timer A clock source select: 0 - TACLK */
#define TASSEL_1            (1*0x100u) /* Timer A clock source select: 1 - ACLK  */
#define TASSEL_2            (2*0x100u) /* Timer A clock source select: 2 - SMCLK */
#define TASSEL_3            (3*0x100u) /* Timer A clock source select: 3 - INCLK */

#define CM1                 (0x8000u)  /* Capture mode 1 */
#define CM0                 (0x4000u)  /* Capture mode 0 */
#define CCIS1               (0x2000u)  /* Capture input select 1 */
#define CCIS0               (0x1000u)  /* Capture input select 0 */
#define SCS                 (0x0800u)  /* Capture sychronize */
#define SCCI                (0x0400u)  /* Latched capture signal (read) */
#define CAP                 (0x0100u)  /* Capture mode: 1 /Compare mode : 0 */
#define OUTMOD2             (0x0080u)  /* Output mode 2 */
#define OUTMOD1             (0x0040u)  /* Output mode 1 */
#define OUTMOD0             (0x0020u)  /* Output mode 0 */
#define CCIE                (0x0010u)  /* Capture/compare interrupt enable */
#define CCI                 (0x0008u)  /* Capture input signal (read) */
#define OUT                 (0x0004u)  /* PWM Output signal if output mode 0 */
#define COV                 (0x0002u)  /* Capture/compare overflow flag */
#define CCIFG               (0x0001u)  /* Capture/compare interrupt flag */

#define OUTMOD_0            (0*0x20u)  /* PWM output mode: 0 - output only */
#define OUTMOD_1            (1*0x20u)  /* PWM output mode: 1 - set */
#define OUTMOD_2            (2*0x20u)  /* PWM output mode: 2 - PWM toggle/reset */
#define OUTMOD_3            (3*0x20u)  /* PWM output mode: 3 - PWM set/reset */
#define OUTMOD_4            (4*0x20u)  /* PWM output mode: 4 - toggle */
#define OUTMOD_5            (5*0x20u)  /* PWM output mode: 5 - Reset */
#define OUTMOD_6            (6*0x20u)  /* PWM output mode: 6 - PWM toggle/set */
#define OUTMOD_7            (7*0x20u)  /* PWM output mode: 7 - PWM reset/set */
#define CCIS_0              (0*0x1000u) /* Capture input select: 0 - CCIxA */
#define CCIS_1              (1*0x1000u) /* Capture input select: 1 - CCIxB */
#define CCIS_2              (2*0x1000u) /* Capture input select: 2 - GND */
#define CCIS_3              (3*0x1000u) /* Capture input select: 3 - Vcc */
#define CM_0                (0*0x4000u) /* Capture mode: 0 - disabled */
#define CM_1                (1*0x4000u) /* Capture mode: 1 - pos. edge */
#define CM_2                (2*0x4000u) /* Capture mode: 1 - neg. edge */
#define CM_3                (3*0x4000u) /* Capture mode: 1 - both edges */

/* T0_A3IV Definitions */
#define TA0IV_NONE          (0x0000u)    /* No Interrupt pending */
#define TA0IV_TACCR1        (0x0002u)    /* TA0CCR1_CCIFG */
#define TA0IV_TACCR2        (0x0004u)    /* TA0CCR2_CCIFG */
#define TA0IV_6             (0x0006u)    /* Reserved */
#define TA0IV_8             (0x0008u)    /* Reserved */
#define TA0IV_TAIFG         (0x000Eu)    /* TA0IFG */

/************************************************************
* Timer1_A3
************************************************************/
#define __MSP430_HAS_T1A3__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_T1A3__ 0x0180
#define TIMER_A1_BASE __MSP430_BASEADDRESS_T1A3__

#define TA1IV_                 (0x011Eu)  /* Timer1_A3 Interrupt Vector Word */
READ_ONLY DEFW( TA1IV             , TA1IV_)
#define TA1CTL_                (0x0180u)  /* Timer1_A3 Control */
DEFW(   TA1CTL               , TA1CTL_)
#define TA1CCTL0_              (0x0182u)  /* Timer1_A3 Capture/Compare Control 0 */
DEFW(   TA1CCTL0             , TA1CCTL0_)
#define TA1CCTL1_              (0x0184u)  /* Timer1_A3 Capture/Compare Control 1 */
DEFW(   TA1CCTL1             , TA1CCTL1_)
#define TA1CCTL2_              (0x0186u)  /* Timer1_A3 Capture/Compare Control 2 */
DEFW(   TA1CCTL2             , TA1CCTL2_)
#define TA1R_                  (0x0190u)  /* Timer1_A3 Counter Register */
DEFW(   TA1R                 , TA1R_)
#define TA1CCR0_               (0x0192u)  /* Timer1_A3 Capture/Compare 0 */
DEFW(   TA1CCR0              , TA1CCR0_)
#define TA1CCR1_               (0x0194u)  /* Timer1_A3 Capture/Compare 1 */
DEFW(   TA1CCR1              , TA1CCR1_)
#define TA1CCR2_               (0x0196u)  /* Timer1_A3 Capture/Compare 2 */
DEFW(   TA1CCR2              , TA1CCR2_)

/* Bits are already defined within the Timer0_Ax */

/* TA1IV Definitions */
#define TA1IV_NONE          (0x0000u)    /* No Interrupt pending */
#define TA1IV_TACCR1        (0x0002u)    /* TA1CCR1_CCIFG */
#define TA1IV_TACCR2        (0x0004u)    /* TA1CCR2_CCIFG */
#define TA1IV_3             (0x0006u)    /* Reserved */
#define TA1IV_4             (0x0008u)    /* Reserved */
#define TA1IV_5             (0x000Au)    /* Reserved */
#define TA1IV_6             (0x000Cu)    /* Reserved */
#define TA1IV_TAIFG         (0x000Eu)    /* TA1IFG */

/************************************************************
* USCI A0
************************************************************/
#define __MSP430_HAS_EUSCI_A0__      /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_EUSCI_A0__ 0x0140
#define EUSCI_A0_BASE __MSP430_BASEADDRESS_EUSCI_A0__

#define UCA0CTLW0_             (0x0140u)  /* USCI A0 Control Word Register 0 */
DEFCW(  UCA0CTLW0            , UCA0CTLW0_)
#define UCA0CTL1            UCA0CTLW0_L  /* USCI A0 Control Register 1 */
#define UCA0CTL0            UCA0CTLW0_H  /* USCI A0 Control Register 0 */
#define UCA0CTLW1_             (0x0142u)  /* USCI A0 Control Word Register 1 */
DEFCW(  UCA0CTLW1            , UCA0CTLW1_)
#define UCA0BRW_               (0x0146u)  /* USCI A0 Baud Word Rate 0 */
DEFCW(  UCA0BRW              , UCA0BRW_)
#define UCA0BR0             UCA0BRW_L /* USCI A0 Baud Rate 0 */
#define UCA0BR1             UCA0BRW_H /* USCI A0 Baud Rate 1 */
#define UCA0MCTLW_             (0x0148u)  /* USCI A0 Modulation Control */
DEFCW(  UCA0MCTLW            , UCA0MCTLW_)
#define UCA0STATW_             (0x014Au)  /* USCI A0 Status Register */
DEFC(   UCA0STATW            , UCA0STATW_)
#define UCA0RXBUF_            (0x014Cu)  /* USCI A0 Receive Buffer */
READ_ONLY_DEFCW( UCA0RXBUF        , UCA0RXBUF_)
#define  UCA0TXBUF_            (0x014Eu)  /* USCI A0 Transmit Buffer */
DEFCW(   UCA0TXBUF           , UCA0TXBUF_)
#define UCA0ABCTL_             (0x0150u)  /* USCI A0 LIN Control */
DEFC(   UCA0ABCTL            , UCA0ABCTL_)
#define UCA0IRCTL_             (0x0152u)  /* USCI A0 IrDA Transmit Control */
DEFCW(  UCA0IRCTL            , UCA0IRCTL_)
#define UCA0IRTCTL          UCA0IRCTL_L  /* USCI A0 IrDA Transmit Control */
#define UCA0IRRCTL          UCA0IRCTL_H  /* USCI A0 IrDA Receive Control */
#define UCA0IE_                (0x015Au)  /* USCI A0 Interrupt Enable Register */
DEFCW(  UCA0IE               , UCA0IE_)
#define UCA0IFG_               (0x015Cu)  /* USCI A0 Interrupt Flags Register */
DEFCW(  UCA0IFG              , UCA0IFG_)
#define UCA0IV_                (0x015Eu)  /* USCI A0 Interrupt Vector Register */
DEFW(   UCA0IV               , UCA0IV_)


/************************************************************
* USCI B0
************************************************************/
#define __MSP430_HAS_EUSCI_B0__       /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_EUSCI_B0__ 0x01C0
#define EUSCI_B0_BASE __MSP430_BASEADDRESS_EUSCI_B0__


#define UCB0CTLW0_             (0x01C0u)  /* USCI B0 Control Word Register 0 */
DEFCW(  UCB0CTLW0            , UCB0CTLW0_)
#define UCB0CTL1            UCB0CTLW0_L  /* USCI B0 Control Register 1 */
#define UCB0CTL0            UCB0CTLW0_H  /* USCI B0 Control Register 0 */
#define UCB0CTLW1_             (0x01C2u)  /* USCI B0 Control Word Register 1 */
DEFCW(  UCB0CTLW1            , UCB0CTLW1_)
#define UCB0BRW_               (0x01C6u)  /* USCI B0 Baud Word Rate 0 */
DEFCW(  UCB0BRW              , UCB0BRW_)
#define UCB0BR0             UCB0BRW_L /* USCI B0 Baud Rate 0 */
#define UCB0BR1             UCB0BRW_H /* USCI B0 Baud Rate 1 */
#define UCB0STATW_             (0x01C8u)  /* USCI B0 Status Word Register */
DEFCW(  UCB0STATW            , UCB0STATW_)
#define UCB0STAT            UCB0STATW_L /* USCI B0 Status Register */
#define UCB0BCNT            UCB0STATW_H /* USCI B0 Byte Counter Register */
#define UCB0TBCNT_             (0x01CAu)  /* USCI B0 Byte Counter Threshold Register */
DEFCW(  UCB0TBCNT            , UCB0TBCNT_)
#define UCB0RXBUF_            (0x01CCu)  /* USCI B0 Receive Buffer */
READ_ONLY_DEFCW( UCB0RXBUF        , UCB0RXBUF_)
#define  UCB0TXBUF_            (0x01CEu)  /* USCI B0 Transmit Buffer */
DEFCW(   UCB0TXBUF           , UCB0TXBUF_)
#define UCB0I2COA0_            (0x01D4u)  /* USCI B0 I2C Own Address 0 */
DEFCW(  UCB0I2COA0           , UCB0I2COA0_)
#define UCB0I2COA1_            (0x01D6u)  /* USCI B0 I2C Own Address 1 */
DEFCW(  UCB0I2COA1           , UCB0I2COA1_)
#define UCB0I2COA2_            (0x01D8u)  /* USCI B0 I2C Own Address 2 */
DEFCW(  UCB0I2COA2           , UCB0I2COA2_)
#define UCB0I2COA3_            (0x01DAu)  /* USCI B0 I2C Own Address 3 */
DEFCW(  UCB0I2COA3           , UCB0I2COA3_)
#define UCB0ADDRX_             (0x01DCu)  /* USCI B0 Received Address Register */
DEFCW(  UCB0ADDRX            , UCB0ADDRX_)
#define UCB0ADDMASK_           (0x01DEu)  /* USCI B0 Address Mask Register */
DEFCW(  UCB0ADDMASK          , UCB0ADDMASK_)
#define UCB0I2CSA_             (0x01E0u)  /* USCI B0 I2C Slave Address */
DEFCW(  UCB0I2CSA            , UCB0I2CSA_)
#define UCB0IE_                (0x01EAu)  /* USCI B0 Interrupt Enable Register */
DEFCW(  UCB0IE               , UCB0IE_)
#define UCB0IFG_               (0x01ECu)  /* USCI B0 Interrupt Flags Register */
DEFCW(  UCB0IFG              , UCB0IFG_)
#define UCB0IV_                (0x01EEu)  /* USCI B0 Interrupt Vector Register */
DEFW(   UCB0IV               , UCB0IV_)

// UCAxCTLW0 UART-Mode Control Bits
#define UCPEN               (0x8000u)    /* Async. Mode: Parity enable */
#define UCPAR               (0x4000u)    /* Async. Mode: Parity     0:odd / 1:even */
#define UCMSB               (0x2000u)    /* Async. Mode: MSB first  0:LSB / 1:MSB */
#define UC7BIT              (0x1000u)    /* Async. Mode: Data Bits  0:8-bits / 1:7-bits */
#define UCSPB               (0x0800u)    /* Async. Mode: Stop Bits  0:one / 1: two */
#define UCMODE1             (0x0400u)    /* Async. Mode: USCI Mode 1 */
#define UCMODE0             (0x0200u)    /* Async. Mode: USCI Mode 0 */
#define UCSYNC              (0x0100u)    /* Sync-Mode  0:UART-Mode / 1:SPI-Mode */
#define UCSSEL1             (0x0080u)    /* USCI 0 Clock Source Select 1 */
#define UCSSEL0             (0x0040u)    /* USCI 0 Clock Source Select 0 */
#define UCRXEIE             (0x0020u)    /* RX Error interrupt enable */
#define UCBRKIE             (0x0010u)    /* Break interrupt enable */
#define UCDORM              (0x0008u)    /* Dormant (Sleep) Mode */
#define UCTXADDR            (0x0004u)    /* Send next Data as Address */
#define UCTXBRK             (0x0002u)    /* Send next Data as Break */
#define UCSWRST             (0x0001u)    /* USCI Software Reset */

// UCAxCTLW0 UART-Mode Control Bits
#define UCSSEL1_L           (0x0080u)    /* USCI 0 Clock Source Select 1 */
#define UCSSEL0_L           (0x0040u)    /* USCI 0 Clock Source Select 0 */
#define UCRXEIE_L           (0x0020u)    /* RX Error interrupt enable */
#define UCBRKIE_L           (0x0010u)    /* Break interrupt enable */
#define UCDORM_L            (0x0008u)    /* Dormant (Sleep) Mode */
#define UCTXADDR_L          (0x0004u)    /* Send next Data as Address */
#define UCTXBRK_L           (0x0002u)    /* Send next Data as Break */
#define UCSWRST_L           (0x0001u)    /* USCI Software Reset */

// UCAxCTLW0 UART-Mode Control Bits
#define UCPEN_H             (0x0080u)    /* Async. Mode: Parity enable */
#define UCPAR_H             (0x0040u)    /* Async. Mode: Parity     0:odd / 1:even */
#define UCMSB_H             (0x0020u)    /* Async. Mode: MSB first  0:LSB / 1:MSB */
#define UC7BIT_H            (0x0010u)    /* Async. Mode: Data Bits  0:8-bits / 1:7-bits */
#define UCSPB_H             (0x0008u)    /* Async. Mode: Stop Bits  0:one / 1: two */
#define UCMODE1_H           (0x0004u)    /* Async. Mode: USCI Mode 1 */
#define UCMODE0_H           (0x0002u)    /* Async. Mode: USCI Mode 0 */
#define UCSYNC_H            (0x0001u)    /* Sync-Mode  0:UART-Mode / 1:SPI-Mode */

// UCxxCTLW0 SPI-Mode Control Bits
#define UCCKPH              (0x8000u)    /* Sync. Mode: Clock Phase */
#define UCCKPL              (0x4000u)    /* Sync. Mode: Clock Polarity */
#define UCMST               (0x0800u)    /* Sync. Mode: Master Select */
//#define res               (0x0020u)    /* reserved */
//#define res               (0x0010u)    /* reserved */
//#define res               (0x0008u)    /* reserved */
//#define res               (0x0004u)    /* reserved */
#define UCSTEM             (0x0002u)    /* USCI STE Mode */

// UCBxCTLW0 I2C-Mode Control Bits
#define UCA10               (0x8000u)    /* 10-bit Address Mode */
#define UCSLA10             (0x4000u)    /* 10-bit Slave Address Mode */
#define UCMM                (0x2000u)    /* Multi-Master Environment */
//#define res               (0x1000u)    /* reserved */
//#define res               (0x0100u)    /* reserved */
#define UCTXACK             (0x0020u)    /* Transmit ACK */
#define UCTR                (0x0010u)    /* Transmit/Receive Select/Flag */
#define UCTXNACK            (0x0008u)    /* Transmit NACK */
#define UCTXSTP             (0x0004u)    /* Transmit STOP */
#define UCTXSTT             (0x0002u)    /* Transmit START */

// UCBxCTLW0 I2C-Mode Control Bits
//#define res               (0x1000u)    /* reserved */
//#define res               (0x0100u)    /* reserved */
#define UCTXACK_L           (0x0020u)    /* Transmit ACK */
#define UCTR_L              (0x0010u)    /* Transmit/Receive Select/Flag */
#define UCTXNACK_L          (0x0008u)    /* Transmit NACK */
#define UCTXSTP_L           (0x0004u)    /* Transmit STOP */
#define UCTXSTT_L           (0x0002u)    /* Transmit START */

// UCBxCTLW0 I2C-Mode Control Bits
#define UCA10_H             (0x0080u)    /* 10-bit Address Mode */
#define UCSLA10_H           (0x0040u)    /* 10-bit Slave Address Mode */
#define UCMM_H              (0x0020u)    /* Multi-Master Environment */
//#define res               (0x1000u)    /* reserved */
//#define res               (0x0100u)    /* reserved */

#define UCMODE_0            (0x0000u)    /* Sync. Mode: USCI Mode: 0 */
#define UCMODE_1            (0x0200u)    /* Sync. Mode: USCI Mode: 1 */
#define UCMODE_2            (0x0400u)    /* Sync. Mode: USCI Mode: 2 */
#define UCMODE_3            (0x0600u)    /* Sync. Mode: USCI Mode: 3 */

#define UCSSEL_0            (0x0000u)    /* USCI 0 Clock Source: 0 */
#define UCSSEL_1            (0x0040u)    /* USCI 0 Clock Source: 1 */
#define UCSSEL_2            (0x0080u)    /* USCI 0 Clock Source: 2 */
#define UCSSEL_3            (0x00C0u)    /* USCI 0 Clock Source: 3 */
#define UCSSEL__UCLK        (0x0000u)    /* USCI 0 Clock Source: UCLK */
#define UCSSEL__ACLK        (0x0040u)    /* USCI 0 Clock Source: ACLK */
#define UCSSEL__SMCLK       (0x0080u)    /* USCI 0 Clock Source: SMCLK */

// UCAxCTLW1 UART-Mode Control Bits
#define UCGLIT1             (0x0002u)    /* USCI Deglitch Time Bit 1 */
#define UCGLIT0             (0x0001u)    /* USCI Deglitch Time Bit 0 */

// UCAxCTLW1 UART-Mode Control Bits
#define UCGLIT1_L           (0x0002u)    /* USCI Deglitch Time Bit 1 */
#define UCGLIT0_L           (0x0001u)    /* USCI Deglitch Time Bit 0 */

// UCBxCTLW1 I2C-Mode Control Bits
#define UCETXINT            (0x0100u)    /* USCI Early UCTXIFG0 */
#define UCCLTO1             (0x0080u)    /* USCI Clock low timeout Bit: 1 */
#define UCCLTO0             (0x0040u)    /* USCI Clock low timeout Bit: 0 */
#define UCSTPNACK           (0x0020u)    /* USCI Acknowledge Stop last byte */
#define UCSWACK             (0x0010u)    /* USCI Software controlled ACK */
#define UCASTP1             (0x0008u)    /* USCI Automatic Stop condition generation Bit: 1 */
#define UCASTP0             (0x0004u)    /* USCI Automatic Stop condition generation Bit: 0 */
#define UCGLIT1             (0x0002u)    /* USCI Deglitch time Bit: 1 */
#define UCGLIT0             (0x0001u)    /* USCI Deglitch time Bit: 0 */

// UCBxCTLW1 I2C-Mode Control Bits
#define UCCLTO1_L           (0x0080u)    /* USCI Clock low timeout Bit: 1 */
#define UCCLTO0_L           (0x0040u)    /* USCI Clock low timeout Bit: 0 */
#define UCSTPNACK_L         (0x0020u)    /* USCI Acknowledge Stop last byte */
#define UCSWACK_L           (0x0010u)    /* USCI Software controlled ACK */
#define UCASTP1_L           (0x0008u)    /* USCI Automatic Stop condition generation Bit: 1 */
#define UCASTP0_L           (0x0004u)    /* USCI Automatic Stop condition generation Bit: 0 */
#define UCGLIT1_L           (0x0002u)    /* USCI Deglitch time Bit: 1 */
#define UCGLIT0_L           (0x0001u)    /* USCI Deglitch time Bit: 0 */

// UCBxCTLW1 I2C-Mode Control Bits
#define UCETXINT_H          (0x0001u)    /* USCI Early UCTXIFG0 */

#define UCGLIT_0            (0x0000u)    /* USCI Deglitch time: 0 */
#define UCGLIT_1            (0x0001u)    /* USCI Deglitch time: 1 */
#define UCGLIT_2            (0x0002u)    /* USCI Deglitch time: 2 */
#define UCGLIT_3            (0x0003u)    /* USCI Deglitch time: 3 */

#define UCASTP_0            (0x0000u)    /* USCI Automatic Stop condition generation: 0 */
#define UCASTP_1            (0x0004u)    /* USCI Automatic Stop condition generation: 1 */
#define UCASTP_2            (0x0008u)    /* USCI Automatic Stop condition generation: 2 */
#define UCASTP_3            (0x000Cu)    /* USCI Automatic Stop condition generation: 3 */

#define UCCLTO_0            (0x0000u)    /* USCI Clock low timeout: 0 */
#define UCCLTO_1            (0x0040u)    /* USCI Clock low timeout: 1 */
#define UCCLTO_2            (0x0080u)    /* USCI Clock low timeout: 2 */
#define UCCLTO_3            (0x00C0u)    /* USCI Clock low timeout: 3 */

/* UCAxMCTLW Control Bits */
#define UCBRS7              (0x8000u)    /* USCI Second Stage Modulation Select 7 */
#define UCBRS6              (0x4000u)    /* USCI Second Stage Modulation Select 6 */
#define UCBRS5              (0x2000u)    /* USCI Second Stage Modulation Select 5 */
#define UCBRS4              (0x1000u)    /* USCI Second Stage Modulation Select 4 */
#define UCBRS3              (0x0800u)    /* USCI Second Stage Modulation Select 3 */
#define UCBRS2              (0x0400u)    /* USCI Second Stage Modulation Select 2 */
#define UCBRS1              (0x0200u)    /* USCI Second Stage Modulation Select 1 */
#define UCBRS0              (0x0100u)    /* USCI Second Stage Modulation Select 0 */
#define UCBRF3              (0x0080u)    /* USCI First Stage Modulation Select 3 */
#define UCBRF2              (0x0040u)    /* USCI First Stage Modulation Select 2 */
#define UCBRF1              (0x0020u)    /* USCI First Stage Modulation Select 1 */
#define UCBRF0              (0x0010u)    /* USCI First Stage Modulation Select 0 */
#define UCOS16              (0x0001u)    /* USCI 16-times Oversampling enable */

/* UCAxMCTLW Control Bits */
#define UCBRF3_L            (0x0080u)    /* USCI First Stage Modulation Select 3 */
#define UCBRF2_L            (0x0040u)    /* USCI First Stage Modulation Select 2 */
#define UCBRF1_L            (0x0020u)    /* USCI First Stage Modulation Select 1 */
#define UCBRF0_L            (0x0010u)    /* USCI First Stage Modulation Select 0 */
#define UCOS16_L            (0x0001u)    /* USCI 16-times Oversampling enable */

/* UCAxMCTLW Control Bits */
#define UCBRS7_H            (0x0080u)    /* USCI Second Stage Modulation Select 7 */
#define UCBRS6_H            (0x0040u)    /* USCI Second Stage Modulation Select 6 */
#define UCBRS5_H            (0x0020u)    /* USCI Second Stage Modulation Select 5 */
#define UCBRS4_H            (0x0010u)    /* USCI Second Stage Modulation Select 4 */
#define UCBRS3_H            (0x0008u)    /* USCI Second Stage Modulation Select 3 */
#define UCBRS2_H            (0x0004u)    /* USCI Second Stage Modulation Select 2 */
#define UCBRS1_H            (0x0002u)    /* USCI Second Stage Modulation Select 1 */
#define UCBRS0_H            (0x0001u)    /* USCI Second Stage Modulation Select 0 */

#define UCBRF_0             (0x00)    /* USCI First Stage Modulation: 0 */
#define UCBRF_1             (0x10)    /* USCI First Stage Modulation: 1 */
#define UCBRF_2             (0x20)    /* USCI First Stage Modulation: 2 */
#define UCBRF_3             (0x30)    /* USCI First Stage Modulation: 3 */
#define UCBRF_4             (0x40)    /* USCI First Stage Modulation: 4 */
#define UCBRF_5             (0x50)    /* USCI First Stage Modulation: 5 */
#define UCBRF_6             (0x60)    /* USCI First Stage Modulation: 6 */
#define UCBRF_7             (0x70)    /* USCI First Stage Modulation: 7 */
#define UCBRF_8             (0x80)    /* USCI First Stage Modulation: 8 */
#define UCBRF_9             (0x90)    /* USCI First Stage Modulation: 9 */
#define UCBRF_10            (0xA0)    /* USCI First Stage Modulation: A */
#define UCBRF_11            (0xB0)    /* USCI First Stage Modulation: B */
#define UCBRF_12            (0xC0)    /* USCI First Stage Modulation: C */
#define UCBRF_13            (0xD0)    /* USCI First Stage Modulation: D */
#define UCBRF_14            (0xE0)    /* USCI First Stage Modulation: E */
#define UCBRF_15            (0xF0)    /* USCI First Stage Modulation: F */

/* UCAxSTATW Control Bits */
#define UCLISTEN            (0x0080u)  /* USCI Listen mode */
#define UCFE                (0x0040u)  /* USCI Frame Error Flag */
#define UCOE                (0x0020u)  /* USCI Overrun Error Flag */
#define UCPE                (0x0010u)  /* USCI Parity Error Flag */
#define UCBRK               (0x0008u)  /* USCI Break received */
#define UCRXERR             (0x0004u)  /* USCI RX Error Flag */
#define UCADDR              (0x0002u)  /* USCI Address received Flag */
#define UCBUSY              (0x0001u)  /* USCI Busy Flag */
#define UCIDLE              (0x0002u)  /* USCI Idle line detected Flag */

/* UCBxSTATW I2C Control Bits */
#define UCBCNT7             (0x8000u)  /* USCI Byte Counter Bit 7 */
#define UCBCNT6             (0x4000u)  /* USCI Byte Counter Bit 6 */
#define UCBCNT5             (0x2000u)  /* USCI Byte Counter Bit 5 */
#define UCBCNT4             (0x1000u)  /* USCI Byte Counter Bit 4 */
#define UCBCNT3             (0x0800u)  /* USCI Byte Counter Bit 3 */
#define UCBCNT2             (0x0400u)  /* USCI Byte Counter Bit 2 */
#define UCBCNT1             (0x0200u)  /* USCI Byte Counter Bit 1 */
#define UCBCNT0             (0x0100u)  /* USCI Byte Counter Bit 0 */
#define UCSCLLOW            (0x0040u)  /* SCL low */
#define UCGC                (0x0020u)  /* General Call address received Flag */
#define UCBBUSY             (0x0010u)  /* Bus Busy Flag */

/* UCBxTBCNT I2C Control Bits */
#define UCTBCNT7            (0x0080u)  /* USCI Byte Counter Bit 7 */
#define UCTBCNT6            (0x0040u)  /* USCI Byte Counter Bit 6 */
#define UCTBCNT5            (0x0020u)  /* USCI Byte Counter Bit 5 */
#define UCTBCNT4            (0x0010u)  /* USCI Byte Counter Bit 4 */
#define UCTBCNT3            (0x0008u)  /* USCI Byte Counter Bit 3 */
#define UCTBCNT2            (0x0004u)  /* USCI Byte Counter Bit 2 */
#define UCTBCNT1            (0x0002u)  /* USCI Byte Counter Bit 1 */
#define UCTBCNT0            (0x0001u)  /* USCI Byte Counter Bit 0 */

/* UCAxIRCTL Control Bits */
#define UCIRRXFL5           (0x8000u)  /* IRDA Receive Filter Length 5 */
#define UCIRRXFL4           (0x4000u)  /* IRDA Receive Filter Length 4 */
#define UCIRRXFL3           (0x2000u)  /* IRDA Receive Filter Length 3 */
#define UCIRRXFL2           (0x1000u)  /* IRDA Receive Filter Length 2 */
#define UCIRRXFL1           (0x0800u)  /* IRDA Receive Filter Length 1 */
#define UCIRRXFL0           (0x0400u)  /* IRDA Receive Filter Length 0 */
#define UCIRRXPL            (0x0200u)  /* IRDA Receive Input Polarity */
#define UCIRRXFE            (0x0100u)  /* IRDA Receive Filter enable */
#define UCIRTXPL5           (0x0080u)  /* IRDA Transmit Pulse Length 5 */
#define UCIRTXPL4           (0x0040u)  /* IRDA Transmit Pulse Length 4 */
#define UCIRTXPL3           (0x0020u)  /* IRDA Transmit Pulse Length 3 */
#define UCIRTXPL2           (0x0010u)  /* IRDA Transmit Pulse Length 2 */
#define UCIRTXPL1           (0x0008u)  /* IRDA Transmit Pulse Length 1 */
#define UCIRTXPL0           (0x0004u)  /* IRDA Transmit Pulse Length 0 */
#define UCIRTXCLK           (0x0002u)  /* IRDA Transmit Pulse Clock Select */
#define UCIREN              (0x0001u)  /* IRDA Encoder/Decoder enable */

/* UCAxIRCTL Control Bits */
#define UCIRTXPL5_L         (0x0080u)  /* IRDA Transmit Pulse Length 5 */
#define UCIRTXPL4_L         (0x0040u)  /* IRDA Transmit Pulse Length 4 */
#define UCIRTXPL3_L         (0x0020u)  /* IRDA Transmit Pulse Length 3 */
#define UCIRTXPL2_L         (0x0010u)  /* IRDA Transmit Pulse Length 2 */
#define UCIRTXPL1_L         (0x0008u)  /* IRDA Transmit Pulse Length 1 */
#define UCIRTXPL0_L         (0x0004u)  /* IRDA Transmit Pulse Length 0 */
#define UCIRTXCLK_L         (0x0002u)  /* IRDA Transmit Pulse Clock Select */
#define UCIREN_L            (0x0001u)  /* IRDA Encoder/Decoder enable */

/* UCAxIRCTL Control Bits */
#define UCIRRXFL5_H         (0x0080u)  /* IRDA Receive Filter Length 5 */
#define UCIRRXFL4_H         (0x0040u)  /* IRDA Receive Filter Length 4 */
#define UCIRRXFL3_H         (0x0020u)  /* IRDA Receive Filter Length 3 */
#define UCIRRXFL2_H         (0x0010u)  /* IRDA Receive Filter Length 2 */
#define UCIRRXFL1_H         (0x0008u)  /* IRDA Receive Filter Length 1 */
#define UCIRRXFL0_H         (0x0004u)  /* IRDA Receive Filter Length 0 */
#define UCIRRXPL_H          (0x0002u)  /* IRDA Receive Input Polarity */
#define UCIRRXFE_H          (0x0001u)  /* IRDA Receive Filter enable */

/* UCAxABCTL Control Bits */
//#define res               (0x80)    /* reserved */
//#define res               (0x40)    /* reserved */
#define UCDELIM1            (0x20)    /* Break Sync Delimiter 1 */
#define UCDELIM0            (0x10)    /* Break Sync Delimiter 0 */
#define UCSTOE              (0x08)    /* Sync-Field Timeout error */
#define UCBTOE              (0x04)    /* Break Timeout error */
//#define res               (0x02)    /* reserved */
#define UCABDEN             (0x01)    /* Auto Baud Rate detect enable */

/* UCBxI2COA0 Control Bits */
#define UCGCEN              (0x8000u)  /* I2C General Call enable */
#define UCOAEN              (0x0400u)  /* I2C Own Address enable */
#define UCOA9               (0x0200u)  /* I2C Own Address Bit 9 */
#define UCOA8               (0x0100u)  /* I2C Own Address Bit 8 */
#define UCOA7               (0x0080u)  /* I2C Own Address Bit 7 */
#define UCOA6               (0x0040u)  /* I2C Own Address Bit 6 */
#define UCOA5               (0x0020u)  /* I2C Own Address Bit 5 */
#define UCOA4               (0x0010u)  /* I2C Own Address Bit 4 */
#define UCOA3               (0x0008u)  /* I2C Own Address Bit 3 */
#define UCOA2               (0x0004u)  /* I2C Own Address Bit 2 */
#define UCOA1               (0x0002u)  /* I2C Own Address Bit 1 */
#define UCOA0               (0x0001u)  /* I2C Own Address Bit 0 */

/* UCBxI2COA0 Control Bits */
#define UCOA7_L             (0x0080u)  /* I2C Own Address Bit 7 */
#define UCOA6_L             (0x0040u)  /* I2C Own Address Bit 6 */
#define UCOA5_L             (0x0020u)  /* I2C Own Address Bit 5 */
#define UCOA4_L             (0x0010u)  /* I2C Own Address Bit 4 */
#define UCOA3_L             (0x0008u)  /* I2C Own Address Bit 3 */
#define UCOA2_L             (0x0004u)  /* I2C Own Address Bit 2 */
#define UCOA1_L             (0x0002u)  /* I2C Own Address Bit 1 */
#define UCOA0_L             (0x0001u)  /* I2C Own Address Bit 0 */

/* UCBxI2COA0 Control Bits */
#define UCGCEN_H            (0x0080u)  /* I2C General Call enable */
#define UCOAEN_H            (0x0004u)  /* I2C Own Address enable */
#define UCOA9_H             (0x0002u)  /* I2C Own Address Bit 9 */
#define UCOA8_H             (0x0001u)  /* I2C Own Address Bit 8 */

/* UCBxI2COAx Control Bits */
#define UCOAEN              (0x0400u)  /* I2C Own Address enable */
#define UCOA9               (0x0200u)  /* I2C Own Address Bit 9 */
#define UCOA8               (0x0100u)  /* I2C Own Address Bit 8 */
#define UCOA7               (0x0080u)  /* I2C Own Address Bit 7 */
#define UCOA6               (0x0040u)  /* I2C Own Address Bit 6 */
#define UCOA5               (0x0020u)  /* I2C Own Address Bit 5 */
#define UCOA4               (0x0010u)  /* I2C Own Address Bit 4 */
#define UCOA3               (0x0008u)  /* I2C Own Address Bit 3 */
#define UCOA2               (0x0004u)  /* I2C Own Address Bit 2 */
#define UCOA1               (0x0002u)  /* I2C Own Address Bit 1 */
#define UCOA0               (0x0001u)  /* I2C Own Address Bit 0 */

/* UCBxI2COAx Control Bits */
#define UCOA7_L             (0x0080u)  /* I2C Own Address Bit 7 */
#define UCOA6_L             (0x0040u)  /* I2C Own Address Bit 6 */
#define UCOA5_L             (0x0020u)  /* I2C Own Address Bit 5 */
#define UCOA4_L             (0x0010u)  /* I2C Own Address Bit 4 */
#define UCOA3_L             (0x0008u)  /* I2C Own Address Bit 3 */
#define UCOA2_L             (0x0004u)  /* I2C Own Address Bit 2 */
#define UCOA1_L             (0x0002u)  /* I2C Own Address Bit 1 */
#define UCOA0_L             (0x0001u)  /* I2C Own Address Bit 0 */

/* UCBxI2COAx Control Bits */
#define UCOAEN_H            (0x0004u)  /* I2C Own Address enable */
#define UCOA9_H             (0x0002u)  /* I2C Own Address Bit 9 */
#define UCOA8_H             (0x0001u)  /* I2C Own Address Bit 8 */

/* UCBxADDRX Control Bits */
#define UCADDRX9            (0x0200u)  /* I2C Receive Address Bit 9 */
#define UCADDRX8            (0x0100u)  /* I2C Receive Address Bit 8 */
#define UCADDRX7            (0x0080u)  /* I2C Receive Address Bit 7 */
#define UCADDRX6            (0x0040u)  /* I2C Receive Address Bit 6 */
#define UCADDRX5            (0x0020u)  /* I2C Receive Address Bit 5 */
#define UCADDRX4            (0x0010u)  /* I2C Receive Address Bit 4 */
#define UCADDRX3            (0x0008u)  /* I2C Receive Address Bit 3 */
#define UCADDRX2            (0x0004u)  /* I2C Receive Address Bit 2 */
#define UCADDRX1            (0x0002u)  /* I2C Receive Address Bit 1 */
#define UCADDRX0            (0x0001u)  /* I2C Receive Address Bit 0 */

/* UCBxADDRX Control Bits */
#define UCADDRX7_L          (0x0080u)  /* I2C Receive Address Bit 7 */
#define UCADDRX6_L          (0x0040u)  /* I2C Receive Address Bit 6 */
#define UCADDRX5_L          (0x0020u)  /* I2C Receive Address Bit 5 */
#define UCADDRX4_L          (0x0010u)  /* I2C Receive Address Bit 4 */
#define UCADDRX3_L          (0x0008u)  /* I2C Receive Address Bit 3 */
#define UCADDRX2_L          (0x0004u)  /* I2C Receive Address Bit 2 */
#define UCADDRX1_L          (0x0002u)  /* I2C Receive Address Bit 1 */
#define UCADDRX0_L          (0x0001u)  /* I2C Receive Address Bit 0 */

/* UCBxADDRX Control Bits */
#define UCADDRX9_H          (0x0002u)  /* I2C Receive Address Bit 9 */
#define UCADDRX8_H          (0x0001u)  /* I2C Receive Address Bit 8 */

/* UCBxADDMASK Control Bits */
#define UCADDMASK9            (0x0200u)  /* I2C Address Mask Bit 9 */
#define UCADDMASK8            (0x0100u)  /* I2C Address Mask Bit 8 */
#define UCADDMASK7            (0x0080u)  /* I2C Address Mask Bit 7 */
#define UCADDMASK6            (0x0040u)  /* I2C Address Mask Bit 6 */
#define UCADDMASK5            (0x0020u)  /* I2C Address Mask Bit 5 */
#define UCADDMASK4            (0x0010u)  /* I2C Address Mask Bit 4 */
#define UCADDMASK3            (0x0008u)  /* I2C Address Mask Bit 3 */
#define UCADDMASK2            (0x0004u)  /* I2C Address Mask Bit 2 */
#define UCADDMASK1            (0x0002u)  /* I2C Address Mask Bit 1 */
#define UCADDMASK0            (0x0001u)  /* I2C Address Mask Bit 0 */

/* UCBxADDMASK Control Bits */
#define UCADDMASK7_L        (0x0080u)  /* I2C Address Mask Bit 7 */
#define UCADDMASK6_L        (0x0040u)  /* I2C Address Mask Bit 6 */
#define UCADDMASK5_L        (0x0020u)  /* I2C Address Mask Bit 5 */
#define UCADDMASK4_L        (0x0010u)  /* I2C Address Mask Bit 4 */
#define UCADDMASK3_L        (0x0008u)  /* I2C Address Mask Bit 3 */
#define UCADDMASK2_L        (0x0004u)  /* I2C Address Mask Bit 2 */
#define UCADDMASK1_L        (0x0002u)  /* I2C Address Mask Bit 1 */
#define UCADDMASK0_L        (0x0001u)  /* I2C Address Mask Bit 0 */

/* UCBxADDMASK Control Bits */
#define UCADDMASK9_H        (0x0002u)  /* I2C Address Mask Bit 9 */
#define UCADDMASK8_H        (0x0001u)  /* I2C Address Mask Bit 8 */

/* UCBxI2CSA Control Bits */
#define UCSA9               (0x0200u)  /* I2C Slave Address Bit 9 */
#define UCSA8               (0x0100u)  /* I2C Slave Address Bit 8 */
#define UCSA7               (0x0080u)  /* I2C Slave Address Bit 7 */
#define UCSA6               (0x0040u)  /* I2C Slave Address Bit 6 */
#define UCSA5               (0x0020u)  /* I2C Slave Address Bit 5 */
#define UCSA4               (0x0010u)  /* I2C Slave Address Bit 4 */
#define UCSA3               (0x0008u)  /* I2C Slave Address Bit 3 */
#define UCSA2               (0x0004u)  /* I2C Slave Address Bit 2 */
#define UCSA1               (0x0002u)  /* I2C Slave Address Bit 1 */
#define UCSA0               (0x0001u)  /* I2C Slave Address Bit 0 */

/* UCBxI2CSA Control Bits */
#define UCSA7_L             (0x0080u)  /* I2C Slave Address Bit 7 */
#define UCSA6_L             (0x0040u)  /* I2C Slave Address Bit 6 */
#define UCSA5_L             (0x0020u)  /* I2C Slave Address Bit 5 */
#define UCSA4_L             (0x0010u)  /* I2C Slave Address Bit 4 */
#define UCSA3_L             (0x0008u)  /* I2C Slave Address Bit 3 */
#define UCSA2_L             (0x0004u)  /* I2C Slave Address Bit 2 */
#define UCSA1_L             (0x0002u)  /* I2C Slave Address Bit 1 */
#define UCSA0_L             (0x0001u)  /* I2C Slave Address Bit 0 */

/* UCBxI2CSA Control Bits */
#define UCSA9_H             (0x0002u)  /* I2C Slave Address Bit 9 */
#define UCSA8_H             (0x0001u)  /* I2C Slave Address Bit 8 */

/* UCAxIE UART Control Bits */
#define UCTXCPTIE           (0x0008u)  /* UART Transmit Complete Interrupt Enable */
#define UCSTTIE             (0x0004u)  /* UART Start Bit Interrupt Enalble */
#define UCTXIE              (0x0002u)  /* UART Transmit Interrupt Enable */
#define UCRXIE              (0x0001u)  /* UART Receive Interrupt Enable */

/* UCAxIE/UCBxIE SPI Control Bits */

/* UCBxIE I2C Control Bits */
#define UCBIT9IE            (0x4000u)  /* I2C Bit 9 Position Interrupt Enable 3 */
#define UCTXIE3             (0x2000u)  /* I2C Transmit Interrupt Enable 3 */
#define UCRXIE3             (0x1000u)  /* I2C Receive Interrupt Enable 3 */
#define UCTXIE2             (0x0800u)  /* I2C Transmit Interrupt Enable 2 */
#define UCRXIE2             (0x0400u)  /* I2C Receive Interrupt Enable 2 */
#define UCTXIE1             (0x0200u)  /* I2C Transmit Interrupt Enable 1 */
#define UCRXIE1             (0x0100u)  /* I2C Receive Interrupt Enable 1 */
#define UCCLTOIE            (0x0080u)  /* I2C Clock Low Timeout interrupt enable */
#define UCBCNTIE            (0x0040u)  /* I2C Automatic stop assertion interrupt enable */
#define UCNACKIE            (0x0020u)  /* I2C NACK Condition interrupt enable */
#define UCALIE              (0x0010u)  /* I2C Arbitration Lost interrupt enable */
#define UCSTPIE             (0x0008u)  /* I2C STOP Condition interrupt enable */
#define UCSTTIE             (0x0004u)  /* I2C START Condition interrupt enable */
#define UCTXIE0             (0x0002u)  /* I2C Transmit Interrupt Enable 0 */
#define UCRXIE0             (0x0001u)  /* I2C Receive Interrupt Enable 0 */

/* UCAxIFG UART Control Bits */
#define UCTXCPTIFG          (0x0008u)  /* UART Transmit Complete Interrupt Flag */
#define UCSTTIFG            (0x0004u)  /* UART Start Bit Interrupt Flag */
#define UCTXIFG             (0x0002u)  /* UART Transmit Interrupt Flag */
#define UCRXIFG             (0x0001u)  /* UART Receive Interrupt Flag */

/* UCAxIFG/UCBxIFG SPI Control Bits */
#define UCTXIFG             (0x0002u)  /* SPI Transmit Interrupt Flag */
#define UCRXIFG             (0x0001u)  /* SPI Receive Interrupt Flag */

/* UCBxIFG Control Bits */
#define UCBIT9IFG           (0x4000u)  /* I2C Bit 9 Possition Interrupt Flag 3 */
#define UCTXIFG3            (0x2000u)  /* I2C Transmit Interrupt Flag 3 */
#define UCRXIFG3            (0x1000u)  /* I2C Receive Interrupt Flag 3 */
#define UCTXIFG2            (0x0800u)  /* I2C Transmit Interrupt Flag 2 */
#define UCRXIFG2            (0x0400u)  /* I2C Receive Interrupt Flag 2 */
#define UCTXIFG1            (0x0200u)  /* I2C Transmit Interrupt Flag 1 */
#define UCRXIFG1            (0x0100u)  /* I2C Receive Interrupt Flag 1 */
#define UCCLTOIFG           (0x0080u)  /* I2C Clock low Timeout interrupt Flag */
#define UCBCNTIFG           (0x0040u)  /* I2C Byte counter interrupt flag */
#define UCNACKIFG           (0x0020u)  /* I2C NACK Condition interrupt Flag */
#define UCALIFG             (0x0010u)  /* I2C Arbitration Lost interrupt Flag */
#define UCSTPIFG            (0x0008u)  /* I2C STOP Condition interrupt Flag */
#define UCSTTIFG            (0x0004u)  /* I2C START Condition interrupt Flag */
#define UCTXIFG0            (0x0002u)  /* I2C Transmit Interrupt Flag 0 */
#define UCRXIFG0            (0x0001u)  /* I2C Receive Interrupt Flag 0 */

/* USCI UART Definitions */
#define USCI_NONE            (0x0000u)   /* No Interrupt pending */
#define USCI_UART_UCRXIFG    (0x0002u)   /* USCI UCRXIFG */
#define USCI_UART_UCTXIFG    (0x0004u)   /* USCI UCTXIFG */
#define USCI_UART_UCSTTIFG   (0x0006u)   /* USCI UCSTTIFG */
#define USCI_UART_UCTXCPTIFG (0x0008u)   /* USCI UCTXCPTIFG */

/* USCI SPI Definitions */
#define USCI_SPI_UCRXIFG    (0x0002u)    /* USCI UCRXIFG */
#define USCI_SPI_UCTXIFG    (0x0004u)    /* USCI UCTXIFG */

/* USCI I2C Definitions */
#define USCI_I2C_UCALIFG    (0x0002u)    /* USCI I2C Mode: UCALIFG */
#define USCI_I2C_UCNACKIFG  (0x0004u)    /* USCI I2C Mode: UCNACKIFG */
#define USCI_I2C_UCSTTIFG   (0x0006u)    /* USCI I2C Mode: UCSTTIFG*/
#define USCI_I2C_UCSTPIFG   (0x0008u)    /* USCI I2C Mode: UCSTPIFG*/
#define USCI_I2C_UCRXIFG3   (0x000Au)    /* USCI I2C Mode: UCRXIFG3 */
#define USCI_I2C_UCTXIFG3   (0x000Cu)    /* USCI I2C Mode: UCTXIFG3 */
#define USCI_I2C_UCRXIFG2   (0x000Eu)    /* USCI I2C Mode: UCRXIFG2 */
#define USCI_I2C_UCTXIFG2   (0x0010u)    /* USCI I2C Mode: UCTXIFG2 */
#define USCI_I2C_UCRXIFG1   (0x0012u)    /* USCI I2C Mode: UCRXIFG1 */
#define USCI_I2C_UCTXIFG1   (0x0014u)    /* USCI I2C Mode: UCTXIFG1 */
#define USCI_I2C_UCRXIFG0   (0x0016u)    /* USCI I2C Mode: UCRXIFG0 */
#define USCI_I2C_UCTXIFG0   (0x0018u)    /* USCI I2C Mode: UCTXIFG0 */
#define USCI_I2C_UCBCNTIFG  (0x001Au)    /* USCI I2C Mode: UCBCNTIFG */
#define USCI_I2C_UCCLTOIFG  (0x001Cu)    /* USCI I2C Mode: UCCLTOIFG */
#define USCI_I2C_UCBIT9IFG  (0x001Eu)    /* USCI I2C Mode: UCBIT9IFG */

/************************************************************
* WATCHDOG TIMER
************************************************************/
#define __MSP430_HAS_WDT__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_WDT__ 0x0120
#define WDT_BASE __MSP430_BASEADDRESS_WDT__

#define WDTCTL_                (0x0120u)  /* Watchdog Timer Control */
DEFW(   WDTCTL               , WDTCTL_)
/* The bit names have been prefixed with "WDT" */
#define WDTIS0              (0x0001u)
#define WDTIS1              (0x0002u)
#define WDTSSEL             (0x0004u)
#define WDTCNTCL            (0x0008u)
#define WDTTMSEL            (0x0010u)
#define WDTNMI              (0x0020u)
#define WDTNMIES            (0x0040u)
#define WDTHOLD             (0x0080u)

#define WDTPW               (0x5A00u)

/* WDT-interval times [1ms] coded with Bits 0-2 */
/* WDT is clocked by fSMCLK (assumed 1MHz) */
#define WDT_MDLY_32         (WDTPW+WDTTMSEL+WDTCNTCL)                         /* 32ms interval (default) */
#define WDT_MDLY_8          (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS0)                  /* 8ms     " */
#define WDT_MDLY_0_5        (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS1)                  /* 0.5ms   " */
#define WDT_MDLY_0_064      (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS1+WDTIS0)           /* 0.064ms " */
/* WDT is clocked by fACLK (assumed 32KHz) */
#define WDT_ADLY_1000       (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL)                 /* 1000ms  " */
#define WDT_ADLY_250        (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL+WDTIS0)          /* 250ms   " */
#define WDT_ADLY_16         (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL+WDTIS1)          /* 16ms    " */
#define WDT_ADLY_1_9        (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL+WDTIS1+WDTIS0)   /* 1.9ms   " */
/* Watchdog mode -> reset after expired time */
/* WDT is clocked by fSMCLK (assumed 1MHz) */
#define WDT_MRST_32         (WDTPW+WDTCNTCL)                                  /* 32ms interval (default) */
#define WDT_MRST_8          (WDTPW+WDTCNTCL+WDTIS0)                           /* 8ms     " */
#define WDT_MRST_0_5        (WDTPW+WDTCNTCL+WDTIS1)                           /* 0.5ms   " */
#define WDT_MRST_0_064      (WDTPW+WDTCNTCL+WDTIS1+WDTIS0)                    /* 0.064ms " */
/* WDT is clocked by fACLK (assumed 32KHz) */
#define WDT_ARST_1000       (WDTPW+WDTCNTCL+WDTSSEL)                          /* 1000ms  " */
#define WDT_ARST_250        (WDTPW+WDTCNTCL+WDTSSEL+WDTIS0)                   /* 250ms   " */
#define WDT_ARST_16         (WDTPW+WDTCNTCL+WDTSSEL+WDTIS1)                   /* 16ms    " */
#define WDT_ARST_1_9        (WDTPW+WDTCNTCL+WDTSSEL+WDTIS1+WDTIS0)            /* 1.9ms   " */

/* INTERRUPT CONTROL */
/* These two bits are defined in the Special Function Registers */
/* #define WDTIE               0x01 */
/* #define WDTIFG              0x01 */


/************************************************************
* TLV Descriptors
************************************************************/
#define __MSP430_HAS_TLV__                 /* Definition to show that Module is available */

#define TLV_START             (0x13C0u)    /* Start Address of the TLV structure */
#define TLV_END               (0x13FFu)    /* End Address of the TLV structure */
#define JTAG_DIS_PWD1         (0xFFDCu)
#define JTAG_DIS_PWD2         (0xFFDEu)
#define TLV_CHKSUM_L            (0x00)
#define TLV_CHKSUM_H            (0x01)
#define TLV_DIE_RECORD_TAG      (0x02)
#define TLV_DIE_RECORD_LEN      (0x03)
#define TLV_LOT_WAFER_ID        (0x04)
#define TLV_DIE_X_POS           (0x08)
#define TLV_DIE_Y_POS           (0x0A)
#define TLV_DIE_TEST_RESULTS    (0x0C)
#define TLV_REF_TAG             (0x0E)
#define TLV_REF_LEN             (0x0F)
#define TLV_CAL_REFCAL1         (0x10)
#define TLV_CAL_REFCAL0         (0x11)
#define TLV_DCO_TAG             (0x12)
#define TLV_DCO_LEN             (0x13)
#define TLV_CAL_CSIRFCAL        (0x14)
#define TLV_CAL_CSIRTCAL        (0x15)
#define TLV_CAL_CSERFCAL        (0x16)
#define TLV_CAL_CSERTCAL        (0x17)
#define TLV_SD24_TAG            (0x18)
#define TLV_SD24_LEN            (0x19)
#define TLV_CAL_SD24TRIM        (0x1A)
#define TLV_EMPTY               (0x1B)
#define TLV_EMPTY_TAG           (0x1C)
#define TLV_EMPTY_LEN           (0x1D)

/************************************************************
* Interrupt Vectors (offset from 0xFFE0)
************************************************************/

#define PORT2_VECTOR        (1 * 2u)  /* 0xFFE2 Port 2 */
#define PORT1_VECTOR        (4 * 2u)  /* 0xFFE8 Port 1 */
#define TIMER0_A1_VECTOR    (5 * 2u)  /* 0xFFEA Timer0_A CC1-2, TA0 */
#define TIMER0_A0_VECTOR    (6 * 2u)  /* 0xFFEC Timer0_A CC0 */
#define SD24_VECTOR         (7 * 2u)  /* 0xFFEE Sigma Delta ADC */
#define USCI_B0_VECTOR      (8 * 2u)  /* 0xFFF0 USCI B0 Receive/Transmit */
#define USCI_A0_VECTOR      (9 * 2u)  /* 0xFFF2 USCI A0 Receive/Transmit */
#define WDT_VECTOR          (10 * 2u) /* 0xFFF4 Watchdog Timer */
#define VMON_VECTOR         (11 * 2u) /* 0xFFF6 Voltage Monitor */
#define TIMER1_A1_VECTOR    (12 * 2u) /* 0xFFF8 Timer1_A CC1-4, TA1 */
#define TIMER1_A0_VECTOR    (13 * 2u) /* 0xFFFA Timer1_A CC0 */
#define NMI_VECTOR          (14 * 2u) /* 0xFFFC Non-maskable */
#define RESET_VECTOR        (15 * 2u) /* 0xFFFE Reset [Highest Priority] */

/************************************************************
* End of Modules
************************************************************/
#pragma language=default

#endif /* #ifndef __MSP430i2020 */
