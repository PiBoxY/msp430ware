MSP-EXP430F6137Rx Experimenter's Board User Experience Demo v1.0.0.0
Production & Test version
Texas Instruments, Inc.

********************************************************************************
Revision History
********************************************************************************

Ver 1.0.0.0 - April 25th, 2011
D. Dang
- Initial Production Release

********************************************************************************
Project File Listing
********************************************************************************
|
|--CCS                        : Folder containing the CCSv4 project files
   |--lnk_msp430f6137.cmd     : Device linker command file   
   |--system_pre_init.c       : CCS-specific runtime library customization
|--F5xx_F6x_Core_Lib          : Folder containing MSP430 low-level HAL (SLAA448)
|--IAR                        : Folder containing the IAR EW430 project files
   |--low_level_init.c        : IAR-specific runtime library customization
|--MSP-EXP430F6137_HAL        : Folder containing board-level HAL
|--SimpliciTI                 : Folder containing the SimpliciTI wireless stack
   |--Application             
      |--configuration        : Folder containing customized network configurations    
|--readme.txt                 : This file
|--MspExp430F6137RxUserExperience.c                 : Main User Experience file
|--MSP-EXP430F6137Rx User Experience Manifest.pdf   : Software manifest