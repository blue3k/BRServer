
set COMMAND_ROOT=%~dp0
set SERVER_ROOT=%COMMAND_ROOT%..\..\..

set PATH_BIN=%SERVER_ROOT%\Server\bin\x64Debug
set PATH_LIB=%SERVER_ROOT%\Server\Lib
set SWIG=%SERVER_ROOT%\Tools\swig\swig.exe

set SWIG_OPTIONS=-csharp -c++ -addextern -Fstandard -nodefaultctor -nodefaultdtor -DSWIGWORDSIZE64 -D_WIN64 -namespace BR
set SWIG_INCLUDS=-I%PATH_LIB%
set PATH_OUTPUT=%COMMAND_ROOT%

%SWIG% %SWIG_OPTIONS% %SWIG_INCLUDS% BR\SWIG_monitoring.i

pause

