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
#include "Multithread/SFThread.h"
#include "Net/SFNetDef.h"
#include "ServerEntity/ServerEntity.h"



namespace SF {

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
}; // namespace SF




