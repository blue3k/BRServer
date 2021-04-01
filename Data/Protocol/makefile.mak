#
PROTO_BUILD=../../../StormForge/Bin/SFProtocolCompiler.exe



OUTPUT_DOC_PATH=../../Doc/Protocol
OUTPUT_SHARPCPP_PATH=../../Src/SFEngineDLL/Sharp
OUTPUT_SHARP_PATH=../../Src/SFEngineDLL/Sharp

OUTPUT_PATH=../../Server/Lib/ServerProtocol/Protocol
OUTPUT_CPP_PATH=$(OUTPUT_PATH)



PROTO_BUILD_PARAM_CPP=+outCPP=$(OUTPUT_CPP_PATH) +pch=ServerProtocolPCH.h +LogFuncPrefix=protocolTrace\( +gen=MessageClassBuilder  +gen=MessageParsingMapBuilder  +gen=MessageDebugBuilder 
PROTO_BUILD_PARAM_CPP_POLICY=+outCPP=../../Server/Lib/ServerProtocol/Protocol +pch=ServerProtocolPCH.h +LogFuncPrefix=protocolTrace\( +gen=MessageNetPolicyBuilder
PROTO_BUILD_PARAM_CPP_SERVERSERVICE=+outCPP=../../Server/Lib/ServerProtocol/Protocol +pch=ServerProtocolPCH.h +LogFuncPrefix=protocolTrace\( +gen=MessageServerServiceBuilder
PROTO_BUILD_PARAM_DOC=+out=$(OUTPUT_DOC_PATH) +gen=MessageGithubDocBuilder


SOURCES=ProtocolClusterServer.xml \
	ProtocolGameInstanceServer.xml \
	ProtocolGameInstanceManager.xml \
	ProtocolGameParty.xml \
	ProtocolGamePartyManager.xml \
	ProtocolGameServer.xml \
	ProtocolLoginServer.xml \
	ProtocolMonitoring.xml \
	ProtocolPartyMatching.xml \
	ProtocolPartyMatchingQueue.xml \
	ProtocolRankingServer.xml \
	ProtocolServer.xml \
	ProtocolChatChannel.xml \
	ProtocolChatChannelManager.xml \
	ProtocolRelayServer.xml 

SOURCES_POSTFIXED=$(SOURCES:.xml=.h)
TARGET_FILES=$(SOURCES_POSTFIXED:%=$(OUTPUT_PATH)/%)


$(OUTPUT_PATH)/%.h: %.xml $(PROTO_BUILD)
	$(PROTO_BUILD) $(PROTO_BUILD_PARAM_CPP) +in=$<
	$(PROTO_BUILD) $(PROTO_BUILD_PARAM_CPP_POLICY) +in=$<
	$(PROTO_BUILD) $(PROTO_BUILD_PARAM_CPP_SERVERSERVICE) +in=$<
	$(PROTO_BUILD) $(PROTO_BUILD_PARAM_DOC) +in=$<


build: $(SOURCES) $(TARGET_FILES) 
	@echo Done



clean:
	rm $(OUTPUT_PATH)/*.h
	rm -R Temp/*
	
	