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

#include "Common/Typedefs.h"
#include "Common/ClassUtil.h"
#include "Common/BrBaseTypes.h"

#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/GameSystem.h"
#include "Common/BrComponent.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/GameLog/ChattingHistory.h"

#include "Common/Indexing.h"
#include "Common/StaticHashTable.h"

#include "ServerSystem/PlayerEntityInformation.h"

namespace BR {
namespace Policy {
	class IPolicyGameServer;
	class ISvrPolicyGameParty;
};
};

namespace BR {
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
		inline UINT GetNumPlayer();

	public:

		// Initialize entity to proceed new connection
		virtual HRESULT InitializeEntity( EntityID newEntityID );

		// Close entity and clear transaction
		virtual HRESULT TerminateEntity();


		//////////////////////////////////////////////////////////////////////////////////////////
		//
		//	foreach implementations
		//

		// foreach game player
		HRESULT ForeachPlayer( std::function<HRESULT(PartyPlayer* pPlayer)> func );

		HRESULT ForeachPlayerGameServer( std::function<HRESULT(PartyPlayer* pPlayer, Policy::IPolicyGameServer *pPolicy)> func );
		HRESULT ForeachPlayerSvrGameParty( std::function<HRESULT(PartyPlayer* pPlayer, Policy::ISvrPolicyGameParty *pPolicy)> func );




		//////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game Player
		//

		// Join Player
		HRESULT JoinPlayer( PartyPlayer* &pPlayer, bool bIsSilent = true );

		// Player leave
		HRESULT LeavePlayer( PartyPlayer* &pPlayer, bool bIsSilent = true );

		// Select new leader
		HRESULT SelectNewLeader( bool bIsSilent = true );

		// Leave all player
		HRESULT LeaveAllPlayer();

		// Find Player pilotid
		HRESULT FindPlayer( PlayerID pltID, PartyPlayer* &pPartyPlayer );

		// Called when a player get out of game
		HRESULT OnPlayerGetOutOfParty( PartyPlayer *pPlayer, bool bIsSilent = true );

		// Pending close transaction
		HRESULT PendingCloseTransaction();
	};

#include "GamePartyEntity.inl"

}; // namespace Svr
}; // namespace BR
