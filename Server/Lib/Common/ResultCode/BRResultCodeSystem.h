////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Blue3k
// 
// Author : KyungKun Ko
//
// Description :  System predefined error code, same value with HRESULT
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stdint.h>


namespace BR
{
	namespace ResultCode
	{
		enum {
			SUCCESS					= 0x00000000L,
			SUCCESS_FALSE			= 0x00000001L,
			INVALID_POINTER			= 0xC0000002L,
			FAIL					= 0xC0000003L,
			NOT_IMPLEMENTED			= 0xC0000004L,
			OUT_OF_MEMORY			= 0xC0000005L,
			UNEXPECTED				= 0xC0000006L,
			INVALID_ARG				= 0xC0000007L,
		};
	};
}