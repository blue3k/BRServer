﻿////////////////////////////////////////////////////////////////////////////////
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
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServerService/ServerService.h"
#include "ServerSystem/SvrTrace.h"



namespace SF
{
 	namespace Svr
	{
 		ServerService::ServerService( ServerServiceInformation* pService)
			: ServerServiceBase(pService, Policy::IPolicyServer::ID_POLICY)
		{
			static_assert((UINT)Policy::IPolicyServer::ID_POLICY == (UINT)ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyServer());
		}


		// Cmd: 
		Result ServerService::GenericFailureCmd( const TransactionID &InTransactionID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyServer()->GenericFailureCmd( InRouteContext, InTransactionID ) );

		Proc_End:

			return hr;

		}; // Result ServerService::GenericFailureCmd( const TransactionID &InTransactionID )
		// C2S: Server Started or Connected
		Result ServerService::ServerConnectedC2SEvt( const EntityID &InSenderEntityID, const ServiceInformation &InClusterManagerServiceInformation, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyServer()->ServerConnectedC2SEvt( InRouteContext, InClusterManagerServiceInformation, InStartUpTime, InPrivateAddress ) );

		Proc_End:

			return hr;

		}; // Result ServerService::ServerConnectedC2SEvt( const EntityID &InSenderEntityID, const ServiceInformation &InClusterManagerServiceInformation, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress )


	}; // namespace Svr
}; // namespace SF


