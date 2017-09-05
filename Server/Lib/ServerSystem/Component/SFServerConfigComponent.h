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

	class ServerConfigComponent : public LibraryComponent
	{
	public:

	private:

		IMemoryManager& m_MemoryManager;

		String m_ConfigPath;

	public:

		// Constructor
		ServerConfigComponent(const char* configPath);
		~ServerConfigComponent();

		// MemoryManager
		IMemoryManager& GetMemoryManager() { return m_MemoryManager; }


		static constexpr FixedString TypeName = "ServerConfig";

		virtual const FixedString& GetTypeName() override { return TypeName; }

		// Initialize component
		virtual Result InitializeComponent() override;
		// Terminate component
		virtual void DeinitializeComponent() override;
	};
}

