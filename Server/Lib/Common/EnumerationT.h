////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2010 Bluenauts
// 
// Author : KyungKun Ko
// Date : 12/19/2010
//
// Description : SR enumeration wrapper base
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"


namespace SR
{
	//////////////////////////////////////////////////////////////////////////////////////
	//
	//	Enumeration base
	//

	// Implementation of enumeration
#define ENUMERATIONMAP_BEGIN(EnumType)\
	class  EnumType##EnumMap : public std::tr1::unordered_map<std::string, EnumType>\
	{\
	private:\
		static EnumType##EnumMap stm_Map;\
	public:\
		typedef iterator EnumMapItr;\
		enum { INVALID_ENUM_VALUE = -1 };\
		EnumType##EnumMap()\
		{\


#define ENUMERATIONMAP_ENUMITEM(EnumValue)\
	insert(std::make_pair(#EnumValue, EnumValue));\


#define ENUMERATIONMAP_END(EnumType) \
		}\
		static const std::string& ToString ( EnumType value )\
		{ \
			static std::string strInvalid = "INVALID_ENUM_VALUE";\
			\
 			for( EnumMapItr itr = stm_Map.begin(); itr != stm_Map.end(); ++itr )\
			{\
 				if (itr->second == value)\
					return itr->first;\
			}\
			return strInvalid;\
		}\
		static EnumType ToEnum ( const std::string& strValue )\
		{ \
			EnumMapItr itr = stm_Mp.find(strValue);\
			if( itr != stm_Map.end() )\
				return itr->second;\
				\
			return (EnumType)INVALID_ENUM_VALUE;\
		}\
	};\


#define ENUMERATIONMAP_IMPLEMENT(EnumType) \
	EnumType##EnumMap stm_Map;\



#include "EnumerationT.inl"


}; // namespace Svr



