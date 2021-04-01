////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/GameServerMsgDebug.h"
#include "Protocol/GameServerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 		///////////////////////////////////////////////////////////////
		// GameServer Debug trace mapping
		extern std::unordered_map<uint32_t,MessageHandlingFunction> MessageDebugTraceMap;

		void RegisterDebugTraceGameServer()
		{
 			// Cmd: Register player so that the player can connection and join to the game server.
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::RegisterPlayerToJoinGameServerCmd::MID.IDSeq.MsgID,&Message::GameServer::RegisterPlayerToJoinGameServerCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::RegisterPlayerToJoinGameServerRes::MID.IDSeq.MsgID,&Message::GameServer::RegisterPlayerToJoinGameServerRes::TraceOut));
			// Cmd: Same to RegisterPlayerToJoinGameServer, but can run on player entity. only works when the player entity exists. If player disconnected for some reason, you can run this method to connect.
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd::MID.IDSeq.MsgID,&Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes::MID.IDSeq.MsgID,&Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes::TraceOut));
			// C2S: Chatting message event.
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::ChatMessageC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::ChatMessageC2SEvt::TraceOut));
			// C2S: Notification event for P2P
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::NotifyC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::NotifyC2SEvt::TraceOut));
			// C2S: Friend Accepted
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::FriendAcceptedC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::FriendAcceptedC2SEvt::TraceOut));
			// C2S: Friend Remove
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::FriendRemovedC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::FriendRemovedC2SEvt::TraceOut));
			// C2S: Request Player Status Update
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::RequestPlayerStatusUpdateC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::RequestPlayerStatusUpdateC2SEvt::TraceOut));
			// C2S: Notify Player Status Updated
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt::TraceOut));
			// C2S: Party invite Notification
			MessageDebugTraceMap.insert(std::make_pair(Message::GameServer::NotifyPartyInviteC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::NotifyPartyInviteC2SEvt::TraceOut));
		}; // void RegisterDebugTraceGameServer()


	}; // namespace Protocol
}; // namespace SF


