////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Co.
// 
// Author : Madk
// 
// Description : Table system definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"


namespace BR {
namespace GameMasterServer
{
	// Initialize tables
	HRESULT InitializeTable();

	// Release all tables
	HRESULT TerminateTable();

}; // namespace GameMasterServer
};

