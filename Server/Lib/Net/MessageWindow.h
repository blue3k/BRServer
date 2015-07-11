////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves.
// 
// Author : KyungKun Ko
//
// Description : Net message window classes. 
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Net/NetDef.h"
#include "Net/NetCtrl.h"
#include "Net/NetSystem.h"
#include "Common/HRESNet.h"
#include "Common/PageQueue.h"
#include "Common/SystemSynchronize.h"
#include "Net/Connection.h"


namespace BR {
namespace Net {


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Packet message Sorting Window class
	//

	class MsgWindow
	{
	public:

		typedef enum tag_MessageElementState
		{
			MSGSTATE_FREE,
			MSGSTATE_DATA,
			MSGSTATE_CANFREE
		} MessageElementState;

		// Message element
		typedef struct tagMessageElement
		{
			MessageElementState state;
			ULONG ulTimeStamp;
			Message::MessageData *pMsg;
		} MessageElement;

	protected:
		// Base sequence value( sequence Head)
		UINT		m_uiBaseSequence;

		// Window base index
		UINT		m_uiWndBaseIndex;

		// Window size
		UINT		m_uiWndSize;

		// Message count in window
		UINT		m_uiMsgCount;

		// Message data array
		MessageElement*	m_pMsgWnd;

	public:
		// Constructor
		MsgWindow( INT iWndSize );
		virtual ~MsgWindow();

		// get window size
		FORCEINLINE INT GetWindowSize();

		// get message count in window
		FORCEINLINE UINT GetMsgCount();

		// get message base sequence
		FORCEINLINE UINT GetBaseSequence();

		// Get message info in window, index based on window base
		HRESULT GetAt( UINT uiIdx, MsgWindow::MessageElement* &pTimeMsg );
		
		// Clear window element
		virtual void ClearWindow();

	};

	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Recv message window
	//

	class RecvMsgWindow
	{
	public:

		typedef Message::MessageData* MessageElement;

		enum { 
			CIRCULAR_QUEUE_SIZE = 64,
			SYNC_MASK_BITS = 1,
			MESSAGE_WINDOW_SIZE = CIRCULAR_QUEUE_SIZE - SYNC_MASK_BITS,
		};

	private:

		std::atomic<UINT64>			m_uiSyncMask;

		// Base sequence value( sequence Head)
		std::atomic<UINT16>			m_uiBaseSequence;

		// Message count in window
		std::atomic<UINT>			m_uiMsgCount;

		// Message data array
		std::atomic<MessageElement>*	m_pMsgWnd;

	public:
		// Constructor
		RecvMsgWindow();
		~RecvMsgWindow();

		// get window size
		FORCEINLINE INT GetWindowSize()						{ return MESSAGE_WINDOW_SIZE; }

		// get message count in window
		FORCEINLINE UINT GetMsgCount()						{ return m_uiMsgCount.load(std::memory_order_relaxed); }

		// get message base sequence
		FORCEINLINE UINT16 GetBaseSequence()					{ return m_uiBaseSequence.load(std::memory_order_relaxed); }

		// Add message
		HRESULT AddMsg( Message::MessageData* pIMsg );

		// Pop message and return it if can
		HRESULT PopMsg( Message::MessageData* &pIMsg );

		// Get SyncMask
		UINT64 GetSyncMask();
		
		// Clear window element
		void ClearWindow();
	};
	
	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Send message window
	//

	class SendMsgWindow : public MsgWindow
	{
	public:

	private:
		UINT m_uiHeadSequence;

		// Until this can do thread safe release
		CriticalSection m_Lock;

	private:
		// Release message sequence and slide window if can
		void ReleaseMessage(UINT iIdx);


	public:
		// Constructor
		SendMsgWindow( INT iWndSize );
		~SendMsgWindow();

		CriticalSection& GetLock()				{ return m_Lock; }

		// Clear window element
		virtual void ClearWindow();

		// Get available size at the end
		UINT GetAvailableSize();
		
		// Add a message at the end
		HRESULT EnqueueMessage( ULONG ulTimeStampMS, Message::MessageData* pIMsg );

		// Release message sequence and slide window if can
		HRESULT ReleaseMsg( UINT16 uiSequence );
		// Release message by message mask
		HRESULT ReleaseMsg( UINT16 uiSequenceBase, UINT64 uiMsgMask );

	};

	class SendMsgWindowMT
	{
	public:

	private:
		enum {
			CIRCULAR_QUEUE_SIZE = 64,
			SYNC_MASK_BITS = 1,
			MESSAGE_WINDOW_SIZE = CIRCULAR_QUEUE_SIZE - SYNC_MASK_BITS,
		};

		// Message element
		typedef struct tagMessageElement
		{
			std::atomic<ULONG> ulTimeStamp;
			std::atomic<Message::MessageData*> pMsg;
		} MessageElement;


	private:
		std::atomic<UINT>	m_uiHeadSequence;

		// Base sequence value( sequence Head)
		std::atomic<UINT>	m_uiBaseSequence;

		// Message count in window
		std::atomic<UINT>	m_uiMsgCount;

		// Message data array
		MessageElement*	m_pMsgWnd;

	private:
		// Release message sequence and slide window if can
		void ReleaseMessage(UINT uiSequence);


	public:
		// Constructor
		SendMsgWindowMT();
		~SendMsgWindowMT();

		// get window size
		FORCEINLINE INT GetWindowSize()						{ return MESSAGE_WINDOW_SIZE; }

		// get message count in window
		FORCEINLINE UINT GetMsgCount()						{ return m_uiMsgCount.load(std::memory_order_relaxed); }

		// Clear window element
		virtual void ClearWindow();

		// Get available size at the end
		INT GetAvailableSize();

		// Add a message at the end
		HRESULT EnqueueMessage(ULONG ulTimeStampMS, Message::MessageData* pIMsg);

		// Release message sequence and slide window if can
		HRESULT ReleaseMsg(UINT16 uiSequence);

		// Release message by message mask
		HRESULT ReleaseMsg(UINT16 uiSequenceBase, UINT64 uiMsgMask);

	};


	#include "MessageWindow.inl"

}  // namespace Net
}; // namespace BR



