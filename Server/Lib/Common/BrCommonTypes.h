////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : Brave base types
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/BrBaseTypes.h"


namespace BR {

#pragma pack(push)
#pragma pack(4)
	
	// Game ID
	enum class GameID : UINT16
	{
		Invalid,				// Not used
		Conspiracy,				// Conspiracy
		Game,					// Generic game
	};

	// Notification message
	enum class NotificationType : UINT16
	{
		None,				// Not used
		MessageNotify,
		FriendRequest,
		GiftStamina,
	};

	// Player information
	struct PlayerInformation
	{
		enum {
			MAX_NAME = 64,
		};

		AccountID		PlayerID;
		FacebookUID		FBUID;
		char			NickName[MAX_NAME];
		UINT64			LastActiveTime;
		UINT32			Level;
		UINT32			IsPlayingGame;

		inline PlayerInformation();
		inline PlayerInformation(const PlayerInformation& src);
		inline PlayerInformation(AccountID playerID, FacebookUID fbUID, const char* nickName, UINT level, BYTE isPlayingGame, UINT64 lastActiveTime);
		inline HRESULT InitPlayerInformation(AccountID playerID, FacebookUID fbUID, const char* nickName, UINT level, BYTE isPlayingGame, UINT64 lastActiveTime);
		inline PlayerInformation& operator = ( const PlayerInformation& src );
		inline bool operator == ( const PlayerInformation& src ) const;
	};
	

	struct RankingPlayerInformation : public PlayerInformation
	{
		UINT32			WeeklyWin;
		UINT32			WeeklyLose;

		inline RankingPlayerInformation();
		inline RankingPlayerInformation(const RankingPlayerInformation& src);
		inline RankingPlayerInformation(AccountID playerID, FacebookUID fbUID, const char* nickName, UINT level, UINT weeklyWin, UINT weeklyLose, BYTE isPlayingGame, UINT64 lastActiveTime);
		inline RankingPlayerInformation& operator = ( const RankingPlayerInformation& src );
		inline bool operator == ( const RankingPlayerInformation& src ) const;
	};


	struct FriendInformation : public RankingPlayerInformation
	{
		UINT64			LastStaminaSent;

		inline FriendInformation();
		inline FriendInformation(const FriendInformation& src);
		inline FriendInformation(AccountID playerID, FacebookUID fbUID, const char* nickName, UINT level, UINT weeklyWin, UINT weeklyLose, BYTE isPlayingGame, UINT64 lastActiveTime, UINT64 LastStaminaSent);
		inline FriendInformation& operator = ( const FriendInformation& src );
		inline bool operator == ( const FriendInformation& src ) const;
	};

	struct ServerFriendInformation : public FriendInformation
	{
		INT ShardID;

		ServerFriendInformation() : ShardID(0) {}
		ServerFriendInformation(const ServerFriendInformation& src) : FriendInformation(src), ShardID(src.ShardID) {}
		ServerFriendInformation(AccountID playerID, INT shardID, FacebookUID fbUID, const char* nickName, UINT level, UINT weeklyWin, UINT weeklyLose, BYTE isPlayingGame, UINT64 lastActiveTime, UINT64 LastStaminaSent)
			: FriendInformation(playerID, fbUID, nickName, level, weeklyWin, weeklyLose, isPlayingGame, lastActiveTime, LastStaminaSent)
			, ShardID(shardID)
		{
		}
		ServerFriendInformation& operator = (const ServerFriendInformation& src)
		{
			FriendInformation::operator = (src);
			ShardID = src.ShardID;
			return *this;
		}
		bool operator == (const ServerFriendInformation& src) const { return FriendInformation::operator ==(src); }
	};



	struct TotalRankingPlayerInformation
	{
		enum {
			MAX_NAME = 64,
		};

		UINT32			RankingID;
		UINT32			Ranking;
		AccountID		PlayerID;
		FacebookUID		FBUID;
		char			NickName[MAX_NAME];
		UINT32			Level;
		UINT32			Win;
		UINT32			Lose;

		inline TotalRankingPlayerInformation();
		inline TotalRankingPlayerInformation(const TotalRankingPlayerInformation& src);
		inline TotalRankingPlayerInformation(UINT32 rankingID, UINT32 ranking, AccountID playerID, FacebookUID fbUID, const char* nickName, UINT level, UINT win, UINT lose);
		inline TotalRankingPlayerInformation& operator = ( const TotalRankingPlayerInformation& src );
		inline bool operator == ( const TotalRankingPlayerInformation& src ) const;
	};


#pragma pack(pop)

#include "BrCommonTypes.inl"


};


