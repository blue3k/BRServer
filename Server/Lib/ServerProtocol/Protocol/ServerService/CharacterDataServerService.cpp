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
		Result CharacterDataServerService::AddCharacterDataCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const Array<NamedBoxedValue>& InAttributes )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyCharacterDataServer(GetConnection()).AddCharacterDataCmd( InRouteContext, InTransactionID, InPlayerID, InCharacterName, InAttributes ) );

		Proc_End:

			return hr;

		}; // Result CharacterDataServerService::AddCharacterDataCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const Array<NamedBoxedValue>& InAttributes )
		// Cmd: Remove character data
		Result CharacterDataServerService::RemoveCharacterDataCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyCharacterDataServer(GetConnection()).RemoveCharacterDataCmd( InRouteContext, InTransactionID, InPlayerID, InCharacterName ) );

		Proc_End:

			return hr;

		}; // Result CharacterDataServerService::RemoveCharacterDataCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName )
		// Cmd: Get character data
		Result CharacterDataServerService::GetCharacterDataCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyCharacterDataServer(GetConnection()).GetCharacterDataCmd( InRouteContext, InTransactionID, InPlayerID, InCharacterName ) );

		Proc_End:

			return hr;

		}; // Result CharacterDataServerService::GetCharacterDataCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName )
		// Cmd: Set(add or update) attribute value
		Result CharacterDataServerService::SetAttributeCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const Array<NamedBoxedValue>& InAttributes )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyCharacterDataServer(GetConnection()).SetAttributeCmd( InRouteContext, InTransactionID, InPlayerID, InCharacterName, InAttributes ) );

		Proc_End:

			return hr;

		}; // Result CharacterDataServerService::SetAttributeCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const Array<NamedBoxedValue>& InAttributes )
		// Cmd: Remove an attribute value
		Result CharacterDataServerService::RemoveAttributeCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const Array<StringCrc32>& InAttributeNames )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyCharacterDataServer(GetConnection()).RemoveAttributeCmd( InRouteContext, InTransactionID, InPlayerID, InCharacterName, InAttributeNames ) );

		Proc_End:

			return hr;

		}; // Result CharacterDataServerService::RemoveAttributeCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const Array<StringCrc32>& InAttributeNames )
		// Cmd: Attribute add
		Result CharacterDataServerService::AttributeValueAddCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const FLOAT &InValue )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyCharacterDataServer(GetConnection()).AttributeValueAddCmd( InRouteContext, InTransactionID, InPlayerID, InCharacterName, InAttributeName, InValue ) );

		Proc_End:

			return hr;

		}; // Result CharacterDataServerService::AttributeValueAddCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const FLOAT &InValue )
		// Cmd: Attribute subtract
		Result CharacterDataServerService::AttributeValueSubCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const FLOAT &InValue )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyCharacterDataServer(GetConnection()).AttributeValueSubCmd( InRouteContext, InTransactionID, InPlayerID, InCharacterName, InAttributeName, InValue ) );

		Proc_End:

			return hr;

		}; // Result CharacterDataServerService::AttributeValueSubCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const FLOAT &InValue )
		// Cmd: Compare and exchange attribute value
		Result CharacterDataServerService::AttributeValueCASCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const StringCrc32 &InAttributeType, const uint64_t &InExpected, const uint64_t &InNewValue )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyCharacterDataServer(GetConnection()).AttributeValueCASCmd( InRouteContext, InTransactionID, InPlayerID, InCharacterName, InAttributeName, InAttributeType, InExpected, InNewValue ) );

		Proc_End:

			return hr;

		}; // Result CharacterDataServerService::AttributeValueCASCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const StringCrc32 &InAttributeType, const uint64_t &InExpected, const uint64_t &InNewValue )


	}; // namespace Svr
}; // namespace SF


