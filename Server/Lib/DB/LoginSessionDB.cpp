
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

			SFUniquePtr<QueryRegisterAuthTicketCmd> pQuery(new(GetHeap()) QueryRegisterAuthTicketCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey((uint)playerID);

			pQuery->PlayerID = playerID;
			pQuery->AuthTicket = authTicket;
			pQuery->LoginEntityUID = (int64_t)(Context)loginEntityUID;
			pQuery->GameEntityUID = 0;
			//		pQuery->HeartbeatTime = 0;
			pQuery->Result = 0;

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));

			return hr;
		}

		// Register authenticate ticket
		Result LoginSessionDB::ReplaceLoginSession(TransactionID Sender, const PlayerID& playerID, const AuthTicket& oldAuthTicket, const AuthTicket& authTicket, const EntityUID& loginEntityUID)
		{
			ScopeContext hr;

			SFUniquePtr<QueryReplaceLoginSessionCmd> pQuery(new(GetHeap()) QueryReplaceLoginSessionCmd(GetHeap()));
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

			SFUniquePtr<QueryDeleteLoginSessionCmd> pQuery(new(GetHeap()) QueryDeleteLoginSessionCmd(GetHeap()));
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

			SFUniquePtr<QueryConnectedToGameServerCmd> pQuery(new(GetHeap()) QueryConnectedToGameServerCmd(GetHeap()));
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

			SFUniquePtr<QueryValidateGameServerSessionCmd> pQuery(new(GetHeap()) QueryValidateGameServerSessionCmd(GetHeap()));
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

		// Game server heartbeat
		Result LoginSessionDB::GameServerHeartbeat(TransactionID Sender, const PlayerID& playerID, const AuthTicket& authTicket, const EntityUID& gameEntityUID)
		{
			ScopeContext hr;

			SFUniquePtr<QueryGameServerHeartbeatCmd> pQuery(new(GetHeap()) QueryGameServerHeartbeatCmd(GetHeap()));
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

