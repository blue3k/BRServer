////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Madk
// 
// Author : KyungKun Ko
//
// Description : Default library component
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/BrLibComponents.h"


namespace BR {


	class LibComponentDefault : public Component
	{
	public:

		enum {
			ComponentID = (UINT)LibCompoentIDs::ThirdParty
		};

	private:

	public:

		LibComponentDefault();
		~LibComponentDefault();

		// Initialize server component
		virtual Result InitializeComponent() override;
		// Terminate server component
		virtual void TerminateComponent() override;
	};


}

