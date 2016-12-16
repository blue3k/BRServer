////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Blue3k
// 
// Author : MadK
//
// Description : Debug definitions
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include <stdint.h>


namespace BR
{

	typedef union _Result
	{
		enum {
			CodeBits		= 16,
			FacilityBits	= 12,
			ReservedBits	= 1,
			CustomBits		= 1,
			SeverityBits	= 2,
		};

		struct
		{
			int32_t Code : CodeBits;
			int32_t Facility : FacilityBits;
			int32_t Reserved : ReservedBits;
			int32_t Custom : CustomBits;
			int32_t Severity : SeverityBits;
		} ByBits;

		// Result code value
		int32_t HResultCode;


		_Result()
			: HResultCode(0)
		{
		}

		_Result(int32_t ResultCode)
			: HResultCode(ResultCode)
		{
		}

		explicit operator int32_t() const { return HResultCode; }

		operator bool() const { return HResultCode >= 0; }

		_Result& operator = (const _Result& op)
		{
			HResultCode = op.HResultCode;
			return *this;
		}

		bool operator == (const _Result& op) const
		{
			return HResultCode == op.HResultCode;
		}
		bool operator != (const _Result& op) const
		{
			return HResultCode != op.HResultCode;
		}

		bool operator == (const int32_t& op) const
		{
			return HResultCode == op;
		}
		bool operator != (const int32_t& op) const
		{
			return HResultCode != op;
		}
	} Result;
}
