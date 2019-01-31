////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : ChatChannelManager Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
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
		Result ChatChannelManagerService::CreateChannelCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName, const char* InPasscode, const PlayerInformation &InCreator )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyChatChannelManager(GetConnection()).CreateChannelCmd( InRouteContext, InTransactionID, InRouteHopCount, InChannelName, InPasscode, InCreator ) );

		Proc_End:

			return hr;

		}; // Result ChatChannelManagerService::CreateChannelCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName, const char* InPasscode, const PlayerInformation &InCreator )
		// Cmd: Find a channel instance with name
		Result ChatChannelManagerService::FindChannelCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyChatChannelManager(GetConnection()).FindChannelCmd( InRouteContext, InTransactionID, InRouteHopCount, InChannelName ) );

		Proc_End:

			return hr;

		}; // Result ChatChannelManagerService::FindChannelCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName )
		// C2S: Notification that a chat channel instance has deleted
		Result ChatChannelManagerService::ChatChannelDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyChatChannelManager(GetConnection()).ChatChannelDeletedC2SEvt( InRouteContext, InRouteHopCount ) );

		Proc_End:

			return hr;

		}; // Result ChatChannelManagerService::ChatChannelDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount )


	}; // namespace Svr
}; // namespace SF


