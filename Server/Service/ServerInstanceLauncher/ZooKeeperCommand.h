////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromForge
// 
// Author : KyungKun Ko
//
// Description : Commands
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Server/BrServer.h"

#include "ZooKeeper/SFZooKeeper.h"


namespace SF {
namespace ServerInstanceLauncher {



	//////////////////////////////////////////////////////////////////////////
	//
	//	Server Instance Launcher server class
	//


	class CommandWatcher : public ZooKeeperWatcher
	{
	public:


	private:
		IHeap& m_Heap;
		String m_CommandRootPath;
		SortedSet<String> m_CommandNodes;

		std::function<void(const String&)> m_OnNewCommandHandler;

	public:

		CommandWatcher(IHeap& heap, const char* commandRoot);
		~CommandWatcher();

		IHeap& GetHeap() { return m_Heap; }
		const String& GetRootPath() { return m_CommandRootPath; }


		// Start watching commands
		void WatchCommands();

		// Try to Consume command
		Result ConsumeCommand(String command);


		void SetNewCommandHandler(std::function<void(const String&)> handler) { m_OnNewCommandHandler = handler; }


		/////////////////////////////////////////////
		//
		//	Overridable Event handling
		//

		virtual Result OnNewEvent(const ZKEvent& eventOut) override;
		virtual void OnComlition(ZooKeeperTask& pTask) override;
		//virtual void OnStatComlition(StatTask& pTask) override;
		//virtual void OnDataComlition(DataTask& pTask) override;
		virtual void OnStringsComlition(StringsTask& pTask) override;
		//virtual void OnStringsStatComlition(StringsStatTask& pTask) override;
		//virtual void OnStringComlition(StringTask& pTask) override;
		//virtual void OnACLComlition(ACLTask& pTask) override;
	};




} // namespace ServerInstanceLauncher
} // namespace SF







