
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
//
// Description : DB
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
	

#include "SFTypedefs.h"
#include "DB/DBClusterManager.h"
#include "Types/BrSvrTypes.h"
#include "Container/SFArray.h"
#include "Types/BrGameTypes.h"
#include "Component/ServerComponent.h"

namespace SF {
namespace DB {

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	GameDB Class 
	//

	class LoginSessionDB : public DBClusterManager
	{
	public:

		enum {
			ComponentID = Svr::ServerComponentID_LoginSessionDB,
		};

	private:

	public:
		// constructor / destructor
		LoginSessionDB();
		virtual ~LoginSessionDB();

		Result InitializeComponent() { return ResultCode::SUCCESS; }
		virtual void TerminateComponent() ;

	public:

		/////////////////////////////////////////////////////////////////////////////////
		//
		//	DB Interface
		//

		// Register authenticate ticket
		Result RegisterAuthTicket( TransactionID Sender, const PlayerID &playerID, const AuthTicket& authTicket, const EntityUID& loginEntityUID );

		Result ReplaceLoginSession(TransactionID Sender, const PlayerID &playerID, const AuthTicket& oldAuthTicket, const AuthTicket& authTicket, const EntityUID& loginEntityUID);

		Result DeleteLoginSession(TransactionID Sender, const PlayerID &playerID, const AuthTicket& authTicket);

		// Joined game server
		Result ConnectedToGameServer( TransactionID Sender, const PlayerID &playerID, const AuthTicket& authTicket, const EntityUID& loginEntityUID, const EntityUID& gameEntityUID );

		// Validate game server session
		Result ValidateGameServerSession(TransactionID Sender, const PlayerID &playerID, const AuthTicket& authTicket, const EntityUID& gameEntityUID);

		// Game server heartbeat
		Result GameServerHeartbeat( TransactionID Sender, const PlayerID &playerID, const AuthTicket& authTicket, const EntityUID& gameEntityUID );


	};

} // namespace DB
}  // namespace SF
