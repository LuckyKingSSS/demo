#include "stdafx.h"

#include "NLETaskBase.h"
#include "NLECommon_Const.h"



CNLETaskBase::CNLETaskBase()
{
	m_uID = NLECommon::Const::TASKID_INVALID;
}

CNLETaskBase::~CNLETaskBase()
{

}

STDOVERRIDEMETHODIMP CNLETaskBase::NonDelegatingQueryInterface(REFIID riid, LPVOID *ppObj)
{
	if (riid == IID_INLETask)
	{
		INLETask* pTask = static_cast<INLETask *>(static_cast<CNLETaskBase *>(this) );

		pTask->AddRef();
		*ppObj = pTask;
		return S_OK;
	}
	return E_NOTIMPL;
}

HRESULT __stdcall CNLETaskBase::Initialize(UINT uID, INLETaskDelegate*pDelegate, LPVOID pParam)
{
	m_uID = uID;
	m_pDelegate = pDelegate;
	m_pDelegateParam = pParam;
	m_eState = NLETaskStateReady;
	m_uProgress = 0;

	return NotifyDelegate();
}

UINT __stdcall CNLETaskBase::GetTaskID()
{
	return m_uID;
}

HRESULT __stdcall CNLETaskBase::Start(void)
{
	m_eState = NLETaskStateRunning;
	SetCurrentProcess(0);

	return NotifyDelegate();
}

HRESULT __stdcall CNLETaskBase::Pause(void)
{
	m_eState = NLETaskStatePaused;
	return NotifyDelegate();
}

HRESULT __stdcall CNLETaskBase::Stop(void)
{
	m_eState = NLETaskStateStopped;
	return NotifyDelegate();
}

NLEBOOL __stdcall CNLETaskBase::Executable()
{
	return TRUE;
}

HRESULT __stdcall CNLETaskBase::Finish(void)
{
	m_eState = NLETaskStateFinished;
	_ASSERT(m_uProgress == 100);
	return NotifyDelegate();
}

HRESULT CNLETaskBase::NotifyDelegate()
{
	if (m_pDelegate)
	{
		return m_pDelegate->OnStatus(m_eState, m_uProgress, m_pDelegateParam);
	}
	return S_OK;
}

UINT CNLETaskBase::GetCurrentProgess()
{
	return m_uProgress;
}

void CNLETaskBase::SetCurrentProcess(UINT uProgress)
{
	m_uProgress = uProgress; _ASSERT(m_uProgress <= 100);
}

NLETaskState CNLETaskBase::GetCurrentState()
{
	return m_eState;
}

void CNLETaskBase::SetCurrentState(NLETaskState eState)
{
	m_eState = eState;
}
