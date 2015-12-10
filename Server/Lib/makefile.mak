
#--------------------------------------------------------------------
# Build rules

BUILD_LIBS=Common DB Net Protocol ServerSystem Table

export ROOT_PATH?=/home/blue3k/work/BRServer
export BUILD_MODE?=DEBUG


build : 
	for buildTool in $(BUILD_LIBS); do \
		$(MAKE) -C $$buildTool -f makefile.mak build; \
	done


clean : 
	for buildTool in $(BUILD_LIBS); do \
		$(MAKE) -C $$buildTool -f makefile.mak clean; \
	done


