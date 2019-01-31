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

#include "SFTypedefs.h"
#include "ServerLog/SvrLog.h"
#include "Container/SFStaticLinkedList.h"
#include "Memory/SFMemory.h"
#include "Thread/SFSynchronization.h"

namespace SF {

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
		StaticArray<uint8_t,BufferSize> m_StaticBuffer;

	public:
		// Constructor
		Memento(IHeap& memMgr)
			: m_StaticBuffer(memMgr)
		{
		}

		// Save data to memento
		template<class DataType>
		Result Add( const DataType &data )
		{
			Result hr = ResultCode::SUCCESS;
			void *pPtr = nullptr;
			SaveData *pNewData = nullptr;

			size_t uiSize = sizeof(DataType) + sizeof(SaveData);
			trcChk(m_StaticBuffer.reserve( uiSize ) );

			pNewData = (SaveData*)m_StaticBuffer.data();
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
		Result RestoreAllData()
		{
			for(auto itList = m_LinkedList.begin(); itList.IsValid(); ++itList)
			{
				auto pSavedData = (SaveData*)&(*itList);
				memcpy(pSavedData->pOrgDataPtr, pSavedData->pSavedDataPtr, pSavedData->DataSize);
			}

			return ResultCode::SUCCESS;
		}
	};


}; // namespace BR



