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

		extern std::unordered_map<uint32_t,HandleParseMessageTo> MessageParseToVariableMap;
		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserGameServer()
		{
 			// Cmd: Kick
			MessageParseToVariableMap.insert(std::make_pair(Message::GameServer::RegisterPlayerToJoinGameServerCmd::MID.IDSeq.MsgID,&Message::GameServer::RegisterPlayerToJoinGameServerCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::RegisterPlayerToJoinGameServerCmd::MID.IDSeq.MsgID,&Message::GameServer::RegisterPlayerToJoinGameServerCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::GameServer::RegisterPlayerToJoinGameServerRes::MID.IDSeq.MsgID,&Message::GameServer::RegisterPlayerToJoinGameServerRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::RegisterPlayerToJoinGameServerRes::MID.IDSeq.MsgID,&Message::GameServer::RegisterPlayerToJoinGameServerRes::ParseMessageToMessageBase));
			// Cmd: Kick
			MessageParseToVariableMap.insert(std::make_pair(Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd::MID.IDSeq.MsgID,&Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd::MID.IDSeq.MsgID,&Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes::MID.IDSeq.MsgID,&Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes::MID.IDSeq.MsgID,&Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes::ParseMessageToMessageBase));
			// C2S: Chatting message
			MessageParseToVariableMap.insert(std::make_pair(Message::GameServer::ChatMessageC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::ChatMessageC2SEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::ChatMessageC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::ChatMessageC2SEvt::ParseMessageToMessageBase));
			// C2S: Notification
			MessageParseToVariableMap.insert(std::make_pair(Message::GameServer::NotifyC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::NotifyC2SEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::NotifyC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::NotifyC2SEvt::ParseMessageToMessageBase));
			// C2S: Friend Accept
			MessageParseToVariableMap.insert(std::make_pair(Message::GameServer::FriendAcceptedC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::FriendAcceptedC2SEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::FriendAcceptedC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::FriendAcceptedC2SEvt::ParseMessageToMessageBase));
			// C2S: Friend Remove
			MessageParseToVariableMap.insert(std::make_pair(Message::GameServer::FriendRemovedC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::FriendRemovedC2SEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::FriendRemovedC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::FriendRemovedC2SEvt::ParseMessageToMessageBase));
			// C2S: Request Player Status Update
			MessageParseToVariableMap.insert(std::make_pair(Message::GameServer::RequestPlayerStatusUpdateC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::RequestPlayerStatusUpdateC2SEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::RequestPlayerStatusUpdateC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::RequestPlayerStatusUpdateC2SEvt::ParseMessageToMessageBase));
			// C2S: Notify Player Status Updated
			MessageParseToVariableMap.insert(std::make_pair(Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt::ParseMessageToMessageBase));
			// C2S: Notification
			MessageParseToVariableMap.insert(std::make_pair(Message::GameServer::NotifyPartyInviteC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::NotifyPartyInviteC2SEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameServer::NotifyPartyInviteC2SEvt::MID.IDSeq.MsgID,&Message::GameServer::NotifyPartyInviteC2SEvt::ParseMessageToMessageBase));
		}; // void RegisterParserGameServer()


	}; // namespace Protocol
}; // namespace SF


