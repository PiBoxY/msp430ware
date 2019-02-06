CLS
@echo off
rem This is a small helper script that runs through the test collection for one device (folder)
rem
rem Dung Dang, MSP430 Applications, Texas Instruments, Inc.
rem Last modified: 06/25/2011
:input
@echo Select the device and frequency version of the firmware
@echo 1. MSPEXP430F6137Rx --- US 915 MHz
@echo 2. MSPEXP430F6137Rx --- EU 868 MHz
@echo 3. MSPEXP430F6137Rx --- LF 433 MHz
@echo 4. MSPEXP430F5137Rx --- US 915 MHz
@echo 5. MSPEXP430F5137Rx --- EU 868 MHz
@echo 6. MSPEXP430F5137Rx --- LF 413 MHz
@echo 7. Exit
set INPUT=
set /P INPUT=Select firwmware [1-7]: %=%

rem if "%1" == "" (
rem 	echo Missing device parameter! Please specify a device!
rem 	exit /B
rem )
rem if "%2" == "" (
rem 	echo Missing TXT file
rem 	exit /B
rem )
rem MSP430Flasher.exe -n %1 -w "%2" -v -g -z [VCC] 
if "%INPUT%"=="1" goto :1
if "%INPUT%"=="2" goto :2
if "%INPUT%"=="3" goto :3
if "%INPUT%"=="4" goto :4
if "%INPUT%"=="5" goto :5
if "%INPUT%"=="6" goto :6
if "%INPUT%"=="7" goto :7
@echo Please enter [1-7]
goto input
:1 
set device=CC430F6137
set firmware=MSP-EXP430F6137RxUserExperience_US_915MHz.txt
goto program
:2 
set device=CC430F6137
set firmware=MSP-EXP430F6137RxUserExperience_EU_868MHz.txt
goto program
:3 
set device=CC430F6137
set firmware=MSP-EXP430F6137RxUserExperience_LF_433MHz.txt
goto program
:4 
set device=CC430F5137
set firmware=MSP-EXP430F5137RxUserExperience_US_915MHz.txt
goto program
:5 
set device=CC430F5137
set firmware=MSP-EXP430F5137RxUserExperience_EU_868MHz.txt
goto program
:6 
set device=CC430F5137
set firmware=MSP-EXP430F5137RxUserExperience_LF_433MHz.txt
goto program


:program
@echo Programing %firmware% into %device% ......
MSP430Flasher.exe -n %device% -w %firmware% -v -g -z [VCC]
goto input

:7 
@echo Goodbye, happy RFing!
pause