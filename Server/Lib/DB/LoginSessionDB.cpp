
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : DB
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SFTypedefs.h"

#include "DB/LoginSessionDB.h"
#include "DB/LoginSessionQuery.h"



namespace SF {
namespace DB {

	
	//////////////////////////////////////////////////////////////////////////////////
	//
	//	LoginSessionDB Class 
	//
	
	// constructor / destructor
	LoginSessionDB::LoginSessionDB()
	{
	}

	LoginSessionDB::~LoginSessionDB()
	{
	}

	void LoginSessionDB::TerminateComponent()
	{
		TerminateDB();
	}


	
	/////////////////////////////////////////////////////////////////////////////////
	//
	//	DB Interface
	//

	// Register authenticate ticket
	Result LoginSessionDB::RegisterAuthTicket( TransactionID Sender, const PlayerID &playerID, const AuthTicket& authTicket, const EntityUID& loginEntityUID )
	{
		Result hr = ResultCode::SUCCESS;
		QueryRegisterAuthTicketCmd *pQuery = nullptr;

		dbMem( pQuery = new(GetMemoryManager()) QueryRegisterAuthTicketCmd );

		pQuery->SetPartitioningKey((uint)playerID);

		pQuery->PlayerID = playerID;
		pQuery->AuthTicket = authTicket;
		pQuery->LoginEntityUID = (int64_t)(Context)loginEntityUID;
		pQuery->GameEntityUID = 0;
//		pQuery->HeartbitTime = 0;
		pQuery->Result = 0;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}
	
	// Register authenticate ticket
	Result LoginSessionDB::ReplaceLoginSession(TransactionID Sender, const PlayerID &playerID, const AuthTicket& oldAuthTicket, const AuthTicket& authTicket, const EntityUID& loginEntityUID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryReplaceLoginSessionCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetMemoryManager()) QueryReplaceLoginSessionCmd);

		pQuery->SetPartitioningKey((uint)playerID);

		pQuery->PlayerID = playerID;
		pQuery->OldAuthTicket = oldAuthTicket;
		pQuery->AuthTicket = authTicket;
		pQuery->LoginEntityUID = (int64_t)loginEntityUID.UID;
		pQuery->Result = 0;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}


	// Register authenticate ticket
	Result LoginSessionDB::DeleteLoginSession( TransactionID Sender, const PlayerID &playerID, const AuthTicket& authTicket )
	{
		Result hr = ResultCode::SUCCESS;
		QueryDeleteLoginSessionCmd *pQuery = nullptr;

		dbMem( pQuery = new(GetMemoryManager()) QueryDeleteLoginSessionCmd );

		pQuery->SetPartitioningKey((uint)playerID);


		pQuery->PlayerID = playerID;
		pQuery->AuthTicket = authTicket;
		pQuery->Result = 0;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	// Joined game server
	Result LoginSessionDB::ConnectedToGameServer( TransactionID Sender, const PlayerID &playerID, const AuthTicket& authTicket, const EntityUID& loginEntityUID, const EntityUID& gameEntityUID )
	{
		Result hr = ResultCode::SUCCESS;
		QueryConnectedToGameServerCmd *pQuery = nullptr;

		dbMem( pQuery = new(GetMemoryManager()) QueryConnectedToGameServerCmd );

		pQuery->SetPartitioningKey((uint)playerID);

		pQuery->PlayerID = playerID;
		pQuery->AuthTicket = authTicket;
		pQuery->LoginEntityUID = (int64_t)(Context)loginEntityUID;
		pQuery->GameEntityUID = (int64_t)(Context)gameEntityUID;
		pQuery->Result = 0;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}


	// Validate game server session
	Result LoginSessionDB::ValidateGameServerSession(TransactionID Sender, const PlayerID &playerID, const AuthTicket& authTicket, const EntityUID& gameEntityUID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryValidateGameServerSessionCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetMemoryManager()) QueryValidateGameServerSessionCmd);

		pQuery->SetPartitioningKey((uint)playerID);

		pQuery->PlayerID = playerID;
		pQuery->AuthTicket = authTicket;
		pQuery->GameEntityUID = (int64_t)(Context)gameEntityUID;
		pQuery->Result = 0;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	// Game server heartbit
	Result LoginSessionDB::GameServerHeartBit( TransactionID Sender, const PlayerID &playerID, const AuthTicket& authTicket, const EntityUID& gameEntityUID )
	{
		Result hr = ResultCode::SUCCESS;
		QueryGameServerHeartBitCmd *pQuery = nullptr;

		dbMem( pQuery = new(GetMemoryManager()) QueryGameServerHeartBitCmd );

		pQuery->SetPartitioningKey((uint)playerID);

		pQuery->PlayerID = playerID;
		pQuery->AuthTicket = authTicket;
		pQuery->GameEntityUID = (int64_t)(Context)gameEntityUID;
		pQuery->Result = 0;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	


} //namespace DB
} // namespace SF

