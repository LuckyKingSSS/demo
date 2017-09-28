/*******************************

SingleLock.h
***************************************/
#pragma once

#ifndef WIN32
	#include <pthread.h>
#endif

class CNLEMutex
{
public:
	CNLEMutex()
	{
#ifdef WIN32
        InitializeCriticalSection(&m_mutex);
#else
		pthread_mutexattr_t attr = { 0 };
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&m_mutex, &attr);
		pthread_mutexattr_destroy(&attr);
#endif
    };

    ~CNLEMutex() {
#ifdef WIN32
        DeleteCriticalSection(&m_mutex);
#else
        pthread_mutex_destroy(&m_mutex);
#endif
    };

    void Lock() {
#ifdef WIN32
        EnterCriticalSection(&m_mutex);
#else
		pthread_mutex_lock(&m_mutex);
#endif
    };

    void Unlock() {
#ifdef WIN32
        LeaveCriticalSection(&m_mutex);
#else
		pthread_mutex_unlock(&m_mutex);
#endif
    };

private:

#ifdef WIN32
    CRITICAL_SECTION m_mutex;
#else
	pthread_mutex_t  m_mutex;
#endif

};

class CNLESingleLock
{
public:
	CNLESingleLock(CNLEMutex* pCritical)
		: m_pMutex(pCritical),m_bLocked(1)
	{
		m_pMutex->Lock();
	}

	virtual ~CNLESingleLock()
	{
		if(m_bLocked)
			m_pMutex->Unlock();
		m_bLocked = 0;
	}


protected:
	CNLEMutex* 	        m_pMutex;
	int					m_bLocked;
};

