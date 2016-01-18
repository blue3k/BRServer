
MAKE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
CURRENT_DIR?=$(MAKE_DIR)
ROOT_DIR?=$(CURRENT_DIR)../

###################################################


###################################################

BASE_PATH?=$(CURRENT_DIR)
BUILD_LIBS = toolLib PathConvert ValidatePath BRResultCodeCompiler
SHARP_FLAGS = 

ifeq ($(BUILD_CONFIGURATION), 'DEBUG')
	SHARP_FLAGS = -debug
endif	

export SHARP_FLAGS
export BASE_PATH
	
build : 
	$(MAKE) -C ../Server/LibSharp/CommonSharp build
	$(MAKE) -C ../Server/LibSharp/CoreSharp build
	
	for buildTool in $(BUILD_LIBS); do \
		$(MAKE) -C $$buildTool build; \
	done



