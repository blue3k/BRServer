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

#include "Memory/SFMemory.h"
#include "Util/SFTimeUtil.h"
#include "Types/BrSvrTypes.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Container/SFHashTable.h"
#include "Container/SFIndexing.h"
#include "Entity/MasterEntity.h"
#include "Entity/EntityInformation.h"


namespace SF {
namespace Svr {

	class Entity;
	class ServerEntity;


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ServiceEntity class
	//		- Collection of service cluster server entity
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
}; // namespace SF




