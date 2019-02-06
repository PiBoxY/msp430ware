/*******************************************************************************
 *
 *  usbTest.c
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

#include "hal_MSP-EXP430F5438.h"
#include "usbTest.h"

/**********************************************************************//**
 * @brief  Executes the "USB-UART" menu option in the User Experience
 *         example code.
 *
 * Allows communication between the MSP430 UART and a computer serial
 * COM port via a USB <> serial converter.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void usbTest(void)
{
    unsigned char quit;
    volatile unsigned char i;

    halButtonsInterruptEnable(BUTTON_ALL);
    halUsbInit();
    halLcdClearScreen();
    halLcdPrintLine("baudrate = 57600 ", 0, 0);
    halLcdPrintLine("Type on PC       ", 1, 0);
    halLcdPrintLine("or Press Buttons ", 2, 0);
    quit = 0;

    while (quit == 0)
    {
        buttonsPressed = 0;
        __bis_SR_register(LPM0_bits + GIE);
        __no_operation();

        if (buttonsPressed & BUTTON_UP)
            halUsbSendChar('U');
        if (buttonsPressed & BUTTON_DOWN)
            halUsbSendChar('D');
        if (buttonsPressed & BUTTON_LEFT)
            halUsbSendChar('L');
        if (buttonsPressed & BUTTON_RIGHT)
            halUsbSendChar('R');
        if (buttonsPressed & BUTTON_SELECT)
        {
            halUsbSendChar('Q');
            halLcdClearScreen();
            quit = 1;
        }
        if (buttonsPressed & BUTTON_S1)
            halUsbSendChar('1');

        if (buttonsPressed & BUTTON_S2)
            halUsbSendChar('2');

        if (bufferSize > 0)
        {
            halLcdPrint(halUsbReceiveBuffer,  OVERWRITE_TEXT);
            halLcdCursor();
            for (i = 0; i < bufferSize; i++)
                halUsbReceiveBuffer[i] = '\0';
            bufferSize  = 0;
        }
    }
    halUsbShutDown();
    halLcdCursorOff();
}

