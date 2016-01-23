////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : String key
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"



namespace BR
{
	// String key value, It doesn't make a copy of the string, so managing string buffer is user's job
	class StringKey
	{
	private:

		mutable size_t m_Hash;
		size_t m_Length;
		const char* m_String;

	private:

		int CompareRawString(const char* strCmp, size_t length)  const;

	public:

		StringKey();
		StringKey(const char* str);
		StringKey(const StringKey& src);

		size_t GetHash() const;

		StringKey& operator = (const StringKey& src)
		{
			m_Hash = src.m_Hash;
			m_Length = src.m_Length;
			m_String = src.m_String;
			return *this;
		}

		bool operator == (const StringKey& src) { return GetHash() == src.GetHash() && CompareRawString(src.m_String, src.m_Length) == 0; }
		bool operator != (const StringKey& src) { return GetHash() != src.GetHash() || CompareRawString(src.m_String, src.m_Length) != 0; }
		bool operator <= (const StringKey& src) { return GetHash() <= src.GetHash() && CompareRawString(src.m_String, src.m_Length) <= 0; }
		bool operator >= (const StringKey& src) { return GetHash() >= src.GetHash() && CompareRawString(src.m_String, src.m_Length) >= 0; }
		bool operator < (const StringKey& src)  { return GetHash() < src.GetHash() && CompareRawString(src.m_String, src.m_Length) < 0; }
		bool operator > (const StringKey& src)  { return GetHash() > src.GetHash() && CompareRawString(src.m_String, src.m_Length) > 0; }
	};

}; // namespace BR




