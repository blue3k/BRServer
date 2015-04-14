////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Thread safe hash table
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/BrAssert.h"
#include "Common/Indexing.h"
#include "Common/Synchronize.h"
#include "Common/SystemSynchronize.h"
#include "Common/ArrayUtil.h"
#include "Common/HashTableTrait.h"
#include "Common/SortedMap.h"
#include "Common/DualSortedMap.h"


namespace BR {
namespace Hash {


		/////////////////////////////////////////////////////////////////////////////////
		//
		//	Thread safe hash map
		//

		template<	typename KeyType,
					typename ItemType, 
					typename Trait = UniqueKeyTrait, 
					typename ThreadTrait = ThreadSyncTraitMT,
					typename HasherType = Hash::hash<KeyType> >
		class HashTable2
		{
		public:

			//typedef Hash::hash<KeyType> HasherType;
			typedef ItemType ValueType;

			__if_exists( Trait::UniqueKey )
			{
				typedef void UniqueKey;
			}

			__if_exists( ThreadTrait::ThreadSafe )
			{
				class WriteOnlyLock
				{
				public:
				private:
					CriticalSection m_WriteCriticalSection;

				public:
					WriteOnlyLock(){}
					~WriteOnlyLock(){}

					// Exclusive lock/unlock
					inline void ExLock() { m_WriteCriticalSection.Lock(); }
					inline void ExUnlock() { m_WriteCriticalSection.UnLock(); }

					// Non-Exclusive lock/unlock
					inline void NonExLock() {}
					inline void NonExUnlock() {}

					// Query status
					inline CounterType GetTicketCount() const { return 0; }
					inline CounterType GetNonExclusiveCount() const { return 0; }
					inline bool	IsLocked() const { return false; }
				};

				typedef WriteOnlyLock	TicketLockType;
				typedef DualSortedMap<KeyType, ItemType> BucketContainer;
			}
			__if_not_exists( ThreadTrait::ThreadSafe )
			{
				typedef FakeTicketLock	TicketLockType;
				typedef SortedMap<KeyType, ItemType> BucketContainer;
			}

			// Hash bucket
			class Bucket
			{
			public:
				// thread lock for bucket access
				TicketLockType	m_Lock;

			public:

				void ReadLock()
				{
					m_Lock.NonExLock();
				}

				void ReadUnlock()
				{
					m_Lock.NonExUnlock();
				}

				void WriteLock()
				{
					m_Lock.ExLock();
				}

				void WriteUnlock()
				{
					m_Lock.ExUnlock();
				}

				friend HashTable2;


			public:
				// Bucket item container
				BucketContainer	*m_Items;


				// Constructor
				Bucket()
					: m_Items(nullptr)
				{
				}

				// Copy Constructor 
				Bucket( const Bucket& src )
					: m_Items(nullptr)
				{
					// No one use this bucket, while this operation
					//Assert( !src.m_Lock.IsLocked() );
					Assert(false);
				}

				//Bucket( Bucket&& src )
				//	:m_Items(src.m_Items)
				//{
				//	// No one use this bucket, while this operation
				//	Assert( !src.m_Lock.IsLocked() );
				//}

				// Destructor
				~Bucket()
				{
					if (m_Items) delete m_Items;
				}

				Bucket& operator = ( const Bucket& src )
				{
					// No one use this bucket, while this operation
					Assert( !src.m_Lock.IsLocked() );
					Assert( !m_Lock.IsLocked() );

					//m_Items = src.m_Items;
					Assert(false);
					return *this;
				}

				Bucket& operator = ( Bucket&& src )
				{
					// No one use this bucket, while this operation
					Assert( !src.m_Lock.IsLocked() );
					Assert( !m_Lock.IsLocked() );

					//m_Items = src.m_Items;
					Assert(false);
					return *this;
				}

				void SetObjectPool(typename BucketContainer::ObjectPool &objectPool)
				{
					if (m_Items) delete m_Items;
					m_Items = new BucketContainer();
				}

				// validate bucket id
				bool Validate( size_t iMyBucket, size_t szNumBucket )
				{
					bool isSuccess = true;
					// Validate only debug mode
//#ifdef _DEBUG
//					std::atomic_thread_fence(std::memory_order_acquire);
//					m_Items->ForeachOrder(0, (UINT)m_Items->GetItemCount(), [](const KeyType& key, const ValueType& value) -> bool
//					{
//						size_t hashVal = HasherType()(key);
//						size_t iBucket = hashVal%szNumBucket;
//						AssertRel( iBucket == iMyBucket );
//						if( iBucket != iMyBucket )
//						{
//							isSuccess &= false;
//						}
//						return true;
//					});
//#endif
					return isSuccess;
				}
			};



		private:

			typename BucketContainer::ObjectPool m_ObjectPool;

			// bucket
			std::vector<Bucket> m_Buckets;

			// total count of item
			SyncCounter		m_lItemCount;

		public:

			HashTable2( INT iBucketCount = 16 )
				:m_lItemCount(0)
			{
				SetBucketCount(iBucketCount);
			}

			virtual ~HashTable2()
			{
			}

			CounterType GetItemCount()								{ return m_lItemCount.load(std::memory_order_relaxed); }

			// resize bucket
			// NOTE: This method is NOT THREAD SAFE and NOT DATA SAFE
			void SetBucketCount( size_t iBucketCount )
			{
				m_Buckets.resize( iBucketCount );
				for (int iBucket = 0; iBucket < m_Buckets.size(); iBucket++)
				{
					m_Buckets[iBucket].SetObjectPool(m_ObjectPool);
				}
			}

			// Bucket Count
			size_t GetBucketCount() const							{ return m_Buckets.size(); }



			//////////////////////////////////////////////////////////////////////////
			//
			//	Insert/erase/clear
			//

			HRESULT Insert( const KeyType key, const ItemType &data )
			{
				size_t hashVal = HasherType()(key);
				size_t iBucket = hashVal%m_Buckets.size();

				Bucket& bucket = m_Buckets[iBucket];
				TicketScopeLockT<TicketLockType> scopeLock( TicketLock::LOCK_EXCLUSIVE, bucket.m_Lock );

				__if_exists(Trait::UniqueKey)
				{
					ItemType dataFound;
					__if_exists(ThreadTrait::ThreadSafe)
					{
						if (SUCCEEDED(bucket.m_Items->FindInWriteTree(key, dataFound)))
						{
							return E_FAIL;
						}
					}
					__if_not_exists(ThreadTrait::ThreadSafe)
					{
						if (SUCCEEDED(bucket.m_Items->Find(key, dataFound)))
						{
							return E_FAIL;
						}
					}
				}
				bucket.m_Items->Insert(key, data);
				__if_exists(ThreadTrait::ThreadSafe)
				{
					bucket.m_Items->CommitChanges();
				}

				m_lItemCount.fetch_add(1,std::memory_order_relaxed);

#ifdef _DEBUG
				Assert( bucket.Validate(iBucket, m_Buckets.size()) );
#endif
				return S_OK;
			}

			HRESULT Find( const KeyType& keyVal, ItemType &data )
			{
				size_t hashVal = HasherType()( keyVal );
				size_t iBucket = hashVal%m_Buckets.size();

				Bucket& bucket = m_Buckets[iBucket];
				TicketScopeLockT<TicketLockType> scopeLock( TicketLock::LOCK_NONEXCLUSIVE, bucket.m_Lock );

				HRESULT hr = bucket.m_Items->Find(keyVal, data);
				//__if_exists(ItemType::m_pObject)
				//{
				//	if (SUCCEEDED(hr))
				//	{
				//		AssertRel(data != ItemType(0));
				//		//AssertRel(data->Value == keyVal);
				//	}
				//}
				return hr;
			}

			// Erase a data from hash map
			HRESULT Erase(const KeyType &key, ValueType& erasedValue)
			{
				if (m_Buckets.size() == 0)
					return S_FALSE;

				size_t hashVal = HasherType()(key);
				size_t iBucket = hashVal%m_Buckets.size();

				Bucket& bucket = m_Buckets[iBucket];
				TicketScopeLockT<TicketLockType> scopeLock(TicketLock::LOCK_EXCLUSIVE, bucket.m_Lock);

				if (SUCCEEDED(bucket.m_Items->Remove(key, erasedValue)))
				{
					__if_exists(ThreadTrait::ThreadSafe)
					{
						bucket.m_Items->CommitChanges();
					}
					m_lItemCount.fetch_sub(1, std::memory_order_relaxed);
					return S_OK;
				}

				Assert(bucket.Validate(iBucket, m_Buckets.size()));

				return E_FAIL;
			}


			HRESULT Clear()
			{
				m_lItemCount = 0;

				auto iterBucket = m_Buckets.begin();
				for( ; iterBucket != m_Buckets.end(); ++iterBucket )
				{
					iterBucket->m_Items->ClearMap();
				}

				return S_OK;
			}

			bool Validate()
			{
#ifdef _DEBUG
				BucketListType::iterator iterBucket = m_Buckets.begin();
				for( INT iBucket = 0; iterBucket != m_Buckets.end(); ++iterBucket, ++iBucket )
				{
					if( !iterBucket->Validate( iBucket, m_Buckets.size() ) )
						return false;
				}
#endif
				return true;
			}

			void Foreach(std::function<bool(const KeyType& key, const ItemType& item)> func)
			{
				auto iterBucket = m_Buckets.begin();
				for (INT iBucket = 0; iterBucket != m_Buckets.end(); ++iterBucket, ++iBucket)
				{
					if (iterBucket->m_Items == nullptr)
						continue;

					iterBucket->m_Items->ForeachOrder(0, (UINT)iterBucket->m_Items->GetItemCount(), func);
				}

			}
		};


}; // namespace Hash
}; // namespace BR




