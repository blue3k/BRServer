////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : TaeHoon Jung
//
// Description : Linked list
//
////////////////////////////////////////////////////////////////////////////////


private:
	struct Node
	{
		Node() : m_pNext(nullptr), m_pPrev(nullptr)
		{
		}

		Node(const DataType& Value) : m_pNext(nullptr), m_pPrev(nullptr)
		{
			m_Value = Value;
		}

		DataType					m_Value;
		Node*						m_pNext;
		Node*						m_pPrev;
	};


public:
	// Iterator
	class Iterator
	{
		friend class LinkedList;

	private:
		Iterator();

	public:
		Iterator(const Iterator& rhs) : m_pContainer(rhs.m_pContainer), m_pNode(rhs.m_pNode)
		{
			Assert(m_pContainer != nullptr);
			Assert(m_pNode != nullptr);
		}

		Iterator(LinkedList* pContainer, Node* pNode) : m_pContainer(pContainer), m_pNode(pNode)
		{
			Assert(m_pContainer != nullptr);
			Assert(m_pNode != nullptr);
		}

		~Iterator()
		{
		}

		Iterator& operator++ ()
		{
			NextIter();
			return *this;
		}

		const Iterator& operator++ () const
		{
			NextIter();
			return *this;
		}

		Iterator& operator-- ()
		{
			PrevIter();
			return *this;
		}

		const Iterator& operator-- () const
		{
			PrevIter();
			return *this;
		}

		DataType& operator* ()
		{
			Assert(m_pContainer != nullptr);
			Assert(m_pNode	!= nullptr);

			return m_pNode->m_Value;
		}

		const DataType& operator* () const
		{
			Assert(m_pContainer != nullptr);
			Assert(m_pNode	!= nullptr);

			return m_pNode->m_Value;
		}

		bool operator!= (const Iterator& rhs) const
		{
			return ( (m_pContainer != rhs.m_pContainer) || (m_pNode != rhs.m_pNode) );
		}

		bool operator== (const Iterator& rhs) const
		{
			return ( (m_pContainer == rhs.m_pContainer) && (m_pNode == rhs.m_pNode) );
		}

		Iterator& operator= ( const Iterator& rhs )
		{
			m_pContainer	= rhs.m_pContainer;
			m_pNode			= rhs.m_pNode;

			Assert(m_pContainer != nullptr);
			Assert(m_pNode != nullptr);
		}

		Node* GetNode() const
		{
			return m_pNode;
		}

		void NextIter()
		{
			Assert(m_pContainer != nullptr);
			Assert(m_pNode	!= nullptr);
			Assert(m_pNode != &m_pContainer->m_Tail);

			m_pNode = m_pNode->m_pNext;
		}

		void PrevIter()
		{
			Assert(m_pContainer != nullptr);
			Assert(m_pNode	!= nullptr);
			Assert(m_pNode->m_pPrev != &m_pContainer->m_Head);

			m_pNode = m_pNode->m_pPrev;
		}

	private:
		LinkedList*					m_pContainer;
		Node*						m_pNode;
	};

private:
	Node m_Head;
	Node m_Tail;
	CounterType m_Size;

	// Node memory pool
	BR::MemoryPool *m_pNodePool;

public:
	LinkedList() : m_Size(0)
	{
		HRESULT hr = BR::MemoryPoolManager::GetMemoryPool( sizeof(Node), m_pNodePool );
		Assert(SUCCEEDED(hr)&&m_pNodePool);

		// Init list
		m_Head.m_pNext = &m_Tail;
		m_Tail.m_pPrev = &m_Head;
	}

	~LinkedList()
	{
		Clear();
	}

	Iterator Begin() 
	{
		return Iterator(this, m_Head.m_pNext);
	}

	Iterator End() 
	{
		return Iterator(this, &m_Tail);
	}

	CounterType GetSize() const
	{
		return m_Size;
	}

	void Clear()
	{
		for (Node* pCurNode = m_Head.m_pNext; pCurNode != &m_Tail; pCurNode = pCurNode->m_pNext)
		{
			pCurNode->~Node();
			m_pNodePool->Free(pCurNode);
		}

		// Init list
		m_Head.m_pNext = &m_Tail;
		m_Tail.m_pPrev = &m_Head;

		m_Size = 0;
	}

	HRESULT Insert(Iterator& Iter, const DataType& Value)
	{
		HRESULT hr = S_SYSTEM_OK;

		Assert(Iter.m_pNode != nullptr);

		void* pPtr = nullptr;

		hr = m_pNodePool->Alloc(pPtr);
		Assert(SUCCEEDED(hr));
		Node* pNewNode = new(pPtr)Node(Value);

		// Set link
		pNewNode->m_pNext = Iter.m_pNode;
		pNewNode->m_pPrev = Iter.m_pNode->m_pPrev;

		Iter.m_pNode->m_pPrev->m_pNext	= pNewNode;
		Iter.m_pNode->m_pPrev			= pNewNode;

		if (SUCCEEDED(hr))
			++m_Size;
Proc_End:
		return hr;
	}

	HRESULT Erase(Iterator& Iter)
	{
		HRESULT hr = S_SYSTEM_OK;

		Assert(Iter.m_pNode != nullptr);
		Assert(Iter.m_pNode != &m_Tail);

		Node* pDelNode = Iter.m_pNode;

		// Set link
		pDelNode->m_pPrev->m_pNext = pDelNode->m_pNext;
		pDelNode->m_pNext->m_pPrev = pDelNode->m_pPrev;

		pDelNode->~Node();
		m_pNodePool->Free(pDelNode);

		--m_Size;

Proc_End:
		return hr;
	}