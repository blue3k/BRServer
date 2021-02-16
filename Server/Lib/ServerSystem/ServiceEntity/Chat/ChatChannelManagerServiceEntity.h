////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game ChatChannel service class definition
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

#include "PerformanceCounter/PerformanceCounter.h"

namespace SF {
	namespace Svr {

		class Entity;
		class ServerEntity;
		class GameChatChannelEntity;


		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	ChatChannelManagerServiceEntity class
		//

		class ChatChannelManagerServiceEntity : public ServiceEntity
		{
		public:

			using super = ServiceEntity;

			enum { ComponentID = ServerComponentID_ChatChannelManagerService };

		private:

			PerformanceCounterRaw<uint64_t> m_ChatChannelCount;

		protected:


		public:

			ChatChannelManagerServiceEntity(GameID gameID, const EndpointAddress& endpoint);
			~ChatChannelManagerServiceEntity();


			//////////////////////////////////////////////////////////////////////////
			//
			//	Entity operations
			//

			virtual Result InitializeEntity(EntityID newEntityID) override;

			virtual Result RegisterServiceMessageHandler() override;

			//////////////////////////////////////////////////////////////////////////
			//
			//	Game ChatChannel operations
			//

			// Add new Entity
			virtual Result CreateChatChannel(const StringCrc64& name, const PlayerInformation& creator, EntityUID playerUID, const SharedPointerT<MessageEndpoint>& remoteEndpoint, EntityUID& ChatChannelUID);

			virtual Result FindChatChannel(const StringCrc64& name, EntityUID& ChatChannelUID);

			// Called when a game ChatChannel is deleted
			virtual Result FreeChatChannel(EntityUID ChatChannelUID);

			// Initialize server component
			Result InitializeComponent() { return ResultCode::SUCCESS; }
			// Terminate server component
			void TerminateComponent() {  }


		};

	} // namespace Svr
} // namespace SF

