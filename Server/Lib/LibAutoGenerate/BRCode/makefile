

OUTPUT_PATH=../../Common/ResultCode
SHARP_OUTPUT_PATH=../../../LibSharp/CommonSharp/ResultCode
TABLE_PATH=../../../../../Conspiracy/trunk/Design/Table/xsd/serverErrorTable_kor.xml
CLIENT_PATH=../../../../../Conspiracy/trunk/Client/BREngine/Net/

RESULT_COMPILER=../../../../Tools/bin/BRResultCodeCompiler


SOURCES = $(wildcard ResultCode*.xml)

TARGET_FILES=$(addprefix $(OUTPUT_PATH)/BR,$(SOURCES:.xml=.h))



# $(OUTPUT_PATH)/BR%.h: %.xml
	# $(RESULT_COMPILER) facilitiy:ResultFacility.xml in:$< tableout:$(TABLE_PATH) out:$(OUTPUT_PATH)

.PHONY: build

build:
	$(RESULT_COMPILER) facilitiy:ResultFacility.xml $(addprefix +in:,$(SOURCES)) tablepath:$(TABLE_PATH) out:$(OUTPUT_PATH) outSharp:$(SHARP_OUTPUT_PATH)
	cp -R $(SHARP_OUTPUT_PATH) $(CLIENT_PATH)
	@echo Done

	
clean:
	rm $(TARGET_FILES)
