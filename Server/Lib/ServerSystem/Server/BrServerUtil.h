////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server Utility definition
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"
#include "Types/SFEngineTypedefs.h"
#include "Object/SFSharedPointer.h"


namespace SF {
	
	//////////////////////////////////////////////////////////////////////////
	//
	//	Utility helper function
	//

	struct EngineInitParam;

	// Get Server UID
	uint GetServerUID();

	// Get server ID
	ServerID GetMyServerID();


	void InitializeEngine(EngineInitParam& initParam);

	// Initialize and deinitialization
	void InitializeEngineForServer();

	void DeinitializeEngine();


#include "BrServerUtil.inl"


} // namespace SF





