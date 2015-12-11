#!/bin/bash


case $1 in
	"DEBUG") ;;
	"RELEASE") ;;
	*)	echo "The first parameter need to be DEBUG or RELEASE"
		exit 0 ;;
esac


case $2 in
	"build") ;;
	"clean") ;;
	*)	echo "The second parameter need to be build or clean"
		exit 0 ;;
esac



pushd `dirname $0` > /dev/null
cd ..

export ROOT_PATH=`pwd`
export BUILD_MODE=$1

cd Server

#rm -rf bin/$BUILD_MODE/*

make -j -f makefile.mak $2


popd > /dev/null





