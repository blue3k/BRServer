
#--------------------------------------------------------------------
# Build rules

BUILD_LIBS=Common DB Net Protocol ServerSystem Table

export ROOT_PATH=/home/blue3k/work/BRServer
#export PROJECT_NAME=%PROJECT_NAME%
#export BUILD_MODE=%BUILD_MODE%


build : 
	for buildTool in $(BUILD_LIBS); do \
		export PROJECT_NAME=$$buildTool\
		$(MAKE) -C $$buildTool -f makefile.mak build; \
	done


clean : 
	for buildTool in $(BUILD_LIBS); do \
		export PROJECT_NAME=$$buildTool\
		$(MAKE) -C $$buildTool -f makefile.mak clean; \
	done


