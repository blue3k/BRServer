////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : GameServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/Message/GameServerMsgDebug.h"
#include "Protocol/Message/GameServerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 		///////////////////////////////////////////////////////////////
		// GameServer Debug trace mapping
		extern std::unordered_map<uint32_t,MessageHandlingFunction> MessageDebugTraceMap;

		void RegisterDebugTraceGameServer()
		{
 			// Cmd: Kick
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::RegisterPlayerToJoinGameServerCmd::MID.IDSeq.MsgID,&Message::GameServer::RegisterPlayerToJoinGameServerCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::RegisterPlayerToJoinGameServerRes::MID.IDSeq.MsgID,&Message::GameServer::RegisterPlayerToJoinGameServerRes::TraceOut));
			// Cmd: Kick
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd::MID.IDSeq.MsgID,&Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes::MID.IDSeq.MsgID,&Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes::TraceOut));
			// C2S: Chatting message
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::ChatMessageC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::ChatMessageC2SEvt::TraceOut));
			// C2S: Notification
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::NotifyC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::NotifyC2SEvt::TraceOut));
			// C2S: Friend Accept
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::FriendAcceptedC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::FriendAcceptedC2SEvt::TraceOut));
			// C2S: Friend Remove
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::FriendRemovedC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::FriendRemovedC2SEvt::TraceOut));
			// C2S: Request Player Status Update
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::RequestPlayerStatusUpdateC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::RequestPlayerStatusUpdateC2SEvt::TraceOut));
			// C2S: Notify Player Status Updated
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt::TraceOut));
			// C2S: Notification
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::NotifyPartyInviteC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::NotifyPartyInviteC2SEvt::TraceOut));
		}; // void RegisterDebugTraceGameServer()


	}; // namespace Protocol
}; // namespace SF


