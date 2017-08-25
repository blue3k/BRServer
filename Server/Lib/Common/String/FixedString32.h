////////////////////////////////////////////////////////////////////////
//
// FixedString
//

#pragma  once

#include <stdint.h>
#include "Hasher32.h"



class FixedString32
{
private:

	uint32_t m_Crc;

public:
	constexpr FixedString32()
		: m_Crc(0)
	{

	}

	constexpr FixedString32(uint32_t stringCrc)
		: m_Crc(stringCrc)
	{

	}

	constexpr FixedString32(const char* text)
		: m_Crc(Hasher_Crc32().Hash(text))
	{

	}

	constexpr uint32_t GetHash() const
	{
		return m_Crc;
	}

	constexpr operator uint32_t() const
	{
		return m_Crc;
	}

};

typedef FixedString32 string_crc;

