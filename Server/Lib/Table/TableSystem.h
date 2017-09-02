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

#include "SFTypedefs.h"


namespace SF {

	class ServerConfig;

namespace GameTable {

	// Initialize tables
	Result InitializeTable(const ServerConfig* config);

	int GetTableVersion();

	Result LoadTables();

	// Release all tables
	Result TerminateTable();

	Result Update();

}; // namespace GameTable

}

