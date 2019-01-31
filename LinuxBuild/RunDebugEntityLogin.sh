#!/bin/bash



pushd `dirname $0` > /dev/null
cd ../Server/bin/DEBUG

rm -rf ../log/*

./BREntityServer -debug -n:BREntityServer1&

sleep 2s

./BREntityServer -debug -n:BREntityServer2&

sleep 2s

./BRModuleServer -debug -n:BRLoginServer01&




popd > /dev/null





