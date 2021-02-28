////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Game Party service implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "Util/SFStrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Multithread/SFThread.h"
#include "MemoryManager/SFMemory.h"
#include "Net/SFNetDef.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/Party/GamePartyEntity.h"
#include "ServiceEntity/Party/GamePartyManagerServiceEntity.h"
#include "ServiceEntity/Party/GamePartyManagerServiceTrans.h"
#include "ServiceEntity/Party/GamePartyEntityTrans.h"
#include "SvrTrace.h"
#include "SvrConst.h"

#include "PerformanceCounter/PerformanceCounterClient.h"

#include "Protocol/Message/GamePartyManagerMsgClass.h"
#include "Protocol/Message/GamePartyMsgClass.h"




namespace SF {
	namespace Svr {


		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity informations
		//

		GamePartyManagerServiceEntity::GamePartyManagerServiceEntity(GameID gameID, ClusterMembership initialMembership)
			: super(gameID, ClusterID::GamePartyManager, initialMembership)
			, m_PartyCount("PartyCount")
		{
			// Game party manager transactions
			RegisterMessageHandler<PartyManagerTransCreateParty>();
			RegisterMessageHandler<PartyManagerTransPartyDeleted>();
		}

		GamePartyManagerServiceEntity::~GamePartyManagerServiceEntity()
		{
		}

		Result GamePartyManagerServiceEntity::InitializeEntity(EntityID newEntityID)
		{
			Result hr = ResultCode::SUCCESS;

			auto pInstance = PerformanceCounterClient::GetDefaultCounterInstance();
			if (pInstance != nullptr)
			{
				pInstance->AddCounter(&m_PartyCount);
			}

			svrChk(super::InitializeEntity(newEntityID));

			// TODO: We will use stream db queue later
			//svrChk(Service::ServiceDirectory->WatchForService(GetGameID(), ClusterID::MatchingQueue_Game_4x1, ClusterID::MatchingQueue_Game_4x1W));
			//svrChk(Service::ServiceDirectory->WatchForService(GetGameID(), ClusterID::MatchingQueue_Game_8x1, ClusterID::MatchingQueue_Game_8x1W));


		Proc_End:

			return hr;
		}

		Result GamePartyManagerServiceEntity::RegisterServiceMessageHandler()
		{
			Result hr = ResultCode::SUCCESS;

			svrCheck(super::RegisterServiceMessageHandler());

			// Game party manager transactions
			RegisterMessageHandler<PartyManagerTransCreateParty>();
			RegisterMessageHandler<PartyManagerTransPartyDeleted>();

			// Game party instance transactions
			RegisterMessageHandler<PartyTransJoinParty>();
			RegisterMessageHandler<PartyTransLeaveParty>();
			RegisterMessageHandler<PartyTransKickPlayer>();
			RegisterMessageHandler<PartyTransChatMessage>();
			RegisterMessageHandler<PartyTransQuickChatMessage>();

			RegisterMessageHandler<PartyTransStartGameMatchCmd>();
			RegisterMessageHandler<PartyTransCancelGameMatchCmd>();
			RegisterMessageHandler<PartyTransPartyMatchingCanceled>();
			RegisterMessageHandler<PartyTransMatchingItemDequeued>();
			RegisterMessageHandler<PartyTransPartyGameMatchedS2CEvt>();

		return hr;
		}


		//////////////////////////////////////////////////////////////////////////
		//
		//	Game party operations
		//

		// Add new Entity
		Result GamePartyManagerServiceEntity::CreateGameParty(GameID gameID, const PlayerInformation& creator, EntityUID playerUID, const SharedPointerT<MessageEndpoint>& remoteEndpoint, PartyUID& partyUID)
		{
			Result hr = ResultCode::SUCCESS;
			GamePartyEntity* pGameParty = nullptr;
			PartyPlayer* pPlayer = nullptr;

			svrChkPtr(pGameParty = new(GetHeap()) GamePartyEntity(gameID));

			svrChk(Service::EntityManager->AddEntity(EntityFaculty::Party, pGameParty));

			svrMem(pPlayer = new(GetHeap()) PartyPlayer(GetHeap(), playerUID, creator));
			svrChk(pPlayer->SetRemoteEndpoint(playerUID, remoteEndpoint));
			svrChk(pGameParty->JoinPlayer(pPlayer));

			partyUID = pGameParty->GetEntityUID();
			pGameParty = nullptr;

			++m_PartyCount;
			m_LocalWorkload.fetch_add(1, std::memory_order_relaxed);

		Proc_End:

			// close party instance if it failed to initialize
			if (pGameParty)
				pGameParty->PendingCloseTransaction("Game party creation is failed");

			return hr;
		}

		// Called when a game party is deleted
		Result GamePartyManagerServiceEntity::FreeGameParty(PartyUID partyUID)
		{
			Result hr = ResultCode::SUCCESS;

			if (!(Service::EntityManager->RemoveEntity(partyUID.GetEntityID())))
			{
				svrTrace(Error, "Failed to delete party info {0}", partyUID);
			}
			else
			{
				--m_PartyCount;
				svrTrace(SVR_INFO, "Party deleted {0}", partyUID);
				m_LocalWorkload.fetch_sub(1, std::memory_order_relaxed);
			}

			//Proc_End:

			return hr;
		}







	} // namespace Svr {
} // namespace SF {

