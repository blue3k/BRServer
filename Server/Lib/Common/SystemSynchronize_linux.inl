
#pragma once





///////////////////////////////////////////////////////////////////////////////
//
//	Critical Section
//

class CriticalSection : public MutexBase
{
private:
	pthread_mutex_t m_CriticalSection;

public:
	CriticalSection()
	{
		Result hr;
		pthread_mutexattr_t mAttr;
		pthread_mutexattr_init(&mAttr);
		pthread_mutexattr_settype(&mAttr, PTHREAD_MUTEX_RECURSIVE_NP);
		int result = pthread_mutex_init(&m_CriticalSection, &mAttr);
		if (result != 0)
		{
			const char* errorString = strerror(result);
			printf("%s", errorString);
			hr = ResultCode::FAIL;
			switch(result)
			{
				case EBUSY:		hr = ResultCode::UNEXPECTED;		break;
				case EINVAL:	hr = ResultCode::INVALID_ARG;		break;
				case EAGAIN:	hr = ResultCode::UNEXPECTED;		break;
				case ENOMEM:	hr = ResultCode::OUT_OF_MEMORY;		break;
				case EPERM:		hr = ResultCode::FAIL;			break;
				default:
					hr = ResultCode::UNEXPECTED;
					break;
			}

			unused(hr);
		}

		if ((hr))
		{
			Lock();
			UnLock();
		}
	}
	~CriticalSection()
	{
		pthread_mutex_destroy(&m_CriticalSection);
	}

	virtual void Lock()
	{
		pthread_mutex_lock(&m_CriticalSection);
	}

	virtual void UnLock()
	{
		pthread_mutex_unlock(&m_CriticalSection);
	}

};



class Mutex : public MutexBase
{
private:
	pthread_mutex_t m_CriticalSection;

public:
	Mutex()
	{
		pthread_mutexattr_t mAttr;
		pthread_mutexattr_init(&mAttr);
		pthread_mutexattr_settype(&mAttr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&m_CriticalSection, &mAttr);
	}

	~Mutex()
	{
		pthread_mutex_destroy(&m_CriticalSection);
	}

	virtual void Lock() override
	{
		pthread_mutex_lock(&m_CriticalSection);
	}

	virtual void UnLock() override
	{
		pthread_mutex_unlock(&m_CriticalSection);
	}

};




class Event
{
public:

	Event(bool isInitialySet = false, bool autoReset = true)
		:m_AutoReset(autoReset)
	{
		sem_init(&m_hEvent, 1, isInitialySet ? 1 : 0);
	}

	~Event()
	{
		sem_close(&m_hEvent);
		sem_destroy(&m_hEvent);
	}

	void Reset()
	{
		::timespec waitTime;
		memset(&waitTime, 0, sizeof(waitTime));

		if (clock_gettime(CLOCK_REALTIME, &waitTime) == -1)
			return;

		waitTime.tv_nsec += 1;
		sem_timedwait(&m_hEvent, (const timespec*)&waitTime);
	}

	void Set()
	{
		int value = 0;
		int error = sem_getvalue(&m_hEvent, &value);
		Assert(error == 0);
		if (value == 1 || error != 0)
			return;

		sem_post(&m_hEvent);
	}


	bool WaitEvent(DurationMS waitTimeMs)
	{
		// we need mutex version
		timespec waitTime;
		memset(&waitTime, 0, sizeof(waitTime));

		if (clock_gettime(CLOCK_REALTIME, &waitTime) == -1)
			return false;

		UINT uiWaitTimeMs = waitTimeMs.count();
		waitTime.tv_sec += uiWaitTimeMs / 1000;
		waitTime.tv_nsec += 1000000 * (uiWaitTimeMs % 1000);
		int waitRes = sem_timedwait(&m_hEvent, &waitTime);
		if (waitRes == ETIMEDOUT)
			return false;
		else if (waitRes == EAGAIN)
			return false;

		if (!m_AutoReset)
			sem_post(&m_hEvent);

		return true;
	}

private:
	sem_t	m_hEvent;
	bool	m_AutoReset;
};



class Semaphore
{
public:

	Semaphore(LONG lInitialCount = 0, LONG lMaxCount = LONG_MAX)
	{
		sem_init(&m_hSemaphore, lMaxCount, lInitialCount);
	}

	~Semaphore()
	{
		sem_close(&m_hSemaphore);
		sem_destroy(&m_hSemaphore);
	}

	void Release(LONG lResleaseCount = 1)
	{
		sem_post(&m_hSemaphore);
	}

	bool Acquire(UINT uiWaitTimeMs)
	{
		timespec waitTime;
		memset(&waitTime, 0, sizeof(waitTime));

		if (clock_gettime(CLOCK_REALTIME, &waitTime) == -1)
			return false;

		waitTime.tv_nsec += uiWaitTimeMs * 1000000;
		sem_timedwait(&m_hSemaphore, &waitTime);
		return true;
	}

private:
	sem_t	m_hSemaphore;
};



