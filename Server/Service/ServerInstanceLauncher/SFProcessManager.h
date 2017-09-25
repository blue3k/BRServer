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
	//	Server Instance Launcher server class
	//


	class ProcessManager
	{
	public:


	private:
		IHeap& m_Heap;
		String m_CommandRootPath;
		SortedSet<String> m_CommandNodes;

		std::function<void(const String&, const Json::Value&)> m_OnNewCommandHandler;

	public:

		ProcessManager(IHeap& heap, const char* commandRoot);
		~ProcessManager();

		IHeap& GetHeap() { return m_Heap; }

		// Start watching commands
		void WatchCommands();

		// Try to Consume command
		Result ConsumeCommand(String command);


		void SetNewCommandHandler(std::function<void(const String&,const Json::Value&)> handler) { m_OnNewCommandHandler = handler; }


	};




} // namespace SF







