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
#include "String/SFStringFormat.h"
#include "rdkafka/rdkafkacpp.h"


namespace SF {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ClusteredServiceEntity class
	//

	ServiceEntity::ServiceEntity(GameID gameID, ClusterID clusterID, const EndpointAddress& endpoint)
		: m_GameID(gameID)
		, m_ClusterID(clusterID)
	{
		// Update channel path to yours
		m_MessageEndpointConfig.MessageServer = endpoint.MessageServer;
		if (!m_MessageEndpointConfig.MessageServer.IsNullOrEmpty())
			m_MessageEndpointConfig.Channel.Format("{0}_{1}_{2}", endpoint.Channel, gameID, clusterID);

		// This entity will be updated on designated thread rather than shared thicker thread
		SetUseDesignatedThread(true);
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

			std::string errstr;
			m_ListenEndpoint->GetTopicConfig()->set("offset.store.method", "broker", errstr);
			m_ListenEndpoint->GetTopicConfig()->set("auto.offset.reset", "earliest", errstr);

			svrCheck(m_ListenEndpoint->RequestData(StreamDB::OFFSET_END));
		}

		svrCheck(Service::ServiceDirectory->RegisterLocalService(m_GameID, m_ClusterID, GetEntityUID(), m_MessageEndpointConfig));

		return hr;
	}

	Result ServiceEntity::UpdateListenEndpoint()
	{
		if (m_ListenEndpoint == nullptr)
			return ResultCode::SUCCESS;

		if (!m_ListenEndpoint->IsDateRequested())
		{
			if (!m_ListenEndpoint->RequestData(StreamDB::OFFSET_END))
				return ResultCode::NO_DATA_EXIST;
		}

		// This is designated thread tick function
		// I don't sleep except waiting on the date.
		UniquePtr<StreamDBConsumer::StreamMessageData> receivedMessageData;
		Result hr = m_ListenEndpoint->PollData(receivedMessageData, 500);
		if (!hr)
		{
			if (hr == ResultCode::NO_DATA_EXIST || hr == ResultCode::END_OF_STREAM)
			{
				// ignore those error
				hr = ResultCode::SUCCESS;
			}
			else
			{
				svrTrace(Error, "ServiceEntity: failed to fetch data from message endpoint hr:{0}", hr);
			}

			return hr;
		}

		SharedPointerT<Message::MessageData> pMsg;
		if (receivedMessageData->size() < sizeof(Message::MessageHeader))
			return ResultCode::INVALID_FORMAT;

		auto* pMsgHeader = reinterpret_cast<Message::MessageHeader*>(receivedMessageData->data());
		svrCheckPtr(pMsg = Message::MessageData::NewMessage(GetHeap(), pMsgHeader->msgID.ID, pMsgHeader->Length, receivedMessageData->data()));

		hr = ProcessMessage(nullptr, pMsg);

		return hr;
	}

	Result ServiceEntity::TickUpdate(TimerAction* pAction)
	{
		Result hr;

		svrCheck(super::TickUpdate(pAction));

		UpdateListenEndpoint();

		return hr;

	}

} // namespace SF



