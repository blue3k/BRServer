////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server Type inline implementations. 
//	
//
////////////////////////////////////////////////////////////////////////////////



//
//
//////////////////////////////////////////////////////////////////////////////////
////
////	BLOB wrapper class 
////
//
//BLOBBuffer::BLOBBuffer()
//{
//	cbSize = 0;
//	pBlobData = NULL;
//}
//
//
//BLOBBuffer::BLOBBuffer( UINT uiSize, const BYTE *pData )
//{
//	cbSize = 0;
//	pBlobData = NULL;
//
//	if( uiSize > 0 && pData != NULL && Allocate( uiSize ) )
//	{
//		cbSize = uiSize;
//		memcpy( pBlobData, pData, uiSize );
//	}
//}
//
//BLOBBuffer::BLOBBuffer( const BLOBBuffer& src )
//{
//	cbSize = 0;
//	pBlobData = NULL;
//
//	if( src.cbSize > 0 && src.pBlobData != NULL && Allocate( src.cbSize ) )
//	{
//		cbSize = src.cbSize;
//		memcpy( pBlobData, src.pBlobData, src.cbSize );
//	}
//}
//
//
//BLOBBuffer::BLOBBuffer( const BLOB& src )
//{
//	cbSize = 0;
//	pBlobData = NULL;
//
//	if( src.cbSize > 0 && src.pBlobData != NULL && Allocate( src.cbSize ) )
//	{
//		cbSize = src.cbSize;
//		memcpy( pBlobData, src.pBlobData, src.cbSize );
//	}
//}
//
//BLOBBuffer::~BLOBBuffer()
//{
//	if( pBlobData )
//		delete[] pBlobData;
//}
//
//
//BLOBBuffer::operator LPBLOB() const
//{
//	return (BLOB*)this;
//}
//
//BLOBBuffer& BLOBBuffer::operator = ( const BLOBBuffer& src )
//{
//	if( src.cbSize > 0 && src.pBlobData != NULL && Reallocate( src.cbSize ) )
//	{
//		cbSize = src.cbSize;
//		memcpy( pBlobData, src.pBlobData, src.cbSize );
//	}
//
//	return *this;
//}
//
//BLOBBuffer& BLOBBuffer::operator = ( const BLOB& src )
//{
//	if( src.cbSize > 0 && src.pBlobData != NULL && Reallocate( src.cbSize ) )
//	{
//		cbSize = src.cbSize;
//		memcpy( pBlobData, src.pBlobData, src.cbSize );
//	}
//
//	return *this;
//}
//
//








