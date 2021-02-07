////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Server service Component entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "String/SFStrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/SFThread.h"
#include "Net/SFNetDef.h"
#include "Net/SFMessageEndpoint.h"
#include "StreamDB/SFStreamDB.h"
#include "Service/ServerService.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServiceEntity/ServiceEntity.h"
#include "ServerEntity/ServerEntityManager.h"
#include "SvrTrace.h"
#include "Task/ServerTaskEvent.h"



namespace SF {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ClusteredServiceEntity class
	//

	ServiceEntity::ServiceEntity(GameID gameID, ClusterID clusterID, const ServerConfig::MessageEndpoint& endpoint)
		: m_GameID(gameID)
		, m_ClusterID(clusterID)
	{
		m_MessageEndpointConfig = endpoint;
	}

	ServiceEntity::~ServiceEntity()
	{
	}

	Result ServiceEntity::StartInitialization()
	{
		Result hr;

		if (!m_MessageEndpointConfig.MessageServer.IsNullOrEmpty())
		{
			m_ListenEndpoint = new(GetHeap()) StreamDBConsumer;
			svrCheck(m_ListenEndpoint->Initialize(m_MessageEndpointConfig.MessageServer, m_MessageEndpointConfig.Channel));
		}

		svrCheck(Service::ServiceDirectory->RegisterLocalService(m_GameID, m_ClusterID, GetEntityUID(), m_MessageEndpointConfig));


		return hr;
	}

	Result ServiceEntity::TickUpdate(TimerAction* pAction)
	{
		Result hr;

		svrCheck(super::TickUpdate(pAction));



		return hr;

	}

} // namespace SF



