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


namespace BR {


	class LibComponentThirdParty : public Component
	{
	public:

		enum {
			ComponentID = (UINT)LibCompoentIDs::ThirdParty
		};

	private:

	public:

		LibComponentThirdParty();
		~LibComponentThirdParty();

		// Initialize server component
		virtual HRESULT InitializeComponent() override;
		// Terminate server component
		virtual void TerminateComponent() override;
	};


}

