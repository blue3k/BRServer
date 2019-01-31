

SRC_LIST = Properties/AssemblyInfo.cs


.PHONY: build

build :
	mcs $(SHARP_FLAGS) -t:library $(SRC_LIST) -out:../../bin/CommonSharp.dll
	cp ../../bin/CommonSharp.dll ../../../Tools/bin/CommonSharp.dll

