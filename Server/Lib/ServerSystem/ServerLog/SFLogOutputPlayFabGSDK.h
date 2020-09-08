////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2019 The Braves
// 
// Author : KyungKun Ko
//
// Description : PlayFablog output
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "SFAssert.h"
#include "Object/SFLibraryComponent.h"
#include "Object/SFSharedPointer.h"
#include "Util/SFLog.h"
#include "IO/SFFile.h"
#include "String/SFString.h"


namespace SF {



	class LogOutputPlayFabGSDKComponent : public LibraryComponent
	{
	public:
		static constexpr StringCrc64 TypeName = "LogOutputPlayFabGSDK";

	private:

		class MyOutputHandler : public Log::LogOutputHandler
		{
		private:

		public:
			MyOutputHandler(const LogChannelMask& logMask);

			virtual void PrintOutput(const Log::LogModule::LogItem* logMessage) override;

			virtual void Flush() override;
		};

		MyOutputHandler m_Handler;


	public:

		LogOutputPlayFabGSDKComponent(const LogChannelMask& logMask);
		~LogOutputPlayFabGSDKComponent();

		virtual const StringCrc64& GetTypeName() override { return TypeName; }

		MyOutputHandler& GetHandler() { return m_Handler; }

		// Initialize server component
		virtual Result InitializeComponent() override;
		// Terminate server component
		virtual void DeinitializeComponent() override;
	};



}

