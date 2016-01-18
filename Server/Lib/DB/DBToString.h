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

#include "Common/Typedefs.h"
#include "Common/ToString.h"

#include "my_global.h"
#include "mysql.h"


namespace BR {
	

	template<>
	inline HRESULT ToString( char*& pBuff, INT& iBuffLen, const enum_field_types& Data, int Option )
	{
		const char* name = nullptr;

		switch( Data )
		{
		case MYSQL_TYPE_DECIMAL: name = "MYSQL_TYPE_DECIMAL";break;
		case MYSQL_TYPE_TINY: name = "MYSQL_TYPE_TINY";break;
		case MYSQL_TYPE_SHORT: name = "MYSQL_TYPE_SHORT";break;
		case MYSQL_TYPE_LONG: name = "MYSQL_TYPE_LONG";break;
		case MYSQL_TYPE_FLOAT: name = "MYSQL_TYPE_FLOAT";break;
		case MYSQL_TYPE_DOUBLE: name = "MYSQL_TYPE_DOUBLE";break;
		case MYSQL_TYPE_NULL: name = "MYSQL_TYPE_NULL";break;
		case MYSQL_TYPE_TIMESTAMP: name = "MYSQL_TYPE_TIMESTAMP";break;
		case MYSQL_TYPE_LONGLONG: name = "MYSQL_TYPE_LONGLONG";break;
		case MYSQL_TYPE_INT24: name = "MYSQL_TYPE_INT24";break;
		case MYSQL_TYPE_DATE: name = "MYSQL_TYPE_DATE";break;
		case MYSQL_TYPE_TIME: name = "MYSQL_TYPE_TIME";break;
		case MYSQL_TYPE_DATETIME: name = "MYSQL_TYPE_DATETIME";break;
		case MYSQL_TYPE_YEAR: name = "MYSQL_TYPE_YEAR";break;
		case MYSQL_TYPE_NEWDATE: name = "MYSQL_TYPE_NEWDATE";break;
		case MYSQL_TYPE_VARCHAR: name = "MYSQL_TYPE_VARCHAR";break;
		case MYSQL_TYPE_BIT: name = "MYSQL_TYPE_BIT";break;
		case MYSQL_TYPE_TIMESTAMP2: name = "MYSQL_TYPE_TIMESTAMP2";break;
		case MYSQL_TYPE_DATETIME2: name = "MYSQL_TYPE_DATETIME2";break;
		case MYSQL_TYPE_TIME2: name = "MYSQL_TYPE_TIME2";break;
		case MYSQL_TYPE_NEWDECIMAL: name = "MYSQL_TYPE_NEWDECIMAL";break;
		case MYSQL_TYPE_ENUM: name = "MYSQL_TYPE_ENUM";break;
		case MYSQL_TYPE_SET: name = "MYSQL_TYPE_SET";break;
		case MYSQL_TYPE_TINY_BLOB: name = "MYSQL_TYPE_TINY_BLOB";break;
		case MYSQL_TYPE_MEDIUM_BLOB: name = "MYSQL_TYPE_MEDIUM_BLOB";break;
		case MYSQL_TYPE_LONG_BLOB: name = "MYSQL_TYPE_LONG_BLOB";break;
		case MYSQL_TYPE_BLOB: name = "MYSQL_TYPE_BLOB";break;
		case MYSQL_TYPE_VAR_STRING: name = "MYSQL_TYPE_VAR_STRING";break;
		case MYSQL_TYPE_STRING: name = "MYSQL_TYPE_STRING";break;
		case MYSQL_TYPE_GEOMETRY: name = "MYSQL_TYPE_GEOMETRY";break;
		};

		unused(Option);
		if( name )
		{
			if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, name ) ) )
				return E_SYSTEM_FAIL;
		}
		else
			return ToStringEnum( pBuff, iBuffLen, Data );

		return S_OK;
	}


}; // namespace BR



