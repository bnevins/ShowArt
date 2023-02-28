REM copy ..\..\dist\*.* "C:\Program Files\Elf\Showart"
set target=c:\temp
echo %target%
copy e:\dev\cpp\win\showart\Debug\Showartd.exe %target%
copy e:\dev\cpp\win\showart\Release\Showart.exe %target%
copy e:\dev\cpp\win\diblib\Debug\bndibd.dll %target%
copy e:\dev\cpp\win\diblib\Release\bndib.dll %target%
set target=
