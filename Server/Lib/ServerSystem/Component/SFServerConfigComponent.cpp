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


#include "stdafx.h"
#include "String/FixedString32.h"
#include "SFEngine.h"
#include "Service/ServerService.h"
#include "Component/SFServerConfigComponent.h"
#include "Component/SFZooKeeperSessionComponent.h"
#include "ServerConfig/SFServerConfigZooKeeper.h"
#include "SvrTrace.h"



namespace SF
{

	ServerConfigComponent::ServerConfigComponent(const char* configPath)
		: LibraryComponent(TypeName)
		, m_MemoryManager(GetSystemMemoryManager())
		, m_ConfigPath(GetMemoryManager(), configPath)
	{
		AddDependency<ZooKeeperSessionComponent>();
	}

	ServerConfigComponent::~ServerConfigComponent()
	{
	}


	// Initialize component
	Result ServerConfigComponent::InitializeComponent()
	{
		auto result = LibraryComponent::InitializeComponent();
		if (!result)
			return result;



		ServerConfigZooKeeper zkConfigLoader(**Service::ServerConfig, Service::ZKSession->GetZooKeeperSession());
		result = zkConfigLoader.LoadConfig(m_ConfigPath);
		if (!result)
		{
			svrTrace(Info, "Failed to load server config hr:{0}", result);
		}

		return result;
	}

	// Terminate component
	void ServerConfigComponent::DeinitializeComponent()
	{

		LibraryComponent::DeinitializeComponent();
	}

}


