/*
 * Copyright (C) 2010 - 2016 Texas Instruments Incorporated - http://www.ti.com/ 
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
*/

/**
* \file StdUseCase.h
*
* \par StdUseCase.h contains global definitions, structs, prototypes and variables required by StdUseCase.cpp.
**/

#ifndef _STD_USECASE_
#define _STD_USECASE_

/*****************************************************************************/
/*                                 INCLUDES                                  */
/*****************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <time.h>
#include <iostream>
#include <string>
#include <fstream>
#include <regex>

#include <MSP430.h>
#include <MSP430_Debug.h>
#include <MSP430_EEM.h>
#include <MSP430_FET.h>

/*****************************************************************************/
/*                               DEFINITIONS                                 */
/*****************************************************************************/

extern char *optarg;
extern int   optind;

//! \brief Default target voltage
#define TARGET_VCC 3000

#ifndef WM_USER
#define WM_USER 0x0400
#endif

#define WM_BpMESSAGE      (WM_USER + 1)
#define WM_SsMESSAGE      (WM_USER + 2)
#define WM_StoMESSAGE     (WM_USER + 3)
#define WM_SStateMESSAGE  (WM_USER + 4)
#define WM_WarMESSAGE     (WM_USER + 5)
#define WM_CPUStopMESSAGE (WM_USER + 6)

#define fBpMESSAGE      0x0001
#define fSsMESSAGE      0x0002
#define fStoMESSAGE     0x0004
#define fSStateMESSAGE  0x0008
#define fWarMESSAGE     0x0010
#define fCPUStopMESSAGE 0x0020

//! \brief Used for state polling
#define RUN_TILL_TIMEOUT (STOPPED-1)

//! \brief Maximum password length
#define MAX_PASS      500
//! \brief Maximum device name length
#define MAX_NAME_LEN  50
//! \brief Maximum number of connected USB FETs
#define MAX_FET_NR    16
//! \brief Maximum file path length
#define MAX_PATH_LEN  1024

// References for version return values of MSP430_Initialize() 
//! \brief Mismatch between DLL version and FET firmware - FET firmware update required
#define DLL_FW_MISMATCH           -1
//! \brief Broken eZ-FET detected - FET recovery required
#define BROKEN_EZ_FET             -2
//! \brief FET uses DLLv2 firmware - Major upgrade required
#define V2_TO_V3_UPGRADE_REQUIRED -3

// additional defines
#ifndef __PRINT_LOG_OPTS_
//! \brief Message types for Print_N_Log()
#define __PRINT_LOG_OPTS_
//! \brief Standard message
#define STD_MSG            0
//! \brief Error message
#define ERR_MSG            1
//! \brief Report message
#define REPORT_MSG         2
//! \brief Log message
#define LOG_MSG            3
//! \brief One line message
#define ONE_LINE_MSG       4
#endif

//! \brief maximum number of options
#define MAX_OPTS           1000

#ifndef __INST_STATE_
//! \brief Definitions for the global state bitmask inst_state
#define __INST_STATE_
//! \brief This flag is set if progess bar should be displayed during FW update
#define PROG_BAR_ON    (0x0001)
//! \brief This flag is set if external memory should be written
#define WRITE_EXT_MEM  (0x0002)
//! \brief This flag is set if an MSP430L092 or RF430 device should be programmed
#define MODE_SELECT    (0x0004)
//! \brief This flag is set if the console output should be disabled
#define QUIET_MODE     (0x0008)
//! \brief This flag is set if the flasher routine is aborted due to an error
#define ALT_EXIT       (0x0010)
//! \brief This flag is set if the BSL memory should be unlocked
#define WRITE_BSL      (0x0020)
//! \brief This flag is set if logging should be disabled
#define LOG_OFF        (0x0040)
//! \brief This flag is set if the INFO A memory should be unlocked
#define UNLOCK_FLASH   (0x0080)
//! \brief This flag is set if the target device should be secured
#define BLOW_FUSE      (0x0100)
//! \brief This flag is set if FET FW update should be performed without a user prompt
#define SKIP_UDPROMPT  (0x0200)
//! \brief This flag is set if the number of specified BP addresses exceeds the number of available hardware breakpoints
#define BP_LIMIT       (0x0400)
//! \brief This flag is set if no reset should be applied to the device during start-up
#define NO_RESET       (0x0800)
//! \brief This flag is set if a 'hard' reset (using the RST/NMI pin) should be executed on exit
#define RESET_ON_EXIT  (0x1000)
//! \brief This flag is set if an erase operation should be performed without writing the memory afterwards
#define ERASE_NO_WRITE (0x2000)
//! \brief This flag is set if the target device is an MSP432
#define TARGETARCH_432 (0x4000)
#endif

#ifndef __OPT_FLAGS_
#define __OPT_FLAGS_
#define OPT_INTERFACE (0x0001)
#define OPT_INFILE    (0x0002)
#define OPT_DEVNAME   (0x0004)
#define OPT_PASSWORD  (0x0008)
#define OPT_EXITSPECS (0x0010)
#define OPT_READSPECS (0x0020)
#define OPT_BPSPECS   (0x0040)
#define OPT_FETSPEED  (0x0080)
#endif

#ifndef __SPACED_ARG_OPTS_
#define __SPACED_ARG_OPTS_
#define COLL_UA_KS        (0x0000)    // argument: is [] collection, is unattached to the trigger, contains meaningful spaces
#define FILENAME_UA_KS    (0x0001)    // argument: is filename (i.e. contains an extension), ...
#define COLL_A_KS         (0x0002)    // argument: is attached to the trigger...
#define FILENAME_A_KS     (0x0003)
#define COLL_UA_RS        (0x0004)    // argument: does not contain meaningful spaces, ...
#define FILENAME_UA_RS    (0x0005)
#define COLL_A_RS         (0x0006)
#define FILENAME_A_RS     (0x0007)            
#define IS_FILENAME       (0x0001)
#define IS_ATTACHED       (0x0002)
#define REMOVE_SPACES     (0x0004)    
#endif

#ifndef    NULL
    #define NULL 0
#endif

#define EOF (-1)

/*****************************************************************************/
/*                             STRUCTS & ENUMS                               */
/*****************************************************************************/

//! \brief Struct for storing command line options
struct sDefOptions {
        std::string pcDeviceName;
        char* pcDeviceMode;
        char* pcPassword;
        int32_t pcPWlen;
        char* pcInterface;
        char* pcReadSpecs;
        char* pcReadFile;
        char* pcReadMem;
        char* pcFile;
        const char* pcEraseType;
        char* pcVerify;
        int32_t pcVCC;
        char* pcExitSpecs;
        char* pcVerifyMemoryFile;
        char* pcVerifyRegisterFile;
        char* pcBreakpoints;
        char* pcTimeout;
        char* pcFETspeed;
};
typedef struct sDefOptions sDefOptions_t;

//! \brief Struct for storing additional options
struct sAddOptions {
    char*  OptLetter;
    char** OptString;
};
typedef struct sAddOptions sAddOptions_t;

//! \brief Struct for runtime entries
struct sRTE {
    //! \brief For storing device name if entered during run time
    char device_name_entry[MAX_NAME_LEN];
    //! \brief For storing filename if entered during runtime 
    char filename_entry[MAX_PATH_LEN];
    //! \brief For storing the processed filename
    char filename_processed[MAX_PATH_LEN];
    //! \brief For storing password length when calculated
    int32_t password_length;
    //! \brief For storing password if entered during run time
    char password_entry[MAX_PASS];
    //! \brief For storing memory dump filename
    char read_file[MAX_PATH_LEN];
    //! \brief For storing processed filename for verify
    char verify_file_processed[MAX_PATH_LEN];
    //! \brief For storing memory dump segment
    char read_mem[MAX_NAME_LEN];
    //! \brief For storing the processed exit_specs
    char exit_specs_processed[MAX_NAME_LEN];
    //! \brief For storing the processed read_specs
    char read_specs_processed[MAX_NAME_LEN];
    //! \brief For storing the processed breakpoint specs
    char bp_specs_processed[MAX_NAME_LEN];
    //! \brief For storing the type of the read file
    enum FILE_TYPE file_type;
};
typedef struct sRTE sRTE_t;

//! \brief Struct for default options
struct sDef {
    char defInterface[6];
    char defEraseType[10];
    char defPassword[1];
    char defTRUE[5];
    char defFALSE[6];
    int32_t deviceAC;
    char defTimeout[6];
    char defFETspeed[7];
};
typedef struct sDef sDef_t;

//! \brief Struct for storing information on all connected USB FETs
struct sUsbFETs {
    char* COMport;
    int32_t status;
};
typedef sUsbFETs sUsbFETs_t;

//! \brief In case a UseCase will not execute regularly, a message will be
//! printed in the command prompt. Exitcodes are defined as followed:
enum EXIT_CODE
{
    // No Error, application was executed.
    EXIT_NO_ERROR,
    // Unknown Error.
    EXIT_UNKNOWN,
    // MSP430_Configure().
    EXIT_CONFIGURE,
    // MSP430_Erase().
    EXIT_ERASE,
    // MSP430_EraseCheck().
    EXIT_ERASECHECK,
    // MSP430_Memory(,,,WRITE).
    EXIT_MEMORY_WRITE,
    // MSP430_Memory(,,,READ)
    EXIT_MEMORY_READ,
    // MSP430_Register(,,WRITE)
    EXIT_REGISTER_WRITE,
    // MSP430_Register(,,READ)
    EXIT_REGISTER_READ,
    // MSP430_Reset()
    EXIT_RESET,
    // MSP430_Run(runmode,FALSE)
    EXIT_RUN,
    // Various errors, see prompt on screen for more information
    EXIT_VARIOUS,
    // MSP430_State(,FALSE,)
    EXIT_STATE_NOSTOP,
    // MSP430_State(,TRUE,)
    EXIT_STATE_STOP,
    // MSP430_Breakpoint()
    EXIT_BREAKPOINT,
    // MSP430_Device()
    EXIT_DEVICE,
    // MSP430_OpenDevice()
    EXIT_OPENDEVICE,
    // MSP430_VerifyMem()
    EXIT_VERIFYMEM,
    // MSP430_Register(s)()
    EXIT_REGISTERS,
    // MSP430_Initialize()
    EXIT_INITIALIZE,
    // MSP430_EEM_Init()
    EXIT_EEM_INIT,
    // MSP430_EEM_SetBreakpoint()
    EXIT_EEM_SETBREAKPOINT,
    // MSP430_EEM_GetBreakpoint()
    EXIT_EEM_GETBREAKPOINT,
    // MSP430_EEM_SetCombineBreakpoint()
    EXIT_EEM_SETCOMBINEBREAKPOINT,
    // MSP430_EEM_GetCombineBreakpoint()
    EXIT_EEM_GETCOMBINEBREAKPOINT,
    // MSP430_EEM_SetClockControl()
    EXIT_EEM_SETCLOCKCONTROL,
    // MSP430_EEM_GetClockControl()
    EXIT_EEM_GETCLOCKCONTROL,
    // MSP430_CcSetModuleNames()
    EXIT_CCSETMODULENAMES,
    // MSP430_CcGetModuleNames()
    EXIT_CCGETMODULENAMES,
    // MSP430_EEM_Open()
    EXIT_EEM_OPEN,
    // MSP430_EEM_Close()
    EXIT_EEM_CLOSE,
    // MSP430_EEM_Write_Register()
    EXIT_EEM_WRITE_REGISTER,
    // MSP430_EEM_Read_Register()
    EXIT_EEM_READ_REGISTER,
    // MSP430_EEM_Read_Register_Test()
    EXIT_EEM_READ_REGISTER_TEST,
    // MSP430_EEM_SetSequencer()
    EXIT_EEM_SETSEQUENCER,
    // MSP430_EEM_GetSequencer()
    EXIT_EEM_GETSEQUENCER,
    // MSP430_EEM_ReadSequencerState()
    EXIT_EEM_READSEQUENCERSTATE,
    // MSP430_EEM_SetTrace()
    EXIT_EEM_SETTRACE,
    // MSP430_EEM_GetTrace()
    EXIT_EEM_GETRACE,
    // MSP430_EEM_ReadTraceBuffer()
    EXIT_EEM_READTRACEBUFFER,
    // MSP430_EEM_RefreshTraceBuffer()
    EXIT_EEM_REFRESHTRACEBUFFER,
    // MSP430_EEM_SetVariable()
    EXIT_EEM_SETVARIABLE,
    // MSP430_EEM_SetVariableWatch()
    EXIT_EEM_SETVARIABLEWATCH,
    // MSP430_EEM_GetVariableWatch()
    EXIT_EEM_GETVARIABLEWATCH,
    // MSP430_VCC()
    EXIT_VCC,
    // MSP430_GetCurVCCT()
    EXIT_GETCURVCCT,
    // MSP430_GetExtVoltage()
    EXIT_GETEXTVOLTAGE,
    // MSP430_FET_GetFwVersion()
    EXIT_FET_GETFWVERSION,
    // MSP430_FET_GetHwVersion()
    EXIT_FET_GETHWVERSION,
    // MSP430_FET_FwUpdate()
    EXIT_FET_FWUPDATE,
    // MSP430_FET_I2C()
    EXIT_FET_I2C,
    // MSP430_FET_SelfTest()
    EXIT_FET_SELFTEST,
    // MSP430_FET_SetSignals()
    EXIT_FET_SETSIGNALS,
    // MSP430_FET_Reset()
    EXIT_FET_RESET,
    // reserved
    EXIT_RESERVED1,
    // reserved
    EXIT_RESERVED2,
    // MSP430_ReadOutFile()
    EXIT_READOUTFILE,
    // MSP430_GetNumberOfUsbIfs()
    EXIT_GETNUMBEROFUSBIFS,
    // MSP430_GetNameOfUsbIf()
    EXIT_GETNAMEOFUSBIF,
    // MSP430_ProgramFile()
    EXIT_PROGRAMFILE,
    // MSP430_VerifyFile()
    EXIT_VERIFYFILE,
    // No MSP430.dll Error, see screen
    EXIT_NODLL
};

/******************************************************************************/

extern DEVICE_T g_tTargetDevice;
extern int32_t g_lTargetDeviceRegisters[16];
extern sDefOptions_t g_sOpt;
extern int32_t bpEventFlags;
extern int32_t ValueParam;

extern uint32_t   MyGlob_MsgId;
extern uint32_t MyGlob_wParam;
extern int32_t MyGlob_lParam;

typedef int BOOL;

#ifndef WIN32
typedef uint32_t DWORD;
#endif

/******************************************************************************/
/*                           FUNCTION PROTOTYPES                              */
/******************************************************************************/

// main Flasher routine
int Flasher(int argc, char* argv[], sDefOptions_t* sOpt, sRTE_t* RTE);

// functions with DLL interaction
void StdUseCase_Exit(int exitCode, int32_t lErrorNumber);
void StdUseCase_Close(sDefOptions_t* sOpt);
int StdUseCase_GetUsbIfs(void);
void StdUseCase_InterfaceInit(sDefOptions_t* sOpt);
void StdUseCase_CheckFirmware(sDefOptions_t* sOpt);
void StdUseCase_ReadFWVersion(void);
void StdUseCase_ReadHWVersion(void);
void StdUseCase_PowerUp(sDefOptions_t* sOpt);
void StdUseCase_SpecialErase(sDefOptions_t* sOpt);
void StdUseCase_GetDevice(sDefOptions_t* sOpt);
void StdUseCase_DisplayResults(int argc, char* argv[], sDefOptions_t* sOpt);
void StdUseCase_Erase(sDefOptions_t* sOpt);
void StdUseCase_ReadWrite(sDefOptions_t* sOpt, sRTE_t* RTE);
void StdUseCase_BlowFuse(void);
void StdUseCase_SetBreakpoints(int32_t *AddrArray);
void StdUseCase_ClearBreakpoint(int32_t *AddrArray, int number);
void StdUseCase_RunProgram(sDefOptions_t* sOpt, int32_t* AddrArray);
void StdUseCase_Reset(int32_t method, int32_t execute, int32_t releaseJTAG);
void StdUseCase_GetCpuState(void);
void StdUseCase_Run(void);
void StdUseCase_EemInit (void);
void StdUseCase_EemReadStateStorageBuffer(int16_t NumberOfValidEntries);
void StdUseCase_PrintBpParameter_t(BpParameter_t* pBpBuffer);
BOOL StdUseCase_WaitForTrigger(DWORD dwMilliseconds);
BOOL StdUseCase_WaitForEvent(uint32_t MsgId, long dwMilliseconds);

// support functions
void EnableLog(int argc, char* argv[]);
void AddOpts(char* cOpts);
void RegisterOpt(char* OptLetter, char** OptString);
void StartTimer(void);
void StopTimer(void);
void PrintElapsedTime(void);
void PrintVersion(void);
void Help(void);
void Exit_Specs(void);
int Print_N_Log(unsigned short, const char*, ...);
int EvaluateOptions(int argc, char* argv[], sDefOptions_t* sOpt, sRTE_t* RTE);
int getopt(int argc, char **argv, char *opts, sRTE_t* RTE);
int ReadSpacedArgs(int, char**, int*, unsigned short, char*);
STATUS_T ParseInputFile(char*, int32_t *lStartAddress, int32_t *lProgramLength);

// callback functions
int Init_System_Notify_Callback(void);
void MY_MSP430_CallbackFunc(uint32_t MsgId, uint32_t wParam, uint32_t lParam, int32_t clientHandle);

/*****************************************************************************/

#endif //_STD_USECASE_

/*****************************************************************************/
/*                           END OF SOURCE FILE                              */
/*****************************************************************************/
