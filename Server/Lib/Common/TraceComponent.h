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

#include "Common/Typedefs.h"
#include "Common/BrLibComponents.h"
#include "Common/SharedPointer.h"


namespace BR {


	class LibComponentTrace : public Component
	{
	public:

		enum {
			ComponentID = (UINT)LibCompoentIDs::Trace
		};

	private:

		SharedPointerT<SharedObject> m_pInstance;

	public:

		LibComponentTrace();
		LibComponentTrace(const char* modulePath, const char* moduleName, const char* traceCfgPath);
		~LibComponentTrace();

		// Initialize server component
		virtual Result InitializeComponent() override;
		// Terminate server component
		virtual void TerminateComponent() override;
	};


}

