////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameInstanceManager Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Protocol/Policy/GameInstanceManagerIPolicy.h"
#include "Protocol/Policy/GameInstanceManagerNetPolicy.h"
#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 		// Cmd: Create a game instance
		HRESULT NetPolicyGameInstanceManager::CreateGameCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const UINT16 &InNumberOfBotPlayer, const UINT16 &InMaxPlayer )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstanceManager::CreateGameCmd::BuildIMsg(pMsg, InContext, InRouteContext, InRouteHopCount, InNumberOfBotPlayer, InMaxPlayer));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameInstanceManager::CreateGameCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const UINT16 &InNumberOfBotPlayer, const UINT16 &InMaxPlayer )
		// C2S: Game instance notify of deletion
		HRESULT NetPolicyGameInstanceManager::GameDeletedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstanceManager::GameDeletedC2SEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameInstanceManager::GameDeletedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount )


		// Cmd: Create a game instance
		HRESULT NetSvrPolicyGameInstanceManager::CreateGameRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstanceManager::CreateGameRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstanceManager::CreateGameRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )


	}; // namespace Policy
}; // namespace BR


