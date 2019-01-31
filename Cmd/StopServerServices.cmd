

set COMMAND_ROOT=%~dp0
set SERVER_1=\\127.0.0.1 -u braves -p braves1234
set SERVICE_COMMAND=stop

call %COMMAND_ROOT%\_RunServerServices.cmd

echo on
for %%n in (%ENTITY_SERVICES%) do psservice %SERVICE_COMMAND% %%n

for %%n in (%OTHER_SERVICES%) do psservice %SERVICE_COMMAND% %%n

echo off


pause
