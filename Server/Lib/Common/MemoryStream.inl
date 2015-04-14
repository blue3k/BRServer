////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Brave base types - binary
//
////////////////////////////////////////////////////////////////////////////////



// Get target memory buffer size
UINT OutputMemoryStream::GetBufferSize()
{
	return m_BufferSize;
}

// Get writen size
UINT OutputMemoryStream::GetSize()
{
	return m_WritingPosition;
}

// Get remain size
UINT OutputMemoryStream::GetRemainSize()
{
	return m_BufferSize - m_WritingPosition;
}

// Get memory buffer pointer
const BYTE* OutputMemoryStream::GetData()
{
	return m_Buffer;
}




