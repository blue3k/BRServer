////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Server Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Common/BrBaseTypes.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServerService/ServerService.h"
#include "ServerSystem/SvrTrace.h"



namespace BR
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
		HRESULT ServerService::GenericFailureCmd( const TransactionID &InTransactionID )
		{
 			HRESULT hr = S_SYSTEM_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyServer()->GenericFailureCmd( InRouteContext, InTransactionID ) );

		Proc_End:

			return hr;

		}; // HRESULT ServerService::GenericFailureCmd( const TransactionID &InTransactionID )
		// C2S: Server Started or Connected
		HRESULT ServerService::ServerConnectedC2SEvt( const EntityID &InSenderEntityID, const ServiceInformation &InClusterManagerServiceInformation, const UINT32 &InStartUpTime, const NetAddress &InPrivateAddress )
		{
 			HRESULT hr = S_SYSTEM_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyServer()->ServerConnectedC2SEvt( InRouteContext, InClusterManagerServiceInformation, InStartUpTime, InPrivateAddress ) );

		Proc_End:

			return hr;

		}; // HRESULT ServerService::ServerConnectedC2SEvt( const EntityID &InSenderEntityID, const ServiceInformation &InClusterManagerServiceInformation, const UINT32 &InStartUpTime, const NetAddress &InPrivateAddress )


	}; // namespace Svr
}; // namespace BR


