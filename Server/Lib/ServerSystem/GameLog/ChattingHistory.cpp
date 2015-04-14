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


#include "StdAfx.h"
#include "Common/MemoryStream.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/GameLog/ChattingHistory.h"
#include "Common/GameConst.h"




namespace BR {
namespace Svr {


	
	// Initialize 
	HRESULT ChattingHistory::InitializeLog()
	{
		return __super::InitializeLog();
	}

	// Terminate
	void ChattingHistory::TerminateLog()
	{
		__super::TerminateLog();
	}

	// Add chatting log
	HRESULT ChattingHistory::AddChattingLog( ULONG timeStamp, PlayerID player, BYTE playerStatus, ChatType type, const char* message )
	{
		HRESULT hr = S_OK;
		if( message == nullptr )
			return E_INVALIDARG;

		size_t messageBufferSize = strlen(message) + 1;
		if (messageBufferSize >= GameConst::MAX_CHATMESSAGE_SIZE)
		{
			messageBufferSize = GameConst::MAX_CHATMESSAGE_SIZE;
		}

		BYTE* itemBuffer = NewLogItemBuffer<GameLogChatMessage>( sizeof(GameLogChatMessage) + messageBufferSize );
		svrMem( itemBuffer );
		GameLogChatMessage *chatLog = new(itemBuffer) GameLogChatMessage(timeStamp,(UINT)messageBufferSize);
		svrMem( chatLog );

		svrChk( chatLog->SetChatMessage( player, playerStatus, type, message ) );

		svrChk( AddLogItem( chatLog ) );
		chatLog = nullptr;

	Proc_End:

		return hr;
	}


	
};	// Svr
};	// namespace BR


