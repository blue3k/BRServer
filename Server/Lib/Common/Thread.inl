////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Thread utilities
//
////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//
//	thread
//




ThreadHandle Thread::GetThread()
{
	return std::thread::native_handle();
}

ThreadID Thread::GetThreadID()
{
	return std::thread::get_id();
}

Thread::PRIORITY Thread::GetPriority()
{
	return m_threadPriority;
}


// Get end event handle
inline Event& Thread::GetKillEvent()
{
	return m_KillEvent;
}

