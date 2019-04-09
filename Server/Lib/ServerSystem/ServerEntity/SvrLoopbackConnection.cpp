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
#include "SFTypedefs.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "SvrLoopbackConnection.h"
#include "ServerEntity/ServerEntity.h"
#include "SvrTrace.h"



namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////////
	//
	// PlayerInfo Class
	//

	LoopbackConnection::LoopbackConnection( NetClass netClass, Svr::ServerEntity* pServerEntity )
		: m_pServerEntity(pServerEntity)
	{
		SetRemoteInfo(netClass, pServerEntity->GetServerID());
		SetConnectionState(Net::ConnectionState::CONNECTED);
	}

	LoopbackConnection::~LoopbackConnection()
	{
	}

	// Send message to connected entity
	Result LoopbackConnection::Send( MessageDataPtr &pMsg )
	{
		Result hr = ResultCode::SUCCESS;

		svrChkPtr( pMsg );

		Protocol::PrintDebugMessage("LoopBackSend", pMsg);

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


		return hr;
	}



}; // namespace Svr
}; // namespace SF






