

#-----------------------------------------------------------------
# inputs

# need to put generic defines
INCLUDES?=.
DEFINES?=
LIBS?=

# in-case not specified - for test
OBJ_PATH=$(ROOT_PATH)/Server/bin/obj
BIN_PATH=$(ROOT_PATH)/Server/bin/bin
LIB_PATH=$(ROOT_PATH)/Server/bin/lib
TOOL_BIN_PATH=$(ROOT_PATH)/Tools/bin
LINK_TYPE?=EXE
BUILD_MODE?=DEBUG

VALIDDATEPATH=mono $(TOOL_BIN_PATH)/ValidatePath.exe

export OBJ_PATH
export BIN_PATH
export LIB_PATH

export VALIDDATEPATH

#-----------------------------------------------------------------
# variables

GREP_SOURCES = $(shell find . -type f -regex '.*\.\(c\|cpp\)') 
SOURCES ?= $(GREP_SOURCES:./%=%) 


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
	TARGET_BUILD_COMMAND=$(AR) cr $(TARGET_FILE) $(OBJECTS)
else ifeq "$(LINK_TYPE)" "DLL"
	TARGET_FILE=$(TARGET_BIN_PATH)/$(PROJECT_NAME).so
	TARGET_BUILD_COMMAND=$(CC) -shared $(LDFLAGS) $(OBJECTS) -o $(TARGET_FILE)  
endif


INCLUDES+= $(ROOT_PATH)/Server/Lib /usr/include /usr/include/mysql
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


export CC
export DEFINES
export CFLAGS
export CPPFLAGS
export LDFLAGS

export DEPENDANCIES

export TARGET_FILE
export TARGET_OBJ_PATH
export TARGET_BIN_PATH
export TARGET_LIB_PATH
export TARGET_BUILD_COMMAND

#--------------------------------------------------------------------
# Common rules

$(TARGET_OBJ_PATH)/%.o: %.cpp
	$(VALIDDATEPATH) +path:$@
	$(CC) $(CPPFLAGS) $< -o $@


$(TARGET_OBJ_PATH)/%.d: %.cpp
	@echo update dependency $<
	@set -e; rm -f $@; \
	$(VALIDDATEPATH) +path:$@
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$


$(TARGET_OBJ_PATH):
	mkdir -p $(TARGET_OBJ_PATH)	

$(TARGET_LIB_PATH):
	mkdir -p $(TARGET_LIB_PATH)	

$(TARGET_BIN_PATH):
	mkdir -p $(TARGET_BIN_PATH)	


#--------------------------------------------------------------------
# Build rules
	
$(TARGET_FILE): $(TARGET_OBJ_PATH) $(OBJECTS)
	@echo Build Target $< $(OBJECTS)
	$(TARGET_BUILD_COMMAND)


$(DEPENDANCIES): | $(TARGET_OBJ_PATH) $(TARGET_LIB_PATH) $(TARGET_BIN_PATH)

