
PATH=%PATH%;C:\cygwin64\bin

set VSDIR="C:\Program Files (x86)\Microsoft Visual Studio 14.0"
call %VSDIR%\VC\vcvarsall.bat amd64


bash GenerateFiles.sh


pause
