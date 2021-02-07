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

#include "Memory/SFMemory.h"
#include "Types/BrSvrTypes.h"
#include "Container/SFPageQueue.h"
#include "Util/LocalUIDGenerator.h"
#include "Entity/Entity.h"
#include "Entity/SimpleUserEntity.h"
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
		class ServerMUDP;
	};

	class ISvrPolicyLogin;
};




namespace SF {



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ServerServiceComponentEntity class
	//

	class LoginServiceEntity : public ServiceEntity
	{
	public:
		using super = ServiceEntity;

	private:

		const ServerConfig::NetPublic *m_PublicNetSocket = nullptr;
		SharedPointerT<Net::ServerNet> m_pNetPublic;

		PageQueue<SharedPointerAtomicT<Net::Connection>> m_NewConnectionQueue;

	protected:


	public:

		LoginServiceEntity(const ServerConfig::NetPublic *publicNetSocket, const ServerConfig::MessageEndpoint& endpoint);
		~LoginServiceEntity();

		// We are not going to use hashed key
		virtual uint KeyHash( uint64_t key ) { return (uint)key; }


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

		// Get net public
		Net::ServerNet* GetNetPublic() { return *m_pNetPublic; }

		// Process network event
		Result ProcessNewConnection();
	};


} // namespace SF
