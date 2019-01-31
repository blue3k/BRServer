////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server ServerTaskEvent manager
//	
//
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////
//
//	ServerTaskEvent class -  interface for task operation
//



inline ServerTaskEvent::ServerTaskEvent()
 : EventType(EventTypes::NONE)
{
}

inline ServerTaskEvent& ServerTaskEvent::operator = (void* src)
{
	assert(src == nullptr);
	EventType = EventTypes::NONE;
	TaskPtr = WeakPointerT<TickTask>();
	return *this;
}

inline bool ServerTaskEvent::operator == (const ServerTaskEvent& src) const
{
	return src.EventType == EventType && src.TaskPtr == TaskPtr;
}


inline bool ServerTaskEvent::operator != (const ServerTaskEvent& src) const
{
	return src.EventType != EventType || src.TaskPtr != TaskPtr;
}




