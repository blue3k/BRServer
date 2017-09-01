////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game server entity
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"
#include "Thread/Thread.h"
#include "Net/NetDef.h"
#include "ServerSystem/ServerEntity.h"



namespace BR {

	namespace Policy
	{
		class IPolicyGameServer;
	};

namespace Svr {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Game server entity entity class
	//

	class GameServerEntity : public ServerEntity
	{
	private:
		
	protected:


	public:
		GameServerEntity();
		virtual ~GameServerEntity();

		// Initialize entity to proceed new connection
		Result InitializeEntity( EntityID newEntityID );

	};




}; // namespace Svr
}; // namespace BR




