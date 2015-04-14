////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Memory allocation library
//	
//
////////////////////////////////////////////////////////////////////////////////



#pragma once


#include "Typedefs.h"
#include "BrAssert.h"



namespace BR
{
	
	struct MemBlockHdr
	{
		enum { 
			MEM_MAGIC = 0x187e16c5,
			MEM_MAGIC_FREE = 0xb3ad84af,
		};
		intptr_t uiSize;
		union {
			UINT32 uiMagic;
			CounterType Padding;
		};
	};


}; // namespace BR



