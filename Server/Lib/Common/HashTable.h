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
#include "Common/ArrayUtil.h"
#include "Common/HashTableTrait.h"


namespace BR {
namespace Hash {


		/////////////////////////////////////////////////////////////////////////////////
		//
		//	Thread safe hash map
		//

		template<	typename ItemType, 
					typename Indexer, 
					typename Trait = UniqueKeyTrait, 
					typename ThreadTrait = ThreadSyncTraitMT,
					typename Hasher = Hash::hash<Indexer::Type>,
					typename MapItemType = MapItem<typename Indexer::Type, typename ItemType>,
					typename BucketContainer = std::vector<MapItemType> >
		class HashTable
		{
		public:

			__if_exists( Trait::UniqueKey )
			{
				typedef void UniqueKey;
			}

			typedef typename ItemType ItemType;
			typedef typename Indexer Indexer;
			typedef typename Indexer::Type KeyType;

			__if_exists( ThreadTrait::ThreadSafe )
			{
			typedef TicketLock	TicketLockType;
			}
			__if_not_exists( ThreadTrait::ThreadSafe )
			{
			typedef FakeTicketLock	TicketLockType;
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

				friend HashTable;


			public:
				// Bucket item container
				BucketContainer	m_Items;


				// Constructor
				Bucket()
				{
				}

				// Copy Constructor 
				Bucket( const Bucket& src )
					:m_Items(src.m_Items)
				{
					// No one use this bucket, while this operation
					Assert( !src.m_Lock.IsLocked() );
				}

				Bucket( Bucket&& src )
					:m_Items(src.m_Items)
				{
					// No one use this bucket, while this operation
					Assert( !src.m_Lock.IsLocked() );
				}

				// Destructor
				~Bucket()
				{
				}

				Bucket& operator = ( const Bucket& src )
				{
					// No one use this bucket, while this operation
					Assert( !src.m_Lock.IsLocked() );
					Assert( !m_Lock.IsLocked() );

					m_Items = src.m_Items;
					return *this;
				}

				Bucket& operator = ( Bucket&& src )
				{
					// No one use this bucket, while this operation
					Assert( !src.m_Lock.IsLocked() );
					Assert( !m_Lock.IsLocked() );

					m_Items = src.m_Items;
					return *this;
				}

				// validate bucket id
				bool Validate( size_t iMyBucket, size_t szNumBucket )
				{
					// Validate only debug mode
#ifdef _DEBUG
					//_ReadBarrier();

					BucketContainer::iterator iter = m_Items.begin();
					for( ; iter != m_Items.end(); ++iter )
					{
						Indexer::Type curIdx = iter->Key;
						size_t hashVal = Hasher()( curIdx );
						size_t iBucket = hashVal%szNumBucket;
						AssertRel( iBucket == iMyBucket );
						if( iBucket != iMyBucket )
						{
							return false;
						}
					}
#endif
					return true;
				}
			};

			// Bucket list container type
			typedef std::vector<Bucket> BucketListType;


			///////////////////////////////////////////
			// Hash table iterator
			class iterator
			{
			private:
				// Is Bucket internal iterator?, then only iterate inside bucket
				bool m_bIsIterInBucket;
				// Bucket list iterator. directing which bucket
				typename BucketListType::iterator m_iterBucket;
				// Main container pointer
				HashTable			*m_pContainer;
				// Index in bucket container
				INT					m_iIdx;
				// MapItemType
				mutable MapItemType		*m_pCache;

				enum { END_IDX = -1 };

				// constructor
				iterator( HashTable *pContainer, typename BucketListType::iterator iterBucket, INT iIdx, bool bIsBucketIter = false )
					:m_bIsIterInBucket(bIsBucketIter),
					m_pContainer(pContainer),
					m_iterBucket(iterBucket),
					m_iIdx(iIdx)
				{
					m_pCache = nullptr;
					if( m_pContainer && m_iterBucket != m_pContainer->bucket_end() && m_iIdx > END_IDX )
						m_iterBucket->ReadLock();
				}

				friend class HashTable;


				void NextIter()
				{
					Assert(m_pContainer != NULL);
					Assert( m_iterBucket != m_pContainer->bucket_end() );
					Assert( m_iIdx >= 0 );
					m_iIdx++;
					if( m_iIdx < (INT)m_iterBucket->m_Items.size() )
						return;

					if( m_bIsIterInBucket )
					{
						m_iterBucket = m_pContainer->bucket_end();
						m_iIdx = END_IDX;
						return;
					}

					m_iterBucket->ReadUnlock();
					m_iterBucket++;
					while( m_iterBucket != m_pContainer->bucket_end() )
					{
						if( m_iterBucket->m_Items.size() != 0 )
						{
							m_iterBucket->ReadLock();
							if( m_iterBucket->m_Items.size() != 0 )
							{
								m_iIdx = 0;
								return;
							}
							// failed search again
							m_iterBucket->ReadUnlock();
						}
						m_iterBucket++;
					}

					m_iIdx = END_IDX;
				}

				// set iter
				void Set( HashTable *pContainer, typename BucketListType::iterator iterBucket, INT iIdx, bool bIsLock = true, bool bIsBucketIter = false )
				{
					if( m_pContainer && m_iterBucket != m_pContainer->bucket_end() && m_iIdx > END_IDX )
						m_iterBucket->ReadUnlock();

					m_bIsIterInBucket = bIsBucketIter;
					m_pContainer = pContainer;
					m_iterBucket = iterBucket;
					m_iIdx = iIdx;

					if( bIsLock && m_pContainer && m_iterBucket != m_pContainer->bucket_end() && m_iIdx > END_IDX )
						m_iterBucket->ReadLock();
				}

			public:
				iterator()
					:m_bIsIterInBucket(false),
					m_pContainer(NULL),
					m_iIdx(END_IDX)
				{
				}

				iterator( const iterator& src )
					:m_bIsIterInBucket(false),
					m_pContainer(src.m_pContainer),
					m_iterBucket(src.m_iterBucket),
					m_iIdx(src.m_iIdx)
				{
					if( m_pContainer && m_iterBucket != m_pContainer->bucket_end() )
					{
						m_iterBucket->ReadLock();
					}

				}

				~iterator()
				{
					if( m_pContainer && m_iterBucket != m_pContainer->bucket_end() /*&& m_iIdx > END_IDX*/ )
						m_iterBucket->ReadUnlock();
				}


				iterator& operator++()
				{
					NextIter();

					return *this;
				}

				const iterator& operator++() const
				{
					NextIter();

					return *this;
				}


				ItemType& operator* ()
				{
					AssertRel(m_pContainer != NULL);
					m_pCache = &m_iterBucket->m_Items[m_iIdx];
					return m_iterBucket->m_Items[m_iIdx].Data;
				}

				const ItemType& operator* () const
				{
					AssertRel(m_pContainer != NULL);
					m_pCache = &m_iterBucket->m_Items[m_iIdx];
					return m_iterBucket->m_Items[m_iIdx].Data;
				}

				ItemType& operator-> ()
				{
					AssertRel(m_pContainer != NULL);
					m_pCache = &m_iterBucket->m_Items[m_iIdx];
					return m_iterBucket->m_Items[m_iIdx].Data;
				}

				const ItemType& operator-> () const
				{
					AssertRel(m_pContainer != NULL);
					m_pCache = &m_iterBucket->m_Items[m_iIdx];
					return m_iterBucket->m_Items[m_iIdx].Data;
				}

				bool operator !=( const iterator& op ) const
				{
					return ( (m_pContainer != op.m_pContainer) || (m_iterBucket != op.m_iterBucket) || (m_iIdx != op.m_iIdx) );
				}

				bool operator ==( const iterator& op ) const
				{
					return ( (m_pContainer == op.m_pContainer) && (m_iterBucket == op.m_iterBucket) && (m_iIdx == op.m_iIdx) );
				}

				// Check validity
				bool IsValid()
				{
					return m_pContainer && m_iterBucket != m_pContainer->bucket_end();
				}

				// reset iterator and make invalid
				void Reset()
				{
					*this = nullptr;
				}

				iterator& operator = ( const iterator& op )
				{
					if( m_pContainer && m_iterBucket != m_pContainer->bucket_end() )
					{
						m_iterBucket->ReadUnlock();
					}

					m_pContainer = op.m_pContainer;
					m_iterBucket = op.m_iterBucket;
					m_iIdx = op.m_iIdx;

					//// if some write lock occurred after op gain read lock then this case make Dead lock
					//// if you got this assert then use with another way
					//Assert( m_pContainer == NULL || m_iterBucket == m_pContainer->bucket_end() );
					if( m_pContainer && m_iterBucket != m_pContainer->bucket_end() )
					{
						m_iterBucket->ReadLock();
					}

					return *this;
				}

				iterator& operator = ( const void* pPtr )
				{
					Assert( pPtr == 0 );

					if( m_pContainer && m_iterBucket != m_pContainer->bucket_end() )
					{
						m_iterBucket->ReadUnlock();
						m_iterBucket = m_pContainer->bucket_end();
					}

					m_pContainer = NULL;
					m_iIdx = END_IDX;

					// if some write lock occurred after op gain read lock then this case make Dead lock
					// if you got this assert then use with another way
					Assert( m_pContainer == NULL || m_iterBucket == m_pContainer->bucket_end() );

					return *this;
				}
			};

		private:
			// bucket
			BucketListType m_Bucket;

			// total count of item
			std::atomic<LONG>		m_lItemCount;

		public:

			HashTable( INT iBucketCount = 16 )
				:m_lItemCount(0)
			{
				m_Bucket.resize( iBucketCount );
			}

			virtual ~HashTable()
			{
			}

			// resize bucket
			// NOTE: This method is NOT THREAD SAFE and NOT DATA SAFE
			void SetBucketCount( size_t iBucketCount )
			{
				m_Bucket.resize( iBucketCount );
			}

			// Bucket Count
			size_t GetBucketCount() const
			{
				return m_Bucket.size();
			}

			// Get bucket 
			const Bucket& GetBucket( size_t iBucket ) const
			{
				return m_Bucket[iBucket];
			}
			Bucket& GetBucket( size_t iBucket )
			{
				return m_Bucket[iBucket];
			}


			// Get bucket size, count of item in bucket
			size_t GetBucketSize( size_t iBucket ) const
			{
				return m_Bucket[iBucket].m_Items.size();
			}


			// bucket iterator
			typename BucketListType::iterator bucket_begin()
			{
				return m_Bucket.begin();
			}

			typename BucketListType::iterator bucket_end()
			{
				return m_Bucket.end();
			}

			// Iterator 
			HRESULT begin( iterator &iter )
			{
				iter = begin();

				return iter.IsValid() ? S_OK : E_FAIL;
			}

			iterator begin()
			{
				BucketListType::iterator iterBucket = m_Bucket.begin();
				iterator iter = end();
				for( ; iterBucket != m_Bucket.end(); ++iterBucket )
				{
					if( iterBucket->m_Items.size() > 0 )
					{
						iter.Set( this, iterBucket, 0 );
						return iter;
					}
				}

				return iter;
			}

			iterator end()
			{
				return iterator(this,m_Bucket.end(),iterator::END_IDX);
			}

			long size()
			{
				return m_lItemCount.load(std::memory_order_relaxed);
			}


			//////////////////////////////////////////////////////////////////////////
			//
			//	Insert/erase/clear
			//

			HRESULT insert( const ItemType &data )
			{
				Indexer::Type inKey = Indexer()(data);
				size_t hashVal = Hasher()( inKey );
				size_t iBucket = hashVal%m_Bucket.size();

				Bucket& bucket = m_Bucket[iBucket];
				TicketScopeLockT<TicketLockType> scopeLock( TicketLock::LOCK_EXCLUSIVE, bucket.m_Lock );
				//_ReadBarrier();
				std::atomic_thread_fence(std::memory_order_consume);

				MapItemType MapData;
				MapData.Key = inKey;
				MapData.Data = data;

				__if_exists(Trait::UniqueKey)
				{
					BucketContainer::iterator iter = bucket.m_Items.begin();
					for( ; iter != bucket.m_Items.end(); ++iter )
					{
						Indexer::Type curIdx = iter->Key;
						if( equal_to<Indexer::Type>()(inKey, curIdx) )
						{
							return E_FAIL;
						}
					}

					bucket.m_Items.push_back( MapData );
				}
				__if_not_exists(Trait::UniqueKey)
				{
					BucketContainer::iterator iter = bucket.m_Items.begin();
					bool bIsInserted = false;
					for( ; iter != bucket.m_Items.end(); ++iter )
					{
						Indexer::Type curKey = iter->Key;
						if( equal_to<Indexer::Type>()(inKey, curKey) )
						{
							bucket.m_Items.insert( iter, MapData );
							bIsInserted = true;
							break;
						}
					}
					if( !bIsInserted )
					{
						bucket.m_Items.push_back( MapData );
					}
				}

				m_lItemCount.fetch_add(1,std::memory_order_relaxed);

				//_WriteBarrier();
				std::atomic_thread_fence(std::memory_order_seq_cst);

#ifdef _DEBUG
				Assert( bucket.Validate(iBucket, m_Bucket.size()) );
#endif
				return S_OK;
			}

			HRESULT find( const typename Indexer::Type& keyVal, ItemType &data )
			{
				size_t hashVal = Hasher()( keyVal );
				size_t iBucket = hashVal%m_Bucket.size();

				Bucket& bucket = m_Bucket[iBucket];
				TicketScopeLockT<TicketLockType> scopeLock( TicketLock::LOCK_NONEXCLUSIVE, bucket.m_Lock );

				BucketContainer::iterator iter = bucket.m_Items.begin();
				for( ; iter != bucket.m_Items.end(); ++iter )
				{
					if( equal_to<Indexer::Type>()( keyVal, iter->Key ) )
					{
						data = iter->Data;
						return S_OK;
					}
				}

				return E_FAIL;
			}

			HRESULT find( const typename Indexer::Type& keyVal, iterator &iterData )
			{
				size_t hashVal = Hasher()( keyVal );
				size_t iBucket = hashVal%m_Bucket.size();

				Bucket& bucket = m_Bucket[iBucket];

				iterData = end();
				bucket.ReadLock();

				BucketContainer::iterator iter = bucket.m_Items.begin();
				for( INT iIdx = 0; iter != bucket.m_Items.end(); ++iter, ++iIdx )
				{
					if( equal_to<Indexer::Type>()( keyVal, iter->Key/*Indexer()(iter->Data)*/ ) )
					{
						iterData.Set( this, m_Bucket.begin() + iBucket, iIdx, false );
						return S_OK;
					}
				}

				bucket.ReadUnlock();
				return E_FAIL;
			}

			// Erase a data from hash map
			HRESULT erase(const ItemType &data)
			{
				if( m_Bucket.size() == 0 )
					return S_FALSE;

				Indexer::Type inKey = Indexer()(data);
				size_t hashVal = Hasher()( inKey );
				size_t iBucket = hashVal%m_Bucket.size();

				Bucket& bucket = m_Bucket[iBucket];

				bucket.WriteLock();
				//_ReadBarrier();
				std::atomic_thread_fence(std::memory_order_consume);

				BucketContainer::iterator iter = bucket.m_Items.begin();
				for( ; iter != bucket.m_Items.end(); ++iter )
				{
					Indexer::Type curIdx = iter->Key;
					if( equal_to<Indexer::Type>()(inKey, curIdx) )
					{
						bucket.m_Items.erase( iter );
						m_lItemCount.fetch_sub(1,std::memory_order_relaxed);
						//_WriteBarrier();
						std::atomic_thread_fence(std::memory_order_seq_cst);
						bucket.WriteUnlock();
						return S_OK;
					}
				}

				Assert( bucket.Validate(iBucket, m_Bucket.size()) );
				bucket.WriteUnlock();

				return E_FAIL;
			}

			HRESULT eraseByKey(const KeyType &key)
			{
				if (m_Bucket.size() == 0)
					return S_FALSE;

				Indexer::Type inKey = key;// Indexer()(data);
				size_t hashVal = Hasher()(inKey);
				size_t iBucket = hashVal%m_Bucket.size();

				Bucket& bucket = m_Bucket[iBucket];

				bucket.WriteLock();
				//_ReadBarrier();
				std::atomic_thread_fence(std::memory_order_consume);

				BucketContainer::iterator iter = bucket.m_Items.begin();
				for (; iter != bucket.m_Items.end(); ++iter)
				{
					Indexer::Type curIdx = iter->Key;
					if (equal_to<Indexer::Type>()(inKey, curIdx))
					{
						bucket.m_Items.erase(iter);
						m_lItemCount.fetch_sub(1, std::memory_order_relaxed);
						//_WriteBarrier();
						std::atomic_thread_fence(std::memory_order_seq_cst);
						bucket.WriteUnlock();
						return S_OK;
					}
				}

				Assert(bucket.Validate(iBucket, m_Bucket.size()));
				bucket.WriteUnlock();

				return E_FAIL;
			}

			HRESULT erase( iterator &iterData )
			{
				Indexer::Type Key;
				if( iterData.m_pContainer != this )
					return E_FAIL;

				if( iterData.m_iterBucket == bucket_end() )
					return E_INVALIDARG;

				if( iterData.m_iIdx <= iterator::END_IDX )
					return E_INVALIDARG;

				ItemType data = *iterData;
				INT iIdx = iterData.m_iIdx;

				// NOTE : if bucket size changed then this operation will not safe
				BucketListType::iterator iterBucket = iterData.m_iterBucket;


				Key = Indexer()(data);
#ifdef _DEBUG
				size_t iBucket = &(*iterData.m_iterBucket) - &m_Bucket[0];
				size_t hashVal1 = Hasher()( Key );
				size_t iBucketTem = hashVal1%m_Bucket.size();
				Assert( iBucket == iBucketTem );
#endif

				iterData = end();

				iterBucket->WriteLock();
				//_ReadBarrier();
				std::atomic_thread_fence(std::memory_order_consume);

				//bool bNotFound = true;
				if( iIdx >= (INT)iterBucket->m_Items.size()
					|| iterBucket->m_Items[iIdx].Key != Key)
				{
					// data not found. maybe erased?
					iterBucket->WriteUnlock();
					return E_FAIL;
				}
				BucketContainer::iterator iterBucketData = iterBucket->m_Items.begin() + iIdx;
#ifdef _DEBUG
				Indexer::Type idx = iterBucketData->Key;//Indexer()(*iterBucketData);
				size_t hashVal2 = Hasher()( idx );
				size_t iBucketTem2 = hashVal2%m_Bucket.size();
				Assert(iBucket == iBucketTem2);
#endif
				//There is a narrow window before iterBucket is locked a new data is erased and added again. so it could be same data or not
				//if(data != datTempBackup)
				iterBucket->m_Items.erase(iterBucketData);

				m_lItemCount.fetch_sub(1, std::memory_order_relaxed);

				//_WriteBarrier();
				std::atomic_thread_fence(std::memory_order_seq_cst);
#ifdef _DEBUG
				Assert( iterBucket->Validate(iBucket, m_Bucket.size()) );
#endif
				iterBucket->WriteUnlock();

				return S_OK;
			}

			HRESULT clear()
			{
				m_lItemCount = 0;

				BucketListType::iterator iterBucket = m_Bucket.begin();
				for( ; iterBucket != m_Bucket.end(); ++iterBucket )
				{
					iterBucket->m_Items.clear();
				}

				return S_OK;
			}

			bool Validate()
			{
#ifdef _DEBUG
				BucketListType::iterator iterBucket = m_Bucket.begin();
				for( INT iBucket = 0; iterBucket != m_Bucket.end(); ++iterBucket, ++iBucket )
				{
					if( !iterBucket->Validate( iBucket, m_Bucket.size() ) )
						return false;
				}
#endif
				return true;
			}
		};


}; // namespace Hash
}; // namespace BR




