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
namespace Svr
{
	class Entity;
	class EntityTable;
	
	//////////////////////////////////////////////////////////////////////////
	//
	//	Utility helper function
	//



	// Get Server UID
	uint GetServerUID();

	// Get server ID
	ServerID GetMyServerID();


	void InitializeEngine();

	// Initialize and deinitialization
	void InitializeEngineForServer();

	void DeinitializeEngine();


#include "BrServerUtil.inl"


}; // namespace SvrUtil
}; // namespace SF





