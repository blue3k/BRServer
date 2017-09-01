////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game master server
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
		class IPolicyGameMasterServer;
	};

namespace Svr
{

	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Zone server entity class
	//

	class GameMasterServerEntity : public ServerEntity
	{
	public:


	private:

		StaticAllocator< sizeof(EntityMessageHandlerItem)*20 > m_Allocator;

	protected:

		virtual MemoryAllocator& GetAllocator()			{ return m_Allocator; }

	public:
		GameMasterServerEntity( UINT uiTransQueueSize, UINT TransResQueueSize );
		virtual ~GameMasterServerEntity();


		// Get Server IPolicy
		Policy::IPolicyGameMasterServer* GetPolicy();

		// Initialize entity to proceed new connection
		Result InitializeEntity( EntityID newEntityID );

	};




}; // namespace Svr
}; // namespace BR




