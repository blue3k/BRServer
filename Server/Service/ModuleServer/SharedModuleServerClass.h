////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : Game server class
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Server/BrServer.h"
#include "Entity/Entity.h"
#include "Transaction/Transaction.h"





namespace SF {
namespace Net {
	class ServerMUDP;
}};


namespace SF {
namespace SharedModuleServer
{

	class EntityServerEntity;
	class GameServerEntity;
	class SharedModuleServerEntity;



	//////////////////////////////////////////////////////////////////////////
	//
	//	Game server class
	//

	class SharedModuleServer : public Svr::BrServer
	{
	public:

	private:


	protected:


	public:
		SharedModuleServer();
		~SharedModuleServer();

		///////////////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		// Initialize private Network
		virtual Result InitializeEntities() override;

		// Close Private Network
		virtual Result CloseEntities() override;
	};


	// Query Game server instance
	inline SharedModuleServer* GetMyServer() { return dynamic_cast<SharedModuleServer*>(Svr::BrServer::GetInstance()); }


} // namespace SharedModuleServer
} // namespace SF







