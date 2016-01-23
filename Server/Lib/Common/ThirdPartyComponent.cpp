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
#include "Common/Trace.h"
#include "Common/ThirdPartyComponent.h"
#include "Common/Utility.h"
#include "Common/BrXML.h"



namespace BR
{

	LibComponentThirdParty::LibComponentThirdParty()
		: Component((UINT)ComponentID)
	{
	}

	LibComponentThirdParty::~LibComponentThirdParty()
	{
	}

	// Initialize server component
	HRESULT LibComponentThirdParty::InitializeComponent()
	{
		HRESULT hr = Component::InitializeComponent();
		if (FAILED(hr)) return hr;

		xmlInitParser();

		return hr;
	}

	// Terminate server component
	void LibComponentThirdParty::TerminateComponent()
	{
		Component::TerminateComponent();

		/*
		* Cleanup function for the XML library.
		*/
		xmlCleanupParser();
		/*
		* this is to debug memory for regression tests
		*/
		xmlMemoryDump();
	}


};	// namespace BR


