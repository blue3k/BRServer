////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromForge
// 
// Author : KyungKun Ko
//
// Description : Process manager
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Server/BrServer.h"

#include "ZooKeeper/SFZooKeeper.h"


namespace SF {



	//////////////////////////////////////////////////////////////////////////
	//
	//	Child process manager class
	//		- This should be singleton otherwise it will mis-function on Linux based machine
	//

	class ProcessManager
	{
	public:

		const uint PROCESS_TERMINATE_TIMEOUT = 5 * 60 * 1000; // Max 5 min wait

		struct ProcessInfo
		{
			FixedString Name;

			NativeHandle ProcessHandle;
			intptr_t ProcessID;

			TimeStampSec LatestActionTime; // Time stamp for started, or stopped
		};

		typedef std::function<void(const ProcessInfo&)> ProcessEndHandler;

	private:
		IHeap& m_Heap;

		// Child Process list managed by this class
		SortedArray<FixedString, ProcessInfo> m_ProcesseInfos;

		ProcessEndHandler m_OnEndHandler;

	public:

		ProcessManager(IHeap& heap);
		~ProcessManager();

		IHeap& GetHeap() { return m_Heap; }

		void SetProcessEndHandled(const ProcessEndHandler& handler) { m_OnEndHandler = handler; }

		// Add child process 
		Result StartProcess(const char* processName, const char* processPath, const Array<const char*>& args);
		Result StopProcess(const char* processName);

		// update process running status
		void UpdateProcessStatus();
	};



	template<> inline ProcessManager::ProcessInfo DefaultValue<ProcessManager::ProcessInfo>()
	{
		static ProcessManager::ProcessInfo Dummy = { nullptr, };
		return Dummy;
	}


} // namespace SF







