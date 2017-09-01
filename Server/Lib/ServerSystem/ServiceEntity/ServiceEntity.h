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


#include "SFTypedefs.h"
#include "Common/ClassUtil.h"
#include "Memory/SFMemory.h"
#include "Util/TimeUtil.h"
#include "Common/BrSvrTypes.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Common/HashTable.h"
#include "Common/Indexing.h"

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
		virtual Result RegisterServiceMessageHandler( ServerEntity *pServerEntity )  { return ResultCode::SUCCESS; }

	};





}; // namespace Svr
}; // namespace BR




