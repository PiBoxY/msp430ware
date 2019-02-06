CLS
@echo off
rem This is a small helper script that runs through the test collection for one device (folder)
rem
rem Dung Dang, MSP430 Applications, Texas Instruments, Inc.
rem Last modified: 02/25/2011
:input
@echo MSP-EXP430G2-LaunchPad Firmware Programmer

set device=MSP430G2231
set firmware=MSP-EXP430G2-LaunchPad.txt

@echo Programing %firmware% into %device% ......
..\..\MSP430Flasher\MSP430Flasher.exe -n %device% -w %firmware% -v -g -z [VCC]

pause