
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

#-----------------------------------------------------------------
# inputs

# in-case not specified - for test
OBJ_PATH?=$(current_dir)obj
BIN_PATH?=$(current_dir)bin
LIB_PATH?=$(current_dir)lib
LINK_TYPE?=EXE
BUILD_MODE?=DEBUG

SOURCES := $(shell find . -type f -regex '.*\.\(c\|cpp\)') 


TARGET_OBJ_PATH=$(OBJ_PATH)/$(BUILD_MODE)/$(PROJECT_NAME)
TARGET_BIN_PATH=$(BIN_PATH)/$(BUILD_MODE)/$(PROJECT_NAME)
TARGET_LIB_PATH=$(LIB_PATH)/$(BUILD_MODE)

export TARGET_OBJ_PATH
export TARGET_BIN_PATH
export TARGET_LIB_PATH

OBJECTS=$(addprefix $(TARGET_OBJ_PATH)/,$(SOURCES:.cpp=.o))
DEPENDANCIES=$(addprefix $(TARGET_OBJ_PATH)/,$(SOURCES:.cpp=.d))

TARGET_FILE=$(TARGET_BIN_PATH)/$(PROJECT_NAME)
TARGET_BUILD_COMMAND=

ifeq "$(LINK_TYPE)" "EXE"
	TARGET_FILE=$(TARGET_BIN_PATH)/$(PROJECT_NAME)
	TARGET_BUILD_COMMAND=$(CC) $(LDFLAGS) $(OBJECTS) -o $(TARGET_FILE) 
else ifeq "$(LINK_TYPE)" "LIB"
	TARGET_FILE=$(TARGET_LIB_PATH)/$(PROJECT_NAME).a
	TARGET_BUILD_COMMAND=$(AR) a $(TARGET_FILE) $(OBJECTS)
else ifeq "$(LINK_TYPE)" "DLL"
	TARGET_FILE=$(TARGET_BIN_PATH)/$(PROJECT_NAME).so
	TARGET_BUILD_COMMAND=$(CC) -shared $(LDFLAGS) $(OBJECTS) -o $(TARGET_FILE)  
endif


# need to put generic defines
INCLUDES?=.
DEFINES?=
LIBS?=

DEFINES+= $(BUILD_MODE) $(LOGNAME)

CC=g++
CFLAGS=-c -g -std=c++14 -pthread -static-libgcc -Wall $(addprefix -I,$(INCLUDES)) $(addprefix -D,$(DEFINES))
CPPFLAGS:=$(CFLAGS)
LDFLAGS=-g -std=c++14 -pthread -static-libgcc $(addprefix -l,$(LIBS))

ifeq "$(BUILD_MODE)" "DEBUG"
	CFLAGS+=-O0
	CPPFLAGS+=-O0
else
endif


#--------------------------------------------------------------------
# Common ruls

$(TARGET_OBJ_PATH)/%.o: %.cpp
	$(CC) $(CPPFLAGS) $< -o $@


$(TARGET_OBJ_PATH)/%.d: %.cpp
	@echo update dependency $<
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(DEPENDANCIES): | $(TARGET_OBJ_PATH) $(TARGET_LIB_PATH) $(TARGET_BIN_PATH)

$(TARGET_OBJ_PATH):
	mkdir -p $(TARGET_OBJ_PATH)	

$(TARGET_LIB_PATH):
	mkdir -p $(TARGET_LIB_PATH)	

$(TARGET_BIN_PATH):
	mkdir -p $(TARGET_BIN_PATH)	


#--------------------------------------------------------------------
# Build rules
	
include $(DEPENDANCIES)


$(TARGET_FILE): $(TARGET_OBJ_PATH) $(OBJECTS)
	@echo Build Target $< $(OBJECTS)
	$(TARGET_BUILD_COMMAND)




build: $(SOURCES) $(TARGET_FILE)
	@echo Done


clean: $(TARGET_OBJ_PATH) $(TARGET_LIB_PATH) $(TARGET_BIN_PATH)
	rm -rf $(TARGET_OBJ_PATH)/*.o $(TARGET_OBJ_PATH)/*.d
	rm -rf $(TARGET_LIB_PATH)/*.d $(TARGET_FILE)
	rm -rf $(TARGET_BIN_PATH)/*.o $(TARGET_FILE)
