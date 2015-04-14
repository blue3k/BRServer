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

#include "Common\Synchronize.h"
#include "Common\ArrayUtil.h"
#include "Common\ObjectPool.h"
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
				: Balance(0)
				, DepthOfChildren(0)
				, NumberOfChildren(0)
				, Key(0)
				, Value(0)
				, IsCloned(false)
				, Left(nullptr)
				, Right(nullptr)
				, NextPendingFree(nullptr)
				, UpdateSerial(0)
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
			HRESULT Reserve(size_t szReserv)
			{
				if (szReserv <= m_TraversalHistory.GetAllocatedSize())
					return S_OK;

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
		HRESULT Insert(KeyType key, const ValueType& value, INT64 *insertedOrder = nullptr);

		// Remove an item and return the removed value
		HRESULT Remove(KeyType key, ValueType& value);

		HRESULT FindInWriteTree(KeyType key, ValueType& value);

		// commit changes
		HRESULT CommitChanges();

		// Find a key value
		HRESULT Find(KeyType key, ValueType& value, INT64 *pOrder = nullptr);

		// get number of values
		SynchronizeCounterType GetItemCount()									{ return m_ReadItemCount; }
		SynchronizeCounterType GetWriteItemCount()								{ return m_ItemCount.load(std::memory_order_relaxed);  }

		// enumerate the values
		HRESULT ForeachOrder(INT startOrderIndex, UINT count, const std::function<bool(const KeyType&, const ValueType&)>& functor);
		HRESULT ForeachReverseOrder(INT startOrderIndex, UINT count, const std::function<bool(const KeyType&, const ValueType&)>& functor);

		//
		HRESULT ForeachOrderWrite(INT startOrderIndex, UINT count, const std::function<bool(const KeyType&, const ValueType&)>& functor);

	private:

		// find parent node or candidate
		HRESULT FindNode(OperationTraversalHistory &travelHistory, KeyType key, MapNode* &pNode);

		MapNode* FindSmallestNode(OperationTraversalHistory &travelHistory, MapNode* pRootNode);
		MapNode* FindBiggestNode(OperationTraversalHistory &travelHistory, MapNode* pRootNode);

		HRESULT FindNodeRead(OperationTraversalHistory &travelHistory, MapNode* pRootNode, KeyType key, MapNode* &pNode);
		MapNode* FindSmallestNodeRead(OperationTraversalHistory &travelHistory, MapNode* pRootNode);
		MapNode* FindBiggestNodeRead(OperationTraversalHistory &travelHistory, MapNode* pRootNode);

		HRESULT ForeachOrder(MapNode* pRootNode, INT startOrderIndex, UINT count, const std::function<bool(const KeyType&, const ValueType&)>& functor);
		HRESULT ForeachReverseOrder(MapNode* pRootNode, INT startOrderIndex, UINT count, const std::function<bool(const KeyType&, const ValueType&)>& functor);

		//RoateLeft(pNode);
		// Update valance factor and return new balance value
		void FixupBalance(OperationTraversalHistory &travelHistory);

		INT64 CalculateOrder(OperationTraversalHistory &travelHistory, MapNode* pNode);

		// Commit pending free list
		void CommitPendingFree();
		HRESULT ForeachPendingFree(std::function<void(MapNode*)> functor);


		MapNode* CloneNode(MapNode* mapNodeToClone);
		MapNode* AllocateNode(KeyType key, const ValueType& value);
		void FreeNode(MapNode* pNode);
		void PendingFreeNode(MapNode* pNode);
	};

#include "DualSortedMap.inl"

}

