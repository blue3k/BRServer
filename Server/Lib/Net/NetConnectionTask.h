////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves.
// 
// Author : KyungKun Ko
//
// Description : Net Connection task manager
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Net/NetDef.h"
#include "Net/NetCtrl.h"
#include "ServerSystem/TaskManager.h"
#include "ServerSystem/Task.h"
#include "Common/SharedObj.h"
#include "Common/MemoryPool.h"


namespace BR {
namespace Net {

	class ConnectionManager;

	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Connection TickTask
	//

	class ConnectionTask : public Svr::TickTask, public MemoryPoolObject<ConnectionTask>
	{
	private:
		// Connection to manage
		Connection*		m_pConnection;

		// ConnectionManager
		ConnectionManager *m_pConnectionManager;


	public:
		// Connection task
		ConnectionTask();
		~ConnectionTask();

		// Set connection to manage
		void SetConnection( ConnectionManager *pConMgr, Connection *pConnection );

		// Get connection
		Connection* GetConnection();

		// Get connection manager
		ConnectionManager* GetConnectionManager();

		// Run the task
		virtual Result TickUpdate(Svr::TimerAction* pAction);
	};



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Connection TickTask manager
	//

	class ConnectionTaskManager : public Svr::TaskManager
	{
	public :

	private:

	public:
		ConnectionTaskManager();
		virtual ~ConnectionTaskManager();

		
		// Create TickTask
		virtual Result CreateTask( ConnectionTask* &pTask );

	};



} // namespace Net
} // namespace BR



