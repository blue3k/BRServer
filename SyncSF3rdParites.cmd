
PATH=%PATH%;C:\Perl64\bin
set OPENSSL_DIR=..\StormForge\3rdParties\src\openssl


xcopy Server\bin\lib\x64Debug\*.dll Server\bin\x64Debug\ /y
xcopy Server\bin\lib\x64RelWithDebInfo\*.dll Server\bin\x64RelWithDebInfo\ /y
xcopy Server\bin\lib\x64Debug\*.dll TestApp\x64Debug\ /y
xcopy Server\bin\lib\x64RelWithDebInfo\*.dll TestApp\x64RelWithDebInfo\ /y


robocopy ..\StormForge\buildWindows\x64Debug\bin Server\bin\x64Debug\ *.dll *.pdb
robocopy ..\StormForge\buildWindows\x64RelWithDebInfo\bin Server\bin\x64RelWithDebInfo\ *.dll *.pdb *.exe

robocopy ..\StormForge\buildWindows\x64Debug\bin Server\bin\x64Debug\netcoreapp3.1 *.dll *.pdb
robocopy ..\StormForge\buildWindows\x64RelWithDebInfo\bin Server\bin\x64RelWithDebInfo\netcoreapp3.1 *.dll *.pdb *.exe

robocopy ..\StormForge\buildWindows\x64RelWithDebInfo\bin Server\bin\ *.dll *.pdb *.exe
robocopy ..\StormForge\buildWindows\x64Debug\ TestApp\x64Debug\ *.dll *.pdb *.exe
robocopy ..\StormForge\buildWindows\x64RelWithDebInfo\ TestApp\x64RelWithDebInfo\ *.dll *.pdb *.exe


robocopy %OPENSSL_DIR%\buildWindows\openssl\bin Server\bin\x64Debug *.dll *.pdb
robocopy %OPENSSL_DIR%\buildWindows\openssl\bin Server\bin\x64RelWithDebInfo *.dll *.pdb

robocopy %OPENSSL_DIR%\buildWindows\openssl\bin Server\bin\x64Debug\netcoreapp3.1 *.dll *.pdb
robocopy %OPENSSL_DIR%\buildWindows\openssl\bin Server\bin\x64RelWithDebInfo\netcoreapp3.1 *.dll *.pdb

robocopy %OPENSSL_DIR%\buildWindows\openssl\bin buildWindows\x64\TestApp\UnitTest\Debug\ *.dll *.pdb
robocopy %OPENSSL_DIR%\buildWindows\openssl\bin buildWindows\x64\TestApp\UnitTest\Release\ *.dll *.pdb



REM set MYSQL_DIR=..\StormForge\3rdParties\src\mysql
REM robocopy %MYSQL_DIR%\buildWindows\x64\lib64\debug Server\bin\x64Debug\ *.dll *.pdb *.exe
REM robocopy %MYSQL_DIR%\buildWindows\x64\lib64 Server\bin\x64RelWithDebInfo\ *.dll *.pdb *.exe
REM robocopy %MYSQL_DIR%\buildWindows\x64\lib64\debug buildWindows\x64\TestApp\UnitTest\Debug\ *.dll *.pdb *.exe
REM robocopy %MYSQL_DIR%\buildWindows\x64\lib64 buildWindows\x64\TestApp\UnitTest\Release\ *.dll *.pdb *.exe


:quit

pause



