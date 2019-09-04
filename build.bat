@echo off
:start
hbmk2 %1
IF %ERRORLEVEL% GTR 1000 goto setup
goto :eof

:setup
set HB_INSTALL_PREFIX=C:\harbour32
SET CD_SRC=%CD%
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"
cd %CD_SRC%
set path=%PATH%;%HB_INSTALL_PREFIX%\bin;%HB_INSTALL_PREFIX%\bin\win\msvc
goto:start

