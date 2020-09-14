////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
// 
// Author : Generated
// 
// Description : CharacterDataServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Net/SFNetDef.h"
#include "Net/SFMessage.h"
#include "Net/SFConnection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/CharacterDataServerNetPolicy.h"
#include "Protocol/Message/CharacterDataServerMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 		// Cmd: Add character data to the account
		Result NetPolicyCharacterDataServer::AddCharacterDataCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const VariableTable &InAttributes )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::CharacterDataServer::AddCharacterDataCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID, InCharacterName, InAttributes);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyCharacterDataServer::AddCharacterDataCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const VariableTable &InAttributes )
		// Cmd: Remove character data
		Result NetPolicyCharacterDataServer::RemoveCharacterDataCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::CharacterDataServer::RemoveCharacterDataCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID, InCharacterName);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyCharacterDataServer::RemoveCharacterDataCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName )
		// Cmd: Get character data
		Result NetPolicyCharacterDataServer::GetCharacterDataCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::CharacterDataServer::GetCharacterDataCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID, InCharacterName);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyCharacterDataServer::GetCharacterDataCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName )
		// Cmd: Set(add or update) attribute value
		Result NetPolicyCharacterDataServer::SetAttributeCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const VariableTable &InAttributes )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::CharacterDataServer::SetAttributeCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID, InCharacterName, InAttributes);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyCharacterDataServer::SetAttributeCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const VariableTable &InAttributes )
		// Cmd: Remove an attribute value
		Result NetPolicyCharacterDataServer::RemoveAttributesCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const Array<StringCrc32>& InAttributeNames )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::CharacterDataServer::RemoveAttributesCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID, InCharacterName, InAttributeNames);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyCharacterDataServer::RemoveAttributesCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const Array<StringCrc32>& InAttributeNames )
		// Cmd: Attribute add
		Result NetPolicyCharacterDataServer::AttributeValueAddCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const FLOAT &InValue )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::CharacterDataServer::AttributeValueAddCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID, InCharacterName, InAttributeName, InValue);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyCharacterDataServer::AttributeValueAddCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const FLOAT &InValue )
		// Cmd: Attribute subtract
		Result NetPolicyCharacterDataServer::AttributeValueSubCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const FLOAT &InValue )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::CharacterDataServer::AttributeValueSubCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID, InCharacterName, InAttributeName, InValue);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyCharacterDataServer::AttributeValueSubCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const FLOAT &InValue )
		// Cmd: Compare and exchange attribute value
		Result NetPolicyCharacterDataServer::AttributeValueCASCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const StringCrc32 &InAttributeType, const uint64_t &InExpected, const uint64_t &InNewValue )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::CharacterDataServer::AttributeValueCASCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID, InCharacterName, InAttributeName, InAttributeType, InExpected, InNewValue);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyCharacterDataServer::AttributeValueCASCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const StringCrc32 &InAttributeType, const uint64_t &InExpected, const uint64_t &InNewValue )


		// Cmd: Add character data to the account
		Result NetSvrPolicyCharacterDataServer::AddCharacterDataRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::CharacterDataServer::AddCharacterDataRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyCharacterDataServer::AddCharacterDataRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		// Cmd: Remove character data
		Result NetSvrPolicyCharacterDataServer::RemoveCharacterDataRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::CharacterDataServer::RemoveCharacterDataRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyCharacterDataServer::RemoveCharacterDataRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		// Cmd: Get character data
		Result NetSvrPolicyCharacterDataServer::GetCharacterDataRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const StringCrc32 &InCharacterName, const VariableTable &InAttributes )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::CharacterDataServer::GetCharacterDataRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult, InCharacterName, InAttributes);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyCharacterDataServer::GetCharacterDataRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const StringCrc32 &InCharacterName, const VariableTable &InAttributes )
		// Cmd: Set(add or update) attribute value
		Result NetSvrPolicyCharacterDataServer::SetAttributeRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::CharacterDataServer::SetAttributeRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyCharacterDataServer::SetAttributeRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		// Cmd: Remove an attribute value
		Result NetSvrPolicyCharacterDataServer::RemoveAttributesRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::CharacterDataServer::RemoveAttributesRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyCharacterDataServer::RemoveAttributesRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		// Cmd: Attribute add
		Result NetSvrPolicyCharacterDataServer::AttributeValueAddRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::CharacterDataServer::AttributeValueAddRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyCharacterDataServer::AttributeValueAddRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		// Cmd: Attribute subtract
		Result NetSvrPolicyCharacterDataServer::AttributeValueSubRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::CharacterDataServer::AttributeValueSubRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyCharacterDataServer::AttributeValueSubRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		// Cmd: Compare and exchange attribute value
		Result NetSvrPolicyCharacterDataServer::AttributeValueCASRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::CharacterDataServer::AttributeValueCASRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyCharacterDataServer::AttributeValueCASRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )


	}; // namespace Policy
}; // namespace SF


