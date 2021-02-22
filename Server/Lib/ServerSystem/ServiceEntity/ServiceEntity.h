////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
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
#include "Util/SFTimeUtil.h"
#include "Types/BrSvrTypes.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Container/SFHashTable.h"
#include "Container/SFIndexing.h"
#include "Entity/MasterEntity.h"
#include "Entity/EntityInformation.h"
#include "ServerConfig/SFServerConfig.h"
#include "Variable/SFVariableTable.h"

namespace SF {

	class StreamDBConsumer;
	class Entity;


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ServiceEntity class
	//		- Collection of service cluster server entity
	//

	class ServiceEntity : public Svr::MasterEntity
	{
	public:

		using super = Svr::MasterEntity;

	private:

		// Game id
		GameID m_GameID;

		// Cluster ID
		ClusterID m_ClusterID;

		StringCrc64 m_ClusterName;

		String m_DataCenterPath;

		bool m_Initialized = false;

		SharedPointerT<StreamDBConsumer> m_ListenEndpoint;

		EndpointAddress m_MessageEndpointConfig;

		VariableTable m_CustomAttributes;

	public:

		ServiceEntity(GameID gameID, ClusterID clusterID, const EndpointAddress& endpoint = {});
		~ServiceEntity();

		// Initialized
		bool GetInitialized() { return m_Initialized; }
		void SetInitialized(bool value) { m_Initialized = value; }

		GameID GetGameID() const { return m_GameID; }

		ClusterID GetClusterID() const { return m_ClusterID; }

		StringCrc64 GetClusterName() const { return m_ClusterName; }

		const EndpointAddress& GetMessageEndpointConfig() const { return m_MessageEndpointConfig; }

		const VariableTable& GetCustomAttributes() const { return m_CustomAttributes; }
		VariableTable& GetCustomAttributes() { return m_CustomAttributes; }

		// Get message endpoint
		const SharedPointerT<StreamDBConsumer>& GetListenEndpoint() const { return m_ListenEndpoint; }

		// Initialization called on transaction, if the entity is created after boot up. this method wouldn't be called
		virtual Result StartInitialization() { return ResultCode::SUCCESS; }

		// start initialization
		virtual Result InitializeEntity(EntityID newEntityID) override;

		// Register message handler for this component
		virtual Result RegisterServiceMessageHandler() { return ResultCode::SUCCESS; }

		Result UpdateListenEndpoint();
		virtual Result TickUpdate(TimerAction* pAction) override;
	};


} // namespace SF

