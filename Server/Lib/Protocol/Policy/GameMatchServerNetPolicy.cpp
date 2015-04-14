////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameMatchServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Protocol/Policy/GameMatchServerIPolicy.h"
#include "Protocol/Policy/GameMatchServerNetPolicy.h"
#include "Protocol/Message/GameMatchServerMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 		// Cmd: Match
		HRESULT NetPolicyGameMatchServer::RegisterPartyMatchCmd( const Context &InContext, const RouteContext &InRouteContext, const PartyUID &InPartyUID, const UINT32 &InNumGamePlayer )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameMatchServer::RegisterPartyMatchCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPartyUID, InNumGamePlayer));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameMatchServer::RegisterPartyMatchCmd( const Context &InContext, const RouteContext &InRouteContext, const PartyUID &InPartyUID, const UINT32 &InNumGamePlayer )
		// Cmd: Match
		HRESULT NetPolicyGameMatchServer::RegisterPlayerMatchCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const UINT32 &InNumGamePlayer )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameMatchServer::RegisterPlayerMatchCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPlayerID, InNumGamePlayer));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameMatchServer::RegisterPlayerMatchCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const UINT32 &InNumGamePlayer )


		// Cmd: Match
		HRESULT NetSvrPolicyGameMatchServer::RegisterPartyMatchRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameMatchServer::RegisterPartyMatchRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameMatchServer::RegisterPartyMatchRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		// Cmd: Match
		HRESULT NetSvrPolicyGameMatchServer::RegisterPlayerMatchRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameMatchServer::RegisterPlayerMatchRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameMatchServer::RegisterPlayerMatchRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		// S2C: Game is matched
		HRESULT NetSvrPolicyGameMatchServer::GameMatchedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const GameInsID &InGameInsID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameMatchServer::GameMatchedS2CEvt::BuildIMsg(pMsg, InRouteContext, InDestPlayerID, InGameInsID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameMatchServer::GameMatchedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const GameInsID &InGameInsID )


	}; // namespace Policy
}; // namespace BR


