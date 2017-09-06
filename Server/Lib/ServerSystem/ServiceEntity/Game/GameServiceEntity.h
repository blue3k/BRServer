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
#include "Container/PageQueue.h"
#include "Util/LocalUIDGenerator.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Transaction/MessageRoute.h"
#include "Container/HashTable.h"
#include "Container/Indexing.h"

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

	class GameServiceEntity : public FreeReplicaClusterServiceEntity
	{
	public:


	private:
		ServerConfig::NetPublic*	m_PublicNetSocket;
		Net::ServerMUDP*			m_pNetPublic;
		GameID						m_GameID;

	protected:


	public:

		GameServiceEntity(GameID gameID, ServerConfig::NetPublic *publicNetSocket, ClusterMembership initialMembership = ClusterMembership::StatusWatcher );
		~GameServiceEntity();

		// We are not going to use hashed key
		virtual uint KeyHash( uint64_t key ) { return (uint)key; }
		

		ServerConfig::NetPublic* GetPublicNetConfig() { return m_PublicNetSocket;  }

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
		Result ProcessPublicNetworkEvent();
		
	};






}; // namespace Svr
}; // namespace SF




