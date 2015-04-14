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

		// Indexing Constant member function Functor
		template<typename ClassType,typename FuncRtnType,typename FuncRtnType (ClassType::*pfMemFunc)()const,typename IndexType = FuncRtnType>
		class ConstMemFunc
		{
		public:
			typedef ClassType ClassType;
			typedef IndexType Type;

			IndexType operator ()(const ClassType& clsObj) const
			{
				return (Type)(clsObj.*pfMemFunc)();
			}

			IndexType operator ()(const ClassType* clsObj) const
			{
				return (Type)(clsObj->*(pfMemFunc))();
			}
		};
		
		// Indexing Constant member function Functor
		template<typename ClassType,typename FuncRtnType,typename const FuncRtnType& (ClassType::*pfMemFunc)()const,typename IndexType = FuncRtnType>
		class ConstMemRefFunc
		{
		public:
			typedef ClassType ClassType;
			typedef IndexType Type;

			IndexType operator ()(const ClassType& clsObj) const
			{
				return (Type)(clsObj.*pfMemFunc)();
			}

			IndexType operator ()(const ClassType* clsObj) const
			{
				return (Type)(clsObj->*(pfMemFunc))();
			}
		};

		// Indexing member function
		template<typename ClassType,typename FuncRtnType,typename FuncRtnType (ClassType::*pfMemFunc)(),typename IndexType = FuncRtnType>
		class MemFunc
		{
		public:
			typedef ClassType ClassType;
			typedef IndexType Type;

			IndexType operator ()(ClassType& clsObj) const
			{
				return (Type)(clsObj.*pfMemFunc)();
			}

			IndexType operator ()(ClassType* clsObj) const
			{
				return (Type)(clsObj->*(pfMemFunc))();
			}
		};

		// Indexing member data
		template<typename ClassType,typename MemDataType,MemDataType ClassType::*pfMemData,typename IndexType = MemDataType>
		class MemData
		{
		public:
			typedef ClassType ClassType;
			typedef IndexType Type;

			IndexType operator ()(const ClassType& clsObj) const
			{
				return (Type)(clsObj.*pfMemData);
			}

			IndexType operator ()(const ClassType* clsObj) const
			{
				return (Type)(clsObj->*(pfMemData));
			}
		};
		
		// Indexing member data
		template<typename ClassType,typename MemDataType,MemDataType ClassType::*pfMemData>
		class MapItemConverter
		{
		public:
			typedef ClassType ClassType;
			typedef typename MemDataType Type;

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
			typedef ClassType ClassType;
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

		// Indexing Composit 2 index with hashing
		template<typename ClassType,typename Index1, typename Index2>
		class CompositHash
		{
		public:
			typedef ClassType ClassType;
			typedef size_t Type;

			typedef std::tr1::hash<typename Index1::Type> Hasher1;
			typedef std::tr1::hash<typename Index2::Type> Hasher2;

			size_t operator ()(const ClassType& clsObj) const
			{
				size_t carry = Hasher1()(Index1()(clsObj)) + 0x9e3779b9;
				carry^=Hasher2()(Index2()(clsObj))+0x9e3779b9+(carry<<6)+(carry>>2);
				return carry;
			}

			size_t operator ()(const ClassType* clsObj) const
			{
				size_t carry = Hasher1()(Index1()(clsObj)) + 0x9e3779b9;
				carry^=Hasher2()(Index2()(clsObj))+0x9e3779b9+(carry<<6)+(carry>>2);
				return carry;
			}
		};

		// Combine two 32bit value to 64bit value
		template<typename ClassType,typename Index1, typename Index2>
		class Composit64
		{
		public:
			typedef ClassType ClassType;
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
				return std::tr1::hash<KeyType>()(_Keyval);
			}
		};


		template<>
		class hash<sockaddr_in6>
		{
		public:
			typedef sockaddr_in6 KeyType;
			typedef std::tr1::hash<ULONG> StdHash;

			const unsigned MAGIC = 0x9e3779b9;

			size_t operator()(const KeyType& _Keyval) const
			{
				size_t carry = StdHash()(_Keyval.sin6_port) + MAGIC;

				for (int iAddr = 0; iAddr < _countof(_Keyval.sin6_addr.u.Word); iAddr++)
				{
					carry ^= StdHash()((ULONG)(_Keyval.sin6_addr.u.Word[iAddr])) + MAGIC;
				}

				return carry;
			}
		};


		template<>
		class hash<ULONGLONG>
		{
		public:
			typedef ULONGLONG KeyType;
			typedef std::tr1::hash<ULONG> StdHash;

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
			size_t operator()(const typename IndexType::ClassType& _Keyval) const
			{
				return std::hash()(IndexType()(_Keyval));
			}
		};


		// functor for operator==
		template< typename IndexType >
		struct equal_to
		{
			bool operator()(const typename IndexType& _Left, const typename IndexType& _Right) const
			{
				return IndexType(_Left) == IndexType(_Right);
			}

			bool operator()(const typename IndexType* _Left, const typename IndexType* _Right) const
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




