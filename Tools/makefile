
CURRENT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))


###################################################


###################################################

BASE_PATH?=$(CURRENT_DIR)
BUILD_LIBS = toolLib PathConvert ValidatePath
SHARP_FLAGS = 

ifeq ($(BUILD_CONFIGURATION), 'DEBUG')
	SHARP_FLAGS = -debug
endif	

export SHARP_FLAGS
export BASE_PATH
	
build : 
	@echo $(BASE_PATH)
	for buildTool in $(BUILD_LIBS); do \
		$(MAKE) -C $$buildTool build; \
	done



