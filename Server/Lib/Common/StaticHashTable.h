////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : static Thread safe hash table
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/BrAssert.h"
#include "Common/Indexing.h"
#include "Common/Synchronization.h"
#include "Common/SynchronizationTrait.h"
#include "Common/ArrayUtil.h"
#include "Common/HashTableTrait.h"
#include "Common/OrderedLinkedList.h"


namespace BR {
namespace Hash {


		/////////////////////////////////////////////////////////////////////////////////
		//
		//	Thread safe hash map
		//		- This class uses static ordered linked list to store mapped items so that we don't need to allocate any additional memory
		//			However, it's a bit labor intensive when you use it.
		//

		template<	typename ItemType, 
					typename Indexer, 
					typename MapItemConverter, 
					typename Trait = UniqueKeyTrait, 
					typename ThreadTrait = ThreadSyncTraitReadWriteT<Indexer::Type, ItemType*>,
					size_t	DefaultBucketSize = 128,
					typename Hasher = Hash::hash<typename Indexer::Type> >
		class StaticHashTable
		{
		public:

			typedef typename ThreadTrait::TicketLockType		TicketLockType;
			typedef typename ItemType							ItemType;
			typedef typename Indexer							Indexer;
			typedef typename MapItemConverter					MapItemConverter;
			typedef typename MapItemConverter::Type				MapItemType;
			typedef OrderedLinkedList<typename Indexer::Type>	BucketContainer;

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

				friend StaticHashTable;


			public:
				// Bucket container
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
					if( ThreadTrait::ThreadSafe )
					{
					Assert( !src.m_Lock.IsLocked() );
					}
				}

				Bucket( Bucket&& src )
					:m_Items(src.m_Items)
				{
					if( ThreadTrait::ThreadSafe )
					{
					// No one use this bucket, while this operation
					Assert( !src.m_Lock.IsLocked() );
					}
				}

				// Destructor
				~Bucket()
				{
				}

				Bucket& operator = ( const Bucket& src )
				{
					// No one use this bucket, while this operation
					if( ThreadTrait::ThreadSafe )
					{
					Assert( !src.m_Lock.IsLocked() );
					Assert( !m_Lock.IsLocked() );
					}

					m_Items = src.m_Items;
					return *this;
				}

				Bucket& operator = ( Bucket&& src )
				{
					if( ThreadTrait::ThreadSafe )
					{
					// No one use this bucket, while this operation
					Assert( !src.m_Lock.IsLocked() );
					Assert( !m_Lock.IsLocked() );
					}

					m_Items = src.m_Items;
					return *this;
				}

				// validate bucket id
				bool Validate( size_t iMyBucket, size_t szNumBucket )
				{
					// Validate only debug mode
#ifdef _DEBUG
					//_ReadBarrier();
					std::atomic_thread_fence(std::memory_order_consume);
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
			typedef StaticArray<Bucket,DefaultBucketSize> BucketListType;


			///////////////////////////////////////////
			// Hash table iterator
			class iterator
			{
			private:
				// Is Bucket internal iterator?, then only iterate inside bucket
				bool								m_bIsIterInBucket;
				// Bucket list iterator. directing which bucket
				typename BucketListType::iterator	m_iterBucket;
				// Main container pointer
				StaticHashTable*					m_pContainer;
				// iterator in bucket
				typename BucketContainer::iterator	m_itInBucket;
				// MapItemType
				mutable MapItemType					*m_pCache;

				// constructor
				iterator( StaticHashTable *pContainer, typename BucketListType::iterator iterBucket, typename BucketContainer::iterator itInBucket, bool bIsBucketIter = false )
					:m_bIsIterInBucket(bIsBucketIter),
					m_pContainer(pContainer),
					m_iterBucket(iterBucket),
					m_itInBucket(itInBucket)
				{
					m_pCache = NULL;
					if( m_pContainer && m_iterBucket != m_pContainer->bucket_end() && m_itInBucket.IsValid() )
						m_iterBucket->ReadLock();
				}

				friend class StaticHashTable;


				void NextIter()
				{
					Assert(m_pContainer != NULL);
					Assert( m_iterBucket != m_pContainer->bucket_end() );
					Assert( m_itInBucket.IsValid() );
					++m_itInBucket;
					if( m_itInBucket.IsValid() )
						return;

					if( m_bIsIterInBucket )
					{
						m_iterBucket = m_pContainer->bucket_end();
						m_itInBucket = nullptr;
						return;
					}

					m_iterBucket->ReadUnlock();
					++m_iterBucket;
					while( m_iterBucket != m_pContainer->bucket_end() )
					{
						if( m_iterBucket->m_Items.size() != 0 )
						{
							m_iterBucket->ReadLock();
							if( m_iterBucket->m_Items.size() != 0 )
							{
								m_itInBucket = m_iterBucket->m_Items.begin();
								return;
							}
							// failed search again
							m_iterBucket->ReadUnlock();
						}
						++m_iterBucket;
					}

					m_itInBucket = nullptr;
				}

				// set iter
				void Set( StaticHashTable *pContainer, typename BucketListType::iterator iterBucket )//, typename BucketContainer::iterator itInBucket, bool bIsLock = true, bool bIsBucketIter = false )
				{
					if( m_pContainer && m_iterBucket != m_pContainer->bucket_end() && m_itInBucket.IsValid() )
						m_iterBucket->ReadUnlock();

					m_bIsIterInBucket = false;
					m_pContainer = pContainer;
					m_iterBucket = iterBucket;
					m_itInBucket = nullptr;//itInBucket;

					//if( bIsLock && m_pContainer && m_iterBucket != m_pContainer->bucket_end() && m_itInBucket.IsValid() )
					if( m_pContainer && m_iterBucket != m_pContainer->bucket_end() )
						m_iterBucket->ReadLock();
				}

				void SetInBucketIter( typename BucketContainer::iterator itInBucket )
				{
					Assert(m_itInBucket == nullptr);
					m_itInBucket = itInBucket;//itInBucket;
				}

			public:
				iterator()
					:m_bIsIterInBucket(false),
					m_pContainer(NULL)
				{
				}

				iterator( const iterator& src )
					:m_bIsIterInBucket(false),
					m_pContainer(src.m_pContainer),
					m_iterBucket(src.m_iterBucket),
					m_itInBucket(src.m_itInBucket)
				{
					if( m_pContainer && m_iterBucket != m_pContainer->bucket_end() )
					{
						m_iterBucket->ReadLock();
					}
					//Assert( m_pContainer == nullptr || m_iterBucket == m_pContainer->bucket_end() );
				}

				~iterator()
				{
					if( m_pContainer && m_iterBucket != m_pContainer->bucket_end() )
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

				ItemType* operator* ()
				{
					Assert(m_pContainer != NULL);
					m_pCache = &*m_itInBucket;
					AssertRel(m_pCache);
					return MapItemConverter()(m_pCache);
				}

				const ItemType* operator* () const
				{
					Assert(m_pContainer != NULL);
					m_pCache = &*m_itInBucket;
					AssertRel(m_pCache);
					return MapItemConverter()(m_pCache);
				}

				ItemType* operator-> ()
				{
					Assert(m_pContainer != NULL);
					m_pCache = &*m_itInBucket;
					AssertRel(m_pCache);
					return MapItemConverter()(m_pCache);
				}

				const ItemType* operator-> () const
				{
					Assert(m_pContainer != NULL);
					m_pCache = &*m_itInBucket;
					AssertRel(m_pCache);
					return MapItemConverter()(m_pCache);
				}

				bool operator !=( const iterator& op ) const
				{
					return ( (m_pContainer != op.m_pContainer) || (m_iterBucket != op.m_iterBucket) || (m_itInBucket != op.m_itInBucket) );
				}

				bool operator ==( const iterator& op ) const
				{
					return ( (m_pContainer == op.m_pContainer) && (m_iterBucket == op.m_iterBucket) && (m_itInBucket == op.m_itInBucket) );
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
						m_iterBucket->ReadUnlock();

					m_pContainer = op.m_pContainer;
					m_iterBucket = op.m_iterBucket;
					m_itInBucket = op.m_itInBucket;

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
					m_itInBucket = nullptr;

					return *this;
				}
			};

		private:
			// bucket
			BucketListType m_Bucket;

			// total count of item
			LONG		m_lItemCount;

		public:

			StaticHashTable( INT iBucketCount = 16 )
				:m_lItemCount(0)
			{
				m_Bucket.resize( iBucketCount );
			}

			virtual ~StaticHashTable()
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
				auto iterBucket = m_Bucket.begin();
				for( ; iterBucket != m_Bucket.end(); ++iterBucket )
				{
					if( !iterBucket->m_Items.empty() )
					{
						return iterator(this, iterBucket, iterBucket->m_Items.begin());
					}
				}

				return end();
			}

			iterator end()
			{
				return iterator(this,m_Bucket.end(),nullptr);
			}

			size_t size()
			{
				return m_lItemCount;
			}

			//////////////////////////////////////////////////////////////////////////
			//
			//	Insert/erase/clear
			//

			HRESULT insert( const ItemType* data )
			{
				Indexer::Type inKey = Indexer()(data);
				MapItemConverter::Type *inItem = MapItemConverter()(data);
				size_t hashVal = Hasher()( inKey );
				size_t iBucket = hashVal%m_Bucket.size();

				Bucket& bucket = m_Bucket[iBucket];
				TicketScopeLockT<TicketLockType> scopeLock( TicketLock::LockMode::LOCK_EXCLUSIVE, bucket.m_Lock );

				//_ReadBarrier();
				std::atomic_thread_fence(std::memory_order_consume);

				typename BucketContainer::Node *pPrevNode = nullptr;

				if( FAILED(bucket.m_Items.FindPrevNode( inKey, pPrevNode )) )
					return E_FAIL;

				if(Trait::UniqueKey)
				{
					if( pPrevNode->pNext && pPrevNode->pNext->Key == inKey )
					{
						return E_FAIL;
					}
				}

				bucket.m_Items.Insert( pPrevNode, inKey, inItem );

				m_lItemCount++;
				//_WriteBarrier();
				std::atomic_thread_fence(std::memory_order_seq_cst);

#ifdef _DEBUG
				Assert( bucket.Validate(iBucket, m_Bucket.size()) );
#endif
				return S_OK;
			}

			HRESULT find( const typename Indexer::Type& keyVal, ItemType *data )
			{
				size_t hashVal = Hasher()( keyVal );
				size_t iBucket = hashVal%m_Bucket.size();

				Bucket& bucket = m_Bucket[iBucket];
				TicketScopeLockT<TicketLockType> scopeLock( TicketLock::LockMode::LOCK_NONEXCLUSIVE, bucket.m_Lock );

				typename BucketContainer::Node *pPrevNode = nullptr;
				if( FAILED(bucket.m_Items.FindPrevNode( inKey, pPrevNode )) )
					return E_FAIL;

				if( pPrevNode->pNext == nullptr || pPrevNode->pNext->Key != inKey )
					return E_FAIL;

				data = MapItemConverter()(pPrevNode->pNext);

				return E_FAIL;
			}

			HRESULT find( const typename Indexer::Type& keyVal, iterator &iterData )
			{
				size_t hashVal = Hasher()( keyVal );
				size_t iBucket = hashVal%m_Bucket.size();

				Bucket& bucket = m_Bucket[iBucket];

				iterData = end();

				// Set operation will lock the bucket
				iterData.Set( this, m_Bucket.begin() + iBucket );

				typename BucketContainer::Node *pPrevNode = nullptr;
				if( FAILED(bucket.m_Items.FindPrevNode( keyVal, pPrevNode )) )
				{
					iterData = nullptr;
					return E_FAIL;
				}

				if( pPrevNode->pNext == nullptr || pPrevNode->pNext->Key != keyVal )
				{
					iterData = nullptr;
					return E_FAIL;
				}

				iterData.SetInBucketIter( pPrevNode );

				return S_OK;
			}

			// Erase a data from hash map
			HRESULT erase( const ItemType *data )
			{
				Indexer::Type inKey = Indexer()(data);
				size_t hashVal = Hasher()( inKey );
				size_t iBucket = hashVal%m_Bucket.size();

				Bucket& bucket = m_Bucket[iBucket];
				TicketScopeLockT<TicketLockType> scopeLock( TicketLock::LockMode::LOCK_EXCLUSIVE, bucket.m_Lock );
				//_ReadBarrier();
				std::atomic_thread_fence(std::memory_order_consume);
				
				typename BucketContainer::Node *pPrevNode = nullptr;
				if( FAILED(bucket.m_Items.FindPrevNode( inKey, pPrevNode )) )
					return E_FAIL;

				if( pPrevNode->pNext == nullptr || pPrevNode->pNext->Key != inKey )
					return E_FAIL;

				bucket.m_Items.Remove( pPrevNode, pPrevNode->pNext );
				m_lItemCount--;
				//_WriteBarrier();
				std::atomic_thread_fence(std::memory_order_seq_cst);

				Assert( bucket.Validate(iBucket, m_Bucket.size()) );

				return E_FAIL;
			}

			HRESULT erase( iterator &iterData )
			{
				Indexer::Type Key;
				if( iterData.m_pContainer != this )
					return E_FAIL;

				if( iterData.m_iterBucket == bucket_end() )
					return E_INVALIDARG;

				if( !iterData.m_itInBucket.IsValid() )
					return E_INVALIDARG;

				ItemType* data = *iterData;

				// NOTE : if bucket size changed then this operation will not safe
				Bucket& bucket = *iterData.m_iterBucket;

				Key = Indexer()(data);
				iterData = end();

				TicketScopeLockT<TicketLockType> scopeLock( TicketLock::LockMode::LOCK_EXCLUSIVE, bucket.m_Lock );

				//_ReadBarrier();
				std::atomic_thread_fence(std::memory_order_consume);

				typename BucketContainer::Node *pPrevNode = nullptr;
				if( FAILED(bucket.m_Items.FindPrevNode( Key, pPrevNode )) )
					return E_FAIL;

				if( pPrevNode->pNext == nullptr || pPrevNode->pNext->Key != Key )
					return E_FAIL;

				bucket.m_Items.Remove( pPrevNode, pPrevNode->pNext );
				m_lItemCount--;
				//_WriteBarrier();
				std::atomic_thread_fence(std::memory_order_seq_cst);

				//Assert( bucket.Validate(iBucket, m_Bucket.size()) );

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




