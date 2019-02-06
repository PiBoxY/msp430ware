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
* \file Support.cpp
*
* \par This file contains all support functions which are not directly interacting with the MSP430.DLL
**/

/*****************************************************************************/
/*                                 INCLUDES                                  */
/*****************************************************************************/

#include "StdUseCase.h"
#include <sys/stat.h>
#include <algorithm>

#ifdef WIN32
#include <direct.h>
#endif

using namespace std;

/*****************************************************************************/
/*                                 VARIABLES                                 */
/*****************************************************************************/

//! \brief This bitmask contains global states and is accessed in various functions
unsigned int inst_state = 0;

//! \brief File pointer for logfile
FILE* lfile = NULL;

//! \brief String of all available command line switches (*: = has argument; *_ = argument optional)
char g_cAllOpts[MAX_OPTS] = "n:N:o:O:p:P:l_L_i:I:j:J:m:M:r:R:w:W:d:D:e:E:t:T:z:Z:v_V_aAbBqQHh?xXgGfFuUsS";

//! \brief Struct of additional options
sAddOptions_t g_sAddOpt[MAX_OPTS];
//! \brief Global counter for additional options
unsigned long g_lCountOpt = 0;

//! \brief Default options
sDef_t g_Def = {"TIUSB", "ERASE_ALL", "", "TRUE", "FALSE", 0, "10000"};

// Breakpoint help variables
//! \brief Number of breakpoints
int BPcount = 0;
//! \brief Array for storing breakpoint addresses
int32_t BPAddr[50];

/*****************************************************************************/
/*                                FUNCTIONS                                  */
/*****************************************************************************/

/*****************************************************************************/
//! \brief This function enables the logging feature unless it's disabled by
//! the -g switch.
//! \details If logging is enabled, directory 'Log' will be created in line
//! with the executable
//! \param[in] argc Number of arguments in the argument string
//! \param[in] argv[] Pointer to the argument string
void EnableLog(int argc, char* argv[])
{
    int i;

    // check if logging should be disabled
    for(i=1;i<argc;i++)
    {
        if((string(argv[i]) == "-g") || (string(argv[i]) == "-G"))
        {
            inst_state |= LOG_OFF;
        }
    }
    
    if ((~inst_state & LOG_OFF) && ((lfile = fopen("Log/log.txt", "a")) == NULL))
    {
        printf("\n");
        printf("* Unable to access log file. Creating folder...");

#ifdef WIN32
        if(_mkdir("Log") == 0)
#else
        if(mkdir("Log", 0777) == 0)
#endif
        {
            printf("done\n");
            lfile = fopen("Log/log.txt", "a");
        }
        else
        {
            inst_state |= LOG_OFF;
            printf("\n");
            printf("**  ERROR: Unable to create directory for logfile. Logging disabled.\n");
        }
    }
}

/*****************************************************************************/
//! \brief This function prints the version of the Flasher executable.
void PrintVersion(void)
{
    Print_N_Log(STD_MSG,"* -----/|-------------------------------------------------------------------- *\n");
    Print_N_Log(STD_MSG,"*     / |__                                                                   *\n");
    Print_N_Log(STD_MSG,"*    /_   /   MSP Flasher v1.3.10                                             *\n");
    Print_N_Log(STD_MSG,"*      | /                                                                    *\n");
    Print_N_Log(STD_MSG,"* -----|/-------------------------------------------------------------------- *\n");
    Print_N_Log(STD_MSG,"*\n");
}

/*****************************************************************************/
//! \brief This function prints a soft copy of the command line arguments which 
//! can be passed to the Flasher.
//! \details It is called whenever invalid command line arguments are used.
void Help(void)
{
    Print_N_Log(1,"*******************************************************************************\n");
    Print_N_Log(1,"Usage: MSP430Flasher [OPTIONS]                                                 \n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," -n DEVICE_NAME              (optional for MSP430, required for MSP432)        \n");
	Print_N_Log(1,"                             specifies the name of the target MSP - prompt in  \n");
    Print_N_Log(1,"                             case of mismatch                                  \n");
    Print_N_Log(1,"                             Use -n NO_TARGET to run MSP Flasher without       \n");
    Print_N_Log(1,"                             attempting a target connection (FET detection or  \n");
    Print_N_Log(1,"                             FET firmware update only)                         \n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," -i (TI)USB | DETECT |       specifies the COM port of a connected debug tool  \n");
    Print_N_Log(1,"    COMn (Win) |             (default: TIUSB/USB = first detected FET tool is  \n");
    Print_N_Log(1,"    ttyACMn (Linux) |        used) For info on how to connect to specific eZ-  \n");
    Print_N_Log(1,"    usbmodem* (OSX)          tools, see the MSP Flasher manual.                \n");
    Print_N_Log(1,"                             Use -i DETECT  to execute a FET detection sweep,  \n");
    Print_N_Log(1,"                             displaying detailed info about all connected debug\n");
    Print_N_Log(1,"                             tools. User is prompted to pick a FET.            \n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," -j fast | medium | slow     sets FET speed for JTAG/SBW - only applicable for \n");
    Print_N_Log(1,"                             MSP-FET! Option will be ignored for all other FET \n");
    Print_N_Log(1,"                             tools. Default = medium.                          \n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," -a                          non-intrusive target connection: use this switch  \n");
    Print_N_Log(1,"                             if no reset should be applied to the target device\n");
    Print_N_Log(1,"                             on start-up. Correct target device name needs to  \n");
    Print_N_Log(1,"                             be specified using the -n switch!                 \n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," -r [filenname,mem_sect]     specifies a memory section to read from and a file\n");
    Print_N_Log(1,"                             to write to. mem_sect: RAM, INFO, MAIN, BSL or    \n");
    Print_N_Log(1,"                             specific memory areas: 0x****-0x*****             \n");
    Print_N_Log(1,"                             The file extension of 'filename' determines the   \n");
    Print_N_Log(1,"                             data format: .txt = TI-TXT, .hex/.a43 = Intel-Hex \n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," -w filenname                specifies a file for device programming. Supported\n");
    Print_N_Log(1,"                             data formats: TI-TXT (.txt), Intel-Hex (.hex/.a43)\n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," -b                          unlocks BSL memory for writing                    \n");
    Print_N_Log(1,"                             (use only with -w switch)                         \n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," -u                          unlocks InfoA memory for writing                  \n");
    Print_N_Log(1,"                             (use only with -w switch)                         \n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," -e ERASE_ALL |              Erase memory. Default: ERASE_ALL (INFO&MAIN)      \n");
	Print_N_Log(1,"    ERASE_MAIN |             ERASE_MAIN: erase MAIN memory only                \n");
    Print_N_Log(1,"    ERASE_SEGMENT |          ERASE_SEGMENT: erase segment to be programmed only\n");
    Print_N_Log(1,"    ERASE_TOTAL |            ERASE_TOTAL: applicable for FR5xx/FR6xx only!     \n");
    Print_N_Log(1,"    ERASE_USER_CODE |        ERASE_USER_CODE: applicable for FR4xx only!       \n");
    Print_N_Log(1,"    NO_ERASE                 NO_ERASE: use only with -w switch                 \n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," -v filename (optional)      triggers verification of the target memory against\n");
	Print_N_Log(1,"                             a target code file. If -w is used, no argument is \n");
    Print_N_Log(1,"                             required. For a stand-alone verify, provide the   \n");
    Print_N_Log(1,"                             path to a target code file as an argument.        \n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," -z [exit_spec,...]          specifies state of device on exit                 \n");
    Print_N_Log(1,"                             (view available 'exit_spec's using -x switch)     \n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," -g                          switches log OFF (default: ON)                    \n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," -q                          triggers QUIET MODE (no system messages displayed)\n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," -d [breakpoint addresses]   specifies addresses for hardware breakpoints and  \n");
    Print_N_Log(1,"                             triggers RUN_TO_BREAKPOINT mode                   \n");
    Print_N_Log(1,"                             WARNING! This option is deprecated and will no    \n");
    Print_N_Log(1,"                             longer be maintained. All breakpoint functionality\n");
    Print_N_Log(1,"                             will be removed in a future version of MSP Flasher\n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," -t timeout_in_ms            specifies the breakpoint timeout (in milliseconds)\n");
    Print_N_Log(1,"                             WARNING! This option is deprecated and will no    \n");
    Print_N_Log(1,"                             longer be maintained. All breakpoint functionality\n");
    Print_N_Log(1,"                             will be removed in a future version of MSP Flasher\n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," -p JTAG PASSWORD            specifies the JTAG password (hex format: 0x...).  \n");
    Print_N_Log(1,"                             (if any, default: no password)                    \n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," -s                          suppresses the FET firmware update prompt         \n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," -o L | C                    specifies operation mode for L092 or RF430 devices\n");
    Print_N_Log(1,"                             (L = normal mode, C = ROM development mode)       \n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," -l PASSWORD LENGTH          OBSOLETE! If used, this option will be ignored!   \n");
    Print_N_Log(1,"                             JTAG password length is determined automatically  \n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," -m JTAG | SBW2 | SBW4 |     OBSOLETE! If used, this option will be ignored!   \n");
    Print_N_Log(1,"    AUTO                     Communication protocol is determined automatically\n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1,"*******************************************************************************\n");
}

/*****************************************************************************/
//! \brief This function prints a soft copy of the possible exit specification 
//! arguments for the -z switch
void Exit_Specs(void)
{
    Print_N_Log(1,"*******************************************************************************\n");
    Print_N_Log(1,"EXIT_SPECS for -z switch (in square brackets, use ',' as delimiters)           \n");
    Print_N_Log(1,"List of exit specifications that can be passed as arguments to the -z switch:  \n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," VCC                         switches VCC on (default: off)                    \n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," VCC=xxxx                    sets VCC to specified value in milliVolts         \n");
    Print_N_Log(1,"                             min: VCC=1800, default: VCC=3000, max: VCC=3600   \n");
    Print_N_Log(1,"                             Not supported by eZ emulators!                    \n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1," RESET                       executes 'hard' reset (using RST/NMI pin) on exit \n");
    Print_N_Log(1,"                                                                               \n");
    Print_N_Log(1,"*******************************************************************************\n");
}

/*****************************************************************************/
//! \brief Add parameters given in cOpts to existing parameters
//! \param[in] cOpts Parameter string to be added
void AddOpts(char* cOpts)
{
    int   iStdOptSize;
    int   iOptSize;

    iStdOptSize = strlen(g_cAllOpts);    // detect string-length of all parameters
    iOptSize = strlen(cOpts);            // detect string-length of current parameter

    memcpy((char*)&g_cAllOpts[iStdOptSize],cOpts,iOptSize);    // add current parameter to used parameters
    memcpy((char*)&g_cAllOpts[iStdOptSize+1],":",1);           // add seperation to parameters
}

/*****************************************************************************/
//! \brief Add parameter to struct of all additional parameters
//! \param[in] OptLetter Pointer to new letter
//! \param[in] OptString Pointer to the assigned string
void RegisterOpt(char* OptLetter, char** OptString)
{
    AddOpts(OptLetter);
    g_sAddOpt[g_lCountOpt].OptLetter = OptLetter;    // add letter
    g_sAddOpt[g_lCountOpt].OptString = OptString;    // add assigned string
    g_lCountOpt++;                                   // count up register
}

/*****************************************************************************/
//! \brief This function evaluates options from the argument string and 
//! configures the Flasher accordingly
//! \param[in] argc Number of arguments in the argument string
//! \param[in] argv[] Pointer to the argument string
//! \param[in] sOpt Pointer to the default options
//! \param[out] RTE Pointer to the struct for runtime entries
//! \return  0: Getopt routine encountered no errors,
//! 1: perform exit on abort - display help,
//! 2: perform exit on abort - display exitSpecs
int EvaluateOptions(int argc, char* argv[], sDefOptions_t* sOpt, sRTE_t* RTE)
{
    int   RetState;
    unsigned long lCountAddOpt;
    unsigned long i;
    char  cOpt;
    char* mk_upper = NULL;
    unsigned short OptFlags;

    RetState    = 0;
    OptFlags    = 0;
    lCountAddOpt= 0;

    Print_N_Log(REPORT_MSG,"* Evaluating triggers...");

    // get parameter
    while ((cOpt = getopt(argc, ( char**)argv, g_cAllOpts, RTE)) != EOF )
    {
        // choose code for determined parameter
        switch (cOpt)
        {
            case 'n':    // device name
            case 'N':
                sOpt->pcDeviceName = optarg;
                if((string(sOpt->pcDeviceName) == "MSP430L092") ||
				   (string(sOpt->pcDeviceName) == "MSP430C092") ||
				   (string(sOpt->pcDeviceName).find("RF430") != string::npos))
                {
                    inst_state |= MODE_SELECT;
                    g_Def.deviceAC = 0xA55AA55A;
				}
				if(string(sOpt->pcDeviceName).find("MSP432") != string::npos)
				{
					inst_state |= TARGETARCH_432;
				}
                break;
            case 'o':    // device mode
            case 'O':
                sOpt->pcDeviceMode = optarg;
                inst_state |= MODE_SELECT;
                break;
            case 'p':    // password
            case 'P':
                sOpt->pcPassword = optarg;
                OptFlags |= OPT_PASSWORD;
                break;
            case 'l':    // password length
            case 'L':    // OBSOLETE
                break;
            case 'i':    // interface
            case 'I':
                sOpt->pcInterface = optarg;
                OptFlags |= OPT_INTERFACE;                        
                break;
			case 'j':    // InterfaceSpeed
			case 'J':
				sOpt->pcFETspeed = optarg;
				OptFlags |= OPT_FETSPEED;
				break;
            case 'm':    // connection mode
            case 'M':    // OBSOLETE
                break;
            case 'r':    // read specifications
            case 'R':    
                sOpt->pcReadSpecs = optarg;
                OptFlags |= OPT_READSPECS;
                break;
            case 'w':    // input file
            case 'W':    
                sOpt->pcFile = optarg;
                OptFlags |= OPT_INFILE;
                break;
            case 'a':    // no reset during start-up
            case 'A':    
                inst_state |= NO_RESET;
                break;
            case 'b':    // BSL memory access
            case 'B':    
                inst_state |= WRITE_BSL;
                break;
            case 'u':    // locked flash memory access
            case 'U':    
                inst_state |= UNLOCK_FLASH;
                break;
            case 'e':    // erasetype
            case 'E':
                sOpt->pcEraseType = optarg;
                if(sOpt->pcFile == NULL && strcmp(sOpt->pcEraseType, "NO_ERASE"))
                {
                    inst_state |= ERASE_NO_WRITE;
                }
                break;
            case 'f':    // device secure
            case 'F':
                inst_state |= BLOW_FUSE;
                break;
            case 'v':    // verify
            case 'V':
				if(optarg)
				{
                    sOpt->pcVerify = optarg;
				}
				else
				{
                    sOpt->pcVerify = g_Def.defTRUE;
				}
                break;
            case 'z':   // exit specs
            case 'Z':    
                sOpt->pcExitSpecs = optarg;
                OptFlags |= OPT_EXITSPECS;
                break;
            case 'g':   // disable log
            case 'G':   // handled in EnableLog() 
                inst_state |= LOG_OFF;
                break;
            case 'q':   // quiet mode
            case 'Q':    
                inst_state |= QUIET_MODE;
                break;
            case 'H':    // show help on the display
            case 'h':
            case '?':
                Print_N_Log(REPORT_MSG,"");
                return(RetState = 1);
                break;
            case 'x':   // show exit specs
            case 'X':    
                Print_N_Log(REPORT_MSG,"");
                return(RetState = 2);
                break;
            case 'd':   // breakpoints
            case 'D':
                sOpt->pcBreakpoints = optarg;
                OptFlags |= OPT_BPSPECS;
                break;
            case 't':    // breakpoint timeout
            case 'T':
                sOpt->pcTimeout = optarg;
                break;
            case 's':    // suppress FET firmware update prompt
            case 'S':
                inst_state |= SKIP_UDPROMPT;
                break;
        }
    }
    
    // Special handling for devices with ROM development mode
    if(inst_state & MODE_SELECT)
    {

        char device_mode_entry;    // for storing device mode if entered during runtime
        
        if ((sOpt->pcDeviceMode == NULL) ||                        // if device mode is not specified
            (((strchr("Ll", *(sOpt->pcDeviceMode)) == NULL)  &&    // if entered character is...
              (strchr("Cc", *(sOpt->pcDeviceMode)) == NULL)) ||    // ...invalid
              (strlen(sOpt->pcDeviceMode) > 1)))                   // if entered mode has more than one character
        {
            if (sOpt->pcDeviceMode == NULL)    // if device mode is not specified
            {
                Print_N_Log(ERR_MSG,"- The device you are trying to access can\n");
                Print_N_Log(ERR_MSG,"- operate in multiple modes but no mode has\n");
                Print_N_Log(ERR_MSG,"- been specified. Please use trigger -o. \n");
            }
            else if (((strchr("Ll", *(sOpt->pcDeviceMode)) == NULL)  &&    // if entered character is...
                      (strchr("Cc", *(sOpt->pcDeviceMode)) == NULL)) ||    // ...invalid
                      (strlen(sOpt->pcDeviceMode) > 1))                    // if entered mode has more than one character
            {
                Print_N_Log(ERR_MSG,"* Invalid argument for -o switch\n");
            }
            Print_N_Log(ERR_MSG,"- Please enter L for normal mode, C for\n");    // prompt user for device mode
            Print_N_Log(ERR_MSG,"- ROM development mode or enter n to abort:\n");
            do
            {
                cin >> device_mode_entry;
                cin.ignore();
                Print_N_Log(LOG_MSG, "%c\n", device_mode_entry);
            }
            while(strchr("lLcCn", device_mode_entry) == NULL);

            if (device_mode_entry == 'n') 
            {
                return(RetState = 1);    // perform exit on abort
            }
            else 
            {
                sOpt->pcDeviceMode = &device_mode_entry;    // update device mode
            }
        }

        if(strchr("Ll", *(sOpt->pcDeviceMode)) != NULL)
        {
            g_Def.deviceAC = 0xA55AA55A;
			if(string(sOpt->pcDeviceName).find("RF430") == string::npos)
			{
                inst_state |= WRITE_EXT_MEM;
			}
        }
        else if (strchr("Cc", *(sOpt->pcDeviceMode)) != NULL)
        {
            g_Def.deviceAC = 0x5AA55AA5;
        }
    }

	if(OptFlags & OPT_FETSPEED)
    {
		for(mk_upper = sOpt->pcFETspeed; *mk_upper != '\0'; mk_upper++)    // converts argument to uppercase
            *mk_upper = toupper(*mk_upper);
		string FETspeed(sOpt->pcFETspeed ? sOpt->pcFETspeed : "");
		if(FETspeed != "MEDIUM" && FETspeed != "FAST" && FETspeed != "SLOW")
		{
			Print_N_Log(ERR_MSG,"* Invalid argument for -j switch.\n");
            Print_N_Log(ERR_MSG,"* Terminating...\n");
            exit(EXIT_VARIOUS);
		}
	}

    if(OptFlags & OPT_PASSWORD)
    {
        string Password(sOpt->pcPassword ? sOpt->pcPassword : "");

        if(Password.find("0x") == string::npos)    // if entered password is not hex
        {
            Print_N_Log(ERR_MSG,"* Please enter hex password (beginning with 0x)\n");
            Print_N_Log(ERR_MSG,"* Terminating...\n");
            exit(EXIT_VARIOUS);
        }
        if ((Password.length() - 2) % 4 != 0)    // if entered password is not multiple of 16-bit words
        {
            Print_N_Log(ERR_MSG,"* Invalid password length!\n");
            Print_N_Log(ERR_MSG,"* Please enter a multiple of 16 bit words.\n");
            Print_N_Log(ERR_MSG,"* Terminating...\n");
            exit(EXIT_VARIOUS);
        }
		else
		{
			sOpt->pcPWlen = (Password.length() - 2) / 4;
		}
    }

    if (OptFlags & OPT_INFILE)
    {
        // filename must be valid. Check:
        while(fopen(sOpt->pcFile, "r") == NULL)
        {
            Print_N_Log(ERR_MSG,"- The entered filename is invalid.\n");
            Print_N_Log(ERR_MSG,"-\n");
            Print_N_Log(ERR_MSG,"- Please reenter the filename or enter n to abort: ");

            cin.getline(RTE->filename_entry, MAX_PATH_LEN);
            Print_N_Log(LOG_MSG, "%s\n", RTE->filename_entry);
            
            string RTE_FileName(RTE->filename_entry ? RTE->filename_entry : "");
            if(RTE_FileName == "n" || RTE_FileName == "N")
            {
                return(RetState = 1); // perform exit on abort
            }
            else 
            {
                sOpt->pcFile = RTE->filename_entry;                     // update filename
            }
        }
    }

    // evaluate exit specs
    if(OptFlags & OPT_EXITSPECS)
    {
        char* exit_spec_start = NULL;    // pointers for separating delimited exit specs
        char* exit_spec_stop = NULL;
        char exit_spec[MAX_NAME_LEN];    // for storing individual exit specs temporarily for processing
        
        exit_spec_start = sOpt->pcExitSpecs + 1;
        for(exit_spec_stop = exit_spec_start; *exit_spec_stop != '\0'; exit_spec_stop++)
        {
            if(strchr(",]", *exit_spec_stop) != NULL)
            {
                strncpy(exit_spec, exit_spec_start, exit_spec_stop - exit_spec_start);
                exit_spec[exit_spec_stop - exit_spec_start] = '\0';
                if (string(exit_spec) == "VCC")
                {
                    sOpt->pcVCC = TARGET_VCC;
                }
                else if(string(exit_spec).find("=") != std::string::npos)
                {
                    char remove[] = "VCC= ";
                    string str = string(exit_spec);

                    for(unsigned int i=0; i < strlen(remove); ++i)
                    {
                        str.erase(std::remove(str.begin(), str.end(), remove[i]), str.end());
                    }

                    sOpt->pcVCC = stol(str,nullptr);
                }
                else if(string(exit_spec) == "RESET")
                {
                    inst_state |= RESET_ON_EXIT;
                }
                else
                {
                    Print_N_Log(STD_MSG, "* Exit specification '%s' is invalid and will be ignored.\n", exit_spec);
                }
                if(*exit_spec_stop == ']')
                {
                    break;
                }
                exit_spec_start = exit_spec_stop + 1;
            }
        }
    }

    // evaluate breakpoint specs
    if(OptFlags & OPT_BPSPECS)
    {
        char* bp_specs = NULL;
        char bp_temp[MAX_NAME_LEN];              //for storing individual breakpoint addresses temporarily
        int j = 0;

        bp_specs = sOpt->pcBreakpoints;

        while(*bp_specs != ']')
        {
            bp_specs++;                          // skip '[' and ','
            for(bp_specs; (*bp_specs != ',') && (*bp_specs != ']'); bp_specs++)
            {
                if(*bp_specs != ' ')
                {
                    bp_temp[j] = *bp_specs;
                }
                j++;
            }
            bp_temp[j] = '\0';
            j = 0;

            BPAddr[BPcount] = strtol(bp_temp, NULL, 16);
            BPcount++;
        }
    }

    // if memory read is specified
    if(OptFlags & OPT_READSPECS)
    {
        char* exit_spec_start = NULL;    // pointers for separating delimited read specs
        char* exit_spec_stop = NULL;
        
        for(sOpt->pcReadFile = sOpt->pcReadSpecs + 1; 
           ((*sOpt->pcReadFile == ' ')||(*sOpt->pcReadFile == ']')||(*sOpt->pcReadFile == '\0'));
            sOpt->pcReadFile++)
        {
            if(*sOpt->pcReadFile == '\0')
            {
                Print_N_Log(ERR_MSG,"\n* ':' option requires an argument -- r\n");    // error message
                Print_N_Log(ERR_MSG,"* Terminating...\n");
                return (RetState = 1);
            }
        }
        if(((exit_spec_stop = strstr(sOpt->pcReadFile, ".txt")) != NULL) || ((exit_spec_stop = strstr(sOpt->pcReadFile, ".txt")) != NULL))
        {
            RTE->file_type = FILETYPE_TI_TXT;
               exit_spec_start = exit_spec_stop + 4;
            *(exit_spec_start++) = '\0';
            
            while((*exit_spec_start == ' ') || (*exit_spec_start == ','))
            {
                if(*(exit_spec_start++) == ']')
                {
                    Print_N_Log(ERR_MSG,"* Memory section for trigger -r not specified\n");
                    return (RetState = 1);
                }
            }
            
            sOpt->pcReadMem = exit_spec_start;
            if((exit_spec_stop = strchr(sOpt->pcReadMem, ',')) != NULL)
                *exit_spec_stop = '\0';
            if((exit_spec_stop = strchr(sOpt->pcReadMem, ' ')) != NULL)
                *exit_spec_stop = '\0';
            if((exit_spec_stop = strchr(sOpt->pcReadMem, ']')) != NULL)
                *exit_spec_stop = '\0';
            for(mk_upper = sOpt->pcReadMem; *mk_upper != '\0'; mk_upper++)    // converts argument to uppercase
                    *mk_upper = toupper(*mk_upper);
        }
        else 
        if(((exit_spec_stop = strstr(sOpt->pcReadFile, ".a43")) != NULL) || ((exit_spec_stop = strstr(sOpt->pcReadFile, ".A43")) != NULL) ||
           ((exit_spec_stop = strstr(sOpt->pcReadFile, ".hex")) != NULL) || ((exit_spec_stop = strstr(sOpt->pcReadFile, ".HEX")) != NULL) ||
           ((exit_spec_stop = strstr(sOpt->pcReadFile, ".int")) != NULL) || ((exit_spec_stop = strstr(sOpt->pcReadFile, ".INT")) != NULL))
        {
            RTE->file_type = FILETYPE_INTEL_HEX;
            exit_spec_start = exit_spec_stop + 4;
            *(exit_spec_start++) = '\0';
            while((*exit_spec_start == ' ') || (*exit_spec_start == ','))
            {
                if(*(exit_spec_start++) == ']')
                {
                    Print_N_Log(ERR_MSG,"* Memory section for trigger -r not specified\n");
                    return (RetState = 1);
                }
            }
            sOpt->pcReadMem = exit_spec_start;
            if((exit_spec_stop = strchr(sOpt->pcReadMem, ',')) != NULL)
                *exit_spec_stop = '\0';
            if((exit_spec_stop = strchr(sOpt->pcReadMem, ' ')) != NULL)
                *exit_spec_stop = '\0';
            if((exit_spec_stop = strchr(sOpt->pcReadMem, ']')) != NULL)
                *exit_spec_stop = '\0';
            for(mk_upper = sOpt->pcReadMem; *mk_upper != '\0'; mk_upper++)    //converts argument to uppercase
                    *mk_upper = toupper(*mk_upper);
        }
        else
        {
            Print_N_Log(ERR_MSG, "* Invalid file extension for trigger -r\n");
            return (RetState = 1);
        }
        
        if (fopen(sOpt->pcReadFile, "w") == NULL)
        {
            Print_N_Log(ERR_MSG, "* Unable to open file for memory dump.\n");
            return (RetState = 1);
        }

        string ReadMem(sOpt->pcReadMem ? sOpt->pcReadMem : "");
        if ((ReadMem != "RAM")  &&
            (ReadMem != "INFO") &&
            (ReadMem != "MAIN") &&
            (ReadMem != "BSL")  &&
            (ReadMem.find("0X") == string::npos))
        {
            Print_N_Log(ERR_MSG,"* Invalid memory section for trigger -r\n");
            return (RetState = 1);
        }
        strcpy(RTE->read_file, sOpt->pcReadFile);
        strcpy(RTE->read_mem, sOpt->pcReadMem);
        sOpt->pcReadFile = RTE->read_file;
        sOpt->pcReadMem = RTE->read_mem;
    }
    
    const string Interface(sOpt->pcInterface ? sOpt->pcInterface : "");
    if ((Interface != "TIUSB")       &&
        (Interface != "USB")         &&
        (Interface != "DETECT")      &&
        (Interface.find( "COM" ) == string::npos ) &&
        (Interface.find( "ttyACM" ) == string::npos ) &&
		(Interface.find( "usbmodem" ) == string::npos ))
    {
        Print_N_Log(ERR_MSG,"* Invalid argument for -i trigger. Default used (USB).\n");
        sOpt->pcInterface = g_Def.defInterface;
    }

    const string EraseType(sOpt->pcEraseType ? sOpt->pcEraseType : "");
    if ((EraseType != "NO_ERASE")  &&
		(EraseType != "ERASE_ALL")  && 
        (EraseType != "ERASE_MAIN") &&
        (EraseType != "ERASE_SEGMENT") &&
        (EraseType != "ERASE_TOTAL") &&
        (EraseType != "ERASE_USER_CODE")) 
    {
        Print_N_Log(ERR_MSG,"* Invalid argument for -e trigger. Default used (ERASE_ALL).\n");
        sOpt->pcEraseType = g_Def.defEraseType;
    }

    // get additional use case specific parameters
    while ((cOpt = getopt(argc, ( char**)argv, g_cAllOpts, RTE)) != EOF )
    {
        for(i = 0; i < g_lCountOpt; i++)                        // for every element of struct of additional parameters
        {
            if(*g_sAddOpt[i].OptLetter == *(char*)&cOpt)        // if letter in struct is according to current additional letter
            {
                *g_sAddOpt[i].OptString = optarg;               // reassign parameter string given from input string
                lCountAddOpt++;                                 // count up number of non-default arguments
            }
        }
    }

    return RetState;
}

/*****************************************************************************/
//! \brief This function reads out options from the argument string
//! \param[in] argc Number of arguments in the argument string
//! \param[in] **argv Pointer to the argument string
//! \param[out] *opts Pointer to a string containing all possible switches
//! \param[out] *RTE Pointer to the struct for runtime entries
int getopt(int argc, char **argv, char *opts, sRTE_t* RTE)
{
    static   int   sp = 1;
    register int   c;
    register char *cp;
    char* mk_upper = NULL;

    if(sp == 1)    // static
    {
        // optind initially 1. 

        if(optind >= argc          ||     // if the parameters have been exhausted OR 
            argv[optind][0] != '-' ||     // the current parameter doesn't begin with '-' OR 
            argv[optind][1] == '\0')      // the current parameter is a single character  
        {                                                                            
            optind = 1;    // reset index                                                                
            return(EOF);   // EOF
        }
        
        else if(string(argv[optind]) == "--")
        {
            optind++;                                                
            return(EOF);
        }
    }
    
    c = argv[optind][sp];     // c stores parameter
    
    // if parameter is illegal (':' is used as a delimiter in the array of legal parameters)
    if(c == ':' || (cp=strchr(opts, c)) == NULL)    // cp pointer to parameter within the array of legal ones
    {  
        Print_N_Log(ERR_MSG,"\n* ':' illegal option -- %c\n\n", c);    // error message
        
        if(argv[optind][++sp] == '\0')    // if the end of the parameter has been reached
        {
            optind++;    // move on to next
            sp = 1;      // reset char index
        }
        return('?');     // display HELP
    }

    // if parameter is legal (except for H/h)
    if(*++cp == ':')
    {
        if(argv[optind][sp+1] != '\0')    // if argument is attached to parameter switch 
        {
            if(argv[optind][sp+1] == '-')    // if another switch follows
            {
                Print_N_Log(ERR_MSG,"* ':' option requires an argument -- %c\n\n", c);
                sp = 1;
                return('?');
            }
            if (strchr("rR", c) != NULL)
            {
                if(ReadSpacedArgs(argc, argv, &optind, COLL_A_KS, RTE->read_specs_processed))
                {
                    return('?');
                }
                optarg = RTE->read_specs_processed;
            }
            else
            if (strchr("wW", c) != NULL)
            {
                if(ReadSpacedArgs(argc, argv, &optind, FILENAME_A_KS, RTE->filename_processed))
                {
                    return('?');
                }
                optarg = RTE->filename_processed;
            }
            else
            if (strchr("zZ", c) != NULL)                // if exitspecs are expected
            {
                if(ReadSpacedArgs(argc, argv, &optind, COLL_A_RS, RTE->exit_specs_processed))
                {
                    return('?');
                }
                optarg = RTE->exit_specs_processed;
                for(mk_upper = optarg; *mk_upper != '\0'; mk_upper++)
                {
                    *mk_upper = toupper(*mk_upper);    // convert argument to uppercase
                }
            }    
            else
            if (strchr("pP", c) != NULL)
            {
                optarg = &argv[optind++][sp+1];
            }
            else
            {
                optarg = &argv[optind++][sp+1];    // optarg points to argument
                for(mk_upper = optarg; *mk_upper != '\0'; mk_upper++)
                {
                    *mk_upper = toupper(*mk_upper);    // convert argument to uppercase
                }
            }

        }    
        
        else if((++optind >= argc)||(argv[optind][0] == '-'))    // if no argument follows parameter switch
        {
            Print_N_Log(ERR_MSG,"* ':' option requires an argument -- %c\n\n", c);    // error message
            sp = 1;
            return('?');
        }
        else
        {
            if (strchr("rR", c) != NULL)
            {
                if(ReadSpacedArgs(argc, argv, &optind, COLL_UA_KS, RTE->read_specs_processed))
                {
                    return('?');
                }
                optarg = RTE->read_specs_processed;
            }
            else
            if (strchr("wW", c) != NULL)    // if a filename is expected
            {
                if(ReadSpacedArgs(argc, argv, &optind, FILENAME_UA_KS, RTE->filename_processed))
                {
                    return('?');
                }
                optarg = RTE->filename_processed;
            }
            else
            if (strchr("zZ", c) != NULL)    // if exitspecs are expected
            {
                if(ReadSpacedArgs(argc, argv, &optind, COLL_UA_RS, RTE->exit_specs_processed))
                {
                    return('?');
                }
                optarg = RTE->exit_specs_processed;
                for(mk_upper = optarg; *mk_upper != '\0'; mk_upper++)
                {
                    *mk_upper = toupper(*mk_upper);    // convert argument to uppercase
                }
            }
            else
            if (strchr("pP", c) != NULL)
            {
                optarg = argv[optind++];
            }
			else
			if (strchr("iI", c) != NULL)
            {
                optarg = argv[optind++]; // optarg points to argument (do not convert argument to uppercase)
            }
            else
            {
                optarg = argv[optind++];    // optarg points to argument
                for(mk_upper = optarg; *mk_upper != '\0'; mk_upper++)
                {
                    *mk_upper = toupper(*mk_upper);    // convert argument to uppercase
                }
            }
        }    
        sp = 1;    // reset char index
    }
	else if(*cp == '_') // if parameter has optional argument
	{
        // if there is an argument following the switch    
		if(!((++optind >= argc)||(argv[optind][0] == '-')))
		{
		    if (strchr("vV", c) != NULL)    // if a filename is expected
            {
                if(ReadSpacedArgs(argc, argv, &optind, FILENAME_UA_KS, RTE->verify_file_processed))
                {
                    return('?');
                }
                optarg = RTE->verify_file_processed;
            }
		}
		else
		{
            optarg = NULL;   // optarg points to NULL
		}
		sp = 1;      // reset char index
	}
    else    // if parameter has no arguments
    {
        if(argv[optind][++sp] == '\0')
        {
            sp = 1;      // reset char index
            optind++;    // move to next parameter
        }
        optarg = NULL;   // optarg points to NULL
    }
    return(c);
}

/*****************************************************************************/
//! \brief This function logs messages and prints them to the console
//! \param[in] type Type of the message
//!    - 0: standard message
//!    - 1: error message
//!    - 2: stage report message (anything ending in ellipses '...' and requiring an appended 'done' on completion)
//!    - 3: use to log user inputs or to print anything only to log and not to stdout
//!    - 4: use for loop-handled single line outputs
//! \param[in] fmt Pointer to C string, may include embedded format specifiers analogous to the 'printf' function
//! \return
int Print_N_Log(unsigned short type, const char* fmt, ...)
{
    time_t tm;
    char ctm[30];
    static unsigned short type_prev;
    char text[200];
    
    va_list arg_list;        
    va_start(arg_list, fmt);
    
    vsprintf(text, fmt, arg_list);
    
    // if logging is enabled
    if(!(inst_state & LOG_OFF))
    {
        if (type_prev == 2)    // if previous message was of 'REPORT' type
        {
            if (type == 2)     // ...and current message, too:
            {
                fprintf(lfile, "done\n");    // append 'done' to indicate completion
            }
            else
            {
                fprintf(lfile, "\n");
            }
        }
    }
    
    time(&tm);
    strcpy(ctm,ctime(&tm));
    *strchr(ctm, '\n') = '\0';
    
    // if logging is enabled
    if(!(inst_state & LOG_OFF))
    {
        if(type != 4)
        {
            fprintf(lfile, "%s:\t%s", ctm,text);
        }
        else
        {
            fprintf(lfile, "%s", text);
        }
    }
    
    // if console output is suppressed
    if(((inst_state & QUIET_MODE) && (type != 1)) || (type == 3))
    {
        type_prev = type;
        return 0;
    }

    if (inst_state & PROG_BAR_ON) 
    {
        return(printf("%s", text));
    }
    if (type_prev == 2)    // if previous message was of 'REPORT' type
    {
        if (type == 2)     // ...and current message, too:
        {
            printf("done\n");    // append 'done' to indicate completion
        }
        else
        {
            printf("\n");
        }
    }
    
    type_prev = type;
    
    return(printf("%s", text));
}

/*****************************************************************************/
//! \brief Process the argument string
//! \param[in] argc Number of arguments in the argument string
//! \param[in] argv[] Pointer to the argument string
//! \param[in] p_optind Argument index
//! \param[in] specs LSB 0: 1 - filename      , 0 - '[]' specifications
//!                      1: 1 - attached      , 0 - unattached
//!                      2: 1 - remove spacing, 0 - keep spacing
//! \param[out] processed Pointer to location for processed arguments
//! \return
int ReadSpacedArgs(int argc, char* argv[], int* p_optind, unsigned short specs, char* processed)
{
    char* optarg;               // pointer to first part of argument
    int optind = *p_optind;     // argument index
    int runtime_end;            // index to last part of argument
    
    if (specs & IS_ATTACHED)    // if argument is attached to trigger
    {
        // first part of argument follows trigger 
        optarg = &argv[optind][2];
    }
    else
    {
        // first part of argument is the following input argument (optind++ externally)
        optarg = argv[optind];
    }
    
    if (!(specs & IS_FILENAME))    // if argument is of type '[]' specs
    {                                                                        
        if (*optarg != '[')    // if the argument doesn't begin with '['
        {
            Print_N_Log(ERR_MSG,"* Error: '[' expected at beginning of\n");
            Print_N_Log(ERR_MSG,"* exit_spec list -- z\n");    
            return('?');
        }
    }
    strcpy(processed, optarg);    // copy first argument into processed
    
    if (!(specs & IS_FILENAME))    // if argument is of type '[]' specs
    {
        for (runtime_end = optind++; runtime_end < argc; runtime_end++)    // look through all following input args                            
        {                                                                  //optind++ (first part read already)                        
            if (strchr(argv[runtime_end], ']') != NULL )    // until ']' is reached, then:
            {
                break;    // stop counting
            }
        }                                                                    
        if (runtime_end >= argc)    // if ']' is not found
        {
            Print_N_Log(ERR_MSG,"* Error: ']' expected at end of\n");
            Print_N_Log(ERR_MSG,"* exit_spec list -- z\n\n");        
            return('?');
        }   // runtime_end indexes last part of arg                                                                    
    }    
    else    // else if argument is of type filename
    {
        for (runtime_end = optind++; runtime_end < argc; runtime_end++)    // search for an extension until found                            
        {                                                                                            
            if(    ( strstr(argv[runtime_end], ".txt") != NULL ) || ( strstr(argv[runtime_end], ".TXT") != NULL ) ||
                   ( strstr(argv[runtime_end], ".a43") != NULL ) || ( strstr(argv[runtime_end], ".A43") != NULL ) ||
                   ( strstr(argv[runtime_end], ".hex") != NULL ) || ( strstr(argv[runtime_end], ".HEX") != NULL ) ||
                   ( strstr(argv[runtime_end], ".int") != NULL ) || ( strstr(argv[runtime_end], ".INT") != NULL ))
            {
                break;
            }
        }
        if (runtime_end >= argc)    // if not found
        {
            Print_N_Log(ERR_MSG,"* file extension unknown -- w\n\n");
            return('?');
        }   // runtime_end indexes last part of arg
    }
    
    if (specs & REMOVE_SPACES)    // if remove ' ' is set
    {
        while (optind <= runtime_end)    // while unread parts exist
        {
            strcat(processed, argv[optind++]);    // add to processed, removing ' '
        }
    }
    else
    {
        while (optind <= runtime_end)    // while unread parts exist
        {
            *(strchr(processed, '\0')+1) = '\0';    // add to processed
            *strchr(processed, '\0') = ' ';         // w/o removing ' '
            strcat(processed, argv[optind++]);                                
        }
    }
    
    *p_optind = optind;    // update value of external optind
    return(0);
}

/*****************************************************************************/
//! \brief This function parses the input file containing the targetcode to
//! determine start address and length of the *first* memory block
//! \param[in] targetcode Name incl. path of the file containing the targetcode
//! \param[out] lStartAddress Pointer to memory location for the start address
//! \param[out] lProgramLength Pointer to location for memory block length
//! \return STATUS_OK if passed, STATUS_ERROR if failed to open file
STATUS_T ParseInputFile(char targetcode[], int32_t* lStartAddress, int32_t* lProgramLength)
{
    char* buffer = NULL;
	int32_t lSize = 0;
    size_t result;

	int16_t start = 0;
	int16_t length = 0;

    FILE* lInputFile = NULL;

    if((lInputFile = fopen(targetcode, "r")) == NULL)
    {
        Print_N_Log(ERR_MSG,"* Failed to open input file!\n");
        return STATUS_ERROR;
    }
    else
    {
        fseek(lInputFile , 0 , SEEK_END);
        lSize = ftell(lInputFile);
        rewind(lInputFile);

        buffer = (char*) malloc (sizeof(char)*lSize + 1);    // allocate memory
        result = fread(buffer, 1, lSize, lInputFile);        // copy file into the buffer
        buffer[lSize] = '\0';
        
        fclose(lInputFile);
        
        std::string tmp(buffer);
        free(buffer);

        if((strstr(targetcode, ".txt") != NULL) || (strstr(targetcode, ".TXT") != NULL))
        {
            std::smatch match;
            // regex pattern for TI-txt (captures full memory blocks)
            std::regex regex("@(.+)\n([^qQ@]+)");

            std::regex_search(tmp, match, regex);
            std::string address = match[1];
            std::string code = match[2];
        
            code.erase(remove(code.begin(), code.end(), ' '), code.end());    //remove whitespaces
            code.erase(remove(code.begin(), code.end(), '\n'), code.end()); //and newlines

            char * p;
			*lStartAddress = (int32_t)strtol(address.c_str(), &p, 16);
			*lProgramLength = (int32_t)(code.size()/2);
        }
        else if((strstr(targetcode, ".a43") != NULL) || (strstr(targetcode, ".A43") != NULL) ||
                (strstr(targetcode, ".hex") != NULL) || (strstr(targetcode, ".HEX") != NULL) ||
                (strstr(targetcode, ".int") != NULL) || (strstr(targetcode, ".INT") != NULL))
        {
            std::smatch match;
            // regex pattern for INTEL-hex (captures data from one line only)
            std::regex regex(":(\\w\\w)(\\w\\w\\w\\w)(\\w\\w)(\\w*)\\w\\w");
        
			int32_t next_address = 0;
			int32_t bytecount = 0;
			int32_t address = 0;
			int32_t recordtype = 0;
			int32_t prev_recordtype = 0;
			int32_t length = 0;

            while(std::regex_search(tmp, match, regex))
            {
                char * p;
                bytecount = strtol(string(match[1]).c_str(), &p, 16);
                address = strtol(string(match[2]).c_str(), &p, 16);
                recordtype = strtol(string(match[3]).c_str(), &p, 16);
                std::string data = match[4];
                
                length += bytecount;

                if(next_address)
                {
                    if((prev_recordtype == 2) || 
					   ((prev_recordtype == 4) &&
					   (recordtype == 0)))
                    {
                        *lStartAddress += address;
                    }
                    else if(address != next_address)
                    {
                        break;
                    }
                }
                else
                {
                    switch(recordtype)
                    {
                        case 0:
                            *lStartAddress = address;
                        case 2:
                            *lStartAddress = address << 4;
                        case 4:
                            *lStartAddress = address << 16;
                    }
                }

                next_address = address + bytecount;
                tmp.erase(0, bytecount + 12);
            }

            *lProgramLength = length;
        }

        return STATUS_OK;
    }
}

/*****************************************************************************/
/*                           END OF SOURCE FILE                              */
/*****************************************************************************/
