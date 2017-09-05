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
#include "ServerEntity/ServerEntity.h"
#include "Transaction/MessageRoute.h"


namespace SF {

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
		EntityServerEntity( uint uiTransQueueSize = 0, uint TransResQueueSize = 0);
		virtual ~EntityServerEntity();



		// Initialize entity to proceed new connection
		Result InitializeEntity( EntityID newEntityID );


		// Process Connection event
		virtual Result ProcessConnectionEvent( const Net::ConnectionEvent& conEvent );
	};




}; // namespace Svr
}; // namespace SF




