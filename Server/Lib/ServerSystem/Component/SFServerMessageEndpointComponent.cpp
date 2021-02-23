////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) StormForge
// 
// Author : KyungKun Ko
//
// Description : Server message endpoint
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "Util/SFStringCrc64.h"
#include "Component/SFServerMessageEndpointComponent.h"
#include "EngineObject/SFEngineObject.h"
#include "Entity/Entity.h"
#include "SvrTrace.h"
#include "Util/SFStringFormat.h"

#include "rdkafka/rdkafkacpp.h"


namespace SF
{

	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	MessageConsumeWorker class
	//

	class MessageConsumeWorker : public Thread
	{
	private:

		IHeap& m_Heap;

		SharedPointerT<StreamDBConsumer> m_ListenEndpoint;

	public:
		MessageConsumeWorker(IHeap& heap, const SharedPointerT<StreamDBConsumer>& messageEndpoint)
			: m_Heap(heap)
			, m_ListenEndpoint(messageEndpoint)
		{
		}

		IHeap& GetHeap() { return m_Heap; }

		// Object task
		virtual bool Tick() override
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
			SFUniquePtr<StreamDBConsumer::StreamMessageData> receivedMessageData;
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
					svrTrace(Error, "ServerMessageConsumer: failed to fetch data from message endpoint hr:{0}", hr);
				}
			}
			else
			{
				SharedPointerT<Message::MessageData> pMsg;
				if (receivedMessageData->size() < (sizeof(Message::MessageHeader) + sizeof(RouteContext)))
				{
					svrTrace(Error, "ServerMessageConsumer: invalid message data size");
					return true;
				}

				auto* pMsgHeader = reinterpret_cast<Message::MessageHeader*>(receivedMessageData->data());
				SharedPointerT<Svr::Entity> pEntity;

				svrCheckPtr(pMsg = Message::MessageData::NewMessage(GetHeap(), pMsgHeader->msgID.ID, pMsgHeader->Length, receivedMessageData->data()));

				// First try to route message
				RouteContext routeContext = *(RouteContext*)pMsg->GetMessageData();
				if (routeContext.GetTo().UID == 0)
				{
					svrTrace(Error, "ServerMessageConsumer: message doesn't have valid destination");
					return true;
				}

				if (!Service::EntityTable->find(routeContext.GetTo().GetEntityID(), pEntity))
				{
					svrTrace(Error, "ServerMessageConsumer: Can't find target entity, entityId:{0}", routeContext.GetTo());
					return true;
				}

				auto senderEndpoint = Service::MessageEndpointManager->GetEndpoint(routeContext.GetFrom());
				pEntity->ProcessMessage(senderEndpoint, pMsg);
			}

			return true;
		}
	};




	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ServerMessageEndpointComponent class
	//

	ServerMessageEndpointComponent::ServerMessageEndpointComponent()
		: LibraryComponent("ServerMessageEndpointComponent")
		, m_Heap(GetSystemHeap())
	{
	}

	ServerMessageEndpointComponent::~ServerMessageEndpointComponent()
	{
	}


	// Initialize component
	Result ServerMessageEndpointComponent::InitializeComponent()
	{
		Result hr;
		svrCheck(LibraryComponent::InitializeComponent());

		auto serverEndpointAddress = Service::ServerConfig->ServerEndpointAddress;

		String serverChannel;
		serverChannel.Format("{0}_{1:X4}", serverEndpointAddress.Channel, Service::ServerConfig->UID);
		serverEndpointAddress.Channel = serverChannel;

		m_ServerEndpoint = new(GetHeap()) StreamDBConsumer;
		svrCheckPtr(m_ServerEndpoint);
		svrCheck(m_ServerEndpoint->Initialize(serverEndpointAddress.MessageServer, serverEndpointAddress.Channel));

		std::string errstr;
		m_ServerEndpoint->GetTopicConfig()->set("offset.store.method", "broker", errstr);
		m_ServerEndpoint->GetTopicConfig()->set("auto.offset.reset", "earliest", errstr);

		m_ServerEndpoint->RequestData(StreamDB::OFFSET_END);

		m_MessageConsumeWorker = new(GetHeap()) MessageConsumeWorker(GetHeap(), m_ServerEndpoint);
		svrCheckPtr(m_MessageConsumeWorker);

		m_MessageConsumeWorker->Start();

		// register server endpoint so that others can find it
		auto gameId = Service::ServerConfig->GameClusterID;
		auto serverEntityUID = EntityUID(Service::ServerConfig->UID, 0);

		svrCheck(Service::ServiceDirectory->RegisterLocalService(gameId, ClusterID::None, serverEntityUID, serverEndpointAddress, {}));

		return hr;
	}

	// Terminate component
	void ServerMessageEndpointComponent::DeinitializeComponent()
	{
		if (m_MessageConsumeWorker != nullptr)
			m_MessageConsumeWorker->Stop();
		IHeap::Delete(m_MessageConsumeWorker);

		LibraryComponent::DeinitializeComponent();
	}

}


