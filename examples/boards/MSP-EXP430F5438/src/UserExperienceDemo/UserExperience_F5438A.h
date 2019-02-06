/*******************************************************************************
 *
 *  UserExperience_F5438A.h
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

#ifndef USER_EXPERIENCE_H
#define USER_EXPERIENCE_H

// Device type definitions
#define F5438             0x3854
#define F5438A            0x8005

extern unsigned char adcTempISR;
extern unsigned char fftISR;
extern unsigned char audioISR;
extern const unsigned int TI_BUG[];

extern unsigned char boardMode;
extern unsigned char lcdBackLightLevelSetting;
extern unsigned char lcdContrastSetting;
extern unsigned long lastAudioByteFlash;
extern int Acc_X_Calibrated_Offset;
extern int Acc_Y_Calibrated_Offset;
extern int Acc_Z_Calibrated_Offset;

extern unsigned char wakeUpOnAcc;
extern unsigned char boardModeLOCAL;
extern int Acc_X_Calibrated_OffsetLOCAL;
extern int Acc_Y_Calibrated_OffsetLOCAL;
extern int Acc_Z_Calibrated_OffsetLOCAL;
extern unsigned char lcdBackLightLevelSettingLOCAL;
extern unsigned char lcdContrastSettingLOCAL;
extern unsigned char wakeUpOnAccLOCAL;

extern unsigned long lastAudioByte;

extern unsigned char RTCAccHalfSec, RTCExit64Hz, RTCExitSec, RTCAccSec;

extern void setupRTC(void);
extern void UserExperience(void);

#endif /* USER_EXPERIENCE_H */
