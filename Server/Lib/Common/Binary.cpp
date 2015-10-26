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
#include "Common/Binary.h"
#include "Common/BrAssert.h"





namespace BR
{

	Binary::Binary( const Binary& src )
	{
		src;
		AssertRel( !"Do not use this operation!, Use BinaryHeader::CopyBinary" );
	}

	// Copy operator
	Binary& Binary::operator = ( const Binary& src )
	{
		src;
		AssertRel( !"Do not use this operation!, Use BinaryHeader::CopyBinary" );
		return *this;
	}


	// Copy
	HRESULT Binary::CopyBinary( Binary& dst, const Binary& src )
	{
		if( dst.MaxSize < src.Size )
			return E_ABORT;

		if( dst.Magic != src.Magic )
			return E_INVALIDARG;

		memcpy( &dst.Version, &src.Version, src.Size - (UINT)((intptr_t)&dst.Version - (intptr_t)&dst) );

		return S_OK;
	}


};	// namespace BR


