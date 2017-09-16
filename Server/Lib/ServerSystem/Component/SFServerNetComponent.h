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





namespace SF
{

	class ServerNetComponent : public LibraryComponent
	{
	public:

		static constexpr FixedString TypeName = "SFServerNetComponent";

	private:

		IMemoryManager& m_MemoryManager;

	public:

		// Constructor
		ServerNetComponent();
		~ServerNetComponent();

		// MemoryManager
		IMemoryManager& GetMemoryManager() { return m_MemoryManager; }

		virtual const FixedString& GetTypeName() override { return TypeName; }

		// Initialize component
		virtual Result InitializeComponent() override;
		// Terminate component
		virtual void DeinitializeComponent() override;
	};
}

