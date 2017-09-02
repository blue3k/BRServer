////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : 
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/MemoryStream.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/GameLog/ChattingHistory.h"
#include "GameConst.h"




namespace SF {
namespace Svr {


	
	// Initialize 
	Result ChattingHistory::InitializeLog()
	{
		return GameLog<GameConst::MAX_CHATLOG_BUFFER, GameConst::MAX_CHATLOG>::InitializeLog();
	}

	// Terminate
	void ChattingHistory::TerminateLog()
	{
		GameLog<GameConst::MAX_CHATLOG_BUFFER, GameConst::MAX_CHATLOG>::TerminateLog();
	}

	// Add chatting log
	Result ChattingHistory::AddChattingLog( TimeStampSec timeStamp, PlayerID player, uint8_t playerStatus, ChatType type, const char* message )
	{
		Result hr = ResultCode::SUCCESS;
		uint8_t* itemBuffer = nullptr;
		GameLogChatMessage *chatLog = nullptr;

		if( message == nullptr )
			return ResultCode::INVALID_ARG;

		size_t messageBufferSize = strlen(message) + 1;
		if (messageBufferSize >= GameConst::MAX_CHATMESSAGE_SIZE)
		{
			messageBufferSize = GameConst::MAX_CHATMESSAGE_SIZE;
		}

		itemBuffer = NewLogItemBuffer<GameLogChatMessage>( sizeof(GameLogChatMessage) + messageBufferSize );
		svrMem( itemBuffer );
		chatLog = new(itemBuffer) GameLogChatMessage(timeStamp,(uint)messageBufferSize);
		svrMem( chatLog );

		svrChk( chatLog->SetChatMessage( player, playerStatus, type, message ) );

		svrChk( AddLogItem( chatLog ) );
		chatLog = nullptr;

	Proc_End:

		return hr;
	}


	
};	// Svr
};	// namespace SF


