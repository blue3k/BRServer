////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : KyungKun Ko
//
// Description : Game master server Transactions
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "ServerSystem/Transaction.h"
#include "Common/MemoryPool.h"

#include "Protocol/Message/GameMasterServerMsgClass.h"
#include "Protocol/Policy/GameMasterServerIPolicy.h"



namespace BR {
namespace GameMasterServer {

	class GameServerEntity;



	class TransS2SEvtPlayerEntered : public Svr::MessageTransaction< GameServerEntity, Policy::ISvrPolicyGameMasterServer, Message::GameMasterServer::PlayerEnteredC2SEvt, TransS2SEvtPlayerEntered >
	{
	private:

	public:
		TransS2SEvtPlayerEntered( Message::MessageData* &pIMsg ) :MessageTransaction( pIMsg ) {}
		virtual ~TransS2SEvtPlayerEntered() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};


	class TransS2SEvtPlayerLeaved : public Svr::MessageTransaction< GameServerEntity, Policy::ISvrPolicyGameMasterServer, Message::GameMasterServer::PlayerLeavedC2SEvt, TransS2SEvtPlayerLeaved >
	{
	private:

	public:
		TransS2SEvtPlayerLeaved( Message::MessageData* &pIMsg ) :MessageTransaction( pIMsg ) {}
		virtual ~TransS2SEvtPlayerLeaved() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};







};// namespace WorldMasterServer 
};// namespace BR 

