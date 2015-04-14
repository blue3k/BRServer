////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Bluenatus
// 
// Author : KyungKun Ko
//
// Description : shared object
//
////////////////////////////////////////////////////////////////////////////////



#pragma intrinsic (_InterlockedIncrement, _InterlockedDecrement)


inline ISharedObj::ISharedObj()
: m_RefCount(0)
, m_pOwner(nullptr)
{
}

inline ISharedObj::~ISharedObj()
{
}

CounterType ISharedObj::GetRefCount() const
{
	return m_RefCount;
}

void ISharedObj::AddRef(void* pOwner) const
{
	m_pOwner = pOwner;
	m_RefCount.fetch_add(1,std::memory_order_acquire);
}

void ISharedObj::Release() const
{
	if (m_RefCount.fetch_sub(1,std::memory_order_release) <= 1)
		DeleteThis();
}




inline SharedObj::SharedObj()
{
}

inline SharedObj::~SharedObj()
{
}



inline void SharedObj::DeleteThis() const
{
	delete this;
}





/////////////////////////////////////////////////////////////////////
//
//	Multiple referenced object pointer manage class
//

template<class SharedObjType>
SharedPtr<SharedObjType>::SharedPtr()
:m_pObj(NULL),
m_pRefObj(NULL)
{
}

template<class SharedObjType>
SharedPtr<SharedObjType>::SharedPtr( SharedObjType *pObj )
:m_pObj(pObj)
{
	if( m_pObj ) m_pObj->AddRef();
	m_pRefObj = m_pObj;
}

template<class SharedObjType>
SharedPtr<SharedObjType>::SharedPtr( const SharedPtr<SharedObjType> &spSrc )
:m_pObj(spSrc.m_pObj)
{
	if( m_pObj ) m_pObj->AddRef();
	m_pRefObj = m_pObj;
}

template<class SharedObjType>
SharedPtr<SharedObjType>::~SharedPtr()
{
	if( m_pRefObj ) m_pRefObj->Release();
}

template<class SharedObjType>
SharedObjType *SharedPtr<SharedObjType>::operator ->()
{
	return m_pObj;
}

template<class SharedObjType>
const SharedObjType *SharedPtr<SharedObjType>::operator ->() const
{
	return m_pObj;
}

template<class SharedObjType>
SharedObjType& SharedPtr<SharedObjType>::operator *()
{
	return *m_pObj;
}

template<class SharedObjType>
const SharedObjType& SharedPtr<SharedObjType>::operator *() const
{
	return *m_pObj;
}

template<class SharedObjType>
SharedPtr<SharedObjType>::operator SharedObjType* ()
{
	return m_pObj;
}

template<class SharedObjType>
SharedPtr<SharedObjType>::operator const SharedObjType* () const
{
	return m_pObj;
}

template<class SharedObjType>
SharedPtr<SharedObjType>& SharedPtr<SharedObjType>::operator=( const SharedPtr<SharedObjType> &src )
{
	if( m_pObj == src.m_pObj )
		return *this;

	if( m_pObj ) m_pObj->Release();

	m_pObj = src.m_pObj;
	m_pRefObj = m_pObj;

	if( m_pObj ) m_pObj->AddRef();

	return *this;
}

// Copy operator
template<class SharedObjType>
SharedPtr<SharedObjType>& SharedPtr<SharedObjType>::operator=( const SharedObjType *pSrc ) const
{
	if( m_pObj == pSrc )
		return *this;

	if( m_pObj ) m_pObj->Release();

	m_pObj = pSrc;
	m_pRefObj = m_pObj;

	if( m_pObj ) m_pObj->AddRef();

	return *this;
}

template<class SharedObjType>
SharedPtr<SharedObjType>& SharedPtr<SharedObjType>::operator=( SharedObjType *pSrc )
{
	if( m_pObj == pSrc )
		return *this;

	if( m_pObj ) m_pObj->Release();

	m_pObj = pSrc;
	m_pRefObj = m_pObj;

	if( m_pObj ) m_pObj->AddRef();

	return *this;
}



