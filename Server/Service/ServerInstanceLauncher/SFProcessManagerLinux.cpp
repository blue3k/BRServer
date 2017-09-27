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

#if LINUX


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

		pid_t child_pid;
		/* Duplicate this process. */
		child_pid = fork();
		if (child_pid != 0)
		{
			/* This is the parent process. */
			ProcessInfo info;
			info.Name = processName;
			info.ProcessHandle = nullptr;
			info.ProcessID = child_pid;
			info.LatestActionTime = Util::Time.GetTimeUTCSec();
			m_ProcesseInfos.Set(info.Name, info);
		}
		else {
			/* Now execute PROGRAM, searching for it in the path. */
			execvp(processPath, args.data());
			/* The execvp function returns only if an error occurs. */
			svrTrace(Error, “execvp is failed”);
			abort();
		}

		return ResultCode::SUCCESS;
	}

	Result ProcessManager::StopProcess(const char* processName)
	{
		auto itProc = m_ProcesseInfos.find(processName);
		if (itProc.ProcessID == 0)
			return ResultCode::INVALID_ARG;

		kill(itProc.ProcessID, SIGTERM);

		waitpid(itProc.ProcessID, &status, 0);


		itProc.LatestActionTime = Util::Time.GetTimeUTCSec();

		m_OnEndHandler(itProc);

		m_ProcesseInfos.Remove(itProc.Name);
	}

	// update process running status
	void ProcessManager::UpdateProcessStatus()
	{
		StaticArray<FixedString, 128> removed(GetHeap());

		while(1)
		{
			int status = 0;
			auto waitResult = waitpid(-1, &status, 0);
			if (waitResult <= 0)
				break;

			for (auto& itProc : m_ProcesseInfos)
			{
				if (itProc.ProcessID != waitResult)
					continue;

				itProc.LatestActionTime = Util::Time.GetTimeUTCSec();

				m_OnEndHandler(itProc);

				itProc.ProcessHandle = 0;
				itProc.ProcessID = 0;

				removed.push_back(itProc.Name);
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



