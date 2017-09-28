/*******************************

SyncEvent_Linux32.h
***************************************/
#pragma once
#ifndef WIN32
#include "Linux32_Typedef.h"
#endif
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
	: m_nEventRefCount(0)
	, m_bManualRest(bManualRest)
	{
		if (!bInitState)
			m_nEventRefCount = -1;
    };

    ~SyncEvent() {
    };
	
	static DWORD timeGetTime()
	{
		tms tm;
		return (times(&tm) * 10);
	}	
	
	int SetEvent()
	{
		CSingleLock sl(&m_CritSec);
		return m_nEventRefCount++; 
	}
	
	int ResetEvent()
	{
		CSingleLock sl(&m_CritSec);
		return m_nEventRefCount--; 
	}
	
	DWORD Wait(UINT uMilliSecondsTimeOut = 0xFFFFFFFF)
	{
		DWORD state = EWS_OK;
		
		int nRefCount = m_nEventRefCount; // GetEventRefCount()
		if (nRefCount < 0)
		{
			if (0 == uMilliSecondsTimeOut)
			{
				state = EWS_TIMEOUT;
			}
			else
			{
				DWORD dwTime = timeGetTime();
				while(m_nEventRefCount < 0)
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
	
	int GetEventRefCount()
	{
		//CSingleLock sl(&m_CritSec);
		return m_nEventRefCount; 
	}
	
	
private:
    CCritical	m_CritSec;
	
	int			m_nEventRefCount;
	
	BOOL		m_bManualRest;

};

