#!/bin/bash


export PATH=/cygdrive/c/cygwin64/bin:$PATH

cd HResult

make -f makefile.mak $1

cd ..



cd Protocol

make -f makefile.mak $1

cd ..



