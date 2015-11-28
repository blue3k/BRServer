////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Server Type implementations. 
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/StrUtil.h"
#include "ServerSystem/SvrTypes.h"




namespace BR {



	//////////////////////////////////////////////////////////////////////////////////
	////
	////	BLOB wrapper class, control memory from CRT heap
	////


	//// Allocate given size of buffer 
	//HRESULT BLOBBuffer::Allocate( UINT uiSize )
	//{
	//	if( pBlobData )
	//		delete[] pBlobData;
	//	pBlobData = NULL;

	//	if( uiSize > 0 )
	//	{
	//		pBlobData = new BYTE[uiSize];
	//		if( pBlobData == NULL )
	//			return E_OUTOFMEMORY;
	//	}

	//	cbSize = uiSize;

	//	return S_OK;
	//}

	//// Reallocate given size of buffer 
	//HRESULT BLOBBuffer::Reallocate( UINT uiSize )
	//{
	//	BYTE *pBlobDataTem = NULL;

	//	if( uiSize > 0 )
	//	{
	//		pBlobDataTem = new BYTE[uiSize];
	//		if( pBlobDataTem == NULL )
	//			return E_OUTOFMEMORY;
	//	}

	//	if( cbSize && pBlobData )
	//		memcpy( pBlobDataTem, pBlobData, cbSize );

	//	cbSize = uiSize;


	//	if( pBlobData )
	//		delete[] pBlobData;
	//	pBlobData = pBlobDataTem;

	//	return S_OK;
	//}


	void Dummp_SvrTypes() {}
	


}; // namespace BR {

