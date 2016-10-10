////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 MadK
// 
// Author : KyungKun Ko
//
// Description : Default library component
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/BrAssert.h"
#include "Common/Trace.h"
#include "Common/DefaultLibComponent.h"
#include "Common/Utility.h"
#include "Common/BrRandom.h"
#include "Common/BrXML.h"



namespace BR
{

	LibComponentDefault::LibComponentDefault()
		: Component((UINT)ComponentID)
	{
	}

	LibComponentDefault::~LibComponentDefault()
	{
	}

	// Initialize server component
	Result LibComponentDefault::InitializeComponent()
	{
		Result hr = Component::InitializeComponent();
		if (FAILED(hr)) return hr;

		srand((UINT)Util::Time.GetRawTimeMs().time_since_epoch().count());
		Util::Random.Srand(nullptr);

		xmlInitParser();

		LIBXML_TEST_VERSION

		return hr;
	}

	// Terminate server component
	void LibComponentDefault::TerminateComponent()
	{
		Component::TerminateComponent();

		// Cleanup function for the XML library.
		xmlCleanupParser();

		// this is to debug memory for regression tests
		xmlMemoryDump();
	}


};	// namespace BR


