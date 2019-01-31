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
#include "String/SFFixedString32.h"
#include "Component/SFZooKeeperSessionComponent.h"
#include "EngineObject/SFEngineObject.h"


namespace SF
{

	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ZooKeeperSessionComponent class
	//

	class ZooKeeperSessionObject : public EngineObject
	{
	private:
		String m_ServerAddresses;
		ZooKeeper& m_zkInstance;

	public:
		ZooKeeperSessionObject(const String& serverAddress, ZooKeeper& zkInstance)
			: EngineObject(&GetEngineHeap(), "ZooKeeperSessionObject")
			, m_ServerAddresses(serverAddress)
			, m_zkInstance(zkInstance)
		{
		}

		// Object task
		virtual Result OnTick(EngineTaskTick tick) override
		{
			ZooKeeper::ZKEvent zkEvent;
			while (m_zkInstance.DequeueEvent(zkEvent))
			{
				if (zkEvent.Components.EventType == ZooKeeper::EVENT_SESSION)
				{
					if (!m_zkInstance.IsConnected() && m_zkInstance.GetState() != ZooKeeper::STATE_CONNECTING)
					{
						m_zkInstance.Connect(m_ServerAddresses);
					}
				}
			}
			return ResultCode::SUCCESS;
		}
	};




	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ZooKeeperSessionComponent class
	//

	ZooKeeperSessionComponent::ZooKeeperSessionComponent(const char* serverAddresses, uint32_t zkLogLevel)
		: LibraryComponent("ZooKeeperSessionComponent")
		, m_Heap(GetSystemHeap())
		, m_zkInstance(GetSystemHeap())
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

	ZooKeeper* ZooKeeperSessionComponent::GetZooKeeperSession()
	{
		if(!m_zkInstance.IsConnected())
		{
			if(m_zkInstance.GetState() != ZooKeeper::STATE_CONNECTING)
				m_zkInstance.Connect(m_ServerAddresses);
			m_zkInstance.WaitForConnected();
		}
		return &m_zkInstance;
	}


	// Initialize component
	Result ZooKeeperSessionComponent::InitializeComponent()
	{
		auto result = LibraryComponent::InitializeComponent();
		if (!result)
			return result;

		if (!m_zkInstance.IsConnected())
			return ResultCode::FAIL;


		m_SessionObject = new(GetEngineHeap()) ZooKeeperSessionObject(m_ServerAddresses, m_zkInstance);
		m_SessionObject->SetTickGroup(EngineTaskTick::AsyncTick);

		return result;
	}

	// Terminate component
	void ZooKeeperSessionComponent::DeinitializeComponent()
	{
		m_SessionObject->Dispose();
		m_SessionObject = nullptr;

		LibraryComponent::DeinitializeComponent();
	}

}


