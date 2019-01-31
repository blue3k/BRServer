

set PATH_BIN=%COMMAND_ROOT%..\bin
del /s %PATH_BIN%\log\*.txt


set COMMAND_ROOT=%~dp0
set SERVER_1=\\127.0.0.1 -u braves -p braves1234
set SERVICE_COMMAND=start

call %COMMAND_ROOT%\_RunServerServices.cmd


echo on
for %%n in (%ENTITY_SERVICES%) do psservice %SERVICE_COMMAND% %%n & timeout /t 2 /nobreak

timeout /t 2 /nobreak

for %%n in (%OTHER_SERVICES%) do psservice %SERVICE_COMMAND% %%n & timeout /t 1 /nobreak

echo off


pause
