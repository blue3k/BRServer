
PATH=%PATH%;C:\cygwin64\bin

cd HResult

make -f makefile.mak %1

cd ..



cd Protocol

make -f makefile.mak %1

cd ..



