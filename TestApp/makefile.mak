
#--------------------------------------------------------------------
# Build rules

BUILD_SUB_PATHS= UnitTest

export ROOT_PATH?=/home/blue3k/work/BRServer
export BUILD_MODE?=DEBUG


.PHONY: build

build : 
	for buildSub in $(BUILD_SUB_PATHS); do \
		$(MAKE) -C $$buildSub -f makefile.mak build; \
	done


clean : 
	for buildSub in $(BUILD_SUB_PATHS); do \
		$(MAKE) -C $$buildSub -f makefile.mak clean; \
	done


