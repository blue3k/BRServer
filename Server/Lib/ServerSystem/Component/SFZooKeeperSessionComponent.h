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
#include "String/SFFixedString32.h"
#include "String/SFString.h"
#include "Object/SFLibraryComponent.h"

#include "Service/ServerService.h"

#include "ZooKeeper/SFZooKeeper.h"




namespace SF
{
	

	class ZooKeeperSessionComponent : public LibraryComponent, public ZooKeeperSessionService
	{
	public:

		static constexpr StringCrc64 TypeName = "ZooKeeperSessionComponent";

	private:

		IHeap& m_Heap;
		String m_ServerAddresses;
		ZooKeeper m_zkInstance;

		SharedPointerT<class ZooKeeperSessionObject> m_SessionObject;

	public:

		// Constructor
		ZooKeeperSessionComponent(const char* serverAddresses, uint32_t zkLogLevel);
		~ZooKeeperSessionComponent();

		// Heap
		IHeap& GetHeap() { return m_Heap; }


		virtual ZooKeeper* GetZooKeeperSession() override;

		virtual const StringCrc64& GetTypeName() override { return TypeName; }

		// Initialize component
		virtual Result InitializeComponent() override;
		// Terminate component
		virtual void DeinitializeComponent() override;
	};
}

