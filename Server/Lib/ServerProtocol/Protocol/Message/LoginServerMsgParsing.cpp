////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StormForge
// 
// Author : Generated
// 
// Description : LoginServer Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/Message/LoginServerMsgParsing.h"
#include "Protocol/Message/LoginServerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// LoginServer message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserLoginServer()
		{
 			// Cmd: Notify user joind and see it's valid authticket instance
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::LoginServer::PlayerJoinedToGameServerCmd::MID.IDSeq.MsgID,&Message::LoginServer::PlayerJoinedToGameServerCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::LoginServer::PlayerJoinedToGameServerRes::MID.IDSeq.MsgID,&Message::LoginServer::PlayerJoinedToGameServerRes::ParseMessageToMessageBase));
			// Cmd: Kick logged in player
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::LoginServer::KickPlayerCmd::MID.IDSeq.MsgID,&Message::LoginServer::KickPlayerCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::LoginServer::KickPlayerRes::MID.IDSeq.MsgID,&Message::LoginServer::KickPlayerRes::ParseMessageToMessageBase));
		}; // void RegisterParserLoginServer()


	}; // namespace Protocol
}; // namespace SF


