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
#include "Net/NetDef.h"
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
		HRESULT NetPolicyServer::GenericFailureCmd( const RouteContext &InRouteContext, const Context &InContext )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Server::GenericFailureCmd::BuildIMsg(pMsg, InRouteContext, InContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyServer::GenericFailureCmd( const RouteContext &InRouteContext, const Context &InContext )
		// C2S: Server Started or Connected
		HRESULT NetPolicyServer::ServerConnectedC2SEvt( const RouteContext &InRouteContext, const ServiceInformation &InClusterManagerServiceInformation, const UINT32 &InStartUpTime, const NetAddress &InPrivateAddress )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Server::ServerConnectedC2SEvt::BuildIMsg(pMsg, InRouteContext, InClusterManagerServiceInformation, InStartUpTime, InPrivateAddress));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyServer::ServerConnectedC2SEvt( const RouteContext &InRouteContext, const ServiceInformation &InClusterManagerServiceInformation, const UINT32 &InStartUpTime, const NetAddress &InPrivateAddress )


		// Cmd: 
		HRESULT NetSvrPolicyServer::GenericFailureRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Server::GenericFailureRes::BuildIMsg(pMsg, InRouteContext, InContext, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyServer::GenericFailureRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult )


	}; // namespace Policy
}; // namespace BR


