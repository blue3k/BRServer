////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves.
// 
// Author :MadK
//
// Description : Game Const Value definitions. 
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace SF {
namespace GameConst {

	namespace Chat
	{
		enum ChatType
		{
			SAY,
			TEAM,
			WHISPER,
			NOTICE,
		};
	};	// namespace Chat


	constexpr int MAX_GCMKEYS		= 512;
	constexpr int MAX_NAME		= 64;
	constexpr int MAX_EMAIL		= 128;
	constexpr int MAX_GAMEPLAYER	= 8;

	constexpr int MAX_COMPLITIONSTATE = 16;

	constexpr int MAX_GAMELOG			= 100;
	constexpr int MAX_GAMELOG_BUFFER	= 1024;
	constexpr int MAX_CHATMESSAGE_SIZE= 1024;
	constexpr int MAX_CHATLOG			= 300;
	constexpr int MAX_CHATLOG_BUFFER	= 1024;

	constexpr int MAX_CHATSYNC		= 40;

	constexpr int AI_VOTE_TIMEOUT	= 60*1000;

	constexpr int MAX_RANKING_QUERY_COUNT = 20;

	constexpr int PLAYER_UPDATE_STATUS_TIME = 30 * 1000;

	constexpr int MAX_PLAYER_REVEAL = 1;

	constexpr int MATCHING_TIMEOUT = 60*1000;


};// namespace Const
};// namespace SF


