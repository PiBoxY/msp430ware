@echo off
CLS
rem This is a small helper script that automatically downloads the 
rem latest binary image for MSP-EXP430F438 User Experience code.
rem
rem William Goh, MSP430 Applications, Texas Instruments, Inc.
rem Last modified: 08/12/2011
set device=MSP430F438
set firmware=MSP-EXP430F5438UserExperience.txt
goto program

:program
@echo Programing %firmware% into %device% ......
IF exist log ( echo Log directory already exist ) ELSE ( mkdir log && echo Log directory created)
MSP430Flasher.exe -n %device% -w %firmware% -v -g -z [VCC]

pause