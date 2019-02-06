@echo off

set PATH=%~dp0\util;%PATH%

if "%1"=="not_chrome" goto NOT_CHROME else goto CHROME

:CHROME

"%~dp0/node.exe" "%~dp0/src/main_chrome.js" %*
goto END

:NOT_CHROME

"%~dp0/node.exe" "%~dp0/src/main.js" %*

:END

