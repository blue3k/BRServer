////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 MadK
// 
// Author : KyungKun Ko
//
// Description : SortedMap(Multithread)
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Synchronization.h"
#include "Common/ArrayUtil.h"
#include "Common/ObjectPool.h"
#include "Common/StackWalker.h"


namespace BR
{
	///////////////////////////////////////////////////////////
	//
	//	Concurrent SortedMap
	//	- Implementation of concurrent AVLTree
	//

	template<class KeyType, class ValueType>
	class DualSortedMap
	{
	public:
		//typedef UINT KeyType;
		//typedef UINT ValueType;

		typedef CounterType SynchronizeCounterType;
		typedef std::atomic<SynchronizeCounterType> SynchronizeCounter;

		enum {
			BalanceTolerance = 4,
			MaxRebalancingTry = 20
		};



		class MapNode;

#define DEBUG_NODEPTR
		//typedef MapNode* ReferenceAccessPoint;
		class ReferenceAccessPoint
		{
		private:
			std::atomic<MapNode*> m_NodePtr;
#ifdef DEBUG_NODEPTR
			MapNode* m_DebugPtr;
#endif
		public:
			ReferenceAccessPoint()
				: m_NodePtr(nullptr)
#ifdef DEBUG_NODEPTR
				, m_DebugPtr(nullptr)
#endif
			{}

			ReferenceAccessPoint(ReferenceAccessPoint& src)
			{
				auto srcPtr = src.m_NodePtr.load(std::memory_order_relaxed);
#ifdef DEBUG_NODEPTR
				m_DebugPtr = srcPtr;
#endif
				m_NodePtr.store(srcPtr, std::memory_order_relaxed);
			}

			ReferenceAccessPoint(MapNode* pNode)
			{
				auto srcPtr = pNode;
#ifdef DEBUG_NODEPTR
				m_DebugPtr = srcPtr;
#endif
				m_NodePtr.store(srcPtr, std::memory_order_relaxed);
			}

			MapNode* load(std::memory_order order = std::memory_order_relaxed) const
			{
				return m_NodePtr.load(order);
			}

			MapNode* store(MapNode* pNode, std::memory_order order = std::memory_order_relaxed)
			{
				m_NodePtr.store(pNode, order);
#ifdef DEBUG_NODEPTR
				m_DebugPtr = pNode;
#endif
					return pNode;
			}

			bool operator != (MapNode* pNode)				{ return load() != pNode; }
			bool operator == (MapNode* pNode)				{ return load() == pNode; }

			MapNode* operator = (const ReferenceAccessPoint& src)
			{
				MapNode* pNode = src.load();
				store(pNode);
				return pNode;
			}

			MapNode* operator = (MapNode* pNode)
			{
				store(pNode);
				return pNode;
			}
		};


		class MapNode : public ObjectPoolObject
		{
		public:
			KeyType Key;
			ValueType Value;

			INT Balance;
			INT DepthOfChildren;
			// Number of nodes - include child
			INT NumberOfChildren;

			UINT UpdateSerial;

			ReferenceAccessPoint Left;
			ReferenceAccessPoint Right;

			MapNode* NextPendingFree;
#ifdef DEBUG
			CallStackTrace StackTrace;
#endif
			bool IsCloned;

			MapNode()
				: Key(0)
				, Value(0)
				, Balance(0)
				, DepthOfChildren(0)
				, NumberOfChildren(0)
				, UpdateSerial(0)
				, Left(nullptr)
				, Right(nullptr)
				, NextPendingFree(nullptr)
				, IsCloned(false)
			{
			}

			void SetValue()
			{
				Balance = 0;
				DepthOfChildren = 0;
				NumberOfChildren = 0;
				Key = 0;
				Value = 0;
				IsCloned = false;
				Left = nullptr;
				Right = nullptr;
				UpdateSerial = 0;
			}

			void SetValue(UINT updateSerial, KeyType key, ValueType value)
			{
				Balance = 0;
				DepthOfChildren = 0;
				NumberOfChildren = 0;
				Key = key;
				Value = value;
				IsCloned = false;
				Left = nullptr;
				Right = nullptr;
				UpdateSerial = updateSerial;
			}

			void SetValue(UINT updateSerial, const MapNode& src)
			{
				Balance = src.Balance;
				DepthOfChildren = src.DepthOfChildren;
				NumberOfChildren = src.NumberOfChildren;
				Key = src.Key;
				Value = src.Value;
				IsCloned = false;
				Left = src.Left;
				Right = src.Right;
				UpdateSerial = updateSerial;
			}

			int UpdateBalanceFactor();
		};

		typedef ObjectPoolT<MapNode> ObjectPool;

	private:

		// Search history buffer
		class OperationTraversalHistory
		{
		public:

			enum { GrowthBy = 32 };

		private:

			MapNode* &m_Root;
			StaticArray<MapNode*, GrowthBy*2> m_TraversalHistory;

		public:

			OperationTraversalHistory(MapNode* &root, SynchronizeCounterType totalItemCount)
				: m_Root(root)
			{
				Assert((size_t)ceil(log2(totalItemCount + 1)) <= m_TraversalHistory.GetAllocatedSize());
				m_TraversalHistory.SetIncreaseSize(GrowthBy);
			}

			size_t GetHistorySize()											{ return m_TraversalHistory.GetSize(); }

			void Clear()
			{
				m_TraversalHistory.Clear();
			}

			void SetConserveDataOnResize(bool conserveDataOnResize)
			{
				m_TraversalHistory.SetConserveDataOnResize(conserveDataOnResize);
			}

			void AddHistory(MapNode* pNode)
			{
				m_TraversalHistory.push_back(pNode);
			}

			void RemoveLastHistory()
			{
				Assert(m_TraversalHistory.GetSize() > 0);
				m_TraversalHistory.SetSize(m_TraversalHistory.GetSize() - 1);
			}

			void TruncateHistoryFrom(int iIndex)									{ m_TraversalHistory.SetSize(iIndex); }

			MapNode* GetHistory(int iIndex)											{ return m_TraversalHistory[iIndex]; }

			MapNode* GetLastHistory()												{ if (m_TraversalHistory.GetSize() == 0) return nullptr; return m_TraversalHistory[m_TraversalHistory.GetSize() - 1]; }

			// set Reserve size
			Result Reserve(size_t szReserv)
			{
				if (szReserv <= m_TraversalHistory.GetAllocatedSize())
					return ResultCode::SUCCESS;

				szReserv = GrowthBy * ((szReserv + GrowthBy - 1) / GrowthBy);

				return m_TraversalHistory.Reserve(szReserv);
			}


			void Replace(UINT updateSerial, int nodeIndex, MapNode* pNode, MapNode* pNewNode);
		};


	
	private:

		// read Root
		// ping pong counter
		SyncCounter m_ReadCount[3];
		ReferenceAccessPoint m_ReadRoot;
		SynchronizeCounter m_ReadIndex;

		//For read debug
		MapNode* m_CurReadRoot;
		volatile MapNode* m_PrevReadRoot;

		// Write root
		MapNode* m_WriteRoot;

		bool m_DeleteNodePool;
		ObjectPool *m_pNodePool;

		UINT		m_PendingFreeCount;
		MapNode*	m_PendingFreeList;
		//MapNode*	m_PendingFreeListTail;


		UINT m_UpdateSerial;
		bool m_IsModified;

		// item count in the tree
		SynchronizeCounter m_ItemCount;
		SynchronizeCounterType m_ReadItemCount;

	public:

		DualSortedMap(ObjectPool *pNodePool = nullptr);
		~DualSortedMap();

		void ClearMap();

		// Insert a key
		Result Insert(KeyType key, const ValueType& value, INT64 *insertedOrder = nullptr);

		// Remove an item and return the removed value
		Result Remove(KeyType key, ValueType& value);

		Result FindInWriteTree(KeyType key, ValueType& value);

		// commit changes
		Result CommitChanges();

		// Find a key value
		Result Find(KeyType key, ValueType& value, INT64 *pOrder = nullptr);

		// get number of values
		SynchronizeCounterType GetItemCount()									{ return m_ReadItemCount; }
		SynchronizeCounterType GetWriteItemCount()								{ return m_ItemCount.load(std::memory_order_relaxed);  }

		// enumerate the values
		Result ForeachOrder(INT startOrderIndex, UINT count, const std::function<bool(const KeyType&, const ValueType&)>& functor);

		template<class Func>
		Result ForeachReverseOrder(INT startOrderIndex, UINT count, Func functor)
		{
			auto readIdx = m_ReadIndex.load(std::memory_order_relaxed) % countof(m_ReadCount);
			ScopeCounter localCounter(m_ReadCount[readIdx]);

			m_CurReadRoot = m_ReadRoot.load(std::memory_order_acquire);
			auto readRoot = (MapNode*)m_CurReadRoot;
			if (readRoot == nullptr)
				return ResultCode::FAIL;

			return ForeachReverseOrder(readRoot, startOrderIndex, count, functor);
		}

		//
		Result ForeachOrderWrite(INT startOrderIndex, UINT count, const std::function<bool(const KeyType&, const ValueType&)>& functor);

	private:

		// find parent node or candidate
		Result FindNode(OperationTraversalHistory &travelHistory, KeyType key, MapNode* &pNode);

		MapNode* FindSmallestNode(OperationTraversalHistory &travelHistory, MapNode* pRootNode);
		MapNode* FindBiggestNode(OperationTraversalHistory &travelHistory, MapNode* pRootNode);

		Result FindNodeRead(OperationTraversalHistory &travelHistory, MapNode* pRootNode, KeyType key, MapNode* &pNode);
		MapNode* FindSmallestNodeRead(OperationTraversalHistory &travelHistory, MapNode* pRootNode);
		MapNode* FindBiggestNodeRead(OperationTraversalHistory &travelHistory, MapNode* pRootNode);

		Result ForeachOrder(MapNode* pRootNode, INT startOrderIndex, UINT count, const std::function<bool(const KeyType&, const ValueType&)>& functor);

		template<class Func>
		Result ForeachReverseOrder(MapNode* pRootNode, INT startOrderIndex, UINT count, Func functor)
		{
			if (pRootNode == nullptr)
				return ResultCode::SUCCESS;

			OperationTraversalHistory travelHistory(pRootNode, m_ReadItemCount);

			MapNode* pCurNode = pRootNode;
			if (pCurNode == nullptr)
			{
				return ResultCode::SUCCESS;
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
				return ResultCode::SUCCESS;
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


			return ResultCode::SUCCESS;
		}
		//RoateLeft(pNode);
		// Update valance factor and return new balance value
		void FixupBalance(OperationTraversalHistory &travelHistory);

		INT64 CalculateOrder(OperationTraversalHistory &travelHistory, MapNode* pNode);

		// Commit pending free list
		void CommitPendingFree();
		Result ForeachPendingFree(std::function<void(MapNode*)> functor);


		MapNode* CloneNode(MapNode* mapNodeToClone);
		MapNode* AllocateNode(KeyType key, const ValueType& value);
		void FreeNode(MapNode* pNode);
		void PendingFreeNode(MapNode* pNode);
	};

#include "DualSortedMap.inl"

}

