

OUTPUT_PATH=../../Common

SOURCES = $(wildcard *.mc)

TARGET_FILES=$(addprefix $(OUTPUT_PATH)/,$(SOURCES:.mc=.h))



$(OUTPUT_PATH)/%.h: %.mc
	mc $< -u -r ../obj -b -n -h $(OUTPUT_PATH)


build: $(TARGET_FILES)
	@echo Done

	
clean:
	rm $(OUTPUT_PATH)/HRES*.h
