// *************************************************************************************************
//
//      Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/
//
//
//        Redistribution and use in source and binary forms, with or without
//        modification, are permitted provided that the following conditions
//        are met:
//
//          Redistributions of source code must retain the above copyright
//          notice, this list of conditions and the following disclaimer.
//
//          Redistributions in binary form must reproduce the above copyright
//          notice, this list of conditions and the following disclaimer in the
//          documentation and/or other materials provided with the
//          distribution.
//
//          Neither the name of Texas Instruments Incorporated nor the names of
//          its contributors may be used to endorse or promote products derived
//          from this software without specific prior written permission.
//
//        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//        "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//        LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//        LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//        DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//        THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// *************************************************************************************************

#ifndef DATALOG_H_
#define DATALOG_H_

// *************************************************************************************************
// Include section

// *************************************************************************************************
// Prototypes section

// internal functions
extern void reset_datalog(void);
extern u8 is_datalog(void);
extern void do_datalog(void);
extern void display_datalog(u8 line, u8 update);
extern void stop_datalog(void);

// menu functions
extern void sx_datalog(u8 line);

// *************************************************************************************************
// Defines section

// Data logger state
#define DATALOG_OFF                                             (0u)
#define DATALOG_ON                                              (1u)

// Data memory range: 0x8000 - 0x9DFF (7.5kB)
#define DATALOG_MEMORY_START            (0x8000)
#define DATALOG_MEMORY_END              (0x9DFF)
#define DATALOG_BYTES_PER_PAGE          (512u)
#define DATALOG_PAGE_START                              (DATALOG_MEMORY_START / \
                                                         DATALOG_BYTES_PER_PAGE)
#define DATALOG_PAGE_END                                (DATALOG_MEMORY_END / \
                                                         DATALOG_BYTES_PER_PAGE)

// Store data in buffer before writing to flash memory
#define DATALOG_BUFFER_SIZE             (128u)
#define DATALOG_BUFFER_WRITE_THRESHOLD  (100u)

// Datalogging interval in seconds
#define DATALOG_INTERVAL                        (5u)

// Datalogger state machine commands
#define DATALOG_CMD_START                       (BIT0)
#define DATALOG_CMD_CLOSE                       (BIT1)
#define DATALOG_CMD_ERASE                       (BIT2)
#define DATALOG_CMD_ADD_DATA                    (BIT3)
#define DATALOG_CMD_WRITE_DATA                  (BIT4)

// Datalog modes
#define DATALOG_MODE_HEARTRATE                  (BIT0)
#define DATALOG_MODE_TEMPERATURE                (BIT1)
#define DATALOG_MODE_ALTITUDE                   (BIT2)
#define DATALOG_MODE_ACCELERATION               (BIT3)

// *************************************************************************************************
// Global Variable section

// Flags
typedef union
{
    struct
    {
        u8 on          : 1; // 1 = data logging has started
        u8 memory_full : 1; // 1 = memory is full
        u8 one_second  : 1; // 1 = 1 second has elapsed
    } flag;
    u8 all;                 // Shortcut to all flags (for reset)
} datalog_flags;

struct datalog
{
    // Flags
    datalog_flags flags;

    // Data logging mode
    u8 mode;

    // Data logging interval
    u8 interval;

    // Data logging delay counter
    u8 delay;

    // Datalog memory write pointer
    u16 *wptr;

    // Datalogger write buffer index
    u8 idx;

    // Datalogger write buffer
    u8 buffer[DATALOG_BUFFER_SIZE];
};
extern struct datalog sDatalog;

// *************************************************************************************************
// Extern section

#endif                          /*DATALOG_H_ */
