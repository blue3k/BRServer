

cd External/gtest
cmake CMakeLists.txt
make
mkdir -p ../../Server/bin/lib/
cp *.a ../../Server/bin/lib/



cd ../zlib-1.2.8
chmod a+x configure
./configure
make
cp libz.a ../../Server/bin/lib/


cd ../jsoncpp
scons platform=linux-gcc
cp libs/linux-gcc-5.1.1/*.a ../../Server/bin/lib/
cp libs/linux-gcc-5.1.1/*.so ../../Server/bin/
