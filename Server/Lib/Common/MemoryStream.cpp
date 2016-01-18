////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : 
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/BrAssert.h"
#include "Common/MemoryStream.h"





namespace BR
{

	// Constructor
	OutputMemoryStream::OutputMemoryStream()
		:m_BufferSize(0), m_Buffer(nullptr), m_WritingPosition(0)
	{
	}

	OutputMemoryStream::OutputMemoryStream(UINT bufferSize, BYTE* buffer)
		: m_BufferSize(bufferSize), m_Buffer(buffer), m_WritingPosition(0)
	{
	}

	void OutputMemoryStream::SetStreamBuffer(UINT bufferSize, BYTE* buffer)
	{
		m_BufferSize = bufferSize;
		m_Buffer = buffer;
	}

	// Write data to stream
	HRESULT OutputMemoryStream::Write( UINT sizeToWrite, const BYTE* pData )
	{
		if( pData == nullptr )
			return E_SYSTEM_INVALIDARG;

		if( (m_WritingPosition + sizeToWrite) > m_BufferSize )
			return E_SYSTEM_OUTOFMEMORY;

		memcpy( m_Buffer + m_WritingPosition, pData, sizeToWrite );
		m_WritingPosition += sizeToWrite;

		return S_OK;
	}

};	// namespace BR


