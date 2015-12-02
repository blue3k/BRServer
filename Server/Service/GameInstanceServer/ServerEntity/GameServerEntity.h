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
#include "ServerSystem/ServerEntity/GameServerEntity.h"


namespace BR {
namespace ConspiracyGameInstanceServer {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Game server entity entity class
	//

	class GameServerEntity : public Svr::GameServerEntity
	{
	private:
		typedef Svr::GameServerEntity super;

	public:
		GameServerEntity();
		virtual ~GameServerEntity();


		// Initialize entity to proceed new connection
		virtual HRESULT InitializeEntity( EntityID newEntityID ) override;

		// Process Connection event
		virtual HRESULT ProcessConnectionEvent( const BR::Net::IConnection::Event& conEvent ) override;

		// register message handlers
		virtual HRESULT RegisterMessageHandlers() override;

	};


}; // namespace ConspiracyGameInstanceServer
}; // namespace BR




