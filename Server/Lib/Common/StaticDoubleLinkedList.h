////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : static hash map
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "SFAssert.h"
#include "Common/MemoryPool.h"
#include "Common/HashTableTrait.h"
#include "Common/ResultCode/BRResultCodeSystem.h"

namespace BR {


	/////////////////////////////////////////////////////////////////////////////////
	//
	//	Thread safe hash map
	//

	class StaticDoubleLinkedList
	{
	public:

		// Single linked list
		struct Node {
			Node*		pPrev;
			Node*		pNext;
			//DataType	Value;
		};

		// iterator class
		class iterator
		{
		private:
			mutable Node *m_pHeader;
			mutable Node *m_pCur;

		public:
			iterator() : m_pCur(nullptr)
			{}
			iterator(const Node* pHeader, const Node* pSrc)
				: m_pHeader(const_cast<Node*>(pHeader))
				, m_pCur(const_cast<Node*>(pSrc))
			{}
			iterator( const iterator& itSrc )
				: m_pHeader(itSrc.m_pHeader)
				, m_pCur(itSrc.m_pCur)
			{}

			iterator& operator++()
			{
				if( m_pCur != nullptr ) m_pCur = m_pCur->pNext;
				return *this;
			}

			const iterator& operator++() const
			{
				if( m_pCur != nullptr ) m_pCur = m_pCur->pNext;
				return *this;
			}

			bool IsValid() const
			{
				return m_pCur != nullptr && m_pCur != m_pHeader;
			}
			
			Node* operator* ()
			{
				AssertRel(m_pCur != nullptr);
				return m_pCur != m_pHeader ? m_pCur : nullptr;
			}

			const Node* operator* () const
			{
				AssertRel(m_pCur != nullptr);
				return m_pCur != m_pHeader ? m_pCur : nullptr;
			}

/*
			DataType* operator-> ()
			{
				AssertRel(m_pCur != nullptr);
				return &m_pCur->Value;
			}

			const DataType* operator-> () const
			{
				AssertRel(m_pCur != nullptr);
				return &m_pCur->Value;
			}
*/
			bool operator !=( const iterator& op ) const
			{
				return m_pCur != op.m_pCur;
			}

			bool operator ==( const iterator& op ) const
			{
				return m_pCur == op.m_pCur;
			}

			iterator& operator = ( const iterator& itSrc )
			{
				m_pCur = itSrc.m_pCur;
				return *this;
			}

			friend class StaticDoubleLinkedList;
		};

	private:
		// Header for node
		Node	m_Header;

		size_t	m_NumItems;

	public:

		// Constructor
		StaticDoubleLinkedList()
			:m_NumItems(0)
		{
			memset( &m_Header, 0, sizeof(m_Header) );
			m_Header.pPrev = &m_Header;
			m_Header.pNext = &m_Header;
		}

		virtual ~StaticDoubleLinkedList()
		{
		}

		Node* PopFront()
		{
			if(m_Header.pNext == &m_Header)
				return nullptr;

			auto pRemoved = m_Header.pNext;
			Remove(m_Header.pNext);

			return pRemoved;
		}

		// insert after specific node, if null for pPrevNode, added at front
		Result Add(Node* pNew)
		{
			if (pNew == nullptr)
				return ResultCode::INVALID_POINTER;

			if (pNew->pNext != nullptr || pNew->pPrev != nullptr)
				return ResultCode::UNEXPECTED;

			pNew->pPrev = &m_Header;
			pNew->pNext = m_Header.pNext;
			m_Header.pNext = pNew;
			Assert(pNew->pNext->pPrev == &m_Header);
			pNew->pNext->pPrev = pNew;

			m_NumItems++;

			return ResultCode::SUCCESS;
		}
		
		// Remove 
		Result Remove( Node* pRemove )
		{
			if (pRemove == nullptr)
				return ResultCode::INVALID_POINTER;

			if (pRemove->pNext == nullptr || pRemove->pPrev == nullptr)
				return ResultCode::UNEXPECTED;

			pRemove->pPrev->pNext = pRemove->pNext;
			pRemove->pNext->pPrev = pRemove->pPrev;
			pRemove->pNext = nullptr;
			pRemove->pPrev = nullptr;

			m_NumItems--;

			return ResultCode::SUCCESS;
		}

		iterator begin()
		{
			return iterator(&m_Header, m_Header.pNext);
		}

		iterator end()
		{
			return iterator();
		}

		size_t size()
		{
			return m_NumItems;
		}
		
		Result erase( const iterator& itCur )
		{
			if( !itCur.IsValid() )
				return ResultCode::FAIL;

			return Remove( itCur.m_pCur );
		}
		
		bool empty()
		{
			return m_Header.pNext == m_Header.pPrev;
		}
		
		void clear()
		{
			memset( &m_Header, 0, sizeof(m_Header) );
			m_Header.pPrev = &m_Header;
			m_Header.pNext = &m_Header;
		}
	};

}; // namespace BR




