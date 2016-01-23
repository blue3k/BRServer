////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game play system
//	
////////////////////////////////////////////////////////////////////////////////




#include "stdafx.h"

#include "Common/TimeUtil.h"
#include "Common/BrRandom.h"
#include "Common/ResultCode/BRResultCodeSvr.h"
#include "Common/ResultCode/BRResultCodeCommon.h"
#include "Common/ResultCode/BRResultCodeGame.h"

#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServerUtil.h"
#include "GameSvrConst.h"

#include "Protocol/Policy/GameInstanceIPolicy.h"
#include "GameServerClass.h"
#include "Protocol/Policy/GameIPolicy.h"

#include "GameSystem/UserFriendSystem.h"
#include "GameSystem/UserGamePlayerInfoSystem.h"

#include "GameInstance/GamePlayerEntity.h"

#include "Table/conspiracy/LevelTbl.h"


BR_MEMORYPOOL_IMPLEMENT(GameServer::UserFriendSystem);


namespace BR {
namespace GameServer {


	
	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlay state calss
	//

	// Constructor 
	UserFriendSystem::UserFriendSystem( GamePlayerEntity* pEntity )
		:GameSystem(pEntity)
	{
	}

	UserFriendSystem::~UserFriendSystem()
	{
	}

	// Initialzie system
	HRESULT UserFriendSystem::InitializeComponent()
	{
//		m_MaxFriend = 0;
		m_LatestStatusSync = TimeStampSec(DurationSec(0));
		return S_SYSTEM_OK;
	}

	// Terminate component
	void UserFriendSystem::TerminateComponent()
	{
		m_Friends.Clear();
	}


	////////////////////////////////////////////////////////////////////
	//
	//	Operations
	//

	
	// Clear friend list
	void UserFriendSystem::ClearFriendList()
	{
		m_Friends.Clear();
	}

	// Check whether he is a friend or not
	bool UserFriendSystem::IsFriend( PlayerID friendID )
	{
		return GetFriend(friendID) != nullptr;
	}
	
	// Get friend information
	ServerFriendInformation* UserFriendSystem::GetFriend(PlayerID friendID)
	{
		for( unsigned int index = 0; index < m_Friends.GetSize(); index++ )
		{
			if( m_Friends[index].PlayerID == friendID )
				return &m_Friends[index];
		}

		return nullptr;
	}

	// Change friend nick name
	HRESULT UserFriendSystem::SetFriendName( PlayerID friendID, const char* strNewName )
	{
		for( unsigned int index = 0; index < m_Friends.GetSize(); index++ )
		{
			if( m_Friends[index].PlayerID == friendID )
			{
				return StrUtil::StringCpy( m_Friends[index].NickName, strNewName );
			}
		}

		return E_SYSTEM_FAIL;
	}


	UINT UserFriendSystem::GetMaxFriendSlot()
	{
		return GetOwner().GetComponent<UserGamePlayerInfoSystem>()->GetFriendSlot();
	}

	bool UserFriendSystem::CanAddFriend()
	{
		return m_Friends.GetSize() < GetMaxFriendSlot();
	}

	// Add a friend
	HRESULT UserFriendSystem::AddFriend(const ServerFriendInformation& info)
	{
		if( IsFriend(info.PlayerID) )
			return S_SYSTEM_FALSE;

		if (!CanAddFriend())
		{
			return E_MAX_FRIEND;
		}

		return m_Friends.Append( 1, &info );
	}

	// Remove a friend
	HRESULT UserFriendSystem::RemoveFriend( PlayerID friendID )
	{
		auto* pFriend = GetFriend(friendID);
		if( pFriend == nullptr )
			return S_SYSTEM_FALSE;

		return m_Friends.Remove( pFriend );
	}

	UINT UserFriendSystem::GetNumberOfFriends()
	{
		return (UINT)m_Friends.GetSize();
	}

	HRESULT UserFriendSystem::ForeachFriends(UINT start, UINT maxCount, std::function<HRESULT(const ServerFriendInformation&)> functor)
	{
		for (unsigned int index = start; index < m_Friends.GetSize() && index < maxCount; index++)
		{
			HRESULT hr = functor( m_Friends[index] );
			if( FAILED(hr) )
				return hr;
		}

		return S_SYSTEM_OK;
	}

}; // namespace GameServer
}; // namespace BR







