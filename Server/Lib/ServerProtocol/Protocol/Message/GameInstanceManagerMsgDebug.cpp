////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameInstanceManager Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/Message/GameInstanceManagerMsgDebug.h"
#include "Protocol/Message/GameInstanceManagerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 		///////////////////////////////////////////////////////////////
		// GameInstanceManager Debug trace mapping
		extern std::unordered_map<uint32_t,MessageHandlingFunction> MessageDebugTraceMap;

		void RegisterDebugTraceGameInstanceManager()
		{
 			// Cmd: Create a game instance
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstanceManager::CreateGameInstanceCmd::MID.IDSeq.MsgID,&Message::GameInstanceManager::CreateGameInstanceCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstanceManager::CreateGameInstanceRes::MID.IDSeq.MsgID,&Message::GameInstanceManager::CreateGameInstanceRes::TraceOut));
		}; // void RegisterDebugTraceGameInstanceManager()


	}; // namespace Protocol
}; // namespace SF


