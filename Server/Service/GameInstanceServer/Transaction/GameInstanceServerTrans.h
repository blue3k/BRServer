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

#include "Common/Typedefs.h"
#include "ServerSystem/ServerTransaction.h"
#include "Common/MemoryPool.h"
#include "Common/BRBaseTypes.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameIPolicy.h"
#include "ConspiracyGameInstanceServerClass.h"


namespace BR {
namespace ConspiracyGameInstanceServer {

	
	class GameInstanceServerStartProcess : public Svr::TransactionT<GameInstanceServer,GameInstanceServerStartProcess>
	{
	private:

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

		HRESULT OnTimer(Svr::TransactionResult* pRes);

		HRESULT InitializeServices();

		virtual HRESULT OnCloseTransaction( HRESULT hrRes ) override;

		// Start Transaction
		virtual HRESULT StartTransaction();
	};





};// namespace ConspiracyGameInstanceServer 
};// namespace BR 

