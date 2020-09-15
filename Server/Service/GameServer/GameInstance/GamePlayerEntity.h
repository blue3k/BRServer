////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game player Entity
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"

#include "Util/SFTimeUtil.h"
#include "Container/SFHashTable.h"
#include "Memory/SFMemoryPool.h"
#include "Entity/Entity.h"
#include "GameConst.h"
#include "Types/BrGameTypes.h"
#include "GameSvrConst.h"
#include "ServiceEntity/Game/GamePlayerEntity.h"
#include "GameSystem/UserSystemComponentIDs.h"


namespace SF {
namespace Net {
	class Connection;
	class ServerNet;
};

};



namespace SF {
namespace GameServer {


	class GameServer;
	class InvenSystem;
	class GameInstanceEntity;


	// GCM predefined messages
	#define BRPUSHMSG_DISCONNECTED		"BRDisconnected"
	#define BRPUSHMSG_SYNC				"BRSync"


	//////////////////////////////////////////////////////////////////////////
	//
	//	Game Instance entity class
	//

	class GamePlayerEntity : public Svr::GamePlayerEntity
	{
	public:

		typedef Svr::GamePlayerEntity super;

	private:

		ComponentCarrier<UserSystemComponentID_Max> m_ComponentCarrier;

		bool m_ClosingPended = false;

	public:

		GamePlayerEntity();
		~GamePlayerEntity();

		ComponentCarrier<UserSystemComponentID_Max>& GetComponentCarrier() { return m_ComponentCarrier; }

		// Get component with its type
		template< class ComponentType >
		ComponentType* GetComponent() { return m_ComponentCarrier.GetComponent<ComponentType>(); }

		// Get component with its type
		template< class ComponentType >
		const ComponentType* GetComponent() const { return m_ComponentCarrier.GetComponent<ComponentType>(); }


		virtual const PlayerInformation& GetPlayerInformation() const override;
		virtual const ServerFriendInformation& GetFriendInformation() const override;


		virtual void SetLatestActiveTime(UTCTimeStampSec latestActiveTime) override;
		virtual Result UpdateDBSync(TransactionID transID = TransactionID()) override;

		virtual Result UpdateGameConfig() override;
		virtual Result UpdateGamePlayer() override;


		virtual Result InitializeEntity(EntityID newEntityID) override;

		virtual void PendingCloseTransaction(const char* reason) override;

		virtual Result RegisterMessageHandlers() override;
	};


}; // namespace GameServer
}; // namespace SF
