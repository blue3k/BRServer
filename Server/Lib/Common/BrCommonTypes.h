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
		MyTownHero,				// MyTownHero
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
		uint64_t			LastActiveTime;
		uint32_t			Level;
		uint32_t			IsPlayingGame;

		inline PlayerInformation();
		inline PlayerInformation(const PlayerInformation& src);
		inline PlayerInformation(AccountID playerID, FacebookUID fbUID, const char* nickName, UINT level, BYTE isPlayingGame, uint64_t lastActiveTime);
		inline Result InitPlayerInformation(AccountID playerID, FacebookUID fbUID, const char* nickName, UINT level, BYTE isPlayingGame, uint64_t lastActiveTime);
		inline PlayerInformation& operator = ( const PlayerInformation& src );
		inline bool operator == ( const PlayerInformation& src ) const;
	};
	

	struct RankingPlayerInformation : public PlayerInformation
	{
		uint32_t			WeeklyWin;
		uint32_t			WeeklyLose;

		inline RankingPlayerInformation();
		inline RankingPlayerInformation(const RankingPlayerInformation& src);
		inline RankingPlayerInformation(AccountID playerID, FacebookUID fbUID, const char* nickName, UINT level, UINT weeklyWin, UINT weeklyLose, BYTE isPlayingGame, uint64_t lastActiveTime);
		inline RankingPlayerInformation& operator = ( const RankingPlayerInformation& src );
		inline bool operator == ( const RankingPlayerInformation& src ) const;
	};


	struct FriendInformation : public RankingPlayerInformation
	{
		uint64_t			LastStaminaSent;

		inline FriendInformation();
		inline FriendInformation(const FriendInformation& src);
		inline FriendInformation(AccountID playerID, FacebookUID fbUID, const char* nickName, UINT level, UINT weeklyWin, UINT weeklyLose, BYTE isPlayingGame, uint64_t lastActiveTime, uint64_t LastStaminaSent);
		inline FriendInformation& operator = ( const FriendInformation& src );
		inline bool operator == ( const FriendInformation& src ) const;
	};

	struct ServerFriendInformation : public FriendInformation
	{
		int32_t ShardID;

		ServerFriendInformation() : ShardID(0) {}
		ServerFriendInformation(const ServerFriendInformation& src) : FriendInformation(src), ShardID(src.ShardID) {}
		ServerFriendInformation(AccountID playerID, int32_t shardID, FacebookUID fbUID, const char* nickName, UINT level, UINT weeklyWin, UINT weeklyLose, BYTE isPlayingGame, uint64_t lastActiveTime, uint64_t LastStaminaSent)
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

		uint32_t		RankingID;
		uint32_t		Ranking;
		AccountID		PlayerID;
		FacebookUID		FBUID;
		char			NickName[MAX_NAME];
		uint32_t		Level;
		uint32_t		Win;
		uint32_t		Lose;

		inline TotalRankingPlayerInformation();
		inline TotalRankingPlayerInformation(const TotalRankingPlayerInformation& src);
		inline TotalRankingPlayerInformation(uint32_t rankingID, uint32_t ranking, AccountID playerID, FacebookUID fbUID, const char* nickName, UINT level, UINT win, UINT lose);
		inline TotalRankingPlayerInformation& operator = ( const TotalRankingPlayerInformation& src );
		inline bool operator == ( const TotalRankingPlayerInformation& src ) const;
	};


#pragma pack(pop)

#include "BrCommonTypes.inl"


};


