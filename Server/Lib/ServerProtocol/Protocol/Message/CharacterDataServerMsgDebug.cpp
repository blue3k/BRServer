////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : CharacterDataServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/Message/CharacterDataServerMsgDebug.h"
#include "Protocol/Message/CharacterDataServerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 		///////////////////////////////////////////////////////////////
		// CharacterDataServer Debug trace mapping
		extern std::unordered_map<uint32_t,MessageHandlingFunction> MessageDebugTraceMap;

		void RegisterDebugTraceCharacterDataServer()
		{
 			// Cmd: Add character data to the account
			MessageDebugTraceMap.insert(std::make_pair(Message::CharacterDataServer::AddCharacterDataCmd::MID.IDSeq.MsgID,&Message::CharacterDataServer::AddCharacterDataCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::CharacterDataServer::AddCharacterDataRes::MID.IDSeq.MsgID,&Message::CharacterDataServer::AddCharacterDataRes::TraceOut));
			// Cmd: Remove character data
			MessageDebugTraceMap.insert(std::make_pair(Message::CharacterDataServer::RemoveCharacterDataCmd::MID.IDSeq.MsgID,&Message::CharacterDataServer::RemoveCharacterDataCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::CharacterDataServer::RemoveCharacterDataRes::MID.IDSeq.MsgID,&Message::CharacterDataServer::RemoveCharacterDataRes::TraceOut));
			// Cmd: Get character data
			MessageDebugTraceMap.insert(std::make_pair(Message::CharacterDataServer::GetCharacterDataCmd::MID.IDSeq.MsgID,&Message::CharacterDataServer::GetCharacterDataCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::CharacterDataServer::GetCharacterDataRes::MID.IDSeq.MsgID,&Message::CharacterDataServer::GetCharacterDataRes::TraceOut));
			// Cmd: Set(add or update) attribute value
			MessageDebugTraceMap.insert(std::make_pair(Message::CharacterDataServer::SetAttributeCmd::MID.IDSeq.MsgID,&Message::CharacterDataServer::SetAttributeCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::CharacterDataServer::SetAttributeRes::MID.IDSeq.MsgID,&Message::CharacterDataServer::SetAttributeRes::TraceOut));
			// Cmd: Remove an attribute value
			MessageDebugTraceMap.insert(std::make_pair(Message::CharacterDataServer::RemoveAttributesCmd::MID.IDSeq.MsgID,&Message::CharacterDataServer::RemoveAttributesCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::CharacterDataServer::RemoveAttributesRes::MID.IDSeq.MsgID,&Message::CharacterDataServer::RemoveAttributesRes::TraceOut));
			// Cmd: Attribute add
			MessageDebugTraceMap.insert(std::make_pair(Message::CharacterDataServer::AttributeValueAddCmd::MID.IDSeq.MsgID,&Message::CharacterDataServer::AttributeValueAddCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::CharacterDataServer::AttributeValueAddRes::MID.IDSeq.MsgID,&Message::CharacterDataServer::AttributeValueAddRes::TraceOut));
			// Cmd: Attribute subtract
			MessageDebugTraceMap.insert(std::make_pair(Message::CharacterDataServer::AttributeValueSubCmd::MID.IDSeq.MsgID,&Message::CharacterDataServer::AttributeValueSubCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::CharacterDataServer::AttributeValueSubRes::MID.IDSeq.MsgID,&Message::CharacterDataServer::AttributeValueSubRes::TraceOut));
			// Cmd: Compare and exchange attribute value
			MessageDebugTraceMap.insert(std::make_pair(Message::CharacterDataServer::AttributeValueCASCmd::MID.IDSeq.MsgID,&Message::CharacterDataServer::AttributeValueCASCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::CharacterDataServer::AttributeValueCASRes::MID.IDSeq.MsgID,&Message::CharacterDataServer::AttributeValueCASRes::TraceOut));
		}; // void RegisterDebugTraceCharacterDataServer()


	}; // namespace Protocol
}; // namespace SF


