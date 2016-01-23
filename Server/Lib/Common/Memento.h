////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Binary stream
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/Trace.h"
#include "Common/StaticLinkedList.h"
#include "Common/Memory.h"
#include "Common/Synchronization.h"

namespace BR {

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Memento
	//

	template<size_t BufferSize = 1024>
	class Memento
	{
	public:

		typedef StaticLinkedList<ThreadSyncTraitNone> LinkedList;

		struct SaveData : public LinkedList::Node
		{
			void* pSavedDataPtr;
			void* pOrgDataPtr;
			size_t DataSize;
		};

	private:
		// saved data list
		LinkedList m_LinkedList;

		// Memory allocator for save data
		StaticAllocator<BufferSize> m_StaticAllocator;

	public:
		// Constructor
		Memento()
			:m_StaticAllocator(STDAllocator::GetInstance())
		{
		}

		// Save data to memento
		template<class DataType>
		HRESULT Add( const DataType &data )
		{
			HRESULT hr = S_SYSTEM_OK;
			void *pPtr = nullptr;
			SaveData *pNewData = nullptr;

			size_t uiSize = sizeof(DataType) + sizeof(SaveData);
			trcChk( m_StaticAllocator.Alloc( uiSize, pPtr ) );

			pNewData = (SaveData*)pPtr;
			memset(pNewData,0, uiSize);
			pNewData->pOrgDataPtr = (void*)&data;
			pNewData->pSavedDataPtr = (void*)(pNewData+1);
			pNewData->DataSize = sizeof(DataType);

			memcpy(pNewData->pSavedDataPtr, pNewData->pOrgDataPtr, sizeof(DataType));

			trcChk(m_LinkedList.Add(pNewData));

		Proc_End:

			return hr;
		}

		// Restore All saved data
		HRESULT RestoreAllData()
		{
			for(auto itList = m_LinkedList.begin(); itList.IsValid(); ++itList)
			{
				auto pSavedData = (SaveData*)&(*itList);
				memcpy(pSavedData->pOrgDataPtr, pSavedData->pSavedDataPtr, pSavedData->DataSize);
			}

			return S_SYSTEM_OK;
		}
	};


}; // namespace BR



