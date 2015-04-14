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

#include "Common/StrUtil.h"
#include "Common/ClassUtil.h"
#include "Common/Memory.h"
#include "Common/BrGameTypes.h"
#include "Common/GameConst.h"
#include "Common/CircularQueue.h"
#include "Common/Binary.h"
#include "ServerSystem/SvrTypes.h"
#include "ServerSystem/GameLog/GameLog.h"





namespace BR {

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
		virtual HRESULT InitializeLog() override;

		// Terminate
		virtual void TerminateLog() override;

		// Add chatting log
		virtual HRESULT AddChattingLog( ULONG timeStamp, PlayerID player, BYTE playerStatus, ChatType type, const char* message );

		//// Get chatting binary
		//HRESULT GetChattingBinary( OutputMemoryStream& stream, UINT startIndex, UINT count );

		// 
	};


//#include "GameLog.inl"

}; // namespace BR
}; // namespace Svr





