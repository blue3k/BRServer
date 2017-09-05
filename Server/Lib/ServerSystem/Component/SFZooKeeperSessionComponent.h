////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StormForge
// 
// Author : KyungKun Ko
//
// Description : Server Config
//	
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Types/SFEngineTypedefs.h"
#include "String/FixedString32.h"
#include "String/SFString.h"
#include "Object/LibraryComponent.h"

#include "Service/ServerService.h"

#include "ZooKeeper/SFZooKeeper.h"
#include "ServerConfig/SFServerConfig.h"




namespace SF
{

	class ZooKeeperSessionComponent : public LibraryComponent
	{
	public:

	private:

		IMemoryManager& m_MemoryManager;
		String m_ServerAddresses;
		ZooKeeper m_zkInstance;

	public:

		// Constructor
		ZooKeeperSessionComponent(const char* serverAddresses);
		~ZooKeeperSessionComponent();

		// MemoryManager
		IMemoryManager& GetMemoryManager() { return m_MemoryManager; }

		ZooKeeper& GetZooKeeper() { return m_zkInstance; }


		static constexpr FixedString TypeName = "ServerConfig";

		virtual const FixedString& GetTypeName() override { return TypeName; }

		// Initialize component
		virtual Result InitializeComponent() override;
		// Terminate component
		virtual void DeinitializeComponent() override;
	};
}

