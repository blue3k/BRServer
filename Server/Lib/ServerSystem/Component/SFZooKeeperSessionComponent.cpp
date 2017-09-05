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
	ZooKeeperSessionComponent::ZooKeeperSessionComponent(const char* serverAddresses)
		: LibraryComponent("ZooKeeperSessionComponent")
		, m_MemoryManager(GetSystemMemoryManager())
		, m_ServerAddresses(GetMemoryManager(), serverAddresses)
		, m_zkInstance(GetMemoryManager())
	{
		// some components will need 
		m_zkInstance.Connect(m_ServerAddresses);// "127.0.0.1:2181");
		m_zkInstance.WaitForConnected();
	}

	ZooKeeperSessionComponent::~ZooKeeperSessionComponent()
	{
		m_zkInstance.Close();
		m_zkInstance.WaitForDisconnected();
	}


	// Initialize component
	Result ZooKeeperSessionComponent::InitializeComponent()
	{
		auto result = LibraryComponent::InitializeComponent();
		if (!result)
			return result;


		return result;
	}

	// Terminate component
	void ZooKeeperSessionComponent::DeinitializeComponent()
	{

		LibraryComponent::DeinitializeComponent();
	}

}


