#!/bin/bash



pushd `dirname $0` > /dev/null
cd ../Server/bin/DEBUG

pkill EntityServer
pkill GameServer
pkill LoginServer
sleep 0.5s
pkill GameInstanceSer
pkill SharedModuleSer


popd > /dev/null





