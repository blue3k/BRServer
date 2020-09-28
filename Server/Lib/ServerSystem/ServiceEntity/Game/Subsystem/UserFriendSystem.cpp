////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game play system
//	
////////////////////////////////////////////////////////////////////////////////




#include "ServerSystemPCH.h"

#include "Util/SFTimeUtil.h"
#include "Util/SFRandom.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"

#include "SvrConst.h"
#include "SvrTrace.h"
#include "Server/BrServerUtil.h"

#include "Protocol/Policy/GameInstanceNetPolicy.h"
#include "Protocol/Policy/GameNetPolicy.h"

#include "ServiceEntity/Game/Subsystem/UserFriendSystem.h"

#include "ServiceEntity/Game/GamePlayerEntity.h"





namespace SF {
namespace Svr {


	constexpr int UserFriendSystem::MAX_FRIENDS;
	constexpr int UserFriendSystem::MAX_FRIEND_REQUEST;
	constexpr int UserFriendSystem::STATUS_SYNC_TIME;


	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlay state class
	//

	// Constructor 
	UserFriendSystem::UserFriendSystem( GamePlayerEntity* pEntity )
		: GameSystem(pEntity)
		, m_Friends(GetHeap())
	{
	}

	UserFriendSystem::~UserFriendSystem()
	{
	}

	// Initialize system
	Result UserFriendSystem::InitializeComponent()
	{
//		m_MaxFriend = 0;
		m_LatestStatusSync = TimeStampSec(DurationSec(0));
		return ResultCode::SUCCESS;
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
		for( unsigned int index = 0; index < m_Friends.size(); index++ )
		{
			if( m_Friends[index].PlayerID == friendID )
				return &m_Friends[index];
		}

		return nullptr;
	}

	// Change friend nick name
	Result UserFriendSystem::SetFriendName( PlayerID friendID, const char* strNewName )
	{
		for( unsigned int index = 0; index < m_Friends.size(); index++ )
		{
			if( m_Friends[index].PlayerID == friendID )
			{
				return StrUtil::StringCopy( m_Friends[index].NickName, strNewName );
			}
		}

		return ResultCode::FAIL;
	}


	uint UserFriendSystem::GetMaxFriendSlot()
	{
		// TODO: FIXME
		//return GetOwner().GetComponent<UserGamePlayerInfoSystem>()->GetFriendSlot();
		return 20;
	}

	bool UserFriendSystem::CanAddFriend()
	{
		return m_Friends.size() < GetMaxFriendSlot();
	}

	// Add a friend
	Result UserFriendSystem::AddFriend(const ServerFriendInformation& info)
	{
		if( IsFriend(info.PlayerID) )
			return ResultCode::SUCCESS_FALSE;

		if (!CanAddFriend())
		{
			return ResultCode::MAX_FRIEND;
		}

		return m_Friends.push_back( info );
	}

	// Remove a friend
	Result UserFriendSystem::RemoveFriend( PlayerID friendID )
	{
		auto* pFriend = GetFriend(friendID);
		if( pFriend == nullptr )
			return ResultCode::SUCCESS_FALSE;

		return m_Friends.RemoveItem( *pFriend );
	}

	uint UserFriendSystem::GetNumberOfFriends()
	{
		return (uint)m_Friends.size();
	}

	Result UserFriendSystem::ForeachFriends(uint start, uint maxCount, std::function<Result(const ServerFriendInformation&)> functor)
	{
		for (unsigned int index = start; index < m_Friends.size() && index < maxCount; index++)
		{
			Result hr = functor( m_Friends[index] );
			if( !(hr) )
				return hr;
		}

		return ResultCode::SUCCESS;
	}

} // namespace Svr
} // namespace SF



