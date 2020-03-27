@echo off
:start
hbmk2 %1 %2 %3 %4 %5 %6 %7 %8 %9
IF %ERRORLEVEL% GTR 1000 goto setup
goto :eof

:setup
set HB_INSTALL_PREFIX=C:\harbour
SET CD_SRC=%CD%
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
cd %CD_SRC%
set path=%PATH%;%HB_INSTALL_PREFIX%\bin;%HB_INSTALL_PREFIX%\bin\win\msvc64
goto:start

