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
* \file StdUseCase.cpp
**/

/*****************************************************************************/
/*                                 INCLUDES                                  */
/*****************************************************************************/

#include "StdUseCase.h"

#ifndef WIN32
#include <thread>
#include <mutex>
#include <condition_variable>
#endif

using namespace std;

/*****************************************************************************/
/*                                 VARIABLES                                 */
/*****************************************************************************/

//! \brief Struct for command line options
sDefOptions_t g_sOpt = { "Unknown", NULL, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL };
//! \brief Struct for runtime entries
sRTE_t g_RTE;
//! \brief Array for storing information on all connected USB FETs
sUsbFETs_t UsbFETs[MAX_FET_NR] = { {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0},
                                   {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} };

//! \brief MSP-FET430PIF version string
static const int8_t myLptVersion[] = "L_01";
//! \brief FET firmware version number
int32_t  FWVersion;
//! \brief Hardware version string
uint8_t* HWVersion = (uint8_t*)myLptVersion;
//! \brief Hardware version number
char FETversion[6] = {0};
//! \brief Hardware type string
std::string FETtype;

//! \brief Struct for target device information
DEVICE_T g_tTargetDevice;
//! \brief Array for target device CPU register content
int32_t g_lTargetDeviceRegisters[16];

// Status messages
//! \brief Return value for MSP430_Initialize
STATUS_T Ret_Initialize;
//! \brief Return value for MSP430_VCC
STATUS_T Ret_VCC;
//! \brief Return value for MSP430_Configure
STATUS_T Ret_Configure;
//! \brief Return value for MSP430_OpenDevice
STATUS_T Ret_OpenDevice;
//! \brief Return value for MSP430_MSP430_ReadOutFile
STATUS_T Ret_ReadFile;
//! \brief Return value for MSP430_ProgramFile
STATUS_T Ret_ProgramFile;
//! \brief Return value for MSP430_VerifyFile
STATUS_T Ret_VerifyFile;
//! \brief Return value for MSP430_Reset
STATUS_T Ret_Reset;
//! \brief Return value for MSP430_Registers
STATUS_T Ret_Registers;
//! \brief Return value for MSP430_FET_FwUpdate
STATUS_T Ret_FWUpdate;
//! \brief Return value for MSP430_FET_GetHwVersion
STATUS_T Ret_GetHwVersion;
//! \brief Return value for MSP430_FET_GetFwVersion
STATUS_T Ret_GetFwVersion;
//! \brief Return value for MSP430_MSP430_EEM_SetBreakpoint
STATUS_T Ret_Breakpoints;
//! \brief Return value for MSP430_Run
STATUS_T Ret_Run;
//! \brief Return value for MSP430_Erase
STATUS_T Ret_Erase;
//! \brief Return value for MSP430_Device
STATUS_T Ret_Device;

//! \brief Check value for DLL/FET-firmware compatibility
int32_t lVersion;
//! \brief Return value for MSP430_Error_Number
int32_t lErrorNumber;

//! \brief Handle for FW update callback function
int32_t myHandle;

extern sDef_t g_Def;
extern unsigned int inst_state;
extern FILE* lfile;

uint32_t wParam_prev = 0;

// Breakpoint help variables
extern int BPcount;
extern int32_t BPAddr[];

// the following declarations are merged from getopt.cpp
//! \brief Index for command line options
int optind = 1;
//! \brief Pointer to an option argument
char *optarg;

#ifndef INFINITE
const unsigned INFINITE = 0xFFFFFFFF;
#endif

#ifndef WIN32
void Sleep(unsigned ms)
{
    this_thread::sleep_for(chrono::milliseconds(ms));
}
#endif

class Event
{
public:
    Event();
    bool wait(unsigned int timeout);
    void set();
    void reset();

private:
#ifdef WIN32
    HANDLE event_;
#else
    bool hasOccured_;
    mutex mutex_;
    condition_variable condition_;
#endif
};

#ifdef WIN32
Event::Event() : event_(CreateEvent(NULL, FALSE, FALSE, NULL)) {}

void Event::set()
{
    SetEvent(event_);
}

void Event::reset()
{
    ResetEvent(event_);
}

bool Event::wait(unsigned int msTimeout)
{
    return (WaitForSingleObject(event_, msTimeout) == WAIT_OBJECT_0);
}
#else
Event::Event() : hasOccured_(false) {}

void Event::set()
{
    unique_lock<mutex> lock(mutex_);
    hasOccured_ = true; 
    condition_.notify_all();
}

void Event::reset()
{
    unique_lock<mutex> lock(mutex_);
    hasOccured_ = false;
}

bool Event::wait(unsigned int msTimeout)
{
    unique_lock<mutex> lock(mutex_);
    return condition_.wait_for(lock, chrono::milliseconds(msTimeout), [this](){return hasOccured_;});
}
#endif


/*****************************************************************************/
/*                                FUNCTIONS                                  */
/*****************************************************************************/

/*****************************************************************************/
//! \brief This function initializes the debug interface.
//! \param[in] sOpt Pointer to the default options
void StdUseCase_InterfaceInit(sDefOptions_t* sOpt)
{
    // set all status messages to STATUS_ERROR (convention)
    Ret_Initialize   = STATUS_ERROR;
    Ret_VCC          = STATUS_ERROR;
    Ret_Configure    = STATUS_ERROR;
    Ret_OpenDevice   = STATUS_ERROR;
    Ret_ReadFile     = STATUS_ERROR;
    Ret_ProgramFile  = STATUS_ERROR;
    Ret_VerifyFile   = STATUS_ERROR;
    Ret_Reset        = STATUS_ERROR;
    Ret_Registers    = STATUS_ERROR;
    Ret_FWUpdate     = STATUS_ERROR;
    Ret_GetHwVersion = STATUS_ERROR;
    Ret_GetFwVersion = STATUS_ERROR;
    Ret_Breakpoints  = STATUS_ERROR;
    Ret_Run          = STATUS_ERROR;
    Ret_Erase        = STATUS_ERROR;
    Ret_Device       = STATUS_ERROR;
    lVersion       = 0;
    lErrorNumber   = NO_ERR;

    string Interface(sOpt->pcInterface ? sOpt->pcInterface : "");
    int NumberOfUsbIfs = 0, nr = 0;

    if(inst_state & TARGETARCH_432)
    {
        try
        {
            if(MSP430_SetTargetArchitecture(MSP432_M4) != STATUS_OK)
            {
                Print_N_Log(ERR_MSG,"* Failed to set target architecture to MSP432!");
                inst_state |= ALT_EXIT;
                StdUseCase_Exit(EXIT_VARIOUS, 0);
            }
        }
        catch(exception)
        {
            Print_N_Log(ERR_MSG,"* This version of the MSP Debug Stack does not support MSP432!");
            inst_state |= ALT_EXIT;
            StdUseCase_Exit(EXIT_VARIOUS, 0);
        }
    }

    if(Interface == "DETECT")
    {
        Print_N_Log(REPORT_MSG,"* Executing FET detection sweep: ");
    }
    else
    {
        Print_N_Log(REPORT_MSG,"* Checking for available FET debuggers: ");
    }
    
    NumberOfUsbIfs = StdUseCase_GetUsbIfs();

    int i = 0;
    bool FoundUsbFET = false;
        
    // if a "broken" USB FET is connected, no other USB FETs can be detected
    if(string(UsbFETs[0].COMport) == "HID_FET")
    {
        Print_N_Log(STD_MSG,"* Corrupted USB FET firmware detected. Starting recovery.\n");

        if((MSP430_Initialize(UsbFETs[0].COMport, &lVersion) == STATUS_OK) && (lVersion == BROKEN_EZ_FET))
        {    
            // perform USB FET recovery
            if(MSP430_FET_FwUpdate(NULL, &MY_MSP430_CallbackFunc, myHandle) != STATUS_OK)
            {
                inst_state |= ALT_EXIT;
                StdUseCase_Exit(EXIT_FET_FWUPDATE, 0);
            }
            else
            {
                Print_N_Log(STD_MSG,"* USB FET recovery completed.\n");
            }
            // scan for all connected USB FETs again
            NumberOfUsbIfs = StdUseCase_GetUsbIfs();
        }
        else
        {
            inst_state |= ALT_EXIT;
            StdUseCase_Exit(EXIT_INITIALIZE, 0);
        }
    }

    // loop through detected USB interfaces
    for(i=0;i<NumberOfUsbIfs;i++)
    {
        if (UsbFETs[i].status != 1 && FoundUsbFET == false)
        {
            if((Interface == "USB")                        ||    // if no COM port specified
               (Interface == "TIUSB")                     ||    // ...
               (Interface == string(UsbFETs[i].COMport)))       // OR if USB FET was found on selected COM port
            {
                Print_N_Log(STD_MSG,"* Found USB FET @ %s <- Selected\n", UsbFETs[i].COMport);
                FoundUsbFET = true;
                nr = i;
            }
            else if(Interface == "DETECT")
            {
                int32_t tmpVersion = 0;
                char* VersionInfo = (char*)"";

                if(MSP430_Initialize(UsbFETs[i].COMport, &tmpVersion) != STATUS_OK)
                {
                    inst_state |= ALT_EXIT;
                    StdUseCase_Exit(EXIT_INITIALIZE, 0);
                }
                else
                {
                    StdUseCase_ReadHWVersion();
                }

                if(tmpVersion<0)
                {
                    VersionInfo = (char*)"(FW update needed)";
                }
                Print_N_Log(STD_MSG," - %d: %s @ %s %s\n", i+1, FETtype.data(), UsbFETs[i].COMport, VersionInfo);
            }
            if(MSP430_Close(0) != STATUS_OK)
            {
                Print_N_Log(ERR_MSG,"* ERROR: Failed to close connection to FET!\n");
                StdUseCase_Exit(EXIT_VARIOUS, 0);
            }
        }
        else if(UsbFETs[i].status == 1)
        {
            Print_N_Log(STD_MSG,"* Found USB FET @ %s <- Already in use\n", UsbFETs[i].COMport);
        }
        else
        {
            Print_N_Log(STD_MSG,"\n");
        }
    }

    if(Interface == "DETECT")
    {
        char input;
        int nr_input = 0;

        Print_N_Log(STD_MSG," - Select FET number: ");
        cin >> input;
        cin.ignore();
        nr_input = atoi(&input);
        Print_N_Log(LOG_MSG, "%d\n", nr_input);

        while(nr_input > NumberOfUsbIfs || nr_input <= 0)
        {
            Print_N_Log(STD_MSG," - Invalid entry! Select FET number: ");

            cin >> input;
            cin.ignore();
            nr_input = atoi(&input);
            Print_N_Log(LOG_MSG, "%d\n", nr_input);
        }
        nr = nr_input-1;
        Interface = UsbFETs[nr].COMport;
        FoundUsbFET = true;
    }

    if(FoundUsbFET == false)
    {
        Print_N_Log(ERR_MSG,"* Couldn't find available FET @ %s. Aborting.\n", UsbFETs[nr].COMport);
        inst_state |= ALT_EXIT;
        StdUseCase_Exit(EXIT_VARIOUS, 0);
    }

    Print_N_Log(REPORT_MSG,"* Initializing interface @ %s...", UsbFETs[nr].COMport);
    
    Ret_Initialize = MSP430_Initialize(UsbFETs[nr].COMport, &lVersion);
    if (Ret_Initialize == STATUS_ERROR)
    {
        inst_state |= ALT_EXIT;
        StdUseCase_Exit(EXIT_INITIALIZE, 0);
    }

    // set interface speed
    if(sOpt->pcFETspeed != NULL)
    {
        int speed = 0;
        char* mk_upper = NULL;

        Print_N_Log(REPORT_MSG,"* Setting FET speed...");
        for(mk_upper = sOpt->pcFETspeed; *mk_upper != '\0'; mk_upper++)    // converts argument to uppercase
            *mk_upper = toupper(*mk_upper);
        
        string FETspeed(sOpt->pcFETspeed ? sOpt->pcFETspeed : "");
        if(FETspeed == "FAST") { speed = 0; }
        else if (FETspeed == "MEDIUM") { speed = 1; }
        else if (FETspeed == "SLOW") { speed = 2; }

        if(MSP430_Configure(SET_INTERFACE_SPEED, speed) != STATUS_OK)
        {
            StdUseCase_Exit(EXIT_CONFIGURE, 0);
        }
    }
}

/*****************************************************************************/
//! \brief This function scans for all connected USB FETs and stores the
//! respective COM port and the status of the FET
//! \return int Number of found USB FETs
int StdUseCase_GetUsbIfs(void)
{
    int i = 0;
    int32_t NumberOfUsbIfs = 0;

    if(MSP430_GetNumberOfUsbIfs(&NumberOfUsbIfs) != STATUS_OK)
    {
        StdUseCase_Exit(EXIT_GETNUMBEROFUSBIFS, 0);
    }

    if(NumberOfUsbIfs == 0)
    {
        Print_N_Log(ERR_MSG,"* Couldn't find any connected USB FETs!\n");
        StdUseCase_Exit(EXIT_VARIOUS, 0);
    }

    for(i=0;i<NumberOfUsbIfs;i++)
    {
        if(MSP430_GetNameOfUsbIf(i, &UsbFETs[i].COMport, &UsbFETs[i].status) != STATUS_OK)
        {
            Print_N_Log(ERR_MSG,"* Error reading FET information!\n");
            StdUseCase_Exit(EXIT_GETNAMEOFUSBIF, 0);
        }
    }
    return NumberOfUsbIfs;
}

/*****************************************************************************/
//! \brief This function checks whether the MSP430.dll version number and the
//! version of the debugger firmware match. If they don't, an update of the
//! firmware can be performed.
//! \details By default, the user is prompted about the update but it can be
//! forced by using the -s switch.
//! \param[in] sOpt Pointer to the option struct
void StdUseCase_CheckFirmware(sDefOptions_t* sOpt)
{
    bool uptodate = true;

    Print_N_Log(REPORT_MSG,"* Checking firmware compatibility: ");

    if (lVersion == DLL_FW_MISMATCH || lVersion == V2_TO_V3_UPGRADE_REQUIRED)    // if conflict between firmware versions
    {
        uptodate = false;
        Print_N_Log(ERR_MSG,"* The firmware of your FET is outdated.\n");    // prompt user about an update

        if(inst_state & SKIP_UDPROMPT)
        {
            Ret_Initialize = STATUS_ERROR; // convention, set to error first

            Print_N_Log(REPORT_MSG,"* Skipped firmware update prompt.\n");
            if(MSP430_FET_FwUpdate(NULL, &MY_MSP430_CallbackFunc, myHandle) != STATUS_OK) // if update fails
            {
                inst_state |= ALT_EXIT;
                StdUseCase_Exit(EXIT_FET_FWUPDATE, 0);
            }
            if( (lVersion == V2_TO_V3_UPGRADE_REQUIRED) && (Ret_FWUpdate == STATUS_OK) ) // UIF is now enumerated as CDC device, so reinitialize
            {
                Print_N_Log(REPORT_MSG,"* FET has been updated to DLLv3. Reinitializing...\n");
                Sleep(10000);
            }

            Ret_Initialize = MSP430_Initialize(sOpt->pcInterface, &lVersion);
            if (Ret_Initialize != STATUS_OK)
            {
                inst_state |= ALT_EXIT;
                StdUseCase_Exit(EXIT_INITIALIZE, 0);
            }
            uptodate = true;
        }
        else
        {
            bool updProcRunning = true;
            Print_N_Log(ERR_MSG,"- Would you like to update it? (Y/N): ");

            while (updProcRunning)
            {
                char FW_Update_entry;                        // for storing runtime single key entries
                cin  >> FW_Update_entry;
                cin.ignore();
                Print_N_Log(LOG_MSG, "%c\n", FW_Update_entry);
                        
                if(strchr("Yy", FW_Update_entry) != NULL)
                {
                    while((Ret_FWUpdate = MSP430_FET_FwUpdate(NULL, &MY_MSP430_CallbackFunc, myHandle)) != STATUS_OK) // if update fails
                    {
                        Print_N_Log(ERR_MSG,"* Update failed. (R)etry/(C)ancel? ");  // prompt user
                                
                        while(1)
                        {
                            cin  >> FW_Update_entry;
                            cin.ignore();
                            Print_N_Log(LOG_MSG, "%c\n", FW_Update_entry);
                            if (strchr("CcRr", FW_Update_entry) != NULL) break;
                        }
                                
                        if        (strchr("Cc", FW_Update_entry) != NULL) break;
                        else if (strchr("Rr", FW_Update_entry) != NULL) continue;
                    }
                    if((lVersion == V2_TO_V3_UPGRADE_REQUIRED) && (Ret_FWUpdate == STATUS_OK)) // UIF is now enumerated as CDC device, so reinitialize
                    {
                        Print_N_Log(REPORT_MSG,"* FET has been updated to DLLv3. Reinitializing...\n");
                        Sleep(10000);
                    }
                    Ret_Initialize = MSP430_Initialize(sOpt->pcInterface, &lVersion);
                    uptodate = true;
                }
                else if(strchr("Nn", FW_Update_entry) != NULL) break;
                else continue;
                
                // in case of update failure or unsuccessful initialization, quit flasher
                if ((Ret_FWUpdate == STATUS_ERROR) || (Ret_Initialize == STATUS_ERROR))
                {
                    inst_state |= ALT_EXIT;
                    StdUseCase_Exit(EXIT_FET_FWUPDATE, 0);
                }
                // otherwise continue
                else
                {
                    updProcRunning = false;
                }
            }
        }
    }
    else
    {
        Ret_FWUpdate = STATUS_OK;
    }

    if(uptodate)
    {
        Print_N_Log(STD_MSG,"* FET firmware is up to date.\n");
    }
    else
    {
        Print_N_Log(STD_MSG,"* Warning: FW mismatch! Correct functionality not guaranteed!\n");
    }

    // abort execution here if NO_TARGET option is selected
    if(sOpt->pcDeviceName == "NO_TARGET")
    {
        MSP430_Close(0);
        inst_state |= ALT_EXIT;
        StdUseCase_Exit(EXIT_NO_ERROR, 0);
    }
}

/*****************************************************************************/
//! \brief This function reads out the debugger firmware version.
void StdUseCase_ReadFWVersion(void)
{
    Print_N_Log(REPORT_MSG,"* Reading FW version...");
            
    Ret_GetFwVersion = MSP430_FET_GetFwVersion(&FWVersion);
    lErrorNumber = MSP430_Error_Number();
    // if FET does not support Fw call
    if(lErrorNumber == INTERFACE_SUPPORT_ERR)
    {
        Ret_GetFwVersion = STATUS_OK;
    }
}

/*****************************************************************************/
//! \brief This function reads out the debugger hardware version.
void StdUseCase_ReadHWVersion(void)
{
    int32_t  HWVersionCount;
    char temp[3] = {0};
            
    Ret_GetHwVersion = MSP430_FET_GetHwVersion(&HWVersion,&HWVersionCount);
    lErrorNumber = MSP430_Error_Number();
    // if FET does not support Hw call
    if(lErrorNumber == INTERFACE_SUPPORT_ERR)
    {
        Ret_GetHwVersion = STATUS_OK;
        HWVersion = (uint8_t*)myLptVersion;
    }

    FETversion[0] = HWVersion[0];
    sprintf(temp, "%d", (int)HWVersion[3]);
    FETversion[1] = temp[0];
    FETversion[2] = 0x2E; // "."
    sprintf(temp, "%d", (int)HWVersion[2]);
    FETversion[3] = temp[0];
    FETversion[4] = temp[1];

    string HWversionString = string(FETversion);
    if(HWversionString == "E2.0") FETtype = "eZ430";
    else if(HWversionString == "E3.0") FETtype = "eZ-FET lite";
    else if(HWversionString == "E4.0") FETtype = "eZ-FET";
    else if(HWversionString == "U1.40") FETtype = "MSP-FET430UIF";
    else if(HWversionString == "U1.64") FETtype = "MSP-FET430UIF";
    else if(HWversionString == "U3.0") FETtype = "MSP-FET";
    else FETtype = "Unknown FET type";
}

/*****************************************************************************/
//! \brief This function sets the target voltage.
void StdUseCase_PowerUp(sDefOptions_t* sOpt)
{    
    if(sOpt->pcVCC > 3600 || sOpt->pcVCC < 1800)
    {
        if(sOpt->pcVCC != 0)
        {
            Print_N_Log(REPORT_MSG,"* Invalid voltage specified in exit specs. Using default (%d mV)...", TARGET_VCC);
        }
        else
        {
            Print_N_Log(REPORT_MSG,"* Setting VCC to %d mV...", TARGET_VCC);
        }

        Ret_VCC = MSP430_VCC(TARGET_VCC);
    }
    else
    {
        int VCC = 0;

        if(strcmp(FETversion, "E2.0") == 0 || strcmp(FETversion, "E3.0") == 0 || strcmp(FETversion, "E4.0") == 0)
        {
            Print_N_Log(STD_MSG,"* Debugger does not support target voltages other than %d mV!\n", TARGET_VCC);
            VCC = TARGET_VCC;
        }
        else
        {
            VCC = sOpt->pcVCC;
        }

        Print_N_Log(REPORT_MSG,"* Setting VCC to %d mV...", VCC);            
        Ret_VCC = MSP430_VCC(VCC);
    }

    lErrorNumber = MSP430_Error_Number();

    if(Ret_VCC != STATUS_OK)
    {
        // check if target is powered by external voltage source
        if(lErrorNumber == EX_POWER_OK)
        {
            Print_N_Log(REPORT_MSG,"* Target is powered externally.");
        }
        else
        {
            StdUseCase_Exit(EXIT_VCC, lErrorNumber);
        }
    }
}

/*****************************************************************************/
//! \brief Erase the target device's memory including active memory protection
//! \param[in] sOpt Pointer to the default options
void StdUseCase_SpecialErase(sDefOptions_t* sOpt)
{
    // Execute "total erase" to remove active memory protection before 
    // attempting to open device (for FR5xx/FR6xx devices - except FR57xx)
    if(strcmp(sOpt->pcEraseType, "ERASE_TOTAL") == 0)
    {
        Print_N_Log(REPORT_MSG,"* Performing total erase...");
        Ret_Erase = MSP430_Erase(ERASE_TOTAL, 0x0, 0x0);
        if(Ret_Erase != STATUS_OK)
        {
            StdUseCase_Exit(EXIT_ERASE, 0);
        }
    }
    // Execute "user code erase" (for FR4xx devices)
    if(strcmp(sOpt->pcEraseType, "ERASE_USER_CODE") == 0)
    {
        Print_N_Log(REPORT_MSG,"* Performing user code erase...");
        Ret_Erase = MSP430_Erase(ERASE_USER_CODE, 0x0, 0x0);
        if(Ret_Erase != STATUS_OK)
        {
            StdUseCase_Exit(EXIT_ERASE, 0);
        }
    }
}

/*****************************************************************************/
//! \brief Access device and read device information
//! \param[in] sOpt Pointer to the default options
void StdUseCase_GetDevice(sDefOptions_t* sOpt)
{
    char c = 0;
    int k = 0;
    int targetID = DEVICE_UNKNOWN;

    if(sOpt->pcDeviceName != "" && sOpt->pcDeviceName != "Unknown")
    {
        // if there are any "wildcard"-characters in the device name 
        // (e.g. MSP430G2xx1), make sure they are in lower case
        replace(sOpt->pcDeviceName.begin(), sOpt->pcDeviceName.end(), 'X', 'x');

        if(inst_state & NO_RESET)
        {
            DEVICE_T tempDeviceInfo = {};
            int locDeviceID = 0;

            do
            {
                Ret_Device = MSP430_Device(locDeviceID, (uint8_t*)&tempDeviceInfo.endian, sizeof(tempDeviceInfo.buffer));
                if((char*)tempDeviceInfo.string == sOpt->pcDeviceName)
                {
                    targetID = locDeviceID;
                    break;
                }
                locDeviceID++;
            }
            while(Ret_Device == STATUS_OK);

            if(Ret_Device != STATUS_OK)
            {
                Print_N_Log(ERR_MSG,"* ERROR: Target \"%s\" not found. Ignoring -a switch!\n", sOpt->pcDeviceName.c_str());
                inst_state &= ~NO_RESET;
            }
            else
            {
                Print_N_Log(STD_MSG,"* Attempting non-intrusive access to target (no reset on start-up).\n");
            }
        }
    }
    else
    {
        if(inst_state & NO_RESET)
        {
            Print_N_Log(ERR_MSG,"* ERROR: Target device name not specified. Ignoring -a switch!\n");
            inst_state &= ~NO_RESET;
        }
    }

    Print_N_Log(REPORT_MSG,"* Accessing device...");

    Ret_OpenDevice = MSP430_OpenDevice(const_cast<char*>(sOpt->pcDeviceName.c_str()),sOpt->pcPassword,sOpt->pcPWlen,g_Def.deviceAC,targetID);                                 
    if(Ret_OpenDevice == STATUS_OK)
    {
        // if access without reset (attach to running target) was requested...
        if(inst_state & NO_RESET)
        {
            int32_t state = 0;
            // halt potentially running target device
            if(MSP430_State(&state, 1, 0) != STATUS_OK)
            {
                StdUseCase_Exit(EXIT_STATE_STOP, 0);
            }
        }

        // if device was opened successfully, read out device information
        Print_N_Log(REPORT_MSG,"* Reading device information...");
        Ret_OpenDevice = MSP430_GetFoundDevice( (uint8_t*)&g_tTargetDevice.endian, sizeof(g_tTargetDevice.buffer));
        if(Ret_OpenDevice != STATUS_OK)
        {
            StdUseCase_Exit(EXIT_OPENDEVICE, 0);
        }
    }
    else
    {
        StdUseCase_Exit(EXIT_OPENDEVICE, 0);
    }
    // if -n switch is used
    if(sOpt->pcDeviceName != "Unknown")
    {
        // check if selected and found device matches
        if(sOpt->pcDeviceName != (char*)g_tTargetDevice.string)
        {
            // in case of wildcard characters in the FETSTRING (MSPDS-internal device name)
            if(strstr("x", (char*)g_tTargetDevice.string) != NULL)
            {
                Print_N_Log(ERR_MSG,"* Warning: Found device might not match -n selection:\n");
            }
            else
            {
                Print_N_Log(ERR_MSG,"* Warning: Found device does not match -n selection:\n");
            }
            Print_N_Log(ERR_MSG,"- Selected: %s\n", sOpt->pcDeviceName.c_str());
            Print_N_Log(ERR_MSG,"- Found:    %s\n", g_tTargetDevice.string);
            Print_N_Log(ERR_MSG,"- Continue? (Y/N): ");

            char Check_entry;

            cin  >> Check_entry;
            cin.ignore();
            Print_N_Log(LOG_MSG, "%c\n", Check_entry);
                        
            if(strchr("Yy", Check_entry) == NULL)
            {
                StdUseCase_Exit(EXIT_VARIOUS, 0);
            }
        }
    }
}

/*****************************************************************************/
//! \brief This function displays the results of the Flasher execution. It is
//! also called whenever the execution is aborted due to an error. 
//! \param[in] int argc: number of arguments in the argument string
//! \param[in] char* argv[]: pointer to the argument string
//! \param[in] sDefOptions_t* sOpt: pointer to the default options    
void StdUseCase_DisplayResults(int argc, char* argv[], sDefOptions_t* sOpt)
{
    STATUS_T Ret;

    Ret = Ret_Initialize | Ret_GetFwVersion | Ret_GetHwVersion | Ret_VCC | Ret_OpenDevice | Ret_ReadFile | Ret_ProgramFile | Ret_Reset;

    Print_N_Log(REPORT_MSG, "* ");

    Print_N_Log(STD_MSG,"* ----------------------------------------------------------------------------\n");
    Print_N_Log(STD_MSG,"* Arguments   : ");
    for(int32_t p = 1; p < argc; p++)
    {
        Print_N_Log(ONE_LINE_MSG, "%s ",argv[p]);
    }
    Print_N_Log(ONE_LINE_MSG, "\n");
    Print_N_Log(STD_MSG,"* ----------------------------------------------------------------------------\n");

    if(Ret == STATUS_OK)
    {
        Print_N_Log(STD_MSG,"* Driver      : loaded\n");
        Print_N_Log(STD_MSG,"* Dll Version : %d\n", lVersion);
        Print_N_Log(STD_MSG,"* FwVersion   : %d\n", FWVersion);
        Print_N_Log(STD_MSG,"* Interface   : %s\n", sOpt->pcInterface);
        Print_N_Log(STD_MSG,"* HwVersion   : %c %d.%d\n",HWVersion[0], HWVersion[3], HWVersion[2]);
        Print_N_Log(STD_MSG,"* JTAG Mode   : AUTO\n");
        Print_N_Log(STD_MSG,"* Device      : %s\n", g_tTargetDevice.string);
        Print_N_Log(STD_MSG,"* EEM         : Level %d, ClockCntrl %d\n", g_tTargetDevice.emulation, g_tTargetDevice.clockControl);
                
        if(sOpt->pcReadFile != NULL)
        {
            Print_N_Log(STD_MSG,"* Read File   : %s (memory segment = %s)\n", sOpt->pcReadFile, sOpt->pcReadMem);
        }

        if(sOpt->pcFile != NULL || inst_state & ERASE_NO_WRITE)
        {
            Print_N_Log(STD_MSG,"* Erase Mode  : %s\n", sOpt->pcEraseType);
        }

        if(sOpt->pcFile != NULL)
        {
            Print_N_Log(STD_MSG,"* Prog.File   : %s\n", sOpt->pcFile);
            if(!strcmp(sOpt->pcVerify, "TRUE"))
            {
                Print_N_Log(STD_MSG,"* Verified    : TRUE\n");
            }
            else if(!strcmp(sOpt->pcVerify, "FALSE"))
            {
                Print_N_Log(STD_MSG,"* Verified    : FALSE\n");
            }
            else
            {
                Print_N_Log(STD_MSG,"* Verify File : %s\n", sOpt->pcVerify);
            }
            // if BSL memory should be unlocked
            if(inst_state & WRITE_BSL)
            {
                Print_N_Log(STD_MSG,"* BSL Unlock  : TRUE\n");
            }
            else
            {
                Print_N_Log(STD_MSG,"* BSL Unlock  : FALSE\n");
            }

            // if INFO A memory should be unlocked                    
            if(inst_state & UNLOCK_FLASH)
            {
                Print_N_Log(STD_MSG,"* InfoA Access: TRUE\n");
            }
            else
            {
                Print_N_Log(STD_MSG,"* InfoA Access: FALSE\n");
            }
        }
        else if(strcmp(sOpt->pcVerify, "FALSE") && strcmp(sOpt->pcVerify, "TRUE"))
        {
            //in case of stand-alone verify
            Print_N_Log(STD_MSG,"* Verify File : %s\n", sOpt->pcVerify);
        }

        if(sOpt->pcVCC == 0)
        {
            Print_N_Log(STD_MSG,"* VCC OFF\n");
        }
        else
        {
            if(strcmp(FETversion, "E2.0") == 0 || strcmp(FETversion, "E3.0") == 0 || strcmp(FETversion, "E4.0") == 0)
            {
                Print_N_Log(STD_MSG,"* VCC ON      : 3000 mV\n");
            }
            else
            {
                Print_N_Log(STD_MSG,"* VCC ON      : %d mV\n", sOpt->pcVCC);
            }
        }

        if(sOpt->pcBreakpoints != NULL)
        {
            // only if device secure is not requested
            if(!(inst_state & BLOW_FUSE))
            {
                Print_N_Log(STD_MSG,"* Breakpoints : %d set\n", BPcount);
                if(inst_state & BP_LIMIT)
                {
                    Print_N_Log(STD_MSG,"* ATTENTION: Device supports %d Breakpoints. Further Breakpoints ignored.\n", g_tTargetDevice.nBreakpoints);
                }
            }
            else
            {
                Print_N_Log(STD_MSG, "* Breakpoints : Disabled due to secure device (-f switch)\n");
            }
        }
        Print_N_Log(STD_MSG,"* ----------------------------------------------------------------------------\n");
    }
    else
    {
        Print_N_Log(ERR_MSG,"  %d,%d,%d,%d,%d,%d,%d,%d\n", Ret_Initialize , Ret_GetFwVersion , Ret_GetHwVersion , Ret_VCC , Ret_OpenDevice , Ret_ReadFile, Ret_ProgramFile , Ret_Reset);
        Print_N_Log(ERR_MSG,"* ERROR : %s\n", MSP430_Error_String(lErrorNumber));
        StdUseCase_Exit(EXIT_VARIOUS, lErrorNumber);
    }
}

/*****************************************************************************/
//! \brief Erase MSP430 memory. This function is only used in case an erase
//! should be performed without writing to the memory afterwards.
//! \param[in] sOpt Pointer to the option struct
void StdUseCase_Erase(sDefOptions_t* sOpt)
{
    // if memory should be erased without writing afterwards
    if(sOpt->pcFile == NULL && (inst_state & ERASE_NO_WRITE) && strcmp(sOpt->pcEraseType, "ERASE_TOTAL") && strcmp(sOpt->pcEraseType, "ERASE_USER_CODE"))
    {
        int32_t lwStart;
        int32_t lwLength;
        int32_t lEraseType = ERASE_ALL;
        string EraseType(sOpt->pcEraseType ? sOpt->pcEraseType : "");

        if(EraseType == "ERASE_SEGMENT")
        {
            Print_N_Log(STD_MSG,"* Warning: -e ERASE_SEGMENT must be used with -w switch. Command will be ignored.");
        }
        else
        {
            string DeviceName = sOpt->pcDeviceName;

            if(DeviceName == "MSP430L092")
            {
                inst_state |= MODE_SELECT;
                sOpt->pcEraseType = "ERASE_MAIN";
            }
            else if(DeviceName == "MSP430C092")
            {
                sOpt->pcEraseType = "ERASE_MAIN";
            }
            if(EraseType == "ERASE_MAIN")
            {
                lEraseType = ERASE_MAIN;
            }

            if(inst_state & TARGETARCH_432)
            {
                lwStart = g_tTargetDevice.main32BitStart;
                lwLength = g_tTargetDevice.main32BitEnd - g_tTargetDevice.main32BitStart + 1;
            }
            else
            {
                lwStart = g_tTargetDevice.mainStart;
                lwLength = g_tTargetDevice.mainEnd - g_tTargetDevice.mainStart + 1;
            }

            Print_N_Log(REPORT_MSG,"* Erasing device memory...");

            if(MSP430_Erase(lEraseType, lwStart, lwLength) != STATUS_OK)
            {
                Print_N_Log(ERR_MSG,"* Erase operation failed. Device memory was not erased correctly!\n");
                StdUseCase_Exit(EXIT_ERASE, 0);
            }
        }
    }
}

/*****************************************************************************/
//! \brief Read and/or write MSP430 memory, verify file transfer
//! \param[in] sOpt Pointer to the option struct
//! \param[in] RTE Pointer to the struct for runtime entries
void StdUseCase_ReadWrite(sDefOptions_t* sOpt, sRTE_t* RTE)
{
    string DeviceName = sOpt->pcDeviceName;
    string EraseType(sOpt->pcEraseType ? sOpt->pcEraseType : "");
    string ReadFile(sOpt->pcReadFile ? sOpt->pcReadFile : "");
    string ReadMem(sOpt->pcReadMem ? sOpt->pcReadMem : "");
    ReadMem.erase(std::remove(ReadMem.begin(), ReadMem.end(), ' '), ReadMem.end());

    if(ReadFile != "")        // if MSP430 memory should be dumped
    {
        int32_t lwStart;
        int32_t lwLength;
                        
        if(ReadMem == "RAM")
        {
            if(inst_state & TARGETARCH_432)
            {
                lwStart = g_tTargetDevice.ram32BitStart;
                lwLength = g_tTargetDevice.ram32BitEnd - g_tTargetDevice.ram32BitStart + 1;
            }
            else
            {
                lwStart = g_tTargetDevice.ramStart;
                lwLength = g_tTargetDevice.ramEnd - g_tTargetDevice.ramStart + 1;
            }
        }
        else if(ReadMem == "INFO")
        {
            if(inst_state & TARGETARCH_432)
            {
                lwStart = g_tTargetDevice.info32BitStart;
                lwLength = g_tTargetDevice.info32BitEnd - g_tTargetDevice.info32BitStart + 1;
            }
            else
            {
                lwStart = g_tTargetDevice.infoStart;
                lwLength = g_tTargetDevice.infoEnd - g_tTargetDevice.infoStart + 1;
            }              
            Print_N_Log(REPORT_MSG, "* Unlocking Info Memory A...");
            Ret_Configure = MSP430_Configure(LOCKED_FLASH_ACCESS, ENABLE);
            if(Ret_Configure == STATUS_ERROR)
            {
                StdUseCase_Exit(EXIT_CONFIGURE, 0);
            }
        }
        else if(ReadMem == "MAIN")
        {
            if(inst_state & TARGETARCH_432)
            {
                lwStart = g_tTargetDevice.main32BitStart;
                lwLength = g_tTargetDevice.main32BitEnd - g_tTargetDevice.main32BitStart + 1;
            }
            else
            {
                lwStart = g_tTargetDevice.mainStart;
                lwLength = g_tTargetDevice.mainEnd - g_tTargetDevice.mainStart + 1;
            }
        }
        else if(ReadMem == "BSL")
        {
            if(inst_state & TARGETARCH_432)
            {
                lwStart = g_tTargetDevice.bsl32BitStart;
                lwLength = g_tTargetDevice.bsl32BitEnd - g_tTargetDevice.bsl32BitStart + 1;
            }
            else
            {
                lwStart = g_tTargetDevice.bslStart;
                lwLength = g_tTargetDevice.bslEnd - g_tTargetDevice.bslStart + 1;
            }
            Print_N_Log(REPORT_MSG, "* Unlocking BSL memory...");
            Ret_Configure = MSP430_Configure(UNLOCK_BSL_MODE, 1);
            if(Ret_Configure == STATUS_ERROR)
            {
                StdUseCase_Exit(EXIT_CONFIGURE, 0);
            }
        }
        else if(ReadMem.find("0X") != string::npos)
        {
            size_t del_pos = ReadMem.find("-");
            lwStart = (int32_t)strtol(ReadMem.substr(0, del_pos).c_str(), NULL, 16);
            /*if (lwStart % 2)
            {
                lwStart += 1;
                Print_N_Log(ERR_MSG, "* Warning : Uneven start address. Using 0x%x instead.\n", lwStart);
            }*/

            lwLength = (int32_t)strtol(ReadMem.substr(del_pos+1, string::npos).c_str(), NULL, 16) - lwStart + 1;
            if (lwLength <= 0)
            {
                Print_N_Log(ERR_MSG, "* ERROR : Invalid memory boundaries specified.\n");
                StdUseCase_Exit(EXIT_READOUTFILE, 0);
            }      
        }

        Print_N_Log(REPORT_MSG,"* Dumping memory from %s into %s...", sOpt->pcReadMem, sOpt->pcReadFile);
        Ret_ReadFile = MSP430_ReadOutFile(lwStart, lwLength, sOpt->pcReadFile, RTE->file_type);
        if(Ret_ReadFile != STATUS_OK)
        {
            Print_N_Log(ERR_MSG, "* ERROR : Unable to read from memory.\n");
            if (lwLength <= 0)
            StdUseCase_Exit(EXIT_READOUTFILE, 0);
        } 
    }
    else
    {
        Ret_ReadFile = STATUS_OK;
    }

    // if MSP430 memory should be programmed
    if(Ret_OpenDevice == STATUS_OK && sOpt->pcFile != NULL)
    {
        // if BSL write is enabled
        if(Ret_OpenDevice == STATUS_OK && inst_state & WRITE_BSL)
        {                  
            Print_N_Log(REPORT_MSG, "* Unlocking BSL memory...");
            Ret_Configure = MSP430_Configure(UNLOCK_BSL_MODE, ENABLE);
            if(Ret_Configure == STATUS_ERROR)
            {
                StdUseCase_Exit(EXIT_CONFIGURE, 0);
            }
        }

        // if locked flash access is selected
        if(Ret_OpenDevice == STATUS_OK && inst_state & UNLOCK_FLASH)
        {
            Print_N_Log(REPORT_MSG, "* Unlocking Info Memory A...");
            Ret_Configure = MSP430_Configure(LOCKED_FLASH_ACCESS, ENABLE);
            if(Ret_Configure == STATUS_ERROR)
            {
                StdUseCase_Exit(EXIT_CONFIGURE, 0);
            }
        }
                        
        int32_t lEraseType = ERASE_ALL;

        // evaluate erase type
        // only the L092 requires writing to external memory
        if(DeviceName == "MSP430L092")
        {
            inst_state |= MODE_SELECT;
            sOpt->pcEraseType = "ERASE_MAIN";
        }
        else
        if(DeviceName == "MSP430C092")
        {
            sOpt->pcEraseType = "ERASE_MAIN";
        }
        if(EraseType == "ERASE_MAIN")
        {
            lEraseType = ERASE_MAIN;
        }
        if(EraseType == "NO_ERASE")
        {
            lEraseType = 0; // = ERASE_SEGMENT (will be ignored by MSP430_ProgramFile() -> no erase
        }
        if(EraseType == "ERASE_SEGMENT")
        {
            int32_t lStartAddress = 0;
            int32_t lProgramLength = 0;
            
            lEraseType = ERASE_SEGMENT;
            if(ParseInputFile(sOpt->pcFile, &lStartAddress, &lProgramLength) != STATUS_OK)
            {
                StdUseCase_Exit(EXIT_VARIOUS, 26); // ErrorNumber 26 = "File I/O Error"
            }

            if(MSP430_Erase(lEraseType, lStartAddress, lProgramLength) != STATUS_OK)
            {
                Print_N_Log(ERR_MSG,"* Erase check failed. Memory segment was not erased correctly!\n");
                StdUseCase_Exit(EXIT_ERASE, 0);
            }
            
            if(MSP430_EraseCheck(lStartAddress, lProgramLength) != STATUS_OK)
            {
                Print_N_Log(ERR_MSG,"* Erase check failed. Memory segment was not erased correctly!\n");
                StdUseCase_Exit(EXIT_ERASE, 0);
            }
        }

        Print_N_Log(REPORT_MSG,"* Loading file into device...");

        // program target code into MSP430 memory without verify
        Ret_ProgramFile = MSP430_ProgramFile(sOpt->pcFile, lEraseType, false);

        if(Ret_ProgramFile != STATUS_OK)
        {
            StdUseCase_Exit(EXIT_PROGRAMFILE, 0);
        }
    }
    else
    {
        Ret_ProgramFile = STATUS_OK;  // <- simply set status to OK
    }

    // verify data transfer
    if(strcmp(sOpt->pcVerify, "FALSE"))
    {
        if((!strcmp(sOpt->pcVerify, "TRUE")) && (sOpt->pcFile != NULL))
        {
            Print_N_Log(REPORT_MSG,"* Verifying memory (%s)...", sOpt->pcFile);
            Ret_VerifyFile = MSP430_VerifyFile(sOpt->pcFile);
        }
        else if(strcmp(sOpt->pcVerify, "TRUE"))  // separate file for verify
        {
            Print_N_Log(REPORT_MSG,"* Verifying memory (%s)...", sOpt->pcVerify);
            Ret_VerifyFile = MSP430_VerifyFile(sOpt->pcVerify);
        }
        
        if(Ret_VerifyFile != STATUS_OK)
        {
            StdUseCase_Exit(EXIT_VERIFYFILE, 0);
        }
    }

    // L092 handling
    if ((inst_state & WRITE_EXT_MEM)      &&  // if data should be written to external memory
        (sOpt->pcFile != NULL)            &&  // ...and data was transferred into RAM
        (Ret_ProgramFile != STATUS_ERROR))    // ...successfully
    {
        Print_N_Log(REPORT_MSG,"* Writing to external memory...");
        Ret_Configure = MSP430_Configure(WRITE_EXTERNAL_MEMORY, ENABLE);        // write to external memory
        if(Ret_Configure == STATUS_ERROR)
        {
            StdUseCase_Exit(EXIT_CONFIGURE, 0);
        }
    }
}

/*****************************************************************************/
//! \brief If triggered by -f switch, this function secures the device's JTAG
//! interface
void StdUseCase_BlowFuse(void)
{
    if(inst_state & BLOW_FUSE)
    {
        Print_N_Log(REPORT_MSG,"* Securing target device...");

        if(MSP430_Secure() != STATUS_OK)
        {
            Print_N_Log(ERR_MSG,"* Target device could not be secured!\n");
            StdUseCase_Exit(EXIT_VARIOUS, 0);
        }
    }
}

/*****************************************************************************/
//! \brief This function sets hardware breakpoints
//! \details If the number of entered breakpoint addresses is greater than the 
//! number of hardware breakpoints supported by the device, the additional 
//! breakpoints are ignored.
//! \details StdUseCase_EemInit() must be called prior to this funcion.
//! \param[in] AddrArray Pointer to an array of the designated breakpoint addresses
void StdUseCase_SetBreakpoints(int32_t *AddrArray)
{ 
    if(inst_state & BLOW_FUSE)
    {
        Print_N_Log(STD_MSG,"* Breakpoints disabled. Target device secure requested.");
    }
    else
    {
        BpParameter_t pBpBuffer;

        uint16_t  pwBpHandle = 0;    // breakpoint handle, initialized with '0'
        int32_t BPLocal[50] = {0};
  
        signed int Addrcount = 0;
        int invalid = 0;
        int tempcount = 0;
        int localcount;
        BOOL BPflag = false;
        BOOL Addrflag = false;

        Ret_Breakpoints = STATUS_ERROR;

        pBpBuffer.bpMode = BP_CODE;
        pBpBuffer.bpType = BP_MAB;
        pBpBuffer.bpAccess = BP_FETCH;
        pBpBuffer.bpAction = BP_BRK;
        pBpBuffer.bpOperat = BP_EQUAL;
        pBpBuffer.lMask = 0xFFFFF;

        Print_N_Log(REPORT_MSG,"* Setting Breakpoints...");

        if(AddrArray[Addrcount] != 0)
        {
            for(localcount = 0; AddrArray[localcount] != 0; localcount++)
            {
                BPLocal[localcount] = AddrArray[localcount];
            }
            BPLocal[localcount] = 0;
            BPflag = true;
        }

        while(BPLocal[Addrcount] != 0)
        {
            if(Addrcount <= g_tTargetDevice.nBreakpoints)
            {
                pwBpHandle = 0;
                if((BPLocal[Addrcount] < g_tTargetDevice.mainStart) || (BPLocal[Addrcount] > (long)g_tTargetDevice.mainEnd))
                {
                    tempcount = Addrcount;
                    Print_N_Log(STD_MSG, "* Error creating breakpoint @ 0x%x: invalid address!\n", BPLocal[Addrcount]);
                    if(BPLocal[Addrcount+1] == 0)
                    {
                        BPLocal[Addrcount] = 0;
                    }
                    while(BPLocal[tempcount] != 0)
                    {
                        BPLocal[tempcount] = BPLocal[tempcount+1];
                        tempcount++;
                    }
                    invalid++;
                    Addrflag = true;
                }
                else
                {
                    pBpBuffer.lAddrVal = BPLocal[Addrcount];
                    MSP430_EEM_SetBreakpoint(&pwBpHandle,&pBpBuffer);
                }
            }
            if(Addrcount == g_tTargetDevice.nBreakpoints-1)
            {
                inst_state |= BP_LIMIT;
                BPcount = g_tTargetDevice.nBreakpoints;
            }
            if(!Addrflag)
                Addrcount++;
            Addrflag = false;
        }
        BPcount -= invalid;
        if(BPflag)
        {
            for(localcount = 0; BPLocal[localcount] != 0; localcount++)
            {
                AddrArray[localcount] = BPLocal[localcount];
            }
        }
    }
}

/*****************************************************************************/
//! \brief Execute the program in the target memory
//! \details If one or more breakpoints are set, StdUseCase_RunProgram will run 
//! the program, reading and displaying the CPU registers each time a
//! breakpoint is hit.
//! \details Once a breakpoint is hit, it is cleared.
//! \details StdUseCase_EemInit() must be called prior to this funcion.
//! \param[in] AddrArray Pointer to the array containing the breakpoint addresses
//! \param[in] sOpt Pointer to the default options
void StdUseCase_RunProgram(sDefOptions_t* sOpt, int32_t* AddrArray)
{
    STATUS_T RetState;

    // if breakpoints are set and device secure is not requested
    if(sOpt->pcBreakpoints != NULL && !(inst_state & BLOW_FUSE))
    {
        int BPnumber = 1;
        int runcount = 1;

        while(runcount <= BPcount)
        {
            Print_N_Log(STD_MSG,"* Running Program in RUN_TO_BREAKPOINT mode...\n");

            StdUseCase_Run();

            if(StdUseCase_WaitForEvent(WM_BpMESSAGE, strtol(sOpt->pcTimeout, NULL, 10)))
            {
                RetState = MSP430_Registers(g_lTargetDeviceRegisters, ALL_REGS, READ);  //update registers
                if (RetState != STATUS_OK)
                {
                    Print_N_Log(ERR_MSG," ERROR: MSP430_Registers(..., ALL_REGS, READ)\n");
                    StdUseCase_Exit(EXIT_REGISTER_READ, 0);
                }
    
                while((g_lTargetDeviceRegisters[PC] != AddrArray[BPnumber-1]))  //determine breakpoint number
                {                                                                
                    BPnumber++;
                    if(BPnumber > BPcount)
                    {    
                        BPnumber = 1;
                    }
                }
                Print_N_Log(STD_MSG,"* Hit breakpoint #%d @ 0x%x...\n",BPnumber, g_lTargetDeviceRegisters[PC]);
                
                StdUseCase_ClearBreakpoint(&AddrArray[0], BPnumber);

                StdUseCase_GetCpuState();
            }
            runcount++;
        }
    }
}

/*****************************************************************************/
//! \brief This function clears a single hardware breakpoint.
//! \details StdUseCase_EemInit() must be called prior to this funcion.
//! \param[in] AddrArray Pointer to the array containing the breakpoint addresses
//! \param[in] number The number of the breakpoint to be cleared. 
//! -Used to determine its address in the AddrArray[]
//! -Also serves as breakpoint handle for the MSP430_EEM_SetBreakpoint fuction
void StdUseCase_ClearBreakpoint(int32_t *AddrArray, int number)
{
    Ret_Breakpoints = STATUS_ERROR;
    
    BpParameter_t    pBpBuffer;
    uint16_t  pwBpHandle = 0;            // breakpoint handle, initialized with '0'

    Print_N_Log(STD_MSG,"* Clearing Breakpoint #%d...", number);

    pBpBuffer.bpMode = BP_CLEAR;
    pBpBuffer.lAddrVal = AddrArray[number-1];
    pwBpHandle = (int16_t)(number);

    Ret_Breakpoints = MSP430_EEM_SetBreakpoint(&pwBpHandle,&pBpBuffer);
    if(Ret_Breakpoints != STATUS_OK)
    {
        StdUseCase_Exit(EXIT_EEM_SETBREAKPOINT, 0);
    }
}

/*****************************************************************************/
/*****************************************************************************/
/**
 * Enhanced Emulation Module (Debug)
 * Help functions for using the on-chip emulation logic EEM
 */
/*****************************************************************************/
/*****************************************************************************/

int32_t   bpEventFlags = 0;
Event MyEvent;
Event MyBpEvent;
Event MySsEvent;
Event MyStoEvent;
Event MySStateEvent;
Event MyCPUStopEvent;

int32_t   ValueParam;
uint32_t   MyGlob_MsgId;
uint32_t MyGlob_wParam;
int32_t MyGlob_lParam;

/*****************************************************************************/
//! \brief Notify callback function for MSP430.dll PostMessages
//! \param[in] MsgId Message ID
//! \param[in] wParam Generic parameter 1
//! \param[in] lParam Generic parameter 2
//! \param[in] clientHandle Callback handle
void MY_DLL430_CallbackFunc(uint32_t MsgId, uint32_t wParam, int32_t lParam, int32_t clientHandle)
{
    ValueParam = lParam;
    MyGlob_MsgId = MsgId;
    MyGlob_wParam = wParam;
    MyGlob_lParam = lParam;

    switch(MsgId)
    {
    case WM_BpMESSAGE:
        bpEventFlags |= fBpMESSAGE;
        Print_N_Log(STD_MSG," MESSAGE: Breakpoint hit...\n");
        MyBpEvent.set();
        MyEvent.set();
        break;
    case WM_SsMESSAGE:
        bpEventFlags |= fSsMESSAGE;
        Print_N_Log(STD_MSG," MESSAGE: Single step complete...\n");
        MySsEvent.set();
        MyEvent.set();
        break;
    case WM_StoMESSAGE:
        bpEventFlags |= fStoMESSAGE;
        Print_N_Log(STD_MSG," MESSAGE: Storage on trace buffer...\n");
        StdUseCase_EemReadStateStorageBuffer(wParam);
        MyStoEvent.set();
        MyEvent.set();
        break;
    case WM_SStateMESSAGE:
        bpEventFlags |= fSStateMESSAGE;
        Print_N_Log(STD_MSG," MESSAGE: Change in new state of the sequencer...\n");
        MySStateEvent.set();
        MyEvent.set();
        break;
    case WM_WarMESSAGE:
        bpEventFlags |= fWarMESSAGE;
        Print_N_Log(STD_MSG," WARNING: Postmessage warning from DLL...\n");
        break;
    case WM_CPUStopMESSAGE:
        bpEventFlags |= fCPUStopMESSAGE;
        Print_N_Log(STD_MSG," MESSAGE: Device CPU stopped...\n");
        MyCPUStopEvent.set();
        MyEvent.set();
        break;
    default:
        break;
    }
}

/*****************************************************************************/
//! \brief Initialize the Enhanced Emulation Module (EEM) 
void StdUseCase_EemInit(void)
{
    STATUS_T    RetState = STATUS_ERROR;
    MessageID_t MsgId;

    MsgId.uiMsgIdBreakpoint = WM_BpMESSAGE;
    MsgId.uiMsgIdSingleStep = WM_SsMESSAGE;
    MsgId.uiMsgIdStorage    = WM_StoMESSAGE;
    MsgId.uiMsgIdState      = WM_SStateMESSAGE;
    MsgId.uiMsgIdWarning    = WM_WarMESSAGE;
    MsgId.uiMsgIdCPUStopped = WM_CPUStopMESSAGE;
    
    // initialize EEM, assign callback function, handlernumber and msgstruct
    RetState = MSP430_EEM_Init(MY_DLL430_CallbackFunc, 0, &MsgId);
    if (RetState == STATUS_OK)
    {
        Print_N_Log(REPORT_MSG,"* Initializing EEM...");
    }
    else
    {
        StdUseCase_Exit(EXIT_EEM_INIT, 0);
    }
}

/*****************************************************************************/
//! \brief Start the program execution
void StdUseCase_Run(void)
{
    Ret_Run = MSP430_Run(RUN_TO_BREAKPOINT, false);
    if (Ret_Run != STATUS_OK)
    {
        StdUseCase_Exit(EXIT_RUN, 0);
    }
}

/*****************************************************************************/
//! \brief Reset the device using the specified method(s)
//! \param[in] method The bit mask specifying the method(s) to use to reset the 
//! device:
//! - PUC_RESET: The device is reset using PUC (i.e., a "soft" reset).
//! - RST_RESET: The device is reset using the RST/NMI pin (i.e., a "hard" reset).
//! - FORCE_PUC_RESET
//! - FORCE_RST_RESET
//! \param[in] execute Start device execution (when TRUE).
//! \param[in] releaseJTAG Release the JTAG control signals (when TRUE). 
//! 'execute' must be TRUE.
void StdUseCase_Reset(int32_t method, int32_t execute, int32_t releaseJTAG)
{
    if(!(inst_state & NO_RESET))
    {
        Ret_Reset = MSP430_Reset(method, execute, releaseJTAG);
        if (Ret_Reset != STATUS_OK)
        {
            StdUseCase_Exit(EXIT_RUN, 0);
        }
    }
    else
    {
        Ret_Reset = STATUS_OK;
    }
}

/*****************************************************************************/
//! \brief This function reads all CPU registers and prints/logs their content. 
void StdUseCase_GetCpuState(void)
{
    STATUS_T RetState = MSP430_Registers(g_lTargetDeviceRegisters, ALL_REGS, READ);

    if (RetState != STATUS_OK)
    {
        Print_N_Log(ERR_MSG,"  ERROR: MSP430_Registers(..., ALL_REGS, READ)\n");
        StdUseCase_Exit(EXIT_REGISTER_READ, 0);
    }

    Print_N_Log(STD_MSG,"  ||  Processor State: CPU Registers\n");
    Print_N_Log(STD_MSG,"  ||              PC=0x%5.5X R4 =0x%5.5X\n",g_lTargetDeviceRegisters[PC],g_lTargetDeviceRegisters[R4]);
    Print_N_Log(STD_MSG,"  ||              SP=0x%5.5X R5 =0x%5.5X\n",g_lTargetDeviceRegisters[SP],g_lTargetDeviceRegisters[R5]);
    Print_N_Log(STD_MSG,"  ||              SR=0x%5.5X R6 =0x%5.5X\n",g_lTargetDeviceRegisters[SR],g_lTargetDeviceRegisters[R6]);
    Print_N_Log(STD_MSG,"  ||                         R7 =0x%5.5X\n",g_lTargetDeviceRegisters[R7]);
    Print_N_Log(STD_MSG,"  ||                         R8 =0x%5.5X\n",g_lTargetDeviceRegisters[R8]);
    Print_N_Log(STD_MSG,"  ||                         R9 =0x%5.5X\n",g_lTargetDeviceRegisters[R9]);
    Print_N_Log(STD_MSG,"  ||                         R10=0x%5.5X\n",g_lTargetDeviceRegisters[R10]);
    Print_N_Log(STD_MSG,"  ||                         R11=0x%5.5X\n",g_lTargetDeviceRegisters[R11]);
    Print_N_Log(STD_MSG,"  ||                         R12=0x%5.5X\n",g_lTargetDeviceRegisters[R12]);
    Print_N_Log(STD_MSG,"  ||                         R13=0x%5.5X\n",g_lTargetDeviceRegisters[R13]);
    Print_N_Log(STD_MSG,"  ||                         R14=0x%5.5X\n",g_lTargetDeviceRegisters[R14]);
    Print_N_Log(STD_MSG,"  ||                         R15=0x%5.5X\n",g_lTargetDeviceRegisters[R15]);
}

/*****************************************************************************/
//! \brief Wait for trigger or timeout
//! \param[in] dwMilliseconds Timeout (in ms)
//! \return TRUE: EEM trigger occured\n
//! FALSE: timeout occured
BOOL StdUseCase_WaitForTrigger(DWORD dwMilliseconds)
{
    BOOL RetState;

    RetState = false;

    Print_N_Log(STD_MSG,"* Waiting for event (trigger or timeout %d sec)...\n",dwMilliseconds/1000);
    
    // wait until breakpoint hit occurs or wait for dwMilliseconds milliseconds
    // if event occurs, BpMessage is locked up in MY_DLL430_CallbackFunc
    if (MyEvent.wait(dwMilliseconds))
    {
        Print_N_Log(STD_MSG,"  EVENT: EEM TRIGGER occured\n");
        MyEvent.reset();
        RetState = true;
    }
    else
    {
        Print_N_Log(ERR_MSG,"  EVENT: Timeout occured\n");
        StdUseCase_Exit(EXIT_VARIOUS, 0);
    }

    return RetState;
}

/*****************************************************************************/
//! \brief Wait for specific event
//! \param[in] MsgId Event to wait for
//! \param[in] dwMilliseconds Timeout (in ms)
//! \return  TRUE: EEM trigger occured\n
//! FALSE: timeout occured
BOOL StdUseCase_WaitForEvent(uint32_t MsgId, long dwMilliseconds)
{
    BOOL   RetState;
    Event *EventToWaitFor = NULL;

    RetState = false;

    Print_N_Log(STD_MSG,"  Waiting for -> ");
    switch(MsgId)
    {
    case WM_BpMESSAGE:
        EventToWaitFor = &MyBpEvent;
        Print_N_Log(STD_MSG,"  Breakpoint hit ");
        break;
    case WM_SsMESSAGE:
        EventToWaitFor = &MySsEvent;
        Print_N_Log(STD_MSG,"  Single step complete ");
        break;
    case WM_StoMESSAGE:
        EventToWaitFor = &MyStoEvent;
        Print_N_Log(STD_MSG,"  Storage on trace buffer ");
        break;
    case WM_SStateMESSAGE:
        EventToWaitFor = &MySStateEvent;
        Print_N_Log(STD_MSG,"  Change in new state of the sequencer ");
        break;
    case WM_CPUStopMESSAGE:
        EventToWaitFor = &MyCPUStopEvent;
        Print_N_Log(STD_MSG,"  Device CPU stopped ");
        break;
    default:
        Print_N_Log(ERR_MSG,"   \n  %%ERROR%%: Not a defined event. Execution will continue immediately.\n");
        break;
    }

    if(EventToWaitFor)
    {
        if(dwMilliseconds == INFINITE)
        {
            Print_N_Log(STD_MSG,"  (INFINITE)...\n");
        }
        else
        {
            Print_N_Log(STD_MSG,"  (timeout %d sec)...\n",dwMilliseconds/1000);
        }
        // wait until breakpoint hit occurs or wait for dwMilliseconds milliseconds
        // if event occurs, BpMessage is locked up in MY_DLL430_CallbackFunc
        if (EventToWaitFor->wait(static_cast<unsigned int>(dwMilliseconds)))
        {
            Print_N_Log(STD_MSG,"  EVENT: occured\n");
            MyStoEvent.reset();
            RetState = true;
        }
        else
        {
            Print_N_Log(ERR_MSG,"  EVENT: Timeout occured\n");
        }
    }
    return RetState;
}

/*****************************************************************************/
//! \brief Read the state storage buffer
//! \details StdUseCase_EemInit() must be called prior to this funcion.
//! \param[in] NumberOfValidEntries Number of valid state entries
void StdUseCase_EemReadStateStorageBuffer(int16_t NumberOfValidEntries)
{
    STATUS_T RetState;
    TraceBuffer_t TraceBuffer[8];       // place for 8 state storage entries

    Print_N_Log(STD_MSG,"  MSP430_EEM_ReadTraceBuffer()\n");
    RetState = MSP430_EEM_ReadTraceBuffer(TraceBuffer);
    if(RetState != STATUS_OK)
    {
        StdUseCase_Exit(EXIT_EEM_READTRACEBUFFER, 0);
    }

    Print_N_Log(STD_MSG,"  Nr.  MAB    MDB    Ctrl\n");
    for (int i=0; i<8; i++)
    {
        Print_N_Log(STD_MSG,"  %i:   %04x   %04x   %04x   ", i+1, TraceBuffer[i].lTrBufMAB, TraceBuffer[i].lTrBufMDB, TraceBuffer[i].wTrBufCNTRL);
        if(i < NumberOfValidEntries)
        {
            Print_N_Log(STD_MSG,"  New\n");
        }
        else
        {
            Print_N_Log(STD_MSG,"  Old\n");
        }
    }
}

/*****************************************************************************/
//! \brief This function prints and logs all Breakpoint parameters.
//! \param[in] pBpBuffer Breakpoint info buffer
void StdUseCase_PrintBpParameter_t(BpParameter_t* pBpBuffer)
{
    Print_N_Log(STD_MSG,"  Content of BpParameter_t struct:\n");
    Print_N_Log(STD_MSG,"  bpMode:        0x%5.5X\n",pBpBuffer->bpMode);
    Print_N_Log(STD_MSG,"  lAddrVal:      0x%5.5X\n",pBpBuffer->lAddrVal);
    Print_N_Log(STD_MSG,"  bpType:        0x%5.5X\n",pBpBuffer->bpType);
    Print_N_Log(STD_MSG,"  lReg:          0x%5.5X\n",pBpBuffer->lReg);
    Print_N_Log(STD_MSG,"  bpAccess:      0x%5.5X\n",pBpBuffer->bpAccess);
    Print_N_Log(STD_MSG,"  bpAction:      0x%5.5X\n",pBpBuffer->bpAction);
    Print_N_Log(STD_MSG,"  bpOperat:      0x%5.5X\n",pBpBuffer->bpOperat);
    Print_N_Log(STD_MSG,"  lMask:         0x%5.5X\n",pBpBuffer->lMask);
    Print_N_Log(STD_MSG,"  lRangeEndAdVa: 0x%5.5X\n",pBpBuffer->lRangeEndAdVa);
    Print_N_Log(STD_MSG,"  bpRangeAction: 0x%5.5X\n",pBpBuffer->bpRangeAction);
    Print_N_Log(STD_MSG,"  bpCondition:   0x%5.5X\n",pBpBuffer->bpCondition);
    Print_N_Log(STD_MSG,"  lCondMdbVal:   0x%5.5X\n",pBpBuffer->lCondMdbVal);
    Print_N_Log(STD_MSG,"  bpCondAccess:  0x%5.5X\n",pBpBuffer->bpCondAccess);
    Print_N_Log(STD_MSG,"  lCondMask:     0x%5.5X\n",pBpBuffer->lCondMask);
    Print_N_Log(STD_MSG,"  bpCondOperat:  0x%5.5X\n",pBpBuffer->bpCondOperat);
    Print_N_Log(STD_MSG,"  wExtCombine:   0x%5.5X\n",pBpBuffer->wExtCombine);
}

/*****************************************************************************/
//! \brief This function is of type MSP430_FET_NOTIFY_FUNC and generates system 
//! messages to inform the user of the progress of the firmware update executed 
//! by MSP430_FET_FwUpdate()
//! \details For more details see the definition of MSP430_FET_NOTIFY_FUNC in 
//! msp430_fet.h
//! \param[in] MsgId Message ID
//! \param[in] wParam Generic parameter 1
//! \param[in] lParam Generic parameter 2
//! \param[in] clientHandle Callback handle
void MY_MSP430_CallbackFunc( uint32_t MsgId, uint32_t wParam, uint32_t lParam, int32_t clientHandle)
{
    if(clientHandle == myHandle)
    {
        static unsigned int out_length;    // keeps track of the number of characters output for formatting purposes
        static unsigned int prog_bar_idx;  // index for outputting progress bar symbols        
        
        switch(MsgId)
        {
            case BL_INIT:
                Print_N_Log(STD_MSG,"\n*********************************************************\n");
                Print_N_Log(STD_MSG,"*                                                        \n");
                Print_N_Log(STD_MSG,"*  Initializing Update Bootloader.                       \n");
                break;
            case BL_ERASE_INT_VECTORS:
                Print_N_Log(STD_MSG,"*  Erasing mapped interrupt vectors.                     \n");
                break;
            case BL_ERASE_FIRMWARE:
                Print_N_Log(STD_MSG,"*  Erasing firmware memory section.                      \n");
                break;
            case BL_PROGRAM_FIRMWARE:
                Print_N_Log(STD_MSG,"*  Programming new firmware:                             \n");
                Print_N_Log(STD_MSG,"");
                inst_state |= PROG_BAR_ON;
                Print_N_Log(LOG_MSG, "*  PROGRESS BAR                                         \n");
                break;
            case BL_DATA_BLOCK_PROGRAMMED:
                out_length = prog_bar_idx = 0;
            
                if(wParam_prev > wParam) printf("\n"); 
                printf("\r");
                out_length += printf("*  ");
            
                for(prog_bar_idx = 0; prog_bar_idx < wParam; prog_bar_idx += 2)
                    out_length += printf("|");                            //prints | character for every 2% completion
            
                out_length += printf(" %u%%", wParam);                    //prints percentage of completion
            
                for( ;out_length <= 57; out_length++) printf(" ");        //completes the line and border        
                break;
            case BL_EXIT:
                Print_N_Log(STD_MSG,"\n");
                inst_state &= ~(PROG_BAR_ON);
                Print_N_Log(STD_MSG,"*  Exit Update Bootlader and reboot firmware.            \n");
                break;
            case BL_UPDATE_DONE:
                Print_N_Log(STD_MSG,"\n*  Update was successfully finished.                   \n");
                Print_N_Log(STD_MSG,"*                                                        \n");
                Print_N_Log(STD_MSG,"*********************************************************\n");
                break;
            case BL_UPDATE_ERROR:
                inst_state &= ~(PROG_BAR_ON);
                Print_N_Log(ERR_MSG,"*  An error occured during firmware update.             \n");
                Print_N_Log(ERR_MSG,"*                                                       \n");
                Print_N_Log(ERR_MSG,"********************************************************\n");
                break;
            case BL_WAIT_FOR_TIMEOUT:
                inst_state &= ~(PROG_BAR_ON);
                Print_N_Log(ERR_MSG,"*  An error occured during firmware update.             \n");
                Print_N_Log(ERR_MSG,"*                                                       \n");
                Print_N_Log(ERR_MSG,"********************************************************\n");
                break;
        }
    }
}

/*****************************************************************************/
//! \brief Exit the Flasher routine, close the logfile
//! \param[in] exitCode Exit Code to be displayed and written into the log file
//! \param[in] lErrorNumber Error number if MSP430_Error_Number() has already been
//! called by another function. The error number is cleared afterwards, so on
//! calling MSP430_Error_Number() twice, lErrorNumber would always be 0.
void StdUseCase_Exit(int exitCode, int32_t lErrorNumber)
{
    // The ErrorNumber is cleared after it was read once. If it has been read
    // out before, make sure it's not overwritten.
    if (lErrorNumber == 0)
    {
        lErrorNumber = MSP430_Error_Number();
    }

    if (lErrorNumber)
    {
        Print_N_Log(ERR_MSG,"# Exit: %d\n", exitCode);
        Print_N_Log(ERR_MSG,"# ERROR: %s\n", MSP430_Error_String(lErrorNumber));
    }
    
    StdUseCase_Close(&g_sOpt);

    // close logfile
    if(lfile != NULL)
    {
        fclose(lfile);
    }

    exit(exitCode);
}

/*****************************************************************************/
//! \brief Trigger VCC on and/or apply hard reset via RST/NMI (if specified in 
//! sOpt) and disconnect from the device
//! \param[in] sOpt Pointer to the option struct
void StdUseCase_Close(sDefOptions_t* sOpt)
{
    STATUS_T Ret_Close;
    int32_t lErrorNumber;

    Ret_Close = STATUS_ERROR;
    
    if(!(inst_state & ALT_EXIT))
    {
        if(inst_state & RESET_ON_EXIT)
        {
            if(inst_state & BLOW_FUSE)
            {
                Print_N_Log(ERR_MSG,"* Device-secure specified. Exit spec [RESET] ignored.");
            }
            else
            {
                Print_N_Log(REPORT_MSG,"* Resetting device (RST/NMI)...");
                MSP430_Reset(FORCE_RST_RESET, 0, 0);  // do a "hard" reset
            }
        }

        if(sOpt->pcVCC != 0)
        {
            Print_N_Log(REPORT_MSG,"* Starting target code execution...");

            // if "hard" reset is not required do a "soft" reset
            if(!(inst_state & RESET_ON_EXIT) && !(inst_state & NO_RESET))
            {
                if(inst_state & TARGETARCH_432)
                {
                    MSP430_Reset(FORCE_PUC_RESET, true, false);
                }
                else
                {
                    MSP430_Reset(ALL_RESETS, true, false);
                }
            }
            else
            {
                StdUseCase_Run();
            }

            Ret_Close = MSP430_Close(0);   // disconnect from MSP430 without turning down Vcc
        }
        else
        {
            Print_N_Log(REPORT_MSG,"* Powering down...");
            Ret_Close = MSP430_Close(1);   // disconnect from MSP430 - turn down Vcc
        }

        Print_N_Log(REPORT_MSG,"* Disconnecting from device...");
    }

    lErrorNumber = MSP430_Error_Number();
    
    if((Ret_Close == STATUS_OK) || (lErrorNumber == 0))
    {
        Print_N_Log(REPORT_MSG, "* ");

        Print_N_Log(STD_MSG,"* ----------------------------------------------------------------------------\n");
        Print_N_Log(STD_MSG,"* Driver      : closed (%s)\n", MSP430_Error_String(lErrorNumber));
        Print_N_Log(STD_MSG,"* ----------------------------------------------------------------------------\n");
        Print_N_Log(STD_MSG,"*/\n");
    }
    else
    {
        Print_N_Log(ERR_MSG,"* ----------------------------------------------------------------------------\n");
        Print_N_Log(ERR_MSG,"* ERROR on close: %s\n", MSP430_Error_String(lErrorNumber));
        Print_N_Log(ERR_MSG,"* ----------------------------------------------------------------------------\n");
        Print_N_Log(ERR_MSG,"*/\n");
    }    
}

/*****************************************************************************/
/*                           END OF SOURCE FILE                              */
/*****************************************************************************/
