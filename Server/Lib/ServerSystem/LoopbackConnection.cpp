////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Loopback connection
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/TypeDefs.h"
#include "Common/HRESCommon.h"
#include "LoopbackConnection.h"
#include "ServerSystem/ServerEntity.h"



namespace BR {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////////
	//
	// PlayerInfo Class
	//

	LoopbackConnection::LoopbackConnection( NetClass netClass, Svr::ServerEntity* pServerEntity )
		:m_pServerEntity(pServerEntity)
	{
		m_ConnectInfo.SetRemoteInfo(netClass, pServerEntity->GetServerID());
		SetConnectionState(IConnection::STATE_CONNECTED);
	}

	LoopbackConnection::~LoopbackConnection()
	{
	}

	// Send message to connected entity
	HRESULT LoopbackConnection::Send( Message::MessageData* &pMsg )
	{
		HRESULT hr = S_OK;

		svrChkPtr( pMsg );


		BR::Net::Connection::PrintDebugMessage( "LoopbackMsg:", pMsg );

		Message::MessageHeader* pMsgHeader = pMsg->GetMessageHeader();
		UINT uiPolicy = pMsgHeader->msgID.IDs.Policy;
		if( uiPolicy == 0 
			|| uiPolicy >= BR::POLICY_NETMAX ) // invalid policy
		{
			svrErr( E_NET_BADPACKET_NOTEXPECTED );
		}

		svrChkPtr( m_pServerEntity );
		if (m_pServerEntity->GetTaskWorker() != nullptr && m_pServerEntity->GetTaskWorker()->GetThreadID() != GetCurrentThreadId())
		{
			svrChk(m_pServerEntity->OnRecvMessage(this, pMsg));
			//svrChk(m_pServerEntity->ProcessMessage(this, pMsg));
		}
		else
		{
			svrChk(m_pServerEntity->ProcessMessage(this, pMsg));
		}

		pMsg = nullptr;

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pMsg );

		return hr;
	}



}; // namespace Svr
}; // namespace BR





