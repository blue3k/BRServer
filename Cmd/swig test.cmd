
set COMMAND_ROOT=%~dp0

set PATH_BIN=%COMMAND_ROOT%..\Server\bin\x64Debug
set PATH_LIB=%COMMAND_ROOT%..\Server\Lib
set SWIG=%COMMAND_ROOT%..\Tools\swig\swig.exe
set SWIG_OPTIONS=-csharp -c++ -addextern -Fstandard -nodefaultctor -nodefaultdtor -outcurrentdir -namespace BR
set SWIG_INCLUDS=-I%PATH_LIB%

%SWIG% %SWIG_OPTIONS% %PATH_LIB%\server.i

pause
