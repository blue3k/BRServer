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
	//Result BLOBBuffer::Allocate( UINT uiSize )
	//{
	//	if( pBlobData )
	//		delete[] pBlobData;
	//	pBlobData = NULL;

	//	if( uiSize > 0 )
	//	{
	//		pBlobData = new BYTE[uiSize];
	//		if( pBlobData == NULL )
	//			return ResultCode::OUT_OF_MEMORY;
	//	}

	//	cbSize = uiSize;

	//	return ResultCode::SUCCESS;
	//}

	//// Reallocate given size of buffer 
	//Result BLOBBuffer::Reallocate( UINT uiSize )
	//{
	//	BYTE *pBlobDataTem = NULL;

	//	if( uiSize > 0 )
	//	{
	//		pBlobDataTem = new BYTE[uiSize];
	//		if( pBlobDataTem == NULL )
	//			return ResultCode::OUT_OF_MEMORY;
	//	}

	//	if( cbSize && pBlobData )
	//		memcpy( pBlobDataTem, pBlobData, cbSize );

	//	cbSize = uiSize;


	//	if( pBlobData )
	//		delete[] pBlobData;
	//	pBlobData = pBlobDataTem;

	//	return ResultCode::SUCCESS;
	//}


	void Dummp_SvrTypes() {}
	


}; // namespace BR {

