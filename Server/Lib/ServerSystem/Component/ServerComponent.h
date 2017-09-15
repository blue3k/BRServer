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
#include "ServerService/ServerServiceBase.h"
#include "Component/ServerComponentCarrier.h"


namespace SF {
namespace Svr {

	class Entity;
	class ServerEntity;
	class ServerServiceBase;


	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	IServerComponent base class
	//

	class IServerComponent : public Component
	{
	public:
		IServerComponent( uint id ) : Component(id)
		{
		}

		virtual ~IServerComponent()
		{
		}
	};
	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	IServerComponent base class
	//

	template<class AdaptedObject>
	class ServerComponentAdapterT : public IServerComponent
	{
	private:
		SharedPointerT<AdaptedObject> m_Instance;

	public:
		ServerComponentAdapterT(const SharedPointerT<AdaptedObject>& adapter)
			: IServerComponent(AdaptedObject::ComponentID)
			, m_Instance(adapter)
		{
		}

		virtual ~ServerComponentAdapterT()
		{
		}

		AdaptedObject* operator *()
		{
			return *m_Instance;
		}

		AdaptedObject* operator ->()
		{
			return *m_Instance;
		}

		operator AdaptedObject*()
		{
			return *m_Instance;
		}

		// Initialize server component
		virtual Result InitializeComponent()
		{
			Result result = IServerComponent::InitializeComponent();
			if (!result)
				return result;

			if(m_Instance != nullptr)
				return m_Instance->InitializeComponent();

			return result;
		}

		// Terminate server component
		virtual void TerminateComponent()
		{
			if (!GetIsInitialized())
				return;

			IServerComponent::TerminateComponent();

			if(m_Instance != nullptr)
				m_Instance->TerminateComponent();
		}

	};


	

#include "ServerComponent.inl"


}; // namespace Svr
}; // namespace SF




