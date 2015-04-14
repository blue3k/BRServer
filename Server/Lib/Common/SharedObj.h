////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : shared object
//
////////////////////////////////////////////////////////////////////////////////


#pragma once 

#include "Common/Typedefs.h"
#include "Common/Synchronize.h"



namespace BR
{

	/////////////////////////////////////////////////////////////////////
	//
	//	Multiple referenced object interface define
	//

	interface ISharedObj
	{
		// Reference count
		mutable SyncCounter	m_RefCount;
		mutable void* m_pOwner;

		ISharedObj();
		virtual ~ISharedObj();

		inline CounterType GetRefCount() const;

		inline virtual void AddRef(void* pOwner = nullptr) const;
		inline virtual void Release() const;

		virtual void DeleteThis() const = 0;
	};



	/////////////////////////////////////////////////////////////////////
	//
	//	Multiple referenced object
	//

	class SharedObj : public ISharedObj
	{
	public:
		SharedObj();
		virtual ~SharedObj();
		//virtual void AddRef() const;
		//virtual void Release() const;
		//unsigned int GetRefCount() const;
		virtual void DeleteThis() const;

	private:
		//mutable LONG m_lRefCount;
	};


	/////////////////////////////////////////////////////////////////////
	//
	//	Multiple referenced object pointer manage class
	//

	template<class SharedObjType>
	class SharedPtr
	{
	public:

		// hash functor
		class hash
		{
		private:
			std::tr1::hash<const SharedObjType*> realHash;

		public:
			size_t operator()(const SharedPtr<SharedObjType>& _Keyval) const
			{
				const SharedObjType* pPtr = _Keyval;
				return realHash(pPtr);
			}
		};

		// functor for operator==
		struct equal_to
		{
			bool operator()(const SharedPtr<SharedObjType>& _Left, const SharedPtr<SharedObjType>& _Right) const
			{
				return ((const SharedObjType*)_Left == (const SharedObjType*)_Right);
			}
		};


	private:
		SharedObjType *m_pObj;
		ISharedObj *m_pRefObj;

	public:
		SharedPtr();
		SharedPtr( SharedObjType *pObj );
		template<class Type2>
		SharedPtr( Type2 *pObj )
		{
			m_pObj = dynamic_cast<SharedObjType*>(pObj);
			if( m_pObj ) m_pObj->AddRef();
			m_pRefObj = m_pObj;
		}
		SharedPtr( const SharedPtr<SharedObjType> &spSrc );
		template<class Type2>
		SharedPtr( const SharedPtr<Type2> &spSrc )
		{
			SharedObjType *pNewObj = dynamic_cast<SharedObjType*>((Type2*)(const Type2*)spSrc);
			m_pObj = dynamic_cast<SharedObjType*>(pNewObj);
			if( m_pObj ) m_pObj->AddRef();
			m_pRefObj = m_pObj;
		}
		virtual ~SharedPtr();


		SharedObjType *operator ->();
		const SharedObjType *operator ->() const;

		SharedObjType& operator *();
		const SharedObjType& operator *() const;

		operator SharedObjType* ();
		operator const SharedObjType* () const;

		// Copy operator
		SharedPtr<SharedObjType>& operator=( const SharedPtr<SharedObjType> &src );
		SharedPtr<SharedObjType>& operator=( const SharedObjType *pSrc ) const;
		SharedPtr<SharedObjType>& operator=( SharedObjType *pSrc );

		template<class SharedObjType2>
		SharedPtr<SharedObjType>& operator=( SharedPtr<SharedObjType2>& ptr)
		{
			SharedObjType *pOldPtr = m_pObj;

			SharedObjType *pNewObj = dynamic_cast<SharedObjType*>((SharedObjType2*)(const SharedObjType2*)ptr);
			if( pNewObj == m_pObj )
				return *this;

			if( m_pObj ) m_pObj->Release();

			m_pObj = pNewObj;
			m_pRefObj = m_pObj;

			if( m_pObj ) m_pObj->AddRef();

			return *this;
		}
	};


#ifndef SRSmartIPtr

    #define SRSmartIPtr(classname)						\
    class classname;										\
	typedef BR::SharedPtr<classname> classname##Ptr; \

#endif // SRSmartPointer



	#include "SharedObj.inl"


}; // namespace BR

