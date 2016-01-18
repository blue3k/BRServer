#


OUTPUT_PATH=../BRResultCodeCompiler
SYNC_PATH=../../../Server/Lib/LibAutoGenerate/BRCode



SOURCES=BRResultCode.xsd \
	BRResultCodeFacility.xsd \
	serverErrorTable.xsd 



SOURCES_POSTFIXED=$(SOURCES:.xsd=.cs)
TARGET_FILES=$(SOURCES_POSTFIXED:%=$(OUTPUT_PATH)/%)
TARGET_XSDS=$(SOURCES:%=$(SYNC_PATH)/%)


$(OUTPUT_PATH)/%.cs: %.xsd
	xsd -c -namespace:BR.Table $< /out:"..\BRResultCodeCompiler"
	@echo cp $< $@


$(SYNC_PATH)/%.xsd: %.xsd
	cp $< $@


build: $(TARGET_FILES) $(TARGET_XSDS)
	@echo Done


clean:
	rm $(TARGET_FILES)
