////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : GameMasterServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/Message/GameMasterServerMsgDebug.h"
#include "Protocol/Message/GameMasterServerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 		///////////////////////////////////////////////////////////////
		// GameMasterServer Debug trace mapping
		extern std::unordered_map<uint32_t,MessageHandlingFunction> MessageDebugTraceMap;

		void RegisterDebugTraceGameMasterServer()
		{
 			// C2S: Player entered
			MessageDebugTraceMap.insert(std::make_pair(Message::GameMasterServer::PlayerEnteredC2SEvt::MID.IDSeq.MsgID,&Message::GameMasterServer::PlayerEnteredC2SEvt::TraceOut));
			// C2S: Player leaved
			MessageDebugTraceMap.insert(std::make_pair(Message::GameMasterServer::PlayerLeavedC2SEvt::MID.IDSeq.MsgID,&Message::GameMasterServer::PlayerLeavedC2SEvt::TraceOut));
		}; // void RegisterDebugTraceGameMasterServer()


	}; // namespace Protocol
}; // namespace SF


