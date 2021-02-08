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
#include "String/SFStringCrc64.h"
#include "Component/SFServerMessageEndpointComponent.h"
#include "EngineObject/SFEngineObject.h"
#include "Entity/Entity.h"
#include "SvrTrace.h"

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
				svrCheckPtr(pMsg);

				// First try to route message
				RouteContext routeContext = *(RouteContext*)pMsg->GetMessageData();
				if (routeContext.GetTo().UID == 0)
				{
					svrTrace(Error, "ServerMessageConsumer: message doesn't have valid destination");
					return true;
				}

				if (!Service::EntityTable->find(routeContext.GetTo().GetEntityID(), pEntity))
				{
					svrTrace(Error, "ServerMessageConsumer: Can't find target entity");
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

		m_MessageConsumeWorker = new(GetHeap()) MessageConsumeWorker(GetHeap(), m_ServerEndpoint);
		svrCheckPtr(m_MessageConsumeWorker);

		m_MessageConsumeWorker->Start();

		// register server endpoint so that others can find it
		auto serverEndpointAddress = Service::ServerConfig->ServerEndpointAddress;
		auto gameId = Service::ServerConfig->GameClusterID;
		auto serverEntityUID = EntityUID(Service::ServerConfig->UID, 0);

		svrCheck(Service::ServiceDirectory->RegisterLocalService(gameId, ClusterID::None, serverEntityUID, serverEndpointAddress));

		return hr;
	}

	// Terminate component
	void ServerMessageEndpointComponent::DeinitializeComponent()
	{
		if (m_MessageConsumeWorker != nullptr)
			m_MessageConsumeWorker->Stop();
		delete m_MessageConsumeWorker;

		LibraryComponent::DeinitializeComponent();
	}

}


