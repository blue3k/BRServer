////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Binary stream
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/Binary.h"
#include "Common/ArrayUtil.h"

namespace BR {

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Output Memory stream
	//

	class OutputMemoryStream
	{
	public:

	private:

		// Memoryy space size
		UINT m_BufferSize;

		BYTE* m_Buffer;

		// Writen size
		UINT m_WritingPosition;


	public:
		// Constructor
		OutputMemoryStream();
		OutputMemoryStream(UINT bufferSize, BYTE* buffer);

		void SetStreamBuffer(UINT bufferSize, BYTE* buffer);

		// Get target memory buffer size
		UINT GetBufferSize();

		// Get writen size
		UINT GetSize();

		// Get remain size
		UINT GetRemainSize();

		// Get memory buffer pointer
		const BYTE* GetData();

		// Write data to stream
		HRESULT Write( UINT sizeToWrite, const BYTE* pData );
	};

	
	template<size_t BufferSize>
	class StaticOutputMemoryStream : public OutputMemoryStream
	{
	private:
		StaticArray<BYTE, BufferSize> m_Buffer;

	public:
		StaticOutputMemoryStream()
		{
			SetStreamBuffer(BufferSize, m_Buffer.data());
		}

		Array<BYTE>& ToArray()
		{
			m_Buffer.SetSize(GetSize());
			return m_Buffer;
		}
	};

#include "MemoryStream.inl"


}; // namespace BR



