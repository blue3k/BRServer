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
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
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
 			// Cmd: Notify Login server that client is successfully connected and joined to game server so that login server clear the player information.
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::LoginServer::PlayerJoinedToGameServerCmd::MID.IDSeq.MsgID,&Message::LoginServer::PlayerJoinedToGameServerCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::LoginServer::PlayerJoinedToGameServerRes::MID.IDSeq.MsgID,&Message::LoginServer::PlayerJoinedToGameServerRes::ParseMessageToMessageBase));
			// Cmd: Kick logged in player, used to kick player on other login server to prevent duplicated login.
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::LoginServer::KickPlayerCmd::MID.IDSeq.MsgID,&Message::LoginServer::KickPlayerCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::LoginServer::KickPlayerRes::MID.IDSeq.MsgID,&Message::LoginServer::KickPlayerRes::ParseMessageToMessageBase));
		}; // void RegisterParserLoginServer()


	}; // namespace Protocol
}; // namespace SF


