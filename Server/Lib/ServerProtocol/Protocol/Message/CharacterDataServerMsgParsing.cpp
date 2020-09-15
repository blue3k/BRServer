////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 Kyungkun Ko
// 
// Author : Generated
// 
// Description : CharacterDataServer Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/Message/CharacterDataServerMsgParsing.h"
#include "Protocol/Message/CharacterDataServerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// CharacterDataServer message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserCharacterDataServer()
		{
 			// Cmd: Add character data to the account
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::CharacterDataServer::AddCharacterDataCmd::MID.IDSeq.MsgID,&Message::CharacterDataServer::AddCharacterDataCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::CharacterDataServer::AddCharacterDataRes::MID.IDSeq.MsgID,&Message::CharacterDataServer::AddCharacterDataRes::ParseMessageToMessageBase));
			// Cmd: Remove character data
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::CharacterDataServer::RemoveCharacterDataCmd::MID.IDSeq.MsgID,&Message::CharacterDataServer::RemoveCharacterDataCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::CharacterDataServer::RemoveCharacterDataRes::MID.IDSeq.MsgID,&Message::CharacterDataServer::RemoveCharacterDataRes::ParseMessageToMessageBase));
			// Cmd: Get character list
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::CharacterDataServer::GetCharacterDataListCmd::MID.IDSeq.MsgID,&Message::CharacterDataServer::GetCharacterDataListCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::CharacterDataServer::GetCharacterDataListRes::MID.IDSeq.MsgID,&Message::CharacterDataServer::GetCharacterDataListRes::ParseMessageToMessageBase));
			// Cmd: Get character data
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::CharacterDataServer::GetCharacterDataCmd::MID.IDSeq.MsgID,&Message::CharacterDataServer::GetCharacterDataCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::CharacterDataServer::GetCharacterDataRes::MID.IDSeq.MsgID,&Message::CharacterDataServer::GetCharacterDataRes::ParseMessageToMessageBase));
			// Cmd: Set(add or update) attribute value
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::CharacterDataServer::SetAttributeCmd::MID.IDSeq.MsgID,&Message::CharacterDataServer::SetAttributeCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::CharacterDataServer::SetAttributeRes::MID.IDSeq.MsgID,&Message::CharacterDataServer::SetAttributeRes::ParseMessageToMessageBase));
			// Cmd: Remove an attribute value
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::CharacterDataServer::RemoveAttributesCmd::MID.IDSeq.MsgID,&Message::CharacterDataServer::RemoveAttributesCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::CharacterDataServer::RemoveAttributesRes::MID.IDSeq.MsgID,&Message::CharacterDataServer::RemoveAttributesRes::ParseMessageToMessageBase));
			// Cmd: Attribute add
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::CharacterDataServer::AttributeValueAddCmd::MID.IDSeq.MsgID,&Message::CharacterDataServer::AttributeValueAddCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::CharacterDataServer::AttributeValueAddRes::MID.IDSeq.MsgID,&Message::CharacterDataServer::AttributeValueAddRes::ParseMessageToMessageBase));
			// Cmd: Attribute subtract
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::CharacterDataServer::AttributeValueSubCmd::MID.IDSeq.MsgID,&Message::CharacterDataServer::AttributeValueSubCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::CharacterDataServer::AttributeValueSubRes::MID.IDSeq.MsgID,&Message::CharacterDataServer::AttributeValueSubRes::ParseMessageToMessageBase));
			// Cmd: Compare and exchange attribute value
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::CharacterDataServer::AttributeValueCASCmd::MID.IDSeq.MsgID,&Message::CharacterDataServer::AttributeValueCASCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::CharacterDataServer::AttributeValueCASRes::MID.IDSeq.MsgID,&Message::CharacterDataServer::AttributeValueCASRes::ParseMessageToMessageBase));
		}; // void RegisterParserCharacterDataServer()


	}; // namespace Protocol
}; // namespace SF


