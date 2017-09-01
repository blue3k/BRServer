#
PROTO_BUILD=./../StormForge/Bin/SFProtocolCompiler.exe

OUTPUT_PATH=../../Server/Lib/ServerProtocol/Message
OUTPUT_SHARP_PATH=../../Src/SFEngineDLL/Sharp
OUTPUT_CPP_PATH=../../Src/SFEngineDLL/Sharp

SOURCES=ProtocolClusterServer.xml \
	ProtocolEntityServer.xml \
	ProtocolGame.xml \
	ProtocolGameInstance.xml \
	ProtocolGameInstanceManager.xml \
	ProtocolGameMasterServer.xml \
	ProtocolGameParty.xml \
	ProtocolGamePartyManager.xml \
	ProtocolGameServer.xml \
	ProtocolLogin.xml \
	ProtocolLoginServer.xml \
	ProtocolMonitoring.xml \
	ProtocolPartyMatching.xml \
	ProtocolPartyMatchingQueue.xml \
	ProtocolRankingServer.xml \
	ProtocolServer.xml


SOURCES_POSTFIXED=$(SOURCES:.xml=.h)
TARGET_FILES=$(SOURCES_POSTFIXED:%=$(OUTPUT_PATH)/%)

$(OUTPUT_PATH)/%.h: %.xml $(PROTO_BUILD)
	$(PROTO_BUILD) -dir:../../Src/ServerProtocol/Protocol/ $<
 

build: $(SOURCES) $(TARGET_FILES) 
	cp -u ../../Src/ServerProtocol/Protocol/CSharp/*.cs $(OUTPUT_SHARP_PATH)
	cp -u ../../Src/ServerProtocol/Protocol/CSharp/*.cpp $(OUTPUT_CPP_PATH)
	@echo Done



clean:
	rm $(OUTPUT_PATH)/*.h
	rm -R Temp/*