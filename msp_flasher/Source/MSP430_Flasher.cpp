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
* \mainpage
*
* \par MSP Flasher is a user-friendly, shell-based interface providing the user with easy access to MSP430 and MSP432 devices and the ability to program files into these devices.
*
* \par Usage in Command Prompt: MSP430Flasher [OPTIONS]
*
* \par Developed using MS Visual Studio 2010
*
* \par (c) 2010 - 2016 by Texas Instruments
*
* \author Robert Lessmeier
*
* \version 1.3.10.0
*
* \file MSP430_Flasher.cpp
*
* \par This file holds the main() function as well as the main Flasher routine with all top level function calls
**/

/*****************************************************************************/
/*                                 INCLUDES                                  */
/*****************************************************************************/

#include "StdUseCase.h"

/*****************************************************************************/
/*                                 VARIABLES                                 */
/*****************************************************************************/

extern sRTE_t g_RTE;
extern sDef_t g_Def;
extern int32_t BPAddr[];

/*****************************************************************************/
/*                                 FUNCTIONS                                 */
/*****************************************************************************/

//***************************************************************************//
//! \brief Main function
//! \param[in] argc Number of arguments in the argument string
//! \param[in] argv Pointer to the argument string
int main(int argc, char* argv[])
{
    int RetState = 0;
    
    // Prepare default options
    g_sOpt.pcInterface = g_Def.defInterface;
    g_sOpt.pcEraseType = g_Def.defEraseType;
    g_sOpt.pcVerify    = g_Def.defFALSE;
    g_sOpt.pcVCC       = 0;
    g_sOpt.pcPassword  = g_Def.defPassword;
    g_sOpt.pcTimeout   = g_Def.defTimeout;
    
    // Execute Flasher routine
    RetState = Flasher(argc, argv, &g_sOpt, &g_RTE);

    switch(RetState)
    {
        case 1:
            Help();
            return RetState;
            break;
        case 2:
            Exit_Specs();
            return RetState;
            break;
    }

    StdUseCase_Close(&g_sOpt);
    return RetState;
}

//***************************************************************************//
//! \brief Main Flasher routine
//! \param[in] argc Number of arguments in the argument string
//! \param[in] argv[] Pointer to the argument string
//! \param[in] sOpt Pointer to struct for options from command line and/or 
//! list of default options
//! \param[in] RTE Pointer to struct for options entered at runtime
//! \return STATUS_OK: The initialization was successful.\n
//! STATUS_ERROR: The initialization failed.
int Flasher(int argc, char* argv[], sDefOptions_t* sOpt, sRTE_t* RTE)
{
    int RetState = 0;

//==ENABLE LOG===============================================================//
    
    EnableLog(argc, argv);

//==PRINT VERSION INFO=======================================================//

    PrintVersion();

//==EVALUATE STANDARD OPTIONS================================================//

    RetState = EvaluateOptions(argc, argv, sOpt, RTE);

//==INITIALIZE INTERFACE=====================================================//
    
    // std options are evaluated, continue initialization
    if(!RetState)
    {        
        StdUseCase_InterfaceInit(&g_sOpt);
        
//==CHECK FIRMWARE COMPATIBILITY=============================================//

        StdUseCase_CheckFirmware(&g_sOpt);
    
//==READ FW and HW VERSIONS==================================================//

        StdUseCase_ReadFWVersion();
        StdUseCase_ReadHWVersion();

//==POWER UP ===============================================================//

        StdUseCase_PowerUp(&g_sOpt);

//==GET DEVICE UNDER JTAG CONTROL============================================//
		
		StdUseCase_SpecialErase(&g_sOpt);
        StdUseCase_GetDevice(&g_sOpt);

        // if Breakpoints should be set
        if(sOpt->pcBreakpoints != NULL)
        {
            StdUseCase_EemInit();
        }

//==ERASE MEMORY (optional)==================================================//

        StdUseCase_Erase(&g_sOpt);

//==PROGRAM & VERIFY / READ OUT DEVICE MEMORY================================//

        StdUseCase_ReadWrite(&g_sOpt, &g_RTE);

//==RESET DEVICE=============================================================//

        StdUseCase_Reset(PUC_RESET, 0, 0);

//==SET BREAKPOINTS==========================================================//

        // breakpoints are only set if -d switch is used
        if(sOpt->pcBreakpoints != NULL)
        {
            StdUseCase_SetBreakpoints(BPAddr);
        }

//==CHECK IF DEVICE SHOULD BE SECURED========================================//

        // target device is only secured if -f switch is used
        StdUseCase_BlowFuse();

//==DISPLAY INITIALIZATION RESULTS===========================================//

        StdUseCase_DisplayResults(argc, argv, &g_sOpt);

//==RUN TO BREAKPOINT(S)=====================================================//

        StdUseCase_RunProgram(&g_sOpt, &BPAddr[0]);
    }
        
//===========================================================================//
    
    return (RetState);
}

/*****************************************************************************/
/*                           END OF SOURCE FILE                              */
/*****************************************************************************/
