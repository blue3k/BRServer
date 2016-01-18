

SRC_LIST = PathTool.cs FileUtil.cs ResultCode.cs ToolEnvironment.cs Setting.cs SettingValue.cs ToolDebug.cs Properties/AssemblyInfo.cs



build :
	mcs $(SHARP_FLAGS) -t:library $(SRC_LIST) -out:$(BASE_PATH)/bin/toolLib.dll

