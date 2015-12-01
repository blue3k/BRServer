////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : indexing helpers
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/StrUtil.h"



namespace BR
{
	namespace Indexing
	{

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Indexing Functor
		//

		// Indexing member data
		template<typename ClassType,typename MemDataType,MemDataType ClassType::*pfMemData>
		class MapItemConverter
		{
		public:
			typedef MemDataType Type;

			Type* operator ()(const ClassType& clsObj) const
			{
				return (Type*)&(clsObj.*pfMemData);
			}

			Type* operator ()(const ClassType* clsObj) const
			{
				return (Type*)&(clsObj->*(pfMemData));
			}

			ClassType* operator ()(const MemDataType* memData) const
			{
				if( memData == nullptr ) return nullptr;
				return (ClassType*)( (intptr_t)memData - (intptr_t)&(((ClassType *)0)->*pfMemData) );
			}
		};


		// bypassing Self value
		template<typename ClassType,typename IndexType = ClassType>
		class Self
		{
		public:
			typedef IndexType Type;

			IndexType operator ()(const ClassType& clsObj) const
			{
				return (Type)clsObj;
			}

			IndexType operator ()(const ClassType* clsObj) const
			{
				return (Type)(*clsObj);
			}
		};


		// Combine two 32bit value to 64bit value
		template<typename ClassType,typename Index1, typename Index2>
		class Composit64
		{
		public:
			typedef ULONGLONG Type;

			ULONGLONG operator ()(const ClassType& clsObj) const
			{
				ULONGLONG ulIndex = Index1()(clsObj);
				ulIndex = (ulIndex<<32) | (ULONGLONG)(Index2()(clsObj));
				return ulIndex;
			}

			ULONGLONG operator ()(const ClassType* clsObj) const
			{
				ULONGLONG ulIndex = Index1()(clsObj);
				ulIndex = (ulIndex<<32) | (ULONGLONG)(Index2()(clsObj));
				return ulIndex;
			}
		};

		// Combine two 32bit value to 64bit value
		inline ULONGLONG Combine64( ULONG ulIdx, ULONG ulIdx2 )
		{
			ULONGLONG ulIndex = ulIdx;
			ulIndex = (ulIndex<<32) | (ULONGLONG)(ulIdx2);
			return ulIndex;
		}


		
	}; // namespace Indexing



	namespace Hash
	{
		///////////////////////////////////////////////////////////////////////////
		//
		//	Hash specialize
		//

		// default hash to std hash
		template< class KeyType >
		class hash
		{
		public:
			size_t operator()(const KeyType& _Keyval) const
			{
				return std::hash<KeyType>()(_Keyval);
			}
		};


		template<>
		class hash<sockaddr_in6>
		{
		public:
			typedef sockaddr_in6 KeyType;
			typedef std::hash<ULONG> StdHash;

			const unsigned MAGIC = 0x9e3779b9;

			size_t operator()(const KeyType& _Keyval) const
			{
				size_t carry = StdHash()(_Keyval.sin6_port) + MAGIC;

				auto& rawAddress = _Keyval.sin6_addr.s6_addr;
				auto check = carry;
				for (UINT iAddr = 0; iAddr < countof(rawAddress); iAddr++)
				{
					check ^= StdHash()((ULONG)(rawAddress[iAddr])) + MAGIC;
				}

				return carry;
			}
		};


		template<>
		class hash<ULONGLONG>
		{
		public:
			typedef ULONGLONG KeyType;
			typedef std::hash<ULONG> StdHash;

			size_t operator()(const KeyType& _Keyval) const
			{
				size_t carry = StdHash()((ULONG)(_Keyval>>32)) + 0x9e3779b9;
				carry^= StdHash()((ULONG)_Keyval)+0x9e3779b9+(carry<<6)+(carry>>2);

				return carry;
			}
		};
		
		template<>
		class hash<const char*>
		{
		public:
			typedef const char* KeyType;

			size_t operator()(const KeyType& _Keyval) const
			{
				size_t Value = 2166136261U;
				const char* pCur = _Keyval;

				if( pCur == nullptr )
					return Value;

				for( ; *pCur ; pCur++ )
				{
					Value = 16777619U * Value ^ (size_t)(*pCur);
				}

				return Value;
			}
		};




		///////////////////////////////////////////////////////////////////////////
		//
		//	Hasher
		//

		// std hash functor wrapper
		template< typename IndexType >
		class Hasher
		{
		private:

		public:
			size_t operator()(const IndexType& _Keyval) const
			{
				return std::hash<IndexType>()(_Keyval);
			}
		};


		// functor for operator==
		template< typename IndexType >
		struct equal_to
		{
			bool operator()(const IndexType& _Left, const IndexType& _Right) const
			{
				return IndexType(_Left) == IndexType(_Right);
			}

			bool operator()(const IndexType* _Left, const IndexType* _Right) const
			{
				return IndexType(_Left) == IndexType(_Right);
			}
		};


		template<>
		struct equal_to<const char*>
		{
			bool operator()(const char* _Left, const char* _Right) const
			{
				const int iMaxCmp = 8*1024;

				return StrUtil::StringCmp( _Left, _Right, iMaxCmp ) == 0;
			}

		};


	}; // namespace Hash

}; // namespace BR




