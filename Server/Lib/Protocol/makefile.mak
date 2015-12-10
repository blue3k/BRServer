
#--------------------------------------------------------------------
# Build rules

export LINK_TYPE=LIB
export PROJECT_NAME=Protocol


mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

export mkfile_path
export current_dir

include $(ROOT_PATH)/LinuxBuild/commonLib.mak
include $(ROOT_PATH)/LinuxBuild/common.mak

include $(DEPENDANCIES)


build: $(SOURCES) $(TARGET_FILE)
	@echo Done


clean: $(TARGET_OBJ_PATH) $(TARGET_LIB_PATH) $(TARGET_BIN_PATH)
	rm -rf $(TARGET_OBJ_PATH)
	rm -rf $(TARGET_LIB_PATH)/*.d $(TARGET_FILE)
	rm -rf $(TARGET_BIN_PATH)/*.o $(TARGET_FILE)
