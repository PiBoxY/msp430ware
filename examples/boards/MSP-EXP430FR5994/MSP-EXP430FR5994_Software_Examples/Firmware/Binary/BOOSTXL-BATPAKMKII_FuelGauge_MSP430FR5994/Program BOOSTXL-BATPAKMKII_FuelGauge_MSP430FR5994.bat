CLS
@echo off
rem This is a small helper script that programs a prebuilt binary for a device using MSP430Flasher
rem
rem Eric Chen, MSP430 Applications, Texas Instruments, Inc.
rem Last modified: 03/30/2015
:input
@echo MSP-EXP430FR5994-LaunchPad Firmware Programmer

set device=MSP430FR5994
set firmware=BOOSTXL-BATPAKMKII_FuelGauge_MSP430FR5994.txt

@echo Programing %firmware% into %device% ......
..\MSP430Flasher\MSP430Flasher.exe -n %device% -w %firmware% -v -g -z [VCC]

pause