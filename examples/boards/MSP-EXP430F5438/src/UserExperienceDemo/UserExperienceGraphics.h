/*******************************************************************************
 *
 *  UserExperienceGraphics.h
 *
 *  Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#define MAX_IMAGE     7

extern const unsigned int TI_BUG[];             //Columns: 14, Rows: 106
extern const unsigned int TI_TINY_BUG[];        //Columns: 4, Rows: 36
extern const unsigned int TI[];                 //Columns: 17, Rows: 47
extern const unsigned int MSP430[];             //Columns: 6, Rows: 84

extern const unsigned int IMG_DOWN[];           //Columns: 4, Rows: 32
extern const unsigned int IMG_DOWN_FILLED[];    //Columns: 4, Rows: 32
extern const unsigned int IMG_LEFT[];           //Columns: 4, Rows: 32
extern const unsigned int IMG_LEFT_FILLED[];    //Columns: 4, Rows: 32
extern const unsigned int IMG_RIGHT[];          //Columns: 4, Rows: 32
extern const unsigned int IMG_RIGHT_FILLED[];   //Columns: 4, Rows: 32
extern const unsigned int IMG_UP[];             //Columns: 4, Rows: 32
extern const unsigned int IMG_UP_FILLED[];      //Columns: 4, Rows: 32

#endif /* GRAPHICS_H */
