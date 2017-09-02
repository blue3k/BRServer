
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
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
#include "ServerSystem/ServerComponent.h"

namespace SF {
namespace DB {

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	GameDB Class 
	//

	class LoginSessionDB : private DBClusterManager, public Svr::IServerComponent
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

		virtual void TerminateComponent() override;

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

		// Game server heartbit
		Result GameServerHeartBit( TransactionID Sender, const PlayerID &playerID, const AuthTicket& authTicket, const EntityUID& gameEntityUID );


	};

} // namespace DB
}  // namespace SF
