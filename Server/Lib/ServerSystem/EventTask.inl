////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server EventTask manager
//	
//
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////
//
//	EventTask class -  interface for task operation
//



inline EventTask::EventTask()
 : EventType(EventTypes::NONE)
{
}

inline EventTask& EventTask::operator = (void* src)
{
	assert(src == nullptr);
	EventType = EventTypes::NONE;
	TaskPtr = WeakPointerT<TickTask>();
	return *this;
}

inline bool EventTask::operator == (const EventTask& src) const
{
	return src.EventType == EventType && src.TaskPtr == TaskPtr;
}


inline bool EventTask::operator != (const EventTask& src) const
{
	return src.EventType != EventType || src.TaskPtr != TaskPtr;
}




