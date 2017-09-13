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
#include "Component/SFZooKeeperSessionComponent.h"



namespace SF
{
	ZooKeeperSessionComponent::ZooKeeperSessionComponent(const char* serverAddresses, uint32_t zkLogLevel)
		: LibraryComponent("ZooKeeperSessionComponent")
		, m_Heap(GetSystemMemoryManager())
	{
		m_ServerAddresses = serverAddresses;
		// some components will need 
		m_zkInstance.Connect(m_ServerAddresses);// "127.0.0.1:2181");
		m_zkInstance.WaitForConnected();

		Service::ZKSession = this;
	}

	ZooKeeperSessionComponent::~ZooKeeperSessionComponent()
	{
		Service::ZKSession = nullptr;

		m_zkInstance.Close();
		m_zkInstance.WaitForDisconnected();
	}

	ZooKeeper& ZooKeeperSessionComponent::GetZooKeeperSession()
	{
		if(!m_zkInstance.IsConnected())
		{
			m_zkInstance.Connect(m_ServerAddresses);
			m_zkInstance.WaitForConnected();
		}
		return m_zkInstance;
	}


	// Initialize component
	Result ZooKeeperSessionComponent::InitializeComponent()
	{
		auto result = LibraryComponent::InitializeComponent();
		if (!result)
			return result;

		if (!m_zkInstance.IsConnected())
			return ResultCode::FAIL;

		return result;
	}

	// Terminate component
	void ZooKeeperSessionComponent::DeinitializeComponent()
	{

		LibraryComponent::DeinitializeComponent();
	}

}


