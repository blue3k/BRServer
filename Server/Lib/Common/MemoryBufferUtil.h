////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Memory buffer Utility library
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/BrAssert.h"
#include "Common/Memory.h"



namespace BR
{
	// Implementation of vector with static memory buffer
	template< class DataType, size_t StaticBufferSize = 256 >
	class MemoryBuffer
	{
	private:
		StaticAllocator<StaticBufferSize> m_Allocator;

		DataType *m_DataBuffer;
		// currently allocated size
		size_t	m_Allocated;
		// Increase size when the allocated buffer is small
		size_t	m_Increase;
		// Used buffer size
		size_t	m_Used;

	public:

		MemoryBuffer( MemoryAllocator *pOverflowAllocator = nullptr )
			:m_Allocator(pOverflowAllocator == nullptr ? STDAllocator::GetInstance() : *pOverflowAllocator)
			,m_DataBuffer(nullptr)
			,m_Allocated(0)
			,m_Increase(128)
			,m_Used(0)
		{
		}

		const DataType* GetPtr() const
		{
			return m_Used ? m_DataBuffer : nullptr;
		}
		DataType* GetPtr()
		{
			return m_Used ? m_DataBuffer : nullptr;
		}

		// access operator
		const DataType& operator []( unsigned int index ) const
		{
			AssertRel( GetPtr() && index < m_Used );
			return m_DataBuffer[index];
		}
		DataType& operator []( unsigned int index )
		{
			AssertRel( GetPtr() && index < m_Used );
			return m_DataBuffer[index];
		}

		// Get capacity, allocated size in element
		size_t GetCapacity()
		{
			return m_Allocated;
		}

		// get data size in element
		size_t GetSize()
		{
			return GetUsedSize();
		}

		size_t GetUsedSize()
		{
			return m_Used;
		}

		// Clear 
		void Clear()
		{
			if( m_DataBuffer )
				m_Allocator.Free(m_DataBuffer);
			m_DataBuffer = nullptr;
			m_Allocated = 0;
			m_Used = 0;
		}

		virtual Result Resize( size_t newSize )
		{
			void* pPtr = m_DataBuffer;
			if( FAILED(m_Allocator.Realloc( sizeof(DataType)*newSize, pPtr )) )
				return ResultCode::OUT_OF_MEMORY;

			if( pPtr != m_DataBuffer )
			{
				memcpy( pPtr, m_DataBuffer, sizeof(DataType)*m_Used );
				m_Allocator.Free(m_DataBuffer);
			}

			m_Allocated = newSize;
			m_DataBuffer = (DataType*)pPtr;
			return ResultCode::SUCCESS;
		}

		// Append data
		virtual Result Append( size_t dataSize, const DataType* data )
		{
			if( data == nullptr || dataSize == 0 )
				return ResultCode::INVALID_ARG;

			// Increase size
			if( m_Allocated < (m_Used+dataSize) )
			{
				size_t newSize = m_Allocated + ((dataSize + m_Increase - 1) / m_Increase) * m_Increase;

				Result hr = Resize(newSize);
				if( FAILED(hr) )
					return hr;
			}

			memcpy( m_DataBuffer + m_Used, data, sizeof(DataType)*dataSize );
			m_Used += dataSize;

			return ResultCode::SUCCESS;
		}

		// remove data
		virtual Result Remove( const DataType* data )
		{
			if( data == nullptr )
				return ResultCode::INVALID_ARG;

			if( GetPtr() == nullptr )
				return ResultCode::FAIL;

			DataType* dataList = GetPtr();
			for( unsigned int index = 0; index < GetSize(); index++ )
			{
				if( dataList[index] == *data )
				{
					if( (index+1) < GetSize() )
					{
						memcpy( &dataList[index], &dataList[index+1], sizeof(DataType)*(GetSize() - (index+1)) );
					}
					m_Used--;
					return ResultCode::SUCCESS;
				}
			}

			return ResultCode::FAIL;
		}

		virtual Result RemoveAt( int removeIndex )
		{
			if( removeIndex < 0 || removeIndex >= (INT)GetSize() )
				return ResultCode::INVALID_ARG;

			if( GetPtr() == nullptr )
				return ResultCode::FAIL;

			DataType* dataList = GetPtr();
			if( (size_t)(removeIndex+1) < GetSize() )
			{
				memcpy( &dataList[removeIndex], &dataList[removeIndex+1], sizeof(DataType)*(GetSize() - (removeIndex+1)) );
			}
			m_Used--;

			return ResultCode::SUCCESS;
		}
	};


	// string buffer
	template<size_t StaticBufferSize = 256 >
	class StringBuffer : public MemoryBuffer<char, StaticBufferSize>
	{
	public:
		typedef MemoryBuffer<char, StaticBufferSize> super;

		virtual Result Append( size_t dataSize, const char* data )
		{
			if( data == nullptr || dataSize == 0 )
				return ResultCode::INVALID_ARG;

			Result hr = super::Append( dataSize, data );
			if( FAILED(hr) ) return hr;

			if(super::GetPtr()[super::GetUsedSize()-1] != '\0' )
			{
				hr = super::Append( 1, "" );
				if( FAILED(hr) ) return hr;
			}

			return ResultCode::SUCCESS;
		}
	};

//#include "MemoryBufferUtil.inl"


}; // namespace BR



