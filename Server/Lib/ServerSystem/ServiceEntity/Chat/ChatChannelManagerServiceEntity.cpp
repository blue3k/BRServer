////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Game ChatChannel service implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "String/SFStrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Multithread/SFThread.h"
#include "MemoryManager/SFMemory.h"
#include "Net/SFNetDef.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/Chat/ChatChannelEntity.h"
#include "ServiceEntity/Chat/ChatChannelManagerServiceEntity.h"
#include "ServiceEntity/Chat/ChatChannelManagerServiceTrans.h"
#include "ServiceEntity/Chat/ChatChannelEntityTrans.h"
#include "SvrTrace.h"
#include "SvrConst.h"

#include "PerformanceCounter/PerformanceCounterClient.h"

#include "Protocol/Message/ChatChannelManagerMsgClass.h"
#include "Protocol/Message/ChatChannelMsgClass.h"




namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	ChatChannelManagerServiceEntity::ChatChannelManagerServiceEntity(GameID gameID, const EndpointAddress& endpoint)
		: super(gameID, ClusterID::ChatChannelManager, endpoint)
		, m_ChatChannelCount("ChatChannelCount")
	{
		// Game ChatChannel manager transactions
		BR_ENTITY_MESSAGE(Message::ChatChannelManager::CreateChannelCmd)			{ svrMemReturn(pNewTrans = new(GetHeap()) ChatChannelManagerTransCreateChatChannel(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::ChatChannelManager::FindChannelCmd)				{ svrMemReturn(pNewTrans = new(GetHeap()) ChatChannelManagerTransFindChatChannel(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::ChatChannelManager::ChatChannelDeletedC2SEvt)	{ svrMemReturn(pNewTrans = new(GetHeap()) ChatChannelManagerTransChatChannelDeleted(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
	}

	ChatChannelManagerServiceEntity::~ChatChannelManagerServiceEntity()
	{
	}

	Result ChatChannelManagerServiceEntity::InitializeEntity(EntityID newEntityID)
	{
		Result hr = ResultCode::SUCCESS;

		auto pInstance = PerformanceCounterClient::GetDefaultCounterInstance();
		if (pInstance != nullptr)
		{
			pInstance->AddCounter(&m_ChatChannelCount);
		}

		svrCheck(super::InitializeEntity(newEntityID));

		return hr;
	}

	Result ChatChannelManagerServiceEntity::RegisterServiceMessageHandler()
	{
		Result hr = ResultCode::SUCCESS;

		svrCheck(super::RegisterServiceMessageHandler() );

		// Chat channel manager transactions
		BR_ENTITY_MESSAGE(Message::ChatChannelManager::CreateChannelCmd)			{ svrMemReturn(pNewTrans = new(GetHeap()) ChatChannelManagerTransCreateChatChannel(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::ChatChannelManager::FindChannelCmd)				{ svrMemReturn(pNewTrans = new(GetHeap()) ChatChannelManagerTransFindChatChannel(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::ChatChannelManager::ChatChannelDeletedC2SEvt)	{ svrMemReturn(pNewTrans = new(GetHeap()) ChatChannelManagerTransChatChannelDeleted(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );

		return hr;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	Game ChatChannel operations
	//

	// Add new Entity
	Result ChatChannelManagerServiceEntity::CreateChatChannel(const StringCrc64& name, const PlayerInformation& creator, EntityUID playerUID, const SharedPointerT<MessageEndpoint>& remoteEndpoint, EntityUID &ChatChannelUID )
	{
		Result hr = ResultCode::SUCCESS;
		ChatChannelEntity *pChatChannel = nullptr;
		ChatChannelPlayer *pPlayer = nullptr;

		svrCheckPtr( pChatChannel = new(GetHeap()) ChatChannelEntity(name) );

		svrCheck(Service::EntityManager->AddEntity( EntityFaculty::ChatChannel, pChatChannel ) );

		svrCheckMem( pPlayer = new(GetHeap()) ChatChannelPlayer( creator ) );
		svrCheck( pPlayer->SetRemoteEndpoint( remoteEndpoint, playerUID ) );
		svrCheck( pChatChannel->JoinPlayer( pPlayer ) );

		ChatChannelUID = pChatChannel->GetEntityUID();
		pChatChannel = nullptr;

		++m_ChatChannelCount;

		// close ChatChannel instance if it failed to initialize
		if( pChatChannel )
			pChatChannel->PendingCloseTransaction("Game ChatChannel creation is failed");

		return hr;
	}

	Result ChatChannelManagerServiceEntity::FindChatChannel( const StringCrc64& name, EntityUID &ChatChannelUID )
	{
		Result hr = ResultCode::SUCCESS;
		ChatChannelEntity *pChatChannel = nullptr;
		ChatChannelPlayer *pPlayer = nullptr;

		// TODO: We need to go to chat channel directory to find it
		ChatChannelUID = pChatChannel->GetEntityUID();

		return hr;
	}

	// Called when a game ChatChannel is deleted
	Result ChatChannelManagerServiceEntity::FreeChatChannel( EntityUID ChatChannelUID )
	{
		Result hr = ResultCode::SUCCESS;

		if (!(Service::EntityManager->RemoveEntity(ChatChannelUID.GetEntityID())))
		{
			svrTrace(Error, "Failed to delete ChatChannel info {0}", ChatChannelUID);
		}
		else
		{
			--m_ChatChannelCount;
			svrTrace(SVR_INFO, "ChatChannel deleted {0}", ChatChannelUID);
		}

		return hr;
	}







}; // namespace Svr {
}; // namespace SF {



