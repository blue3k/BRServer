
call ..\Cmd\setup_devenv.cmd

set CURRENT_DIR=%~dp0
set ROOT_PATH=%CURRENT_DIR%../

bash buildTools.sh


pause
