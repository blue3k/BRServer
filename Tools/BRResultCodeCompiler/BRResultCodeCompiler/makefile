

SRC_LIST = Program.cs BRResultCode.cs BRResultCodeFacility.cs ResultCodeExporterH.cs ResultCodeExporterXml.cs serverErrorTable.cs Properties/AssemblyInfo.cs



build :
	mcs $(SHARP_FLAGS) $(SRC_LIST) -r:$(BASE_PATH)bin/toolLib.dll -out:$(BASE_PATH)/bin/BRResultCodeCompiler.exe

