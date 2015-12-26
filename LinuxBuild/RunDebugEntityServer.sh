#!/bin/bash



pushd `dirname $0` > /dev/null
cd ../Server/bin/DEBUG

rm -rf ../log/*

./BREntityServer -n:BREntityServer1

sleep 2s

./BREntityServer -n:BREntityServer2


popd > /dev/null





