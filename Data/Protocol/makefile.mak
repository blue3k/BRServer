#
PROTO_BUILD=../../StormForge/Bin/SFProtocolCompiler.exe


OUTPUT_PATH=../../Server/Lib/ServerProtocol/Message
OUTPUT_SHARPCPP_PATH=../../Src/SFEngineDLL/Sharp
OUTPUT_SHARP_PATH=../../Src/SFEngineDLL/Sharp
OUTPUT_CPP_PATH=$(OUTPUT_PATH)


OUTPUT_PATH=../../Server/Lib/ServerProtocol/Protocol/Message
OUTPUT_CPP_PATH=$(OUTPUT_PATH)
OUTPUT_CPP_SERVERSYSTEM_PATH=../../Server/Lib/ServerSystem/ServerService


PROTO_BUILD_PARAM_CPP=+outCPP=$(OUTPUT_CPP_PATH) +pch=stdafx.h +LogFuncPrefix=protocolTrace\( +gen=MessageClassBuilder  +gen=MessageParsingMapBuilder  +gen=MessageDebugBuilder 
PROTO_BUILD_PARAM_CPP_POLICY=+outCPP=../../Server/Lib/ServerProtocol/Protocol/Policy +pch=stdafx.h +LogFuncPrefix=protocolTrace\( +gen=MessageNetPolicyBuilder
PROTO_BUILD_PARAM_CPP_SERVERSERVICE=+outCPP=../../Server/Lib/ServerProtocol/Protocol/ServerService +pch=stdafx.h +LogFuncPrefix=protocolTrace\( +gen=MessageServerServiceBuilder


SOURCES=ProtocolClusterServer.xml \
	ProtocolEntityServer.xml \
	ProtocolGameInstance.xml \
	ProtocolGameInstanceManager.xml \
	ProtocolGameParty.xml \
	ProtocolGamePartyManager.xml \
	ProtocolGameServer.xml \
	ProtocolLoginServer.xml \
	ProtocolMonitoring.xml \
	ProtocolPartyMatching.xml \
	ProtocolPartyMatchingQueue.xml \
	ProtocolRankingServer.xml \
	ProtocolServer.xml


SOURCES_POSTFIXED=$(SOURCES:.xml=.h)
TARGET_FILES=$(SOURCES_POSTFIXED:%=$(OUTPUT_PATH)/%)

$(OUTPUT_PATH)/%.h: %.xml $(PROTO_BUILD)
	$(PROTO_BUILD) $(PROTO_BUILD_PARAM_CPP) +in=$<
	$(PROTO_BUILD) $(PROTO_BUILD_PARAM_CPP_POLICY) +in=$<
	$(PROTO_BUILD) $(PROTO_BUILD_PARAM_CPP_SERVERSERVICE) +in=$<
 

build: $(SOURCES) $(TARGET_FILES) 
	@echo Done



clean:
	rm $(OUTPUT_PATH)/*.h
	rm -R Temp/*