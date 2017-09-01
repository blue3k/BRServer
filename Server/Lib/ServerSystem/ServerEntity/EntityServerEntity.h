////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Entity server entity
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"
#include "Thread/Thread.h"
#include "Net/NetDef.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/MessageRoute.h"


namespace BR {

	namespace Policy
	{
		class IPolicyEntityServer;
	};

namespace Svr
{



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity server entity
	//

	class EntityServerEntity : public ServerEntity
	{
	public:

	private:


	protected:



	public:
		EntityServerEntity( UINT uiTransQueueSize = 0, UINT TransResQueueSize = 0);
		virtual ~EntityServerEntity();



		// Initialize entity to proceed new connection
		Result InitializeEntity( EntityID newEntityID );


		// Process Connection event
		virtual Result ProcessConnectionEvent( const BR::Net::ConnectionEvent& conEvent );
	};




}; // namespace Svr
}; // namespace BR




