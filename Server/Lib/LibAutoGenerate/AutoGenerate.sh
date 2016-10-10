#!/bin/bash


export PATH=/cygdrive/c/cygwin64/bin:$PATH




cd brcode

make $1

cd ..


cd Protocol

make -f makefile.mak $1

cp ./Temp/Lib/Protocol/CSharp/*.cs ../../../../../Conspiracy/trunk/Client/Conspiracy/Conspiracy/Net/Message

cd ..



