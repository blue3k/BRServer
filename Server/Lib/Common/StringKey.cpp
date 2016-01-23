////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 MadK
// 
// Author : KyungKun Ko
//
// Description : String key
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Trace.h"
#include "Common/BrAssert.h"
#include "Common/StringKey.h"
#include "Common/StrUtil.h"
#include "Common/Indexing.h"




namespace BR
{

	StringKey::StringKey()
		: m_Hash(0)
		, m_Length(0)
		, m_String(nullptr)
	{
	}

	StringKey::StringKey(const char* str)
		: m_Hash(0)
		, m_Length(0)
		, m_String(str)
	{
		if(str != nullptr)
			m_Length = strlen(str);
	}

	StringKey::StringKey(const StringKey& src)
		: m_Hash(src.m_Hash)
		, m_Length(src.m_Length)
		, m_String(src.m_String)
	{
	}

	int StringKey::CompareRawString(const char* strCmp, size_t length) const
	{
		if (m_String == nullptr)
		{
			return strCmp == nullptr ? 0 : -1;
		}

		if (strCmp == nullptr)
			return -1;

		return StrUtil::StringCmp(m_String, (INT)m_Length, strCmp, (INT)length);
	}

	size_t StringKey::GetHash() const
	{
		if (m_Hash != 0) return m_Hash;

		m_Hash = Hash::hash<const char*>()(m_String);

		assert(m_Hash != 0);

		return m_Hash;
	}


}; // namespace BR




