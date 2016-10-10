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
#include "Common/OrderedLinkedList.h"
#include "Common/StringKey.h"


namespace BR {


	class ParameterSetting
	{
	private:

		struct LinkedListNode : public OrderedLinkedList<StringKey>::Node
		{
			const char* Value;
		};

		static int m_BufferUsedOffset;
		static char m_SettingBuffer[128 * 1024];

		static OrderedLinkedList<StringKey> m_Settings;

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

