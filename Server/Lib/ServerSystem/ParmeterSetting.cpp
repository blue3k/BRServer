////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 MadK
// 
// Author : KyungKun Ko
//
// Description : Base type ToString implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/BrAssert.h"
#include "Common/Trace.h"
#include "Common/EnvSettingComponent.h"
#include "Common/Utility.h"
#include "Common/BrXML.h"
#include "ServerSystem/BrService.h"



namespace BR
{

	LibComponentEnvSetting::LibComponentEnvSetting(const std::vector<std::string>& cmdArgs)
		: Component((UINT)ComponentID)
	{

		for (auto itArg : cmdArgs)
		{
			const char* pCurParam = itArg.c_str();

			switch (pCurParam[0])
			{
			case '-':
				pCurParam++;
				if (StrUtil::StringCmpLwr(pCurParam, (INT)strlen(pCurParam), "install", (INT)strlen("install")) == 0)
				{
					m_Settings.insert(std::make_pair("install", "true"));
				}
				else if (pCurParam[0] == 'n' || pCurParam[0] == 'N')
				{
					pCurParam += 2;
					m_Settings.insert(std::make_pair("servicename", "true"));
				}
				else if (pCurParam[0] == 'u' || pCurParam[0] == 'U')
				{
					pCurParam += 2;
					m_Settings.insert(std::make_pair("user", pCurParam));
				}
				else if (pCurParam[0] == 'p' || pCurParam[0] == 'P')
				{
					pCurParam += 2;
					m_Settings.insert(std::make_pair("password", pCurParam));
				}
				else if (StrUtil::StringCmpLwr(pCurParam, (INT)strlen(pCurParam), "uninstall", (INT)strlen("uninstall")) == 0)
				{
					m_Settings.insert(std::make_pair("uninstall", pCurParam));
				}
				else if (StrUtil::StringCmpLwr(pCurParam, (INT)strlen(pCurParam), "debug", (INT)strlen("debug")) == 0)
				{
					m_Settings.insert(std::make_pair("debug", pCurParam));
				}
				break;
			default:
				break;
			};
		}
	}

	LibComponentEnvSetting::~LibComponentEnvSetting()
	{
	}

	// Initialize server component
	HRESULT LibComponentEnvSetting::InitializeComponent()
	{
		HRESULT hr = Component::InitializeComponent();
		if (FAILED(hr)) return hr;

		auto itFound = m_Settings.find("servicename");
		if(itFound != m_Settings.end())
		{
			Util::SetServiceName(itFound->second.c_str());
		}

		//itFound = m_Settings.find("install");
		//if (itFound != m_Settings.end())
		//{
		//	auto itUserName = m_Settings.find("user");
		//	auto itPassword = m_Settings.find("password");
		//	if (itUserName == m_Settings.end() || itPassword == m_Settings.end())
		//	{
		//		svrErr(E_SYSTEM_INVALIDARG);
		//	}
		//	svrChk(Svr::Service::ServiceInstall());
		//}

		//itFound = m_Settings.find("servicename");
		//if (itFound != m_Settings.end())
		//{
		//	svrChk(Svr::Service::ServiceUninstall());
		//}


	Proc_End:


		return hr;
	}

	// Terminate server component
	void LibComponentEnvSetting::TerminateComponent()
	{
		Component::TerminateComponent();

	}


};	// namespace BR


