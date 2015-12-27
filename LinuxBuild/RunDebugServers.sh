#!/bin/bash



pushd `dirname $0` > /dev/null
cd ../Server/bin/DEBUG

rm -rf ../log/*

./BREntityServer -n:BREntityServer1&

sleep 2s

./BREntityServer -n:BREntityServer2&

sleep 2s

./BRLoginServer  -n:BRLoginServer01&


./BRSharedModuleServer -n:BRMatchingQueue01&


sleep 1s


./BRGameServer -n:BRConspiracyGameServer01&
./BRGameServer -n:BRConspiracyGameServer02&

sleep 1s

./BRGameInstanceServer -n:BRConspiracyGameInstanceServer01&
./BRGameInstanceServer -n:BRConspiracyGameInstanceServer02&




popd > /dev/null





