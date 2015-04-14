////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Monitoring entity definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/Typedefs.h"
#include "Common/ClassUtil.h"
#include "Common/Memory.h"
#include "Common/BrSvrTypes.h"
#include "Common/PageQueue.h"
#include "Common/LocalUIDGenerator.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/MessageRoute.h"
#include "Common/HashTable.h"
#include "Common/Indexing.h"
#include "Common/StaticHashTable.h"

#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServiceEntity/ClusteredServiceEntity.h"


namespace BR {
namespace Svr {

	class Entity;
	class ServerEntity;



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ServerServiceComponentEntity class
	//

	class MonitoringServiceEntity : public ServiceEntity
	{
	public:


	private:


	public:

		MonitoringServiceEntity();
		~MonitoringServiceEntity();


		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity operations
		//

		HRESULT InitializeEntity( EntityID newEntityID );

		// clear transaction
		virtual HRESULT ClearEntity();

		virtual HRESULT RegisterServiceMessageHandler(ServerEntity *pServerEntity) override;

		// TickUpdate 
		virtual HRESULT TickUpdate(Svr::TimerAction *pAction = nullptr) override;


		//////////////////////////////////////////////////////////////////////////
		//
		//	Monitoring operations
		//

		
	};





}; // namespace Svr
}; // namespace BR




