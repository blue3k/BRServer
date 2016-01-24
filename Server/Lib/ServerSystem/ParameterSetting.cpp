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
#include "Common/Utility.h"
#include "Common/StrParserUtil.h"
#include "Common/StrUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/ParameterSetting.h"


namespace BR
{

	int ParameterSetting::m_BufferUsedOffset = 0;
	char ParameterSetting::m_SettingBuffer[128 * 1024];

	OrderedLinkedList<StringKey> ParameterSetting::m_Settings;


	HRESULT ParameterSetting::ProcessSingleParameter(const char* argument)
	{
		HRESULT hr = S_SYSTEM_OK;

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

	HRESULT ParameterSetting::ProcessParameter(const char* arg)
	{
		HRESULT hr = S_SYSTEM_OK;
		char* curArg = nullptr;
		char settingBufferForParsing[64 * 1024];

		if (arg == nullptr) return E_SYSTEM_INVALIDARG;

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

	HRESULT ParameterSetting::ProcessParameter(int numArg, const char* argc[])
	{
		HRESULT hr = S_SYSTEM_OK;

		for (int iArg = 0; iArg < numArg; iArg++)
		{
			svrChk(ProcessSingleParameter(argc[iArg]));
		}

	Proc_End:

		return hr;
	}


	// Set setting value
	HRESULT ParameterSetting::SetSetting(const char* settingName, const char* value)
	{
		HRESULT hr = S_SYSTEM_OK;

		char* bufferPos = m_SettingBuffer + m_BufferUsedOffset;
		INT bufferSize = (INT)countof(m_SettingBuffer) - m_BufferUsedOffset;
		StringKey key;
		decltype(m_Settings)::Node *pPrevNode = nullptr;
		LinkedListNode *pNewNode = nullptr;

		char* curSettingValue = bufferPos;
		svrChk(StrUtil::StringCpyEx(bufferPos, bufferSize, value));
		*bufferPos++ = '\0'; bufferSize--;

		key = StringKey(settingName);
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
			if (bufferSize < sizeof(LinkedListNode))
				return E_SYSTEM_OUTOFMEMORY;

			pNewNode = (LinkedListNode*)bufferPos;
			bufferPos += sizeof(LinkedListNode);
			bufferSize -= sizeof(LinkedListNode);

			memset(pNewNode, 0, sizeof(LinkedListNode));
			pNewNode->Key = StringKey(curSettingName);
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
		StringKey key(settingName);

		if (FAILED(m_Settings.FindPrevNode(key, pPrevNode)))
			return defaultValue;

		pCur = pPrevNode->pNext;
		if(pCur == nullptr || pCur->Key != key)
			return defaultValue;

		return ((LinkedListNode*)pCur)->Value;
	}


};	// namespace BR

