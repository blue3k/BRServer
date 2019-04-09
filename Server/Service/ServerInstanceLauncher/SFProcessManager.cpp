////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StormForge
// 
// Author : KyungKun Ko
//
// Description : Process manager
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerInstanceLanucherPCH.h"
#include "SvrConst.h"
#include "SvrTrace.h"
#include "ServerConfig/SFServerConfig.h"
#include "Util/SFTimeUtil.h"

#include "SFProcessManager.h"




namespace SF {


	//////////////////////////////////////////////////////////////////////////
	//
	//	ProcessManager class
	//


	ProcessManager::ProcessManager(IHeap& heap)
		: m_Heap(heap)
		, m_ProcesseInfos(heap)
	{
	}

	ProcessManager::~ProcessManager()
	{

	}


	// Manage processed if they are not managed by 
	void ProcessManager::ManageProcesses(const char* processName, intptr_t processID)
	{
		ProcessInfo info;
		info.Name = (const char*)processName;
		info.ProcessHandle = OpenProcessFromID(processID);
		info.ProcessID = processID;
		info.LatestActionTime = Util::Time.GetTimeUTCSec();

		m_ProcesseInfos.Set(info.Name, info);
	}



} // namespace SF







