////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Protocol parser helper
//	
//
////////////////////////////////////////////////////////////////////////////////





// parsing helper
template< class SrcType >
void PackParamCopy( BYTE* &pMsgCur, SrcType* pSrc, INT iParamSize )
{
	memcpy( pMsgCur, pSrc, iParamSize );
	pMsgCur += iParamSize;
}


// parsing helper
template< class DataType >
HRESULT StreamParamCopy( DataType* pDst, BYTE* &pMsgCur, INT &iCurMsgSize, INT iParamSize )
{
	if( iCurMsgSize < iParamSize ) return E_NET_BADPACKET_SIZE;// sizeCheck

	memcpy( pDst, pMsgCur, iParamSize );

	pMsgCur += iParamSize;
	iCurMsgSize -= iParamSize;

	return S_SYSTEM_OK;
}

template< class DataType >
HRESULT StreamParamLnk( DataType* &pDst, BYTE* &pMsgCur, INT &iCurMsgSize, INT iParamSize )
{
	if( iCurMsgSize < iParamSize ) return E_NET_BADPACKET_SIZE;// sizeCheck

	if( iParamSize > 0 )
	{
		pDst = (DataType*)pMsgCur;

		pMsgCur += iParamSize;
		iCurMsgSize -= iParamSize;
	}
	else
		pDst = NULL;

	return S_SYSTEM_OK;
}

template<>
inline HRESULT StreamParamLnk( const char* &pDst, BYTE* &pMsgCur, INT &iCurMsgSize, INT iParamSize )
{
	if( iCurMsgSize < iParamSize ) return E_NET_BADPACKET_SIZE;// sizeCheck

	if( iParamSize > 0 )
	{
		pDst = (char*)pMsgCur;

		pMsgCur += iParamSize;
		iCurMsgSize -= iParamSize;
	}
	else
		pDst = StrUtil::EmptyString;

	return S_SYSTEM_OK;
}



