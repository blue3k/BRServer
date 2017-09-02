////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game Party entity
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Common/ClassUtil.h"
#include "Types/SFEngineTypedefs.h"

#include "Transaction/MessageRoute.h"
#include "Entity/Entity.h"
#include "Server/BrServer.h"
#include "ServiceEntity/Game/GameSystem.h"
#include "Component/BrComponent.h"
#include "ServerEntity/ServerEntity.h"
#include "ServerSystem/GameLog/ChattingHistory.h"

#include "Container/Indexing.h"
#include "Container/StaticHashTable.h"

#include "Entity/PlayerEntityInformation.h"

namespace SF {
namespace Policy {
	class IPolicyGameServer;
	class ISvrPolicyGameParty;
};
};

namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////////
	//
	//	Party player class
	//

	class PartyPlayer : public PlayerEntityInformation, public MemoryPoolObject<PartyPlayer>
	{
	public:
		// Hash table mapping Item
		typedef OrderedLinkedList<PlayerID>::Node TableItemType;
		TableItemType TableNode;

	private:

	public:

		// Constructor with constructor
		PartyPlayer( const PlayerInformation& player )
			:PlayerEntityInformation(player)
		{
			memset(&TableNode, 0, sizeof(TableNode) );
		}

	};



	//////////////////////////////////////////////////////////////////////////
	//
	//	Game Party entity class
	//

	class GamePartyEntity : public Svr::MasterEntity, public MemoryPoolObject<GamePartyEntity>
	{
	public:

		typedef Hash::StaticHashTable<	PlayerID, PartyPlayer,
										//Indexing::ConstMemFunc<PlayerEntityInformation,PlayerID,&PlayerEntityInformation::GetPlayerID>,
										Indexing::MapItemConverter<PartyPlayer,typename PartyPlayer::TableItemType,&PartyPlayer::TableNode>,
										Hash::NonUniqueKeyTrait, ThreadSyncTraitNone
										> 
				PartyPlayerUIDMap;

	private:

		// Player by PlayerID
		PartyPlayerUIDMap		m_PartyPlayerByUID;


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game info
		//

		// Team Leader ID
		BRCLASS_ATTRIBUTE(PlayerID,LeaderID);

		// Game matching ticket
		BRCLASS_ATTRIBUTE(MatchingQueueTicket,MatchingTicket);

		BRCLASS_ATTRIBUTE_READONLY(ChattingHistory,ChatHistory);

		
	protected:


	public:

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game Party
		//

		GamePartyEntity();
		~GamePartyEntity();


		// Party ID query
		inline PartyID GetPartyID();
		inline PartyUID GetPartyUID();

		// Get player count at this game
		inline uint GetNumPlayer();

	public:

		// Initialize entity to proceed new connection
		virtual Result InitializeEntity( EntityID newEntityID );

		// Close entity and clear transaction
		virtual Result TerminateEntity();


		//////////////////////////////////////////////////////////////////////////////////////////
		//
		//	foreach implementations
		//

		// foreach game player
		Result ForeachPlayer( std::function<Result(PartyPlayer* pPlayer)> func );

		Result ForeachPlayerGameServer( std::function<Result(PartyPlayer* pPlayer, Policy::IPolicyGameServer *pPolicy)> func );
		Result ForeachPlayerSvrGameParty( std::function<Result(PartyPlayer* pPlayer, Policy::NetSvrPolicyGameParty *pPolicy)> func );




		//////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game Player
		//

		// Join Player
		Result JoinPlayer( PartyPlayer* &pPlayer, bool bIsSilent = true );

		// Player leave
		Result LeavePlayer( PartyPlayer* &pPlayer, bool bIsSilent = true );

		// Select new leader
		Result SelectNewLeader( bool bIsSilent = true );

		// Leave all player
		Result LeaveAllPlayer();

		// Find Player pilotid
		Result FindPlayer( PlayerID pltID, PartyPlayer* &pPartyPlayer );

		// Called when a player get out of game
		Result OnPlayerGetOutOfParty( PartyPlayer *pPlayer, bool bIsSilent = true );

		// Pending close transaction
		Result PendingCloseTransaction();
	};

#include "GamePartyEntity.inl"

}; // namespace Svr
}; // namespace SF
