////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game play system
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/TypeDefs.h"
#include "Common/BrGameTypes.h"
#include "Common/MemoryPool.h"
#include "Common/ClassUtil.h"
#include "ServerSystem/GameSystem.h"
#include "Common/MemoryBufferUtil.h"
#include "Common/GameConst.h"
#include "UserSystemComponentIDs.h"


namespace BR {
namespace GameServer {

	class GamePlayerEntity;

	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlaySystem class
	//

	class UserFriendSystem : public Svr::GameSystem<GamePlayerEntity,UserFriendSystem>
	{
	public:

		enum {
			ComponentID			= UserSystemComponentID_Friend,

			MAX_FRIENDS			= 128,
			MAX_FRIEND_REQUEST	= 17,

			STATUS_SYNC_TIME	= 60 * 1000,
		};

		//struct FriendInfo : public FriendInformation
		//{
		//	INT ShardID;

		//	FriendInfo() : ShardID(0) {}
		//	FriendInfo(const FriendInfo& src) : FriendInformation(src), ShardID(src.ShardID) {}
		//	FriendInfo(AccountID playerID, INT shardID, BR::FacebookUID fbUID, const char* nickName, UINT level, UINT weeklyWin, UINT weeklyLose, BYTE isPlayingGame, UINT64 lastActiveTime, UINT64 LastStaminaSent)
		//		: FriendInformation(playerID, fbUID, nickName, level, weeklyWin, weeklyLose, isPlayingGame, lastActiveTime, LastStaminaSent)
		//		, ShardID(shardID)
		//	{
		//	}
		//	FriendInfo& operator = (const FriendInfo& src)
		//	{
		//		__super::operator = (src);
		//		ShardID = src.ShardID;
		//		return *this;
		//	}
		//	bool operator == (const FriendInfo& src) const { return __super::operator ==(src); }
		//};


	private:

		MemoryBuffer<ServerFriendInformation, sizeof(ServerFriendInformation)*MAX_FRIENDS>	m_Friends;

		BRCLASS_ATTRIBUTE(ULONG, LatestStatusSync);

	public:

		// Constructor 
		UserFriendSystem( GamePlayerEntity* pEntity );
		~UserFriendSystem();


		// Initialize server component
		virtual HRESULT InitializeComponent();

		// Terminate server component
		virtual void TerminateComponent();



		////////////////////////////////////////////////////////////////////
		//
		//	Operations
		//

		// Clear friend list
		void ClearFriendList();

		ULONG GetLatestStatusSync()												{ return m_LatestStatusSync; }

		// Check whether he is a friend or not
		bool IsFriend( PlayerID friendID );

		// Get friend information
		ServerFriendInformation* GetFriend(PlayerID friendID);

		// Change friend nick name
		HRESULT SetFriendName( PlayerID friendID, const char* strNewName );

		UINT GetMaxFriendSlot();
		bool CanAddFriend();

		// Add a friend
		HRESULT AddFriend(const ServerFriendInformation& info);

		// Remove a friend
		HRESULT RemoveFriend( PlayerID friendID ); 

		UINT GetNumberOfFriends();

		// iterate all friend list
		HRESULT ForeachFriends(UINT start, UINT maxCount, std::function<HRESULT(const ServerFriendInformation&)> functor);
	};


}; // namespace GameServer
}; // namespace BR







