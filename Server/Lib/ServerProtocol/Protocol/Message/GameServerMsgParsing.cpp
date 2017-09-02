////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StormForge
// 
// Author : Generated
// 
// Description : GameServer Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/Message/GameServerMsgParsing.h"
#include "Protocol/Message/GameServerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// GameServer message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserGameServer()
		{
 			// Cmd: Kick
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::RegisterPlayerToJoinGameServerCmd::MID.IDSeq.MsgID,&Message::GameServer::RegisterPlayerToJoinGameServerCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::RegisterPlayerToJoinGameServerRes::MID.IDSeq.MsgID,&Message::GameServer::RegisterPlayerToJoinGameServerRes::ParseMessageToMessageBase));
			// Cmd: Kick
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd::MID.IDSeq.MsgID,&Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes::MID.IDSeq.MsgID,&Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes::ParseMessageToMessageBase));
			// C2S: Chatting message
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::ChatMessageC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::ChatMessageC2SEvt::ParseMessageToMessageBase));
			// C2S: Notification
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::NotifyC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::NotifyC2SEvt::ParseMessageToMessageBase));
			// C2S: Friend Accept
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::FriendAcceptedC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::FriendAcceptedC2SEvt::ParseMessageToMessageBase));
			// C2S: Friend Remove
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::FriendRemovedC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::FriendRemovedC2SEvt::ParseMessageToMessageBase));
			// C2S: Request Player Status Update
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::RequestPlayerStatusUpdateC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::RequestPlayerStatusUpdateC2SEvt::ParseMessageToMessageBase));
			// C2S: Notify Player Status Updated
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt::ParseMessageToMessageBase));
			// C2S: Notification
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::NotifyPartyInviteC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::NotifyPartyInviteC2SEvt::ParseMessageToMessageBase));
		}; // void RegisterParserGameServer()


	}; // namespace Protocol
}; // namespace SF


