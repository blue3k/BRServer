////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves.
// 
// Author : KyungKun Ko
//
// Description : Net message window inline implementation. 
//	
//
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//	Packet message Sorting Window class
//


// get window size
inline INT MsgWindow::GetWindowSize()
{
	return m_uiWndSize;
}

// get message count in window
inline UINT MsgWindow::GetMsgCount()
{
	return m_uiMsgCount;
}

// get message base sequence
inline UINT MsgWindow::GetBaseSequence()
{
	return m_uiBaseSequence;
}





