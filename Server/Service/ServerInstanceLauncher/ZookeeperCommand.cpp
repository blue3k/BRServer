////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : Entity server implementation
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerInstanceLanucherPCH.h"
#include "SvrConst.h"
#include "SvrTrace.h"
#include "ServerConfig/SFServerConfig.h"
#include "Util/SFTimeUtil.h"

#include "ZookeeperCommand.h"




namespace SF {
namespace ServerInstanceLauncher {


	//////////////////////////////////////////////////////////////////////////
	//
	//	CommandWatcher class
	//


	CommandWatcher::CommandWatcher(IHeap& heap)
		: ZookeeperWatcher(heap)
		, m_Heap(heap)
		, m_CommandRootPath(heap, DefaultCommandRoot)
		, m_CommandNodes(heap)
	{
	}

	CommandWatcher::~CommandWatcher()
	{

	}

	// Start watching commands
	void CommandWatcher::WatchCommands()
	{
		auto zkSession = Service::ZKSession->GetZookeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
			return;

		svrTrace(Debug, "ZK requesting command list");

		// Create command root node if not exists
		if (!zkSession->Exists(m_CommandRootPath))
			zkSession->ACreate(m_CommandRootPath, Json::Value(Json::objectValue), nullptr, 0);


		zkSession->AGetChildren(m_CommandRootPath, this);
	}

	// Try to consume command
	Result CommandWatcher::ConsumeCommand(String command)
	{
		Result hr;
		auto zkSession = Service::ZKSession->GetZookeeperSession();
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
		svrTrace(Debug, "ZKEvent:{0}", eventOut.Components.EventType);

		if (eventOut.Components.EventType == Zookeeper::EVENT_CHILD)
		{
			WatchCommands();
		}
		else if (eventOut.Components.EventType == Zookeeper::EVENT_SESSION)
		{
			if (eventOut.Components.State == Zookeeper::STATE_CONNECTED)
			{
				WatchCommands();
			}
		}
		else
		{

		}

		return ResultCode::SUCCESS;
	}

	void CommandWatcher::OnComplition(ZookeeperTask& pTask)
	{

	}

	void CommandWatcher::OnStringsComplition(StringsTask& pTask)
	{
		if (!pTask.ZKResult)
		{
			//assert(false);
			return;
		}
		DynamicArray<StringCrc64> listNewCommands(GetHeap());
		listNewCommands.reserve(static_cast<int>(m_CommandNodes.size()));

		SortedSet<StringCrc64> listAlreadyHave(GetHeap());
		for (auto itCommand : m_CommandNodes)
		{
			listAlreadyHave.Insert(itCommand);
		}

		// This should be get children list because that's the only thing have string list result
		for (auto& nodeName : pTask.ResultStrings)
		{
			StringCrc64 nodeNameCrc = Service::StringDB->AddNGetString(nodeName);
			
			if (m_CommandNodes.Find(nodeNameCrc))
			{
				listAlreadyHave.Remove(nodeNameCrc);
			}
			else
			{
				// New Service I need to add this to the list
				m_CommandNodes.insert(nodeNameCrc);
				listNewCommands.push_back(nodeNameCrc);
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
			m_OnNewCommandHandler(itNewCommand);
		}
	}


} // namespace ServerInstanceLauncher
} // namespace SF



