
set COMMAND_ROOT=%~dp0

set RELEASE_MODE=x64Debug
set RELEASE_EXTS=*.exe *.dll
set FTP_CMD_TMP=ftpcmd.dat

set PATH_BIN=%COMMAND_ROOT%..\Server\bin\%RELEASE_MODE%
set PATH_DEVSERVER=106.247.237.180
set PATH_SYMBOL=D:\SymbolServer


set SRCSRV_INI_FILE=srcsrv.ini
set SRC_PATH=.\
set PATH=%PATH%;"C:\Program Files (x86)\Windows Kits\8.1\Debuggers\x64\srcsrv";"C:\Program Files (x86)\Windows Kits\8.1\Debuggers\x64"

cd ..
call p4index -ini=%SRCSRV_INI_FILE% -source=%SRC_PATH% -symbols=%PATH_BIN% -debug 

echo Server symstore...
symstore add /r /f %PATH_BIN%\*.exe /s %PATH_SYMBOL% /t "BRServer"
symstore add /r /f %PATH_BIN%\*.dll /s %PATH_SYMBOL% /t "BRServer"
symstore add /r /f %PATH_BIN%\*.pdb /s %PATH_SYMBOL% /t "BRServer"

cd cmd


@echo off
echo user braves> %FTP_CMD_TMP%
echo braves1234>> %FTP_CMD_TMP%
echo bin>> %FTP_CMD_TMP%
echo hash>> %FTP_CMD_TMP%
echo prompt>> %FTP_CMD_TMP%
echo cd /gameserver/bin/%RELEASE_MODE%>> %FTP_CMD_TMP%

for %%n in (%RELEASE_EXTS%) do echo mput %PATH_BIN%\%%n>> %FTP_CMD_TMP%

rem echo mput %PATH_BIN%\*.exe>> %FTP_CMD_TMP%
rem echo mput %PATH_BIN%\*.dll>> %FTP_CMD_TMP%
echo quit>> %FTP_CMD_TMP%
ftp -n -s:%FTP_CMD_TMP% %PATH_DEVSERVER%
del %FTP_CMD_TMP%

echo %PATH_FILEZILLA%\filezilla ftp://braves:braves1234@106.247.237.180:21 --local=%PATH_BIN%

echo set COPY_OPTIONS=/e /xd obj /r:5 /w:1
echo Robocopy  %PATH_BIN%\ %PATH_DEVSERVER%\ %COPY_OPTIONS%

pause
