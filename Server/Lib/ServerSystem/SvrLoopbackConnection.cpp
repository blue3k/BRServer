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
#include "Common/Typedefs.h"
#include "Common/ResultCode/BRResultCodeCommon.h"
#include "SvrLoopbackConnection.h"
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
	Result LoopbackConnection::Send( Message::MessageData* &pMsg )
	{
		Result hr = ResultCode::SUCCESS;
		Message::MessageHeader* pMsgHeader = nullptr;
		UINT uiPolicy = 0;

		svrChkPtr( pMsg );


		Net::Connection::PrintDebugMessage( "LoopbackMsg:", pMsg );

		pMsgHeader = pMsg->GetMessageHeader();
		uiPolicy = pMsgHeader->msgID.IDs.Policy;
		if( uiPolicy == 0 
			|| uiPolicy >= BR::POLICY_NETMAX ) // invalid policy
		{
			svrErr( ResultCode::E_NET_BADPACKET_NOTEXPECTED );
		}

		svrChkPtr( m_pServerEntity );
		if (m_pServerEntity->GetTaskWorker() != nullptr && m_pServerEntity->GetTaskWorker()->GetThreadID() != ThisThread::GetThreadID())
		{
			svrChk(m_pServerEntity->OnRecvMessage(this, pMsg));
		}
		else
		{
			svrChk(m_pServerEntity->ProcessMessage(m_pServerEntity, this, pMsg));
		}

		pMsg = nullptr;

	Proc_End:

		if( !(hr) )
			Util::SafeRelease( pMsg );

		return hr;
	}



}; // namespace Svr
}; // namespace BR






