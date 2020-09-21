
set SF_PATH=%~dp0


git clone https://github.com/blue3k/StormForge.git %SF_PATH%..\StormForge

cd %SF_PATH%..\StormForge
call Setup.cmd

pause
