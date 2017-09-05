////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 MadK
// 
// Author : KyungKun Ko
//
// Description : Base type ToString implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFAssert.h"
#include "Object/SharedPointer.h"
#include "EngineObject/EngineObject.h"
#include "ServerLog/SvrLog.h"
#include "ServerLog/SvrLog_Internal.h"
#include "ServerLog/SvrLogComponent.h"
#include "Util/Utility.h"



namespace SF
{
	class ServerLogObject : public EngineObject
	{
	public:

		ServerLogObject()
			: EngineObject(GetEngineMemoryManager(), "ServerLogObject")
		{}

		// Object task
		virtual Result OnTick(EngineTaskTick tick) { return ResultCode::NOT_IMPLEMENTED; }
	};



	ServerLogComponent::ServerLogComponent(const char* traceCfgPath)
		: LibraryComponent("ServerLogComponent")
	{
	}

	ServerLogComponent::~ServerLogComponent()
	{
	}

	// Initialize server component
	Result ServerLogComponent::InitializeComponent()
	{
		Result hr = LibraryComponent::InitializeComponent();
		if (!(hr)) return hr;

		// initialize exception Handler
		Trace::InitExceptionHandler();

		m_LogObject = new(GetEngineMemoryManager()) ServerLogObject;
		m_LogObject->SetTickGroup(EngineTaskTick::AsyncTick);
		
		return hr;
	}

	// Terminate server component
	void ServerLogComponent::DeinitializeComponent()
	{
		LibraryComponent::DeinitializeComponent();

		m_LogObject = nullptr;
	}


};	// namespace SF


