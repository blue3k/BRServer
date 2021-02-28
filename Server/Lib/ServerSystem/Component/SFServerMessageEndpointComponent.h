////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) StormForge
// 
// Author : KyungKun Ko
//
// Description : Server message endpoint
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
#include "StreamDB/SFStreamDB.h"



namespace SF
{
	
	class ServerMessageEndpointComponent : public LibraryComponent
	{
	public:

		static constexpr StringCrc64 TypeName = "ServerMessageEndpointComponent";

	private:

		IHeap& m_Heap;

		SharedPointerT<StreamDBConsumer> m_ServerEndpoint;

		Thread* m_MessageConsumeWorker{};

	public:

		// Constructor
		ServerMessageEndpointComponent();
		~ServerMessageEndpointComponent();

		// Heap
		IHeap& GetHeap() { return m_Heap; }

		virtual const StringCrc64& GetTypeName() override { return TypeName; }

		// Initialize component
		virtual Result InitializeComponent() override;
		// Terminate component
		virtual void DeinitializeComponent() override;
	};
}

