
cd %~d0
cd %~dp0

@echo off


set SF_PATH=%~dp0..\StormForge\
set SF_3RDPARTY=%SF_PATH%3rdParties 
set SF_SDK=%SF_PATH%..
set TOOL_PATH=%SF_PATH%\3rdParties\tool\bin
set ZIP_EXE="C:\Program Files\7-Zip\7z.exe"

call %SF_3RDPARTY%\FindVC.cmd


cd %SF_SDK%


set ZKUI_INSTALL_FILE_WIN=zkui_win_20160203.zip
if not exist %ZKUI_INSTALL_FILE_WIN% (
	echo downloading ZKUI
    call %TOOL_PATH%\httpget +url:https://github.com/echoma/zkui/wiki/prebuild/win/%ZKUI_INSTALL_FILE_WIN%
	%ZIP_EXE% x %ZKUI_INSTALL_FILE_WIN% -ozkui/*
)


set KAFKATOOL_INSTALL_FILE_WIN=kafkatool_64bit.exe
if not exist %KAFKATOOL_INSTALL_FILE_WIN% (
	echo downloading KAFKATOOL
    call %TOOL_PATH%\httpget +url:https://www.kafkatool.com/download2/%KAFKATOOL_INSTALL_FILE_WIN%
	%KAFKATOOL_INSTALL_FILE_WIN%
)

