////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Connection implementations
//	
//
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Common/Thread.h"
#include "Common/BrAssert.h"
#include "Common/TimeUtil.h"
#include "Common/HRESNet.h"
#include "Net/NetTrace.h"
#include "Net/ConnectionUDP.h"
#include "Net/NetDef.h"
#include "Net/NetServer.h"
#include "Net/NetCtrl.h"
#include "Net/NetConst.h"



namespace BR {
namespace Net {

	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Packet message Sorting Window class
	//

	// Constructor
	MsgWindow::MsgWindow( INT iWndSize )
	:	m_uiBaseSequence(0)
	, m_uiWndBaseIndex(0)
	, m_uiWndSize( iWndSize )
	, m_uiMsgCount( 0 )
	, m_pMsgWnd(nullptr)
	{
		m_pMsgWnd = new MessageElement[GetWindowSize()];
		memset( m_pMsgWnd, 0, sizeof(MessageElement)*GetWindowSize() );
	}

	MsgWindow::~MsgWindow()
	{
		ClearWindow();
		delete[] m_pMsgWnd;
	}


	// Get message info in window, index based on window base
	HRESULT MsgWindow::GetAt( UINT uiIdx, MsgWindow::MessageElement* &pMessageElement )
	{
		UINT iIdxCur = (uiIdx+m_uiWndBaseIndex)%GetWindowSize();

		if( m_pMsgWnd == NULL )
			return E_FAIL;

		pMessageElement = &m_pMsgWnd[iIdxCur];

		return S_OK;
	}


	// Clear window element
	void MsgWindow::ClearWindow()
	{
		Message::MessageData* data = NULL;
		HRESULT hr = S_OK;
		if( m_pMsgWnd )
		{
			for( INT iMsg = 0; iMsg < GetWindowSize(); iMsg++ )
			{
				Util::SafeRelease( m_pMsgWnd[iMsg].pMsg );
			}
			memset( m_pMsgWnd, 0, sizeof(MessageElement)*GetWindowSize() );
		}
		m_uiBaseSequence = 0;
		m_uiWndBaseIndex = 0;
		m_uiMsgCount = 0;
	}

	
	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Recv message window
	//

	RecvMsgWindow::RecvMsgWindow()
		: m_uiSyncMask(0)
		, m_uiBaseSequence(0)
		, m_uiMsgCount(0)
		, m_pMsgWnd(nullptr)
	{
		m_pMsgWnd = new std::atomic<MessageElement>[CIRCULAR_QUEUE_SIZE];
		memset(m_pMsgWnd, 0, sizeof(std::atomic<MessageElement>)*CIRCULAR_QUEUE_SIZE);
	}

	RecvMsgWindow::~RecvMsgWindow()
	{
		ClearWindow();
		delete[] m_pMsgWnd;
	}

	// Add message
	HRESULT RecvMsgWindow::AddMsg( Message::MessageData* pIMsg )
	{
		HRESULT hr = S_OK;
		auto msgSeq = pIMsg->GetMessageHeader()->msgID.IDSeq.Sequence;
		INT diff = Message::SequenceDifference(msgSeq, m_uiBaseSequence);

		if (diff >= GetWindowSize())
		{
			return E_NET_SEQUENCE_OVERFLOW; // No room for new message
		}

		if (diff < 0)
		{
			return S_NET_PROCESSED_SEQUENCE;
		}


		INT iPosIdx = msgSeq % CIRCULAR_QUEUE_SIZE;
		m_uiSyncMask.fetch_or(((UINT64)1) << iPosIdx, std::memory_order_relaxed);

		// check duplicated transaction
		MessageElement exptcted = nullptr;
		while( !m_pMsgWnd[iPosIdx].compare_exchange_weak(exptcted, pIMsg, std::memory_order_acquire) )
		{
			if (exptcted != nullptr)
			{
				// Garbage message could be remain, let's just clean up remain messages
				//Assert(exptcted->GetMessageHeader()->Length == pIMsg->GetMessageHeader()->Length
				//	&& exptcted->GetMessageHeader()->msgID.ID == pIMsg->GetMessageHeader()->msgID.ID
				//	&& exptcted->GetMessageHeader()->Sequence == pIMsg->GetMessageHeader()->Sequence);

				//// same message recived, Just ignore input message
				//return S_NET_PROCESSED_SEQUENCE;
			}
		}

		Util::SafeRelease(exptcted);

		m_uiMsgCount.fetch_add(1,std::memory_order_relaxed);


	Proc_End:

		return hr;
	}

	// Non-thread safe
	// Pop message and return it if can
	HRESULT RecvMsgWindow::PopMsg( Message::MessageData* &pIMsg )
	{
		HRESULT hr = S_OK;
		auto baseSequence = m_uiBaseSequence.load(std::memory_order_relaxed);
		INT iPosIdx = baseSequence % CIRCULAR_QUEUE_SIZE;
		if (m_pMsgWnd == nullptr )//|| m_pMsgWnd[iPosIdx].load(std::memory_order_relaxed) == nullptr)
			return E_FAIL;

		pIMsg = m_pMsgWnd[iPosIdx].exchange(nullptr,std::memory_order_acquire);
		if (pIMsg == nullptr)
			return E_FAIL;

		auto prevSeq = m_uiBaseSequence.fetch_add(1,std::memory_order_release);// Message window clear can't cross bese sequence change, and this will make sure the previous sync mask change is commited.
		Assert(Message::SequenceDifference(pIMsg->GetMessageHeader()->msgID.IDSeq.Sequence, prevSeq) == 0);

		// Between previous exchange and sequence update, the message can be arrived again
		// This will make sure the message is cleaned up
		auto pChkIMsg = m_pMsgWnd[iPosIdx].exchange(nullptr, std::memory_order_release);
		Util::SafeRelease(pChkIMsg);

		m_uiMsgCount.fetch_sub(1, std::memory_order_relaxed);

		auto messageMask = ((UINT64)1) << iPosIdx;
		m_uiSyncMask.fetch_and(~messageMask, std::memory_order_relaxed);

	Proc_End:

		return hr;

	}

	
	// Get SyncMask
	UINT64 RecvMsgWindow::GetSyncMask()
	{
		auto baseSeq = m_uiBaseSequence % CIRCULAR_QUEUE_SIZE;

		UINT64 resultSyncMask = m_uiSyncMask.load(std::memory_order_acquire);
		if (baseSeq != 0)
		{
			auto least = resultSyncMask >> baseSeq;
			auto most = resultSyncMask << (CIRCULAR_QUEUE_SIZE - baseSeq + SYNC_MASK_BITS);
			most >>= SYNC_MASK_BITS; // clear MSB
			resultSyncMask = least | most;
		}

//#ifdef DEBUG
		//UINT64 uiSyncMask = 0;
		//for (INT uiIdx = 0, iSeq = baseSeq; uiIdx < GetWindowSize(); uiIdx++, iSeq++)
		//{
		//	INT iPosIdx = iSeq % CIRCULAR_QUEUE_SIZE;
		//	if( m_pMsgWnd[ iPosIdx ].load(std::memory_order_relaxed) != nullptr )
		//	{
		//		uiSyncMask |= ((UINT64)1)<<uiIdx;
		//	}
		//}
		//AssertRel(resultSyncMask == uiSyncMask);
//#endif
		return resultSyncMask;
	}

	
	// Clear window element
	void RecvMsgWindow::ClearWindow()
	{
		Message::MessageData* data = NULL;
		HRESULT hr = S_OK;
		if (m_pMsgWnd)
		{
			for (INT iMsg = 0; iMsg < CIRCULAR_QUEUE_SIZE; iMsg++)
			{
				auto pMsg = m_pMsgWnd[iMsg].load(std::memory_order_relaxed);
				Util::SafeRelease(pMsg);
			}
			memset(m_pMsgWnd, 0, sizeof(MessageElement)*CIRCULAR_QUEUE_SIZE);
		}
		m_uiBaseSequence.store(0, std::memory_order_release);
		m_uiMsgCount.store(0, std::memory_order_release);
		m_uiSyncMask.store(0, std::memory_order_release);
	}
	
	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Send message window
	//


	SendMsgWindow::SendMsgWindow( INT  iWndSize )
		:MsgWindow(iWndSize),
		m_uiHeadSequence(0)
	{
	}

	SendMsgWindow::~SendMsgWindow()
	{
	}

	// Clear window element
	void SendMsgWindow::ClearWindow()
	{
		MutexScopeLock localLock(GetLock());

		__super::ClearWindow();
		m_uiHeadSequence = m_uiBaseSequence;
	}

	// Get available size at the end
	UINT SendMsgWindow::GetAvailableSize()
	{
		return GetWindowSize() - (m_uiHeadSequence - GetBaseSequence());
	}

	// Add a message at the end
	HRESULT SendMsgWindow::EnqueueMessage( ULONG ulTimeStampMS, Message::MessageData* pIMsg )
	{
		HRESULT hr = S_OK;
		INT iIdx = 0;

		if( GetAvailableSize() == 0 )
			return E_NET_NOT_ENOUGH_WINDOWSPACE;

		if( pIMsg == nullptr )
			return E_POINTER;

		// assign head sequence
		AssertRel(pIMsg->GetMessageHeader()->msgID.IDSeq.Sequence == 0);
		pIMsg->AssignSequence( m_uiHeadSequence );

		iIdx = Message::SequenceDifference(m_uiHeadSequence, m_uiBaseSequence);

		netAssert(iIdx < GetWindowSize());
		netAssert(iIdx >= 0);

		// To window queue array index
		iIdx = (m_uiWndBaseIndex + iIdx)%m_uiWndSize;

		// check duplicated transaction
		netAssert(m_pMsgWnd[ iIdx ].state == MSGSTATE_FREE );

		m_pMsgWnd[ iIdx ].ulTimeStamp = ulTimeStampMS;
		m_pMsgWnd[ iIdx ].pMsg = pIMsg;
		m_pMsgWnd[ iIdx ].state = MSGSTATE_DATA;

		m_uiHeadSequence++;
		m_uiMsgCount++;


	Proc_End:

		return hr;
	}

	// Release message sequence and slide window if can
	HRESULT SendMsgWindow::ReleaseMsg( UINT16 uiSequence )
	{
		HRESULT hr = S_OK;
		INT iIdx;


		if( m_pMsgWnd == NULL )
			return S_OK;// nothing to release


		iIdx = Message::SequenceDifference(uiSequence, m_uiBaseSequence);

		if( iIdx >= GetWindowSize() )
		{
			netErr( E_NET_INVALID_SEQUENCE ); // Out of range
		}

		if(  iIdx < 0 )
		{
			return S_NET_PROCESSED_SEQUENCE;
		}

		UINT iPosIdx = (m_uiWndBaseIndex + iIdx)%GetWindowSize();

		// Makr it as a can-be-freed
		if( m_pMsgWnd[ iPosIdx ].state != MSGSTATE_FREE )
		{
			if( m_pMsgWnd[iPosIdx].pMsg )
			{
				if( m_pMsgWnd[iPosIdx].pMsg->GetMessageHeader()->msgID.IDSeq.Sequence != uiSequence )
				{
					netTrace( TRC_GUARREANTEDCTRL, "Validation error : Message has Invalid Sequence %0%, %1% Required, msg:%2%", m_pMsgWnd[iPosIdx].pMsg->GetMessageHeader()->msgID.IDSeq.Sequence, uiSequence, BR::Arg<UINT32>(m_pMsgWnd[iPosIdx].pMsg->GetMessageHeader()->msgID.ID,-1,16) );
				}

				Util::SafeRelease( m_pMsgWnd[iPosIdx].pMsg );
				m_pMsgWnd[ iPosIdx ].state = MSGSTATE_CANFREE;
			}
		}

		// Slide window
		for( INT iMsg = 0; m_pMsgWnd[ m_uiWndBaseIndex ].state == MSGSTATE_CANFREE && iMsg < GetWindowSize();  )
		{
			m_uiBaseSequence++;
			m_pMsgWnd[ m_uiWndBaseIndex ].state = MSGSTATE_FREE;
			m_uiMsgCount--;
			iMsg++;
			m_uiWndBaseIndex = (m_uiWndBaseIndex+1)%GetWindowSize();
		}

	Proc_End:

		return hr;
	}


	// Release message sequence and slide window if can
	HRESULT SendMsgWindow::ReleaseMsg( UINT16 uiSequenceBase, UINT64 uiMsgMask )
	{
		HRESULT hr = S_OK;
		INT iIdx;

		MutexScopeLock localLock(m_Lock);

		if( m_pMsgWnd == nullptr )
			return S_OK;// nothing to release

		UINT uiSequence = uiSequenceBase, uiCurBit = 0, uiSyncMaskCur = 1;

		iIdx = Message::SequenceDifference(uiSequenceBase, m_uiBaseSequence);

		if( iIdx >= GetWindowSize() )
		{
			netErr( E_NET_INVALID_SEQUENCE ); // Out of range
		}

		if(  iIdx < 0 )
		{
			// SKip already processed message ids
			UINT uiIdx = (UINT)(-iIdx);
			iIdx = 0;

			// If client use same port for different connect this will be happened, and the connection need to be closed
			for (; uiCurBit < uiIdx; uiCurBit++, uiSyncMaskCur <<= 1)
			{
				if ((uiMsgMask & uiSyncMaskCur) == 0)
				{
					netErr(E_UNEXPECTED);
				}
			}
		}
		else if (iIdx > 0)
		{
			// Using other variable is common, but I used uiCurBit because it need to be increased anyway.
			for (; uiCurBit < (UINT)iIdx; uiCurBit++)
			{
				ReleaseMessage(uiCurBit);
			}
		}

		// At this point iIdx will have offset from local base sequence
		for (; uiCurBit < 64; uiCurBit++, uiSyncMaskCur <<= 1, iIdx++)
		{
			if ((uiMsgMask & uiSyncMaskCur) == 0) continue;

			ReleaseMessage((UINT)iIdx);

		}

		// Slide window
		for (INT iMsg = 0; m_pMsgWnd[m_uiWndBaseIndex].state == MSGSTATE_CANFREE && iMsg < GetWindowSize(); iMsg++)
		{
			m_uiBaseSequence++;
			m_pMsgWnd[ m_uiWndBaseIndex ].state = MSGSTATE_FREE;
			m_uiMsgCount--;
			m_uiWndBaseIndex = (m_uiWndBaseIndex+1)%GetWindowSize();
			hr = S_FALSE;
		}


	Proc_End:

		return hr;
	}

	// Release message sequence and slide window if can
	void SendMsgWindow::ReleaseMessage(UINT iIdx)
	{
		//for( UINT iIdx = iStartIdx; iIdx < iNumRelease && iIdx < (UINT)GetWindowSize(); iIdx++ )
		{
			UINT iPosIdx = (m_uiWndBaseIndex + iIdx)%GetWindowSize();

			// Makr it as a can-be-freed
			if( m_pMsgWnd[ iPosIdx ].state == MSGSTATE_FREE || m_pMsgWnd[ iPosIdx ].state == MSGSTATE_CANFREE )
			{
				Util::SafeRelease( m_pMsgWnd[iPosIdx].pMsg );
				return;
				//continue;
			}

			Util::SafeRelease( m_pMsgWnd[iPosIdx].pMsg );
			m_pMsgWnd[ iPosIdx ].state = MSGSTATE_CANFREE;
		}
	}




	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	SendMsgWindowMT
	//

	SendMsgWindowMT::SendMsgWindowMT()
		: m_uiHeadSequence(0)
		, m_uiBaseSequence(0)
		, m_uiMsgCount(0)
		, m_pMsgWnd(nullptr)
	{
		auto messageElements = new MessageElement[CIRCULAR_QUEUE_SIZE];
		memset(messageElements, 0, sizeof(MessageElement)*CIRCULAR_QUEUE_SIZE);
		m_pMsgWnd = messageElements;
	}

	SendMsgWindowMT::~SendMsgWindowMT()
	{
	}

	// Clear window element
	void SendMsgWindowMT::ClearWindow()
	{
		for (int iMsg = 0; iMsg < CIRCULAR_QUEUE_SIZE; iMsg++)
		{
			auto pMsg = m_pMsgWnd[iMsg].pMsg.load(std::memory_order_relaxed);
			Util::SafeRelease(pMsg);
		}
		m_uiHeadSequence = 0;
		m_uiBaseSequence = 0;
		m_uiMsgCount = 0;
	}

	// Get available size at the end
	INT SendMsgWindowMT::GetAvailableSize()
	{
		return GetWindowSize() - (m_uiHeadSequence.load(std::memory_order_relaxed) - m_uiBaseSequence.load(std::memory_order_relaxed));
	}

	// Add a message at the end
	HRESULT SendMsgWindowMT::EnqueueMessage(ULONG ulTimeStampMS, Message::MessageData* pIMsg)
	{
		HRESULT hr = S_OK;
		INT iIdx = 0;
		Message::MessageData* expectedMsg;
		UINT expectedID;

		if (pIMsg == nullptr)
			return E_POINTER;

		if (GetAvailableSize() <= 0)
			return E_NET_NOT_ENOUGH_WINDOWSPACE;

		AssertRel(pIMsg->GetMessageHeader()->msgID.IDSeq.Sequence == 0);

		// start from head sequence
		auto sequence = m_uiHeadSequence.load(std::memory_order_acquire);

		expectedMsg = nullptr;
		do
		{
			if (expectedMsg != nullptr)
			{
				// It's occupied. let's try next slot
				sequence++;
				if (Message::SequenceDifference(sequence, m_uiBaseSequence.load(std::memory_order_relaxed)) > GetWindowSize())
				{
					// out of sequence, this will very suck.
					Assert(false);
					pIMsg->ClearAssignedSequence();
					netErr(E_NET_NOT_ENOUGH_WINDOWSPACE);
				}
				expectedMsg = nullptr;
			}

			iIdx = sequence % CIRCULAR_QUEUE_SIZE;
			pIMsg->ClearAssignedSequence();
			pIMsg->AssignSequence(sequence);

		} while (!m_pMsgWnd[iIdx].pMsg.compare_exchange_weak(expectedMsg, pIMsg, std::memory_order_acquire));


		m_pMsgWnd[iIdx].ulTimeStamp.store(ulTimeStampMS, std::memory_order_relaxed);

		expectedID = sequence;
		while (!m_uiHeadSequence.compare_exchange_weak(expectedID, sequence + 1, std::memory_order_release))
		{
			if (expectedID > sequence)
				break; // somebody already moved on
		}

		m_uiMsgCount.fetch_add(1, std::memory_order_relaxed);


	Proc_End:

		return hr;
	}


	// Release message sequence and slide window if can
	HRESULT SendMsgWindowMT::ReleaseMsg(UINT16 uiSequenceBase, UINT64 uiMsgMask)
	{
		HRESULT hr = S_OK;
		INT iIdx;

		if (m_pMsgWnd == nullptr)
			return S_OK;// nothing to release

		UINT uiSequence = uiSequenceBase, uiCurBit = 0, uiSyncMaskCur = 1;

		iIdx = Message::SequenceDifference(uiSequenceBase, m_uiBaseSequence.load(std::memory_order_acquire));

		if (iIdx >= GetWindowSize())
		{
			netErr(E_NET_INVALID_SEQUENCE); // Out of range
		}

		if (iIdx < 0)
		{
			// SKip already processed message ids
			UINT uiIdx = (UINT)(-iIdx);
			iIdx = 0;
			uiSequenceBase += uiIdx;

			for (; uiCurBit < uiIdx; uiCurBit++, uiSyncMaskCur <<= 1) {
				AssertRel((uiMsgMask & uiSyncMaskCur) != 0);
			}
		}
		else if (iIdx > 0)
		{
			// Using other variable is common, but I used uiCurBit because it need to be increased anyway.
			for (; uiCurBit < (UINT)iIdx; uiCurBit++, uiSequenceBase++)
			{
				ReleaseMessage(uiSequenceBase);
			}
		}

		// At this point iIdx will have offset from local base sequence
		for (; uiCurBit < 64; uiCurBit++, uiSyncMaskCur <<= 1, uiSequenceBase++)
		{
			if ((uiMsgMask & uiSyncMaskCur) == 0) continue;

			ReleaseMessage(uiSequenceBase);
		}


		// Slide window
		auto sequence = m_uiBaseSequence.load(std::memory_order_acquire);
		while (sequence < m_uiHeadSequence.load(std::memory_order_acquire))
		{
			UINT iPosIdx = sequence % CIRCULAR_QUEUE_SIZE;
			if (m_pMsgWnd[iPosIdx].pMsg.load(std::memory_order_relaxed) == nullptr)
			{
				auto expected = sequence;
				while (!m_uiBaseSequence.compare_exchange_weak(expected, sequence + 1, std::memory_order_release))
				{
					if (expected != sequence)
					{
						// somebody released this sequence already, let's move on to next
						Assert(expected > sequence);
						break;
					}
				}
			}
			else
			{
				// We've done shifting
				break;
			}

			sequence++;
		}


	Proc_End:

		return hr;
	}

	// Release message sequence and slide window if can
	void SendMsgWindowMT::ReleaseMessage(UINT uiSequence)
	{
		UINT iPosIdx = uiSequence % CIRCULAR_QUEUE_SIZE;
		m_pMsgWnd[iPosIdx].ulTimeStamp.store(0, std::memory_order_relaxed);
		auto pIMsg = m_pMsgWnd[iPosIdx].pMsg.exchange(nullptr, std::memory_order_release);
		if (pIMsg != nullptr)
		{
			m_uiMsgCount.fetch_sub(1, std::memory_order_relaxed);
			Util::SafeRelease(pIMsg);
		}
	}



} // namespace Net
} // namespace BR


