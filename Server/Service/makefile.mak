
#--------------------------------------------------------------------
# Build rules

BUILD_SERVICES?=EntityServer GameInstanceServer GameServer LoginServer ModuleServer


export ROOT_PATH?=/home/blue3k/work/BRServer
export BUILD_MODE?=DEBUG


build : 
	for buildSub in $(BUILD_SERVICES); do \
		$(MAKE) -C $$buildSub -j -f makefile.mak build; \
	done


clean : 
	for buildSub in $(BUILD_SERVICES); do \
		$(MAKE) -C $$buildSub -j -f makefile.mak clean; \
	done


