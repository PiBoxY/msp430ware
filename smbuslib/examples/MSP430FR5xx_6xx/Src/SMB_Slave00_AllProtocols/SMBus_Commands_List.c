/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
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

#include "SMBus_Commands_List.h"

/*! List of implemented commands for this demo
    This demo implements a list of commands but the approach using the t_SMB_Cmd
    structure. This list can be customized to support new commands, or the
    approach to validate commands and execute instructiong can be different.
    For example, a switch-case or conditional statements can be used to
    simplify the code

   SMBus Command types
   - QUICK_COMMAND: (Note: only Write is supported in this implementation )
    S + Slave_Addr+R/W*+A + P
   - SEND_BYTE
     S + Slave_Addr+W+A + DataByte+A + {PEC+A} + P
   - RECEIVE_BYTE
     S + Slave_Addr+R+A + DataByte+A + {PEC+A} + P
   - WRITE_BYTE
     S + Slave_Addr+W+A + Cmd+A + DataByte+A + {PEC+A} + P
   - WRITE_WORD
     S + Slave_Addr+W+A + Cmd+A + DataByteL+A + DataByteH+A + {PEC+A} + P
   - READ_BYTE
     S + Slave_Addr+W+A + Cmd+A +
     Sr + Slave_Addr+R+A + DataByte+A + {PEC+A} + P
   - READ_WORD has the following format:
      S + Slave_Addr+W+A + Cmd+A +
      Sr + Slave_Addr+R+A + DataByteL+A + DataByteH+A + {PEC+A} + P
   - PROCESS_CALL has the following format:
      S + Slave_Addr+W+A + Cmd+A + DataByteL+A + DataByteH+A +
      Sr + Slave_Addr+R+A + DataByteL+A + DataByteH+A + {PEC+A} + P
   - BLOCK_WRITE has the following format:
      S + Slave_Addr+W+A + Cmd+A + ByteCountN+A + DataByte1+A +...+ DataByteN+A + {PEC+A} +P
   - BLOCK_READ has the following format:
      S + Slave_Addr+W+A + Cmd+A +
      Sr + Slave_Addr+R+A + ByteCountN+A + DataByte1+A +...+ DataByteN+A + {PEC+A} +P
   - BLOCK_WRITE_-BLOCK_READ_PROCESS_CALL has the following format:
      S + Slave_Addr+W+A + Cmd+A + ByteCountN+A + DataByte1+A +...+ DataByteN+A +
      Sr + Slave_Addr+R+A + ByteCountM+A + DataByte1+A +...+ DataByteM+A + {PEC+A} + P
 */
const t_SMB_Cmd SMB_CMD_List[CMD_LIST_LENGHT] =
{
    /* Command          Quick_Led_toggle
        Type            QUICK_COMMAND
        Description     Toggles LED0
     */
    // Not implemented in this list since it's processed automatically

    /* Command          Read_Reg_Ptr
        Type            RECEIVE_BYTE
        Description     Sends the contents of the register pointer by register pointer
     */
    // Not implemented in this list since it's processed automatically

    /* Command          Set_Reg_Ptr
        Type            SEND_BYTE
        CMD             0x00-0x03
        Description     Changes the register pointer
                        Reg0 = SMBus Status (check t_SMBus_Status)
                        Reg1 = SMBus Control (check t_SMBus_Control)
                        Reg2 = P4 status
                        Reg3 = Demo Version
     */
    {0x00, 0x03, Write_Reg_Ptr_hdlr},

    /* Command          Write_Reg
        Type            WRITE_BYTE
        CMD             0x10-0x13
        Description     Changes the value of a register
                        (see previous definition of regs)
     */
    {0x10, 0x03, Write_Reg_hdlr},

    /* Command          Read_Reg
        Type            READ_BYTE
        CMD             0x20-0x23
        Description     Reads the value of a register
                        (see previous definition of regs)
     */

    {0x20, 0x03, Read_Reg_hdlr},

    /* Command          Read_ADC_Ch
        Type            READ_WORD
        CMD             0x30
        Description     Reads the value of ADC channel
     */
    {0x30, 0x00, Read_ADC_Ch_hdlr},

    /* Command          Mult_Bytes
        Type            PROCESS_CALL
        CMD             0x40
        Description     Multiplies 2 bytes
     */
    {0x40, 0x00, Mult_Bytes_hdlr},

    /* Command          Write_String
        Type            BLOCK_WRITE
        CMD             0x50
        Description     Writes a block of data to local string
     */
    {0x50, 0x00, Write_String_hdlr},

    /* Command          Read_String
        Type            BLOCK_READ
        CMD             0x51
        Description     Reads a block of data from local string
     */
    {0x51, 0x00, Read_String_hdlr},

    /* Command          Calc_CRC
        Type            BLW_BLR_PROC_CALL
        CMD             0x60
        Description     Calculates the CRC of a series of bytes
     */
    {0x60, 0x00, Calc_CRC_hdlr},
};
