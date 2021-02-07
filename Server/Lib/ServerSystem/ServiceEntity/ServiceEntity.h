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
#include "ServerConfig/SFServerConfig.h"

namespace SF {

	class StreamDBProducer;
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

		SharedPointerT<StreamDBProducer> m_ListenEndpoint;

		ServerConfig::MessageEndpoint m_MessageEndpointConfig;

	public:

		ServiceEntity(GameID gameID, ClusterID clusterID, const ServerConfig::MessageEndpoint& endpoint = {});
		~ServiceEntity();

		// Initialized
		bool GetInitialized() { return m_Initialized; }
		void SetInitialized(bool value) { m_Initialized = value; }

		GameID GetGameID() const { return m_GameID; }

		ClusterID GetClusterID() const { return m_ClusterID; }

		StringCrc64 GetClusterName() const { return m_ClusterName; }

		const ServerConfig::MessageEndpoint& GetMessageEndpointConfig() const { return m_MessageEndpointConfig; }

		// Get message endpoint
		const SharedPointerT<StreamDBProducer>& GetListenEndpoint() const { return m_ListenEndpoint; }

		// start initialization
		virtual Result StartInitialization();

		// Register message handler for this component
		virtual Result RegisterServiceMessageHandler() { return ResultCode::SUCCESS; }

		virtual Result TickUpdate(TimerAction* pAction) override;
	};


} // namespace SF

