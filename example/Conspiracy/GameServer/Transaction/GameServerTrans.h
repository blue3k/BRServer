////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : Entity server implementation
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Transaction/ServerTransaction.h"
#include "MemoryManager/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameNetPolicy.h"
#include "GameServerClass.h"


namespace SF {
namespace GameServer {

	
	class GameServerStartProcess : public Svr::TransactionT<GameServer>
	{
	private:
		typedef Svr::TransactionT<GameServer> super;

		enum class StartingStep 
		{
			WaitEntityServer,
			WaitInitializeComponents,
			Done
		};

		StartingStep m_Step;

	public:
		GameServerStartProcess(IHeap& heap);
		~GameServerStartProcess();

		// Override delete function
		virtual void Release();

		Result OnTimer(Svr::TransactionResult* pRes);

		Result InitializeServices();

		virtual Result OnCloseTransaction( Result hrRes ) override;

		// Start Transaction
		virtual Result StartTransaction() override;
	};





}// namespace GameServer 
}// namespace SF 

