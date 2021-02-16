////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : Component class definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"

#include "MemoryManager/SFMemory.h"
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
#include "ServiceEntity/ServiceEntity.h"




namespace SF {
	namespace Net {
		class Connection;
		class ServerNet;
	};

	class ISvrPolicyGame;
};



namespace SF {

	class Entity;
	class ServerEntity;



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ServerServiceComponentEntity class
	//

	class GameServiceEntity : public ServiceEntity
	{
	public:

		using super = ServiceEntity;

	private:
		const ServerConfig::NetPublic* m_PublicNetConfig{};
		SharedPointerT<Net::ServerNet>	m_pNetPublic;

		PageQueue<SharedPointerAtomicT<Net::Connection>> m_NewConnectionQueue;

	protected:


	public:

		GameServiceEntity(GameID gameID, const ServerConfig::NetPublic *publicNetSocket, const EndpointAddress& endpoint);
		~GameServiceEntity();

		// We are not going to use hashed key
		virtual uint KeyHash( uint64_t key ) { return (uint)key; }
		
		Net::ServerNet* GetServerNet() { return *m_pNetPublic; }
		const ServerConfig::NetPublic* GetPublicNetConfig() { return m_PublicNetConfig;  }

		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity operations
		//

		virtual Result InitializeEntity( EntityID newEntityID ) override;

		// clear transaction
		virtual Result ClearEntity() override;

		// TickUpdate 
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;


		//////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		// Process network event
		Result ProcessNewConnection();
		
	};

} // namespace SF




