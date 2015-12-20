#!/bin/bash



pushd `dirname $0` > /dev/null
cd ../Server/bin/DEBUG

pkill BREntityServer
pkill BRGameServer
pkill BRLoginServer
sleep 0.5s
pkill BRGameInstanceS
pkill BRSharedModuleS


popd > /dev/null





