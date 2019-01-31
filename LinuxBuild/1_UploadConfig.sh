#!/bin/bash



pushd `dirname $0` > /dev/null
cd ..

export ROOT_PATH=`pwd`
export BUILD_MODE=$1



scp -i ./LinuxBuild/Bravesprivate3.popen -r Server/Config/ ubuntu@52.78.247.43:/GameServer/Server/



popd > /dev/null





