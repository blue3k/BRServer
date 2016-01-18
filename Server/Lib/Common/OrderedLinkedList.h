////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : Single Linked list
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/BrAssert.h"
#include "Common/ResultCode/BRResultCodeSystem.h"


namespace BR {

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Linked list
	//

	template< class KeyType >
	class OrderedLinkedList
	{
	public:

		// Single linked list
		struct Node {
			Node*		pNext;
			// Maximum 64bit Key value
			union {
				KeyType	Key;
				UINT64	Padding;
			};

			Node() {}
		};
		static_assert(sizeof(KeyType) <= sizeof(UINT64), " Maximum 64bit Key value");


		// iterator class
		class iterator
		{
		private:
			Node *m_pCur;

		public:
			iterator() : m_pCur(nullptr)
			{}
			iterator( const Node* pSrc ) : m_pCur(const_cast<Node*>(pSrc))
			{}
			iterator( const iterator& itSrc ) : m_pCur(itSrc.m_pCur)
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
				return m_pCur != nullptr && m_pCur->pNext != nullptr;
			}
			
			Node& operator* ()
			{
				AssertRel(m_pCur != nullptr && m_pCur->pNext != nullptr);
				return *m_pCur->pNext;
			}

			const Node& operator* () const
			{
				AssertRel(m_pCur != nullptr && m_pCur->pNext != nullptr);
				return *m_pCur->pNext;
			}

			Node* operator-> ()
			{
				AssertRel(m_pCur != nullptr && m_pCur->pNext != nullptr);
				return m_pCur->pNext;
			}

			const Node* operator-> () const
			{
				AssertRel(m_pCur != nullptr && m_pCur->pNext != nullptr);
				return m_pCur->pNext;
			}

			bool operator !=( const iterator& op ) const
			{
				// both are end()
				if( !IsValid() && !op.IsValid() )
					return false;

				return ( (m_pCur != op.m_pCur) || ((m_pCur&&op.m_pCur)&&(m_pCur->pNext != op.m_pCur->pNext)) );
			}

			bool operator ==( const iterator& op ) const
			{
				// both are end()
				if( !IsValid() && !op.IsValid() )
					return true;

				return m_pCur == op.m_pCur;
			}
				
			iterator& operator = ( const iterator& itSrc )
			{
				m_pCur = itSrc.m_pCur;
				return *this;
			}

			friend class OrderedLinkedList<KeyType>;
		};

	private:
		// Header for node
		Node	m_Header;

		size_t	m_NumItems;

	public:

		// Constructor
		OrderedLinkedList()
			:m_NumItems(0)
		{
			memset( &m_Header, 0, sizeof(m_Header) );
		}

		virtual ~OrderedLinkedList()
		{
		}

		// Find Previous Node
		HRESULT FindPrevNode( KeyType keyValue, Node* &pPrevNode )
		{
			pPrevNode = &m_Header;
			for( ; pPrevNode->pNext != nullptr; pPrevNode = pPrevNode->pNext )
			{
				Node *pNextNode = pPrevNode->pNext;
				if( pNextNode->Key <= keyValue )
					break;
			}
			return S_OK;
		}

		HRESULT Insert( iterator itNode, KeyType keyValue, Node* pNew )
		{
			if( itNode.m_pCur == nullptr ) return E_SYSTEM_POINTER;

			return Insert( itNode.m_pCur, keyValue, pNew );
		}

		// insert after specific node, if null for pPrevNode, added at front
		HRESULT Insert( Node* pPrevNode, KeyType keyValue, Node* pNew )
		{
			if( pPrevNode == nullptr )
				return E_SYSTEM_POINTER;

			if( pNew->pNext != nullptr )
				return E_SYSTEM_UNEXPECTED;

			// This node should be in the list already
			if( pPrevNode == pNew || pPrevNode->pNext == pNew )
				return E_SYSTEM_UNEXPECTED;

			if( pPrevNode != &m_Header )
			{
				AssertRel(pPrevNode->Key > keyValue);
			}

			pNew->Key = keyValue;
			pNew->pNext = pPrevNode->pNext;

			// This will make this operation as a thread safe operation for MT-read, single write
			//_WriteBarrier();
			std::atomic_thread_fence(std::memory_order_seq_cst);
			pPrevNode->pNext = pNew;

			m_NumItems++;

			return S_OK;
		}

		// Remove 
		
		HRESULT Remove( iterator &itNode )
		{
			if( !itNode.IsValid() ) return E_SYSTEM_POINTER;

			return Remove( itNode.m_pCur, itNode.m_pCur->pNext );
		}
		
		HRESULT FindAndRemove( Node* pSearchPrevNode, Node* pRemove )
		{
			if( pSearchPrevNode == nullptr )
				pSearchPrevNode = &m_Header;

			while( pSearchPrevNode != nullptr && pSearchPrevNode->pNext != pRemove )
			{
				pSearchPrevNode = pSearchPrevNode->pNext;
			}

			if( pSearchPrevNode == nullptr )
				return E_SYSTEM_UNEXPECTED;

			pSearchPrevNode->pNext = pRemove->pNext;
			pRemove->pNext = nullptr;

			m_NumItems--;

			return S_OK;
		}

		HRESULT Remove( Node* pPrevNode, Node* pRemove )
		{
			if( pPrevNode == nullptr )
				return E_SYSTEM_POINTER;

			if( pPrevNode->pNext != pRemove )
				return E_SYSTEM_UNEXPECTED;

			pPrevNode->pNext = pRemove->pNext;
			pRemove->pNext = nullptr;

			m_NumItems--;

			return S_OK;
		}

		iterator begin()
		{
			return iterator(&m_Header);
		}

		iterator end()
		{
			return iterator();
		}

		size_t size()
		{
			return m_NumItems;
		}
		
		HRESULT erase( const iterator& itCur )
		{
			if( !itCur.IsValid() )
				return E_SYSTEM_FAIL;

			return Remove( itCur.m_pCur, itCur.m_pCur->pNext );
		}
		
		bool empty()
		{
			return m_Header.pNext == nullptr;
		}
		
		void clear()
		{
			memset( &m_Header, 0, sizeof(m_Header) );
		}
	};

}; // namespace BR

