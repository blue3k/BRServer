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


#include "SFTypedefs.h"
#include "Common/ClassUtil.h"
#include "Memory/SFMemory.h"
#include "Types/BrSvrTypes.h"
#include "Container/PageQueue.h"
#include "Common/LocalUIDGenerator.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Transaction/MessageRoute.h"
#include "Common/HashTable.h"
#include "Container/Indexing.h"

#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServiceEntity/ClusteredServiceEntity.h"


namespace SF {
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

		Result InitializeEntity( EntityID newEntityID );

		// clear transaction
		virtual Result ClearEntity();

		virtual Result RegisterServiceMessageHandler(ServerEntity *pServerEntity) override;

		// TickUpdate 
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;


		//////////////////////////////////////////////////////////////////////////
		//
		//	Monitoring operations
		//

		
	};





}; // namespace Svr
}; // namespace SF




