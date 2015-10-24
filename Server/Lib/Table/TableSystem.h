////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Madk
// 
// Description : Conspiracy Table system class
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"


namespace BR {
namespace GameTable {

	// Initialize tables
	HRESULT InitializeTable();

	HRESULT LoadTables();

	// Release all tables
	HRESULT TerminateTable();

}; // namespace GameTable

}

