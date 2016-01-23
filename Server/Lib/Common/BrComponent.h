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
#include "Common/ResultCode/BRResultCodeCommon.h"




namespace BR {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	IComponent base class
	//

	class Component
	{
	public:

	private:

		BRCLASS_ATTRIBUTE_READONLY(unsigned int,ComponentID);
		BRCLASS_ATTRIBUTE_READONLY(bool,IsInitialized);

	public:
		Component( unsigned int id ) : m_ComponentID(id), m_IsInitialized(false) {}
		virtual ~Component() {}

		// Initialize server component
		virtual HRESULT InitializeComponent() { m_IsInitialized = true; return S_SYSTEM_OK; }
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
		virtual HRESULT InitializeComponents()
		{
			HRESULT hr = S_SYSTEM_OK;
			size_t iComponent = 0;
			for( ; iComponent < (MaxComponentID+1); iComponent++ )
			{
				if( m_Components[iComponent] != nullptr )
				{
					if( m_Components[iComponent]->GetIsInitialized() )
						m_Components[iComponent]->TerminateComponent();

					hr = m_Components[iComponent]->InitializeComponent();
					if (FAILED(hr)) goto Proc_End;
				}
			}

		Proc_End:

			//if( FAILED(hr) )
			//{
			//	defTrace( Trace::TRC_ERROR, "Component {0} failed to initialize", iComponent );
			//}

			return hr;
		}

		HRESULT TerminateComponents()
		{
			HRESULT hr = S_SYSTEM_OK;
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

			//if (FAILED(hr))
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
		HRESULT AddComponent( bool bAllowDuplicatedComponent = false )
		{
			if( !bAllowDuplicatedComponent && GetComponent<ComponentType>() != nullptr )
			{
				// already inserted
				return S_SYSTEM_FALSE;
			}

			ComponentType* newComponent = new ComponentType;
			if( newComponent == nullptr )
				return E_SYSTEM_OUTOFMEMORY;

			HRESULT hr = AddComponent(newComponent);
			if( FAILED(hr) )
			{
				delete newComponent;
			}

			return hr;
		}

		template< class ComponentType, class ParamType0 >
		HRESULT AddComponent( ParamType0 p0, bool bAllowDuplicatedComponent = false )
		{
			if( !bAllowDuplicatedComponent && GetComponent<ComponentType>() != nullptr )
			{
				// already inserted
				return S_SYSTEM_FALSE;
			}

			ComponentType* newComponent = new ComponentType(p0);
			if( newComponent == nullptr )
				return E_SYSTEM_OUTOFMEMORY;

			HRESULT hr = AddComponent(newComponent);
			if( FAILED(hr) )
			{
				delete newComponent;
			}

			return hr;
		}

		template< class ComponentType, class ParamType0, class ParamType1 >
		HRESULT AddComponent( ParamType0 p0, ParamType1 p1, bool bAllowDuplicatedComponent = false )
		{
			if( !bAllowDuplicatedComponent && GetComponent<ComponentType>() != nullptr )
			{
				// already inserted
				return S_SYSTEM_FALSE;
			}

			ComponentType* newComponent = new ComponentType(p0,p1);
			if( newComponent == nullptr )
				return E_SYSTEM_OUTOFMEMORY;

			HRESULT hr = AddComponent(newComponent);
			if( FAILED(hr) )
			{
				delete newComponent;
			}

			return hr;
		}

		template< class ComponentType, class ParamType0, class ParamType1, class ParamType2 >
		HRESULT AddComponent(ParamType0 p0, ParamType1 p1, ParamType2 p2, bool bAllowDuplicatedComponent = false)
		{
			if (!bAllowDuplicatedComponent && GetComponent<ComponentType>() != nullptr)
			{
				// already inserted
				return S_SYSTEM_FALSE;
			}

			ComponentType* newComponent = new ComponentType(p0, p1, p2);
			if (newComponent == nullptr)
				return E_SYSTEM_OUTOFMEMORY;

			HRESULT hr = AddComponent(newComponent);
			if (FAILED(hr))
			{
				delete newComponent;
			}

			return hr;
		}


		virtual void OnAddComponent( Component* newComponent)
		{}

		template< class ComponentType >
		HRESULT AddComponent( ComponentType* &newComponent )
		{
			// invalid component id range
			if( newComponent->GetComponentID() >= (MaxComponentID+1) )
				return E_SYSTEM_UNEXPECTED;

			if( m_Components[newComponent->GetComponentID()] != nullptr )
			{
				// already registered
				return E_DUPLICATED_COMPONENT;
			}

			m_Components[newComponent->GetComponentID()] = newComponent;

			OnAddComponent(newComponent);

			newComponent = nullptr;

			return S_SYSTEM_OK;
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




} // namespace BR




