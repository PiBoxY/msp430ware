; --COPYRIGHT--,FRAM-Utilities
;  Copyright (c) 2015, Texas Instruments Incorporated
;  All rights reserved.
; 
;  This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
;  Visit http://www.ti.com/tool/msp-fram-utilities for software information and
;  download.
; --/COPYRIGHT--
; Call statement for different code models
callx   .macro  src
    .if $defined(__LARGE_CODE_MODEL__)
        calla   src
    .else
        call    src
    .endif
        .endm

; Return statement for different code models
retx    .macro
    .if $defined(__LARGE_CODE_MODEL__)
        reta
    .else
        ret
    .endif
        .endm

; Define DMA if using DMA3
    .if $defined(__MSP430_HAS_DMAX_3__)
__MSP430_HAS_DMA__  .set    1
    .endif

; Macro for performing copy with either CPU or DMA
copyx   .macro    src, dst, len
    .if $defined(__MSP430_HAS_DMA__)
        clr.b   &DMA0CTL_L      ; sw trigger, channel 0
        movx.a  src,&DMA0SA     ; set src address
        movx.a  dst,&DMA0DA     ; set dst address
        rra.w   len             ; divide length by 2
        mov.w   len, &DMA0SZ    ; set copy size
        mov.w   #DMASWDW+DMADT_1+DMASRCINCR_3+DMADSTINCR_3+DMAEN+DMAREQ,&DMA0CTL    ; trigger DMA copy
    .else
ctpl_copyLoop?:
        movx.w  @src+, 0(dst)   ; copy stack word and increment src ptr
        addx.a  #2,dst          ; increment dst ptr
        subx.a  #2,len          ; decrement stack usage
        jnz     ctpl_copyLoop?  ; loop if usage > 0
    .endif
        .endm

; Macro for performing fill with either CPU or DMA
fillx   .macro    dst, len, val
    .if $defined(__MSP430_HAS_DMA__)
        movx.w  val,0(dst)      ; fill first value
        clr.b   &DMA0CTL_L      ; sw trigger, channel 0
        movx.a  dst,&DMA0SA     ; set src address
        movx.a  dst,&DMA0DA     ; set dst address
        rra.w   len             ; divide length by 2
        mov.w   len, &DMA0SZ    ; set fill size
        mov.w   #DMASWDW+DMADT_1+DMASRCINCR_0+DMADSTINCR_3+DMAEN+DMAREQ,&DMA0CTL    ; trigger DMA fill
    .else
ctpl_fillLoop?:
        movx.w  val,0(dst)      ; fill dst
        addx.a  #2,dst          ; increment dst ptr
        subx.a  #2,len          ; decrement stack usage
        jnz     ctpl_fillLoop?  ; loop if usage > 0
    .endif
        .endm

; Macro for configuring DCO for shutdown
configureDcoShutdown    .macro  div
    .if $defined(__MSP430FR2XX_4XX_FAMILY__)
        bic.w   #SCG0,SR                            ; disable FLL
        mov.w   #DIVM__32+DIVS__1,&CSCTL3           ; Set maximum dividers
        mov.w   #DCORSEL_3,&CSCTL1                  ; Set DCO 8MHz
        clr.w   &CSCTL4                             ; Source MCLK and SMCLK from DCO
        mov.b   div,&CSCTL5_L                       ; Set timeout dividers
    .elseif $defined(__MSP430FR57XX_FAMILY__)
        mov.b   #CSKEY_H,&CSCTL0_H                  ; Unlock CS registers
        mov.w   #DIVM__32+DIVS__32,&CSCTL3          ; Set maximum dividers
        mov.w   #DCOFSEL_3,&CSCTL1                  ; Set DCO 8MHz
        mov.w   #SELM_3+SELS_3,&CSCTL2              ; Source MCLK and SMCLK from DCO
        mov.b   div,&CSCTL3_L                       ; Set timeout dividers
        clr.b   &CSCTL0_H                           ; Lock CS registers
    .elseif $defined(__MSP430FR5XX_6XX_FAMILY__)
        mov.b   #CSKEY_H,&CSCTL0_H                  ; Unlock CS registers
        mov.w   #DIVM__32+DIVS__32,&CSCTL3          ; Set maximum dividers
        mov.w   #DCOFSEL_6,&CSCTL1                  ; Set DCO 8MHz
        mov.w   #SELM_3+SELS_3,&CSCTL2              ; Source MCLK and SMCLK from DCO
        mov.b   div,&CSCTL3_L                       ; Set timeout dividers
        clr.b   &CSCTL0_H                           ; Lock CS registers
    .endif
                    .endm

; Macro for configuring DCO for wakeup
configureDcoWakeup  .macro
    .if $defined(__MSP430FR2XX_4XX_FAMILY__)
        bic.w   #SCG0,SR                            ; disable FLL
        mov.w   #DCORSEL_2,&CSCTL1                  ; set DCO to 4MHz (maximum boot freq)
    .elseif $defined(__MSP430FR57XX_FAMILY__)
        mov.b   #CSKEY_H,&CSCTL0_H                  ; unlock CS registers
        mov.w   #DIVM__2,&CSCTL3                    ; set DCO to 4MHz (maximum boot freq)
        clr.b   &CSCTL0_H                           ; lock CS registers
    .elseif $defined(__MSP430FR5XX_6XX_FAMILY__)
        mov.b   #CSKEY_H,&CSCTL0_H                  ; unlock CS registers
        mov.w   #DIVM__2,&CSCTL3                    ; set DCO to 4MHz (maximum boot freq)
        clr.b   &CSCTL0_H                           ; lock CS registers
    .endif
                    .endm

; Macro for toggling the CTPL benchmark pin.
benchmark   .macro
    .if $defined(CTPL_BENCHMARK)
        xor.b   #CTPL_BENCHMARK_PIN,&CTPL_BENCHMARK_OUT
    .endif
            .endm

; Define RTCIFG if not defined (FR2XX and FR4XX)
    .if $defined(__MSP430FR2XX_4XX_FAMILY__)
    .if !$defined(RTCIFG)
        .asg    RTCIF,RTCIFG
    .endif
    .endif

; Macro to check for GPIO interrupt event
lpmDebug_port   .macro  PXIE, PXIFG
        mov.w   &PXIE,R4                ; Get enabled interrupts
        and.w   &PXIFG,R4               ; Check set interrupt flags
        tst.w   R4                      ; Test if any interrupts were triggered
        jne     lpmDebug_return         ; Jump to return function
                .endm

; Macro to check for RTC interrupt event
lpmDebug_rtc    .macro
        mov.b   &RTCCTL,R4              ; Load RTC control register
        and.b   #(RTCIE+RTCIFG),R4      ; Mask bits
        cmp.b   #(RTCIE+RTCIFG),R4      ; Check RTC IE and IFG are both set
        jeq     lpmDebug_return         ; Jump to return function
                .endm

; Macro to check for RTC_B interrupt event
lpmDebug_rtc_b  .macro
        mov.b   &RTCCTL01_L,R4          ; Load RTC_B control 01 register
        rrum.w  #4,R4                   ; Shift right
        bit.b   R4,&RTCCTL01_L          ; Mask IFG bits
        jne     lpmDebug_return         ; Jump to return function
        mov.b   &RTCPS0CTL_L,R4         ; Load RTC_B prescale 0 register
        and.b   #(RT0PSIE+RT0PSIFG),R4  ; Mask bits
        cmp.b   #(RT0PSIE+RT0PSIFG),R4  ; Check RTC_B IE and IFG are both set
        jeq     lpmDebug_return         ; Jump to return function
        mov.b   &RTCPS1CTL_L,R4         ; Load RTC_B prescale 1 register
        and.b   #(RT1PSIE+RT1PSIFG),R4  ; Mask bits
        cmp.b   #(RT1PSIE+RT1PSIFG),R4  ; Check RTC_B IE and IFG are both set
        jeq     lpmDebug_return         ; Jump to return function
                .endm

; Macro to check for RTC_C interrupt event
lpmDebug_rtc_c  .macro
        mov.b   &RTCCTL0_L,R4           ; Load RTC_C control 0 register
        rrum.w  #4,R4                   ; Shift right
        bit.b   R4,&RTCCTL0_L           ; Mask IFG bits
        jne     lpmDebug_return         ; Jump to return function
        mov.b   &RTCPS0CTL_L,R4         ; Load RTC_C prescale 0 register
        and.b   #(RT0PSIE+RT0PSIFG),R4  ; Mask bits
        cmp.b   #(RT0PSIE+RT0PSIFG),R4  ; Check RTC_C IE and IFG are both set
        jeq     lpmDebug_return         ; Jump to return function
        mov.b   &RTCPS1CTL_L,R4         ; Load RTC_C prescale 1 register
        and.b   #(RT1PSIE+RT1PSIFG),R4  ; Mask bits
        cmp.b   #(RT1PSIE+RT1PSIFG),R4  ; Check RTC_C IE and IFG are both set
        jeq     lpmDebug_return         ; Jump to return function
                .endm

; Allow debugging in LPMx.5 by emulating wakeup
lpmDebug    .macro
    .if $defined(CTPL_LPM_DEBUG)
        movx.a  #CTPL_RAM_START,R4                  ; fill dst
        movx.a  #(__STACK_END-CTPL_RAM_START),R5    ; fill length
        movx.a  #0xffff,R6                          ; fill value
        fillx   R4,R5,R6                            ; fill RAM contents with 0xffff
lpmDebug_top:
    .if $defined(PAIFG)
        lpmDebug_port   PAIE,PAIFG  ; Check PORTA IFG's
    .endif
    .if $defined(PBIFG)
        lpmDebug_port   PBIE,PBIFG  ; Check PORTB IFG's
    .endif
    .if $defined(PCIFG)
        lpmDebug_port   PCIE,PCIFG  ; Check PORTC IFG's
    .endif
    .if $defined(PDIFG)
        lpmDebug_port   PDIE,PDIFG  ; Check PORTD IFG's
    .endif
    .if $defined(PEIFG)
        lpmDebug_port   PEIE,PEIFG  ; Check PORTE IFG's
    .endif
    .if $defined(PFIFG)
        lpmDebug_port   PFIE,PFIFG  ; Check PORTF IFG's
    .endif
    .if $defined(__MSP430_HAS_RTC__)
        lpmDebug_rtc                ; Check RTC IFG's
    .endif
    .if $defined(__MSP430_HAS_RTC_B__)
        lpmDebug_rtc_b              ; Check RTC_B IFG's
    .endif
    .if $defined(__MSP430_HAS_RTC_C__)
        lpmDebug_rtc_c              ; Check RTC_C IFG's
    .endif
        jmp     lpmDebug_top        ; Jump to top
lpmDebug_return:
        mov.b   #PMMPW_H,&PMMCTL0_H ; Unlock PMM registers
        mov.w   #PMMLPM5IFG,&PMMIFG ; Set LPMx.5 wakeup flag
        mov.b   #0,&PMMCTL0_H       ; Lock PMM registers
        mov.w   #0x0FFFE,R4     ; Reset vector
        mov.w   @R4,PC              ; Jump to reset vector
    .endif
            .endm

    .if $defined(__MSP430FR2XX_4XX_FAMILY__)

; Define FRWPPW if not defined (FR2XX and FR4XX)
    .if !$defined(FRWPPW)
FRWPPW  .set    0
    .endif

ctpl_stateFRAM  .usect ".TI.persistent",2,2

; Macro for unlocking FRAM.
unlockFRAM  .macro
        mov.w   &SYSCFG0,R15                        ; Save FRAM state
        and.w   #(BIT1+BIT0),R15                    ; Mask out other bits
        mov.w   #FRWPPW,&SYSCFG0                    ; Unlock FRAM
        mov.w   R15,&ctpl_stateFRAM                 ; Save state to FRAM
            .endm

; Macro for restoring FRAM state.
restoreFRAM .macro
        mov.w   &ctpl_stateFRAM,R15                 ; Restore state fram FRAM
        bis.w   #FRWPPW,R15                         ; Set FRAM password bits
        mov.w   R15,&SYSCFG0                        ; Restore FRAM state
            .endm

    .else
; Macro for unlocking FRAM.
unlockFRAM  .macro
            .endm
; Macro for restoring FRAM state.
restoreFRAM .macro
            .endm
    .endif
