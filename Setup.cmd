
set SF_PATH=%~dp0


git clone https://github.com/blue3k/StormForge.git %SF_PATH%..\StormForge

git clone https://github.com/PlayFab/gsdk.git %SF_PATH%..\gsdk

cd %SF_PATH%..\StormForge
call Setup.cmd

pause
