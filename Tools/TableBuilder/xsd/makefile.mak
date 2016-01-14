#


OUTPUT_PATH=../TableBuilder/TableFormat



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
	@echo Done


clean:
	rm $(OUTPUT_PATH)/*.h
