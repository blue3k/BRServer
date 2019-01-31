#!/bin/bash



pushd `dirname $0` > /dev/null
cd ..

export ROOT_PATH=`pwd`
export BUILD_MODE=$1


scp -i ./LinuxBuild/Bravesprivate2.popen -r Server/bin/DEBUG/ blue3k@10.133.29.154:/GameServer/Server/bin/
scp -i ./LinuxBuild/Bravesprivate2.popen -r Server/Config/ blue3k@10.133.29.154:/GameServer/Server/
scp -i ./LinuxBuild/Bravesprivate2.popen Server/Config/ServerConfig_linux_rel.xml blue3k@10.133.29.154:/GameServer/Server/Config/ServerConfig_linux.xml



popd > /dev/null



