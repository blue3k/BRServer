////////////////////////////////////////////////////////////////////////
//
// FixedString
//

#include <stdint.h>
#include <unordered_map>
#include <algorithm>



class FixedStringDB
{
public:

	const uint64_t DefaultBufferSize = 512 * 1024;
	static const char* DefaultString;

private:

	// String buffer linked list
	struct StringBuffer
	{
		size_t BufferSize;
		size_t RemainSize;
		StringBuffer *Next;
		char* Buffer;

		StringBuffer(size_t bufferSize, char* pBuffer)
			: BufferSize(bufferSize)
			, RemainSize(bufferSize)
			, Buffer(pBuffer)
			, Next(nullptr)
		{
			memset(Buffer, 0, BufferSize);
		}

		// Add string to buffer and return new string address
		const char* AddString(const char* string, size_t strLen);
	};

	// String buffer linked list
	StringBuffer *m_Head;
	

	std::unordered_map<uint32_t, const char*> m_StringMap32;

	std::unordered_map<uint64_t, const char*> m_StringMap64;

private:

	const char* AddStringToBuffer(const char* string);

public:
	FixedStringDB();
	~FixedStringDB();

	// Clear and release all memory
	void Clear();

	size_t GetStringCount() {
		return std::max(m_StringMap32.size(), m_StringMap64.size());
	}

	// Merge all string buffer into one
	bool MergeStringBuffer();

	// Load string table file
	bool LoadStringTable(std::istream& stream);

	// Save current strings to string table
	bool SaveStringTable(std::ostream& stream);

	// Add string to both 32 and 64 hash
	void AddString(const char* str);

	// Add string to specific table
	uint32_t AddNGetString32(const char* str);
	uint64_t AddNGetString(const char* str);

	const char* GetString(uint32_t hash);
	const char* GetString(uint64_t hash);

};



