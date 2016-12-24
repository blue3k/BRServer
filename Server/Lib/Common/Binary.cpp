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
#include "Common/ResultCode/BRResultCodeSystem.h"




namespace BR
{

	//Binary::Binary( const Binary& src )
	//{
	//	//unused(src);
	//	AssertRel( !"Do not use this operation!, Use BinaryHeader::CopyBinary" );
	//}

	// Copy operator
	Binary& Binary::operator = ( const Binary& src )
	{
		unused(src);
		AssertRel( !"Do not use this operation!, Use BinaryHeader::CopyBinary" );
		return *this;
	}


	// Copy
	Result Binary::CopyBinary( Binary& dst, const Binary& src )
	{
		if( dst.MaxSize < src.Size )
			return ResultCode::INVALID_ARG;

		if( dst.Magic != src.Magic )
			return ResultCode::INVALID_ARG;

		memcpy( &dst.Version, &src.Version, src.Size - (UINT)((intptr_t)&dst.Version - (intptr_t)&dst) );

		return ResultCode::SUCCESS;
	}


};	// namespace BR


