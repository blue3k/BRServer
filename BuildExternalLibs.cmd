
set VSDIR="C:\Program Files (x86)\Microsoft Visual Studio 14.0"
call %VSDIR%\VC\vcvarsall.bat x86_amd64
echo set PATH=%PATH%;%VSDIR%\VC\bin\x86_amd64;

cd External\openssl-1.0.2a
call BuildWIN64.cmd
cd %~dp0

cd External\curl7.41
call buildconf.bat
cd %~dp0


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


msbuild "External\curl7.41\ms\curlBuild.sln" /t:build /p:Configuration=Debug
if %errorlevel% neq 0 (
echo Error on build
goto quit
) else (
echo continue
)

msbuild "External\curl7.41\ms\curlBuild.sln" /t:build /p:Configuration=RelWithDebInfo
if %errorlevel% neq 0 (
echo Error on build
goto quit
) else (
echo continue
)

xcopy External\zlib-1.2.8\*.h External\Include\zlib\* /S /R /Y


xcopy External\curl7.41\ms\Debug\* Server\bin\lib\x64Debug\* /y
xcopy External\curl7.41\ms\RelWithDebInfo\* Server\bin\lib\x64Release\* /y



xcopy External\mysql-connector-c-6.1.6-winx64\lib\vs12\mysqlclient.lib Server\bin\lib\x64Debug\ /y
xcopy External\mysql-connector-c-6.1.6-winx64\lib\vs12\mysqlclient.lib Server\bin\lib\x64Release\ /y
xcopy External\mysql-connector-c-6.1.6-winx64\lib\libmysql.* Server\bin\lib\x64Debug\ /y
xcopy External\mysql-connector-c-6.1.6-winx64\lib\libmysql.* Server\bin\lib\x64Release\ /y


xcopy Server\bin\lib\x64Debug\*.dll Server\bin\x64Debug\ /y
xcopy Server\bin\lib\x64Release\*.dll Server\bin\x64Release\ /y
xcopy Server\bin\lib\x64Debug\*.dll TestApp\x64Debug\ /y
xcopy Server\bin\lib\x64Release\*.dll TestApp\x64Release\ /y



pause



