////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
// 
// Description : Conspiracy Table system class
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"


namespace SF {

	namespace GameTable {

		// Initialize tables
		Result InitializeTable();

		int GetTableVersion();

		Result LoadTables();

		// Release all tables
		Result TerminateTable();

		Result Update();

	} // namespace GameTable

}

