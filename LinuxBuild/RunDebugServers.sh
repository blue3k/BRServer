#!/bin/bash



pushd `dirname $0` > /dev/null
cd ../Server/bin/DEBUG

rm -rf ../log/*

./BREntityServer -debug -n:BREntityServer1&

sleep 2s

./BREntityServer -debug -n:BREntityServer2&

sleep 2s

./BRLoginServer -debug -n:BRLoginServer01&


./BRSharedModuleServer -debug -n:BRMatchingQueue01&


sleep 1s


./BRGameServer -debug -n:BRConspiracyGameServer01&
./BRGameServer -debug -n:BRConspiracyGameServer02&

sleep 1s

./BRGameInstanceServer -debug -n:BRConspiracyGameInstanceServer01&
./BRGameInstanceServer -debug -n:BRConspiracyGameInstanceServer02&




popd > /dev/null





