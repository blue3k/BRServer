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


#include "Common/Typedefs.h"
#include "Common/ClassUtil.h"
#include "Common/BrMemory.h"
#include "Common/BrSvrTypes.h"
#include "Common/PageQueue.h"
#include "Common/LocalUIDGenerator.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/MessageRoute.h"
#include "Common/HashTable.h"
#include "Common/Indexing.h"

#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServiceEntity/ClusteredServiceEntity.h"


namespace BR {
namespace Svr {

	class Entity;
	class ServerEntity;



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ServerServiceComponentEntity class
	//

	class RankingServiceEntity : public FreeReplicaClusterServiceEntity
	{
	public:

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
		UINT m_CurrentProcessingNumberofMember;

		Util::TimeStampTimer m_RankingCheckTimer;

	protected:


	public:

		RankingServiceEntity( ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::StatusWatcher );
		~RankingServiceEntity();

		// We are not going to use hashed key
		virtual UINT KeyHash( UINT64 key ) { return (UINT)key; }


		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity operations
		//

		Result InitializeEntity( EntityID newEntityID );

		// clear transaction
		virtual Result ClearEntity();

		// TickUpdate 
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;


		//////////////////////////////////////////////////////////////////////////
		//
		//	Ranking operations
		//

		Result UpdatePlayerScore(const PlayerInformation& player, int64_t score, int64_t& playerRanking);

		Result GetRankingList(int64_t from, int64_t count, Array<TotalRankingPlayerInformation> &rankingList);
		Result GetRankingListAll(Array<TotalRankingPlayerInformation> &rankingList);

		typedef std::function<void(int32_t Ranking, const TotalRankingPlayerInformation*)> ForeEachFuntor;

		Result ForeachAll(const ForeEachFuntor& functor);


		void CommitChanges() { m_RankingMap.CommitChanges(); }
	};





	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	RankingWatcherServiceEntity class
	//

	class RankingWatcherServiceEntity : public ReplicaClusterServiceEntity, public IServerComponent
	{
	public:

	private:

	public:

		RankingWatcherServiceEntity( ClusterID clusterID, UINT componentID );
		~RankingWatcherServiceEntity();
	};



}; // namespace Svr
}; // namespace BR




