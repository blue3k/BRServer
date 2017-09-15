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
#include "Component/BrComponent.h"


namespace SF {
namespace Svr {

	class Entity;
	class ServerEntity;
	class ServerServiceBase;


	enum ServerComponentID
	{
		ServerComponentID_AccountDB,
		ServerComponentID_LoginSessionDB,
		ServerComponentID_GameDB,
		ServerComponentID_GameTransactionDB,
		ServerComponentID_RankingDB,
		ServerComponentID_ServerEntityManager,		               // Server
		ServerComponentID_EntityManager,
		ServerComponentID_ParallelTransactionManager,

		ServerComponentID_ClusterManagerService,
		ServerComponentID_GameClusterService,
		ServerComponentID_GameInstanceManagerService,
		ServerComponentID_GamePartyManagerService,
		ServerComponentID_PartyManagerService,

		ServerComponentID_MatchingWatcherService_4,
		ServerComponentID_MatchingWatcherService_8,


		ServerComponentID_MatchingQueueWatcherService_4x1,
		ServerComponentID_MatchingQueueWatcherService_4x2,
		ServerComponentID_MatchingQueueWatcherService_4x3,
		ServerComponentID_MatchingQueueWatcherService_4x1S,
		ServerComponentID_MatchingQueueWatcherService_4x1W,


		ServerComponentID_MatchingQueueWatcherService_8x1,
		ServerComponentID_MatchingQueueWatcherService_8x2,
		ServerComponentID_MatchingQueueWatcherService_8x3,
		ServerComponentID_MatchingQueueWatcherService_8x4,
		ServerComponentID_MatchingQueueWatcherService_8x5,
		ServerComponentID_MatchingQueueWatcherService_8x6,
		ServerComponentID_MatchingQueueWatcherService_8x7,
		ServerComponentID_MatchingQueueWatcherService_8x1S,
		ServerComponentID_MatchingQueueWatcherService_8x1W,


		ServerComponentID_Max
	};

	

	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Component career base class
	//


	class ServerComponentCarrier
	{
	public:

		static constexpr size_t MaxComponentID = ServerComponentID_Max;

	public:


		IHeap& m_Heap;

		// component array
		Component* m_Components[MaxComponentID + 1];

	public:

		ServerComponentCarrier(IMemoryManager& memMgr)
			: m_Heap(memMgr)
		{
			memset(m_Components, 0, sizeof(Component*)*(MaxComponentID + 1));
		}


		virtual ~ServerComponentCarrier()
		{
			for (size_t iComponent = 0; iComponent < (MaxComponentID + 1); iComponent++)
			{
				if (m_Components[iComponent] != nullptr)
					m_Components[iComponent]->TerminateComponent();
				IHeap::Delete(m_Components[iComponent]);
			}
		}

		IHeap& GetHeap() { return m_Heap; }

		// Clear components
		virtual void ClearComponents()
		{
			for (size_t iComponent = 0; iComponent < (MaxComponentID + 1); iComponent++)
			{
				if (m_Components[iComponent] != nullptr)
					m_Components[iComponent]->TerminateComponent();
				Util::SafeDelete( m_Components[iComponent] );
			}
		}

		// Initialize components
		virtual Result InitializeComponents()
		{
			Result hr = ResultCode::SUCCESS;
			size_t iComponent = 0;
			for (; iComponent < (MaxComponentID + 1); iComponent++)
			{
				if (m_Components[iComponent] != nullptr)
				{
					if (m_Components[iComponent]->GetIsInitialized())
						m_Components[iComponent]->TerminateComponent();

					hr = m_Components[iComponent]->InitializeComponent();
					if (!(hr)) goto Proc_End;
				}
			}

		Proc_End:

			//if( !(hr) )
			//{
			//	defTrace( Error, "Component {0} failed to initialize", iComponent );
			//}

			return hr;
		}

		Result TerminateComponents()
		{
			Result hr = ResultCode::SUCCESS;
			// Terminate in reverse order
			int iComponent = (int)MaxComponentID;
			for (; iComponent >= 0; iComponent--)
			{
				if (m_Components[iComponent] != nullptr)
				{
					m_Components[iComponent]->TerminateComponent();
				}
			}

			//Proc_End:

			//if (!(hr))
			//{
			//	defTrace(Error, "Component {0} failed to terminate", iComponent);
			//}

			return hr;
		}

		// Add component
		Component* RemoveComponent(unsigned int iComponent)
		{
			if (m_Components[iComponent] != nullptr)
			{
				m_Components[iComponent]->TerminateComponent();
			}

			return m_Components[iComponent];
		}

		template< class ComponentType >
		ComponentType* RemoveComponent()
		{
			uint iComponent = ComponentType::ComponentID;
			if (iComponent >= (MaxComponentID + 1) || iComponent < 0)
				return nullptr;

			ComponentType *pComponent = m_Components[iComponent];
			m_Components[iComponent] = nullptr;

			if (pComponent != nullptr)
				pComponent->TerminateComponent();

			return pComponent;
		}


		void OnAddComponent(Component* newComponent);



		// Add component
		template< class ComponentType >
		Result AddComponent(ComponentType * newComponent)
		{
			if (GetComponent<ComponentType>() != nullptr)
			{
				// already inserted
				return ResultCode::SUCCESS_FALSE;
			}

			if (newComponent == nullptr)
				return ResultCode::FAIL;

			auto pAdapter = new(GetHeap()) ServerComponentAdapterT<ComponentType>(newComponent);
			if (pAdapter == nullptr)
				return ResultCode::OUT_OF_MEMORY;

			m_Components[(int)ComponentType::ComponentID] = (pAdapter);

			return ResultCode::SUCCESS;
		}

		// Add component
		template< class ComponentType >
		Result AddComponentWithAdapter()
		{
			if (GetComponent<ComponentType>() != nullptr)
			{
				// already inserted
				return ResultCode::SUCCESS_FALSE;
			}

			ComponentType* newComponent = new(GetHeap()) ComponentType;
			if (newComponent == nullptr)
				return ResultCode::OUT_OF_MEMORY;

			auto pAdapter = new(GetHeap()) ServerComponentAdapterT<ComponentType>(newComponent);
			if (pAdapter == nullptr)
				return ResultCode::OUT_OF_MEMORY;

			m_Components[(int)ComponentType::ComponentID] = (pAdapter);

			return ResultCode::SUCCESS;
		}

		template< class ComponentType, class ParamType0 >
		Result AddComponentWithAdapter(ParamType0 p0)
		{
			if (GetComponent<ComponentType>() != nullptr)
			{
				// already inserted
				return ResultCode::SUCCESS_FALSE;
			}

			ComponentType* newComponent = new(GetHeap()) ComponentType(p0);
			if (newComponent == nullptr)
				return ResultCode::OUT_OF_MEMORY;

			auto pAdapter = new(GetHeap()) ServerComponentAdapterT<ComponentType>(newComponent);
			if (pAdapter == nullptr)
				return ResultCode::OUT_OF_MEMORY;

			m_Components[(int)ComponentType::ComponentID] = (pAdapter);

			return ResultCode::SUCCESS;
		}

		template< class ComponentType, class ParamType0, class ParamType1 >
		Result AddComponentWithAdapter(ParamType0 p0, ParamType1 p1)
		{
			if (GetComponent<ComponentType>() != nullptr)
			{
				// already inserted
				return ResultCode::SUCCESS_FALSE;
			}

			ComponentType* newComponent = new(GetHeap()) ComponentType(p0, p1);
			if (newComponent == nullptr)
				return ResultCode::OUT_OF_MEMORY;

			auto pAdapter = new(GetHeap()) ServerComponentAdapterT<ComponentType>(newComponent);
			if (pAdapter == nullptr)
				return ResultCode::OUT_OF_MEMORY;

			m_Components[(int)ComponentType::ComponentID] = (pAdapter);

			return ResultCode::SUCCESS;
		}

		template< class ComponentType, class ParamType0, class ParamType1, class ParamType2 >
		Result AddComponentWithAdapter(ParamType0 p0, ParamType1 p1, ParamType2 p2)
		{
			if (GetComponent<ComponentType>() != nullptr)
			{
				// already inserted
				return ResultCode::SUCCESS_FALSE;
			}

			ComponentType* newComponent = new(GetHeap()) ComponentType(p0, p1, p2);
			if (newComponent == nullptr)
				return ResultCode::OUT_OF_MEMORY;

			auto pAdapter = new(GetHeap()) ServerComponentAdapterT<ComponentType>(newComponent);
			if (pAdapter == nullptr)
				return ResultCode::OUT_OF_MEMORY;

			m_Components[(int)ComponentType::ComponentID] = (pAdapter);

			return ResultCode::SUCCESS;
		}


		template< class ComponentType >
		Result AddComponentWithAdapter(ComponentType* &newComponent)
		{
			if (GetComponent<ComponentType>() != nullptr)
			{
				// already inserted
				return ResultCode::SUCCESS_FALSE;
			}

			if (newComponent == nullptr)
				return ResultCode::FAIL;

			auto pAdapter = new(GetHeap()) ServerComponentAdapterT<ComponentType>(newComponent);
			if (pAdapter == nullptr)
				return ResultCode::OUT_OF_MEMORY;

			m_Components[(int)ComponentType::ComponentID] = (pAdapter);

			return ResultCode::SUCCESS;
		}



		// Get component with its ID
		Component* GetComponent(int ID)
		{
			if ((size_t)ID >= (MaxComponentID + 1) || ID < 0)
				return nullptr;

			return m_Components[ID];
		}

		template< class ComponentType >
		ComponentType* GetComponent(int ID)
		{
			if ((size_t)ID >= (MaxComponentID + 1) || ID < 0)
				return nullptr;

			if (m_Components[ID] != nullptr)
				return *(ServerComponentAdapterT<ComponentType>*)m_Components[ID];
			else
				return nullptr;
		}


		// Get component with its type
		template< class ComponentType >
		ComponentType* GetComponent()
		{
			uint ID = ComponentType::ComponentID;
			if (ID >= (MaxComponentID + 1) || ID < 0)
				return nullptr;

			if (m_Components[ID] != nullptr)
				return *(ServerComponentAdapterT<ComponentType>*)m_Components[ID];
			else
				return nullptr;
		}

		// Get component with its type
		template< class ComponentType >
		const ComponentType* GetComponent() const
		{
			uint ID = ComponentType::ComponentID;
			if (ID >= (MaxComponentID + 1) || ID < 0)
				return nullptr;

			if (m_Components[ID] != nullptr)
				return *(ServerComponentAdapterT<ComponentType>*)m_Components[ID];
			else
				return nullptr;
		}


	};


#include "ServerComponent.inl"


}; // namespace Svr
}; // namespace SF




