@echo off
rem Convenience shell script to start ImageHit using the Java runtime environment
rem that is bundled with CCSv4.0. This is because ImageHit needs at least J2SE 5.0.
rem (C)2010 Texas Instruments, Inc.
rem Program by Andreas Dannenberg

set CCS_PATH=C:\Program Files\Texas Instruments\ccsv4

if exist "%CCS_PATH%\." goto continue

echo Error: %CCS_PATH% does not exist!
echo Either change the CCS_PATH variable located in this batch file to reflect
echo the location of your CCS installation, or run the ImageHit Java class directly
echo using your favorite JRE by executing "java.exe ImageHit [-v] file"
echo Note that J2SE 5.0 (or higher) is required for ImageHit.
goto :eof

:continue
"%CCS_PATH%\eclipse\jre\bin\java.exe" ImageHit %1 %2 %3 %4 %5
