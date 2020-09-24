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

#include "Zookeeper/SFZookeeper.h"

#if SF_PLATFORM == SF_PLATFORM_WINDOWS
#include <Winternl.h>
#endif


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
			StringCrc64 Name;

			NativeHandle ProcessHandle;
			intptr_t ProcessID;

			UTCTimeStampSec LatestActionTime; // Time stamp for started, or stopped
		};

		typedef std::function<void(const ProcessInfo&)> ProcessEndHandler;

	private:
		IHeap& m_Heap;

		// Child Process list managed by this class
		SortedArray<StringCrc64, ProcessInfo> m_ProcesseInfos;

		ProcessEndHandler m_OnEndHandler = [](const ProcessInfo&) {};



	public:

		ProcessManager(IHeap& heap);
		~ProcessManager();

		IHeap& GetHeap() { return m_Heap; }

		void SetProcessEndHandled(const ProcessEndHandler& handler) { m_OnEndHandler = handler; }

		// Manage processed if they are not managed by 
		void ManageProcesses(const char* processName, intptr_t processID);

		NativeHandle OpenProcessFromID(intptr_t processID);

		// Add child process 
		Result StartProcess(const char* processName, const char* processPath, const Array<const char*>& args);
		Result StopProcess(const char* processName);

		// Stop process directly. doesn't care the process list
		Result StopProcess(NativeHandle hProcess);

		// update process running status
		void UpdateProcessStatus();
	};



} // namespace SF







