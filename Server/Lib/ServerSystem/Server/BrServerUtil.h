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
#include "Object/SharedPointer.h"


namespace SF {
namespace Svr
{
	class Entity;
	class EntityTable;
	
	//////////////////////////////////////////////////////////////////////////
	//
	//	Utility helper function
	//


	// Get entity table
	EntityTable& GetEntityTable();
	Result FindEntity(EntityID entityID, SharedPointerT<Entity>& entity);

	// Get Server UID
	uint GetServerUID();

	// Get server ID
	ServerID GetMyServerID();



#include "BrServerUtil.inl"


}; // namespace SvrUtil
}; // namespace SF





