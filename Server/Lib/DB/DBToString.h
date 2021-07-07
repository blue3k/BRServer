////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : KyungKun Ko
//
// Description : DB type tostring 
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Util/SFToString.h"

#include "MySqlHeaders.h"

namespace SF {


#if !ANDROID

	inline Result operator >> (IInputStream& input, mysqlx::Type& data) { return input.Read(&data, sizeof(data)); }
	inline Result operator << (IOutputStream& output, const mysqlx::Type& data) { return output.Write(&data, sizeof(data)); }


	inline Result _ToString(ToStringContext& context, const mysqlx::Type& Data)
	{
		const char* name = mysqlx::typeName(Data);

		if( name )
		{
			if( !( StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, name ) ) )
				return ResultCode::FAIL;
		}
		else
			return _IToA( context, (uint32_t)Data );

		return ResultCode::SUCCESS;
	}

	DECLARE_BOXING_TEMPLETE_BYVALUE(mysqlx::Type);

#endif


}; // namespace SF



