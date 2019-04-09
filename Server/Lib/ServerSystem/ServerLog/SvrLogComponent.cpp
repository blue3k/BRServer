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


#include "ServerSystemPCH.h"
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
		// config file path
		char m_ConfigFilePath[256];
		time_t m_LastConfigTimeStamp = 0;
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


		void SetMask(FixedString32 maskName, uint32_t maskValue)
		{
			switch (maskName)
			{
			case "default"_crc:
			case "def"_crc:					m_Parameter.MainChannelMasks[(int)LogMainChannels::System] = maskValue; break;
			case "net"_crc:					m_Parameter.MainChannelMasks[(int)LogMainChannels::Net] = maskValue; break;
			case "db"_crc:					m_Parameter.MainChannelMasks[(int)LogMainChannels::DB] = maskValue; break;
			case "svr"_crc:					m_Parameter.MainChannelMasks[(int)LogMainChannels::Svr] = maskValue; break;
			case "protocol"_crc:			m_Parameter.MainChannelMasks[(int)LogMainChannels::Protocol] = maskValue; break;
			case "outputdebugger"_crc:		OutputDebugger = maskValue; break;
			case "outputconsole"_crc:		OutputConsole = maskValue; break;
			case "outputfile"_crc:			OutputFile = maskValue; break;
			case "outputdbgfile"_crc:		break;
			default:
				break;
			}
		}

		void SetMask(FixedString32 maskName, FixedString32 channelName)
		{
			uint32_t mask = 0;
			switch (channelName)
			{
			case "error"_crc:		mask = 1 << static_cast<int>(LogSubChannelType::Error); break;
			case "warning"_crc:		mask = 1 << static_cast<int>(LogSubChannelType::Warning); break;
			case "assert"_crc:		mask = 1 << static_cast<int>(LogSubChannelType::Assert); break;
			case "info"_crc:		mask = 1 << static_cast<int>(LogSubChannelType::Info); break;
			case "custom1"_crc:		mask = 1 << static_cast<int>(LogSubChannelType::Custom1); break;
			case "custom2"_crc:		mask = 1 << static_cast<int>(LogSubChannelType::Custom2); break;
			case "custom3"_crc:		mask = 1 << static_cast<int>(LogSubChannelType::Custom3); break;
			case "custom4"_crc:		mask = 1 << static_cast<int>(LogSubChannelType::Custom4); break;
			case "custom5"_crc:		mask = 1 << static_cast<int>(LogSubChannelType::Custom5); break;
			case "custom6"_crc:		mask = 1 << static_cast<int>(LogSubChannelType::Custom6); break;
			case "custom7"_crc:		mask = 1 << static_cast<int>(LogSubChannelType::Custom7); break;
			case "custom8"_crc:		mask = 1 << static_cast<int>(LogSubChannelType::Custom8); break;
			case "custom9"_crc:		mask = 1 << static_cast<int>(LogSubChannelType::Custom9); break;
			case "custom10"_crc:	mask = 1 << static_cast<int>(LogSubChannelType::Custom10); break;
			case "debug1"_crc:		mask = 1 << static_cast<int>(LogSubChannelType::Debug1); break;
			case "debug2"_crc:		mask = 1 << static_cast<int>(LogSubChannelType::Debug2); break;
			case "debug3"_crc:		mask = 1 << static_cast<int>(LogSubChannelType::Debug3); break;
			case "debug4"_crc:		mask = 1 << static_cast<int>(LogSubChannelType::Debug4); break;
			case "debug5"_crc:		mask = 1 << static_cast<int>(LogSubChannelType::Debug5); break;
			case "debug6"_crc:		mask = 1 << static_cast<int>(LogSubChannelType::Debug6); break;
			case "debug7"_crc:		mask = 1 << static_cast<int>(LogSubChannelType::Debug7); break;
			case "debug8"_crc:		mask = 1 << static_cast<int>(LogSubChannelType::Debug8); break;
			case "allgeneral"_crc:	mask = 0x000000FL; break;
			case "allcustom"_crc:	mask = 0x0003FF0L; break;
			case "alldebug"_crc:	mask = 0xFFFC000L; break;
			case "all"_crc:			mask = 0xFFFFFFFL; break;
			}


			switch (maskName)
			{
			case "default"_crc:
			case "def"_crc:					m_Parameter.MainChannelMasks[(int)LogMainChannels::System].Composited |= mask; break;
			case "net"_crc:					m_Parameter.MainChannelMasks[(int)LogMainChannels::Net].Composited |= mask; break;
			case "db"_crc:					m_Parameter.MainChannelMasks[(int)LogMainChannels::DB].Composited |= mask; break;
			case "svr"_crc:					m_Parameter.MainChannelMasks[(int)LogMainChannels::Svr].Composited |= mask; break;
			case "protocol"_crc:			m_Parameter.MainChannelMasks[(int)LogMainChannels::Protocol].Composited |= mask; break;
			case "outputdebugger"_crc:		OutputDebugger.Composited |= mask; break;
			case "outputconsole"_crc:		OutputConsole.Composited |= mask; break;
			case "outputfile"_crc:			OutputFile.Composited |= mask; break;
			case "outputdbgfile"_crc:		break;
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
				if (curCharValue == '/')
				{
					// Comment line, null terminate and return
					*curChar = '\0';
					return curChar;
				}

				curChar++;
				curCharValue = *curChar;
			}
			return curChar;
		}

		char* FindValueDelimiter(char* curChar)
		{
			if (curChar == nullptr) return nullptr;

			char curCharValue = *curChar;
			do 
			{
				switch (curCharValue)
				{
				case ' ': // White space, we need to null terminate it anyway
				case '\n':
				case '\r':
				case '\t':
					*curChar = '\0';
					break;
				case ',': // We found comma, terminate
					*curChar = '\0';
					return curChar+1;
				case '/': // comment is end of line
					*curChar = '\0';
					// fallthru
				case '\0': // Null terminate, no more search
					return curChar;
				}

				curCharValue = *++curChar;
			} while (true);

			return curChar;
		}

		void LoadTraceConfig()
		{
			std::string strCfgPath = Util::GetModulePath();
			strCfgPath.append(m_ConfigFilePath);

			// Load when file time changed
			struct stat st;
			int ierr = stat(strCfgPath.c_str(), &st);
			if (m_LastConfigTimeStamp == st.st_mtime)
				return;

			m_LastConfigTimeStamp = st.st_mtime;

			FILE *file = fopen(strCfgPath.c_str(), "r");
			if (file == nullptr)
				return;

			char stringBuffer[4096];
			while (fgets(stringBuffer, sizeof(stringBuffer), file) != nullptr)
			{
				char *curChar = stringBuffer;
				StrUtil::StringLower(stringBuffer, sizeof(stringBuffer));

				auto nameStart = SkipSpace(curChar);

				auto endChar = FindDelimiter(nameStart);
				if (endChar == nullptr)
					continue; // invalid line
				char endCharValue = *endChar;
				*endChar = '\0';

				if (endCharValue == '\0')
					continue; // invalid line

				if (endCharValue != '=')
				{
					endChar = SkipSpace(endChar);
					if (*endChar != '=')
						continue; // invalid line
				}

				FixedString32 nameCrc = nameStart;
				SetMask(nameCrc, (uint)0);

				auto curValueStart = SkipSpace(endChar + 1);
				while (curValueStart != nullptr && curValueStart[0] != '\0')
				{
					curValueStart = SkipSpace(curValueStart);
					if (curValueStart == nullptr || *curValueStart == '\0' || *curValueStart == '\r' || *curValueStart == '\n')
						break; // end of channel

					auto curValueEnd = FindValueDelimiter(curValueStart);
					SetMask(nameCrc, curValueStart);
					curValueStart = curValueEnd;
				}

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
			if (Util::TimeSince(m_MaskUpdated) > DurationMS(5 * 1000))
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


