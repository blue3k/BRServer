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
#include "SFAssert.h"
#include "ServerLog/SvrLog.h"
#include "Util/Utility.h"
#include "String/StrParserUtil.h"
#include "String/StrUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "Server/ParameterSetting.h"


namespace SF
{

	int ParameterSetting::m_BufferUsedOffset = 0;
	char ParameterSetting::m_SettingBuffer[128 * 1024];

	OrderedLinkedList<FixedString> ParameterSetting::m_Settings;


	Result ParameterSetting::ProcessSingleParameter(const char* argument)
	{
		Result hr = ResultCode::SUCCESS;

		switch (argument[0])
		{
		case '-':
			argument++;
			if (StrUtil::StringCmpLwr(argument, (INT)strlen(argument), "install", (INT)strlen("install")) == 0)
			{
				svrChk(SetSetting("install", "true"));
			}
			else if (argument[0] == 'n' || argument[0] == 'N')
			{
				argument += 2;
				svrChk(SetSetting("servicename", argument));
			}
			else if (argument[0] == 'u' || argument[0] == 'U')
			{
				argument += 2;
				svrChk(SetSetting("user", argument));
			}
			else if (argument[0] == 'p' || argument[0] == 'P')
			{
				argument += 2;
				svrChk(SetSetting("password", argument));
			}
			else if (StrUtil::StringCmpLwr(argument, (INT)strlen(argument), "uninstall", (INT)strlen("uninstall")) == 0)
			{
				svrChk(SetSetting("uninstall", "true"));
			}
			else if (StrUtil::StringCmpLwr(argument, (INT)strlen(argument), "debug", (INT)strlen("debug")) == 0)
			{
				svrChk(SetSetting("debug", "true"));
			}
			break;
		default:
			break;
		};


	Proc_End:

		return hr;
	}

	Result ParameterSetting::ProcessParameter(const char* arg)
	{
		Result hr = ResultCode::SUCCESS;
		char* curArg = nullptr;
		char settingBufferForParsing[64 * 1024];

		if (arg == nullptr) return ResultCode::INVALID_ARG;

		svrChk(StrUtil::StringCpy(settingBufferForParsing, arg));

		curArg = StrUtil::SkipWhiteSpace(settingBufferForParsing);
		while (curArg != nullptr)
		{
			char* endOfArg = StrUtil::NextWhiteSpace(curArg);
			if (endOfArg != nullptr)
				*endOfArg = '\0';

			svrChk(ProcessSingleParameter(curArg));

			if (endOfArg == nullptr)
				break;

			curArg = StrUtil::SkipWhiteSpace(endOfArg+1);
		}

	Proc_End:

		return hr;
	}

	Result ParameterSetting::ProcessParameter(int numArg, const char* argc[])
	{
		Result hr = ResultCode::SUCCESS;

		for (int iArg = 0; iArg < numArg; iArg++)
		{
			svrChk(ProcessSingleParameter(argc[iArg]));
		}

	Proc_End:

		return hr;
	}


	// Set setting value
	Result ParameterSetting::SetSetting(const char* settingName, const char* value)
	{
		Result hr = ResultCode::SUCCESS;

		char* bufferPos = m_SettingBuffer + m_BufferUsedOffset;
		INT bufferSize = (INT)countof(m_SettingBuffer) - m_BufferUsedOffset;
		FixedString key;
		decltype(m_Settings)::Node *pPrevNode = nullptr;
		LinkedListNode *pNewNode = nullptr;

		char* curSettingValue = bufferPos;
		svrChk(StrUtil::StringCpyEx(bufferPos, bufferSize, value));
		*bufferPos++ = '\0'; bufferSize--;

		key = FixedString(settingName);
		svrChk(m_Settings.FindPrevNode(key, pPrevNode));
		if (pPrevNode->Key == key)
		{
			// We just need to replace the value
			pNewNode = (LinkedListNode*)pPrevNode;
			pNewNode->Value = curSettingValue;
		}
		else
		{
			// Add new value
			char* curSettingName = bufferPos;
			svrChk(StrUtil::StringLwrEx(bufferPos, bufferSize, settingName));
			*bufferPos++ = '\0'; bufferSize--;
			if (bufferSize < (INT)sizeof(LinkedListNode))
				return ResultCode::OUT_OF_MEMORY;

			pNewNode = (LinkedListNode*)bufferPos;
			bufferPos += sizeof(LinkedListNode);
			bufferSize = (decltype(bufferSize))(bufferSize - sizeof(LinkedListNode));

			memset(pNewNode, 0, sizeof(LinkedListNode));
			pNewNode->Key = FixedString(curSettingName);
			pNewNode->Value = curSettingValue;

			svrChk(m_Settings.Insert(pPrevNode, pNewNode->Key, pNewNode));
		}

		m_BufferUsedOffset = (INT)countof(m_SettingBuffer) - bufferSize;

	Proc_End:

		return hr;
	}

	// Get setting value
	const char* ParameterSetting::GetSetting(const char* settingName, const char* defaultValue)
	{
		decltype(m_Settings)::Node *pPrevNode = nullptr, *pCur = nullptr;
		FixedString key(settingName);

		if (!(m_Settings.FindPrevNode(key, pPrevNode)))
			return defaultValue;

		pCur = pPrevNode->pNext;
		if(pCur == nullptr || pCur->Key != key)
			return defaultValue;

		return ((LinkedListNode*)pCur)->Value;
	}


};	// namespace SF


