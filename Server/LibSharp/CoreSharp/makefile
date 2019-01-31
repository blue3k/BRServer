

SRC_LIST = ResultCode.cs Properties/AssemblyInfo.cs


.PHONY: build

build :
	mcs $(SHARP_FLAGS) -t:library $(SRC_LIST) -out:../../bin/CoreSharp.dll
	cp ../../bin/CoreSharp.dll ../../../Tools/bin/CoreSharp.dll

