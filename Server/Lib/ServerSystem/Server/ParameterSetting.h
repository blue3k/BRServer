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

#include "SFTypedefs.h"
#include "Container/OrderedLinkedList.h"
#include "String/FixedString.h"


namespace SF {


	class ParameterSetting
	{
	private:

		struct LinkedListNode : public OrderedLinkedList<FixedString>::Node
		{
			const char* Value;
		};

		static int m_BufferUsedOffset;
		static char m_SettingBuffer[128 * 1024];

		static OrderedLinkedList<FixedString> m_Settings;

	private:

		static Result ProcessSingleParameter(const char* argument);

	public:

		// Prcess parameter
		static Result ProcessParameter(int numArg, const char* argc[]);
		static Result ProcessParameter(const char* arg);

		// Set setting value
		static Result SetSetting(const char* settingName, const char* value);

		// Get setting value
		static const char* GetSetting(const char* settingName, const char* defaultValue = "");
	};


}
