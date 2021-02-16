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


#include "ServerSystemPCH.h"
#include "Util/SFStringCrc64.h"
#include "Component/SFZookeeperSessionComponent.h"
#include "EngineObject/SFEngineObject.h"


namespace SF
{

	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ZookeeperSessionComponent class
	//

	class ZookeeperSessionObject : public EngineObject
	{
	private:
		String m_ServerAddresses;
		Zookeeper& m_zkInstance;

	public:
		ZookeeperSessionObject(IHeap& heap, const String& serverAddress, Zookeeper* zkInstance)
			: EngineObject(&heap, "ZookeeperSessionObject")
			, m_ServerAddresses(serverAddress)
			, m_zkInstance(*zkInstance)
		{
			Assert(zkInstance != nullptr);
		}

		// Object task
		virtual Result OnTick(EngineTaskTick tick) override
		{
			Zookeeper::ZKEvent zkEvent;
			while (m_zkInstance.DequeueEvent(zkEvent))
			{
				if (zkEvent.Components.EventType == Zookeeper::EVENT_SESSION)
				{
					if (!m_zkInstance.IsConnected() && m_zkInstance.GetState() != Zookeeper::STATE_CONNECTING)
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
	//	ZookeeperSessionComponent class
	//

	ZookeeperSessionComponent::ZookeeperSessionComponent(const char* serverAddresses, uint32_t zkLogLevel)
		: LibraryComponent("ZookeeperSessionComponent")
		, m_Heap(GetSystemHeap())
		, m_zkInstance(GetSystemHeap())
	{
		m_ServerAddresses = serverAddresses;
		// some components need connected Zookeeper
		m_zkInstance.Connect(m_ServerAddresses);// "127.0.0.1:2181");
		m_zkInstance.WaitForConnected();

		Service::ZKSession = this;
	}

	ZookeeperSessionComponent::~ZookeeperSessionComponent()
	{
		Service::ZKSession = nullptr;

		m_zkInstance.Close();
		m_zkInstance.WaitForDisconnected();
	}

	Zookeeper* ZookeeperSessionComponent::GetZookeeperSession()
	{
		if(!m_zkInstance.IsConnected())
		{
			if(m_zkInstance.GetState() != Zookeeper::STATE_CONNECTING)
				m_zkInstance.Connect(m_ServerAddresses);
			m_zkInstance.WaitForConnected();
		}
		return &m_zkInstance;
	}


	// Initialize component
	Result ZookeeperSessionComponent::InitializeComponent()
	{
		auto result = LibraryComponent::InitializeComponent();
		if (!result)
			return result;

		if (!m_zkInstance.IsConnected())
			return ResultCode::FAIL;

		m_SessionObject = NewObject<ZookeeperSessionObject>(GetEngineHeap(), m_ServerAddresses, &m_zkInstance);
		m_SessionObject->SetTickGroup(EngineTaskTick::AsyncTick);

		return result;
	}

	// Terminate component
	void ZookeeperSessionComponent::DeinitializeComponent()
	{
		m_SessionObject->Dispose();
		m_SessionObject = nullptr;

		LibraryComponent::DeinitializeComponent();
	}

}


