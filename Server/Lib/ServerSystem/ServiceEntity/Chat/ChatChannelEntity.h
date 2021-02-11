////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game ChatChannel entity
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"

#include "Types/SFEngineTypedefs.h"

#include "Transaction/MessageRoute.h"
#include "Entity/Entity.h"
#include "Server/BrServer.h"
#include "ServiceEntity/Game/GameSystem.h"
#include "Component/BrComponent.h"
#include "ServerEntity/ServerEntity.h"


#include "Container/SFIndexing.h"
#include "Container/SFStaticHashTable.h"

#include "Entity/PlayerEntityInformation.h"

namespace SF {
	class NetPolicyGameServer;
	class NetSvrPolicyChatChannel;
};

namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////////
	//
	//	ChatChannel player class
	//

	class ChatChannelPlayer : public PlayerEntityInformation
	{
	public:
		// Hash table mapping Item
		typedef OrderedLinkedList<PlayerID>::Node TableItemType;
		TableItemType TableNode;

	private:

	public:

		// Constructor with constructor
		ChatChannelPlayer( const PlayerInformation& player )
			:PlayerEntityInformation(player)
		{
			memset(&TableNode, 0, sizeof(TableNode) );
		}

	};



	//////////////////////////////////////////////////////////////////////////
	//
	//	ChatChannel entity class
	//

	class ChatChannelEntity : public MasterEntity
	{
	public:

		typedef StaticHashTable<	PlayerID, ChatChannelPlayer,
										MapItemConverter<ChatChannelPlayer,typename ChatChannelPlayer::TableItemType,&ChatChannelPlayer::TableNode>,
										NonUniqueKeyTrait, ThreadSyncTraitNone
										> 
				ChatChannelPlayerUIDMap;

	private:

		// Player by PlayerID
		ChatChannelPlayerUIDMap		m_ChatChannelPlayerByUID;


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Chat channel
		//

		StringCrc64 m_ChatChannelName;

		// Chatting channel Leader ID
		PlayerID m_LeaderID;

		char m_PassCode[64] = "";
	
	protected:


	public:

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game ChatChannel
		//

		ChatChannelEntity(const StringCrc64& chatChannelName);
		~ChatChannelEntity();

		const StringCrc64& GetChatChannelName() const { return m_ChatChannelName; }

		PlayerID GetLeaderID() { return m_LeaderID; }
		void SetLeaderID(PlayerID value) { m_LeaderID = value; }

		void SetPasscode(const char* passCode);
		const char* GetPasscode() { return m_PassCode; }

		// Get player count at this game
		uint GetNumPlayer() { return (uint)m_ChatChannelPlayerByUID.size(); }

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
		Result ForeachPlayer( std::function<Result(ChatChannelPlayer* pPlayer)> func );

		Result ForeachPlayerGameServer( std::function<Result(ChatChannelPlayer*, NetPolicyGameServer &)> func );
		Result ForeachPlayerSvrChatChannel( std::function<Result(ChatChannelPlayer*, NetSvrPolicyChatChannel &)> func );




		//////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game Player
		//

		// Join Player
		Result JoinPlayer( ChatChannelPlayer* pPlayer, bool bIsSilent = true );

		// Player leave
		Result LeavePlayer( ChatChannelPlayer* pPlayer, bool bIsSilent = true );

		// Select new leader
		Result SelectNewLeader( bool bIsSilent = true );

		// Leave all player
		Result LeaveAllPlayer();

		// Find Player pilotid
		Result FindPlayer( PlayerID pltID, ChatChannelPlayer* &pChatChannelPlayer );

		// Called when a player get out of game
		Result OnPlayerGetOutOfChatChannel( ChatChannelPlayer *pPlayer, bool bIsSilent = true );

		// Pending close transaction
		Result PendingCloseTransaction(const char* reason);
	};


}; // namespace Svr
}; // namespace SF
