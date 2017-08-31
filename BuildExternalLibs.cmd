
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


rem copy libraries
xcopy External\mysql-connector-c-6.1.6-winx64\lib\vs12\mysqlclient.lib Server\bin\lib\x64Debug\ /y
xcopy External\mysql-connector-c-6.1.6-winx64\lib\vs12\mysqlclient.lib Server\bin\lib\x64Release\ /y
xcopy External\mysql-connector-c-6.1.6-winx64\lib\libmysql.* Server\bin\lib\x64Debug\ /y
xcopy External\mysql-connector-c-6.1.6-winx64\lib\libmysql.* Server\bin\lib\x64Release\ /y


xcopy Server\bin\lib\x64Debug\*.dll Server\bin\x64Debug\ /y
xcopy Server\bin\lib\x64Release\*.dll Server\bin\x64Release\ /y
xcopy Server\bin\lib\x64Debug\*.dll TestApp\x64Debug\ /y
xcopy Server\bin\lib\x64Release\*.dll TestApp\x64Release\ /y


xcopy StormForge\Bin\Win\x64Debug\*.dll Server\bin\x64Debug\ /y
xcopy StormForge\Bin\Win\x64Release\*.dll Server\bin\x64Release\ /y
xcopy StormForge\Bin\Win\x64Debug\*.dll TestApp\x64Debug\ /y
xcopy StormForge\Bin\Win\x64Release\*.dll TestApp\x64Release\ /y


:quit

pause



