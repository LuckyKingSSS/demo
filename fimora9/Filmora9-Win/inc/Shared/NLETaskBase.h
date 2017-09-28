#pragma once
#include "INLETask.h"
#include "NLEComPtr.h"

class CNLETaskBase:public INLETask
{
public:
	CNLETaskBase();
	~CNLETaskBase();
public:
	STDOVERRIDEMETHODIMP NonDelegatingQueryInterface(REFIID riid, LPVOID *ppObj);

	virtual HRESULT __stdcall Initialize(UINT uID, INLETaskDelegate*pDelegate, LPVOID pParam) override;

	virtual UINT __stdcall GetTaskID() override;

	virtual HRESULT __stdcall Start(void) override;

	virtual HRESULT __stdcall Pause(void) override;

	virtual HRESULT __stdcall Stop(void) override;

	virtual NLEBOOL __stdcall Executable() override;

	virtual HRESULT __stdcall Finish(void);
	
protected:
	HRESULT NotifyDelegate();

protected:
	UINT GetCurrentProgess();
	/**
	 *@brief 设置进度值，范围[0, 100]
	 */
	void SetCurrentProcess(UINT uProgress);

	NLETaskState GetCurrentState();
	void SetCurrentState(NLETaskState eState);
private:
	UINT m_uID;
	INLETaskDelegate* m_pDelegate;
	LPVOID m_pDelegateParam;
	NLETaskState m_eState;
	UINT m_uProgress;
};

