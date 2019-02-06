MSP-EXP430F5438 Experimenter's Board User Experience Demo v1.53.00
Texas Instruments, Inc.


********************************************************************************
Revision History
********************************************************************************
Ver 1.00 - May 2008 (D. Dang)
- Initial Version

Ver 1.10 - Jan 2009
- Review, documentation of code
- Removal of code for production test
- Additional support for MSP430EXP5438 Rev 1.x

Ver 1.20 - Oct 2009
- Added support for F5438A (PMM & REF module)
- Added support for use of TLV Device ID and ADC calibration values
- Built with Code Composer Studio v3.2

Ver 1.30 - August 2010 (K. Enderle)
- Reorganization of code
- Added ADCtemp, LPM, and FFT programs
- Added support for Grayscale text
- PMM menu modified to enhance ease of use
- Built with Code Composer Studio v4.0

Ver 1.31 - September 2010 (K. Enderle)
- MSP430F5438A-only
- Refactored project structure
- Improved various modules such as hal_lcd
- Added additonal user experience code such as an FFT demo and an ADC12
  power consumption demo

Ver 1.40 - May 2011 (W. Goh)
- Refactored project structure to share code folder
- Refactored code spacing
- Fixed adc temp calculation warnings
- Updated to use the latest HAL library

Ver 1.40.01 - June 2011 (A. Dannenberg)
- Fixed ImageHit JRE compatibility issue

Ver 1.50.00 - August 2011 (W. Goh)
- Added CCSv5 support. Software is not backward compatible with CCSv4.
- Adding binary folder with MSP430Flasher support
- Update to use 5xx/6xx core library v2.1

Ver 1.51.00 - May 2013 (E. Chen)
- Fixed Code Size Limited project build error

Ver 1.52.00 - Aug 2014 (E. Chen)
- Fixed Code Size Limited project build error due to CCS update

Ver 1.53.00 - Oct 2015 (E. Chen)
- Fixed IAR project build error due to IAR linker command change

********************************************************************************
Project File Listing
********************************************************************************
|
|--CCS
   |--MSP430F5438A.ccxml      : Configuration file for MSP430F5438A
   |--lnk_msp430f5438a_mod.cmd: Linker file
   |--system_pre_init.c       : Low level init to stop watchdog timer
|--CCS_Code_Size_Limited
   |--MSP430F5438A.ccxml      : Configuration file for MSP430F5438A
   |--lnk_msp430f5438a_mod.cmd: Linker file
   |--MSP-EXP430F5438_User_Experience_Lib.lib  :Library file containing files excluded from code limited build
   |--system_pre_init.c       : Low level init to stop watchdog timer
|--MSP-EXP430F5438_HAL        : folder containing driver files (Hardware Abstraction Layer)
   |--cc2500.h                : header file for Chipcon CC2500 EMK or EZ430-RF daughterboard
   |--hal_adc.h               : ADC driver header file
   |--hal_adc.c               : ADC driver source file
   |--hal_board.h             : Ports/Power Management/Clock System header file
   |--hal_board.c             : Ports/Power Management/Clock System source file
   |--hal_buttons.h           : Button driver header file
   |--hal_buttons.c           : Button driver source file
   |--hal_lcd.h               : LCD driver header file
   |--hal_lcd.c               : LCD driver source file
   |--hal_lcd_fonts.c         : ASCII font for LCD
   |--hal_lcd_fonts.h         : ASCII font for LCD
   |--hal_rf.h                : RF driver header file
   |--hal_rf.c                : RF driver source file
   |--hal_rf_settings.h       : RF settings header file
   |--hal_rtc.h               : RTC headerfile
   |--hal_rtc.c               : RTC source file
   |--hal_usb.h               : USB driver header file
   |--hal_usb.c               : USB driver source file
   |--hal_MSP-EXP430F5438.h   : Master header file to be singly included in MSP-EXP430F5438 HAL project
|--F5XX_6XX_Core_Lib          : folder containing new hal files
   |--hal_FLASH.h             : FLASH driver header file
   |--hal_FLASH.c             : FLASH driver source file
   |--hal_macros.h            : header containing macros
   |--hal_PMAP.h              : PMAP driver header file
   |--hal_PMAP.c              : PMAP driver source file
   |--hal_PMM.h               : PMM driver header file
   |--hal_PMM.c               : PMM driver source file
   |--hal_TLV.h               : TLV driver header file
   |--hal_TLV.c               : TLV driver source file
   |--hal_UCS.h               : UCS driver header file
   |--hal_UCS.c               : UCS driver source file
|--UserExperienceDemo         : folder containing User Experience Code
   |--5xx_ACTIVE_test.h       : Active test header file
   |--5xx_ACTIVE_test.s43     : Assembly file for active test
   |--5xx_ACTIVE_test.asm     : Assembly file for active test
   |--flashUtils.h            : Flash utilities header file
   |--flashUtils.c            : Flash utilities source file
   |--adcTemp.h               : adcTemp header file
   |--adcTemp.c               : Uses different adc setups to measure the temperature
   |--audio.h                 : Voice Recorder and Playback header file
   |--audio.c                 : Voice Recorder and Playback
   |--balanceBall.h           : balanceBall header file
   |--balanceBall.c           : Use accelerometer to control a ball on LCD
   |--clock.h                 : Analog & digital clocks header file
   |--clock.c                 : Analog & digital clocks
   |--FFT.h                   : FFT header file
   |--FFT.c                   : Performs/displays FFT of voice input
   |--FFT_430.h               : FFT assembly header file
   |--FFT_430.s43             : Assembly file used to perform FFT
   |--FFT_430.asm             : Assembly file used to perform FFT
   |--LPM.h                   : LPM header file
   |--LPM.c                   : Allows user to test various low-power modes
   |--menuSetting.c           : Menu header file
   |--menuSetting.c           : Menu for user settings and calibrations
   |--PMM.c                   : PMM header file
   |--PMM.c                   : Select VCORE and DCO speed in active mode
   |--PowerTest.h             : PowerTest header file
   |--PowerTest.c             : Select between Active and LPM tests
   |--usbTest.h               : usbTest header file
   |--usbTest.c               : UART USB communication with PC host
   |--UserExperienceGraphics.c: Contains images for lcd display
   |--UserExperienceGraphics.h: Contains images for lcd display
   |--UserExperience_F5438A.h : User experience header
   |--UserExperience_F5438A.c : main file, GUI, menu, and event handlers.
|--IAR                        : Folder containing IAR Project files
   |--low_level_init.c        : Low level init to stop watchdog timer
   |--lnk430F5438A_mod.xcl    : Linker file
|--main.c                     : calls UserExperience.c
|--readme.txt                 : this file