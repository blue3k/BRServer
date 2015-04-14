////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Player info manager
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/Typedefs.h"

#include "ServerSystem/SvrTrace.h"
#include "Common/HashTable.h"
#include "Common/Indexing.h"

#include "PlayerInfo.h"



namespace BR {
namespace GameMasterServer {

	//////////////////////////////////////////////////////////////////////////////
	//
	// PlayerInfo manager
	//

	class PlayerInfoManager
	{
	public:


		typedef BR::Hash::HashTable<	PlayerInfo*, 
											BR::Indexing::ConstMemFunc<PlayerInfo,AccountID,&PlayerInfo::GetAccountID>
										> PlayerIDMap;

		typedef BR::Hash::HashTable<	PlayerInfo*, 
										BR::Indexing::ConstMemFunc<PlayerInfo,const char*,&PlayerInfo::GetPlayerName>
										> PlayerNameMap;

	private:


		// PlayerInfo ID
		PlayerIDMap				m_PlayerIDMap;

		// Player Name map
		PlayerNameMap			m_PlayerNameMap;



	public:
		// PlayerInfo Manager
		PlayerInfoManager();
		virtual ~PlayerInfoManager();

		// Initialize mamager 
		HRESULT InitializeManager();

		// Terminate mamager 
		HRESULT TerminateManager();


		// Get number of player
		inline UINT GetNumPlayer();

		//////////////////////////////////////////////////////////////////////////
		//
		//	Player info operations
		//

		// Create PlayerInfo
		HRESULT CreatePlayerInfo( GameServerEntity* pGameEntity, AccountID accID, const char* PlayerName );

		// Create PlayerInfo, Team and PVP id must be released before delete
		HRESULT DeletePlayerInfoByPlayerID( AccountID accID );

		// Get Player info
		HRESULT FindPlayerInfo( AccountID accID, PlayerIDMap::iterator& iterPlayer );

		// Get Player info by name
		HRESULT FindPlayerByName( const char* PlayerName, PlayerNameMap::iterator& iterPlayer );


		// static version
		static HRESULT FindPlayerInfoByID( AccountID accID, PlayerIDMap::iterator& iterPlayer );


		// static version, find first by PlayerID and then PlayerName
		static HRESULT FindPlayerInfo( AccountID accID, const char* PlayerName, PlayerIDMap::iterator& itPlayer, PlayerNameMap::iterator& itNPlayer, PlayerInfo* &pPlayer );


	};


#include "PlayerInfoManager.inl"


}; // namespace GameMasterServer
}; // namespace BR


