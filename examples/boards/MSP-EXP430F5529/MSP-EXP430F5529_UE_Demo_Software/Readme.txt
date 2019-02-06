MSP-EXP430F5529 Experimenter's Board User Experience Demo v1.40.00
Texas Instruments, Inc.


********************************************************************************
Revision History
********************************************************************************
Ver 1.40.00 - August 24th, 2015
- Added new hardware Rev2.2 design files
- Updated User Experience demo to work with new replacement accelerometer

Ver 1.31.00 - April 9th, 2013
- Updated Software to use MSP430 USB Developers Package v3.20.02
- Updated Capacitive Touch Library to v1.1.0
- Updated 5xx6xxCoreLib to v2.1

Ver 1.31.00 - April 9th, 2013
- Updated Software to use MSP430 USB Developers Package v3.20.02
- Updated Capacitive Touch Library to v1.1.0
- Updated 5xx6xxCoreLib to v2.1

Ver 1.30.00 - March 21th, 2012
- Updated Software to use MSP430 USB Developers Package v3.11.
- Modified EchoUsb.c to initalize SD-CARD interface.

Ver 1.20.01 - August 12th, 2011
- Modified MSP430Flasher.bat to automatically create log folder
- Modified Settings.h to reflect the latest version

Ver 1.20.00 - August 3rd, 2011
- Added support for CCSv5

Ver 1.10.01 - May 26th, 2011
- Fixed ImageDog JRE compatibility issue
- Changed default installer directory

Ver 1.10 - May 5th, 2011
- Added "Drivers" folder
- Added limited code size project for CCS
- Changed CCS optimizer settings
- General cleanup / bug fixes

Ver 1.00 - April 7th, 2011
- Initial Production Release


********************************************************************************
Project File Listing
********************************************************************************
|--Documentation                    : Folder containing links to more resources
|--Firmware                         : Folder containing example firmware
   |--Binary                        : Prebuilt example firmware images
   |--Source
      |--CCS                        : Folder containing the CCSv4 project files
         |--lnk_msp430f5529.cmd     : Device linker command file
         |--lnk_msp430f5529_USB.cmd : USB-stack related linker command file
         |--system_pre_init.c       : CCS-specific runtime library customization
      |--CCS_Code_Size_Limited      : Folder containing the CCSv4 project files - 16KB
      |--CTS                        : Folder containing SW library for capacitive touch
      |--Drivers                    : Drivers for USB demo functionality
      |--F5xx_F6x_Core_Lib          : Folder containing MSP430 low-level HAL (SLAA448)
      |--FatFs                      : Folder containing the open-source FatFs file system
      |--IAR                        : Folder containing the IAR EW430 project files
         |--low_level_init.c        : IAR-specific runtime library customization
      |--ImageDog                   : Folder containing an image converter tool
      |--MSP-EXP430F5529_HAL        : Folder containing board-level HAL
      |--USB                        : Folder containing the MSP430 USB stack
         |--USB_User                : USB-stack related application files
      |--UserExperienceDemo         : Folder containing User Experience Code application
      |--UserExperience.c           : Contains main entry point
|--Readme.txt                       : This file
|--MSP-EXP430F5529 User Experience Manifest.pdf   : Software manifest
