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
	//	CommandWatcher class
	//


	CommandWatcher::CommandWatcher(IHeap& heap, const char* commandRoot)
		: ZooKeeperWatcher(heap)
		, m_Heap(heap)
		, m_CommandRootPath(heap, commandRoot)
		, m_CommandNodes(heap)
	{
	}

	CommandWatcher::~CommandWatcher()
	{

	}

	// Start watching commands
	void CommandWatcher::WatchCommands()
	{
		auto zkSession = Service::ZKSession->GetZooKeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
			return;

		svrTrace(Debug, "ZK requesting command list");

		zkSession->AGetChildren(m_CommandRootPath, this);
	}

	// Try to consume command
	Result CommandWatcher::ConsumeCommand(String command)
	{
		Result hr;
		auto zkSession = Service::ZKSession->GetZooKeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
			return ResultCode::NOT_INITIALIZED;

		String commandPath;
		commandPath.Format("{0}/{1}", m_CommandRootPath, command);

		svrTrace(Debug, "ZK take command {0}", command);

		svrChk(zkSession->Delete(commandPath));

	Proc_End:

		return hr;
	}

	/////////////////////////////////////////////
	//
	//	Overridable Event handling
	//

	Result CommandWatcher::OnNewEvent(const ZKEvent& eventOut)
	{
		svrTrace(Debug, "ZKEvent:{0}", eventOut.EventType);

		if (eventOut.EventType == ZooKeeper::EVENT_CHILD)
		{
			WatchCommands();
		}
		else if (eventOut.EventType == ZooKeeper::EVENT_SESSION)
		{
			if (eventOut.State == ZooKeeper::STATE_CONNECTED)
			{
				WatchCommands();
			}
		}
		else
		{

		}

		return ResultCode::SUCCESS;
	}

	void CommandWatcher::OnComlition(ZooKeeperTask& pTask)
	{

	}

	void CommandWatcher::OnStringsComlition(StringsTask& pTask)
	{
		if (!pTask.Result)
		{
			//assert(false);
			return;
		}
		DynamicArray<String> listNewCommands(GetHeap(), static_cast<int>(m_CommandNodes.size()));

		SortedSet<String> listAlreadyHave(GetHeap(), static_cast<int>(m_CommandNodes.size()));
		for (auto itCommand : m_CommandNodes)
		{
			listAlreadyHave.Insert(itCommand);
		}

		// This should be getchildren list because that's the only thing have string list result
		for (auto& nodeName : pTask.ResultStrings)
		{
			if (m_CommandNodes.Find(nodeName))
			{
				listAlreadyHave.Remove(nodeName);
			}
			else
			{
				// New Service I need to add this to the list
				m_CommandNodes.insert(nodeName);
				listNewCommands.push_back(nodeName);
			}
		}

		// If anything not in the list should be removed
		for (auto itRemove : listAlreadyHave)
		{
			if (!m_CommandNodes.Find(itRemove))
				continue;

			m_CommandNodes.Remove(itRemove);
		}

		for (auto itNewCommand : listNewCommands)
		{
			Json::Value value;
			String nodePath;
			nodePath.Format("{0}/{1}", m_CommandRootPath, itNewCOmmand);
			if (GetZKInstance()->Get(nodePath, value))
			{
				m_OnNewCommandHandler(itNewCommand, value);
			}
		}
	}


} // namespace SF







