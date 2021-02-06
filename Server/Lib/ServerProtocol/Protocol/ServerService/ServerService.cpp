////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : Server Server service
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFMessageEndpoint.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Protocol/ServerService/ServerService.h"
#include "SvrTrace.h"



namespace SF
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
		svrCheck(NetPolicyServer(GetTargetEndpoint()).GenericFailureCmd( InRouteContext, InTransactionID ) );

		return hr;

	}; // Result ServerService::GenericFailureCmd( const uint64_t &InTransactionID )
	// C2S: Server Started or Connected
	Result ServerService::ServerConnectedC2SEvt( const EntityID &InSenderEntityID, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
		svrCheck(NetPolicyServer(GetTargetEndpoint()).ServerConnectedC2SEvt( InRouteContext, InStartUpTime, InPrivateAddress ) );

		return hr;

	}; // Result ServerService::ServerConnectedC2SEvt( const EntityID &InSenderEntityID, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress )


}; // namespace SF


