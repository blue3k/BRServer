////////////////////////////////////////////////////////////////////////
//
// FixedString
//

#include "stdafx.h"
#include <stdint.h>
#include <assert.h>
#include <fstream>
#include <algorithm>

#include "String/Hasher32.h"
#include "String/Hasher64.h"
#include "String/FixedStringDB.h"


constexpr uint32_t Hasher_Crc32::stm_CRC32Table[8][256];


const char* FixedStringDB::DefaultString = "NotInStringDB";


const char* FixedStringDB::StringBuffer::AddString(const char* string, size_t strLen)
{
	if (string == nullptr || RemainSize < strLen)
		return nullptr;

	char* startPos = Buffer + (BufferSize - RemainSize);
	memcpy(startPos, string, strLen);
	startPos[strLen] = '\0';

	RemainSize -= strLen + 1; // Include null terminate

	return startPos;
}


FixedStringDB::FixedStringDB()
	: m_Head(0)
	, m_StringMap32(2048)
	, m_StringMap64(2048)
{
}

FixedStringDB::~FixedStringDB()
{
	Clear();
}


// Clear and release all memory
void FixedStringDB::Clear()
{
	auto pCurBlock = m_Head;
	while (pCurBlock != nullptr)
	{
		auto pNext = pCurBlock->Next;
		free(pCurBlock->Buffer - sizeof(StringBuffer));
		pCurBlock = pNext;
	}
	m_Head = nullptr;

	m_StringMap32.clear();
	m_StringMap64.clear();
}

// Merge all string buffer into one
bool FixedStringDB::MergeStringBuffer()
{
	// if nothing to merge
	if (m_Head == nullptr || m_Head->Next == nullptr)
		return true;


	// calculate total string buffer size
	uint64_t stringBufferSize = 0;
	auto pCurBlock = m_Head;
	while (pCurBlock != nullptr)
	{
		auto pNext = pCurBlock->Next;
		stringBufferSize += pCurBlock->BufferSize - pCurBlock->RemainSize;
		pCurBlock = pNext;
	}

	uint64_t allocateSize = std::max(stringBufferSize, DefaultBufferSize);
	char* pBuffer = (char*)malloc(allocateSize + sizeof(StringBuffer));
	StringBuffer * newBuffer = new(pBuffer) StringBuffer(allocateSize, pBuffer + sizeof(StringBuffer));

	// Copy all strings into new buffer, and replace pointer
	for (auto itTbl : m_StringMap32)
	{
		auto newAddr = newBuffer->AddString(itTbl.second, strlen(itTbl.second));
		itTbl.second = newAddr;
	}

	for (auto itTbl : m_StringMap64)
	{
		// Skip duplicated strings
		uint32_t hash32Value = Hash32(itTbl.second);
		const char* newAddr = nullptr;

		auto itFound = m_StringMap32.find(hash32Value);
		if (itFound != m_StringMap32.end())
			newAddr = itFound->second;
		else
			newAddr = newBuffer->AddString(itTbl.second, strlen(itTbl.second));
		itTbl.second = newAddr;
	}


	// Delete all current buffers and replace it
	pCurBlock = m_Head;
	while (pCurBlock != nullptr)
	{
		auto pNext = pCurBlock->Next;
		free(pCurBlock->Buffer - sizeof(StringBuffer));
		pCurBlock = pNext;
	}
	m_Head = newBuffer;

	return true;
}

// Load string table file
bool FixedStringDB::LoadStringTable(std::istream& stream)
{
	uint64_t stringBufferSize;
	int32_t numHash;
	uint64_t stringOffset;

	// TODO: error handling

	// load strings
	stream >> stringBufferSize;

	uint64_t allocateSize = std::max(stringBufferSize, DefaultBufferSize);
	char* pBuffer = (char*)malloc(allocateSize + sizeof(StringBuffer));
	StringBuffer * newBuffer = new(pBuffer) StringBuffer(allocateSize, pBuffer + sizeof(StringBuffer));


	newBuffer->Next = m_Head;
	m_Head = newBuffer;

	stream.read(newBuffer->Buffer, stringBufferSize);
	newBuffer->RemainSize = newBuffer->BufferSize - stringBufferSize;


	// load hash32
	stream.read((char*)&numHash, sizeof(numHash));
	for (int iHash = 0; iHash < numHash; iHash++)
	{
		uint32_t hash32;
		stream.read((char*)&hash32, sizeof(hash32));
		stream.read((char*)&stringOffset, sizeof(stringOffset));

		const char* stringPtr = newBuffer->Buffer + stringOffset;
		if(m_StringMap32.find(hash32) == m_StringMap32.end())
			m_StringMap32.insert(std::make_pair(hash32, stringPtr));
	}

	// load hash64
	stream.read((char*)&numHash, sizeof(numHash));
	for (int iHash = 0; iHash < numHash; iHash++)
	{
		uint64_t hash64;
		stream.read((char*)&hash64, sizeof(hash64));
		stream.read((char*)&stringOffset, sizeof(stringOffset));
		const char* stringPtr = newBuffer->Buffer + stringOffset;
		if (m_StringMap64.find(hash64) == m_StringMap64.end())
			m_StringMap64.insert(std::make_pair(hash64, stringPtr));
	}

	return true;
}

// Save current strings to string table
bool FixedStringDB::SaveStringTable(std::ostream& stream)
{
	uint64_t stringBufferSize;
	int32_t numHash;

	// If nothing to save
	if (m_Head == nullptr)
	{
		return true;
	}

	MergeStringBuffer();

	// save string buffer
	stringBufferSize = m_Head->BufferSize - m_Head->RemainSize;
	stream << stringBufferSize;
	stream.write(m_Head->Buffer, stringBufferSize);


	// save hash32 table
	numHash = (decltype(numHash))m_StringMap32.size();
	stream.write((const char*)&numHash, sizeof(numHash));
	for(auto itTbl : m_StringMap32)
	{
		uint64_t stringOffset = (decltype(stringOffset))((intptr_t)itTbl.second - (intptr_t)m_Head->Buffer);
		uint32_t hash32 = itTbl.first;
		stream.write((const char*)&hash32, sizeof(hash32));
		stream.write((const char*)&stringOffset, sizeof(stringOffset));
	}

	// save hash64
	numHash = (decltype(numHash))m_StringMap64.size();
	stream.write((const char*)&numHash, sizeof(numHash));
	for (auto itTbl : m_StringMap64)
	{
		uint64_t stringOffset = (decltype(stringOffset))((intptr_t)itTbl.second - (intptr_t)m_Head->Buffer);
		uint64_t hash64 = itTbl.first;
		stream.write((const char*)&hash64, sizeof(hash64));
		stream.write((const char*)&stringOffset, sizeof(stringOffset));
	}

	return true;
}


const char* FixedStringDB::AddStringToBuffer(const char* string)
{
	if (string == nullptr)
		return nullptr;

	size_t strLen = strlen(string);
	const char* newAddr = nullptr;

	// find free space from existing buffers
	auto pCurBlock = m_Head;
	while (pCurBlock != nullptr && newAddr == nullptr)
	{
		auto pNext = pCurBlock->Next;
		newAddr = pCurBlock->AddString(string, strLen);
		pCurBlock = pNext;
	}

	// if it's added successfully
	if (newAddr != nullptr)
		return newAddr;

	// we need more buffer
	uint64_t allocateSize = DefaultBufferSize;
	char* pBuffer = (char*)malloc(allocateSize + sizeof(StringBuffer));
	StringBuffer * newBuffer = new(pBuffer) StringBuffer(allocateSize, pBuffer + sizeof(StringBuffer));

	newAddr = newBuffer->AddString(string, strLen);
	// it should be assigned at this moment
	assert(newAddr != nullptr);

	newBuffer->Next = m_Head;
	m_Head = newBuffer;

	return newAddr;
}

// Add string to both 32 and 64 hash
void FixedStringDB::AddString(const char* str)
{
	uint32_t hash32Value = Hash32(str);
	uint64_t hash64Value = Hash64(str);

	auto itFound32 = m_StringMap32.find(hash32Value);
	auto itFound64 = m_StringMap64.find(hash64Value);

	if (itFound64 != m_StringMap64.end())
	{
		// make sure 32 table has the mapping and return
		const char* pStr = itFound64->second;
		if (itFound32 == m_StringMap32.end())
		{
			m_StringMap32.insert(std::make_pair(hash32Value, pStr));
		}

		return;
	}

	if (itFound32 != m_StringMap32.end())
	{
		const char* pStr = itFound32->second;
		// make sure 64 table has the mapping and return
		if (itFound64 == m_StringMap64.end())
		{
			m_StringMap64.insert(std::make_pair(hash32Value, pStr));
		}

		return;
	}

	// Both table don't have it
	const char* newAddr = AddStringToBuffer(str);
	m_StringMap32.insert(std::make_pair(hash32Value, newAddr));
	m_StringMap64.insert(std::make_pair(hash64Value, newAddr));

}

// Add string to specific table
uint32_t FixedStringDB::AddNGetString32(const char* str)
{
	uint32_t hash32Value = Hash32(str);

	auto itFound32 = m_StringMap32.find(hash32Value);
	if (itFound32 != m_StringMap32.end())
	{
		return hash32Value;
	}

	// Both table don't have it
	const char* newAddr = AddStringToBuffer(str);
	m_StringMap32.insert(std::make_pair(hash32Value, newAddr));

	return hash32Value;
}

uint64_t FixedStringDB::AddNGetString(const char* str)
{
	uint64_t hash64Value = Hash64(str);

	auto itFound64 = m_StringMap64.find(hash64Value);
	if (itFound64 != m_StringMap64.end())
	{
		return hash64Value;
	}

	// Both table don't have it
	const char* newAddr = AddStringToBuffer(str);
	m_StringMap64.insert(std::make_pair(hash64Value, newAddr));

	return hash64Value;
}

const char* FixedStringDB::GetString(uint32_t hash)
{
	if (hash == 0)
		return nullptr;

	auto itFound = m_StringMap32.find(hash);
	if (itFound == m_StringMap32.end())
	{
		return DefaultString;
	}

	return itFound->second;
}

const char* FixedStringDB::GetString(uint64_t hash)
{
	if (hash == 0)
		return nullptr;

	auto itFound = m_StringMap64.find(hash);
	if (itFound == m_StringMap64.end())
	{
		return DefaultString;
	}

	return itFound->second;
}




