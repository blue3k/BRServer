////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Class Utility library
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"




namespace BR {
namespace Util {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Generic utility macros
	//

	// Define Attribute
	#define BRCLASS_ATTRIBUTE(AttrType,AttrName)		\
			private:\
				AttrType m_##AttrName;\
			public:\
				FORCEINLINE const AttrType& Get##AttrName() const				{ return m_##AttrName; }\
				FORCEINLINE void Set##AttrName( const AttrType& newValue )		{ m_##AttrName = newValue; }


	// Define pointer Attribute
	#define BRCLASS_ATTRIBUTE_PTR(AttrType,AttrName)		\
			private:\
				AttrType* m_##AttrName;\
			public:\
				FORCEINLINE AttrType* Get##AttrName() const					{ return m_##AttrName; }\
				FORCEINLINE void Set##AttrName( AttrType* newValue )			{ m_##AttrName = newValue; }


	// Define Attribute with r-value operation
	#define BRCLASS_ATTRIBUTE_RVALUE(AttrType,AttrName)		\
			private:\
				AttrType m_##AttrName;\
			public:\
				FORCEINLINE const AttrType& Get##AttrName() const				{ return m_##AttrName; }\
				FORCEINLINE void Set##AttrName( const AttrType& newValue )		{ m_##AttrName = newValue; }\
				FORCEINLINE void Set##AttrName( AttrType&& newValue )			{ m_##AttrName = newValue; }


	// Define Attribute without set method
	#define BRCLASS_ATTRIBUTE_READONLY(AttrType,AttrName)		\
			private:\
				AttrType m_##AttrName;\
			public:\
				FORCEINLINE AttrType Get##AttrName () const						{ return m_##AttrName; }


	// Define Attribute without set method
	#define BRCLASS_ATTRIBUTE_CONST(AttrType,AttrName)		\
			private:\
				AttrType m_##AttrName;\
			public:\
				FORCEINLINE const AttrType& Get##AttrName () const				{ return m_##AttrName; }


	// Define Attribute without set method, use with ptr type
	#define BRCLASS_ATTRIBUTE_READONLY_PTR(AttrType,AttrName)		\
			private:\
				AttrType m_##AttrName;\
			public:\
				inline AttrType Get##AttrName()							{ return m_##AttrName; }


	// Define Attribute without set method
	#define BRCLASS_ATTRIBUTE_STRING(AttrName,StringLen)		\
			private:\
				char m_##AttrName[StringLen];\
			public:\
				FORCEINLINE const char* Get##AttrName() const					{ return m_##AttrName; }\
				FORCEINLINE Result Set##AttrName( const char* newValue )		{ return StrUtil::StringCpy( m_##AttrName, newValue ); }

	
	///////////////////////////////////////////////////////////////////////
	//
	//	Casting
	//

#ifdef DEBUG
	#define BR_DYNAMIC_CAST(type,objPtr)								dynamic_cast<type>(objPtr)
#else
	#define BR_DYNAMIC_CAST(type,objPtr)								( ((objPtr) != nullptr ? (type)(objPtr) : nullptr) )
#endif

}; // namespace Util
}  // namespace BR



