////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StormForge
// 
// Author : KyungKun Ko
//
// Description : Process manager
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "SvrConst.h"
#include "SvrTrace.h"
#include "ServerConfig/SFServerConfig.h"
#include "Util/TimeUtil.h"

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
//#if WINDOWS
//		m_hNTDLL = LoadLibraryA("ntdll.dll");
//		m_NtQueryInformationProcess = (NtQueryInformationProcessT)GetProcAddress((HMODULE)m_hNTDLL, "NtQueryInformationProcess");
//#endif
	}

	ProcessManager::~ProcessManager()
	{

	}




} // namespace SF







