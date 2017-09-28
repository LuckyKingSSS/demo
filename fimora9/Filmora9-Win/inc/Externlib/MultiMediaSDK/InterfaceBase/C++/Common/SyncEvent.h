/*******************************

SyncEvent.h
***************************************/
#pragma once
#ifndef WIN32
#include "Linux32_Typedef.h"
#endif
#ifndef WIN32
	#include "SyncEvent_Linux32.h"

	typedef SyncEvent* HSYNCEVENT;
	
#else

	typedef HANDLE HSYNCEVENT;
	
#endif
	
#ifndef EVENT_WAIT_STATE
#define EVENT_WAIT_STATE

#define EWS_OK			(0)
#define EWS_TIMEOUT		(0x0102L)
#define EWS_ERROR		(-1)

#ifndef WAIT_OBJECT_0
#define WAIT_OBJECT_0	0
#endif

#endif // EVENT_WAIT_STATE



#ifdef WIN32

	#define SyncCreateEvent CreateEvent
	#define SyncCloseEvent CloseHandle
	
	#define SyncResetEvent ResetEvent
	#define SyncSetEvent SetEvent
	#define SyncWaitForSingleObject WaitForSingleObject
	
#else


__inline HSYNCEVENT SyncCreateEvent(void* pReserved = 0, BOOL bManualRest = FALSE, BOOL bInitState = FALSE, LPCTSTR lpName = 0)
{
	return static_cast<HSYNCEVENT>(new SyncEvent(bManualRest, bInitState));
}

__inline void SyncCloseEvent(HSYNCEVENT hEvent)
{
	delete static_cast<SyncEvent*>(hEvent);
}

__inline void SyncResetEvent(HSYNCEVENT hEvent)
{
	if (0 == hEvent)
		return;
		
	hEvent->ResetEvent();
}

__inline void SyncSetEvent(HSYNCEVENT hEvent)
{
	if (0 == hEvent)
		return;
		
	hEvent->SetEvent();
}

__inline DWORD SyncWaitForSingleObject(HSYNCEVENT hEvent, DWORD dwMilliseconds = 0xFFFFFFFF)
{
	if (0 == hEvent)
		return 0;
		
	return hEvent->Wait(dwMilliseconds);
}

	
#endif


