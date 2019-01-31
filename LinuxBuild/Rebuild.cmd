

rem relative path from root
set PROJECT_BASE_PATH=%1
rem project name which will be target name
set PROJECT_NAME=%2
rem should be rebuild, ignored
set MAKE_BUILD_TARGET=%3
rem DEBUG, RELEASE
set BUILD_MODE=%4

if "%1"=="" goto error
if "%2"=="" goto error
if "%3"=="" goto error
if "%4"=="" goto error

call Build.cmd %1 %2 %3 clean
call Build.cmd %1 %2 %3 build

goto quit


error:

@echo Failed %PROJECT_NAME%

quit:

@echo Done %PROJECT_NAME%

