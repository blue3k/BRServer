////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : GameLog definition
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "String/SFStrUtil.h"

#include "Memory/SFMemory.h"
#include "Types/BrGameTypes.h"
#include "GameConst.h"
#include "Container/SFCircularQueue.h"
#include "Types/BrSvrTypes.h"
#include "ServerSystem/GameLog/GameLog.h"





namespace SF {

	class OutputMemoryStream;


namespace Svr {



	//////////////////////////////////////////////////////////////////////////
	//
	//	Game log class
	//


	class ChattingHistory : public GameLog<GameConst::MAX_CHATLOG_BUFFER,GameConst::MAX_CHATLOG>
	{
	public:

	private:

	protected:


	public:
		ChattingHistory(IHeap& heap) : GameLog(heap) {}

		// Initialize 
		virtual Result InitializeLog() override;

		// Terminate
		virtual void TerminateLog() override;

		// Add chatting log
		virtual Result AddChattingLog(TimeStampSec timeStamp, PlayerID player, uint8_t playerStatus, ChatType type, const char* message );

		//// Get chatting binary
		//Result GetChattingBinary( OutputMemoryStream& stream, uint startIndex, uint count );

		// 
	};


//#include "GameLog.inl"

}; // namespace SF
}; // namespace Svr





