////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 MadK
// 
// Author : KyungKun Ko
//
// Description : Shared pointer
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Synchronization.h"
#include "Common/SharedObject.h"

namespace BR
{
	class WeakPointer;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Shared pointer reference class
	//

	class SharedPointer
	{
	public:

	protected:
		mutable SharedObject *m_pObject;

	public:

		SharedPointer()
			:m_pObject(nullptr)
		{
		}

		SharedPointer(const SharedPointer& src)
			:m_pObject(src.m_pObject)
		{
			if (m_pObject != nullptr)
				m_pObject->AddReference();
		}

#ifndef SWIG
		SharedPointer(SharedPointer&& src)
			:m_pObject(src.m_pObject)
		{
			src.m_pObject = nullptr;
		}
#endif
		SharedPointer(SharedObject* pRef)
			:m_pObject(pRef)
		{
			if (m_pObject != nullptr)
			{
				m_pObject->AddReference();
			}
		}

		virtual ~SharedPointer()
		{
			ReleaseReference();
		}

		void ReleaseReference() const
		{
			if (m_pObject == nullptr)
				return;

			//assert(m_pObject->m_ReferenceCount > 0);

			m_pObject->ReleaseReference();

			m_pObject = nullptr;
		}

		explicit operator SharedObject*()
		{
			return m_pObject;
		}
#ifndef SWIG
		explicit operator const SharedObject*() const
		{
			return m_pObject;
		}
#endif
		void operator = (const SharedPointer& src)
		{
			ReleaseReference();

			auto pPtr = src.m_pObject;

			if (pPtr != nullptr)
			{
				Assert(pPtr->GetWeakReferenceCount() > 0 || pPtr->GetReferenceCount() > 0);
				pPtr->AddReference();
			}

			m_pObject = pPtr;
		}

#ifndef SWIG
		void operator = (SharedPointer&& src)
		{
			ReleaseReference();

			m_pObject = src.m_pObject;
			src.m_pObject = nullptr;
		}
#endif

		bool operator == (const SharedPointer& src) const
		{
			return m_pObject == src.m_pObject;
		}

		bool operator != (const SharedPointer& src) const
		{
			return m_pObject != src.m_pObject;
		}

		bool operator == (SharedObject* pRef) const
		{
			return m_pObject == pRef;
		}

		bool operator != (SharedObject* pRef) const
		{
			return m_pObject != pRef;
		}

	protected:

		virtual void SetPointer(SharedObject* pObject)
		{
			ReleaseReference();

			m_pObject = pObject;
		}

		friend class SharedObject;
	};

	template<class ClassType>
	class SharedPointerT : public SharedPointer
	{
	public:
		SharedPointerT()
			:SharedPointer()
		{
		}

		SharedPointerT(const SharedPointerT<ClassType>& src)
			:SharedPointer(src)
		{
		}

#ifndef SWIG
		SharedPointerT(SharedPointerT<ClassType>&& src)
			:SharedPointer(std::forward<SharedPointerT<ClassType>>(src))
		{
		}
#endif

		SharedPointerT(ClassType* pRef)
			:SharedPointer(pRef)
		{
		}

		virtual ~SharedPointerT()
		{
		}

		ClassType* GetObjectPtr()
		{
			return (ClassType*)m_pObject;
		}
#ifndef SWIG
		explicit operator ClassType*()
		{
			return (ClassType*)m_pObject;
		}

		explicit operator const ClassType*() const
		{
			return (ClassType*)m_pObject;
		}

		ClassType* operator ->()
		{
			return (ClassType*)m_pObject;
		}

		ClassType* operator ->() const
		{
			return (ClassType*)m_pObject;
		}
#endif
		bool operator == (const SharedPointer& src) const
		{
			return __super::operator == (src);
		}

		bool operator != (const SharedPointer& src) const
		{
			return __super::operator != (src);
		}

		bool operator == (SharedObject* pRef) const
		{
			return __super::operator == (pRef);
		}

		bool operator != (SharedObject* pRef) const
		{
			return __super::operator != (pRef);
		}

#ifndef SWIG
		SharedPointerT<ClassType>& operator = (const SharedPointer& src)
		{
			auto pObjectSrc = (SharedObject*)(const SharedObject*)src;
			if (pObjectSrc != nullptr)
			{
				if (!TypeCheck(pObjectSrc))
				{
					AssertRel(false);
					return *this;
				}
			}

			__super::operator = (src);

			return *this;
		}

		SharedPointerT<ClassType>& operator = (SharedPointer&& src)
		{
			auto pObjectSrc = (SharedObject*)(const SharedObject*)src;
			if (pObjectSrc != nullptr)
			{
				if (!TypeCheck(pObjectSrc))
				{
					AssertRel(false);
					return *this;
				}
			}

			__super::operator = (std::forward<SharedPointer>(src));

			return *this;
		}

		SharedPointerT<ClassType>& operator = (const SharedPointerT<ClassType>& src)
		{
			__super::operator = (src);
			return *this;
		}

		SharedPointerT<ClassType>& operator = (SharedPointerT<ClassType>&& src)
		{
			__super::operator = (std::forward<SharedPointer>(src));
			return *this;
		}

		SharedPointerT<ClassType>& operator = (void* src)
		{
			assert(src == nullptr);
			__super::operator = (SharedPointer());
			return *this;
		}
#endif

	protected:

		bool TypeCheck(SharedObject* pObject)
		{
#ifdef DEBUG
			if (pObject == nullptr) return true;

			bool sameType = dynamic_cast<ClassType*>(pObject) != nullptr;
			//auto type = typeid(*pObject).name();
			//auto type2 = typeid(ClassType).name();
			//bool sameType = type == type2;
			assert(sameType);
			return sameType;
#else
			// don't check
			return true;
#endif
		}

		virtual void SetPointer(SharedObject* pObject) override
		{
			if (!TypeCheck(pObject))
				return;

			__super::SetPointer(pObject);
		}
	};



	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Weak pointer reference class
	//

	class WeakPointer
	{
	public:

	protected:

		mutable SharedObject *m_pObject;

		WeakPointer(SharedObject* pRef)
			:m_pObject(pRef)
		{
			if (m_pObject != nullptr)
				m_pObject->AddWeakReference();
		}

	public:

		void FromSharedObject(SharedObject *pObject)
		{
			ReleaseReference();

			m_pObject = pObject;

			if (m_pObject != nullptr)
			{
				m_pObject->AddWeakReference();
			}
		}

		WeakPointer()
			:m_pObject(nullptr)
		{
		}

		WeakPointer(const WeakPointer& src)
			:m_pObject(src.m_pObject)
		{
			if (m_pObject != nullptr)
				m_pObject->AddWeakReference();
		}
#ifndef SWIG
		WeakPointer(WeakPointer&& src)
			:m_pObject(src.m_pObject)
		{
			src.m_pObject = nullptr;
		}
#endif
		virtual ~WeakPointer()
		{
			ReleaseReference();
		}

		void ReleaseReference() const
		{
			if (m_pObject == nullptr)
				return;

			m_pObject->ReleaseWeakReference();

			m_pObject = nullptr;
		}

		template<class SharedPointerType>
		void GetSharedPointer(SharedPointerType& pointer) const
		{
			if (m_pObject != nullptr)
				m_pObject->GetSharedPointer(pointer);
			else
				pointer = SharedPointerType();
		}

		explicit operator SharedPointer()
		{
			SharedPointer pointer;
			GetSharedPointer(pointer);
			return pointer;
		}
#ifndef SWIG
		explicit operator const SharedPointer() const
		{
			SharedPointer pointer;
			GetSharedPointer(pointer);
			return pointer;
		}
#endif
		bool operator == (const SharedPointer& src) const
		{
			return m_pObject == (const SharedObject*)src;
		}

		bool operator != (const SharedPointer& src) const
		{
			return m_pObject != (const SharedObject*)src;
		}

		bool operator == (const WeakPointer& src) const
		{
			return m_pObject == src.m_pObject;
		}

		bool operator != (const WeakPointer& src) const
		{
			return m_pObject != src.m_pObject;
		}

		bool operator == (SharedObject* pRef) const
		{
			return m_pObject == pRef;
		}

		bool operator != (SharedObject* pRef) const
		{
			return m_pObject != pRef;
		}

		WeakPointer& operator = (const SharedPointer& src)
		{
			ReleaseReference();

			auto *pObj = const_cast<SharedObject*>((const SharedObject*)src);
			if (pObj == nullptr)
				return *this;

			m_pObject = pObj;

			if (m_pObject != nullptr)
			{
				//Assert(m_pObject->GetWeakReferenceCount() > 0 || m_pObject->GetReferenceCount() > 0);
				m_pObject->AddWeakReference();
			}

			return *this;
		}

		WeakPointer& operator = (const WeakPointer& src)
		{
			ReleaseReference();

			if (src.m_pObject == nullptr || src.m_pObject->GetReferenceCount() == 0)
				return *this;

			auto pPtr = src.m_pObject;

			if (pPtr != nullptr)
			{
				//AssertRel(m_pObject->GetWeakReferenceCount() > 0 || m_pObject->GetReferenceCount() > 0);
				pPtr->AddWeakReference();
			}

			m_pObject = pPtr;

			return *this;
		}
#ifndef SWIG
		WeakPointer& operator = (WeakPointer&& src)
		{
			ReleaseReference();

			m_pObject = src.m_pObject;
			src.m_pObject = nullptr;

			return *this;
		}


		//WeakPointer& operator = (void* src)
		//{
		//	// Assigning non pointer directly isn't thread safe
		//	assert(src != nullptr);
		//	ReleaseReference();

		//	return *this;
		//}
#endif
	};

	template<class ClassType>
	class WeakPointerT : public WeakPointer
	{
	public:
		WeakPointerT()
			:WeakPointer()
		{
		}

		WeakPointerT(const SharedPointerT<typename ClassType>& src)
			:WeakPointer((ClassType*)(const ClassType*)src)
		{
		}

		WeakPointerT(const WeakPointerT<typename ClassType>& src)
			:WeakPointer(src)
		{
		}

		WeakPointerT(WeakPointerT<typename ClassType>&& src)
			:WeakPointer(src)
		{
		}

		~WeakPointerT()
		{
		}

		explicit operator SharedPointerT<ClassType>()
		{
			SharedPointerT<ClassType> pointer;
			GetSharedPointer(pointer);
			return pointer;
		}

		explicit operator const SharedPointerT<ClassType>() const
		{
			SharedPointerT<ClassType> pointer;
			GetSharedPointer(pointer);
			return pointer;
		}

		bool operator == (const SharedPointer& src) const
		{
			return __super::operator == (src);
		}

		bool operator != (const SharedPointer& src) const
		{
			return __super::operator != (src);
		}

		bool operator == (const WeakPointer& src) const
		{
			return __super::operator == (src);
		}

		bool operator != (const WeakPointer& src) const
		{
			return __super::operator != (src);
		}

		bool operator == (SharedObject* pRef) const
		{
			return __super::operator == (pRef);
		}

		bool operator != (SharedObject* pRef) const
		{
			return __super::operator != (pRef);
		}

		WeakPointerT<ClassType>& operator = (const SharedPointerT<ClassType>& src)
		{
			__super::operator = (src);

			return *this;
		}

		WeakPointerT<ClassType>& operator = (const SharedPointer& src)
		{
			__super::operator = (src);

			if (m_pObject != nullptr)
			{
				assert(typeid(m_pObject) == typeid(ClassType));
			}

			return *this;
		}

		WeakPointerT<ClassType>& operator = (const WeakPointer& src)
		{
			__super::operator = (src);

			if (m_pObject != nullptr)
			{
				assert(typeid(m_pObject) == typeid(ClassType));
			}

			return *this;
		}

		WeakPointerT<ClassType>& operator = (WeakPointer&& src)
		{
			__super::operator = (src);

			if (m_pObject != nullptr)
			{
				assert(typeid(m_pObject) == typeid(ClassType));
			}

			return *this;
		}

		WeakPointerT<ClassType>& operator = (const WeakPointerT<ClassType>& src)
		{
			__super::operator = (src);

			return *this;
		}

		WeakPointerT<ClassType>& operator = (WeakPointerT<ClassType>&& src)
		{
			__super::operator = (std::forward<WeakPointerT<ClassType>>(src));

			return *this;
		}

	};

}

