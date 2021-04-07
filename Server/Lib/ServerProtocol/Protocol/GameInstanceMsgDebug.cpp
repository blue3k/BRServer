////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameInstance Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/GameInstanceMsgDebug.h"
#include "Protocol/GameInstanceMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 		///////////////////////////////////////////////////////////////
		// GameInstance Debug trace mapping
		extern std::unordered_map<uint32_t,MessageHandlingFunction> MessageDebugTraceMap;

		void RegisterDebugTraceGameInstance()
		{
 			// Cmd: Join to a game instance. You can call multiple times, but it would be a waste
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::JoinGameInstanceCmd::MID.IDSeq.MsgID,&Message::GameInstance::JoinGameInstanceCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::JoinGameInstanceRes::MID.IDSeq.MsgID,&Message::GameInstance::JoinGameInstanceRes::TraceOut));
			// Cmd: Leave game instance.
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::LeaveGameInstanceCmd::MID.IDSeq.MsgID,&Message::GameInstance::LeaveGameInstanceCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::LeaveGameInstanceRes::MID.IDSeq.MsgID,&Message::GameInstance::LeaveGameInstanceRes::TraceOut));
			// S2C: Player left event.
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::PlayerLeftS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerLeftS2CEvt::TraceOut));
			// Cmd: Player. revive himself
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::GamePlayerReviveCmd::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayerReviveCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::GamePlayerReviveRes::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayerReviveRes::TraceOut));
			// S2C: Player is revived
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::GamePlayerRevivedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayerRevivedS2CEvt::TraceOut));
		}; // void RegisterDebugTraceGameInstance()


	}; // namespace Protocol
}; // namespace SF


