

SRC_LIST = Properties/AssemblyInfo.cs


.PHONY: build

build :
	mcs $(SHARP_FLAGS) -t:library $(SRC_LIST) -out:$(ROOT_PATH)/Server/bin/CommonSharp.dll
	cp $(ROOT_PATH)/Server/bin/CommonSharp.dll $(ROOT_PATH)/Tools/bin/CommonSharp.dll

