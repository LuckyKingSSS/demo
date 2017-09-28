#pragma once

#include <process.h>
#include <objbase.h>

#define IFABORTEVENT_BREAK_WAIT(milliseconds) \
{ \
	DWORD dwRet = WaitForSingleObject(m_hStopThreadEvent, milliseconds); \
	if (dwRet == WAIT_OBJECT_0) \
	{ \
		break; \
	} \
} 

class BaseThread
{
public:

	BaseThread(void)
		: m_hThread(0)
		, m_hStopThreadEvent(0)
	{
		m_hStopThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	virtual ~BaseThread(void)
	{
		Stop();

		CloseHandle(m_hStopThreadEvent);
		m_hStopThreadEvent = NULL;
	}

	BOOL Start()
	{
		if (m_hThread)
			return FALSE;

		UINT thread_id;
		m_hThread = (HANDLE)_beginthreadex (NULL, 0, thread_func, this, 0, &thread_id);

		if ((HANDLE)-1 == m_hThread)
			m_hThread = 0;

		return (0 != m_hThread);
	}

	BOOL Stop()
	{
		if (0 == m_hThread)
			return FALSE;

		SetEvent(m_hStopThreadEvent);
		while (ResumeThread(m_hThread) > 1) ;
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		m_hThread = 0;

		return TRUE;
	}

	BOOL Pause() 
	{
		SuspendThread(m_hThread);
		return TRUE;
	}

	BOOL Resume() 
	{
		ResumeThread(m_hThread);
		return TRUE;
	}

	void Wait() // 等待转换完成
	{
		WaitForSingleObject(m_hThread, INFINITE);
	}


private:
	virtual void svc() = 0
	{
		for (;;)
		{
			IFABORTEVENT_BREAK_WAIT(0);
			// do something
		}
	}

protected:

	HANDLE		m_hThread;
	HANDLE		m_hStopThreadEvent;	

	static UINT _stdcall thread_func (LPVOID pParam)
	{
		CoInitialize(NULL);
		BaseThread * pProcess = (BaseThread*)pParam;
		if (pProcess)
		{
			pProcess->svc();
		}

		CloseHandle(pProcess->m_hThread);
		pProcess->m_hThread = 0;
		CoUninitialize();

		return 0;
	}

};


