////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Br common type definitions
//	
//
////////////////////////////////////////////////////////////////////////////////




PlayerInformation::PlayerInformation()
{
	memset(this, 0, sizeof(PlayerInformation));
}

PlayerInformation::PlayerInformation(const PlayerInformation& src)
{
	(*this) = src;
}

PlayerInformation& PlayerInformation::operator = ( const PlayerInformation& src )
{
	this->PlayerID = src.PlayerID;
	this->FBUID = src.FBUID;
	Level = src.Level;
	IsPlayingGame = src.IsPlayingGame;
	LastActiveTime = src.LastActiveTime;

	StrUtil::StringCpy(NickName, src.NickName);

	return *this;
}

PlayerInformation::PlayerInformation(AccountID playerID, FacebookUID fbUID, const char* nickName, UINT level, BYTE isPlayingGame, UINT64 lastActiveTime)
{
	InitPlayerInformation(playerID, fbUID, nickName, level, isPlayingGame, lastActiveTime);
}

Result PlayerInformation::InitPlayerInformation(AccountID playerID, FacebookUID fbUID, const char* nickName, UINT level, BYTE isPlayingGame, UINT64 lastActiveTime)
{
	if( nickName == nullptr ) 
		return ResultCode::INVALID_ARG;

	this->PlayerID = playerID;
	this->FBUID = fbUID;
	Level = level;
	IsPlayingGame = isPlayingGame;
	LastActiveTime = lastActiveTime;

	return StrUtil::StringCpy(NickName, nickName);
}


bool PlayerInformation::operator == ( const PlayerInformation& src ) const
{
	if( PlayerID == src.PlayerID )
	{
		Assert( FBUID == src.FBUID );
		return true;
	}
	return false;
}




RankingPlayerInformation::RankingPlayerInformation()
	:WeeklyWin(0)
	,WeeklyLose(0)
{
}

RankingPlayerInformation::RankingPlayerInformation(const RankingPlayerInformation& src)
	:PlayerInformation(src)
{
	WeeklyWin = src.WeeklyWin;
	WeeklyLose = src.WeeklyLose;
}

RankingPlayerInformation::RankingPlayerInformation(AccountID playerID, FacebookUID fbUID, const char* nickName, UINT level, UINT weeklyWin, UINT weeklyLose, BYTE isPlayingGame, UINT64 lastActiveTime)
	:PlayerInformation(playerID, fbUID, nickName, level, isPlayingGame, lastActiveTime)
{
	WeeklyWin = weeklyWin;
	WeeklyLose = weeklyLose;
}

RankingPlayerInformation& RankingPlayerInformation::operator = ( const RankingPlayerInformation& src )
{
	*(PlayerInformation*)this = src;
	//InitPlayerInformation(src.PlayerID, src.FBUID, src.NickName, src.Level, src.IsPlayingGame, src.LastActiveTime);
	WeeklyWin = src.WeeklyWin;
	WeeklyLose = src.WeeklyLose;
	return *this;
}

bool RankingPlayerInformation::operator == ( const RankingPlayerInformation& src ) const
{
	if( PlayerID == src.PlayerID )
	{
		Assert( FBUID == src.FBUID );
		return true;
	}
	return false;
}




FriendInformation::FriendInformation()
	:LastStaminaSent(0)
{
}

FriendInformation::FriendInformation(const FriendInformation& src)
	:RankingPlayerInformation(src)
{
	LastStaminaSent = src.LastStaminaSent;
}
 
FriendInformation::FriendInformation(AccountID playerID, FacebookUID fbUID, const char* nickName, UINT level, UINT weeklyWin, UINT weeklyLose, BYTE isPlayingGame, UINT64 lastActiveTime, UINT64 lastStaminaSent)
	:RankingPlayerInformation(playerID, fbUID, nickName, level, weeklyWin, weeklyLose, isPlayingGame, lastActiveTime)
{
	LastStaminaSent = lastStaminaSent;
}

FriendInformation& FriendInformation::operator = ( const FriendInformation& src )
{
	//InitPlayerInformation(src.PlayerID, src.FBUID, src.NickName, src.Level, src.weeklyWin, src.weeklyLose, src.IsPlayingGame, src.LastActiveTime);
	*(RankingPlayerInformation*)this = src;
	LastStaminaSent = src.LastStaminaSent;
	return *this;
}

bool FriendInformation::operator == ( const FriendInformation& src ) const
{
	if( PlayerID == src.PlayerID )
	{
		Assert( FBUID == src.FBUID );
		return true;
	}
	return false;
}





TotalRankingPlayerInformation::TotalRankingPlayerInformation()
{
	memset(this, 0, sizeof(TotalRankingPlayerInformation));
}

TotalRankingPlayerInformation::TotalRankingPlayerInformation(const TotalRankingPlayerInformation& src)
{
	(*this) = src;
}

TotalRankingPlayerInformation& TotalRankingPlayerInformation::operator = ( const TotalRankingPlayerInformation& src )
{
	RankingID = src.RankingID;
	Ranking = src.Ranking;
	this->PlayerID = src.PlayerID;
	this->FBUID = src.FBUID;
	Level = src.Level;
	Win = src.Win;
	Lose = src.Lose;

	StrUtil::StringCpy(NickName, src.NickName);

	return *this;
}

TotalRankingPlayerInformation::TotalRankingPlayerInformation(UINT32 rankingID, UINT32 ranking, AccountID playerID, FacebookUID fbUID, const char* nickName, UINT level, UINT win, UINT lose)
{
	//if( nickName == nullptr ) 
	//	return ResultCode::INVALID_ARG;

	RankingID = rankingID;
	Ranking = ranking;
	this->PlayerID = playerID;
	this->FBUID = fbUID;
	Level = level;
	Win = win;
	Lose = lose;

	StrUtil::StringCpy(NickName, nickName);
}

bool TotalRankingPlayerInformation::operator == ( const TotalRankingPlayerInformation& src ) const
{
	if( PlayerID == src.PlayerID )
	{
		Assert( FBUID == src.FBUID );
		return true;
	}
	return false;
}


