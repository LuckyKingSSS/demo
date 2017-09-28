/*******************************

SingleLock.h
***************************************/
#pragma once

#ifndef WIN32
	#include <pthread.h>
#endif

class CCritical
{
public:
	CCritical()
	{
#ifdef WIN32
        InitializeCriticalSection(&m_CritSec);
#else
		//pthread_attr_t attr = { 0 };
        //pthread_attr_init(&attr);
		pthread_mutex_init(&mutex, 0);
        //pthread_attr_destroy(&attr);
#endif
    };

    ~CCritical() {
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

class CSingleLock
{
public:
	CSingleLock(CCritical* const pCritical)
		: m_pCritical(pCritical),m_bLocked(1)
	{
		m_pCritical->Lock();
	}

	virtual ~CSingleLock()
	{
		if(m_bLocked)
			m_pCritical->Unlock();
		m_bLocked = 0;
	}


protected:
	CCritical* const	m_pCritical;
	int					m_bLocked;
};

