////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 Kyungkun Ko
// 
// Author : Generated
// 
// Description : RelayServer Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/RelayServerMsgParsing.h"
#include "Protocol/RelayServerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// RelayServer message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserRelayServer()
		{
 			// Cmd: Relay Instance
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RelayServer::CreateRelayInstanceCmd::MID.IDSeq.MsgID,&Message::RelayServer::CreateRelayInstanceCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RelayServer::CreateRelayInstanceRes::MID.IDSeq.MsgID,&Message::RelayServer::CreateRelayInstanceRes::ParseMessageToMessageBase));
			// Cmd: Add a player to Relay
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RelayServer::AddPlayerCmd::MID.IDSeq.MsgID,&Message::RelayServer::AddPlayerCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RelayServer::AddPlayerRes::MID.IDSeq.MsgID,&Message::RelayServer::AddPlayerRes::ParseMessageToMessageBase));
			// Cmd: Remove a player to Relay
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RelayServer::RemovePlayerCmd::MID.IDSeq.MsgID,&Message::RelayServer::RemovePlayerCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RelayServer::RemovePlayerRes::MID.IDSeq.MsgID,&Message::RelayServer::RemovePlayerRes::ParseMessageToMessageBase));
		}; // void RegisterParserRelayServer()


	}; // namespace Protocol
}; // namespace SF


