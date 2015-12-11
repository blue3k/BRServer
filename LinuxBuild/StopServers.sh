#!/bin/bash



pushd `dirname $0` > /dev/null
cd ../Server/bin/DEBUG

pkill EntityServer
pkill GameServer
pkill LoginServer
pkill GameInstanceServer
pkill SharedModuleServer


popd > /dev/null





