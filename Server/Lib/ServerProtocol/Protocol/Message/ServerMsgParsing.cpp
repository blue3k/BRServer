////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StormForge
// 
// Author : Generated
// 
// Description : Server Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/Message/ServerMsgParsing.h"
#include "Protocol/Message/ServerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// Server message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageTo> MessageParseToVariableMap;
		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserServer()
		{
 			// Cmd: 
			MessageParseToVariableMap.insert(std::make_pair(Message::Server::GenericFailureCmd::MID.IDSeq.MsgID,&Message::Server::GenericFailureCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::Server::GenericFailureCmd::MID.IDSeq.MsgID,&Message::Server::GenericFailureCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::Server::GenericFailureRes::MID.IDSeq.MsgID,&Message::Server::GenericFailureRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::Server::GenericFailureRes::MID.IDSeq.MsgID,&Message::Server::GenericFailureRes::ParseMessageToMessageBase));
			// C2S: Server Started or Connected
			MessageParseToVariableMap.insert(std::make_pair(Message::Server::ServerConnectedC2SEvt::MID.IDSeq.MsgID,&Message::Server::ServerConnectedC2SEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::Server::ServerConnectedC2SEvt::MID.IDSeq.MsgID,&Message::Server::ServerConnectedC2SEvt::ParseMessageToMessageBase));
		}; // void RegisterParserServer()


	}; // namespace Protocol
}; // namespace SF


