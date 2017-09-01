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
#include "SFAssert.h"
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
	Result OutputMemoryStream::Write( UINT sizeToWrite, const BYTE* pData )
	{
		if( pData == nullptr )
			return ResultCode::INVALID_ARG;

		if( (m_WritingPosition + sizeToWrite) > m_BufferSize )
			return ResultCode::OUT_OF_MEMORY;

		memcpy( m_Buffer + m_WritingPosition, pData, sizeToWrite );
		m_WritingPosition += sizeToWrite;

		return ResultCode::SUCCESS;
	}

};	// namespace BR


