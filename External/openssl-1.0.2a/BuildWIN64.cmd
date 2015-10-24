


set PWD=%~dp0
set OPENSSLDIR=%PWD%\TargetRelease


call perl Configure VC-WIN64A --openssldir=%OPENSSLDIR% --prefix=%PWD%\TargetRelease
call ms\do_win64a
nmake -f ms\nt.mak
nmake -f ms\nt.mak install


call perl Configure debug-VC-WIN64A --openssldir=%OPENSSLDIR% --prefix=%PWD%\TargetDebug
call ms\do_win64a
nmake -f ms\nt.mak
nmake -f ms\nt.mak install


xcopy %PWD%\TargetRelease\include\* %PWD%\..\Include\* /S /R /Y
xcopy %PWD%\TargetRelease\include\* %PWD%\..\..\Server\bin\lib\x64Release\* /S /R /Y 
xcopy %PWD%\TargetRelease\include\* %PWD%\..\..\Server\bin\lib\x64Debug\* /S /R /Y 

xcopy %PWD%\TargetRelease\ssl\* %PWD%\..\..\Server\bin\x64Release\ssl\* /S /R /Y
xcopy %PWD%\TargetDebug\ssl\* %PWD%\..\..\Server\bin\x64Debug\ssl\* /S /R /Y 

xcopy %PWD%\TargetRelease\lib\* %PWD%\..\..\Server\bin\lib\x64Release\* /S /R /Y 
xcopy %PWD%\TargetDebug\lib\* %PWD%\..\..\Server\bin\lib\x64Debug\* /S /R /Y 


pause

