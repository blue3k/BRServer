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
inline bool Thread::GetKillEvent()
{
	return m_KillFlag.load(std::memory_order_acquire);
}

inline void Thread::SetKillEvent()
{
	m_KillFlag.store(true, std::memory_order_release);
}
