
PATH=%PATH%;C:\Perl64\bin

call "%VS140COMNTOOLS%\vsvars32.bat"
rem echo set PATH=%PATH%;%VSDIR%\VC\bin\x86_amd64;


msbuild "BrExternalBuild.sln" /t:build /p:Configuration=Debug
if %errorlevel% neq 0 (
echo Error on build
goto quit
) else (
echo continue
)

msbuild "BrExternalBuild.sln" /t:build /p:Configuration=Release
if %errorlevel% neq 0 (
echo Error on build
goto quit
) else (
echo continue
)


xcopy Server\bin\lib\x64Debug\*.dll Server\bin\x64Debug\ /y
xcopy Server\bin\lib\x64Release\*.dll Server\bin\x64Release\ /y
xcopy Server\bin\lib\x64Debug\*.dll TestApp\x64Debug\ /y
xcopy Server\bin\lib\x64Release\*.dll TestApp\x64Release\ /y


robocopy ..\StormForge\buildWindows\x64Debug\bin Server\bin\x64Debug\ *.dll *.pdb *.lib
robocopy ..\StormForge\buildWindows\x64Debug\lib Server\bin\lib\x64Debug\ *.dll *.pdb *.lib
robocopy ..\StormForge\buildWindows\x64Release\bin Server\bin\x64Release\ *.dll *.pdb *.lib
robocopy ..\StormForge\buildWindows\x64Release\lib Server\bin\lib\x64Release\ *.dll *.pdb *.lib
robocopy ..\StormForge\buildWindows\x64Debug\ TestApp\x64Debug\ *.dll *.pdb *.lib
robocopy ..\StormForge\buildWindows\x64Release\ TestApp\x64Release\ *.dll *.pdb *.lib



robocopy ..\StormForge\3rdParties\src\openssl\buildWindows\openssl\bin Server\bin\x64Debug\ *.dll *.pdb *.lib
robocopy ..\StormForge\3rdParties\src\openssl\buildWindows\openssl\lib Server\bin\lib\x64Debug\ *.dll *.pdb *.lib
robocopy ..\StormForge\3rdParties\src\openssl\buildWindows\openssl\bin Server\bin\x64Release\ *.dll *.pdb *.lib
robocopy ..\StormForge\3rdParties\src\openssl\buildWindows\openssl\lib Server\bin\lib\x64Release\ *.dll *.pdb *.lib
robocopy ..\StormForge\3rdParties\src\openssl\buildWindows\openssl\bin buildWindows\x64\TestApp\UnitTest\Debug\ *.dll *.pdb *.lib
robocopy ..\StormForge\3rdParties\src\openssl\buildWindows\openssl\bin buildWindows\x64\TestApp\UnitTest\Release\ *.dll *.pdb *.lib


robocopy ..\StormForge\3rdParties\src\mysql\buildWindows\x64\lib64\Debug Server\bin\x64Debug\ *.dll *.pdb *.lib
robocopy ..\StormForge\3rdParties\src\mysql\buildWindows\x64\lib64 Server\bin\x64Release\ *.dll *.pdb *.lib
robocopy ..\StormForge\3rdParties\src\mysql\buildWindows\x64\lib64\Debug buildWindows\x64\TestApp\UnitTest\Debug\ *.dll *.pdb *.lib
robocopy ..\StormForge\3rdParties\src\mysql\buildWindows\x64\lib64 buildWindows\x64\TestApp\UnitTest\Release\ *.dll *.pdb *.lib


:quit

pause



