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
#include "ServerSystem/ServerTransaction.h"
#include "Memory/MemoryPool.h"
#include "Types/BrBaseTypes.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameIPolicy.h"
#include "ConspiracyGameInstanceServerClass.h"


namespace BR {
namespace ConspiracyGameInstanceServer {

	
	class GameInstanceServerStartProcess : public Svr::TransactionT<GameInstanceServer,GameInstanceServerStartProcess>
	{
	private:
		typedef Svr::TransactionT<GameInstanceServer, GameInstanceServerStartProcess> super;

		enum class StartingStep 
		{
			WaitEntityServer,
			WaitInitializeComponents,
			Done
		};

		StartingStep m_Step;

	public:
		GameInstanceServerStartProcess();
		~GameInstanceServerStartProcess();

		// Override delete function
		virtual void Release();

		Result OnTimer(Svr::TransactionResult* pRes);

		Result InitializeServices();

		virtual Result OnCloseTransaction( Result hrRes ) override;

		// Start Transaction
		virtual Result StartTransaction();
	};





};// namespace ConspiracyGameInstanceServer 
};// namespace BR 

