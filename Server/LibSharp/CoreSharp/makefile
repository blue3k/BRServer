

SRC_LIST = ResultCode.cs Properties/AssemblyInfo.cs


.PHONY: build

build :
	mcs $(SHARP_FLAGS) -t:library $(SRC_LIST) -out:$(ROOT_PATH)/Server/bin/CoreSharp.dll
	cp $(ROOT_PATH)/Server/bin/CoreSharp.dll $(ROOT_PATH)/Tools/bin/CoreSharp.dll

