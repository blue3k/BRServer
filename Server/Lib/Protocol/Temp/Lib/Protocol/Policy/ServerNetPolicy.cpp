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
#include "Net/Netdef.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Protocol/Policy/ServerIPolicy.h"
#include "Protocol/Policy/ServerNetPolicy.h"
#include "Protocol/Message/ServerMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 		// Cmd: 
		HRESULT NetPolicyServer::GenericFailureCmd( const Context &InContext, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Server::GenericFailureCmd::BuildIMsg(pMsg, InContext, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyServer::GenericFailureCmd( const Context &InContext, const RouteContext &InRouteContext )
		// C2S: Server Started or Connected
		HRESULT NetPolicyServer::ServerConnectedC2SEvt( const RouteContext &InRouteContext, const ServiceInformation &InClusterManagerServiceInformation, const UINT64 &InStartUpTime, const NetAddress &InPublicAddress, const NetAddress &InPrivateAddress )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Server::ServerConnectedC2SEvt::BuildIMsg(pMsg, InRouteContext, InClusterManagerServiceInformation, InStartUpTime, InPublicAddress, InPrivateAddress));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyServer::ServerConnectedC2SEvt( const RouteContext &InRouteContext, const ServiceInformation &InClusterManagerServiceInformation, const UINT64 &InStartUpTime, const NetAddress &InPublicAddress, const NetAddress &InPrivateAddress )


		// Cmd: 
		HRESULT NetSvrPolicyServer::GenericFailureRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Server::GenericFailureRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyServer::GenericFailureRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )


	}; // namespace Policy
}; // namespace BR


