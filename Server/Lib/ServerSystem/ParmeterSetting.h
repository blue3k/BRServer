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


	class LibComponentEnvSetting : public Component
	{
	public:

		enum {
			ComponentID = (UINT)LibCompoentIDs::EnvSetting
		};

	private:

		std::unordered_map<std::string, std::string> m_Settings;

	public:

		LibComponentEnvSetting(const std::vector<std::string>& cmdArgs);
		~LibComponentEnvSetting();

		// Initialize server component
		virtual HRESULT InitializeComponent() override;
		// Terminate server component
		virtual void TerminateComponent() override;
	};


}

