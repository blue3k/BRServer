////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Server constants definitions. 
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Object/LibraryComponent.h"
#include "Object/SharedPointer.h"


namespace SF {

	class ServerLogObject;

	class ServerLogComponent : public LibraryComponent
	{
	public:

	private:

		SharedPointerT<SharedObject> m_pInstance;
		SharedPointerT<ServerLogObject> m_LogObject;

	public:
		static constexpr FixedString TypeName = "ServerLogModule";

		ServerLogComponent();
		ServerLogComponent(const char* traceCfgPath);
		~ServerLogComponent();



		virtual const FixedString& GetTypeName() override { return TypeName; }

		// Initialize server component
		virtual Result InitializeComponent() override;
		// Terminate server component
		virtual void DeinitializeComponent() override;
	};


}

