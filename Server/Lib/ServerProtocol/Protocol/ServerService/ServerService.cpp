////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : Server Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
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


		// Cmd: 
		Result ServerService::GenericFailureCmd( const TransactionID &InTransactionID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyServer(GetConnection()).GenericFailureCmd( InRouteContext, InTransactionID ) );

		Proc_End:

			return hr;

		}; // Result ServerService::GenericFailureCmd( const TransactionID &InTransactionID )
		// C2S: Server Started or Connected
		Result ServerService::ServerConnectedC2SEvt( const EntityID &InSenderEntityID, const ServiceInformation &InClusterManagerServiceInformation, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyServer(GetConnection()).ServerConnectedC2SEvt( InRouteContext, InClusterManagerServiceInformation, InStartUpTime, InPrivateAddress ) );

		Proc_End:

			return hr;

		}; // Result ServerService::ServerConnectedC2SEvt( const EntityID &InSenderEntityID, const ServiceInformation &InClusterManagerServiceInformation, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress )


	}; // namespace Svr
}; // namespace SF


