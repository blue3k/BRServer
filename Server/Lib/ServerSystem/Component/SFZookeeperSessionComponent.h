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
#include "Util/SFStringCrc32.h"
#include "Util/SFString.h"
#include "Component/SFLibraryComponent.h"

#include "Service/ServerService.h"

#include "Zookeeper/SFZookeeper.h"




namespace SF
{
	

	class ZookeeperSessionComponent : public LibraryComponent, public ZookeeperSessionService
	{
	public:

		static constexpr StringCrc64 TypeName = "ZookeeperSessionComponent";

	private:

		IHeap& m_Heap;
		String m_ServerAddresses;
		Zookeeper m_zkInstance;

		SharedPointerT<class ZookeeperSessionObject> m_SessionObject;

	public:

		// Constructor
		ZookeeperSessionComponent(const char* serverAddresses, uint32_t zkLogLevel = 4); // 4: ZOO_LOG_LEVEL_DEBUG
		~ZookeeperSessionComponent();

		// Heap
		IHeap& GetHeap() { return m_Heap; }


		virtual Zookeeper* GetZookeeperSession() override;

		virtual const StringCrc64& GetTypeName() override { return TypeName; }

		// Initialize component
		virtual Result InitializeComponent() override;
		// Terminate component
		virtual void DeinitializeComponent() override;
	};
}

