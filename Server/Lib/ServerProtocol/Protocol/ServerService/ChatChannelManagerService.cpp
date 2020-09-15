////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : ChatChannelManager Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerEntity/ServerEntity.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityInformation.h"
#include "Protocol/ServerService/ChatChannelManagerService.h"
#include "SvrTrace.h"



namespace SF
{
 	namespace Svr
	{
 		ChatChannelManagerService::ChatChannelManagerService( ServerServiceInformation* pService)
			: ServerServiceBase(pService)
		{
		}


		// Cmd: Create a channel instance
		Result ChatChannelManagerService::CreateChannelCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName, const char* InPasscode, const PlayerInformation &InCreator )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyChatChannelManager(GetConnection()).CreateChannelCmd( InRouteContext, InTransactionID, InRouteHopCount, InChannelName, InPasscode, InCreator ) );

			return hr;

		}; // Result ChatChannelManagerService::CreateChannelCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName, const char* InPasscode, const PlayerInformation &InCreator )
		// Cmd: Find a channel instance with name
		Result ChatChannelManagerService::FindChannelCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyChatChannelManager(GetConnection()).FindChannelCmd( InRouteContext, InTransactionID, InRouteHopCount, InChannelName ) );

			return hr;

		}; // Result ChatChannelManagerService::FindChannelCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName )
		// C2S: Notification that a chat channel instance has deleted
		Result ChatChannelManagerService::ChatChannelDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyChatChannelManager(GetConnection()).ChatChannelDeletedC2SEvt( InRouteContext, InRouteHopCount ) );

			return hr;

		}; // Result ChatChannelManagerService::ChatChannelDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount )


	}; // namespace Svr
}; // namespace SF


