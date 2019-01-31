////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : GameInstanceManager Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
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
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstanceManager::CreateGameCmd::MID.IDSeq.MsgID,&Message::GameInstanceManager::CreateGameCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstanceManager::CreateGameRes::MID.IDSeq.MsgID,&Message::GameInstanceManager::CreateGameRes::TraceOut));
			// C2S: Game instance notification of deletion
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstanceManager::GameDeletedC2SEvt::MID.IDSeq.MsgID,&Message::GameInstanceManager::GameDeletedC2SEvt::TraceOut));
		}; // void RegisterDebugTraceGameInstanceManager()


	}; // namespace Protocol
}; // namespace SF


