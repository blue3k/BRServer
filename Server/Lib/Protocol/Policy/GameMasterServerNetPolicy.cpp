////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameMasterServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Protocol/Policy/GameMasterServerIPolicy.h"
#include "Protocol/Policy/GameMasterServerNetPolicy.h"
#include "Protocol/Message/GameMasterServerMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 		// C2S: Player entered
		HRESULT NetPolicyGameMasterServer::PlayerEnteredC2SEvt( const AccountID &InPlayerID, const char* InPlayerName )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameMasterServer::PlayerEnteredC2SEvt::BuildIMsg(pMsg, InPlayerID, InPlayerName));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameMasterServer::PlayerEnteredC2SEvt( const AccountID &InPlayerID, const char* InPlayerName )
		// C2S: Player leaved
		HRESULT NetPolicyGameMasterServer::PlayerLeavedC2SEvt( const AccountID &InPlayerID )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameMasterServer::PlayerLeavedC2SEvt::BuildIMsg(pMsg, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameMasterServer::PlayerLeavedC2SEvt( const AccountID &InPlayerID )




	}; // namespace Policy
}; // namespace BR


