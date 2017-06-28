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
#include "Common/BrAssert.h"
#include "Common/SharedPointer.h"
#include "Common/Trace.h"
#include "Common/Trace_Internal.h"
#include "Common/TraceComponent.h"
#include "Common/Utility.h"



namespace BR
{

	LibComponentTrace::LibComponentTrace()
		: Component((UINT)ComponentID)
	{
	}

	LibComponentTrace::LibComponentTrace(const char* modulePath, const char* moduleName, const char* traceCfgPath)
		: Component((UINT)ComponentID)
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

		//int iMod = 0;

		//for (int iNewMod = 0; iNewMod < Trace::TraceModule::MAX_TRACEMODULE; iNewMod++)
		//{
		//	if (TraceModule::GetModules()[iNewMod] == nullptr)
		//		break;

		//	for (; iMod < Trace::TraceModule::MAX_TRACEMODULE; iMod++)
		//	{
		//		if (trcMods[iMod] == nullptr)
		//		{
		//			trcMods[iMod] = TraceModule::GetModules()[iNewMod];
		//			break;
		//		}
		//	}
		//}

		//TraceOutModule::SetInstance(&trcShare);

		pInstance = SharedPointerT<Trace::TraceOutModule>(new Trace::TraceOutModule);
		Trace::TraceOutModule::SetInstance(pInstance.GetObjectPtr());
		m_pInstance = pInstance;

		pInstance->CheckAndUpdate(Util::Time.GetRawUTCSec());

		pInstance->Start();

		return hr;
	}

	// Terminate server component
	void LibComponentTrace::TerminateComponent()
	{
		Component::TerminateComponent();

		//	if( trcMods )
		//	{
		//		for( int iNewMod = 0; iNewMod < Trace::TraceModule::MAX_TRACEMODULE; iNewMod++ )
		//		{
		//			if( TraceModule::GetModules()[iNewMod] == nullptr )
		//				break;

		//			for( int iMod = 0; iMod < Trace::TraceModule::MAX_TRACEMODULE; iMod++ )
		//			{
		//				if( trcMods[iMod] == TraceModule::GetModules()[iNewMod] )
		//				{
		//					trcMods[iMod] = nullptr;
		//					break;
		//				}
		//			}
		//		}
		//	}

		Trace::TraceOutModule::SetInstance(nullptr);
		m_pInstance = SharedPointerT<SharedObject>();
	}


};	// namespace BR


