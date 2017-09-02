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
#include "Common/SharedPointer.h"
#include "ServerLog/SvrLog.h"
#include "Common/Trace_Internal.h"
#include "Common/TraceComponent.h"
#include "Util/Utility.h"



namespace SF
{

	LibComponentTrace::LibComponentTrace()
		: Component((uint)ComponentID)
	{
	}

	LibComponentTrace::LibComponentTrace(const char* modulePath, const char* moduleName, const char* traceCfgPath)
		: Component((uint)ComponentID)
	{
		if (modulePath != nullptr)
			Util::SetModulePath(modulePath, moduleName);

		if (traceCfgPath != nullptr)
			Trace::TraceModule::CONFIG_FILENAME = traceCfgPath;
	}

	LibComponentTrace::~LibComponentTrace()
	{
	}

	// Initialize server component
	Result LibComponentTrace::InitializeComponent()
	{
		SharedPointerT<Trace::TraceOutModule> pInstance;

		Result hr = Component::InitializeComponent();
		if (!(hr)) return hr;

		// initialize exception Handler
		Trace::InitExceptionHandler();

		pInstance = SharedPointerT<Trace::TraceOutModule>(new Trace::TraceOutModule);
		Trace::TraceOutModule::SetInstance(pInstance.GetObjectPtr());
		m_pInstance = pInstance;

		pInstance->CheckAndUpdate(Util::Time.GetRawUTCSec());

		pInstance->Start();
		pInstance->SetPriority(Thread::PRIORITY::HIGHEST);

		return hr;
	}

	// Terminate server component
	void LibComponentTrace::TerminateComponent()
	{
		Component::TerminateComponent();

		Trace::TraceOutModule::SetInstance(nullptr);
		m_pInstance = SharedPointerT<SharedObject>();
	}


};	// namespace SF


