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

#include "Common/BrBaseTypes.h"
#include "Common/ResultCode/BRResultCodeNet.h"
#include "Net/NetDef.h"
#include "Common/StrUtil.h"

namespace BR {
namespace Protocol {

	// parsing helper
	template< class SrcType >
	inline void PackParamCopy( BYTE* &pMsgCur, SrcType* pSrc, INT iParamSize );


	// parsing helper
	template< class DataType >
	inline Result StreamParamCopy( DataType* pDst, BYTE* &pMsgCur, INT &iMsgSize, INT iParamSize );

	template< class DataType >
	inline Result StreamParamLnk( DataType* &pDst, BYTE* &pMsgCur, INT &iMsgSize, INT iParamSize );


#include "ProtocolHelper.inl"


}; // namespace Protocol
}; // SR


