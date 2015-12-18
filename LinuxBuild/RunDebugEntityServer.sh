#!/bin/bash



pushd `dirname $0` > /dev/null
cd ../Server/bin/DEBUG

rm -rf ../log/*

./EntityServer -debug -n:BREntityServer1&

sleep 2s

./EntityServer -debug -n:BREntityServer2&


popd > /dev/null





