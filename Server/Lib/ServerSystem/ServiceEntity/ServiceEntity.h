////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Component class definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/Typedefs.h"
#include "Common/ClassUtil.h"
#include "Common/Memory.h"
#include "Common/TimeUtil.h"
#include "Common/BrSvrTypes.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Common/HashTable.h"
#include "Common/Indexing.h"
#include "Common/StaticHashTable.h"

#include "ServerSystem/ServiceEntity/EntityInformation.h"


namespace BR {
namespace Svr {

	class Entity;
	class ServerEntity;


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ClusteredServiceEntity class
	//


	class ServiceEntity : public MasterEntity
	{
	public:

	private:

	public:

	public:

		ServiceEntity();
		~ServiceEntity();


		// Register message handler for this component
		virtual HRESULT RegisterServiceMessageHandler( ServerEntity *pServerEntity ) { return S_SYSTEM_OK; }

	};





}; // namespace Svr
}; // namespace BR




