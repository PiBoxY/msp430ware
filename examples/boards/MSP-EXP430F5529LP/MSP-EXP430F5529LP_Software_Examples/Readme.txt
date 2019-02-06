MSP-EXP430F5529LP Software Examples v1.06.00
Texas Instruments, Inc.


********************************************************************************
Welcome to the MSP-EXP430F5529LP LaunchPad!
********************************************************************************

In this package, you will find software examples and source codes that can help
you quickly get started on developing your own project.

The software example projects are located in the 'Source' folder:
 1. OutOfBox_EmulStorageKeyboard  | Out-of-Box Demo source code
 2. SimpleUsbBackchannel          | USB communication via Backchannel UART
                                    Example source code
 
In addition, prebuilt TI-TXT images for each example are located in the 'Binary'
folder. These images are ready to be downloaded onto your LaunchPad using a
MSP430 programming interface, such as UniFlash. Visit https://dev.ti.com/uniflash/
to start downloading examples to your LaunchPad!

You can also find various web links in the 'Documentation' folder pointing to
the Tool Page, Quick Start Guide, User's Guide and MSP430 USB Developers Package.

For more resources, be sure to visit http://www.ti.com/tool/MSP-EXP430F5529LP
Also checkout the Wiki page at http://processors.wiki.ti.com/index.php/MSP430F5529_LaunchPad


********************************************************************************
Revision History
********************************************************************************
Ver 1.06.00 - June 2nd, 2017
- Update to Hardware files  Revision 1.7
- Removed MSP430Flasher and added Readme for using cloud UniFlash with pre-built TI-TXT images

Ver 1.05.00 - June 22nd, 2015
- Added MacOSX + Linux support for MSP430Flasher

Ver 1.04.00 - March 24th, 2015
- Updated driverlib and USB stack in software examples

Ver 1.03.00 - September 26th, 2014
- Replaced backslashes in projectSpec with forward slahses to be compatible 
  with Unix environments

Ver 1.02.00 - June 23rd, 2014
- Updated release packages to distribute hardware and software files separately
- Renamed out of box demo project

Ver 1.01.00 - Jan 2nd, 2014
- Updated Hardware files to Revision 1.5

Ver 1.00.01 - October 09th, 2013
- Added Code Size limited project for emulStorageKeyboard
- Added new folder to include emulation tool driver and simpleUsbBackChannel driver

Ver 1.00.00 - August 27th, 2013
- Initial Release


********************************************************************************
Project File Listing
********************************************************************************
|--Documentation                   : Folder containing links to more resources
|--Drivers                         : Folder containing driver files
   |--Emulation                    : Contains link to MSP430 FET USB driver
   |--SimpleUsbBackChannel         : Contains USB CDC driver for SimpleUsbBackChannel example
|--Firmware                        : Folder containing example firmwares
   |--Binary                       : Folder containing TI-TXT image files and
   |                                 MSP430Flasher
   |--Software                     : Folder containing software examples
      |--OutOfBox_EmulStorageKeyboard : Out-of-Box Demo example source code
      |--SimpleUsbBackchannel      : USB communication via Backchannel UART example source code
|--Readme.txt                      : This file