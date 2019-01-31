
#--------------------------------------------------------------------
# Build rules

BUILD_SUB_PATHS=Lib Service

export ROOT_PATH?=/home/blue3k/work/BRServer
export BUILD_MODE?=DEBUG


build : 
	for buildSub in $(BUILD_SUB_PATHS); do \
		$(MAKE) -C $$buildSub -f makefile.mak build; \
	done


clean : 
	for buildSub in $(BUILD_SUB_PATHS); do \
		$(MAKE) -C $$buildSub -f makefile.mak clean; \
	done


