@echo off

echo Installation for OpenAL++ started!
echo If something goes wrong, make sure the paths in W32INST.BAT are correct
nmake /f openalpp.mak
echo Library compiled and linked!
echo Now for the installation

set ALPPINCLUDEPATH=C:\PROGRAM\MICROS~1\VC98\INCLUDE\ALPP
set ALPPLIBPATH=C:\PROGRAM\MICROS~1\VC98\LIB

mkdir %ALPPINCLUDEPATH%
copy /Y include\*.h %ALPPINCLUDEPATH%
copy /Y lib\openalpp.lib %ALPPLIBPATH%

set ALPPINCLUDEPATH=
set ALPPLIBPATH=

echo Installation complete!