pushd Installer\Release

del showart6.5.exe
del showart.cab
pause
\dev\cabsdk\BIN\cabarc -r -p n showart.cab *.*
pause
d:\dev\SelfExt\CabAttr\cabattr +x showart.cab setup.exe
pause
copy /b D:\dev\SelfExt\FRelease\selfext.exe+showart.cab showart7_0.exe
rem del showart.cab
dir
popd

