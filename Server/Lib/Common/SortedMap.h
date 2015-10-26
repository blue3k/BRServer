////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 MadK
// 
// Author : KyungKun Ko
//
// Description : Sorted map
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Synchronize.h"
#include "Common/ArrayUtil.h"
#include "Common/ObjectPool.h"

namespace BR
{
	///////////////////////////////////////////////////////////
	//
	//	Concurrent SortedMap
	//	- Implementation of concurrent AVLTree
	//

	template<class KeyType, class ValueType>
	class SortedMap
	{
	public:

		typedef CounterType SynchronizeCounterType;

		enum { 
			BalanceTolerance  = 8,
			MaxRebalancingTry = 20
		};


		class MapNode;
		typedef MapNode* ReferenceAccessPoint;

		class MapNode : public ObjectPoolObject
		{
		public:
			KeyType Key;
			ValueType Value;

			INT Balance;
			INT DepthOfChildren;
			// Number of nodes - include child
			INT NumberOfChildren;

			ReferenceAccessPoint Left;
			ReferenceAccessPoint Right;

			MapNode()
				: Balance(0)
				, DepthOfChildren(0)
				, NumberOfChildren(0)
				, Key(0)
				, Value(0)
				, Left(nullptr)
				, Right(nullptr)
			{
			}

			void SetValue()
			{
				Balance = 0;
				DepthOfChildren = 0;
				NumberOfChildren = 0;
				Key = 0;
				Value = 0;
				Left = nullptr;
				Right = nullptr;
			}

			void SetValue(KeyType key, ValueType value)
			{
				Balance = 0;
				DepthOfChildren = 0;
				NumberOfChildren = 0;
				Key = key;
				Value = value;
				Left = nullptr;
				Right = nullptr;
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

			ReferenceAccessPoint &m_Root;
			StaticArray<MapNode*, GrowthBy*2> m_TraversalHistory;

		public:

			OperationTraversalHistory(ReferenceAccessPoint& root, SynchronizeCounterType totalItemCount)
				: m_Root(root)
			{
				Assert((size_t)ceil(log2(totalItemCount + 1)) <= m_TraversalHistory.GetAllocatedSize());
				//m_TraversalHistory.Reserve();
				m_TraversalHistory.SetIncreaseSize(GrowthBy);
			}

			size_t GetHistorySize()									{ return m_TraversalHistory.GetSize(); }

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
				m_TraversalHistory.SetSize(m_TraversalHistory.GetSize()-1);
			}

			void TruncateHistoryFrom(int iIndex)					{ m_TraversalHistory.SetSize(iIndex); }

			MapNode* GetHistory(int iIndex)							{ return m_TraversalHistory[iIndex]; }

			MapNode* GetLastHistory()								{ if (m_TraversalHistory.GetSize() == 0) return nullptr; return m_TraversalHistory[m_TraversalHistory.GetSize()-1]; }

			// set Reserve size
			HRESULT Reserve(size_t szReserv)
			{
				if (szReserv <= m_TraversalHistory.GetAllocatedSize())
					return S_OK;

				szReserv = GrowthBy * ((szReserv + GrowthBy - 1) / GrowthBy);

				return m_TraversalHistory.Reserve(szReserv);
			}

			ReferenceAccessPoint* GetParentAccessPoint(int nodeIndex, MapNode* pNode);
		};


		
	private:

		// Root node
		ReferenceAccessPoint m_Root;

		// item count in the tree
		SynchronizeCounterType m_ItemCount;

		bool m_DeleteNodePool;
		ObjectPool *m_pNodePool;

	public:

		SortedMap(ObjectPool *pNodePool = nullptr);
		~SortedMap();

		void ClearMap();

		// Insert a key
		HRESULT Insert(KeyType key, const ValueType& value, INT64 *insertedOrder = nullptr);

		// Remove an item and return the removed value
		HRESULT Remove(KeyType key, ValueType& value);

		// Find a key value
		HRESULT Find(KeyType key, ValueType& value, INT64 *pOrder = nullptr);

		// get number of values
		SynchronizeCounterType GetItemCount()								{ return m_ItemCount; }

		// enumerate the values
		HRESULT ForeachOrder(INT startOrderIndex, UINT count, const std::function<bool(const KeyType&, const ValueType&)>& functor);
		HRESULT ForeachReverseOrder(INT startOrderIndex, UINT count, const std::function<bool(const KeyType&, const ValueType&)>& functor);


	private:

		// find parent node or candidate
		HRESULT FindNode(OperationTraversalHistory &travelHistory, KeyType key, MapNode* &pNode);

		MapNode* FindSmallestNode(OperationTraversalHistory &travelHistory, MapNode* pRootNode);
		MapNode* FindBiggestNode(OperationTraversalHistory &travelHistory, MapNode* pRootNode);

		// Update valance factor and return new balance value
		void FixupBalance(OperationTraversalHistory &travelHistory);

		INT64 CalculateOrder(OperationTraversalHistory &travelHistory, MapNode* pNode);

		MapNode* AllocateNode(KeyType key, const ValueType& value);
		void FreeNode(MapNode* pNode);
	};

	#include "SortedMap.inl"

}

