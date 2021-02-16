////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2019 The Braves
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
#include "ServiceEntity/ClusteredServiceEntity.h"
#include "Net/SFNetRawUDP.h"




namespace SF {
	namespace Net {
		class Connection;
		class ServerMUDP;
	};

	namespace Policy {
		class ISvrPolicyRelay;
	};
};




namespace SF {
	namespace Svr {

		class Entity;
		class ServerEntity;
		class RelayPlayer;
		class RelayGameInstance;


		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	RelayServiceEntity class
		//

		class RelayServiceEntity : public LoadbalanceClusterServiceEntity
		{
		public:
			typedef LoadbalanceClusterServiceEntity super;

		private:

			GameID m_GameID;

			uint32_t m_MaxInstances;

			const ServerConfig::NetPublic* m_PublicNetSocket = nullptr;
			Net::RawUDP* m_pNetPublic = nullptr;

			PageQueue<SharedPointerAtomicT<Net::Connection>> m_NewConnectionQueue;

		protected:


		public:

			RelayServiceEntity(GameID gameID, const ServerConfig::NetPublic& publicNetSocket, uint32_t maximumRelayInstances, ClusterMembership initialMembership = ClusterMembership::Slave);
			~RelayServiceEntity();


			//////////////////////////////////////////////////////////////////////////
			//
			//	Entity operations
			//

			virtual Result InitializeEntity(EntityID newEntityID) override;

			// clear transaction
			virtual Result ClearEntity() override;

			// TickUpdate 
			virtual Result TickUpdate(TimerAction* pAction = nullptr) override;

			// Get net public
			Net::RawUDP* GetNetPublic() { return m_pNetPublic; }

			////////////////////////////////////////////////////////////////////////////////////
			//
			//	Network handling
			//

			Result OnRecv(const sockaddr_storage& remoteAddr, SharedPointerT<Message::MessageData>& pMsg);


			// Process network event
			Result ProcessNewConnection();
		};


	} // namespace Svr
} // namespace SF

