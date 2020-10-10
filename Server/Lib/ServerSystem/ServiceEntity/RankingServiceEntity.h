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

#include "Memory/SFMemory.h"
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
#include "ServiceEntity/ClusteredServiceEntity.h"


namespace SF {
namespace Svr {

	class Entity;
	class ServerEntity;



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ServerServiceComponentEntity class
	//

	class RankingServiceEntity : public ShardedClusterServiceEntity
	{
	public:

		typedef ShardedClusterServiceEntity super;

		//TODO: I need 128 bit integer for ranking key
		union RankingKey
		{
			struct {
				uint32_t PlayerID;
				uint32_t Score;
			};
			uint64_t RankingKeyValue;
		};

	private:

		DualSortedMap<uint64_t, TotalRankingPlayerInformation*> m_RankingMap;

		DualSortedMap<PlayerID, TotalRankingPlayerInformation*> m_PlayerMap;


		// Currently processing number of members
		uint m_CurrentProcessingNumberofMember;

		Util::TimeStampTimer m_RankingCheckTimer;

	protected:


	public:

		RankingServiceEntity(GameID gameID, ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::Slave);
		~RankingServiceEntity();

		// We are not going to use hashed key
		virtual uint KeyHash( uint64_t key ) { return (uint)key; }


		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity operations
		//

		virtual Result InitializeEntity( EntityID newEntityID ) override;

		// clear transaction
		virtual Result ClearEntity() override;

		// TickUpdate 
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;


		//////////////////////////////////////////////////////////////////////////
		//
		//	Ranking operations
		//

		Result UpdatePlayerScore(const PlayerInformation& player, int64_t score, int64_t& playerRanking);

		Result GetBaseRanking(int64_t score, int64_t& playerRanking);

		Result GetRankingList(int64_t from, int64_t count, Array<TotalRankingPlayerInformation> &rankingList);
		Result GetRankingListAll(Array<TotalRankingPlayerInformation> &rankingList);

		typedef std::function<void(int32_t Ranking, const TotalRankingPlayerInformation*)> ForeEachFuntor;

		Result ForeachAll(const ForeEachFuntor& functor);


		void CommitChanges() { m_RankingMap.CommitChanges(); }

		void PrintAllRanking(const char* fileName);

	};






}; // namespace Svr
}; // namespace SF




