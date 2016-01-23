////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : TaeHoon Jung
//
// Description : Linked list
//
////////////////////////////////////////////////////////////////////////////////


friend class Iterator;

private:
	enum NodeLockType
	{
		LockType_Free,
		LockType_Write,
		LockType_Delete,
	};

	class Node;

private:
#pragma pack(push)
#pragma pack(BR_ALIGN)
	struct BR_DECLARE_ALIGN NodeRef
	{
	private:
		NodeRef(const NodeRef& rhs);

	public:
		NodeRef() : m_pNode(nullptr), m_ReadCount(0), m_Lock(LockType_Free)
		{

		}

		NodeRef(Node* pNode) : m_pNode(pNode), m_ReadCount(0), m_Lock(LockType_Free)
		{
			Assert(m_pNode);
		}

		CounterType IncreaseReadCount()
		{
			return m_ReadCount.fetch_add(1, std::memory_order_relaxed) + 1;
		}

		CounterType DecreaseReadCount()
		{
			return m_ReadCount.fetch_sub(1, std::memory_order_relaxed) - 1;
		}	

		Node*					m_pNode;
		SyncCounter				m_ReadCount;
		volatile NodeLockType	m_Lock;
	};
#pragma pack(pop)

private:
	struct Node
	{
		Node()
		{
		}

		Node(const DataType& Value)
		{
			m_Value = Value;
		}

		DataType					m_Value;

		std::atomic<NodeRef>		m_Next;

	};

public:
	class Iterator
	{
		friend class LinkedList;

	private:
		Iterator();

	public:

		Iterator(const Iterator& rhs) : m_pContainer(rhs.m_pContainer), m_pPredNode(rhs.m_pPredNode)
		{
			Assert(m_pContainer != nullptr);
			Assert(m_pPredNode != nullptr);
			Assert(m_pPredNode != &(m_pContainer->m_Tail));

			// Wait insert/delete lock
			while (m_pPredNode->m_Next.load(std::memory_order_relaxed).m_Lock != LockType_Free)
				ThisThread::SleepFor(DurationMS(3));

			m_pContainer->AddItr(this);

			m_pPredNode->m_Next.IncreaseReadCount();
			m_pNode = m_pPredNode->m_Next.m_pNode;

		}

		Iterator(LinkedList *pContainer, Node* pPredNode) : m_pContainer(pContainer), m_pPredNode(pPredNode)
		{
			Assert(m_pContainer != nullptr);
			Assert(m_pPredNode != nullptr);
			Assert(m_pPredNode != &(m_pContainer->m_Tail));

			// Wait insert/delete lock
			while (m_pPredNode->m_Next.m_Lock != LockType_Free)
				ThisThread::SleepFor(DurationMS(3));

			m_pContainer->AddItr(this);

			// Acquires a current node's read lock!
			m_pPredNode->m_Next.IncreaseReadCount();
			m_pNode = m_pPredNode->m_Next.m_pNode;
		}

	private:
		// For internal use ( end() )
		Iterator(LinkedList *pContainer, Node* pPredNode, Node* pCurrNode) : m_pContainer(pContainer), m_pPredNode(pPredNode), m_pNode(pCurrNode)
		{
			Assert(m_pContainer != nullptr);
			Assert(m_pPredNode != nullptr);
			Assert(m_pNode != nullptr);

			m_pContainer->AddItr(this);

			// Wait insert/delete lock
			m_pPredNode->m_Next.IncreaseReadCount();
		}

	public:
		~Iterator()
		{
			m_pPredNode->m_Next.DecreaseReadCount();
			m_pContainer->RemoveItr(this);
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
			Assert(m_pPredNode != nullptr);

			return m_pNode->m_Value;
		}

		const DataType& operator* () const
		{
			Assert(m_pContainer != nullptr);
			Assert(m_pNode	!= nullptr);
			Assert(m_pPredNode != nullptr);

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
			Node* pOldNode = nullptr;

			if (m_pContainer != nullptr && m_pNode != nullptr && m_pPredNode != nullptr)
				pOldNode = m_pPredNode;

			m_pContainer	= rhs.m_pContainer;
			m_pNode			= rhs.m_pNode;
			m_pPredNode		= rhs.m_pPredNode;

			Assert(m_pContainer != nullptr);
			Assert(m_pPredNode != nullptr);
			Assert(m_pPredNode != &(m_pContainer->m_Tail));

			// Wait insert/delete lock
			while (m_pPredNode->m_Next.m_Lock != LockType_Free)
				ThisThread::SleepFor(DurationMS(3));

			m_pPredNode->m_Next.IncreaseReadCount();

			if (pOldNode != nullptr)
				pOldNode->m_Next.DecreaseReadCount();
		}

		Node* GetNode() const
		{
			return m_pNode;
		}

		Node* GetPredNode()
		{
			return m_pPredNode;
		}

		void SetPredNode(Node* pPredNode)
		{
			m_pPredNode = pPredNode;
		}

		void NextIter()
		{
			// Check Data Validity!
			Assert(m_pContainer != nullptr);
			Assert(m_pPredNode != nullptr);
			Assert(m_pNode != nullptr);
			Assert(m_pNode != &m_pContainer->m_Tail);

			Node* pOldTmp = m_pPredNode;

			m_pPredNode = m_pNode;

			// Wait insert/delete lock
			while (m_pPredNode->m_Next.m_Lock != LockType_Free)
				std::this_thread::sleep_for(DurationMS((3));

			// Acquires a current node's read lock!
			m_pPredNode->m_Next.IncreaseReadCount();

			m_pNode = m_pPredNode->m_Next.m_pNode;

			// Release read lock!
			pOldTmp->m_Next.DecreaseReadCount();
		}

		void PrevIter()
		{
		}

	private:
		void Adjust()
		{
			Assert(m_pContainer != nullptr);
			Assert(m_pPredNode != nullptr);
			Assert(m_pPredNode != &(m_pContainer->m_Tail));

			// Wait insert/delete lock
			while (m_pPredNode->m_Next.m_Lock != LockType_Free)
				ThisThread::SleepFor(DurationMS(3));

			// Acquires a current node's read lock!
			m_pPredNode->m_Next.IncreaseReadCount();
			m_pNode = m_pPredNode->m_Next.m_pNode;
		}

	private:
		LinkedList*							m_pContainer;
		Node*								m_pNode;
		Node*								m_pPredNode;
	};


private:
	Node m_Head;
	Node m_Tail;

	SyncCounter m_Size;

	// Node memory pool
	BR::MemoryPool *m_pNodePool;

	// Ticket to the entering update section
	Ticketing	m_UpdateTicket;

	Ticketing	m_ItrListUpdateTicket;

	typedef std::unordered_set<Iterator*>  ItrList;

	ItrList		m_ItrList;

private:
	void AddItr(Iterator* pItr)
	{
		CounterType myTicket = m_ItrListUpdateTicket.AcquireTicket();

		while (myTicket - m_ItrListUpdateTicket.GetWorkingCompleteCount() > 1)
			ThisThread::SleepFor(DurationMS(3));

		m_ItrList.insert(pItr);

		m_ItrListUpdateTicket.ReleaseTicket();
	}

	void RemoveItr(Iterator* pItr)
	{
		CounterType myTicket = m_ItrListUpdateTicket.AcquireTicket();

		while (myTicket - m_ItrListUpdateTicket.GetWorkingCompleteCount() > 1)
			ThisThread::SleepFor(DurationMS(3));

		Assert(m_ItrList.erase(pItr) == 1);

		m_ItrListUpdateTicket.ReleaseTicket();
	}

	void AdjustItrs(Node* pDeleteNode, Node* pNewPredNode)
	{
		CounterType myTicket = m_ItrListUpdateTicket.AcquireTicket();

		while (myTicket - m_ItrListUpdateTicket.GetWorkingCompleteCount() > 1)
			ThisThread::SleepFor(DurationMS(3));

		for (auto itr = m_ItrList.begin(); itr != m_ItrList.end(); ++itr)
		{
			if ((*itr)->m_pPredNode == pDeleteNode) 
			{
				Node* pOldPredNode = nullptr; 

				do 
				{
					pOldPredNode = (*itr)->m_pPredNode;
					_InterlockedCompareExchangePointer((void**)&((*itr)->m_pPredNode), pNewPredNode, pOldPredNode);
				} while ((*itr)->m_pPredNode != pNewPredNode);

				(*itr)->m_pPredNode->m_Next.IncreaseReadCount();
			}
		}

		m_ItrListUpdateTicket.ReleaseTicket();
	}

public:
	LinkedList()
	{
		HRESULT hr = BR::MemoryPoolManager::GetMemoryPool( sizeof(Node), m_pNodePool );
		Assert(SUCCEEDED(hr)&&m_pNodePool);

		// Init list
		m_Head.m_Next.m_pNode =  &m_Tail;
	}

	~LinkedList()
	{
		Clear();

		m_ItrList.clear();
	}

	Iterator Begin() 
	{
		return Iterator(this, &m_Head);
	}

	Iterator End() 
	{
		return Iterator(this, &m_Tail, &m_Tail);
	}

	CounterType GetSize() const
	{
		return m_Size;
	}

	void Clear()
	{
		// 1. Get  a ticket and wait my turn.
		CounterType myTicket = m_UpdateTicket.AcquireTicket();

		while (myTicket - m_UpdateTicket.GetWorkingCompleteCount() > 1)
			ThisThread::SleepFor(DurationMS(3));

		Node* pPredNode = &m_Head;

		pPredNode->m_Next.m_Lock = LockType_Delete;

		while (true)
		{
			while (pPredNode->m_Next.m_ReadCount > 1)
				ThisThread::SleepFor(DurationMS(3));

			Node* pCurNode = pPredNode->m_Next.m_pNode;

			if (pCurNode == &m_Tail)
				break;

			UINT	iTryCount = 0;
			NodeRef oldNodeRef, newNodeRef;

			do 
			{
				//Defence race condition
				if( (iTryCount%4) == 0 )
					ThisThread::SleepFor(DurationMS(0));

				iTryCount++;

				// Old prednode's next
				oldNodeRef = pPredNode->m_Next;

				// New prednode's nexxt
				newNodeRef.m_pNode		= pCurNode->m_Next.m_pNode;
				newNodeRef.m_ReadCount	= 1;	
			}
			while 
#ifdef _WIN64
			while(_InterlockedCompareExchange128( (__int64*)&(pPredNode->m_Next), (__int64)newNodeRef.m_ReadCount, (__int64)newNodeRef.m_pNode, (__int64*)&oldNodeRef) == 0);
#else
			__int64 iOldHdrValue =0;
			iOldHdrValue = *(__int64*)&oldNodeRef;
			while( _InterlockedCompareExchange64( (__int64*)&(pPredNode->m_Next), *(__int64*)&newNodeRef, iOldHdrValue) != iOldHdrValue);
#endif

			pCurNode->~Node();
			m_pNodePool->Free(pCurNode);

			AdjustItrs(pCurNode, pPredNode);

			m_Size.Decrement();
		}

		pPredNode->m_Next.m_Lock = LockType_Free;

		m_UpdateTicket.ReleaseTicket();
	}

	HRESULT Insert(Iterator& Iter, const DataType& Value)
	{
		HRESULT hr = S_SYSTEM_OK;

		// 1. Get  a ticket and wait my turn.
		CounterType myTicket = m_UpdateTicket.AcquireTicket();

		// Invalid Iterator.
		Assert(Iter.GetNode() != nullptr && Iter.GetPredNode() != nullptr);

		if (Iter.GetPredNode() == &m_Tail)
		{
			m_UpdateTicket.ReleaseTicket();
			return hr;
		}

		bool bReleaseReadLock = false;

		while (myTicket - m_UpdateTicket.GetWorkingCompleteCount() > 1)
		{
			ThisThread::SleepFor(DurationMS(3));

			// 1-1. Release a read lock to avoid deadlock 
			if (!bReleaseReadLock && Iter.GetPredNode()->m_Next.m_ReadCount > 1)
			{
				bReleaseReadLock = true;
				Iter.GetPredNode()->m_Next.DecreaseReadCount();
			}
		}

		// 1-2. Recover a read lock
		if (bReleaseReadLock)
			Iter.Adjust();

		// 2. Acquire a write lock
		Iter.GetPredNode()->m_Next.m_Lock = LockType_Write;

		// 3. Wait until a read lock release
		while (Iter.GetPredNode()->m_Next.m_ReadCount > 1)
			ThisThread::SleepFor(DurationMS(3));


		// 4. Add a new node
		void* pPtr = nullptr;

		hr = m_pNodePool->Alloc(pPtr);
		Assert(SUCCEEDED(hr));
		Node* pNewNode = new(pPtr)Node(Value);


		UINT	iTryCount = 0;
		NodeRef oldNodeRef, newNodeRef;

		do 
		{
			//Defence race condition
			if( (iTryCount%4) == 0 )
				ThisThread::SleepFor(DurationMS(0));

			iTryCount++;

			// Old prednode's next
			oldNodeRef = Iter.GetPredNode()->m_Next;

			// New node's next
			pNewNode->m_Next.m_pNode = Iter.GetPredNode()->m_Next.m_pNode;

			// New prednode's next
			newNodeRef.m_pNode		= pNewNode;
			newNodeRef.m_ReadCount	= 1;
		} 
#ifdef _WIN64
		while(_InterlockedCompareExchange128( (__int64*)&(Iter.GetPredNode()->m_Next), (__int64)newNodeRef.m_ReadCount, (__int64)newNodeRef.m_pNode, (__int64*)&oldNodeRef) == 0);
#else
		__int64 iOldHdrValue =0;
		iOldHdrValue = *(__int64*)&oldNodeRef;
		while( _InterlockedCompareExchange64( (__int64*)&(Iter.GetPredNode()->m_Next), *(__int64*)&newNodeRef, iOldHdrValue) != iOldHdrValue);
#endif

		if (hr == S_SYSTEM_OK)
			m_Size.Increment();

		// 5. Release a write lock
		pNewNode->m_Next.m_Lock				= LockType_Free;
		Iter.GetPredNode()->m_Next.m_Lock	= LockType_Free;

		// 6. Release a ticket 
		m_UpdateTicket.ReleaseTicket();

Proc_End:
		return hr;
	}

	HRESULT Erase(Iterator& Iter)
	{
		HRESULT hr = S_SYSTEM_OK;

		// 1. Get  a ticket and wait my turn.
		CounterType myTicket = m_UpdateTicket.AcquireTicket();

		// Invalid Iterator.
		Assert(Iter.GetNode() != nullptr && Iter.GetPredNode() != nullptr);

		if (Iter.GetPredNode() == &m_Tail || Iter.GetPredNode()->m_Next.m_pNode == &m_Tail)
		{
			m_UpdateTicket.ReleaseTicket();
			return hr;
		}

		bool bReleaseReadLock = false;

		while (myTicket - m_UpdateTicket.GetWorkingCompleteCount() > 1)
		{
			ThisThread::SleepFor(DurationMS(3));

			// 1-1. Release a read lock to avoid deadlock 
			if (!bReleaseReadLock && Iter.GetPredNode()->m_Next.m_ReadCount > 1)
			{
				bReleaseReadLock = true;
				Iter.GetPredNode()->m_Next.DecreaseReadCount();
			}
		}

		// 1-2. Recover a read lock
		if (bReleaseReadLock)
			Iter.Adjust();

		// 2. Acquire a write lock
		Iter.GetPredNode()->m_Next.m_Lock = LockType_Delete;

		// 3. Wait until a read lock release
		while (Iter.GetPredNode()->m_Next.m_ReadCount > 1)
			ThisThread::SleepFor(DurationMS(3));

		// 4. Delete a current node


		UINT	iTryCount = 0;
		NodeRef oldNodeRef, newNodeRef;

		do 
		{
			//Defence race condition
			if( (iTryCount%4) == 0 )
				ThisThread::SleepFor(DurationMS(0));

			iTryCount++;

			// Old prednode's next
			oldNodeRef = Iter.GetPredNode()->m_Next;

			// New prednode's nexxt
			newNodeRef.m_pNode		= Iter.GetNode()->m_Next.m_pNode;
			newNodeRef.m_ReadCount	= 1;		
		} 
#ifdef _WIN64
		while(_InterlockedCompareExchange128( (__int64*)&(Iter.GetPredNode()->m_Next), (__int64)newNodeRef.m_ReadCount, (__int64)newNodeRef.m_pNode, (__int64*)&oldNodeRef) == 0);
#else
		__int64 iOldHdrValue =0;
		iOldHdrValue = *(__int64*)&oldNodeRef;
		while( _InterlockedCompareExchange64( (__int64*)&(Iter.GetPredNode()->m_Next), *(__int64*)&newNodeRef, iOldHdrValue) != iOldHdrValue);
#endif

		AdjustItrs(Iter.GetNode(), Iter.GetPredNode());

		// Release memory
		Iter.GetNode()->~Node();
		m_pNodePool->Free(Iter.GetNode());

		if (hr == S_SYSTEM_OK)
			m_Size.Decrement();

		// 5. Release a delete lock
		Iter.GetPredNode()->m_Next.m_Lock = LockType_Free;

		// 6. Release a ticket 
		m_UpdateTicket.ReleaseTicket();

Proc_End:
		return hr;
	}
		