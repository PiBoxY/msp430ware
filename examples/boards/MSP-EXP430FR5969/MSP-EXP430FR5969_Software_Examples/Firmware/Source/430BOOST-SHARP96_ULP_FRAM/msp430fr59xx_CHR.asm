; --COPYRIGHT--,BSD
; Copyright (c) 2012, Texas Instruments Incorporated
; All rights reserved.
;
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the following conditions
; are met:
;
; *  Redistributions of source code must retain the above copyright
;    notice, this list of conditions and the following disclaimer.
;
; *  Redistributions in binary form must reproduce the above copyright
;    notice, this list of conditions and the following disclaimer in the
;    documentation and/or other materials provided with the distribution.
;
; *  Neither the name of Texas Instruments Incorporated nor the names of
;    its contributors may be used to endorse or promote products derived
;    from this software without specific prior written permission.
;
; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
; AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
; THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
; PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
; CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
; EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
; WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
; OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
; EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
; --/COPYRIGHT--*/

;*****************************************************************************
;
;  Helper functions for use during active power measurement.
;
;*****************************************************************************
 .cdecls C,LIST,  "msp430.h"

            .global CHR_prep
            .global fram_chr_50
            .global fram_chr_66
            .global fram_chr_75
            .global fram_chr_100


;-----------------------------------------------------------------------------
            .text
			.retain
			.retainrefs
;-----------------------------------------------------------------------------
;     Prepare registers for FRAM CHR test code
;-------------------------------------------------------------------------------
CHR_prep
             clr.w  R5
             mov.b  R12,R5
             clr.w  R12
             reta

;-------------------------------------------------------------------------------
;     FRAM access with cache on hit/miss rate 1/1 CHR = 50%
;-------------------------------------------------------------------------------
            .align 8   ; align on 64bit boundary (8x 8bit)
fram_chr_50
            nop
            nop
            nop
            nop

            nop
            nop
            nop
            nop

            nop
            nop
chr_50_0    bit.b   #0x01,R5 ; nop
            jne     chr_50_exit_loop ; nop
            jmp     chr_50_1
            nop

            nop
            nop
            nop
            nop

            nop
            nop
chr_50_1    jmp chr_50_2
			nop

			nop
			nop
			nop
			nop

			nop
			nop
chr_50_2    jmp chr_50_3
            nop

            nop
            nop
            nop
            nop

            nop
            nop
chr_50_3    jmp chr_50_4
			nop

			nop
			nop
			nop
			nop

			nop
			nop
chr_50_4    jmp chr_50_5
			nop

			nop
			nop
			nop
			nop

			nop
			nop
chr_50_5    jmp chr_50_6
			nop

			nop
			nop
			nop
			nop

			nop
			nop
chr_50_6    jmp chr_50_0
			nop
			nop

chr_50_exit_loop
			reta

;-------------------------------------------------------------------------------
;     FRAM access with cache on hit/miss rate 2/1 CHR = 66%
;-------------------------------------------------------------------------------

            .align 8                        ; align on 64bit boundary (8x 8bit)
fram_chr_66
            nop
chr_66_0    bit.b   #01h,R5
            jnz     chr_66_exit_loop
            jmp     chr_66_1

            nop
            nop
            nop
            nop

            nop
chr_66_1    nop
            jmp     chr_66_2
            nop

            nop
            nop
            nop
            nop

            nop
chr_66_2    nop
            jmp     chr_66_3
            nop

            nop
            nop
            nop
            nop

            nop
chr_66_3    nop
            jmp     chr_66_4
            nop

            nop
            nop
            nop
            nop

            nop
chr_66_4    nop
            jmp     chr_66_5
            nop

            nop
            nop
            nop
            nop

            nop
chr_66_5    nop
			jmp     chr_66_0
			nop

			nop

chr_66_exit_loop
			reta

;-------------------------------------------------------------------------------
;     FRAM access with cache on hit/miss rate 3/1 CHR = 75%
;-------------------------------------------------------------------------------
fram_chr_75

            .align 8       ; align on 64bit boundary (8x 8bit)

chr_75_0    bit.w   #01h,R5
            jnz     chr_75_exit_loop
            jmp     chr_75_1
            nop

            nop
            nop
            nop
            nop

chr_75_1    nop
            nop
            jmp     chr_75_2
            nop

            nop
            nop
            nop
            nop

chr_75_2    nop
            nop
            jmp     chr_75_3
            nop

            nop
            nop
            nop
            nop

chr_75_3    nop
            nop
            jmp     chr_75_4
            nop

            nop
            nop
            nop
            nop

chr_75_4    nop
            nop
            jmp     chr_75_0                                                 ; endless loop
            nop

            nop

chr_75_exit_loop
            reta

;-------------------------------------------------------------------------------
;     FRAM access with cache on hit/miss rate 1/0 CHR = 100%
;-------------------------------------------------------------------------------
            .align 8       ; align on 64bit boundary (8x 8bit)

fram_chr_100
            nop;  clr.w   R5
            nop
            nop
            nop

chr_100_0   bit.b   #0x01,R5
            jnz     chr_100_exit_loop
            jmp     chr_100_0
chr_100_exit_loop
			reta

;---------------------------------------------------------------------------
            .END

