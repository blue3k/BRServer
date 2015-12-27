#!/bin/bash



pushd `dirname $0` > /dev/null
cd ../Server/bin/DEBUG


pkill BRGameInstanceS; pkill BRSharedModuleS
sleep 0.5s
pkill BRGameServer; pkill BRLoginServer
sleep 0.5s
pkill BREntityServer




popd > /dev/null





