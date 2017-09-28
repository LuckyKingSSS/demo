/*******************************

SyncEvent_Linux32.h
***************************************/
#pragma once

#include "SingleLock.h"
#include <sys/times.h>
#include <unistd.h>

	
#ifndef EVENT_WAIT_STATE
#define EVENT_WAIT_STATE

#define EWS_OK			(0)
#define EWS_TIMEOUT		(0x0102L)
#define EWS_ERROR		(-1)

#define WAIT_OBJECT_0	0

#endif // EVENT_WAIT_STATE

class SyncEvent
{
public:
	SyncEvent(BOOL bManualRest = FALSE, BOOL bInitState = FALSE) // bInitState - TRUE: Event is signaled
	: m_bSignal(bInitState)
	, m_bManualRest(bManualRest)
	{
    };

    ~SyncEvent() {
    };
	
	static DWORD timeGetTime()
	{
		tms tm;
		return (times(&tm) * 10);
	}	
	
	void SetEvent()
	{
		CSingleLock sl(&m_CritSec);
		m_bSignal = TRUE;
	}
	
	void ResetEvent()
	{
		CSingleLock sl(&m_CritSec);
		m_bSignal = FALSE;
	}
	
	DWORD Wait(UINT uMilliSecondsTimeOut = 0xFFFFFFFF)
	{
		DWORD state = EWS_OK;
		
		if (!m_bSignal)
		{
			if (0 == uMilliSecondsTimeOut)
			{
				state = EWS_TIMEOUT;
			}
			else
			{
				DWORD dwTime = timeGetTime();
				while(!m_bSignal)
				{
					DWORD dwCurTime = timeGetTime();
					if ((dwCurTime - dwTime) >= uMilliSecondsTimeOut)
					{
						state = EWS_TIMEOUT;
						break;
					}
			
					usleep(10000);
				}
			}
		}
		
		if (EWS_OK == state)
		{
			if (!m_bManualRest)
				ResetEvent();
		}
	
		return state;
	}
	
	
private:
    CCritical	m_CritSec;
	
	BOOL		m_bSignal;
	
	BOOL		m_bManualRest;

};

