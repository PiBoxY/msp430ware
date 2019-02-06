MSP430 Flasher - Command Line programmer
For usage instructions please see http://processors.wiki.ti.com/index.php/MSP430_Flasher_-_Command_Line_Programmer

Note: When using the MSP430 Flasher with the MSP430DLLv3 Make sure to update the firmware of your FET430 USB Emulator first.
For instructions visit http://processors.wiki.ti.com/index.php/MSP_Debug_Stack#Upgrade_process_to_MSP430.DLLv3

Change Log:
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


