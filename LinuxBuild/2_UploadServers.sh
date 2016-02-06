#!/bin/bash



pushd `dirname $0` > /dev/null
cd ..

export ROOT_PATH=`pwd`
export BUILD_MODE=$1



scp -C -i ./LinuxBuild/Bravesprivate2.popen -r Server/bin/DEBUG/ blue3k@161.202.74.206:/GameServer/Server/bin/



popd > /dev/null





