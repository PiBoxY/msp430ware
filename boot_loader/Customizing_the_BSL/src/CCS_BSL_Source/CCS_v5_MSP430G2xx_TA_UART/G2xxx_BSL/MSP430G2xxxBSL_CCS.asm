;*******************************************************************************
;   G2xxx BSL is a Boot Strap Loader for MSP430G2xxx based on software UART with 
;      small Flash Footprint so that it fits into info memory (256 - 2 Byte)
;
;                MSP430G2xxx
;             -----------------
;         /|\|                 |
;          | |        P1.0/P1.6|---> LED (only used in MAIN not in BSL)
;          --|RST              |
;            |                 |
;            |      BSLPIN/P1.3|<--- Enable Pin / S2 on LaunchPad
;            |                 |
;            |                 |
;            |          rX/P1.1|<--- UART 9600 8N1
;            |          tX/P1.2|---> rx/tx swapped on LaunchPad!
;             -----------------
;
;
;   Texas Instruments
;   Built with CCS 4.2.3
;
;*******************************************************************************
;  Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/ 
; 
;  Redistribution and use in source and binary forms, with or without 
;  modification, are permitted provided that the following conditions 
;  are met:
;
;    Redistributions of source code must retain the above copyright 
;    notice, this list of conditions and the following disclaimer.
;
;    Redistributions in binary form must reproduce the above copyright
;    notice, this list of conditions and the following disclaimer in the 
;    documentation and/or other materials provided with the   
;    distribution.
;
;    Neither the name of Texas Instruments Incorporated nor the names of
;    its contributors may be used to endorse or promote products derived
;    from this software without specific prior written permission.
;
;  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
;  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
;  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
;  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
;  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
;  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
;  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
;  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
;  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
;  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
;  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;

;*******************************************************************************
            .cdecls C,LIST,"msp430.h"       ; Include device header file

;-------------------------------------------------------------------------------
            ;Main / Customer Application
;-------------------------------------------------------------------------------
LED         .equ    BIT0                    ; RED LED
;LED        .equ    BIT6                    ; GREEN LED
;-------------------------------------------------------------------------------
            .text                           ; User Application
            ; ================================================================
            ; Never place any code / data between .text (RSEG CODE) and main
            ; label. This adress is used as start adress for user code
            ; ================================================================
;-------------------------------------------------------------------------------
            ; Customer Application starts here
main        nop                             ; User Code needs to start here!
	    bis.b   #LED,&P1DIR             ; Make pin an output
            mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop Watchdog Timer

Mainloop    xor.b   #LED,&P1OUT             ; Toggle LED
            mov.w   #0FFFFh,R15             ; Delay to R15
DelayL1     dec.w   R15                     ; Decrement R15
            ; ================================================================
            ; Note that any Register and RAM can be used
            ; ================================================================
            inc.w   R15                     ; Increment R15
            dec.w   R15                     ; Decrement R15
            jnz     DelayL1                 ; Delay over?
            jmp     Mainloop                ; Again
;-------------------------------------------------------------------------------
            ;END of Main / Customer Application
;-------------------------------------------------------------------------------



;-------------------------------------------------------------------------------
            .sect   ".bsl"        ;BSL Application
;-------------------------------------------------------------------------------

;           Flash Main Segment Start Adress (see datasheet)
FLASHSTART  .equ    main                    ; Lowest main flash address

;           CPU registers used for BSL
RXTXData    .equ    R4                    
BitCnt      .equ    R5
rxData      .equ    R6
rCHKSUM     .equ    R7
rPoint      .equ    R8
rTemp       .equ    R9

;           Conditions for 9600 Baud HW/SW UART, MCLK 1 MHz
;           1 MHz / 9600 = 104.15 Cycles per bit
BITTIME_5   .equ    52                      ; 0.5 bit length 
BITTIME     .equ    104                     ; 104 us
NACKCYCL    .equ    BITTIME*2               ; 2 bits low: start + 1 data bits
                                            ;  0 + (1111 1110)b = 0xFE
ACKCYCL     .equ    BITTIME*4               ; 4 bits low: start + 3 data bits
                                            ;  0 + (1111 1000)b = 0xF8
                                            ; Meas: Puls 2us too long but no   
                                            ;  impact at 104us Bittime
RXD         .equ    02h                     ; RXD on P1.1
TXD         .equ    04h                     ; TXD on P1.2
BSLPIN      .equ    BIT3                    ; BSL entry on P1.3 LOW (Use pullup) 

;           Command number definition
CMD_SYNC    .equ    0BAh
            
;-------------------------------------------------------------------------------
;           Jump table for UART Bit detection
;-------------------------------------------------------------------------------
RX_Count    .byte   RX_Edge-RX_Edge         ; Special for TA
            .byte   RX_Bit-RX_Edge          ; RX First Data Bit
            .byte   RX_Bit-RX_Edge          ;
            .byte   RX_Bit-RX_Edge          ;
            .byte   RX_Bit-RX_Edge          ;
            .byte   RX_Bit-RX_Edge          ;
            .byte   RX_Bit-RX_Edge          ;
            .byte   RX_Bit-RX_Edge          ;
            .byte   RX_Bit-RX_Edge          ;
            .byte   RX_Comp-RX_Edge         ; RX Complete, process RX data


;-------------------------------------------------------------------------------
;           Start of BSL Code
;-------------------------------------------------------------------------------
BeginOfInfo jmp     $                       ; Safety: Avoid code run through

RESET
            bit.b   #BSLPIN,&P1IN           ; Pin low invokes BSL
BslEntry?   jz      InvokeBsl
            br      #main                   ; Exit BSL if pin not set

;-------------------------------------------------------------------------------
;           BSL Invoked
;-------------------------------------------------------------------------------
InvokeBsl   mov.w   #280h,SP                ; Init Stackpointer to top of RAM
                                            ; RAM: 0x27f - 0x200

UnlockFlash mov.w   #FWKEY+00h,&FCTL3       ; LOCK=0, all others 0

StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop Watchdog Timer

SetupDCO    ; Set DCO to calibrated 1 MHz:
            clr.b   &DCOCTL                 ; Lowest DCOx and MODx settings

            ; ================================================================
            ; Note that the user needs to ensure that DCO Cal Data is not
            ;  erased during debugging - Read out from InfoA and hardcode
            ;  two bytes in move commands below.
            mov.b   #0b3h,&DCOCTL    ; Copy from address 0x010FEh
            mov.b   #0b3h,&BCSCTL1   ; Copy from address 0x010FFh
            ; Replace 0b3h with values gathered from actual
            ;  device. Values look like that: 0b3h and 086h
            ;  (CCS Debug -> Debugger -> Loading options: Load symbols only)    
            ; ================================================================

            ; ================================================================
            ; For Mass production please enable cal data readout directly from
            ;  InfoA. Empty devices should be programmed without erasing flash
            ;  before. If erased first, CAL data is lost. But can be restored
            ;  with FlashPro430 / GangPro430 programmers. www.elprotronic.com
            ;mov.b   &CALDCO_1MHZ,&DCOCTL   ; Set DCO step + modulation
            ;mov.b   &CALBC1_1MHZ,&BCSCTL1  ; Set range
            ; ================================================================

;OutputSMCLK bis.b   #BIT4,&P1DIR           ; Output SMCLK to check Frequency
;            bis.b   #BIT4,&P1SEL

SetupPins   bis.b   #RXD,&P1SEL             ; Rx pin special function
            bis.b   #TXD,&P1DIR
            bis.b   #TXD,&P1OUT

SetupTA0    ;CC Input0: Capture on falling Edge
            mov.w   #CM_2+CCIS_0+SCS+CAP,&TACCTL0
            ;Timer in Continuous mode, Clock Source is SMCLK
            bis.w   #TASSEL_2+MC_2+TACLR,&TACTL

InitRx      mov.w   #RX_Count,BitCnt        ; UART Bit Branch Pointer

;-------------------------------------------------------------------------------
MainBsl             ; BSL Main Loop
;-------------------------------------------------------------------------------
Wait4sync   call    #RxOneByte
            
SyncCmd?    cmp     #CMD_SYNC,rxData        ; Sync command received?
            jne     BSLEnd                  ; Exit BSL if no Sync Byte 
            
;-------------------------------------------------------------------------------
CmdFct_Erase        ; Erase main flash and restore interrupt vectors
;-------------------------------------------------------------------------------
            ; erase from bottom up
            mov.w   #FLASHSTART, rPoint     ; FLASH start address
EraseSeg    mov     &FWKEY+ERASE,&FCTL1     ; ERASE=1. Erase Segment
            
            ; Start praying here that nothing happens during next CPU cycles
            mov.b   #0h,0(rPoint)           ; Start erase with dummy write
            cmp.w   #0FE00h, rPoint         ; Interrupt vector segment just erased?
            jge     WrtRstVec               ; Restore it!
            add.w   #512,rPoint             ; Next segment (add segment size)
            jmp     EraseSeg                ; Repeat segment erase
            
WrtRstVec   mov     #FWKEY+WRT,&FCTL1       ; WRT=1. Write to segment            
            mov     #RESET,&0FFFEh          ; Point reset vector to BSL
            ; You can stop praying here. Everything went fine once PC is here
            
            ; ================================================================
            ; Erasing Flash and rewriting vectors take approx 16 ms
            ;  Make sure to wait this time before new data is sent to MSP430
            ;  The UART function is placed in flash thus preventing access 
            ;  to it while flash erase / write.
            ; ================================================================
           
;------------------------------------------------------------------------------- 
CmdFct_Write        ; Write 512 - 2 Byte to Main memory
;------------------------------------------------------------------------------- 
            ;mov    #FWKEY+WRT,&FCTL1       ; Writing is still enabled 
            ; Keeping WRT enabled w/o writing does not account into write time
            
            mov     #FLASHSTART,rPoint      ; Point to first position in FLASH
            clr     rCHKSUM                 ; Init Checksum
            
            ; ================================================================
            ; Ensure a minimum delay of 16 ms between SYNC and first byte
            ; ================================================================            
              
CFW_w4d     call    #RxOneByte
            
CFW_Range   cmp     #0FFFEh, rPoint         ; Pointer to Reset Vector?
            jeq     CFW_Done                ; Skip and exit Write
            mov.b   rxData,0(rPoint)        ; Write 8 bit data to flash

CFW_COMM
CFW_Xor     xor.b   @rPoint+,rCHKSUM        ; xor checksum and inc pointer
            ;inc    rPoint                  ; inc done above w/ Auto increment
            jmp     CFW_w4d                 ; wait for next byte
            
CFW_Done    ; ================================================================
            ; rx'ed byte for adress 0xffe (RESET) contains checksum
            ; ================================================================  
            cmp.b   rxData, rCHKSUM         ; XOR'ing calc checksum with sent      
            jeq     LoadACK                 ;  should result in ZERO if match
            
LoadNACK    mov.w   #(NACKCYCL/3),rTemp     ; Delay cycles to rTemp
            jmp     SendACK
LoadACK     mov.w   #(ACKCYCL/3),rTemp      ; /3 because 3 CPU cycles per loop
                                            ;  count required
            
SendACK     bic.b   #TXD,&P1OUT             ; Let PC know about success
DelayACK    dec.w   rTemp                   ; Decrement rTemp
            jnz     DelayACK                ; Delay over?
            bis.b   #TXD,&P1OUT
            
            ; ================================================================
            ; Reset sequence required to exit BSL
            ; Do not jmp to main here - system is not in its initial state!
            ; ================================================================
BSLEnd      ;jmp     MainBsl                ; Start BSL over again
            clr     WDTCTL                  ; Exit BSL by issuing RESET
            ; ================================================================
            ; Reset sequence required to exit BSL
            ; Do not jmp to main here - system is not in its initial state!
            ; ================================================================

;-------------------------------------------------------------------------------
RxOneByte   ; CCR0/UART ISR, RXTXData Buffer holds UART Data
;-------------------------------------------------------------------------------
Wait4CCIFG  bit.w   #CCIFG, &TACCTL0        ; Test CCIFG Bit
            jz      Wait4CCIFG

            add.w   #BITTIME,&TACCR0        ; Bit time till next bit
            bic.w   #CCIFG, &TACCTL0        ; Clear IFG

            mov.b   @BitCnt+,rTemp          ; Branch To Routine
            add     rTemp,PC                ; Branch To Routine
                                            
RX_Edge     bic.w   #CAP+CCIFG,&TACCTL0     ; Switch to Compare mode
            add.w   #BITTIME_5,&TACCR0      ; First Databit 1.5 Bits from edge
            jmp     Wait4CCIFG              ;

RX_Bit      bit.w   #SCCI,&TACCTL0          ; Get bit waiting in SCCI
            rrc.b   RXTXData                ; Store received bit
            jmp     Wait4CCIFG              ;

RX_Comp     ; Byte received. Store and enable next rx cycle
            bis.w   #CAP,&TACCTL0           ; Switch to Capture mode
            mov.w   #RX_Count,BitCnt        ; RX_Count --> Branch Pointer
            mov.b   RXTXData,rxData         ; Copy from Bit to byte buffer
RxDone      ret                             ;

                                                                                       
;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------

            .sect   ".reset"                ; RESET Vector
            .short  RESET                   ; POR, ext. Reset
 
            .end
