////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2019 The Braves
// 
// Author : KyungKun Ko
//
// Description : PlayFablog output
//
////////////////////////////////////////////////////////////////////////////////


#include "SFCorePCH.h"
#include "SFTypedefs.h"
#include "SFAssert.h"
#include "Util/SFLogComponent.h"
#include "Util/SFTimeUtil.h"
#include "Util/SFUtility.h"
#include "String/SFFixedString.h"
#include "Service/SFService.h"
#include "ServerLog/SFLogOutputPlayFabGSDK.h"


#include "gsdk.h"


namespace SF {



	constexpr StringCrc64 LogOutputPlayFabGSDKComponent::TypeName;

	LogOutputPlayFabGSDKComponent::LogOutputPlayFabGSDKComponent(const LogChannelMask& logMask)
		: LibraryComponent(TypeName)
		, m_Handler(logMask)
	{

	}


	LogOutputPlayFabGSDKComponent::~LogOutputPlayFabGSDKComponent()
	{

	}

	LogOutputPlayFabGSDKComponent::MyOutputHandler::MyOutputHandler(const LogChannelMask& logMask)
		: LogOutputHandler(logMask)
	{
	}


	void LogOutputPlayFabGSDKComponent::MyOutputHandler::PrintOutput(const Log::LogModule::LogItem* logMessage)
	{
		if (logMessage == nullptr) return;

		Microsoft::Azure::Gaming::GSDK::logMessage(logMessage->LogBuff);
	}

	void LogOutputPlayFabGSDKComponent::MyOutputHandler::Flush()
	{
	}

	// Initialize server component
	Result LogOutputPlayFabGSDKComponent::InitializeComponent()
	{
		Service::LogModule->RegisterOutputHandler(&m_Handler);
		return ResultCode::SUCCESS;
	}

	// Terminate server component
	void LogOutputPlayFabGSDKComponent::DeinitializeComponent()
	{
		Service::LogModule->UnregisterOutputHandler(&m_Handler);
	}





}	// namespace SF


