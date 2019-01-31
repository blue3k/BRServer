#!/bin/bash



pushd `dirname $0` > /dev/null
cd ../Server/bin/DEBUG


pkill -9 BRGameInstanceS; pkill BRModuleServer
sleep 0.5s
pkill -9 BRGameServer; pkill BRLoginServer
sleep 0.5s
pkill -9 BREntityServer




popd > /dev/null





