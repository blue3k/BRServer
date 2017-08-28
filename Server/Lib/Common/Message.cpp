////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Message
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/Trace.h"
#include "Common/Message.h"
#include "Common/BrMemory.h"
#include "Common/MemoryPool.h"
#include "Common/ResultCode/BRResultCodeNet.h"
#include "Common/BrSvrTypes.h"

#if __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif


namespace BR {
namespace Message {
	

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network Message wrapper class
	//
	
	MessageData::MessageData( UINT uiMsgBufSize, const BYTE* pData )
		:m_bIsSequenceAssigned(false)
	{
		m_pMsgHeader = (MessageHeader*)(this+1);

		if( pData )
		{
			memcpy( m_pMsgHeader, pData, uiMsgBufSize );
			AssertRel( m_pMsgHeader->Length == uiMsgBufSize );
		}
		else
		{
			memset( GetMessageHeader(), 0, sizeof(Message::MessageHeader) );
			// Make sure the sequence is cleared
			Assert(GetMessageHeader()->msgID.IDSeq.Sequence == 0);
		}
	}

	MessageData::~MessageData()
	{
	}
#if __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif
	void MessageData::AssignSequence( UINT sequence )
	{
		// sequence must not assigned twice
		Assert(m_bIsSequenceAssigned == false);
		m_bIsSequenceAssigned = true;

		GetMessageHeader()->msgID.IDSeq.Sequence = sequence;
	}

	void MessageData::GetRouteInfo(RouteContext& routeContext, TransactionID& transID)
	{
		UINT length = 0;
		BYTE* pDataPtr = nullptr;
		GetLengthNDataPtr(length, pDataPtr);

		if (length < sizeof(RouteContext))
		{
			routeContext = 0;
			transID = TransactionID();
			return;
		}

		auto pRouteContext = (RouteContext*)GetMessageData();
		routeContext = *pRouteContext;


		if (length < (sizeof(RouteContext)+sizeof(TransactionID)))
		{
			transID = TransactionID();
			return;
		}

		transID = *(TransactionID*)(pRouteContext+1);
	}


	void MessageData::ClearAssignedSequence()
	{
		// sequence must not assigned twice
		m_bIsSequenceAssigned = false;

		GetMessageHeader()->msgID.IDSeq.Sequence = 0;
	}

	// Initialize message buffer
	MessageData* MessageData::NewMessage( UINT32 uiMsgID, UINT uiMsgBufSize, const BYTE* pData )
	{
		BYTE *pBuffer = nullptr;
		size_t szAllocate = sizeof(MessageData) + uiMsgBufSize;

		if (uiMsgBufSize > MAX_MESSAGE_SIZE)
		{
			defTrace(Trace::TRC_ERROR, "Too big message allocation request, over MAX_MESSAGE_SIZE, {0} is requested", uiMsgBufSize);
			return nullptr;
		}

		if (szAllocate > USHRT_MAX)
		{
			defTrace(Trace::TRC_ERROR, "Too big message allocation request, over 64KB, {0} is requested", szAllocate);
			return nullptr;
		}

		MemoryPool *pMemPool = MemoryPoolManager::GetMemoryPoolBySize(szAllocate);
		void *pPtr = nullptr;
		if (pMemPool == nullptr)
		{
			pPtr = new uint8_t[szAllocate];
		}
		else
		{
			if (!(pMemPool->Alloc(pPtr, "MessageData::NewMessage")))
				return nullptr;
		}

		pBuffer = (BYTE*)pPtr;
		if( pBuffer == nullptr )
			return nullptr;

		MessageData *pMsg = new(pBuffer) MessageData( uiMsgBufSize, pData );

		pMsg->GetMessageHeader()->msgID.ID = uiMsgID;
		pMsg->GetMessageHeader()->Length = uiMsgBufSize;


		// Increase one for message
		pMsg->AddRef();

		return pMsg;
	}


	Message::MessageData* MessageData::Clone()
	{
		if( m_pMsgHeader == nullptr || m_pMsgHeader->Length == 0 )
			return nullptr;

		Message::MessageData* pMessage = NewMessage( m_pMsgHeader->msgID.ID, m_pMsgHeader->Length, (BYTE*)m_pMsgHeader );
		pMessage->GetMessageHeader()->msgID.IDSeq.Sequence = 0;

		return pMessage;
	}

	void MessageData::GetLengthNDataPtr( UINT& length, BYTE* &pDataPtr)
	{
		if( m_pMsgHeader->msgID.IDs.Mobile )
		{
			AssertRel(m_pMsgHeader->Length >= sizeof(MobileMessageHeader));
			length = (uint)(m_pMsgHeader->Length - sizeof(MobileMessageHeader));
			pDataPtr = (BYTE*)(m_pMobileMsgHeader + 1);
		}
		else
		{
			AssertRel(m_pMsgHeader->Length >= sizeof(MessageHeader));
			length = (uint)(m_pMsgHeader->Length - sizeof(MessageHeader));
			pDataPtr = (BYTE*)(m_pMsgHeader + 1);
		}
	}
	
	UINT MessageData::GetDataLength()
	{
		UINT length;
		if( m_pMsgHeader->msgID.IDs.Mobile )
		{
			AssertRel(m_pMsgHeader->Length >= sizeof(MobileMessageHeader));
			length = (uint)(m_pMsgHeader->Length - sizeof(MobileMessageHeader));
		}
		else
		{
			AssertRel(m_pMsgHeader->Length >= sizeof(MessageHeader));
			length = (uint)(m_pMsgHeader->Length - sizeof(MessageHeader));
		}
		return length;
	}


	// Update checksume
	void MessageData::UpdateChecksum()
	{
		UINT length = 0;
		BYTE* pDataPtr = nullptr;
		GetLengthNDataPtr(length,pDataPtr);

		if( m_pMsgHeader == nullptr || m_pMsgHeader->Length == 0 )
		{
			Assert(0);
			return;
		}

		if( length == 0 )
		{
			m_pMsgHeader->Crc32 = 0;
			return;
		}

		m_pMsgHeader->Crc32 = Util::Crc32( 0, length, pDataPtr );
		if( m_pMsgHeader->Crc32 == 0 )
			m_pMsgHeader->Crc32 = ~m_pMsgHeader->Crc32;
	}
	
	// Update checksume
	void MessageData::UpdateChecksumNEncrypt()
	{
		UINT length = 0;
		BYTE* pDataPtr = nullptr;
		GetLengthNDataPtr(length,pDataPtr);

		if( m_pMsgHeader == nullptr || m_pMsgHeader->Length == 0 )
		{
			Assert(0);
			return;
		}

		if( length == 0 )
		{
			m_pMsgHeader->Crc32 = 0;
			return;
		}

		if( m_pMsgHeader->msgID.IDs.Encrypted )
		{
			// skip if the message is already encrypted
			return;
		}

		m_pMsgHeader->Crc32 = Util::Crc32NEncrypt( length, pDataPtr );
		if( m_pMsgHeader->Crc32 == 0 )
			m_pMsgHeader->Crc32 = ~m_pMsgHeader->Crc32;

		Assert( m_pMsgHeader->Crc32 != 0 || length == 0 );

		m_pMsgHeader->msgID.IDs.Encrypted = true;
	}

	Result MessageData::ValidateChecksum()
	{
		UINT length = 0;
		BYTE* pDataPtr = nullptr;
		GetLengthNDataPtr(length,pDataPtr);

		if( m_pMsgHeader == nullptr || m_pMsgHeader->Length == 0 )
		{
			Assert(0);
			return ResultCode::FAIL;
		}

		// Nothing to check
		if( length == 0 )
			return ResultCode::SUCCESS;

		UINT16 Crc32 = Util::Crc32( length, pDataPtr );
		if( Crc32 == 0 ) Crc32 = ~Crc32;

		if( Crc32 != m_pMsgHeader->Crc32 )
			return ResultCode::E_NET_INVALID_MESSAGE_CHECKSUM;

		return ResultCode::SUCCESS;
	}
	
	Result MessageData::ValidateChecksumNDecrypt()
	{
		UINT length = 0;
		BYTE* pDataPtr = nullptr;
		GetLengthNDataPtr(length,pDataPtr);

		if( m_pMsgHeader == nullptr || m_pMsgHeader->Length == 0 )
		{
			m_pMsgHeader->msgID.IDs.Encrypted = false;
			return ResultCode::SUCCESS_FALSE;
		}

		if( !m_pMsgHeader->msgID.IDs.Encrypted )
		{
			return ValidateChecksum();
		}
		
		// Nothing to check
		if( length == 0 )
		{
			m_pMsgHeader->msgID.IDs.Encrypted = false;
			return ResultCode::SUCCESS;
		}

		UINT16 Crc32 = Util::Crc32NDecrypt( length, pDataPtr );
		if( Crc32 == 0 ) Crc32 = ~Crc32;

		m_pMsgHeader->msgID.IDs.Encrypted = false;

		//Assert(m_pMsgHeader->Crc32 != 0);
		if( Crc32 != m_pMsgHeader->Crc32 )
			return ResultCode::E_NET_INVALID_MESSAGE_CHECKSUM;

		return ResultCode::SUCCESS;
	}


	// From ISharedObj
	void MessageData::DeleteThis() const
	{
		size_t szAllocate = sizeof(MessageData) + GetMessageSize();
		this->~MessageData();

		MemoryPool *pMemPool = MemoryPoolManager::GetMemoryPoolBySize(szAllocate);
		if (pMemPool == nullptr)
		{
			delete[] (uint8_t*)this;
		}
		else
		{
			pMemPool->Free((void*)this, "MessageData::DeleteThis");
		}

	}




	Result MessageBase::ParseMsg()
	{ 
		if(m_bIsParsed)
			return m_hrParsing;

		m_bIsParsed = true;
		m_hrParsing = ParseMessage(GetMessage());

		return m_hrParsing;
	}

} // Message
} // BR

#if __GNUC__
#pragma GCC diagnostic pop
#endif
