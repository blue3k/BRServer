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



#include "StdAfx.h"
#include "Common/StrUtil.h"
#include "Common/HRESCommon.h"
#include "Common/HRESSvrSys.h"
#include "ServerSystem/SvrTrace.h"

#include "GameMasterServerClass.h"
#include "PlayerInfoManager.h"
#include "ServerEntity/GameServerEntity.h"


#include "Protocol/Policy/GameMasterServerIPolicy.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Policy/GameIPolicy.h"


namespace BR {
namespace GameMasterServer {


		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity Information table
		//

		// PlayerInfo Manager
		PlayerInfoManager::PlayerInfoManager()
		{
		}

		PlayerInfoManager::~PlayerInfoManager()
		{
		}


		// Initialize mamager 
		HRESULT PlayerInfoManager::InitializeManager()
		{
			return S_OK;
		}

		// Terminate mamager 
		HRESULT PlayerInfoManager::TerminateManager()
		{
			return S_OK;
		}


		//////////////////////////////////////////////////////////////////////////
		//
		//	Player info operations
		//

		// Create PlayerInfo
		HRESULT PlayerInfoManager::CreatePlayerInfo( GameServerEntity* pGameEntity, AccountID accID, const char* PlayerName )
		{
			HRESULT hr = S_OK;
			PlayerInfo *pNewPlayerInfo = nullptr;
			PlayerIDMap::iterator itPlayer;

			if( SUCCEEDED(m_PlayerIDMap.find( accID, itPlayer )) )
			{
				svrErr( E_DUPLICATED_PLAYERID );
			}

			svrChkPtr( pGameEntity );

			svrChkPtr( PlayerName );

			svrMem( pNewPlayerInfo = new PlayerInfo );
			svrChk( pNewPlayerInfo->Initialize(
								pGameEntity->GetServerID(),
								pGameEntity->GetConnection()->GetPolicy<Policy::IPolicyGameServer>(),
								accID, PlayerName ) );

			svrChk( m_PlayerIDMap.insert( pNewPlayerInfo ) );
			m_PlayerNameMap.insert( pNewPlayerInfo );
			pNewPlayerInfo = nullptr;

		Proc_End:

			Util::SafeDelete( pNewPlayerInfo );

			return hr;
		}

		
		// Create PlayerInfo, Team and PVP id must be released before delete
		HRESULT PlayerInfoManager::DeletePlayerInfoByPlayerID( AccountID accID )
		{
			HRESULT hr = S_OK;
			PlayerInfo *pPlayerInfo = nullptr;
			PlayerIDMap::iterator itPlayer;

			if( FAILED(m_PlayerIDMap.find( accID, itPlayer )) )
			{
				svrErr( E_INVALID_PLAYERID );
			}

			pPlayerInfo = *itPlayer;

			m_PlayerNameMap.erase( pPlayerInfo );
			svrChk( m_PlayerIDMap.erase( itPlayer ) );


		Proc_End:

			Util::SafeDelete( pPlayerInfo );

			return hr;
		}

		// Get Player info
		HRESULT PlayerInfoManager::FindPlayerInfo( AccountID accID, PlayerIDMap::iterator& iterPlayer )
		{
			return m_PlayerIDMap.find( accID, iterPlayer );
		}


		// Get Player info by name
		HRESULT PlayerInfoManager::FindPlayerByName( const char* PlayerName, PlayerNameMap::iterator& iterPlayer )
		{
			return m_PlayerNameMap.find( PlayerName, iterPlayer );
		}

		// static version
		HRESULT PlayerInfoManager::FindPlayerInfoByID( AccountID accID, PlayerIDMap::iterator& iterPlayer )
		{
			HRESULT hr = S_OK;
			PlayerInfoManager *pInfoMgr = nullptr;

			svrChkPtr( pInfoMgr = GetPlayerInfoMgr() );

			svrChk( pInfoMgr->FindPlayerInfo( accID, iterPlayer ) );

		Proc_End:
			return hr;
		}

		
		// static version, find first by PlayerID and then PlayerName
		HRESULT PlayerInfoManager::FindPlayerInfo( AccountID accID, const char* PlayerName, PlayerIDMap::iterator& itPlayer, PlayerNameMap::iterator& itNPlayer, PlayerInfo* &pPlayer )
		{
			HRESULT hr = S_OK;
			PlayerInfoManager *pPlayerManager = nullptr;


			svrChkPtr( pPlayerManager = GetPlayerInfoMgr() );
			if( accID > 0 )
			{
				if( FAILED( pPlayerManager->FindPlayerInfo( accID, itPlayer ) ) )
					svrErr( E_PLAYER_NOT_FOUND );
				pPlayer = *itPlayer;
			}
			else if( PlayerName != nullptr )
			{
				if( FAILED( pPlayerManager->FindPlayerByName( PlayerName, itNPlayer ) ) )
					svrErr( E_PLAYER_NOT_FOUND );
				pPlayer = *itNPlayer;
			}
			else
			{
				svrErr( E_PLAYER_NOT_FOUND );
			}

		Proc_End:

			return hr;
		}


}; // namespace GameMasterServer {
}; // namespace BR {



