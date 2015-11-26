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
inline UINT OutputMemoryStream::GetBufferSize()
{
	return m_BufferSize;
}

// Get writen size
inline UINT OutputMemoryStream::GetSize()
{
	return m_WritingPosition;
}

// Get remain size
inline UINT OutputMemoryStream::GetRemainSize()
{
	return m_BufferSize - m_WritingPosition;
}

// Get memory buffer pointer
inline const BYTE* OutputMemoryStream::GetData()
{
	return m_Buffer;
}




