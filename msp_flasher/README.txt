MSP430 Flasher - Command Line programmer (http://www.ti.com/tool/msp430-flasher)
For usage instructions please see SLAU654 (http://www.ti.com/lit/pdf/slau654)


Change Log:
v1.3.10 July 15, 2016
- Updated Flasher packages with MSP Debug Stack v3.8.1.0

v1.3.9 June 9, 2016
- Updated Flasher packages with MSP Debug Stack v3.8.0.2

v1.3.8 February 22, 2016
- Updated Flasher packages with MSP Debug Stack v3.7.0.12
- Added support for MSP432
- Deprecated breakpoint functionality (-d/-t switches)
- Fixed issue with serial port handling on Linux
- Created new native 64bit Flasher package for Linux
- Added uninstaller to Flasher packages
- Moved Flasher user's guide to ti.com (SLAU654)

v1.3.7 August 11, 2015
- Updated Flasher packages with MSP Debug Stack v3.5.1.1

v1.3.6 July 24, 2015
- Updated Flasher packages with MSP Debug Stack v3.5.1.0

v1.3.5 June 30, 2015
- Fixed stand-alone erase (-e used without -w)

v1.3.4 May 8, 2015
- Updated Flasher packages with MSP Debug Stack v3.5.0.1
- Added support for OSX
- Added -j switch to set FET JTAG/SBW speed
- MSP430 Flasher can now be built under 64-bit Linux
- JTAG mode selection is always forced to automatic detection (-m switch will be ignored)
- Password length option (-l) is now obsolete and will be ignored
- Fixed a bug where upper case file extensions would not be recognized properly
- Fixed issue with long file paths

v1.3.3 November 21, 2014
- Updated Flasher packages with MSP Debug Stack v3.4.3.4
- Added support for RF430FRL152H family
- Added non-intrusive device access option (-a)
- Added stand-alone verify option
- Added NO_ERASE option for -e switch
- Added NO_TARGET option for -n switch
- Added FET detection sweep option (-i DETECT)
- Fixed segment erase for memory segments in high memory (>0xFFFF)

v1.3.2 September 3, 2014
- Updated Flasher packages with MSP Debug Stack v3.4.2.7
- Enhanced –r switch to cover read-out of custom memory ranges
- Target device name switch (-n) is now optional. User prompt in case of mismatch
- Fixed segment erase for memory segments in high memory (>0xFFFF)

v1.3.1 April 29, 2014
- Updated Flasher packages with MSP Debug Stack v3.4.1.0
- Added "ERASE_TOTAL" and "ERASE_USER_CODE" options
- List of connected debuggers is now displayed during initialization
- Fixed disconnect/power-down flow
- Minor wording/text placement changes

v1.3.0 February 11, 2014
- Updated Flasher packages with MSP Debug Stack v3.4.0.20
- MSP430 Flasher can now be built and executed on Linux
- Memory can now be erased without being programmed afterwards
- Added exit spec to set VCC to a specific value
- Removed outdated timer functionality

v1.2.3 September 19, 2013
- Updated Flasher package with MSP430.DLLv3.3.1.4
- Improved feedback during FET initialization flow in case FET is already in use
- Fixed BSL unlock issue
- Fixed Reset flow for exit specs [VCC, RESET]

v1.2.2 July 16, 2013
- Updated Flasher package with MSP430.DLLv3.3.1.3
- Added segment erase feature
- Included windows.h instead of afxres.h in Version.rc

v1.2.1 January 31, 2013
- Updated Flasher package with MSP430.DLLv3.3.0.6
- If logging is enabled, directory 'Log' will now automatically be created if missing
- Fixed automatic interface detection for eZ430 tools (Launchpad/EXP-boards/...)
- Fixed minor issue with displaced graphics
- Changed default install directory.

v1.2.0 December 17, 2012
- FW update suppression switch (-s) now only suppresses the user prompt.
  A firmware update is/must be performed if there's a mismatch between FW and DLL versions
- Added exit spec [RESET] to perform a hard reset on exit
- Added support for eZ-FET recovery
- It's now possible to read out device memory larger than 128kB
- Improved error handling
- Removed out-dated/non-used functions
- Version information is now displayed on each Flasher execution
- All functions in the Flasher project were enhanced with doxygen style comments
- In-code commentaries have been improved
- Functions with no DLL interaction have been moved to Support.cpp
- High-Level function calls have been moved to MSP430_Flasher.cpp
- The debug build now compiles to "MSP430Flasher_debug.exe" in the same directory as the release binary. No copying of the MSP430.dll/HIL.dll should be required any longer to debug the MSP430 Flasher.

v1.1.9 September 21, 2012
- Added MSP430.dll v3 (3.2.5.4) to the MSP430 Flasher package

v1.1.8 June 28, 2012
- Added MSP430.dll v3 (3.2.4.5) to the MSP430 Flasher package

v1.1.7 February 24, 2012
- Fixed disable logging option (-g)

v1.1.6 February 08, 2012
- Fixed BSL Unlock Mode

v1.1.5 January 11, 2012
- Added support for UIF major firmware update
- Added MSP430.dll v3 (3.2.3.2) to the MSP430 Flasher package

v1.1.4 November 07, 2011
- Added MSP430.dll v3 (3.2.1.9) to the MSP430 Flasher package

v1.1.3 August 22, 2011
- Updated MSP430 Flasher package with MSP430.dll v2.4.9.1

v1.1.2 May 10, 2011
- Added FET FW update suppression switch (-s)

v1.1.1 May 3, 2011
- Added breakpoint timeout switch (-t)
- MSP430Flasher.exe now returns DLL error codes
- Fixed logfile bug in quiet mode

v1.1.0 Mar. 8, 2011
- Updated MSP430 Flasher package with MSP430.dll v2.4.8.2

v1.0.1 Jan. 18, 2011
- Added Locked Flash Access Switch (-u)
- Added new MSP430.dll (v2.4.7.1)
- Added BSL Unlock Mode
- Added documentation to binary package
- Fixed Manual COM Port Selection
- Returned to the original versioning scheme. Version number is no longer connected to the one of the msp430.dll included with the package.

v2.4.6.1 Nov. 11, 2010
- Added support to blow the JTAG fuse
- Fixed bug where MSP430F413 would not be identified correctly
- Updated version number from v1.0.0 to v2.4.6.1.  The new versioning scheme is aligned with the current msp430.dll version that included with the package.  Future releases that use the same MSP430.dll version will update the fifth digit in the version number (i.e. v2.4.6.1).

v1.0.0 Released Oct 28, 2010
- Initial released


