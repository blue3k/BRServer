////////////////////////////////////////////////////////////////////////
//
// FixedString
//


#pragma  once

#include <stdint.h>
#include "Hasher64.h"

// MSVC compiler generates warning 4307 with constexpr functions which can't be avoided
#ifdef _MSC_VER
#pragma warning(disable : 4307)
#endif

class FixedString
{
private:

	uint64_t m_Hash;

public:
	constexpr FixedString()
		: m_Hash(0)
	{

	}

	constexpr FixedString(uint64_t hash)
		: m_Hash(hash)
	{

	}

	constexpr FixedString(const char* text)
		: m_Hash(Hasher_Murmur64().Hash(text))
	{

	}

	constexpr uint64_t GetHash() const
	{
		return m_Hash;
	}

	constexpr operator uint64_t() const
	{
		return m_Hash;
	}

};



