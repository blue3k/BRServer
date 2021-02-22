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

#include "MemoryManager/SFMemory.h"
#include "Types/BrSvrTypes.h"
#include "Container/SFPageQueue.h"
#include "Util/LocalUIDGenerator.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Transaction/MessageRoute.h"
#include "Container/SFHashTable.h"
#include "Container/SFIndexing.h"

#include "Entity/EntityInformation.h"
#include "ServiceEntity/ServiceEntity.h"


namespace SF {
namespace Svr {

	class Entity;
	class ServerEntity;



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	GameInstanceServiceEntity class
	//

	class GameInstanceServiceEntity : public ServiceEntity
	{
	public:

		using super = ServiceEntity;

	private:

		// Component manager
		ComponentManager m_ComponentManger;

	public:

		GameInstanceServiceEntity(GameID gameID, const EndpointAddress& endpoint);
		~GameInstanceServiceEntity();


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Component manager
		//

		ComponentManager& GetComponentManager() { return m_ComponentManger; }

		template< class ComponentType >
		ComponentType* GetComponent() { return m_ComponentManger.GetComponent<ComponentType>(); }

		// Get component with its type
		template< class ComponentType >
		const ComponentType* GetComponent() const { return m_ComponentManger.GetComponent<ComponentType>(); }


		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity operations
		//

		virtual Result InitializeEntity( EntityID newEntityID ) override;

		// clear transaction
		virtual Result ClearEntity() override;

		// TickUpdate 
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;
	};


} // namespace Svr
} // namespace SF




