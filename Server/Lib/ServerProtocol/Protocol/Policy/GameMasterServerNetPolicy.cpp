////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
// 
// Author : Generated
// 
// Description : GameMasterServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Net/NetDef.h"
#include "Net/Message.h"
#include "Net/Connection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/GameMasterServerNetNetPolicy.h"
#include "Protocol/Message/GameMasterServerMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 		// C2S: Player entered
		Result NetPolicyGameMasterServer::PlayerEnteredC2SEvt( const AccountID &InPlayerID, const char* InPlayerName )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameMasterServer::PlayerEnteredC2SEvt::Create(m_pConnection->GetMemoryManager(), InPlayerID, InPlayerName);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameMasterServer::PlayerEnteredC2SEvt( const AccountID &InPlayerID, const char* InPlayerName )
		// C2S: Player leaved
		Result NetPolicyGameMasterServer::PlayerLeavedC2SEvt( const AccountID &InPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameMasterServer::PlayerLeavedC2SEvt::Create(m_pConnection->GetMemoryManager(), InPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameMasterServer::PlayerLeavedC2SEvt( const AccountID &InPlayerID )




	}; // namespace Policy
}; // namespace SF


