#pragma once

#include "BaseThread.h"
#include "SingleLock.h"
#include "Linux32_typedef.h"
#include "GetTime.h"
#include <list>

typedef BOOL (__stdcall * MSG_CALLBACK)(void* pUserObj, UINT uMsg, WPARAM wParam, LPARAM lParam);

class AsyncCallback : public BaseThread
{
protected:
	struct MSG_INFO
	{
		void* pObj; 
		UINT msg_id; 
		WPARAM wParam; 
		LPARAM lParam; 
		DWORD time;
	};
	std::list<MSG_INFO> m_lstMsg;
	CCritical m_cs;
	MSG_CALLBACK m_pfnCallback;
	DWORD m_dwInterval; //抛消息间隔
public:
	AsyncCallback(void):m_pfnCallback(NULL),m_dwInterval(0){}
	virtual ~AsyncCallback(void){}

	virtual void svc()
	{
		BOOL bCall = FALSE;
		MSG_INFO mi;
		while (true)
		{
			bCall = FALSE;
			IFPAUSESTATE_WAIT;
			IFABORTEVENT_BREAK(10);

			{
				CSingleLock sl(&m_cs);
				if (m_lstMsg.size() > 0)
				{
					mi = m_lstMsg.front();
					m_lstMsg.pop_front();
					bCall = TRUE;
				}
			}

			if (bCall && m_pfnCallback)
			{
				m_pfnCallback(mi.pObj, mi.msg_id, mi.wParam, mi.lParam);
			}
		}
	}

	BOOL PostCallbackMessage(void* pObj, UINT msg_id, WPARAM wParam, LPARAM lParam)
	{
		CSingleLock sl(&m_cs);
		
		MSG_INFO mi = {0};
		mi.pObj = pObj;
		mi.msg_id = msg_id;
		mi.wParam = wParam;
		mi.lParam = lParam;
		mi.time = highGetTime();
		if (m_lstMsg.size() > 0 && mi.time - m_lstMsg.back().time < m_dwInterval)
			return FALSE;

		m_lstMsg.push_back(mi);
		return TRUE;
	}

	void SetFnCallback(MSG_CALLBACK pfn) {m_pfnCallback = pfn;}
	void SetMessageInterval(DWORD dwInterval) {m_dwInterval = dwInterval;}
};