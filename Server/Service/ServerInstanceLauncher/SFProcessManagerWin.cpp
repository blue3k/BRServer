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
#include "String/SFStringFormat.h"

#include "SFProcessManager.h"

#if SF_PLATFORM == SF_PLATFORM_WINDOWS



namespace SF {


	//////////////////////////////////////////////////////////////////////////
	//
	//	ProcessManager class
	//


	NativeHandle ProcessManager::OpenProcessFromID(intptr_t processID)
	{
		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE, FALSE, (DWORD)processID);
		return hProcess;
	}

	// Add child process 
	Result ProcessManager::StartProcess(const char* processName, const char* processPath, const Array<const char*>& args)
	{
		// It should have process name, and null termination
		if (args.size() < 1 || args[args.size() - 1] != nullptr)
		{
			return ResultCode::INVALID_ARG;
		}

		if (m_ProcesseInfos.Contains(processName))
		{
			// NOTE: maybe checking process existance would make it consolidated
			return ResultCode::ALREADY_EXIST;
		}

		STARTUPINFOA si;
		PROCESS_INFORMATION pi;
		String commandLine(GetHeap(), processPath);

		for (auto& itArg : args)
		{
			if (itArg == nullptr)
				continue;

			commandLine.AppendFormat(" {0}", itArg);
		}


		memset(&si, 0, sizeof(si));
		si.cb = sizeof(si);
		memset(&pi, 0, sizeof(pi));

		if (!CreateProcessA(processPath,   // user process name as module name
			(LPSTR)commandLine.data(),        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi)           // Pointer to PROCESS_INFORMATION structure
			)
		{
			auto result = GetLastResultCode();
			svrTrace(Error, "CreateProcess failed {0}", result);
			return result;
		}


		ProcessInfo info;
		info.Name = (const char*)processName;
		info.ProcessHandle = pi.hProcess;
		info.ProcessID = pi.dwProcessId;
		info.LatestActionTime = Util::Time.GetTimeUTCSec();

		m_ProcesseInfos.Set(info.Name, info);

		return ResultCode::SUCCESS;
	}


	Result ProcessManager::StopProcess(const char* processName)
	{
		auto itProc = m_ProcesseInfos.find(processName);
		if (itProc.ProcessHandle == 0)
		{
			svrTrace(Warning, "StopProcess nothing to stop.");
			return ResultCode::SUCCESS_FALSE;
		}

		StopProcess(itProc.ProcessHandle);

		itProc.LatestActionTime = Util::Time.GetTimeUTCSec();

		m_OnEndHandler(itProc);

		m_ProcesseInfos.Remove(itProc.Name, itProc);

		return ResultCode::SUCCESS;
	}

	Result ProcessManager::StopProcess(NativeHandle hProcess)
	{
		// TODO: replace to signal so that it can be handled gracefully
		if (!TerminateProcess(hProcess, 0))
			return GetLastResultCode();

		// Max 5 min wait
		WaitForSingleObject(hProcess, PROCESS_TERMINATE_TIMEOUT);

		CloseHandle(hProcess);

		return ResultCode::SUCCESS;
	}

	// update process running status
	void ProcessManager::UpdateProcessStatus()
	{
		StaticArray<StringCrc64, 128> removed(GetHeap());

		for (auto& itProc : m_ProcesseInfos)
		{
			auto& ProcInfo = itProc.GetValue();

			auto waitResult = WaitForSingleObject(ProcInfo.ProcessHandle, 0);
			switch (waitResult)
			{
			case WAIT_FAILED:
				svrTrace(Error, "Process handle check is failed, {0}, hr:{1}", ProcInfo.Name, GetLastResultCode());
			case WAIT_ABANDONED:
				// Error, let's consider as process termination
			case WAIT_OBJECT_0:
				CloseHandle(ProcInfo.ProcessHandle);
				ProcInfo.LatestActionTime = Util::Time.GetTimeUTCSec();

				m_OnEndHandler(ProcInfo);

				ProcInfo.ProcessHandle = 0;
				ProcInfo.ProcessID = 0;

				removed.push_back(ProcInfo.Name);
				break;
			}
			
		}

		for (auto itRemoved : removed)
		{
			ProcessInfo info;
			m_ProcesseInfos.Remove(itRemoved, info);
		}
	}


} // namespace SF


#endif


