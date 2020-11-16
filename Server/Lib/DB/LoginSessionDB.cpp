
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
//
// Description : DB
//
////////////////////////////////////////////////////////////////////////////////

#include "DBPch.h"
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
		Result LoginSessionDB::RegisterAuthTicket(TransactionID Sender, const PlayerID& playerID, const AuthTicket& authTicket, const EntityUID& loginEntityUID)
		{
			ScopeContext hr;

			UniquePtr<QueryRegisterAuthTicketCmd> pQuery(new(GetHeap()) QueryRegisterAuthTicketCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey((uint)playerID);

			pQuery->PlayerID = playerID;
			pQuery->AuthTicket = authTicket;
			pQuery->LoginEntityUID = (int64_t)(Context)loginEntityUID;
			pQuery->GameEntityUID = 0;
			//		pQuery->HeartbitTime = 0;
			pQuery->Result = 0;

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));

			return hr;
		}

		// Register authenticate ticket
		Result LoginSessionDB::ReplaceLoginSession(TransactionID Sender, const PlayerID& playerID, const AuthTicket& oldAuthTicket, const AuthTicket& authTicket, const EntityUID& loginEntityUID)
		{
			ScopeContext hr;

			UniquePtr<QueryReplaceLoginSessionCmd> pQuery(new(GetHeap()) QueryReplaceLoginSessionCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey((uint)playerID);

			pQuery->PlayerID = playerID;
			pQuery->OldAuthTicket = oldAuthTicket;
			pQuery->AuthTicket = authTicket;
			pQuery->LoginEntityUID = (int64_t)loginEntityUID.UID;
			pQuery->Result = 0;

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));

			return hr;
		}


		// Register authenticate ticket
		Result LoginSessionDB::DeleteLoginSession(TransactionID Sender, const PlayerID& playerID, const AuthTicket& authTicket)
		{
			ScopeContext hr;

			UniquePtr<QueryDeleteLoginSessionCmd> pQuery(new(GetHeap()) QueryDeleteLoginSessionCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey((uint)playerID);


			pQuery->PlayerID = playerID;
			pQuery->AuthTicket = authTicket;
			pQuery->Result = 0;

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));

			return hr;
		}

		// Joined game server
		Result LoginSessionDB::ConnectedToGameServer(TransactionID Sender, const PlayerID& playerID, const AuthTicket& authTicket, const EntityUID& loginEntityUID, const EntityUID& gameEntityUID)
		{
			ScopeContext hr;

			UniquePtr<QueryConnectedToGameServerCmd> pQuery(new(GetHeap()) QueryConnectedToGameServerCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey((uint)playerID);

			pQuery->PlayerID = playerID;
			pQuery->AuthTicket = authTicket;
			pQuery->LoginEntityUID = (int64_t)(Context)loginEntityUID;
			pQuery->GameEntityUID = (int64_t)(Context)gameEntityUID;
			pQuery->Result = 0;

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));

			return hr;
		}


		// Validate game server session
		Result LoginSessionDB::ValidateGameServerSession(TransactionID Sender, const PlayerID& playerID, const AuthTicket& authTicket, const EntityUID& gameEntityUID)
		{
			ScopeContext hr;

			UniquePtr<QueryValidateGameServerSessionCmd> pQuery(new(GetHeap()) QueryValidateGameServerSessionCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey((uint)playerID);

			pQuery->PlayerID = playerID;
			pQuery->AuthTicket = authTicket;
			pQuery->GameEntityUID = (int64_t)(Context)gameEntityUID;
			pQuery->Result = 0;

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));

			return hr;
		}

		// Game server heartbit
		Result LoginSessionDB::GameServerHeartBit(TransactionID Sender, const PlayerID& playerID, const AuthTicket& authTicket, const EntityUID& gameEntityUID)
		{
			ScopeContext hr;

			UniquePtr<QueryGameServerHeartBitCmd> pQuery(new(GetHeap()) QueryGameServerHeartBitCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey((uint)playerID);

			pQuery->PlayerID = playerID;
			pQuery->AuthTicket = authTicket;
			pQuery->GameEntityUID = (int64_t)(Context)gameEntityUID;
			pQuery->Result = 0;

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));

			return hr;
		}




	} //namespace DB
} // namespace SF

