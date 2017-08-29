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


#include "Common/Typedefs.h"
#include "Common/BrBaseTypes.h"
#include "Object/SharedPointer.h"


namespace BR {
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
	UINT GetServerUID();

	// Get server ID
	ServerID GetMyServerID();



#include "BrServerUtil.inl"


}; // namespace SvrUtil
}; // namespace BR





