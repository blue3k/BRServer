///////////////////////////////////////////////////////////
////////////////////
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
//	TickTask exection Group class -  interface for task operation
//


// Get/Set Group Idx
SysUInt TaskWorker::GetGroupID()
{
	return m_GroupID;
}

void TaskWorker::SetGroupID( SysUInt ulGroupID )
{
	m_GroupID = ulGroupID;
}

// Get/Set Group Idx
SysUInt TaskWorker::GetGroupWorkLoad()
{
	return m_GroupWorkLoad + m_GroupWorkLoadDiff;
}

// Set base loop delay
void TaskWorker::SetBaseLoopInterval(DurationMS dwLoopInterval )
{
	m_ulLoopInterval = dwLoopInterval;
}

DurationMS TaskWorker::GetBaseLoopInterval()
{
	return m_ulLoopInterval;
}




////////////////////////////////////////////////////////////////////////////////////////
//
//	TickTask Manager base class -  interface for task manager
//



size_t TaskManager::GetWorkGroupCount()
{
	return m_TaskGroups.size();
}




