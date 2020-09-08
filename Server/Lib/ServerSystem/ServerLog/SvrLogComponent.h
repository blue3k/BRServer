////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Server constants definitions. 
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Object/SFLibraryComponent.h"
#include "Object/SFSharedPointer.h"


namespace SF {

	class ServerLogObject;

	class ServerLogComponent : public LibraryComponent
	{
	public:

	private:
		char m_ConfigFilePath[256];

		SharedPointerT<ServerLogObject> m_LogObject;

	public:
		static constexpr StringCrc64 TypeName = "ServerLogModule";

		ServerLogComponent(const char* traceCfgPath);
		~ServerLogComponent();



		virtual const StringCrc64& GetTypeName() override { return TypeName; }

		// Initialize server component
		virtual Result InitializeComponent() override;
		// Terminate server component
		virtual void DeinitializeComponent() override;
	};


}

