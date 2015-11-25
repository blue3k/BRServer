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
#include "Common/BRBaseTypes.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/BrServer.h"
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
			static_assert(Policy::IPolicyServer::ID_POLICY == ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyServer());
		}


		// Cmd: 
		HRESULT ServerService::GenericFailureCmd( const Context &InContext )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyServer()->GenericFailureCmd( InContext, InRouteContext ) );

		Proc_End:

			return hr;

		}; // HRESULT ServerService::GenericFailureCmd( const Context &InContext )
		// C2S: Server Started or Connected
		HRESULT ServerService::ServerConnectedC2SEvt( const EntityID &InSenderEntityID, const ServiceInformation &InClusterManagerServiceInformation, const UINT32 &InStartUpTime, const NetAddress &InPublicAddress, const NetAddress &InPrivateAddress )
		{
 			HRESULT hr = S_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyServer()->ServerConnectedC2SEvt( InRouteContext, InClusterManagerServiceInformation, InStartUpTime, InPublicAddress, InPrivateAddress ) );

		Proc_End:

			return hr;

		}; // HRESULT ServerService::ServerConnectedC2SEvt( const EntityID &InSenderEntityID, const ServiceInformation &InClusterManagerServiceInformation, const UINT32 &InStartUpTime, const NetAddress &InPublicAddress, const NetAddress &InPrivateAddress )


	}; // namespace Svr
}; // namespace BR


