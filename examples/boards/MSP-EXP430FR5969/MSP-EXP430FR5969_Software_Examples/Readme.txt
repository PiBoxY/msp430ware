MSP-EXP430FR5969 Software Examples v1.04.01
Texas Instruments, Inc.


********************************************************************************
Welcome to the MSP-EXP430FR5969 LaunchPad!
********************************************************************************

In this package, you will find software examples and source codes that can help
you quickly get started on developing your own project.

The software example projects are located in the 'Source' folder:
 1. 430BOOST-SHARP96_ULP_FRAM            | SHARP96x96 LCD display demo source
                                            code
 2. 430BOOST-SHARP96_GrlibExample_FR5969 | Grlib Example for SHARP96x96 LCD
                                            display source code
 3. BOOSTXL-SHARP128_GrlibExample_FR5969 | Grlib Example for SHARP128x128 LCD
                                            display source code
 4. OutOfBox_MSP430FR5969                | The out of box demo source code

In addition, prebuilt TI-TXT images for each example are located in the 'Binary'
folder. These images are ready to be downloaded onto your LaunchPad using a
MSP430 programming interface, such as UniFlash. Visit https://dev.ti.com/uniflash/
to start downloading examples to your LaunchPad!

You can also find various web links in the 'Documentation' folder pointing to
the Tool Page, Quick Start Guide, and User's Guide

For more resources, be sure to visit www.ti.com/tool/msp-exp430fr5969


********************************************************************************
Revision History
********************************************************************************
Ver 1.04.01 - Sept 15th, 2018
- Added Sharp128 Display example source code

Ver 1.04.00 - June 2nd, 2017
- Fixed OOB Demo firmware bugs
- Added Readme for using cloud UniFlash with pre-built TI-TXT images

Ver 1.03.01 - Oct 29th, 2015
- Fixed 430BOOST-SHARP96_ULP_FRAM build error in IAR due to linker command file update

Ver 1.03.00 - June 22nd, 2015
- Added MacOSX + Linux support for MSP430Flasher

Ver 1.02.00 - March 24th, 2015
- Updated DriverLib and GrLib in software examples

Ver 1.01.00 - June 23rd, 2014
- Updated release packages to distribute hardware and software files separately
- Updated Sharp96x96 LCD driver files for Grlib examples
- Added new OutOfBox_FR5969 demo project and GUI

Ver 1.00.02 - April 2nd, 2014
- Fixed software import/build errors on Linux CCS

Ver 1.00.01 - March 13th, 2014
- Fixed Hardware BOM excel file extension

Ver 1.00.00 - February 18th, 2014
- Initial Release


********************************************************************************
Project File Listing
********************************************************************************
|--Documentation                            : Folder containing links to more resources
|--Drivers                                  : Folder containing driver files
   |--Emulation                             : Contains link to MSP430 FET USB driver
|-Firmware                                  : Folder containing example firmwares
  |-Binary                                  : Prebuilt example firmware images
  |-Source                                  : Example firmware source projects
    |--430BOOST-SHARP96_ULP_FRAM            : Sharp96x96 LCD ULP_FRAM Demo source code
    |--430BOOST-SHARP96_GrlibExample_FR5969 : Sharp96 Display Example source code
    |--BOOSTXL-SHARP128_GrlibExample_FR5969 : Sharp128 Display Example source code
    |--OutOfBox_MSP430FR5969                : Out of box demo source code
|-GUI                                       : Folder containing GUI related files
  |-Binary                                  : GUI executables
  |-Source                                  : GUI Source
    |-OutOfBox_MSP432P401R_GUI.zip          : Out of box demo GUI source code
|--Readme.txt                               : This file
