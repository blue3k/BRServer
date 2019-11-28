#
RESULT_BUILD=../../../StormForge/Bin/SFResultCodeCompiler.exe



SERVER_OUTPUT_PATH=../../Server/Lib/ServerSystem/ResultCode
SERVER_SOURCES=ResultCodeDB.xml \
	ResultCodeGame.xml \
	ResultCodeLogin.xml \
	ResultCodeSvr.xml \

ENGINE_SHARP_OUTPUT_PATH=../../StormForge/Engine/Src/SFEngineDLL/Sharp/ResultCode




SERVER_SOURCES_POSTFIXED=$(SERVER_SOURCES:.xml=.h)
SERVER_TARGET_FILES=$(SERVER_SOURCES_POSTFIXED:%=$(SERVER_OUTPUT_PATH)/SF%)


$(SERVER_OUTPUT_PATH)/SF%.h: %.xml $(RESULT_BUILD)
	$(RESULT_BUILD) +out=$(SERVER_OUTPUT_PATH)/ +facility=ServerResultFacility.xml +outSharp=$(ENGINE_SHARP_OUTPUT_PATH) +in=$<
 

build: $(SERVER_SOURCES) $(SERVER_TARGET_FILES) 
	@echo Done


clean:
	rm $(SERVER_TARGET_FILES) 

