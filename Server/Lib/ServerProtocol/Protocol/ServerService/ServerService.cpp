////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : Server Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerEntity/ServerEntity.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityInformation.h"
#include "Protocol/ServerService/ServerService.h"
#include "SvrTrace.h"



namespace SF
{
 	namespace Svr
	{
 		ServerService::ServerService( ServerServiceInformation* pService)
			: ServerServiceBase(pService)
		{
		}


		// Cmd: Generic failure message
		Result ServerService::GenericFailureCmd( const uint64_t &InTransactionID )
		{
 			ScopeContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyServer(GetConnection()).GenericFailureCmd( InRouteContext, InTransactionID ) );

			return hr;

		}; // Result ServerService::GenericFailureCmd( const uint64_t &InTransactionID )
		// C2S: Server Started or Connected
		Result ServerService::ServerConnectedC2SEvt( const EntityID &InSenderEntityID, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress )
		{
 			ScopeContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyServer(GetConnection()).ServerConnectedC2SEvt( InRouteContext, InStartUpTime, InPrivateAddress ) );

			return hr;

		}; // Result ServerService::ServerConnectedC2SEvt( const EntityID &InSenderEntityID, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress )


	}; // namespace Svr
}; // namespace SF


