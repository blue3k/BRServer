#
PROTO_BUILD=../../../bin/tools/ProtocolBuilder.exe

OUTPUT_PATH=../../Protocol/Message

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
	ProtocolRanking.xml \
	ProtocolServer.xml


SOURCES_POSTFIXED=$(SOURCES:.xml=.h)
TARGET_FILES=$(SOURCES_POSTFIXED:%=$(OUTPUT_PATH)/%)


$(OUTPUT_PATH)/%.h: %.xml
	$(PROTO_BUILD) -dir:../../../ $<
 

build: $(SOURCES) $(TARGET_FILES) 
	@echo Done


clean:
	rm $(OUTPUT_PATH)/*.h
	rm -R Temp/*