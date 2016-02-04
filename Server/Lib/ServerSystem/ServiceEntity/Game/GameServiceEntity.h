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


#include "Common/Typedefs.h"
#include "Common/ClassUtil.h"
#include "Common/Memory.h"
#include "Common/BrSvrTypes.h"
#include "Common/PageQueue.h"
#include "Common/LocalUIDGenerator.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/MessageRoute.h"
#include "Common/HashTable.h"
#include "Common/Indexing.h"
#include "Common/StaticHashTable.h"

#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServiceEntity/ClusteredServiceEntity.h"




namespace BR {
	namespace Net {
		class Connection;
		class ServerMUDP;
	};

	namespace Policy {
		class ISvrPolicyGame;
	};
};



namespace BR {
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
		Config::PublicNetSocket*	m_PublicNetSocket;
		Net::ServerMUDP*			m_pNetPublic;

	protected:


	public:

		GameServiceEntity(Config::PublicNetSocket *publicNetSocket, ClusterMembership initialMembership = ClusterMembership::StatusWatcher );
		~GameServiceEntity();

		// We are not going to use hashed key
		virtual UINT KeyHash( UINT64 key ) { return (UINT)key; }
		

		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity operations
		//

		HRESULT InitializeEntity( EntityID newEntityID );

		// clear transaction
		virtual HRESULT ClearEntity();

		// TickUpdate 
		virtual HRESULT TickUpdate(Svr::TimerAction *pAction = nullptr) override;


		//////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		// Process network event
		HRESULT ProcessPublicNetworkEvent();
		
	};






}; // namespace Svr
}; // namespace BR




