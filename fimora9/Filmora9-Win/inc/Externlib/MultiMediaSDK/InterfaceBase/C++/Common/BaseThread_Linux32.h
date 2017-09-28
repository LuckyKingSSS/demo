/*
 *  BaseThread_Linux32.h
 *  ThreadDemo
 *
 *  Created by ws wondershare on 07-5-17.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */
 
 #ifndef WIN32
#include "Linux32_Typedef.h"
#include <pthread.h>
#include <unistd.h>
#include <sys/times.h>
#include "SyncEvent.h"


#define WAITABORTEVENT(a) SyncWaitForSingleObject(m_hStopEvent, a)

#define IFABORTEVENT_BREAK(a) \
	if (EWS_OK == WAITABORTEVENT(a)) \
	{ \
		break; \
	}
	
#define IFPAUSESTATE_WAIT \
	{ \
		while(TS_PAUSE == m_uThreadState) \
		{ \
			usleep(10000); \
		} \
	}


enum THREAD_STATE
{
	TS_INACTIVE,
	TS_RUNNING,
	TS_PAUSE,
	TS_FINISH,
};

	
#ifndef EVENT_WAIT_STATE
#define EVENT_WAIT_STATE

#define EWS_OK			(0)
#define EWS_TIMEOUT		(0x0102L)
#define EWS_ERROR		(-1)

#define WAIT_OBJECT_0	0

#endif // EVENT_WAIT_STATE

class BaseThread
{
public:
	BaseThread()
	: m_uThreadState(TS_INACTIVE)
	, m_nOnPause(0)
	, m_hStopEvent(0)
	, m_hThreadEvent(0)
	{
		printf("BaseThread::BaseThread() [0x%x]...\r\n", this);
		pthread_t thread = { 0 };
		m_Thread = thread; 
		
		m_hStopEvent = SyncCreateEvent(0, TRUE, TRUE, 0);
		m_hThreadEvent = SyncCreateEvent(0, TRUE, TRUE, 0);
	}
	
	~BaseThread()
	{
		Abort();
		Wait();
		
		pthread_join(m_Thread, 0);
		
		m_nOnPause = 0;
		m_uThreadState = TS_INACTIVE;
		printf("BaseThread::~BaseThread() [0x%x]\r\n", this);
		
		
		SyncCloseEvent(m_hStopEvent); m_hStopEvent = 0;
		SyncCloseEvent(m_hThreadEvent); m_hThreadEvent = 0;

	}
	
	BOOL Start()
	{
		if (IsActive())
			return FALSE;
	
		
		SyncResetEvent(m_hStopEvent);
		SyncResetEvent(m_hThreadEvent);
	
		printf("BaseThread::Start() [0x%x]...\r\n", this);
		
		pthread_attr_t attr = { 0 };

        pthread_attr_init(&attr);
 
        int error = pthread_create(&m_Thread,
                                           &attr,
                                           thread_func,
                                           (void*)this);
        pthread_attr_destroy(&attr);

		if (error)
		{
			SyncSetEvent(m_hStopEvent);
			SyncSetEvent(m_hThreadEvent);
			printf("BaseThread::Start() Failed.\r\n");
			return FALSE;
		}

		//vm_thread_set_priority(thread, VM_THREAD_PRIORITY_LOWEST);

		m_nOnPause = 0;

		m_uThreadState = TS_RUNNING;

		return TRUE;
		
	}
	
	int Pause()
	{
		if (IsActive())
		{
			m_nOnPause ++;
			if (1 == m_nOnPause)
				m_uThreadState = TS_PAUSE;
		}
		return m_nOnPause;
	}
	
	int Resume()
	{
		if (IsActive())
		{
			if (m_nOnPause)
				m_nOnPause --;
			if (0 == m_nOnPause)
				m_uThreadState = TS_RUNNING;
		}

		return m_nOnPause;
	}
	
	THREAD_STATE State()
	{
		return m_uThreadState;
	}
	
	BOOL IsRuning()
	{
		return (TS_RUNNING == m_uThreadState); 
	}
	
	BOOL IsPause()
	{
		return (TS_PAUSE == m_uThreadState); 
	}
	
	BOOL IsActive()
	{
		return ((TS_RUNNING == m_uThreadState) || (TS_PAUSE == m_uThreadState)); 
	}
	
	static DWORD timeGetTime()
	{
		tms tm;
		return (times(&tm) * 10);
	}	
	
//	BOOL Stop()
//	{
//		if (!IsActive())
//			return FALSE;
//		
//		printf("BaseThread::Stop() ...\r\n");
//		SyncSetEvent(m_hStopEvent);
//
//		while(Resume());
//
//		SyncWaitForSingleObject(m_hThreadEvent);
//
//		pthread_join(m_Thread, 0);
//		printf("BaseThread::Stop() OK\r\n");
//		
//		return TRUE;
//	}
	
	BOOL Abort() // Post Abort Message
	{
		if (!IsActive())
			return FALSE;
		
		printf("BaseThread::Abort() SyncSetEvent ...\r\n");
		SyncSetEvent(m_hStopEvent);

		while(Resume());
		
		return TRUE;
	}
	
	DWORD Wait(UINT uMilliSecondsTimeOut = 0xFFFFFFFF)
	{	
		if (!IsActive())
		{
			printf("BaseThread::Wait() OK no active\r\n");
			return EWS_OK;
		}
			
		printf("BaseThread::Wait() ...\r\n");
		
		DWORD dwRet = SyncWaitForSingleObject(m_hThreadEvent, uMilliSecondsTimeOut);
		pthread_join(m_Thread, 0);
		
		printf("BaseThread::Wait() OK\r\n");
	
		return dwRet;
	}
	
protected:
	virtual void svc()
	{
		for (;;)
		{
			IFPAUSESTATE_WAIT;
			IFABORTEVENT_BREAK(0);
			// do somthing
		} 
	}
	
protected:
	pthread_t m_Thread; 

	HSYNCEVENT		m_hStopEvent;
	HSYNCEVENT		m_hThreadEvent;
	
	int				m_nOnPause;
	THREAD_STATE	m_uThreadState;
	
	static void* thread_func(void* pParam)
	{
		BaseThread* pProcess = static_cast<BaseThread*>(pParam);
		if (pProcess)
		{
			pProcess->svc();
			SyncSetEvent(pProcess->m_hThreadEvent);
			pProcess->m_uThreadState = TS_FINISH;
		
			printf("BaseThread::thread_func() exit.\r\n");
		
		/*
			pthread_mutex_lock(&pProcess->m_StopMutex);

			// pthread_cond_signal(&m_Cond);
			pthread_cond_broadcast(&pProcess->m_Cond);
			printf("[0x%x] thread exit: pthread_cond_broadcast.\r\n", pProcess);

			pProcess->m_uThreadState = TS_FINISH;
			
			pthread_mutex_unlock(&pProcess->m_StopMutex);
		*/
		}
	}  
	
};

#endif // ifndef WIN32
