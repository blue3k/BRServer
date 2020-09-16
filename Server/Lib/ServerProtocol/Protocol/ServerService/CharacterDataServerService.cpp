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
#include "ServerEntity/ServerEntity.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityInformation.h"
#include "Protocol/ServerService/CharacterDataServerService.h"
#include "SvrTrace.h"



namespace SF
{
 	namespace Svr
	{
 		CharacterDataServerService::CharacterDataServerService( ServerServiceInformation* pService)
			: ServerServiceBase(pService)
		{
		}


		// Cmd: Add character data to the account
		Result CharacterDataServerService::AddCharacterDataCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const char* InCharacterName, const VariableTable &InAttributes )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyCharacterDataServer(GetConnection()).AddCharacterDataCmd( InRouteContext, InTransactionID, InRouteHopCount, InPlayerID, InCharacterName, InAttributes ) );

			return hr;

		}; // Result CharacterDataServerService::AddCharacterDataCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const char* InCharacterName, const VariableTable &InAttributes )
		// Cmd: Remove character data
		Result CharacterDataServerService::RemoveCharacterDataCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyCharacterDataServer(GetConnection()).RemoveCharacterDataCmd( InRouteContext, InTransactionID, InRouteHopCount, InPlayerID, InCharacterName ) );

			return hr;

		}; // Result CharacterDataServerService::RemoveCharacterDataCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName )
		// Cmd: Get character list
		Result CharacterDataServerService::GetCharacterDataListCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyCharacterDataServer(GetConnection()).GetCharacterDataListCmd( InRouteContext, InTransactionID, InRouteHopCount, InPlayerID, InCharacterName ) );

			return hr;

		}; // Result CharacterDataServerService::GetCharacterDataListCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName )
		// Cmd: Get character data
		Result CharacterDataServerService::GetCharacterDataCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyCharacterDataServer(GetConnection()).GetCharacterDataCmd( InRouteContext, InTransactionID, InRouteHopCount, InPlayerID, InCharacterName ) );

			return hr;

		}; // Result CharacterDataServerService::GetCharacterDataCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName )
		// Cmd: Set(add or update) attribute value
		Result CharacterDataServerService::SetAttributeCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const VariableTable &InAttributes )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyCharacterDataServer(GetConnection()).SetAttributeCmd( InRouteContext, InTransactionID, InRouteHopCount, InPlayerID, InCharacterName, InAttributes ) );

			return hr;

		}; // Result CharacterDataServerService::SetAttributeCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const VariableTable &InAttributes )
		// Cmd: Remove an attribute value
		Result CharacterDataServerService::RemoveAttributesCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const Array<StringCrc32>& InAttributeNames )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyCharacterDataServer(GetConnection()).RemoveAttributesCmd( InRouteContext, InTransactionID, InRouteHopCount, InPlayerID, InCharacterName, InAttributeNames ) );

			return hr;

		}; // Result CharacterDataServerService::RemoveAttributesCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const Array<StringCrc32>& InAttributeNames )
		// Cmd: Attribute add
		Result CharacterDataServerService::AttributeValueAddCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const float &InValue )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyCharacterDataServer(GetConnection()).AttributeValueAddCmd( InRouteContext, InTransactionID, InRouteHopCount, InPlayerID, InCharacterName, InAttributeName, InValue ) );

			return hr;

		}; // Result CharacterDataServerService::AttributeValueAddCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const float &InValue )
		// Cmd: Attribute subtract
		Result CharacterDataServerService::AttributeValueSubCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const float &InValue )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyCharacterDataServer(GetConnection()).AttributeValueSubCmd( InRouteContext, InTransactionID, InRouteHopCount, InPlayerID, InCharacterName, InAttributeName, InValue ) );

			return hr;

		}; // Result CharacterDataServerService::AttributeValueSubCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const float &InValue )
		// Cmd: Compare and exchange attribute value
		Result CharacterDataServerService::AttributeValueCASCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const StringCrc32 &InAttributeType, const uint64_t &InExpected, const uint64_t &InNewValue )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyCharacterDataServer(GetConnection()).AttributeValueCASCmd( InRouteContext, InTransactionID, InRouteHopCount, InPlayerID, InCharacterName, InAttributeName, InAttributeType, InExpected, InNewValue ) );

			return hr;

		}; // Result CharacterDataServerService::AttributeValueCASCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const StringCrc32 &InAttributeType, const uint64_t &InExpected, const uint64_t &InNewValue )


	}; // namespace Svr
}; // namespace SF


