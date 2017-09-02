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


	enum  {
		MAX_GCMKEYS		= 512,
		MAX_NAME		= 64,
		MAX_EMAIL		= 128,
		MAX_GAMEPLAYER	= 8,

		MAX_COMPLITIONSTATE = 16,

		MAX_GAMELOG			= 100,
		MAX_GAMELOG_BUFFER	= 1024,
		MAX_CHATMESSAGE_SIZE= 1024,
		MAX_CHATLOG			= 300,
		MAX_CHATLOG_BUFFER	= 1024,

		MAX_CHATSYNC		= 40,

		AI_VOTE_TIMEOUT	= 60*1000,

		MAX_RANKING_QUERY_COUNT = 20,

		PLAYER_UPDATE_STATUS_TIME = 30 * 1000,

		MAX_PLAYER_REVEAL = 1,

		MATCHING_TIMEOUT = 60*1000,
	};


};// namespace Const
};// namespace SF


