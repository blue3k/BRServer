
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\VsDevCmd.bat"

call External\openssl-1.0.2a\Build.bat


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

xcopy External\curl7.41\ms\Debug\* Server\bin\lib\x64Debug\* /y
xcopy External\curl7.41\ms\RelWithDebInfo\* Server\bin\lib\x64Release\* /y



xcopy External\MYSQLConnectorC-6.1.6\lib\vs12\mysqlclient.lib Server\bin\lib\x64Debug\ /y
xcopy External\MYSQLConnectorC-6.1.6\lib\vs12\mysqlclient.lib Server\bin\lib\x64Release\ /y
xcopy External\MYSQLConnectorC-6.1.6\lib\libmysql.* Server\bin\lib\x64Debug\ /y
xcopy External\MYSQLConnectorC-6.1.6\lib\libmysql.* Server\bin\lib\x64Release\ /y


xcopy Server\bin\lib\x64Debug\*.dll Server\bin\x64Debug\ /y
xcopy Server\bin\lib\x64Release\*.dll Server\bin\x64Release\ /y



pause



