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

	class RelayServiceEntity : public LoadbalanceClusterServiceEntity, public Net::RawUDP::MessageHandler
	{
	public:
		typedef LoadbalanceClusterServiceEntity super;

	private:

		const ServerConfig::NetPublic*		m_PublicNetSocket = nullptr;
		Net::RawUDP*						m_pNetPublic = nullptr;

		PageQueue<SharedPointerAtomicT<Net::Connection>> m_NewConnectionQueue;

	protected:


	public:

		RelayServiceEntity(const ServerConfig::NetPublic *publicNetSocket, ClusterMembership initialMembership = ClusterMembership::Slave);
		~RelayServiceEntity();
		

		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity operations
		//

		Result InitializeEntity( EntityID newEntityID );

		// clear transaction
		virtual Result ClearEntity();

		// TickUpdate 
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;

		// Get net public
		Net::RawUDP* GetNetPublic() { return m_pNetPublic; }

		////////////////////////////////////////////////////////////////////////////////////
		//
		//	Network handling
		//

		virtual Result OnRecv(const sockaddr_storage& remoteAddr, SharedPointerT<Message::MessageData>& pMsg) override;


		// Process network event
		Result ProcessNewConnection();
	};








}; // namespace Svr
}; // namespace SF




