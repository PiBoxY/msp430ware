; --COPYRIGHT--,FRAM-Utilities
;  Copyright (c) 2015, Texas Instruments Incorporated
;  All rights reserved.
; 
;  This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
;  Visit http://www.ti.com/tool/msp-fram-utilities for software information and
;  download.
; --/COPYRIGHT--
    .cdecls C,LIST,"msp430.h"               ; Include device header file

    .cdecls C,LIST,"ctpl_benchmark.h"       ; CTPL benchmark file
    .cdecls C,LIST,"ctpl_low_level.h"       ; CTPL header file

    .include "ctpl_low_level_macros.asm"

; Define RAM address
    .if $defined(__MSP430FR2XX_4XX_FAMILY__)
CTPL_RAM_START  .set    2000h
    .else
CTPL_RAM_START  .set    1c00h
    .endif

; State keys
CTPL_STATE_VALID        .set    0xa596
CTPL_STATE_INVALID      .set    0x0000

; FRAM stack copy
ctpl_stackCopy      .usect ".TI.persistent",CTPL_STACK_SIZE,2

; RAM copy
    .if $defined(CTPL_RAM_SIZE)
ctpl_ramCopy        .usect ".TI.persistent",CTPL_RAM_SIZE,2
    .endif

; Low level state variables
ctpl_mode           .usect ".TI.persistent",2,2
ctpl_state          .usect ".TI.persistent",2,2
ctpl_stackUsage     .usect ".TI.persistent",2,2

; Global symbols
    .global __STACK_END

    .sect ".text:ctpl_low_level"

; Declare functions globally
    .global ctpl_init
    .global ctpl_saveCpuStackEnterLpm
    .global ctpl_unlockFRAM
    .global ctpl_restoreFRAM

ctpl_init:
    unlockFRAM                                  ; Unlock FRAM (FR2xx and FR4xx only)
    cmpx.w  #CTPL_STATE_VALID,&ctpl_state       ; Valid ctpl state?
    jne     ctpl_initReturn                     ; No, return
    movx.w  &ctpl_mode,R12                      ; Move ctpl mode to local
    and.b   #CTPL_MODE_BITS,R12                 ; Mask ctpl mode bits
    cmp.b   #CTPL_MODE_SHUTDOWN,R12             ; Shutdown mode?
    jz      ctpl_wakeup                         ; Yes, jump to wakeup (always restore)
    bit.w   #PMMRSTIFG, &PMMIFG                 ; Was there a RST/NMI?
    jnz     ctpl_initResetPowerup               ; Yes, jump to reset/powerup routine
ctpl_initNonReset:
    bit.w   #PMMLPM5IFG, &PMMIFG                ; Was reset due to LPMx.5 wakeup?
    jz      ctpl_initResetPowerup               ; No, jump to reset/powerup routine
    bisx.w  #CTPL_MODE_LPMX5_WAKEUP,&ctpl_mode  ; Yes, set the LPMx.5 wakeup status flag
    jmp     ctpl_wakeup                         ; Jump to wakeup
ctpl_initResetPowerup:
    bitx.w  #CTPL_MODE_RESTORE_RESET,&ctpl_mode ; Allow wakeup from reset/powerup?
    jnz     ctpl_wakeup                         ; Yes, jump to wakeup
ctpl_initReturn:
    movx.w  #CTPL_MODE_NONE,&ctpl_mode          ; Reset the mode to none
    movx.w  #CTPL_STATE_INVALID,&ctpl_state     ; Mark the state as invalid
    restoreFRAM                                 ; Restore FRAM state (FR2xx and FR4xx only)
    retx                                        ; Return

ctpl_saveCpuStackEnterLpm:
    pushx.a SR                                  ; Save SR to stack
    dint                                        ; disable interrupts
    nop                                         ; disable interrupts
    unlockFRAM                                  ; Unlock FRAM (FR2xx and FR4xx only)
    movx.w  R12,&ctpl_mode                      ; Save CTPL mode
    and.b   #CTPL_MODE_BITS,R12                 ; Mask ctpl mode bits
    cmp.b   #CTPL_MODE_NONE,R12                 ; None mode?
    jz      ctpl_return                         ; Yes, return to function
    pushm.a #8,R11                              ; Save R4-R11 to stack
    movx.w  #__STACK_END,R4                     ; Calculate stack usage
    subx.a  SP,R4                               ; Calculate stack usage
    movx.w  R4,&ctpl_stackUsage                 ; Save stack usage
    movx.a  #ctpl_stackCopy,R6                  ; dest ptr
    movx.a  SP,R5                               ; src ptr
    copyx   R5,R6,R4                            ; copy the stack
    .if $defined(CTPL_RAM_SIZE)
    movx.w  #CTPL_RAM_START,R5
    movx.a  #ctpl_ramCopy,R6
    movx.w  #CTPL_RAM_SIZE,R4
    copyx   R5,R6,R4                            ; copy the RAM contents
    .endif
ctpl_setStateValid:
    movx.w  #CTPL_STATE_VALID,&ctpl_state       ; Mark the state as valid
    restoreFRAM                                 ; Restore FRAM state (FR2xx and FR4xx only)
    cmp.b   #CTPL_MODE_SHUTDOWN,R12             ; Check for shutdown mode and disable SVSH
    jnz     ctpl_enterLpm                       ; No, jump to ctpl_enterLpm
ctpl_enterShutdownWithTimeout:
    configureDcoShutdown R13                    ; Reconfigure DCO for shutdown
    bic.w   #255,R13                            ; Clear lower bytes
    swpb    R13                                 ; Swap bytes
    add.w   #WDTPW+WDTCNTCL,R13                 ; Set WDT timeout
    mov.w   R13,&WDTCTL                         ; Set WDT timeout
    mov.b   #PMMPW_H,&PMMCTL0_H                 ; open PMM
    bis.b   #SVSHE,&PMMCTL0_L                   ; enable SVSH
    mov.b   #0,&PMMCTL0_H                       ; close PMM
ctpl_shutdownWaitForSvs:
    benchmark                                   ; Toggle the CTPL benchmark pin
    jmp     ctpl_shutdownWaitForSvs             ; Wait for SVSH to put device into BOR
ctpl_enterLpm:
    benchmark                                   ; Toggle the CTPL benchmark pin
    lpmDebug                                    ; Optional LPMx.5 debug mode
    mov.b   #PMMPW_H,&PMMCTL0_H                 ; Set LPMx.5 bit
    mov.b   #PMMREGOFF,&PMMCTL0_L               ; Set LPMx.5 bit
    bis.w   #LPM4,SR                            ; Enter LPMx.5 mode
    nop                                         ; nop
ctpl_wakeup:
    mov.w   #WDTPW+WDTHOLD,&WDTCTL              ; stop WDT
    configureDcoWakeup                          ; Reconfigure DCO for wakeup
    .if $defined(CTPL_RAM_SIZE)
    movx.w  #CTPL_RAM_START,R6
    movx.a  #ctpl_ramCopy,R5
    movx.w  #CTPL_RAM_SIZE,R4
    copyx   R5,R6,R4                            ; copy the RAM contents
    .endif
    movx.a  #__STACK_END,SP                     ; Reset stack pointer
    movx.w  &ctpl_stackUsage,R4                 ; loop counter
    subx.a  R4,SP                               ; Reset stack pointer
    movx.a  SP,R6                               ; dest ptr
    movx.a  #ctpl_stackCopy,R5                  ; src ptr
    copyx   R5,R6,R4                            ; copy the stack
    popm.a  #8,R11                              ; Restore R4-R11 from stack
    movx.w  #CTPL_STATE_INVALID,&ctpl_state     ; Mark the state as invalid
ctpl_return:
    movx.w  &ctpl_mode,R12                      ; Return CTPL mode
    restoreFRAM                                 ; Restore FRAM state (FR2xx and FR4xx only)
    popx.a  R13                                 ; Restore interrupts
    nop                                         ; Required NOP
    movx.a  R13,SR                              ; Restore interrupts
    nop                                         ; Required NOP
    retx                                        ; Return
    
