#pragma once


#include "INLETransition.h"
#include "NLEComPtr.h"
#include "INLERender.h"
#include "NLEKey.h"

#define MAX_TRANSID_LEN	128

class CNLETransition
{
public:
	CNLETransition()
	{
		m_pFrom = m_pTo = m_pOut = NULL;
	}
	~CNLETransition()
	{

	}

public:
	void SetRender(NLEComPtr<INLERender> pRender)	{ m_pRender = pRender; }
	void SetFrom(NLEComPtr<INLEFrame> pFrom)		{ m_pFrom = pFrom; }
	void SetTo(NLEComPtr<INLEFrame> pTo)			{ m_pTo = pTo; }
	void SetOut(NLEComPtr<INLEFrame> pOut)			{ m_pOut = pOut; }

	BOOL CheckTransition() 
	{ 
		NLE_ASSERT(m_pRender);
		HRESULT hr = S_OK;
		NLEFrameIndex nCurOffset = m_pRender->GetOffset();
		
		NLEPropDecorator pProp = NLECommon::NLEGetProperties(m_pRender);
		NLERange rngMSec = (CNLEValue)pProp[NLEKey::Transition::kTransition_RangeTime];
		NLERational framerate = (CNLEValue)pProp[NLEKey::Render::kRender_Framerate];
		INT nFrameStart = (INT)(rngMSec.Start *framerate.Value() / 1000);
		INT nFrameEnd = nFrameStart + (INT)(rngMSec.End *framerate.Value() / 1000);

		BOOL nEnable = pProp[NLEKey::Transition::kTransition_Enable];
		if (!nEnable || (nCurOffset < nFrameStart || nCurOffset > nFrameEnd))
		{
			m_pTransition = NULL; m_bEnable = FALSE;			
			return m_bEnable;
		}
		
		do
		{
			WCHAR wszTransID[MAX_TRANSID_LEN];
			hr = pProp->GetString(NLEKey::Transition::kTransition_ID, wszTransID, MAX_TRANSID_LEN);
			BREAK_IF_FAILED(hr);

			if (wcslen(wszTransID) == 0)
			{
				m_pTransition = NULL;
				m_szTransID[0] = L'\0';
				break;
			}
			
			if (m_pTransition == NULL)
			{
				hr = NLECommon::GetInterface(CLSID_NLETransition, NULL, 0, IID_INLETransition, (LPVOID*)&m_pTransition);
				BREAK_IF_NULL(m_pTransition);
			}

			wcscpy_s(m_szTransID, MAX_TRANSID_LEN, wszTransID);

			hr = m_pTransition->SetTransID(wszTransID);
			BREAK_IF_FAILED(hr);

			hr = m_pTransition->SetOutSize(FRAME_WIDTH(m_pOut), FRAME_HEIGHT(m_pOut) );
			BREAK_IF_FAILED(hr);

			hr = m_pTransition->SetDuration(nFrameEnd - nFrameStart + 1);
			BREAK_IF_FAILED(hr);

		} while (false);

		if (SUCCEEDED(hr))
		{
			if (m_pTransition != NULL)
			{
				hr = m_pTransition->SetPosition(nCurOffset - nFrameStart);
			}
			m_bEnable = TRUE;
		}
		else
		{
			m_pTransition = NULL;
			m_szTransID[0] = L'\0';
			m_bEnable = FALSE;
		}
		return m_bEnable;
	}
	HRESULT DoTransition()
	{
		if (!m_bEnable)
		{
			return S_FALSE;
		}
		NLE_ASSERT(m_pTransition);

		HRESULT hr = m_pTransition->SetOutSize(FRAME_WIDTH(m_pOut), FRAME_HEIGHT(m_pOut) );
		RETURN_IF_FAILED(hr, hr);

		hr = m_pTransition->DoTransition(FRAME_IMAGE(m_pFrom), FRAME_IMAGE(m_pTo), FRAME_IMAGE(m_pOut) );
		CHECK(hr);
		return hr;
	}
	HRESULT Reset() 
	{ 
		m_pFrom = m_pTo = m_pOut = NULL;
		m_pTransition = NULL;
		m_bEnable = FALSE;
		return S_OK; 
	}
	BOOL Enable() { return m_bEnable; }
private:
	INLERender* m_pRender;
	NLEComPtr<INLEFrame> m_pFrom;
	NLEComPtr<INLEFrame> m_pTo;
	NLEComPtr<INLEFrame> m_pOut;
		
	BOOL m_bEnable;
private:
	WCHAR m_szTransID[MAX_TRANSID_LEN];
	NLEComPtr<INLETransition> m_pTransition;
};
