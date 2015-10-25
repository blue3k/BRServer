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


SOURCES_POSTFIXED=$(SOURCES:.xml=MsgClass.h)
TARGET_FILES=$(SOURCES_POSTFIXED:Protocol%=$(OUTPUT_PATH)/%)


$(OUTPUT_PATH)/%MsgClass.h: Protocol%.xml
	$(PROTO_BUILD) -dir:../../../ $<
 

all: $(SOURCES) $(TARGET_FILES) 
	@echo Done
