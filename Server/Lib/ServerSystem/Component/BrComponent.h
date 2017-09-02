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
#include "ResultCode/SFResultCodeLibrary.h"




namespace SF {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	IComponent base class
	//

	class Component
	{
	public:

	private:

		unsigned int m_ComponentID = 0;
		bool m_IsInitialized = false;

	public:
		Component( unsigned int id ) : m_ComponentID(id), m_IsInitialized(false) {}
		virtual ~Component() {}

		uint GetComponentID() { return m_ComponentID; }
		bool GetIsInitialized() { return m_IsInitialized; }

		// Initialize server component
		virtual Result InitializeComponent() { m_IsInitialized = true; return ResultCode::SUCCESS; }
		// Terminate server component
		virtual void TerminateComponent() { m_IsInitialized = false;  }
	};



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Component career base class
	//

	template< size_t MaxComponentID >
	class ComponentCarrier
	{
	public:

	private:

		// component array
		Component* m_Components[MaxComponentID+1];

	public:
		ComponentCarrier()
		{
			memset( m_Components, 0, sizeof(Component*)*(MaxComponentID+1) );
		}

		virtual ~ComponentCarrier()
		{
			for (size_t iComponent = 0; iComponent < (MaxComponentID + 1); iComponent++)
			{
				if (m_Components[iComponent] != nullptr)
					m_Components[iComponent]->TerminateComponent();
				//Util::SafeDelete( m_Components[iComponent] );
			}
		}

		// Clear components
		virtual void ClearComponents()
		{
			for(size_t iComponent = 0; iComponent < (MaxComponentID+1); iComponent++ )
			{
				if( m_Components[iComponent] != nullptr )
					m_Components[iComponent]->TerminateComponent();
				//Util::SafeDelete( m_Components[iComponent] );
			}
		}
		
		// Initialize components
		virtual Result InitializeComponents()
		{
			Result hr = ResultCode::SUCCESS;
			size_t iComponent = 0;
			for( ; iComponent < (MaxComponentID+1); iComponent++ )
			{
				if( m_Components[iComponent] != nullptr )
				{
					if( m_Components[iComponent]->GetIsInitialized() )
						m_Components[iComponent]->TerminateComponent();

					hr = m_Components[iComponent]->InitializeComponent();
					if (!(hr)) goto Proc_End;
				}
			}

		Proc_End:

			//if( !(hr) )
			//{
			//	defTrace( Trace::TRC_ERROR, "Component {0} failed to initialize", iComponent );
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
			//	defTrace(Trace::TRC_ERROR, "Component {0} failed to terminate", iComponent);
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
			UINT iComponent = ComponentType::ComponentID;
			if (iComponent >= (MaxComponentID + 1) || iComponent < 0)
				return nullptr;

			ComponentType *pComponent = m_Components[iComponent];
			m_Components[iComponent] = nullptr;

			if (pComponent != nullptr)
				pComponent->TerminateComponent();

			return pComponent;
		}

		// Add component
		template< class ComponentType >
		Result AddComponent( bool bAllowDuplicatedComponent = false )
		{
			if( !bAllowDuplicatedComponent && GetComponent<ComponentType>() != nullptr )
			{
				// already inserted
				return ResultCode::SUCCESS_FALSE;
			}

			ComponentType* newComponent = new ComponentType;
			if( newComponent == nullptr )
				return ResultCode::OUT_OF_MEMORY;

			Result hr = AddComponent(newComponent);
			if( !(hr) )
			{
				delete newComponent;
			}

			return hr;
		}

		template< class ComponentType, class ParamType0 >
		Result AddComponent( ParamType0 p0, bool bAllowDuplicatedComponent = false )
		{
			if( !bAllowDuplicatedComponent && GetComponent<ComponentType>() != nullptr )
			{
				// already inserted
				return ResultCode::SUCCESS_FALSE;
			}

			ComponentType* newComponent = new ComponentType(p0);
			if( newComponent == nullptr )
				return ResultCode::OUT_OF_MEMORY;

			Result hr = AddComponent(newComponent);
			if( !(hr) )
			{
				delete newComponent;
			}

			return hr;
		}

		template< class ComponentType, class ParamType0, class ParamType1 >
		Result AddComponent( ParamType0 p0, ParamType1 p1, bool bAllowDuplicatedComponent = false )
		{
			if( !bAllowDuplicatedComponent && GetComponent<ComponentType>() != nullptr )
			{
				// already inserted
				return ResultCode::SUCCESS_FALSE;
			}

			ComponentType* newComponent = new ComponentType(p0,p1);
			if( newComponent == nullptr )
				return ResultCode::OUT_OF_MEMORY;

			Result hr = AddComponent(newComponent);
			if( !(hr) )
			{
				delete newComponent;
			}

			return hr;
		}

		template< class ComponentType, class ParamType0, class ParamType1, class ParamType2 >
		Result AddComponent(ParamType0 p0, ParamType1 p1, ParamType2 p2, bool bAllowDuplicatedComponent = false)
		{
			if (!bAllowDuplicatedComponent && GetComponent<ComponentType>() != nullptr)
			{
				// already inserted
				return ResultCode::SUCCESS_FALSE;
			}

			ComponentType* newComponent = new ComponentType(p0, p1, p2);
			if (newComponent == nullptr)
				return ResultCode::OUT_OF_MEMORY;

			Result hr = AddComponent(newComponent);
			if (!(hr))
			{
				delete newComponent;
			}

			return hr;
		}


		virtual void OnAddComponent( Component* newComponent)
		{}

		template< class ComponentType >
		Result AddComponent( ComponentType* &newComponent )
		{
			// invalid component id range
			if( newComponent->GetComponentID() >= (MaxComponentID+1) )
				return ResultCode::UNEXPECTED;

			if( m_Components[newComponent->GetComponentID()] != nullptr )
			{
				// already registered
				return ResultCode::E_DUPLICATED_COMPONENT;
			}

			m_Components[newComponent->GetComponentID()] = newComponent;

			OnAddComponent(newComponent);

			newComponent = nullptr;

			return ResultCode::SUCCESS;
		}

		// Get component with its ID
		Component* GetComponent( int ID )
		{
			if( (size_t)ID >= (MaxComponentID+1) || ID < 0 )
				return nullptr;

			return m_Components[ID];
		}

		template< class ComponentType >
		ComponentType* GetComponent(int ID)
		{
			if((size_t)ID >= (MaxComponentID+1) || ID < 0 )
				return nullptr;

			if( m_Components[ID] != nullptr )
				return dynamic_cast<ComponentType*>(m_Components[ID]);
			else
				return nullptr;
		}


		// Get component with its type
		template< class ComponentType >
		ComponentType* GetComponent()
		{
			UINT ID = ComponentType::ComponentID;
			if( ID >= (MaxComponentID+1) || ID < 0 )
				return nullptr;

			if( m_Components[ID] != nullptr )
				return (ComponentType*)m_Components[ID];
			else
				return nullptr;
		}
		
		// Get component with its type
		template< class ComponentType >
		const ComponentType* GetComponent() const
		{
			UINT ID = ComponentType::ComponentID;
			if( ID >= (MaxComponentID+1) || ID < 0 )
				return nullptr;

			if( m_Components[ID] != nullptr )
				return (ComponentType*)m_Components[ID];
			else
				return nullptr;
		}

		// iterate all component
		void ForeachComponent( std::function<void(Component*)> functor )
		{
			for(size_t iComponent = 0; iComponent < (MaxComponentID+1); iComponent++ )
			{
				if( m_Components[iComponent] != nullptr )
					functor(m_Components[iComponent]);
			}
		}
	};




} // namespace SF




