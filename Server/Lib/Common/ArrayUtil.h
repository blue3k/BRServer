////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Array Utility library
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "SFAssert.h"
#include "Common/ResultCode/BRResultCodeSystem.h"


namespace BR
{

#pragma pack(push)
#pragma pack(2)
	struct BinaryHeader
	{
		UINT16 Magic;		// Binary magic
		UINT16 Version;		// Binary Version
		UINT32 Count;		// Count
	};
#pragma pack(pop)

	// default binary version and magic
	template< class DataType >
	UINT BinaryMagic()
	{
		return 0;
	}

	template< class DataType >
	UINT BinaryVersion()
	{
		return 0;
	}



	////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	static buffer class
	//

	template< class DataType >
	class Array
	{
	public:
#pragma pack(push)
#pragma pack(4)

#if !defined(SWIG)
		// Binary Type
		struct BinaryType : public BinaryHeader
		{
			DataType pData[1];
		};
#endif

#pragma pack(pop)

#if !defined(SWIG)
		class iterator
		{
		public:
			enum { END_IDX = -1 };

		private:
			// Container that linked with this iterator
			Array<DataType> *m_pContainer;

			// Index 
			mutable UINT m_iIdx;

		public:

			iterator()
				:m_pContainer(nullptr),
				m_iIdx(END_IDX)
			{
			}

			iterator( Array<DataType> *pContainer, UINT index = 0 )
				:m_pContainer(pContainer),
				m_iIdx(index)
			{
				Assert(m_pContainer != nullptr);
			}

			iterator( const iterator& src )
				:m_pContainer(src.m_pContainer),
				m_iIdx(src.m_iIdx)
			{
				Assert(m_pContainer != nullptr);
			}

			~iterator()
			{
			}


			iterator& operator++()
			{
				if( m_pContainer )
				{
					m_iIdx++;
					if( m_iIdx >= m_pContainer->GetSize() )
					{
						m_iIdx = END_IDX;
					}
				}

				return *this;
			}

			const iterator& operator++() const
			{
				if( m_pContainer )
				{
					m_iIdx++;
					if( m_iIdx >= m_pContainer->GetSize() )
					{
						m_iIdx = END_IDX;
					}
				}

				return *this;
			}


			iterator operator + ( size_t szOffset )
			{
				Assert(m_pContainer);
				if( m_pContainer && (m_iIdx + szOffset) < m_pContainer->size() )
				{
					return iterator(m_pContainer,(UINT)(m_iIdx + szOffset));
				}
				return iterator(m_pContainer, -1);
			}


			DataType& operator* ()
			{
				Assert(m_pContainer != NULL);
				Assert(m_iIdx >= 0 && m_iIdx < m_pContainer->GetSize());
				return m_pContainer->data()[m_iIdx];
			}

			const DataType& operator* () const
			{
				Assert(m_pContainer != NULL);
				Assert(m_iIdx >= 0 && m_iIdx < m_pContainer->GetSize());
				return m_pContainer->data()[m_iIdx];
			}

			DataType* operator-> ()
			{
				Assert(m_pContainer != NULL);
				Assert(m_iIdx >= 0 && m_iIdx < m_pContainer->GetSize());
				return &m_pContainer->data()[m_iIdx];
			}

			const DataType* operator-> () const
			{
				Assert(m_pContainer != NULL);
				Assert(m_iIdx >= 0 && m_iIdx < m_pContainer->GetSize());
				return &m_pContainer->data()[m_iIdx];
			}

			bool operator !=( const iterator& op ) const
			{
				return ( (m_pContainer != op.m_pContainer) || (m_iIdx != op.m_iIdx) );
			}

			bool operator ==( const iterator& op ) const
			{
				return ( (m_pContainer == op.m_pContainer) &&  (m_iIdx == op.m_iIdx) );
			}

			// Check validity
			bool IsValid()
			{
				return m_pContainer && m_iIdx < m_pContainer->GetSize();
			}

			// reset iterator and make invalid
			void Reset()
			{
				*this = nullptr;
			}

			iterator& operator = ( const iterator& src )
			{
				m_pContainer = src.m_pContainer;
				m_iIdx = src.m_iIdx;
				return *this;
			}

			iterator& operator = ( const void* pPtr )
			{
				Assert( pPtr == 0 );

				m_pContainer = nullptr;
				m_iIdx = END_IDX;

				return *this;
			}
		};
#endif // #ifndef SWIG

	private:
		// Current size
		size_t m_Size;

		// Allocated Size
		size_t m_AllocatedSize;

		// Increase Size
		size_t m_IncreaseSize;

		// Data pointer
		DataType*	m_pDataPtr;

		bool m_ConserveDataOnResize;

	protected:
		// Update Data pointer
		void SetBuffPtr( size_t AllocatedSize, DataType *pDataPtr );

		// Called for reallocation
		virtual Result IncreaseSize();

	public:
		// Static buffer
		Array();
		virtual ~Array();

		// Get Size
		inline size_t GetSize() const;
		inline size_t size() const { return GetSize(); }

		// set Reserve size
		inline  Result SetSize( size_t szNewSize );
		void resize(size_t szNewSize) { SetSize(szNewSize); }

		// Clear array, buffer size not reallocated, buffer data didn't erased, clear manually if need
		inline void Clear();

		// ConserveDataOnResize
		inline bool GetConserveDataOnResize() const;
		inline void SetConserveDataOnResize(bool conserveDataOnResize);

		// Get Current allocated Size
		size_t GetAllocatedSize() const;

		// Get Array Increase Size
		size_t GetIncreaseSize() const;
		void SetIncreaseSize( size_t szNewIncSize );

		// set Reserve size
		virtual Result Reserve( size_t szReserv ) = 0;

#if !defined(SWIG)
		// Get data pointer
		inline const DataType* data() const;
#endif
		inline DataType* data();

		// Insert an item
		Result Insert(int index, const DataType& NewData);

		// push_back
		Result push_back( const DataType& NewData );
		Result Add(const DataType& NewData) { return push_back(NewData); }
		Result AddItems(size_t numItems, const DataType* NewData);
#if !defined(SWIG)
		Result push_back( DataType&& NewData );
#endif

		// Remove element
		inline Result RemoveAt( INT iIndex );
		inline Result RemoveItem( const DataType& RemoveData );

		// find and return index, -1 is not found
		inline INT FindItem( const DataType& FindData );

		// Element access operator
#if !defined(SWIG)
		inline DataType& operator []( size_t iElement ) const;
		inline DataType& operator []( size_t iElement );
#endif
		inline const DataType& GetAt(UINT iElement) const; // for swig


		// Get binary size
		//size_t GetBinSize() const;

#ifdef SWIG

		//%extend{
		//	DataType* ToArray()
		//	{
		//		return $self->get(index);
		//	}
		//}

#else

		iterator begin() { return iterator(this, GetSize() == 0 ? iterator::END_IDX : 0); }
		const iterator begin() const { return iterator(const_cast<Array<DataType>*>(this), GetSize() == 0 ? iterator::END_IDX : 0); }
		iterator end() { return iterator(this, iterator::END_IDX); }
		const iterator end() const { return iterator(const_cast<Array<DataType>*>(this), iterator::END_IDX); }

		// Foreach operator
		Result Foreach( std::function<Result(DataType&)> functor )
		{
			for( size_t iData = 0; iData < GetSize(); iData++ )
			{
				Result hr = functor( m_pDataPtr[iData] );
				if( !(hr) ) return hr;
			}
			return ResultCode::SUCCESS;
		}

		Result Foreach( std::function<Result(DataType&)> functor ) const
		{
			return const_cast<Array<DataType>*>(this)->Foreach( functor );
		}

		// Copy to Bin
		template< size_t szBinSize >
		Result CopyToBin( BYTE (&Dest)[szBinSize] ) const
		{
			Result hr = ResultCode::SUCCESS;
			BinaryType *pDst = (BinaryType*)Dest;

			static_assert( szBinSize >= sizeof(BinaryType), "Invalid dest binary size" );
			size_t DstMaxCount = (szBinSize - sizeof(BinaryHeader))/sizeof(DataType);
			size_t CopyCount = GetSize();
			if( DstMaxCount < GetSize() )
			{
				//defTrace( Trace::TRC_ERROR, "Error, Invalid Binary size, Droping Data, {0}", typeid(DataType).name() );
				hr = ResultCode::FAIL;

				CopyCount = DstMaxCount;
			}

			pDst->Magic		= BinaryMagic<DataType>();		// Binary magic
			pDst->Version	= BinaryVersion<DataType>();	// Binary Version
			pDst->Count		= (UINT32)CopyCount;		// Count

			memcpy( pDst->pData, m_pDataPtr, sizeof(DataType) * CopyCount );

			return hr;
		}
#endif
	};



	////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Static Array Variable class
	//

	template< class DataType, size_t DefaultBufferSize >
	class StaticArray : public Array<DataType>
	{
	private:
		// static storage for remove heap alloc
		DataType	m_pDefautlBuffer[DefaultBufferSize];

		// Data pointer
		DataType*	m_pAllocatedBuffer;

	public:
		StaticArray();
		virtual ~StaticArray();


		// set Reserve size
		virtual Result Reserve(size_t szReserv);
	};


	////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Dynamic Array class
	//

	template< class DataType >
	class DynamicArray : public Array<DataType>
	{
	private:
		// Data pointer
		DataType*	m_pAllocatedBuffer;

	public:
		DynamicArray(size_t increaseSize = 10);
		virtual ~DynamicArray();


		// set Reserve size
		virtual Result Reserve(size_t szReserv);
	};



	////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Linked Array class
	//

	template< class DataType >
	class LinkedArray : public Array<DataType>
	{
	private:
	public:
		LinkedArray();
		LinkedArray(UINT maxDataCount, UINT dataCount, DataType* pDataPtr);
		virtual ~LinkedArray();

		void SetLinkedBuffer(UINT maxDataCount, UINT dataCount, DataType* pDataPtr);
		void SetLinkedBuffer(const Array<DataType>& srcLink);

		Result Reserve(size_t szReserv) override;
	};


#include "ArrayUtil.inl"


}; // namespace BR



