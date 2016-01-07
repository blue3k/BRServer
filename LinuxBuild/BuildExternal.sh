

#sudo apt-get install cmake
#sudo apt-get install scons

cd ..

pushd `dirname $0` > /dev/null

cd External/jsoncpp/
rm -rf libs/*
rm -rf buildscons/*
scons platform=linux-gcc

cp libs/linux-gcc-5.2.1/libjson_linux-gcc-5.2.1_libmt.a ../../Server/bin/lib/libjsoncpp.a
cp -R include ../../Server/bin/


popd > /dev/null


pushd `dirname $0` > /dev/null


cd External/curl7.41/

./buildconf
./configure

make

cp lib/.libs/libcurl.a ../../Server/bin/lib/libcurl.a
cp -R include/curl ../../Server/bin/include/



popd > /dev/null


pushd `dirname $0` > /dev/null


cd External/zlib-1.2.8/

./configure --static --64

make

cp libz.a ../../Server/bin/lib/libz.a
cp *.h ../../Server/bin/include/zlib/



popd > /dev/null


