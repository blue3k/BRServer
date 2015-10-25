////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 MadK
// 
// Author : KyungKun Ko
//
// Description : Shared pointer
//
////////////////////////////////////////////////////////////////////////////////



	///////////////////////////////////////////////////////////////////////////
	//
	//	DualSortedMap<KeyType,ValueType>::MapNode
	//

	template<class KeyType, class ValueType>
	int DualSortedMap<KeyType,ValueType>::MapNode::UpdateBalanceFactor()
	{
		auto left = Left.load();
		auto right = Right.load();
		int iLeftDepth = 0, iLeftChildren = 0;
		int iRightDepth = 0, iRightChildren = 0;
		if (left != nullptr)
		{
			iLeftDepth = left->DepthOfChildren + 1;
			iLeftChildren = left->NumberOfChildren + 1;
		}
		if (right != nullptr)
		{
			iRightDepth = right->DepthOfChildren + 1;
			iRightChildren = right->NumberOfChildren + 1;
		}

		// Original AVL just uses -BalanceTolerance ~ BalanceTolerance expression
		Balance = iRightDepth - iLeftDepth;
		DepthOfChildren = std::max(iRightDepth, iLeftDepth);
		NumberOfChildren = iLeftChildren + iRightChildren;

		return Balance;
	}

	///////////////////////////////////////////////////////////////////////////
	//
	//	DualSortedMap<KeyType,ValueType>::MapNode
	//

	template<class KeyType, class ValueType>
	void DualSortedMap<KeyType,ValueType>::OperationTraversalHistory::Replace(UINT updateSerial, int nodeIndex, MapNode* pNode, MapNode* pNewNode)
	{
		Assert(GetHistory(nodeIndex) == pNode);
		if (GetHistorySize() <= 1 || nodeIndex < 1) // only the found node is in there
		{
			m_Root = pNewNode;
		}
		else
		{
			auto parentNode = GetHistory(nodeIndex - 1);
			Assert(parentNode->UpdateSerial == updateSerial);
			if (parentNode->Left == pNode)
			{
				parentNode->Left = pNewNode;
			}
			else
			{
				Assert(parentNode->Right == pNode);
				parentNode->Right = pNewNode;
			}
		}
	}


	///////////////////////////////////////////////////////////////////////////
	//
	//
	//

	template<class KeyType, class ValueType>
	DualSortedMap<KeyType, ValueType>::DualSortedMap(ObjectPoolT<MapNode> *pNodePool)
		: m_ItemCount(0)
		, m_ReadItemCount(0)
		, m_ReadRoot(nullptr)
		, m_WriteRoot(nullptr)
		, m_UpdateSerial(1)
		, m_IsModified(false)
		, m_DeleteNodePool(false)
		, m_pNodePool(pNodePool)
		, m_PendingFreeCount(0)
		, m_PendingFreeList(nullptr)
		, m_ReadIndex(0)
		//, m_PendingFreeListTail(nullptr)
	{
		for (int iVal = 0; iVal < _countof(m_ReadCount); iVal++)
		{
			m_ReadCount[iVal] = 0;
		}

		if (m_pNodePool == nullptr)
		{
			m_DeleteNodePool = true;
			m_pNodePool = new ObjectPoolT<MapNode>();
		}
	}

	template<class KeyType, class ValueType>
	DualSortedMap<KeyType,ValueType>::~DualSortedMap()
	{
		ClearMap();

		if (m_pNodePool != nullptr && m_DeleteNodePool)
		{
			delete m_pNodePool;
		}
	}

	template<class KeyType, class ValueType>
	void DualSortedMap<KeyType,ValueType>::ClearMap()
	{
		std::queue<MapNode*> mapNodes;

		CommitChanges();

		MapNode* pCurNode = m_ReadRoot.load();
		if (pCurNode != nullptr) mapNodes.push(pCurNode);

		m_ReadRoot = nullptr;
		while (mapNodes.size() > 0)
		{
			pCurNode = mapNodes.front();
			mapNodes.pop();

			if (pCurNode->Left != nullptr)	mapNodes.push(pCurNode->Left.load());
			if (pCurNode->Right != nullptr)	mapNodes.push(pCurNode->Right.load());

			FreeNode(pCurNode);
		}
		Assert(m_WriteRoot == nullptr);

		m_ItemCount = 0;

		CommitPendingFree();
	}

	// Insert a key
	template<class KeyType, class ValueType>
	HRESULT DualSortedMap<KeyType, ValueType>::Insert(KeyType key, const ValueType& value, INT64 *insertedOrder)
	{
		OperationTraversalHistory travelHistory(m_WriteRoot, m_ItemCount.load(std::memory_order_relaxed));

		MapNode* pFound = nullptr;
		if (FAILED(FindNode(travelHistory, key, pFound)))
		{
			if (m_WriteRoot != nullptr)
				return E_FAIL;
			else
			{
				auto newNode = AllocateNode(key, value);
				m_WriteRoot = newNode;

				if (newNode == nullptr)
					return E_OUTOFMEMORY;

				m_IsModified = true;
				m_ItemCount.fetch_add(1, std::memory_order_relaxed);

				if (insertedOrder != nullptr)
				{
					*insertedOrder = 0;
				}

				return S_OK;
			}
		}

		MapNode* pInserted = nullptr;
		if (key > pFound->Key) // duplicate key will be put at the left most side
		{
			auto right = pFound->Right;
			if (right != nullptr)
				return E_FAIL;

			pFound->Right = pInserted = AllocateNode(key, value);
		}
		else if (key == pFound->Key)
		{
			auto left = pFound->Left = CloneNode(pFound->Left.load());
			if (left != nullptr)
			{
				auto biggestNode = FindBiggestNode(travelHistory, left);
				auto right = biggestNode->Right;
				if (right != nullptr)
					return E_FAIL;

				biggestNode->Right = pInserted = AllocateNode(key, value);
			}
			else
			{
				pFound->Left = pInserted = AllocateNode(key, value);
			}
		}
		else // if (key < pCurNode->Key) 
		{
			auto left = pFound->Left;
			if (left != nullptr)
				return E_FAIL;

			pFound->Left = pInserted = AllocateNode(key, value);
		}


		if (insertedOrder != nullptr)
		{
			*insertedOrder = CalculateOrder(travelHistory, pInserted);
		}

		//__if_exists(ValueType::m_pObject)
		//{
		//	AssertRel(pInserted->Value != ValueType(0));
		//}

		FixupBalance(travelHistory);

		m_IsModified = true;
		m_ItemCount.fetch_add(1, std::memory_order_relaxed);

		return S_OK;
	}


	// Remove an item and return the removed value
	template<class KeyType, class ValueType>
	HRESULT DualSortedMap<KeyType,ValueType>::Remove(KeyType key, ValueType& value)
	{
		OperationTraversalHistory travelHistory(m_WriteRoot, m_ItemCount.load(std::memory_order_relaxed));

		MapNode* pRemoved = nullptr;
		MapNode* pFound = nullptr;
		if (FAILED(FindNode(travelHistory, key, pFound)))
			return E_INVALIDARG;

		// unique key
		if (pFound->Key != key)
			return E_FAIL;

		value = std::forward<ValueType>(pFound->Value);


		ReferenceAccessPoint *pParentPointer = nullptr;
		MapNode* pParentOfReplaced = nullptr;
		MapNode* child = nullptr;

		auto left = pFound->Left.load();
		auto right = pFound->Right.load();
		// If it's not a leap node, find a replacement
		if (left != nullptr || right != nullptr)
		{
			// select a child to replace
			bool rightHeavy = false;
			int leftDepth = left == nullptr ? -1 : left->DepthOfChildren;
			int rightDepth = right == nullptr ? -1 : right->DepthOfChildren;
			if (leftDepth > rightDepth)
			{
			}
			else if (leftDepth < rightDepth)
			{
				rightHeavy = true;
			}
			else
			{
				int leftChildren = left == nullptr ? -1 : left->NumberOfChildren;
				int rightChildren = right == nullptr ? -1 : right->NumberOfChildren;
				if (leftChildren > rightChildren)
				{
				}
				else
				{
					rightHeavy = true;
				}
			}

			if (rightHeavy) // select heavy tree child
			{
				right = pFound->Right = CloneNode(right);
				pRemoved = FindSmallestNode(travelHistory, right);
				Assert(pRemoved->Left == nullptr);
				child = pRemoved->Right.load();
			}
			else
			{
				left = pFound->Left = CloneNode(left);
				pRemoved = FindBiggestNode(travelHistory, left);
				Assert(pRemoved->Right == nullptr);
				child = pRemoved->Left.load();
			}

			// swap value with replacement node
			pFound->Key = pRemoved->Key;
			pFound->Value = std::forward<ValueType>(pRemoved->Value);
		}
		else
		{
			// if it doesn't have any child
			pRemoved = pFound;
		}


		// remove replacement from the tree
		travelHistory.Replace(m_UpdateSerial, (int)travelHistory.GetHistorySize() - 1, pRemoved, child);

		// remove from the traversal history, replacement node will not be need to be took care
		travelHistory.RemoveLastHistory();


		FixupBalance(travelHistory);

		if (pRemoved) FreeNode(pRemoved);

		m_IsModified = true;
		m_ItemCount.fetch_sub(1, std::memory_order_release);

		return S_OK;
	}



	// Find a key value
	template<class KeyType, class ValueType>
	HRESULT DualSortedMap<KeyType, ValueType>::FindInWriteTree(KeyType key, ValueType& value)
	{
		auto pReadRoot = m_WriteRoot;
		if (pReadRoot == nullptr)
		{
			// try in read tree again
			return Find(key, value);
		}

		OperationTraversalHistory travelHistory(pReadRoot, m_ItemCount.load(std::memory_order_relaxed));

		MapNode* pFound = nullptr;
		if (FAILED(FindNode(travelHistory, key, pFound)))
		{
			return E_FAIL;
		}

		// unique key
		if (pFound->Key != key)
		{
			return E_FAIL;
		}

		value = pFound->Value;

		return S_OK;
	}

	// commit changes
	template<class KeyType, class ValueType>
	HRESULT DualSortedMap<KeyType,ValueType>::CommitChanges()
	{
		// Nothing to commit
		if (!m_IsModified)
		{
			return S_OK;
		}

#ifdef DEBUG
		auto oldReadCount0 = m_ReadCount[0].load(std::memory_order_relaxed);
		auto oldReadCount1 = m_ReadCount[1].load(std::memory_order_relaxed);
		auto oldReadCount2 = m_ReadCount[2].load(std::memory_order_relaxed);
#endif

		// flip root
		auto oldRoot = m_PrevReadRoot = m_ReadRoot.load(std::memory_order_relaxed);
		Assert(m_WriteRoot == nullptr || m_WriteRoot != oldRoot);
		if (m_WriteRoot != nullptr)
		{
			Assert(!m_WriteRoot->IsCloned);
		}

		if (oldRoot != nullptr)
		{
			AssertRel(oldRoot->UpdateSerial != m_UpdateSerial);
			AssertRel(oldRoot->UpdateSerial == (m_UpdateSerial-1));
		}

		m_UpdateSerial++; // Some read assert on debug build require memory fence

		auto oldWrite = m_WriteRoot;
		m_WriteRoot = nullptr;

		std::atomic_thread_fence(std::memory_order_seq_cst);

		m_ReadRoot = oldWrite;

		std::atomic_thread_fence(std::memory_order_seq_cst);

		// after flip the root we need to wait all previous working indexies
		auto oldReadIndex = m_ReadIndex.fetch_add(1, std::memory_order_release) % _countof(m_ReadCount);
		while (m_ReadCount[oldReadIndex].load(std::memory_order_relaxed) > 0)
		{
			Sleep(0);
		}

		//Clean up cloned sources
//#ifdef DEBUG
//		ForeachOrder(m_ReadRoot.load(), 0, (UINT)m_ItemCount.load(std::memory_order_relaxed), [&](MapNode* pNode)-> bool
//		{
//			ForeachPendingFree([&pNode](ObjectPoolObject *pPoolObject)
//			{
//				AssertRel(pPoolObject != (ObjectPoolObject*)pNode);
//			});
//			return true;
//		});
//#endif

		CommitPendingFree();


		m_IsModified = false;

		m_ReadItemCount = m_ItemCount.load(std::memory_order_relaxed);

		return S_OK;
	}

	// Find a key value
	template<class KeyType, class ValueType>
	HRESULT DualSortedMap<KeyType, ValueType>::Find(KeyType key, ValueType& value, INT64 *pOrder)
	{
		auto readIdx = m_ReadIndex.load(std::memory_order_relaxed) % _countof(m_ReadCount);
		ScopeCounter localCounter(m_ReadCount[readIdx]);

		m_CurReadRoot = m_ReadRoot.load(std::memory_order_acquire);
		auto pReadRoot = (MapNode*)m_CurReadRoot;
		if (pReadRoot == nullptr)
		{
			return E_FAIL;
		}

		Assert(m_UpdateSerial != pReadRoot->UpdateSerial);
		Assert(m_WriteRoot != pReadRoot);

		OperationTraversalHistory travelHistory(pReadRoot, m_ReadItemCount);

		MapNode* pFound = nullptr;
		
		if (FAILED(FindNodeRead(travelHistory, pReadRoot, key, pFound)))
		{
			return E_FAIL;
		}

		// unique key
		if (pFound->Key != key)
		{
			return E_FAIL;
		}

		//__if_exists(ValueType::m_pObject)
		//{
		//	if (pFound->Value == ValueType(0))
		//	{
		//		//printf("History %d, key %d, found 0x%p\n", (UINT)travelHistory.GetHistorySize(), (UINT)key, pFound);
		//		//for (int history = 0; history < travelHistory.GetHistorySize(); history++)
		//		//{
		//		//	printf("	0x%p => \n", travelHistory.GetHistory(history));
		//		//}

		//		AssertRel(0);
		//	}
		//}

		value = pFound->Value;

		if (pOrder != nullptr)
		{
			*pOrder = CalculateOrder(travelHistory, pFound);
		}

		//__if_exists(ValueType::m_pObject)
		//{
		//	if (pFound->Value == ValueType(0))
		//	{
		//		//printf("History %d, key %d, found 0x%p\n", (UINT)travelHistory.GetHistorySize(), (UINT)key, pFound);
		//		//for (int history = 0; history < travelHistory.GetHistorySize(); history++)
		//		//{
		//		//	printf("	0x%p => \n", travelHistory.GetHistory(history));
		//		//}

		//		AssertRel(0);
		//	}
		//	//AssertRel(pFound->Value != ValueType(0));
		//	AssertRel(value != ValueType(0));
		//	//AssertRel(value->Value == key);
		//}

		return S_OK;
	}

	// enumerate the values
	template<class KeyType, class ValueType>
	HRESULT DualSortedMap<KeyType, ValueType>::ForeachOrder(INT startOrderIndex, UINT count, const std::function<bool(const KeyType&, const ValueType&)>& functor)
	{
		auto readIdx = m_ReadIndex.load(std::memory_order_relaxed) % _countof(m_ReadCount);
		ScopeCounter localCounter(m_ReadCount[readIdx]);

		m_CurReadRoot = m_ReadRoot.load(std::memory_order_acquire);
		auto readRoot = (MapNode*)m_CurReadRoot;
		if (readRoot == nullptr)
			return E_FAIL;

		return ForeachOrder(readRoot, startOrderIndex, count, functor);
	}


	template<class KeyType, class ValueType>
	HRESULT DualSortedMap<KeyType, ValueType>::ForeachReverseOrder(INT startOrderIndex, UINT count, const std::function<bool(const KeyType&, const ValueType&)>& functor)
	{
		auto readIdx = m_ReadIndex.load(std::memory_order_relaxed) % _countof(m_ReadCount);
		ScopeCounter localCounter(m_ReadCount[readIdx]);

		m_CurReadRoot = m_ReadRoot.load(std::memory_order_acquire);
		auto readRoot = (MapNode*)m_CurReadRoot;
		if (readRoot == nullptr)
			return E_FAIL;

		return ForeachReverseOrder(readRoot, startOrderIndex, count, functor);
	}


	// enumerate the values
	template<class KeyType, class ValueType>
	HRESULT DualSortedMap<KeyType, ValueType>::ForeachOrderWrite(INT startOrderIndex, UINT count, const std::function<bool(const KeyType&, const ValueType&)>& functor)
	{
		if (!m_IsModified)
		{
			return ForeachOrder(startOrderIndex, count, functor);
		}

		return ForeachOrder(m_WriteRoot, startOrderIndex, count, functor);
	}

	// find parent node or candidate
	template<class KeyType, class ValueType>
	HRESULT DualSortedMap<KeyType,ValueType>::FindNode(OperationTraversalHistory &travelHistory, KeyType key, MapNode* &pNode)
	{
		MapNode* pCurNode = m_WriteRoot;
		if (pCurNode == nullptr)
		{
			if (!m_IsModified)
			{
				auto readRoot = m_ReadRoot.load(std::memory_order_relaxed);
				if (readRoot == nullptr)
					return E_FAIL;
				Assert(m_UpdateSerial != readRoot->UpdateSerial);
				pCurNode = m_WriteRoot = CloneNode(readRoot);
				Assert(m_WriteRoot != readRoot);
				m_IsModified = true;
			}
			else
			{
				return E_FAIL;
			}
		}
		Assert(m_WriteRoot != m_ReadRoot.load());

		travelHistory.Clear();
		travelHistory.SetConserveDataOnResize(true);

		do
		{
			Assert(pCurNode->UpdateSerial == m_UpdateSerial);
			Assert(!pCurNode->IsCloned);
			travelHistory.AddHistory(pCurNode);

			// multiple key
			if (pCurNode->Key == key)
			{
				pNode = pCurNode;
				return S_OK;
			}

			if (key > pCurNode->Key)
			{
				auto right = pCurNode->Right.load();
				if (right == nullptr)
				{
					pNode = pCurNode;
					return S_OK;
				}
				else
				{
					right = pCurNode->Right = CloneNode(right);
					pCurNode = right;
				}
			}
			else // if (key <= pCurNode->Key)
			{
				auto left = pCurNode->Left.load();
				left = pCurNode->Left = CloneNode(left);

				if (left == nullptr)
				{
					pNode = pCurNode;
					return S_OK;
				}
				else 
				{
					// Handle multiple key
					// choose left most one
					if (pCurNode->Key == key)
					{
						if (left->Key != key)
						{
							pNode = FindBiggestNode(travelHistory, left);
							return S_OK;
						}
					}
					pCurNode = left;
				}
			}

		} while (pCurNode != nullptr);

		travelHistory.SetConserveDataOnResize(false);

		return S_OK;
	}

	template<class KeyType, class ValueType>
	typename DualSortedMap<KeyType,ValueType>::MapNode* DualSortedMap<KeyType,ValueType>::FindSmallestNode(OperationTraversalHistory &travelHistory, MapNode* pRootNode)
	{
		if (pRootNode == nullptr) return nullptr;

		auto pCurNode = pRootNode;
		while (pCurNode != nullptr)
		{
			Assert(pCurNode->UpdateSerial == m_UpdateSerial);
			travelHistory.AddHistory(pCurNode);
			auto left = pCurNode->Left = CloneNode(pCurNode->Left.load());
			if (left == nullptr)
				return pCurNode;

			pCurNode = left;
		}

		return pCurNode;
	}

	template<class KeyType, class ValueType>
	typename DualSortedMap<KeyType,ValueType>::MapNode* DualSortedMap<KeyType,ValueType>::FindBiggestNode(OperationTraversalHistory &travelHistory, MapNode* pRootNode)
	{
		if (pRootNode == nullptr) return nullptr;

		Assert(pRootNode->UpdateSerial == m_UpdateSerial);

		auto pCurNode = pRootNode;
		while (pCurNode != nullptr)
		{
			Assert(pCurNode->UpdateSerial == m_UpdateSerial);
			travelHistory.AddHistory(pCurNode);
			auto right = pCurNode->Right = CloneNode(pCurNode->Right.load());
			if (right == nullptr)
				return pCurNode;

			pCurNode = right;
		}

		return pCurNode;
	}

	template<class KeyType, class ValueType>
	HRESULT DualSortedMap<KeyType, ValueType>::FindNodeRead(OperationTraversalHistory &travelHistory, MapNode* pRootNode, KeyType key, MapNode* &pNode)
	{
		MapNode* pCurNode = pRootNode;
		if (pCurNode == nullptr)
			return E_FAIL;

		do
		{
			travelHistory.AddHistory(pCurNode);

			// multiple key
			if (pCurNode->Key == key)
			{
				pNode = pCurNode;
				return S_OK;
			}

			if (key > pCurNode->Key)
			{
				auto right = pCurNode->Right.load();
				if (right == nullptr)
				{
					pNode = pCurNode;
					return S_OK;
				}
				else
				{
					pCurNode = right;
				}
			}
			else // if (key <= pCurNode->Key)
			{
				auto left = pCurNode->Left.load();
				if (left == nullptr)
				{
					pNode = pCurNode;
					return S_OK;
				}
				else
				{
					// Handl multiple key
					// choose left most one
					if (pCurNode->Key == key)
					{
						if (left->Key != key)
						{
							pNode = FindBiggestNodeRead(travelHistory, left);
							return S_OK;
						}
					}
					pCurNode = left;
				}
			}

		} while (pCurNode != nullptr);

		return S_OK;
	}

	template<class KeyType, class ValueType>
	typename DualSortedMap<KeyType, ValueType>::MapNode* DualSortedMap<KeyType, ValueType>::FindSmallestNodeRead(OperationTraversalHistory &travelHistory, MapNode* pRootNode)
	{
		if (pRootNode == nullptr) return nullptr;

		auto pCurNode = pRootNode;
		while (pCurNode != nullptr)
		{
			travelHistory.AddHistory(pCurNode);

			auto left = pCurNode->Left.load();
			if (left == nullptr)
				return pCurNode;

			pCurNode = left;
		}

		return pCurNode;
	}

	template<class KeyType, class ValueType>
	typename DualSortedMap<KeyType, ValueType>::MapNode* DualSortedMap<KeyType, ValueType>::FindBiggestNodeRead(OperationTraversalHistory &travelHistory, MapNode* pRootNode)
	{
		if (pRootNode == nullptr) return nullptr;

		auto pCurNode = pRootNode;
		while (pCurNode != nullptr)
		{
			travelHistory.AddHistory(pCurNode);

			auto right = pCurNode->Right.load();
			if (right == nullptr)
				return pCurNode;

			pCurNode = right;
		}

		return pCurNode;
	}

	template<class KeyType, class ValueType>
	HRESULT DualSortedMap<KeyType, ValueType>::ForeachOrder(MapNode* pRootNode, INT startOrderIndex, UINT count, const std::function<bool(const KeyType&, const ValueType&)>& functor)
	{
		if (pRootNode == nullptr)
			return S_OK;

		OperationTraversalHistory travelHistory(pRootNode, m_ReadItemCount);

		MapNode* pCurNode = pRootNode;
		if (pCurNode == nullptr)
		{
			return S_OK;
		}

		travelHistory.Clear();
		travelHistory.SetConserveDataOnResize(true);

		// find start point
		do
		{
			travelHistory.AddHistory(pCurNode);

			auto right = pCurNode->Right.load();
			auto rightNumChildren = right != nullptr ? right->NumberOfChildren : -1;
			if (rightNumChildren >= startOrderIndex)
			{
				pCurNode = right;
			}
			else
			{
				if (rightNumChildren >= 0)
				{
					startOrderIndex -= rightNumChildren + 1;
				}
				if (startOrderIndex == 0) // current node is the start point
					break;

				startOrderIndex--;
				auto left = pCurNode->Left.load();
				pCurNode = left;
			}

		} while (pCurNode != nullptr);

		if (pCurNode == nullptr)
		{
			return S_OK;
		}


		// interate
		do
		{
			if (!functor(pCurNode->Key, pCurNode->Value))
				return S_OK;

			count--;
			if (count == 0)
				break;

			auto left = pCurNode->Left.load();
			if (left != nullptr)
			{
				pCurNode = FindBiggestNodeRead(travelHistory, left);
			}
			else // this is a leap node pop up
			{
				travelHistory.RemoveLastHistory();
				MapNode* parent = nullptr;
				do
				{
					parent = travelHistory.GetLastHistory();
					if (parent == nullptr)
					{
						// nothing to process
						pCurNode = nullptr;
						break;
					}

					if (pCurNode == parent->Left.load())
					{
						travelHistory.RemoveLastHistory();
					}
					else
					{
						pCurNode = parent;
						break;
					}

					pCurNode = parent;

				} while (parent != nullptr);
			}

		} while (pCurNode != nullptr);


		return S_OK;
	}


	template<class KeyType, class ValueType>
	HRESULT DualSortedMap<KeyType, ValueType>::ForeachReverseOrder(MapNode* pRootNode, INT startOrderIndex, UINT count, const std::function<bool(const KeyType&, const ValueType&)>& functor)
	{
		if (pRootNode == nullptr)
			return S_OK;

		OperationTraversalHistory travelHistory(pRootNode, m_ReadItemCount);

		MapNode* pCurNode = pRootNode;
		if (pCurNode == nullptr)
		{
			return S_OK;
		}

		travelHistory.Clear();
		travelHistory.SetConserveDataOnResize(true);

		// find start point
		do
		{
			travelHistory.AddHistory(pCurNode);

			auto left = pCurNode->Left.load();
			auto leftNumChildren = left != nullptr ? left->NumberOfChildren : -1;
			if (leftNumChildren >= startOrderIndex)
			{
				pCurNode = left;
			}
			else
			{
				if (leftNumChildren >= 0)
				{
					startOrderIndex -= leftNumChildren + 1;
				}
				if (startOrderIndex == 0) // current node is the start point
					break;

				startOrderIndex--;
				auto right = pCurNode->Right.load();
				pCurNode = right;
			}

		} while (pCurNode != nullptr);

		if (pCurNode == nullptr)
		{
			return S_OK;
		}


		// interate
		do
		{
			if (!functor(pCurNode->Key, pCurNode->Value))
				break;

			count--;
			if (count == 0)
				break;

			auto right = pCurNode->Right.load();
			if (right != nullptr)
			{
				pCurNode = FindSmallestNodeRead(travelHistory, right);
			}
			else // this is a leap node pop up
			{
				travelHistory.RemoveLastHistory();
				MapNode* parent = nullptr;
				do
				{
					parent = travelHistory.GetLastHistory();
					if (parent == nullptr)
					{
						// nothing to process
						pCurNode = nullptr;
						break;
					}

					if (pCurNode == parent->Right.load())
					{
						travelHistory.RemoveLastHistory();
					}
					else
					{
						pCurNode = parent;
						break;
					}

					pCurNode = parent;

				} while (parent != nullptr);
			}

		} while (pCurNode != nullptr);


		return S_OK;
	}

	template<class KeyType, class ValueType>
	void DualSortedMap<KeyType,ValueType>::FixupBalance(OperationTraversalHistory &travelHistory)
	{
		int iRebalancing = 0;
		for (int iHistory = (int)travelHistory.GetHistorySize() - 1; iHistory >= 0; iHistory--)
		{
			auto pCurNode = travelHistory.GetHistory(iHistory);
			auto balance = pCurNode->UpdateBalanceFactor();

			if (std::abs(balance) < BalanceTolerance)
				continue;

			iRebalancing++;
			if (iRebalancing > MaxRebalancingTry)
				continue;

			// Truncate history, it's need to be update
			//auto parentAccessPoint = travelHistory.GetParentAccessPoint(m_UpdateSerial, iHistory, pCurNode);

			travelHistory.TruncateHistoryFrom(iHistory+1);

			Assert(pCurNode->UpdateSerial == m_UpdateSerial);

			if (balance <= -BalanceTolerance)
			{
				auto left = pCurNode->Left = CloneNode(pCurNode->Left.load());
				// left-right case
				if (left->Balance >= 0)
				{
					auto leftRight = left->Right = CloneNode(left->Right.load());
					Assert(leftRight->UpdateSerial == m_UpdateSerial);
					Assert(left->UpdateSerial == m_UpdateSerial);
					left->Right = leftRight->Left;
					pCurNode->Left = leftRight->Right;

					leftRight->Left = left;
					leftRight->Right = pCurNode;

					//*parentAccessPoint = leftRight;
					travelHistory.Replace(m_UpdateSerial, iHistory, pCurNode, leftRight);
					travelHistory.RemoveLastHistory(); // remove pcurnode before add other
					travelHistory.AddHistory(leftRight);
				}
				else // left-left case
				{
					Assert(left->UpdateSerial == m_UpdateSerial);
					pCurNode->Left = left->Right;
					left->Right = pCurNode;
					//*parentAccessPoint = left;
					travelHistory.Replace(m_UpdateSerial, iHistory, pCurNode, left);
					travelHistory.RemoveLastHistory(); // remove pcurnode before add other
				}

				travelHistory.AddHistory(left);
			}
			else// if (balance >= BalanceTolerance)
			{
				auto right = pCurNode->Right = CloneNode(pCurNode->Right.load());
				// right-left case
				if (right->Balance < 0)
				{
					auto rightLeft = right->Left = CloneNode(right->Left.load());
					Assert(rightLeft->UpdateSerial == m_UpdateSerial);
					Assert(right->UpdateSerial == m_UpdateSerial);
					pCurNode->Right = rightLeft->Left;
					right->Left = rightLeft->Right;

					rightLeft->Left = pCurNode;
					rightLeft->Right = right;

					//*parentAccessPoint = rightLeft;
					travelHistory.Replace(m_UpdateSerial, iHistory, pCurNode, rightLeft);
					travelHistory.RemoveLastHistory(); // remove pcurnode before add other
					travelHistory.AddHistory(rightLeft);
				}
				else // right-right case
				{
					Assert(right->UpdateSerial == m_UpdateSerial);
					pCurNode->Right = right->Left;
					right->Left = pCurNode;
					//*parentAccessPoint = right;
					travelHistory.Replace(m_UpdateSerial, iHistory, pCurNode, right);
					travelHistory.RemoveLastHistory(); // remove pcurnode before add other
				}

				travelHistory.AddHistory(right);
			}

			travelHistory.AddHistory(pCurNode);

			iHistory = (int)travelHistory.GetHistorySize();// -1; // loop count will be decreased before procedeed
		}
	}

	template<class KeyType, class ValueType>
	INT64 DualSortedMap<KeyType, ValueType>::CalculateOrder(OperationTraversalHistory &travelHistory, MapNode* pNode)
	{
		if (pNode == nullptr)
			return 0;

		INT64 order = 0;
		int iDepth = (int)travelHistory.GetHistorySize() - 1;
		if (travelHistory.GetLastHistory() == pNode) iDepth--; // remove himself

		for (; iDepth >= 0; iDepth--)
		{
			MapNode* pParentNode = travelHistory.GetHistory(iDepth);
			if (pParentNode->Left.load() == pNode)
			{
				auto right = pParentNode->Right.load();
				order += right == nullptr ? 1 : right->NumberOfChildren + 2;
			}
			else //if (pParentNode->Right.load() == pNode)
			{
				Assert(pParentNode->Right.load() == pNode);
			}

			pNode = pParentNode;
		}

		return order;
	}


	// Commit pending free list
	template<class KeyType, class ValueType>
	void DualSortedMap<KeyType, ValueType>::CommitPendingFree()
	{
		std::atomic_thread_fence(std::memory_order_acq_rel);

		if (m_PendingFreeList)
		{
			//m_PendingFreeListTail->pNext = m_FreeList;
			auto curNode = m_PendingFreeList;
			while (curNode != nullptr)
			{
				auto next = curNode->NextPendingFree;
				curNode->NextPendingFree = nullptr;
				FreeNode(curNode);
				curNode = next;
			}
		}

		m_PendingFreeList = nullptr;
		m_PendingFreeCount = 0;
	}

	template<class KeyType, class ValueType>
	HRESULT DualSortedMap<KeyType, ValueType>::ForeachPendingFree(std::function<void(MapNode*)> functor)
	{
		auto curNode = m_PendingFreeList;
		while (curNode != nullptr)
		{
			functor(curNode);
			curNode = curNode->NextPendingFree;
		}
		return S_OK;
	}


	template<class KeyType, class ValueType>
	typename DualSortedMap<KeyType, ValueType>::MapNode* DualSortedMap<KeyType, ValueType>::CloneNode(MapNode* mapNodeToClone)
	{
		if (mapNodeToClone == nullptr)
			return nullptr;

		//Assert(mapNodeToClone->pNext == nullptr);

		// if already new node
		if (mapNodeToClone->UpdateSerial == m_UpdateSerial)
		{
			return mapNodeToClone;
		}

		Assert(!mapNodeToClone->IsCloned);
		mapNodeToClone->IsCloned = true;

		Assert(m_UpdateSerial != mapNodeToClone->UpdateSerial);

		PendingFreeNode(mapNodeToClone);

		MapNode* newNode = nullptr;
		m_pNodePool->Alloc(newNode);
		Assert(newNode != mapNodeToClone);
		if (newNode != nullptr)
		{
			newNode->SetValue(m_UpdateSerial, *mapNodeToClone);
		}

		Assert(newNode->UpdateSerial != mapNodeToClone->UpdateSerial);

		return newNode;
	}

	template<class KeyType, class ValueType>
	typename DualSortedMap<KeyType, ValueType>::MapNode* DualSortedMap<KeyType, ValueType>::AllocateNode(KeyType key, const ValueType& value)
	{
		MapNode* newNode = nullptr;
		m_pNodePool->Alloc(newNode);
		if (newNode != nullptr)
		{
			newNode->SetValue(m_UpdateSerial, key, value);
		}
		return newNode;

	}

	template<class KeyType, class ValueType>
	void DualSortedMap<KeyType,ValueType>::FreeNode(MapNode* pNode)
	{
//#ifdef DEBUG
//		ForeachOrder(m_WriteRoot, 0, (UINT)m_ItemCount, [&pNode](MapNode* pCurNode)-> bool
//		{
//			Assert(pCurNode != pNode);
//			return true;
//		});
//		ForeachOrder(m_ReadRoot.load(), 0, (UINT)m_ItemCount, [&pNode](MapNode* pCurNode)-> bool
//		{
//			Assert(pCurNode != pNode);
//			return true;
//		});
//#endif
		pNode->Value = ValueType(0);
		m_pNodePool->Free(pNode);
	}

	template<class KeyType, class ValueType>
	void DualSortedMap<KeyType,ValueType>::PendingFreeNode(MapNode* pNode)
	{
		Assert(pNode->IsCloned);

		if (pNode == nullptr)
			return;

#ifdef DEBUG
		new(&pNode->StackTrace) CallStackTrace;
		StackWalker::CaptureCallStack(pNode->StackTrace);
#endif

		AssertRel(pNode->NextPendingFree == nullptr && pNode != m_PendingFreeList);
		pNode->NextPendingFree = m_PendingFreeList;
		m_PendingFreeList = pNode;

		m_PendingFreeCount++;

		return;
	}

