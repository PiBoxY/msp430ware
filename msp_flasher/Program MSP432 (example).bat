CLS
REM -n switch is mandatory for MSP432!
MSP430Flasher.exe -n "MSP432P401R" -w "Firmware432.txt" -v -g -z [VCC] 
pause
MSP430Flasher.exe -n "MSP432P401R" -r [FirmwareOutput432.txt,MAIN]
pause