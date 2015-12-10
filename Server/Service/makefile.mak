
#--------------------------------------------------------------------
# Build rules

BUILD_SUB_PATHS=EntityServer GameInstanceServer GameServer LoginServer SharedModuleServer

export ROOT_PATH?=/home/blue3k/work/BRServer
export BUILD_MODE?=DEBUG


build : 
	for buildSub in $(BUILD_SUB_PATHS); do \
		$(MAKE) -C $$buildSub -j -f makefile.mak build; \
	done


clean : 
	for buildSub in $(BUILD_SUB_PATHS); do \
		$(MAKE) -C $$buildSub -j -f makefile.mak clean; \
	done


