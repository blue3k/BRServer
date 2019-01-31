////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Base type ToString implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFAssert.h"
#include "Object/SFSharedPointer.h"
#include "String/SFFixedString32.h"
#include "String/SFFixedString.h"
#include "String/SFStrUtil.h"
#include "EngineObject/SFEngineObject.h"
#include "ServerLog/SvrLog.h"
#include "ServerLog/SvrLogComponent.h"
#include "Util/SFLogComponent.h"
#include "Util/SFUtility.h"
#include "SFEngine.h"


namespace SF
{
	class ServerLogObject : public EngineObject
	{
	private:
		char m_ConfigFilePath[256];
		LogChannelParameter m_Parameter;
		LogChannelMask OutputConsole;
		LogChannelMask OutputDebugger;
		LogChannelMask OutputFile;

		TimeStampMS m_MaskUpdated;

	public:

		ServerLogObject(const char* configFile)
			: EngineObject(&GetEngineHeap(), "ServerLogObject")
		{
			StrUtil::StringCopy(m_ConfigFilePath, configFile);

			m_MaskUpdated = Util::Time.GetTimeMs();
			LoadTraceConfig();
		}


		void SetMask(const char* maskName, uint32_t maskValue)
		{
			switch (Hash32(maskName))
			{
			case "default"_hash32:
			case "def"_hash32:					m_Parameter.MainChannelMasks[(int)LogMainChannels::System] = maskValue; break;
			case "net"_hash32:					m_Parameter.MainChannelMasks[(int)LogMainChannels::Net] = maskValue; break;
			case "db"_hash32:					m_Parameter.MainChannelMasks[(int)LogMainChannels::DB] = maskValue; break;
			case "svr"_hash32:					m_Parameter.MainChannelMasks[(int)LogMainChannels::Svr] = maskValue; break;
			case "protocol"_hash32:				m_Parameter.MainChannelMasks[(int)LogMainChannels::Protocol] = maskValue; break;
			case "OutputDebugger"_hash32:		OutputDebugger = maskValue; break;
			case "OutputConsole"_hash32:		OutputConsole = maskValue; break;
			case "OutputFile"_hash32:			OutputFile = maskValue; break;
			case "OutputDbgFile"_hash32:		break;
			default:
				break;
			}
		}

		char* SkipSpace(char* curChar)
		{
			while ((*curChar) == ' ' && (*curChar) != '\0')
			{
				curChar++;
			}
			return curChar;
		}

		char* FindDelimiter(char* curChar)
		{
			if (curChar == nullptr) return nullptr;

			char curCharValue = *curChar;
			while (curCharValue != ' ' && curCharValue != '=' && curCharValue != '\0')
			{
				curChar++;
				curCharValue = *curChar;
			}
			return curChar;
		}

		void LoadTraceConfig()
		{
			std::string strCfgPath = Util::GetModulePath();
			strCfgPath.append(m_ConfigFilePath);

			FILE *file = fopen(strCfgPath.c_str(), "r");
			if (file == nullptr)
				return;

			char stringBuffer[4096];
			while (fgets(stringBuffer, sizeof(stringBuffer), file) != nullptr)
			{
				char *curChar = stringBuffer;
				auto nameStart = SkipSpace(curChar);

				auto endChar = FindDelimiter(nameStart);
				if (endChar == nullptr)
					continue; // invalid line
				char endCharValue = *endChar;
				*endChar = '\0';

				if (endCharValue == '\0')
					continue; // invalud line

				if (endCharValue != '=')
				{
					endChar = SkipSpace(endChar);
					if (*endChar != '=')
						continue; // invalud line
				}

				auto numberStart = SkipSpace(endChar + 1);
				if (*numberStart == '\0')
					continue; // invalid line

				auto maskValue = strtol(numberStart, nullptr, 16);
				StrUtil::StringLower(nameStart, sizeof(stringBuffer));
				SetMask(nameStart, (uint)maskValue);
			}

			// Apply to log module
			ApplyMask();
		}

		void ApplyMask()
		{
			Service::LogModule->SetPrintParameter(m_Parameter);

			auto pConsole = Engine::GetEngineComponent<LogOutputConsoleComponent>();
			if (pConsole != nullptr) pConsole->GetHandler().SetOutputMask(OutputConsole);

			auto pDebugger = Engine::GetEngineComponent<LogOutputDebuggerComponent>();
			if (pDebugger != nullptr) pDebugger->GetHandler().SetOutputMask(OutputDebugger);

			auto pFile = Engine::GetEngineComponent<LogOutputFileComponent>();
			if (pFile != nullptr) pFile->GetHandler().SetOutputMask(OutputFile);
		}

		// Object task
		virtual Result OnTick(EngineTaskTick tick) override
		{
			if (Util::TimeSince(m_MaskUpdated) > DurationMS(60 * 1000))
			{
				LoadTraceConfig();
				m_MaskUpdated = Util::Time.GetTimeMs();
			}

			return ResultCode::SUCCESS;
		}
	};



	ServerLogComponent::ServerLogComponent(const char* traceCfgPath)
		: LibraryComponent("ServerLogComponent")
	{
		StrUtil::StringCopy(m_ConfigFilePath, traceCfgPath);
	}

	ServerLogComponent::~ServerLogComponent()
	{
	}

	// Initialize server component
	Result ServerLogComponent::InitializeComponent()
	{
		Result hr = LibraryComponent::InitializeComponent();
		if (!(hr)) return hr;


		auto pLogObject = new(GetEngineHeap()) ServerLogObject(m_ConfigFilePath);
		m_LogObject = pLogObject;
		pLogObject->SetTickGroup(EngineTaskTick::AsyncTick);


		return hr;
	}

	// Terminate server component
	void ServerLogComponent::DeinitializeComponent()
	{
		LibraryComponent::DeinitializeComponent();

		m_LogObject->Dispose();
		m_LogObject = nullptr;
	}


};	// namespace SF


