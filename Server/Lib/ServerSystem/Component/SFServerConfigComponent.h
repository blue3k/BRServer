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
#include "ServerConfig/SFServerConfig.h"




namespace SF
{

	class ServerConfigComponent : public LibraryComponent
	{
	public:
		static constexpr StringCrc64 TypeName = "ServerConfig";

	private:

		IHeap& m_Heap;

		String m_ConfigPath;

	public:

		// Constructor
		ServerConfigComponent(const char* configPath);
		~ServerConfigComponent();

		// Heap
		IHeap& GetHeap() { return m_Heap; }



		virtual const StringCrc64& GetTypeName() override { return TypeName; }

		// Initialize component
		virtual Result InitializeComponent() override;
		// Terminate component
		virtual void DeinitializeComponent() override;
	};
}

