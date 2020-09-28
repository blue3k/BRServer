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

#include "SFTypedefs.h"
#include "Types/BrGameTypes.h"
#include "Memory/SFMemoryPool.h"
#include "String/SFStringCrc32.h"
#include "String/SFHasher32C.h"
#include "ServiceEntity/Game/GameSystem.h"

#include "GameConst.h"



namespace SF {
namespace Svr {

	class GamePlayerEntity;

	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlaySystem class
	//

	class UserFriendSystem : public Svr::GameSystem<GamePlayerEntity,UserFriendSystem>
	{
	public:

		static constexpr StringCrc32 ComponentID = "UserFriendSystem"_crc32c;
		static constexpr int MAX_FRIENDS = 128;
		static constexpr int MAX_FRIEND_REQUEST = 17;
		static constexpr int STATUS_SYNC_TIME = 60 * 1000;


	private:

		StaticArray<ServerFriendInformation, MAX_FRIENDS>	m_Friends;

		TimeStampMS m_LatestStatusSync;

	public:

		// Constructor 
		UserFriendSystem( GamePlayerEntity* pEntity );
		~UserFriendSystem();


		TimeStampMS GetLatestStatusSync() { return m_LatestStatusSync; }
		void SetLatestStatusSync(TimeStampMS value) { m_LatestStatusSync = value; }


		// Initialize server component
		virtual Result InitializeComponent();

		// Terminate server component
		virtual void TerminateComponent();



		////////////////////////////////////////////////////////////////////
		//
		//	Operations
		//

		// Clear friend list
		void ClearFriendList();


		// Check whether he is a friend or not
		bool IsFriend( PlayerID friendID );

		// Get friend information
		ServerFriendInformation* GetFriend(PlayerID friendID);

		// Change friend nick name
		Result SetFriendName( PlayerID friendID, const char* strNewName );

		uint GetMaxFriendSlot();
		bool CanAddFriend();

		// Add a friend
		Result AddFriend(const ServerFriendInformation& info);

		// Remove a friend
		Result RemoveFriend( PlayerID friendID ); 

		uint GetNumberOfFriends();

		// iterate all friend list
		Result ForeachFriends(uint start, uint maxCount, std::function<Result(const ServerFriendInformation&)> functor);
	};


}; // namespace Svr
}; // namespace SF







