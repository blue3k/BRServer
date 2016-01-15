#


OUTPUT_PATH=../TableBuilder/TableFormat
OUTPUT_PATH2=../../../Server/Service/BRMonitoringWeb/BRMonitoringWeb/Conspiracy/table



SOURCES=gameConfigTable.xsd \
	levelTable.xsd \
	organicTable.xsd \
	rewardTable.xsd \
	shopTable.xsd \
	botTalkTable.xsd \
	stringTableBot.xsd 
	


SOURCES_POSTFIXED=$(SOURCES:.xsd=.cs)
TARGET_FILES=$(SOURCES_POSTFIXED:%=$(OUTPUT_PATH)/%)


$(OUTPUT_PATH)/%.cs: %.xsd
	xsd -c -namespace:conspiracy $< /out:"..\TableBuilder\TableFormat"
 

build: $(TARGET_FILES) 
	cp $(OUTPUT_PATH)/*.cs $(OUTPUT_PATH2)/
	@echo Done


clean:
	rm $(OUTPUT_PATH)/*.h
