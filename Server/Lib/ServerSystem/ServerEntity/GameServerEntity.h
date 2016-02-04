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


#include "Common/Typedefs.h"
#include "Common/Thread.h"
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
		GameServerEntity( UINT uiTransQueueSize, UINT TransResQueueSize );
		virtual ~GameServerEntity();

		// Initialize entity to proceed new connection
		HRESULT InitializeEntity( EntityID newEntityID );

	};




}; // namespace Svr
}; // namespace BR




