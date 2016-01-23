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
#include "Common/GameConst.h"




namespace BR {
namespace Svr {


	
	// Initialize 
	HRESULT ChattingHistory::InitializeLog()
	{
		return GameLog<GameConst::MAX_CHATLOG_BUFFER, GameConst::MAX_CHATLOG>::InitializeLog();
	}

	// Terminate
	void ChattingHistory::TerminateLog()
	{
		GameLog<GameConst::MAX_CHATLOG_BUFFER, GameConst::MAX_CHATLOG>::TerminateLog();
	}

	// Add chatting log
	HRESULT ChattingHistory::AddChattingLog( TimeStampSec timeStamp, PlayerID player, BYTE playerStatus, ChatType type, const char* message )
	{
		HRESULT hr = S_SYSTEM_OK;
		BYTE* itemBuffer = nullptr;
		GameLogChatMessage *chatLog = nullptr;

		if( message == nullptr )
			return E_SYSTEM_INVALIDARG;

		size_t messageBufferSize = strlen(message) + 1;
		if (messageBufferSize >= GameConst::MAX_CHATMESSAGE_SIZE)
		{
			messageBufferSize = GameConst::MAX_CHATMESSAGE_SIZE;
		}

		itemBuffer = NewLogItemBuffer<GameLogChatMessage>( sizeof(GameLogChatMessage) + messageBufferSize );
		svrMem( itemBuffer );
		chatLog = new(itemBuffer) GameLogChatMessage(timeStamp,(UINT)messageBufferSize);
		svrMem( chatLog );

		svrChk( chatLog->SetChatMessage( player, playerStatus, type, message ) );

		svrChk( AddLogItem( chatLog ) );
		chatLog = nullptr;

	Proc_End:

		return hr;
	}


	
};	// Svr
};	// namespace BR


