
#pragma once


#include <pthread.h>
#include <assert.h>
#include <semaphore.h>


namespace BR
{


	///////////////////////////////////////////////////////////////////////////////
	//
	//	Critical Section
	//

	class CriticalSection : public MutexBase
	{
	public:
		CriticalSection()
		{
			pthread_mutexattr_t mAttr;
			pthread_mutexattr_settype(&mAttr, PTHREAD_MUTEX_RECURSIVE_NP);
			pthread_mutex_init(&m, &mAttr);
		}
		~CriticalSection()
		{
			pthread_mutexattr_destroy(&m_CriticalSection);
		}

		virtual void Lock()
		{
			pthread_mutex_lock(&m_CriticalSection);
		}

		virtual void UnLock()
		{
			pthread_mutex_unlock(&m_CriticalSection);
		}

	private:
		pthread_mutex_t m_CriticalSection;
	};



	class Mutex : public MutexBase
	{
	public:
		Mutex()
		{
			pthread_mutexattr_t mAttr;
			pthread_mutexattr_settype(&mAttr, PTHREAD_MUTEX_RECURSIVE);
			pthread_mutex_init(&m, &mAttr);
		}

		~Mutex()
		{
			pthread_mutexattr_destroy(&m_CriticalSection);
		}

		virtual void Lock() override
		{
			pthread_mutex_lock(&m_CriticalSection);
		}

		virtual void UnLock() override
		{
			pthread_mutex_unlock(&m_CriticalSection);
		}

	private:

		pthread_mutex_t m_CriticalSection;
	};



	class Event
	{
	public:

		Event(bool isInitialySet = false, bool autoReset = true)
			:m_AutoReset(autoReset)
		{
			sem_init(&m_hEvent, 0, isInitialySet ? 1 : 0);
		}

		~Event()
		{
			if (m_hEvent)
				sem_destroy(&m_hEvent);
		}

		void Reset()
		{
			timespec waitTime;
			memset(&waitTime, 0, sizeof(waitTime));

			if (clock_gettime(CLOCK_REALTIME, &waitTime) == -1)
				return;

			waitTime.tv_nsec += 1;
			sem_timedwait(&m_hEvent, &waitTime);
		}

		void Set()
		{
			if (sem_getvalue(&m_hEvent) == 1)
				return;

			sem_post(&m_hEvent);
		}

		bool WaitEvent(UINT uiWaitTimeMs)
		{
			// we need mutex version
			timespec waitTime;
			memset(&waitTime, 0, sizeof(waitTime));

			if (clock_gettime(CLOCK_REALTIME, &waitTime) == -1)
				return false;

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

}

