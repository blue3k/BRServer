////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Protocol parser helper
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Types/BrBaseTypes.h"
#include "ResultCode/SFResultCodeNet.h"
#include "Net/NetDef.h"
#include "String/StrUtil.h"

namespace BR {
namespace Protocol {

	// parsing helper
	template< class SrcType >
	inline void PackParamCopy( uint8_t* &pMsgCur, SrcType* pSrc, INT iParamSize );


	// parsing helper
	template< class DataType >
	inline Result StreamParamCopy( DataType* pDst, uint8_t* &pMsgCur, INT &iMsgSize, INT iParamSize );

	template< class DataType >
	inline Result StreamParamLnk( DataType* &pDst, uint8_t* &pMsgCur, INT &iMsgSize, INT iParamSize );


#include "ProtocolHelper.inl"


}; // namespace Protocol
}; // SR


