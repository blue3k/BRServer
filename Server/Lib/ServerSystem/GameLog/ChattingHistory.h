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

#include "String/StrUtil.h"
#include "Common/ClassUtil.h"
#include "Memory/SFMemory.h"
#include "Types/BrGameTypes.h"
#include "Common/GameConst.h"
#include "Common/CircularQueue.h"
#include "Common/Binary.h"
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

		// Initialize 
		virtual Result InitializeLog() override;

		// Terminate
		virtual void TerminateLog() override;

		// Add chatting log
		virtual Result AddChattingLog(TimeStampSec timeStamp, PlayerID player, uint8_t playerStatus, ChatType type, const char* message );

		//// Get chatting binary
		//Result GetChattingBinary( OutputMemoryStream& stream, UINT startIndex, UINT count );

		// 
	};


//#include "GameLog.inl"

}; // namespace SF
}; // namespace Svr





