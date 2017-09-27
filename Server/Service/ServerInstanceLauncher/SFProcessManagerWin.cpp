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

#if WINDOWS



namespace SF {


	//////////////////////////////////////////////////////////////////////////
	//
	//	ProcessManager class
	//


	// Add child process 
	Result ProcessManager::StartProcess(const char* processName, const char* processPath, const Array<const char*>& args)
	{
		// It should have process name, and null termination
		if (args.size() < 1 || args[args.size() - 1] != nullptr)
		{
			return ResultCode::INVALID_ARG;
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

	// Manage processed if they are not managed by 
	void ProcessManager::ManageProcesses(const char* prefix)
	{
		//if (m_NtQueryInformationProcess == nullptr)
		//{
		//	svrTrace(Error, "OS doesn't support process detailed information query. Recovering process list doesn't supported!");
		//	return;
		//}

		DWORD processIDs[2048], filledSize;

		if (!EnumProcesses(processIDs, sizeof(processIDs), &filledSize))
		{
			return;
		}

		auto processCount = filledSize / sizeof(DWORD);

		for (auto iProc = 0; iProc < processCount; iProc++)
		{
			if (processIDs[iProc] == 0)
				continue;

			char processName[256] = "";
			//HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processIDs[iProc]);
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processIDs[iProc]);
			if (hProcess == nullptr)
				continue;

			HMODULE hMod;
			DWORD cbNeeded;
			if (!EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
			{
				CloseHandle(hProcess);
				continue;
			}

			GetModuleBaseNameA(hProcess, hMod, processName, (int)(sizeof(processName) / sizeof(char)));

			auto compareSize = (int)strlen(prefix);
			if (!StrUtil::StringCmp(processName, compareSize, prefix, compareSize))
			{
				CloseHandle(hProcess);
				continue;
			}

			compareSize = (int)strlen(Util::GetModuleName());
			if (StrUtil::StringCmp(processName, compareSize, Util::GetModuleName(), compareSize))
				continue;

			// This method doesn't support process started programmatically
			//PROCESS_BASIC_INFORMATION basicInfo;
			//memset(&basicInfo, 0, sizeof(basicInfo));

			//m_NtQueryInformationProcess(hProcess, ProcessBasicInformation, &basicInfo, sizeof(basicInfo), nullptr);

			//if (basicInfo.PebBaseAddress != nullptr)
			//{
			//	auto& pbe = *basicInfo.PebBaseAddress;
			//	svrTrace(Info, "{0}", pbe.ProcessParameters->CommandLine.Buffer);
			//}

			// Kill any process started already
			Result hr = StopProcess(hProcess);
			if (hr)
			{
				svrTrace(Info, "{0} is closed", processName);
			}
			else
			{
				svrTrace(Info, "Failed to close {0}, hr:{1}", processName, hr);
			}

			//CloseHandle(hProcess);
		}
	}

	Result ProcessManager::StopProcess(const char* processName)
	{
		auto itProc = m_ProcesseInfos.find(processName);
		if (itProc.ProcessHandle == 0)
			return ResultCode::INVALID_ARG;

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
		StaticArray<FixedString, 128> removed(GetHeap());

		for (auto& itProc : m_ProcesseInfos)
		{
			auto waitResult = WaitForSingleObject(itProc.ProcessHandle, 0);
			switch (waitResult)
			{
			case WAIT_ABANDONED:
			case WAIT_FAILED:
				// Error, let's consider as process termination
			case WAIT_OBJECT_0:
				CloseHandle(itProc.ProcessHandle);
				itProc.LatestActionTime = Util::Time.GetTimeUTCSec();

				m_OnEndHandler(itProc);

				itProc.ProcessHandle = 0;
				itProc.ProcessID = 0;

				removed.push_back(itProc.Name);
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


