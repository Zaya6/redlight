@echo off

setlocal
set PATH=%CD%\ext\win64;%PATH%
set REDASSETDIR=%CD%\assets\
call bin\redlight.exe

endlocal
