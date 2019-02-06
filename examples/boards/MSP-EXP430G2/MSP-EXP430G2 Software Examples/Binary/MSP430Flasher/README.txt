MSP430 Flasher - Command Line programmer
For usage instructions please see http://processors.wiki.ti.com/index.php/MSP430_Flasher_-_Command_Line_Programmer

Note: When using the MSP430 Flasher with the MSP430DLLv3 Make sure to update the firmware of your FET430 USB Emulator first.
For instructions visit http://processors.wiki.ti.com/index.php/MSP_Debug_Stack#Upgrade_process_to_MSP430.DLLv3

Change Log:
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

