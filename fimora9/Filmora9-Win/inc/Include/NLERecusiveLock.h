/*******************************

SingleLock.h
***************************************/
#pragma once

#ifndef WIN32
	#include <pthread.h>
#endif

class CNLERecursive
{
public:
	CNLERecursive()
	{
#ifdef WIN32
        InitializeCriticalSection(&m_CritSec);
#else
		pthread_mutexattr_t attr;

		pthread_mutexattr_init( &attr );

		pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );

		pthread_mutex_init( &mutex, &attr );

		pthread_mutexattr_destroy( &attr );
#endif
    };

	~CNLERecursive() {
#ifdef WIN32
        DeleteCriticalSection(&m_CritSec);
#else
        pthread_mutex_destroy(&mutex);
#endif
    };

    void Lock() {
#ifdef WIN32
        EnterCriticalSection(&m_CritSec);
#else
		pthread_mutex_lock(&mutex);
#endif
    };

    void Unlock() {
#ifdef WIN32
        LeaveCriticalSection(&m_CritSec);
#else
		pthread_mutex_unlock(&mutex);
#endif
    };

private:

#ifdef WIN32
    CRITICAL_SECTION m_CritSec;
#else
	pthread_mutex_t  mutex;
#endif

};

class CNLERecursiveLock
{
public:
	CNLERecursiveLock(CNLERecursive* const pCritical)
		: m_pCritical(pCritical),m_bLocked(1)
	{
		m_pCritical->Lock();
	}

	virtual ~CNLERecursiveLock()
	{
		if(m_bLocked)
			m_pCritical->Unlock();
		m_bLocked = 0;
	}


protected:
	CNLERecursive* const	m_pCritical;
	int					m_bLocked;
};

