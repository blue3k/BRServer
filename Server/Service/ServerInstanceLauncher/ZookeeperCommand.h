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

#include "Zookeeper/SFZookeeper.h"


namespace SF {
namespace ServerInstanceLauncher {



	//////////////////////////////////////////////////////////////////////////
	//
	//	Server Instance Launcher server class
	//


	class CommandWatcher : public ZookeeperWatcher
	{
	public:

		const char* DefaultCommandRoot = "/BRServerCommand";

	private:
		IHeap& m_Heap;
		String m_CommandRootPath;
		SortedSet<StringCrc64> m_CommandNodes;

		std::function<void(const StringCrc64&)> m_OnNewCommandHandler;

	public:

		CommandWatcher(IHeap& heap);
		~CommandWatcher();

		IHeap& GetHeap() { return m_Heap; }
		const String& GetRootPath() { return m_CommandRootPath; }


		// Start watching commands
		void WatchCommands();

		// Try to Consume command
		Result ConsumeCommand(String command);


		void SetNewCommandHandler(std::function<void(const StringCrc64&)> handler) { m_OnNewCommandHandler = handler; }


		/////////////////////////////////////////////
		//
		//	Overridable Event handling
		//

		virtual Result OnNewEvent(const ZKEvent& eventOut) override;
		virtual void OnComplition(ZookeeperTask& pTask) override;
		//virtual void OnStatComplition(StatTask& pTask) override;
		//virtual void OnDataComplition(DataTask& pTask) override;
		virtual void OnStringsComplition(StringsTask& pTask) override;
		//virtual void OnStringsStatComplition(StringsStatTask& pTask) override;
		//virtual void OnStringComplition(StringTask& pTask) override;
		//virtual void OnACLComplition(ACLTask& pTask) override;
	};




} // namespace ServerInstanceLauncher
} // namespace SF







