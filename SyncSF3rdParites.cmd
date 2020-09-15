
PATH=%PATH%;C:\Perl64\bin



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


robocopy ..\StormForge\3rdParties\src\mysql\buildWindows\x64\lib64\debug Server\bin\x64Debug\ *.dll *.pdb *.exe
robocopy ..\StormForge\3rdParties\src\mysql\buildWindows\x64\lib64 Server\bin\x64Release\ *.dll *.pdb *.exe
robocopy ..\StormForge\3rdParties\src\mysql\buildWindows\x64\lib64\debug buildWindows\x64\TestApp\UnitTest\Debug\ *.dll *.pdb *.exe
robocopy ..\StormForge\3rdParties\src\mysql\buildWindows\x64\lib64 buildWindows\x64\TestApp\UnitTest\Release\ *.dll *.pdb *.exe


:quit

pause



