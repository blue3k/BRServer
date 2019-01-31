////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server TickTask manager
//	
//
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////
//
//	TickTask class -  interface for task operation
//


// Get TickTask ID
uint32_t TickTask::GetTaskID() const
{
	return m_TaskID;
}


// Get TickTask group
SysUInt TickTask::GetTaskGroupID() const
{
	return m_TaskGroupIdx;
}

void TickTask::SetTaskGroupID( SysUInt uiTaskGroupIdx )
{
	m_TaskGroupIdx = uiTaskGroupIdx;
}


// Get TickTask group
inline TickTaskManager* TickTask::GetTaskManager()
{
	return m_pTaskManager;
}

inline void TickTask::SetTaskManager(TickTaskManager *pManager)
{
	Assert(m_pTaskManager == nullptr);
	m_pTaskManager = pManager;
}

inline TaskWorker* TickTask::GetTaskWorker()
{
	return m_pTaskWorker;
}


// Get task Load
SysUInt TickTask::GetTaskLoad() const
{
	return m_TaskLoad;
}

// Get task Load
void TickTask::SetTaskLoad( SysUInt workLoad )
{
	m_TaskLoad = workLoad;
}


