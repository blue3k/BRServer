////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves.
// 
// Author : KyungKun Ko
//
// Description : Network connection task
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/HRESNet.h"
#include "Net/NetConst.h"
#include "Common/Thread.h"
#include "Common/TimeUtil.h"
#include "Net/Connection.h"
#include "Net/NetTrace.h"
#include "Net/NetConnectionTask.h"
#include "Protocol/ProtocolVer.h"
#include "Net/ConnectionManager.h"



BR_MEMORYPOOL_IMPLEMENT(Net::ConnectionTask)


namespace BR {
namespace Net {
	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Connection TickTask
	//

	// Connection task
	ConnectionTask::ConnectionTask()
		: m_pConnection(nullptr)
		, m_pConnectionManager(nullptr)
	{
	}

	ConnectionTask::~ConnectionTask()
	{
	}

	// Set connection to manage
	void ConnectionTask::SetConnection( ConnectionManager *pConMgr, Connection *pConnection )
	{
		m_pConnectionManager = pConMgr;
		m_pConnection = pConnection;
		if (pConnection != nullptr)
		{
			SetTaskID(pConnection->GetCID());
		}
	}

	
	// Get connection
	Connection* ConnectionTask::GetConnection()
	{
		return m_pConnection;
	}

	// Get connection manager
	ConnectionManager* ConnectionTask::GetConnectionManager()
	{
		return m_pConnectionManager;
	}


	// Run the task
	HRESULT ConnectionTask::TickUpdate(Svr::TimerAction *pAction)
	{
		if( m_pConnection != nullptr 
			&& (m_pConnection->GetConnectionState() == IConnection::STATE_CONNECTING
			|| m_pConnection->GetConnectionState() == IConnection::STATE_CONNECTED
				|| m_pConnection->GetConnectionState() == IConnection::STATE_DISCONNECTING))
		{
			m_pConnection->UpdateNetCtrl();
		}

		return S_OK;
	}


	

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Connection TickTask manager
	//

	ConnectionTaskManager::ConnectionTaskManager()
	{
	}

	ConnectionTaskManager::~ConnectionTaskManager()
	{
	}

	// Create TickTask
	HRESULT ConnectionTaskManager::CreateTask( ConnectionTask* &pTask )
	{
		HRESULT hr = S_OK;
		ConnectionTask *pNewTask = nullptr;

		netMem( pNewTask = new ConnectionTask );
		pTask = pNewTask;

	Proc_End:

		return hr;
	}



} // namespace Net
} // namespace BR

