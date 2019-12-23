
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


robocopy ..\StormForge\buildWindows\x64Debug\bin Server\bin\x64Debug\ *.dll *.pdb
robocopy ..\StormForge\buildWindows\x64Release\bin Server\bin\x64Release\ *.dll *.pdb *.exe
robocopy ..\StormForge\buildWindows\x64Release\bin Server\bin\ *.dll *.pdb *.exe
robocopy ..\StormForge\buildWindows\x64Debug\ TestApp\x64Debug\ *.dll *.pdb *.exe
robocopy ..\StormForge\buildWindows\x64Release\ TestApp\x64Release\ *.dll *.pdb *.exe


robocopy ..\StormForge\3rdParties\src\openssl\buildWindows\openssl\bin Server\bin\x64Debug\ *.dll *.pdb *.exe
robocopy ..\StormForge\3rdParties\src\openssl\buildWindows\openssl\bin Server\bin\x64Release\ *.dll *.pdb *.exe
robocopy ..\StormForge\3rdParties\src\openssl\buildWindows\openssl\bin buildWindows\x64\TestApp\UnitTest\Debug\ *.dll *.pdb *.exe
robocopy ..\StormForge\3rdParties\src\openssl\buildWindows\openssl\bin buildWindows\x64\TestApp\UnitTest\Release\ *.dll *.pdb *.exe



:quit

pause



