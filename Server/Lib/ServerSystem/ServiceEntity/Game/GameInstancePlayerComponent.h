////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game player
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"

#include "Util/SFTimeUtil.h"
#include "Container/SFHashTable.h"
#include "MemoryManager/SFMemoryPool.h"
#include "GameConst.h"
#include "Types/BrGameTypes.h"

#include "Entity/Entity.h"
#include "Actor/Movement/SFActorMovement.h"
#include "Component/SFComponent.h"


namespace SF {
	namespace Net {
		class Connection;
	}
}



namespace SF {
	namespace Svr {

		class GameInstanceEntity;
		class GameInstancePlayer;

		//////////////////////////////////////////////////////////////////////////
		//
		//	Game Instance entity class
		//

		class GameInstancePlayerComponent : public Component
		{
		private:

			GameInstancePlayer* m_Owner{};

		public:
			GameInstancePlayerComponent(uint componentId, GameInstancePlayer* owner)
				: Component(componentId)
				, m_Owner(owner)
			{
			}

			GameInstancePlayer& GetOwner() { return *m_Owner; }
		};


		// GameInstancePlayerComponentMovement
		class GameInstancePlayerComponentMovement : public GameInstancePlayerComponent
		{
		public:

			static constexpr StringCrc32 ComponentID = "GameInstancePlayerComponentMovement"_crc;

			using super = GameInstancePlayerComponent;

		private:

			IHeap& m_Heap;

			ReceivedActorMovementManager m_ActorMovement;

		public:

			GameInstancePlayerComponentMovement(IHeap& heap, GameInstancePlayer* owner);

			IHeap& GetHeap() { return m_Heap; }

			ReceivedActorMovementManager& GetMovementManager() { return m_ActorMovement; }

			virtual void TickUpdate() override;

		};

	} // Svr
} // namespace SF
