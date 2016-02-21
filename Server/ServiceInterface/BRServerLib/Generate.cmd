
set COMMAND_ROOT=%~dp0
set SERVER_ROOT=%COMMAND_ROOT%..\..\..

set PATH_LIB=%SERVER_ROOT%\Server\Lib
set SWIG=%SERVER_ROOT%\Tools\swig\swig.exe

set SWIG_OPTIONS=-csharp -c++ -addextern -Fstandard -nodefaultctor -nodefaultdtor -DSWIGWORDSIZE32 -D_WIN64 -namespace BR -outdir Generated
set SWIG_INCLUDS=-I%PATH_LIB%
set PATH_OUTPUT=%COMMAND_ROOT%

%SWIG% %SWIG_OPTIONS% %SWIG_INCLUDS% SWIG_BR.i 

pause

