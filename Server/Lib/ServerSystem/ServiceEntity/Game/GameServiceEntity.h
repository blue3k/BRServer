////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Component class definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"

#include "Memory/SFMemory.h"
#include "Types/BrSvrTypes.h"
#include "Container/SFPageQueue.h"
#include "Util/LocalUIDGenerator.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Transaction/MessageRoute.h"
#include "Container/SFHashTable.h"
#include "Container/SFIndexing.h"

#include "Entity/EntityInformation.h"
#include "ServiceEntity/ClusteredServiceEntity.h"




namespace SF {
	namespace Net {
		class Connection;
		class ServerMUDP;
	};

	namespace Policy {
		class ISvrPolicyGame;
	};
};



namespace SF {
namespace Svr {

	class Entity;
	class ServerEntity;



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ServerServiceComponentEntity class
	//

	class GameServiceEntity : public ShardedClusterServiceEntity
	{
	public:

		typedef ShardedClusterServiceEntity super;

	private:
		const ServerConfig::NetPublic*	m_PublicNet;
		SharedPointerT<Net::ServerMUDP>			m_pNetPublic;
		GameID						m_GameID;

		PageQueue<SharedPointerAtomicT<Net::Connection>> m_NewConnectionQueue;

	protected:


	public:

		GameServiceEntity(GameID gameID, const ServerConfig::NetPublic *publicNetSocket, ClusterMembership initialMembership = ClusterMembership::Slave);
		~GameServiceEntity();

		// We are not going to use hashed key
		virtual uint KeyHash( uint64_t key ) { return (uint)key; }
		
		Net::ServerMUDP* GetServerNet() { return *m_pNetPublic; }
		const ServerConfig::NetPublic* GetPublicNetConfig() { return m_PublicNet;  }

		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity operations
		//

		Result InitializeEntity( EntityID newEntityID );

		// clear transaction
		virtual Result ClearEntity();

		// TickUpdate 
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;


		//////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		// Process network event
		Result ProcessNewConnection();
		
	};






}; // namespace Svr
}; // namespace SF




