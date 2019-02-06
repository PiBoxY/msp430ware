;*******************************************************************************
;
;   FFT_430.asm
;
;   Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
;
;   Redistribution and use in source and binary forms, with or without
;   modification, are permitted provided that the following conditions
;   are met:
;
;   Redistributions of source code must retain the above copyright
;   notice, this list of conditions and the following disclaimer.

;     Redistributions in binary form must reproduce the above copyright
;     notice, this list of conditions and the following disclaimer in the
;     documentation and/or other materials provided with the
;     distribution.
;
;     Neither the name of Texas Instruments Incorporated nor the names of
;     its contributors may be used to endorse or promote products derived
;     from this software without specific prior written permission.
;
;   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
;   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
;   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
;   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
;   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
;   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
;   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
;   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
;   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
;   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
;   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
;*******************************************************************************
 
 .cdecls C,LIST,  "msp430.h"

            .def    FFT_prog
            .text   
            .global FBuff
            .global voice_data

     
            .bss    FStrt,2                 ;start address in block
            .bss    FGRPsiz,2               ;group size
            .bss    FGRPC,2                 ;group count

VecSiz      .equ    256
I_Vec       .equ    voice_data
BitRT       .equ    BitRT_256
Twidd       .equ    Twidd_256


;********************************************************************
; Name     :BitRT_256
; Function :Bit reversed table for FFT input output vector shuffling
; Generator:MSP430/TMS430 FFT explorer V.1.1 (IAR Convention)
; Date     :05-14-2005   16:38:05
;********************************************************************
BitRT_256:  .char   000H,080H,040H,0C0H,020H,0A0H,060H,0E0H
            .char   010H,090H,050H,0D0H,030H,0B0H,070H,0F0H
            .char   008H,088H,048H,0C8H,028H,0A8H,068H,0E8H
            .char   018H,098H,058H,0D8H,038H,0B8H,078H,0F8H
            .char   004H,084H,044H,0C4H,024H,0A4H,064H,0E4H
            .char   014H,094H,054H,0D4H,034H,0B4H,074H,0F4H
            .char   00CH,08CH,04CH,0CCH,02CH,0ACH,06CH,0ECH
            .char   01CH,09CH,05CH,0DCH,03CH,0BCH,07CH,0FCH
            .char   002H,082H,042H,0C2H,022H,0A2H,062H,0E2H
            .char   012H,092H,052H,0D2H,032H,0B2H,072H,0F2H
            .char   00AH,08AH,04AH,0CAH,02AH,0AAH,06AH,0EAH
            .char   01AH,09AH,05AH,0DAH,03AH,0BAH,07AH,0FAH
            .char   006H,086H,046H,0C6H,026H,0A6H,066H,0E6H
            .char   016H,096H,056H,0D6H,036H,0B6H,076H,0F6H
            .char   00EH,08EH,04EH,0CEH,02EH,0AEH,06EH,0EEH
            .char   01EH,09EH,05EH,0DEH,03EH,0BEH,07EH,0FEH
            .char   001H,081H,041H,0C1H,021H,0A1H,061H,0E1H
            .char   011H,091H,051H,0D1H,031H,0B1H,071H,0F1H
            .char   009H,089H,049H,0C9H,029H,0A9H,069H,0E9H
            .char   019H,099H,059H,0D9H,039H,0B9H,079H,0F9H
            .char   005H,085H,045H,0C5H,025H,0A5H,065H,0E5H
            .char   015H,095H,055H,0D5H,035H,0B5H,075H,0F5H
            .char   00DH,08DH,04DH,0CDH,02DH,0ADH,06DH,0EDH
            .char   01DH,09DH,05DH,0DDH,03DH,0BDH,07DH,0FDH
            .char   003H,083H,043H,0C3H,023H,0A3H,063H,0E3H
            .char   013H,093H,053H,0D3H,033H,0B3H,073H,0F3H
            .char   00BH,08BH,04BH,0CBH,02BH,0ABH,06BH,0EBH
            .char   01BH,09BH,05BH,0DBH,03BH,0BBH,07BH,0FBH
            .char   007H,087H,047H,0C7H,027H,0A7H,067H,0E7H
            .char   017H,097H,057H,0D7H,037H,0B7H,077H,0F7H
            .char   00FH,08FH,04FH,0CFH,02FH,0AFH,06FH,0EFH
            .char   01FH,09FH,05FH,0DFH,03FH,0BFH,07FH,0FFH

;********************************************************************
; Name     :Twidd_256
; Function :Twiddle Factors for FFTs W0..W128 rotating clockwise
;          :MaxValue= 32767 (tables kept symmetric arround zero
;          :COS(0) {real} starting with offset 0
;          :-SIN(0){imag} starting with offset 64
; Generator:MSP430/TMS430 FFT explorer V.1.1 (IAR Convention)
; Date     :05-14-2005   16:38:06
;********************************************************************
Twidd_256:  .short   07FFFH,07FF5H,07FD7H,07FA6H,07F61H,07F08H,07E9CH,07E1CH
            .short   07D89H,07CE2H,07C29H,07B5CH,07A7CH,07989H,07883H,0776BH
            .short   07640H,07503H,073B5H,07254H,070E1H,06F5EH,06DC9H,06C23H
            .short   06A6CH,068A5H,066CEH,064E7H,062F1H,060EBH,05ED6H,05CB3H
            .short   05A81H,05842H,055F4H,0539AH,05133H,04EBFH,04C3FH,049B3H
            .short   0471CH,0447AH,041CDH,03F16H,03C56H,0398CH,036B9H,033DEH
            .short   030FBH,02E10H,02B1EH,02826H,02527H,02223H,01F19H,01C0BH
            .short   018F8H,015E1H,012C7H,00FABH,00C8BH,0096AH,00647H,00324H
            .short   00000H,0FCDBH,0F9B8H,0F695H,0F374H,0F054H,0ED38H,0EA1EH
            .short   0E707H,0E3F4H,0E0E6H,0DDDCH,0DAD8H,0D7D9H,0D4E1H,0D1EFH
            .short   0CF04H,0CC21H,0C946H,0C673H,0C3A9H,0C0E9H,0BE32H,0BB85H
            .short   0B8E3H,0B64CH,0B3C0H,0B140H,0AECCH,0AC65H,0AA0BH,0A7BDH
            .short   0A57EH,0A34CH,0A129H,09F14H,09D0EH,09B18H,09931H,0975AH
            .short   09593H,093DCH,09236H,090A1H,08F1EH,08DABH,08C4AH,08AFCH
            .short   089BFH,08894H,0877CH,08676H,08583H,084A3H,083D6H,0831DH
            .short   08276H,081E3H,08163H,080F7H,0809EH,08059H,08028H,0800AH
            .short   08001H,0800AH,08028H,08059H,0809EH,080F7H,08163H,081E3H
            .short   08276H,0831DH,083D6H,084A3H,08583H,08676H,0877CH,08894H
            .short   089BFH,08AFCH,08C4AH,08DABH,08F1EH,090A1H,09236H,093DCH
            .short   09593H,0975AH,09931H,09B18H,09D0EH,09F14H,0A129H,0A34CH
            .short   0A57EH,0A7BDH,0AA0BH,0AC65H,0AECCH,0B140H,0B3C0H,0B64CH
            .short   0B8E3H,0BB85H,0BE32H,0C0E9H,0C3A9H,0C673H,0C946H,0CC21H
            .short   0CF04H,0D1EFH,0D4E1H,0D7D9H,0DAD8H,0DDDCH,0E0E6H,0E3F4H
            .short   0E707H,0EA1EH,0ED38H,0F054H,0F374H,0F695H,0F9B8H,0FCDBH


;******************************************************************************
; Name       :Load_Vec
; Function   :Loads an Testvector into FBUFF for FFT16 testbench
; Argument   :none
; Return     :none
; Uses       :R15,R14,R13
; Benchmark  :231 clocks for 16 complex values (15 words)
;******************************************************************************
FFT_prog    pushm.a    #7,R10                    ;preserve all save-on-entry registers
                                            ;of the CCS runtime environment
            mov.w   #FBuff,R15              ;points to buffer
            movx.a  #I_Vec,R14              ;points to source
            mov.w   #VecSiz,R13             ;16 as count
            mov.w   #0,R12                  ;0 for imags
Load_Vec1:  mov.w   @R14,0(R15)             ;transfer real to buff
            mov.w   R12,2(R15)              ;clear imag in buffer
            add.w   #4,R15                  ;go to next set (4byte/2words)
            add.w   #4,R14 ;#2,R12          ;next input set
            dec     R13                     ;count up to end
            jnz     Load_Vec1               ;and do again

;******************************************************************************
; Name       :PFFT430
; Function   :Pure Complex FFT Calculation without W0 optimization
; Argument   register :none
;            memory   :complex vector in FBuff.W[32]
; Return     register :none
;            memory   :complex vector bit reversed order in FBuff.W[32]
; Scalable   :Vers0
;             in 1st,10th line of FFT and naming of "BitRT_xx" and "Twidd_xx"
;            :Vers1
;             in 1st,2nd,11th line of FFT and naming of "BitRT_xx" and "Twidd_xx"
;
; Vers0 ...
; Uses       :R15,R14,R13,R12,R11,R10,R9,R8,R7,R6,R5; FStrt.W,FGRPC.W
; Benchmark  :3802 clocks for FFT16 (97 words) =>(not good)
; Vers1 ...
; Uses       :R4..R15; FGPRC.W
; Benchmark  :3190 clocks for FFT16 (83 words) =>(better already)
;******************************************************************************
CFFT430:    mov.w   #VecSiz*2,R6            ;make GRPsiz f.FFT16;R6=(FFTsiz*2)     R6 =FGRPsiz
            mov.w   #VecSiz/2,R11           ;R11 as max grp Cnt =(FFTsiz/2)        |   R11=GRPC
            mov.w   #1,&FGRPC               ;one element/group                     |   |
CFFT430_1:  mov.w   #0,R4                   ;strt=0                                |   |               R4=Strt
            mov.w   #0,R7                   ;(n3) cnt in grp                       | R7=n3             |
CFFT430_2:  mov.w   R4,R14                  ;                                      | | |               |
            add.w   #FBuff,R14              ;R14 points to As                      | | |     R14=*As   |
            mov.w   #0,R5                   ;R5  (n1 as cnt)                       | | |     |         |
            cmp.w   #0,R7                   ;!= W0                                 | | |     |         |
            jne     CFFT430_4               ;then as go for std                    | | |     |         |
            ;for W0 real scaling Butterfly  ;otherwise shortcut                    | | |     |         |
            ;Br=Ar-Br      ;Ar=Ar+Br        ;                                      | | |     |         |
CFFT430_5:  mov.w   R14,R15                 ;prepare * to B's                      | | |     | R15=*B's|
            add.w   R6,R15                  ;R15 points to B's                     | | |     | |       |
            mov.w   @R14,R8                 ;R8=Ar'                                | | |     | |   R8=Ar
            rra.w   R8                      ;scale by2                             | | |     | |   |   |
            mov.w   @R15,R9                 ;R9=Br'                                | | |     | | R9=Br |
            rra.w   R9                      ;scale by2                             | | |     | | | | R10=scr
            mov.w   R8,R10                  ;Br:=Ar                                | | |     | | | | | |
            sub.w   R9,R10                  ;Br:=Ar-Br                             | | |     | | | | | |
            mov.w   R10,0(R15)              ;save Br                               | | |     | + | | + |
            mov.w   R8,R10                  ;Ar:=Ar                                | | |     |   | + R10=scr
            add.w   R9,R10                  ;Ar:=Ar+Br                             | | |     |   +   | |
            mov.w   R10,0(R14)              ;save Ar                               | | |     |       + |
            add.w   #4,R14                  ;*A's++  corrected word +imag          | | |     |         |
            add.w   #1,R5                   ;n1++ in word                          | | |     |         |
            cmp.w   R5,R11                  ;next n1                               | | |     |         |
            jne     CFFT430_5               ;same branching                        | | |     |         |
            rla.w   R6                      ;as in real thing                      | | |     |         |
            add.w   R6,R4                   ;                                      | | |     |         |
            rra.w   R6                      ;                                      | | |     |         |
            add.w   #1,R7                   ;n3++ for word                         | | |     |         |
            cmp.w   &FGRPC,R7               ;                                      | | |     |         |
            jl      CFFT430_2               ;                                      | | |     |         |
            rra.w   R6                      ;FGRPsiz=GRPsiz/2                      | | |     |         |
            rra.w   R11                     ;                                      | | |     |         |
            add.w   R6,R4                   ;                                      | | |     |         |
            rla.w   &FGRPC                  ;FGRPC=FGRPC*2                         | | |     |         |
            cmp.w   #2,R6                   ;abort on last                         | | |     |         |
            jne     CFFT430_1               ;                                      | | |     |         |
CFFT430_4:  mov.w   R7,R10                  ;R10:=n3 for bytes!!!                  | | |     |       R10=scr
            add.w   #BitRT,R10              ;R10:=*BitRT(n3)                       | | |     |       | |
            mov.b   @R10,R10                ;R10:=BitRT(n3)                        | | |     |       | |
            add.w   #Twidd,R10              ;R10:=*TWF((BitRT(n3)/2) for words     | | |     |       | |
            mov.w   0(R10),R12              ;Kr in R12                             | | | R12 =Kr     | |
            mov.w   VecSiz/2(R10),R13       ;Ki in R13 offset 4 in words for FFT16 | | | | R13 =Ki   + |
CFFT430_3:  mov.w   R14,R15                 ;                                      | | | | | | R15=*B's
            add.w   R6,R15                  ;R15 points to B's (n2)                | | | | | | |       |
            mov.w   R12,&MPYS               ;2Ci=Kr...                             | | | | | | |       |
            mov.w   2(R15),&OP2             ;2Ci=Kr*Bi...                          | | | | | | |       |
            mov.w   R13,&MACS               ;2Ci=Kr*Bi+Ki*...                      | | | | | | |       |
            mov.w   @R15,&OP2               ;2Ci=Kr*Bi+Ki*Cr                       | | | | | | |       |
            mov.w   &RESHI,R9               ;***       ;R9:=Ci                     | | | | | | | R9=Ci |
            mov.w   R12,&MPYS               ;2Cr=Kr...                             | | | + | | | |     |
            mov.w   @R15,&OP2               ;2Cr=Kr*Br...                          | | |   | | | |     |
            mov.w   &RESHI,R8               ;R8=Kr*Br                              | | |   | | | | R8=Cr
            mov.w   R13,&MPYS               ;2Cr=Kr*Br-Ki...                       | | |   + | | | |   |
            mov.w   2(R15),&OP2             ;2Cr=Kr*Br-Ki*Bi                       | | |     | | | |   |
            sub.w   &RESHI,R8               ;R8:Cr                                 | | |     | | | |   |
            mov.w   @R14,R10                ;Br=Ar...                              | | |     | | | | R10=scr
            rra     R10                     ;Br=Ar/2...                            | | |     | | | | | |
            sub.w   R8,R10                  ;Br=Ar/2-Cr                            | | |     | | | | | |
            mov.w   R10,0(R15)              ;save Br    ..     ..    ..    ..      | | |     | | | | + |
            mov.w   2(R14),R10              ;Bi=Ai...                              | | |     | | | | R10=scr
            rra.w   R10                     ;Bi=Ai/2...                            | | |     | | | | | |
            sub.w   R9,R10                  ;Bi=Ai/2-Ci                            | | |     | + | | | |
            mov.w   R10,2(R15)              ;save Bi                               | | |     |   | | + |
            rra.w   @R14                    ;Ar=Ar/2...                            | | |     |   | |   |
            add.w   R8,0(R14)               ;Ar=Ar/2+Cr                            | | |     |   | +   |
            rra.w   2(R14)                  ;Ai=Ai/2...                            | | |     |   |     |
            add.w   R9,2(R14)               ;Ai=Ai/2+Ci                            | | |     +   +     |
            ;---                                                                   | | |               |
            add.w   #4,R14                  ;*A's++  corrected word +imag          | | |               |
            add.w   #1,R5                   ;n1++ in word                          | | |               |
            cmp.w   R5,R11                  ;next n1                               | | |               |
            jne     CFFT430_3               ;do next butterfly                     | | |               |
            rla.w   R6                      ;                                      | | |               |
            add.w   R6,R4                   ;                                      | | |               |
            rra.w   R6                      ;                                      | | |               |
            add.w   #1,R7                   ;n3++ for word                         | | |               |
            cmp.w   &FGRPC,R7               ;next n3                               | + |               |
            jl      CFFT430_2               ;do next group                         |   |               |
            rra.w   R6                      ;FGRPsiz=GRPsiz/2                      |   +               |
            rra.w   R11                     ;                                      |                   |
            add.w   R6,R4                   ;                                      |                   +
            rla.w   &FGRPC                  ;FGRPC=FGRPC*2                         |
            cmp.w   #2,R6                   ;abort on last                         +
            jne     CFFT430_1               ;next order
            popm.a  #7,R10                  ;restore all save-on-entry registers
                                            ;of the CCS runtime environment
            reta
            .end

