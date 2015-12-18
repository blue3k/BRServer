#!/bin/bash



pushd `dirname $0` > /dev/null
cd ../Server/bin/DEBUG

rm -rf ../log/*

./EntityServer -debug -n:BREntityServer1&

sleep 2s

./EntityServer -debug -n:BREntityServer2&

sleep 2s

./LoginServer -debug -n:BRLoginServer01&


./SharedModuleServer -debug -n:BRMatchingQueue01&


sleep 1s


./GameServer -debug -n:BRConspiracyGameServer01&
./GameServer -debug -n:BRConspiracyGameServer02&

sleep 1s

./GameInstanceServer -debug -n:BRConspiracyGameInstanceServer01&
./GameInstanceServer -debug -n:BRConspiracyGameInstanceServer02&




popd > /dev/null





