#include "stdafx.h"

#include "NLEService.h"
#include "NLEKey.h"

#include "NLEMacroDefine.h"
#include "NLECommon.h"
#include "INLEFactory.h"
#include "NLEComError.h"

CNLEService::CNLEService()
{
    m_pParent = NULL;
	m_pProperties = NULL;
	m_nSession = 0;
}

CNLEService::~CNLEService()
{

}

STDOVERRIDEMETHODIMP CNLEService::NonDelegatingQueryInterface(REFIID riid, LPVOID *ppObj)
{
	if (riid == IID_INLEService)
	{
		INLEService* pSvr = static_cast<INLEService *>(static_cast<CNLEService *>(this));

		pSvr->AddRef();
		*ppObj = pSvr;
		return S_OK;
	}

	if (riid == IID_INLEProperties)
	{
		INLEProperties* pSvr = static_cast<INLEProperties *>(this);
		pSvr->AddRef();
		*ppObj = pSvr;
		return S_OK;
	}

	return E_FAIL;
}

HRESULT CNLEService::Init(void)
{
	HRESULT hr = S_OK;
	if (NULL == m_pProperties)
	{
		hr = NLECommon::GetInterface(CLSID_NLEProperties,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_INLEProperties,
			(LPVOID *)&m_pProperties);
		NLE_ASSERT(m_pProperties);
	}
	m_nSession = 0;
	return hr;
}

NLEPropertyListeners CNLEService::GetListeners(LPCWSTR keyName)
{
	if (NULL == keyName)
	{
		return m_listeners;
	}
	
	NLEPropertyListeners tmpListenerList;
	NLEPropertyListeners::iterator it = m_listeners.begin();
	while (it != m_listeners.end() )
	{
		if (wcscmp(it->keyName, keyName) == 0)
		{
			tmpListenerList.push_back(*it);
		}
		++it;
	}
	return tmpListenerList;
}


template <typename FuncType, typename ParamType>
HRESULT CNLEService::NotifyListeners(LPCWSTR keyName, ParamType param)
{
//    LOGGER_INFO_(L"Notify keyName:%ls", keyName);
    
	HRESULT hr = S_OK;
	NLEPropertyListeners lstListen = this->GetListeners(keyName);
	NLEPropertyListeners::iterator it = lstListen.begin();
	while (it != lstListen.end())
	{
		FuncType pfn = (FuncType)(it->listener);
		PropSetingClass* pObj = it->pObj;
		_ASSERT(pObj);
		if (pObj)
		{
			hr = (pObj->*pfn)(keyName,param);
			CHECK(hr);
			RETURN_IF_FAILED(hr, hr);
		}
		++it;
	}
	return hr;
}

template <typename FuncType, typename ParamType1, typename ParamType2, typename ParamType3>
HRESULT CNLEService::NotifyListeners(LPCWSTR keyName, ParamType1 p1, ParamType2 p2, ParamType3 p3)
{
	HRESULT hr = S_OK;
	NLEPropertyListeners lstListen = this->GetListeners(keyName);
	NLEPropertyListeners::iterator it = lstListen.begin();
	while (it != lstListen.end())
	{
		FuncType pfn = (FuncType)(it->listener);
		PropSetingClass* pThis = it->pObj;
		_ASSERT(pThis);
		if (pThis)
		{
			hr = (pThis->*pfn)(keyName, p1,p2,p3);
			_ASSERT(SUCCEEDED(hr));
		}
		++it;
	}
	return hr;
}

HRESULT __stdcall CNLEService::SetColorF(LPCWSTR keyName, NLEColorF color)
{
	return this->Set(keyName, color);
}

HRESULT __stdcall CNLEService::GetColorF(LPCWSTR keyName, NLEColorF *pColor)
{
	_ASSERT(m_pProperties);
	RETURN_IF_FAILED(WillGetProperty(keyName), E_NLE_ACCESS_DENIED);

	HRESULT hr = m_pProperties->GetColorF(keyName, pColor);

	DoneGetProperty(keyName);

	return hr;
}

void STDMETHODCALLTYPE CNLEService::RegisterListeners()
{

}

HRESULT __stdcall CNLEService::AddListener(LPCWSTR keyName, AnyFunction pfn, PropSetingClass* pThis /*= NULL*/)
{
	LPVOID propThis = pThis;
	HRESULT hr = E_FAIL;

	NLEPropertyListeners::iterator it = m_listeners.begin();
	while (it != m_listeners.end())
	{
		if (wcscmp(keyName, it->keyName) == 0 &&
			(pfn == it->listener) &&
			(pThis == it->pObj))
		{
			return S_OK;
		}
		++it;
	}
	m_listeners.push_back(NLEPropetyListener(keyName, (AnyFunction)pfn, (scombase::CUnknown*)propThis));
	return S_OK;
}

HRESULT __stdcall CNLEService::SetValue(LPCWSTR keyName, CNLEValue& val)
{
	HRESULT hr = E_NLE_UNEXPECTED;

	_ASSERT(m_pProperties);
	PropValue propValue = { NLEVariantValue, &val };
	RETURN_IF_FAILED(hr = WillSetProperty(keyName, val), hr);
	
	if (FAILED( (hr = NotifyListeners<FunctionSetValue, CNLEValue&>(keyName, val) ) ) )
	{
		LOGGER_ERROR_(L"Failed to notify listeners(%s:%s)", keyName, val.ToString().c_str()); CHECK(hr);
		return hr;
	}
	
	hr = m_pProperties->SetValue(keyName, val); CHECK(hr);
	
	DoneSetProperty(keyName);

	return hr;
}

HRESULT __stdcall CNLEService::GetValue(LPCWSTR keyName, CNLEValue& val)
{
	HRESULT hr = E_NLE_UNEXPECTED;

	_ASSERT(m_pProperties);
    PropValue propValue = { NLEVariantValue, &val };
	RETURN_IF_FAILED(hr = WillGetProperty(keyName), hr);
		
	if (FAILED(hr = m_pProperties->GetValue(keyName, val)))
	{
		LOGGER_ERROR_(L"Failed to get property :%s", keyName); _ASSERT(FALSE);
		return hr;
	}	
	DoneGetProperty(keyName);
	return S_OK;
}

HRESULT __stdcall CNLEService::DelValue(LPCWSTR keyName)
{
	HRESULT hr = E_NLE_UNEXPECTED;

	_ASSERT(m_pProperties);
	RETURN_IF_FAILED(hr = WillDeleteProperty(keyName), hr);
		
	if (FAILED(hr = m_pProperties->Delete(keyName)))
	{
		LOGGER_ERROR_(L"Failed to delte property:%s", keyName); _ASSERT(FALSE);
		return hr;
	}
	
	DoneDeleteProperty(keyName);

	return hr;
}

HRESULT __stdcall CNLEService::GetCount(INT *pCount)
{
	_ASSERT(m_pProperties);
	return m_pProperties->GetCount(pCount);
}

// 
HRESULT __stdcall CNLEService::SetInt(LPCWSTR keyName, INT nValue)
{
	return this->Set<int>(keyName, (int&)nValue);
}

HRESULT __stdcall CNLEService::GetInt(LPCWSTR keyName, INT *pValue)
{
	_ASSERT(m_pProperties);
	RETURN_IF_FAILED(WillGetProperty(keyName), E_NLE_ACCESS_DENIED);

	HRESULT hr = m_pProperties->GetInt(keyName, pValue);

	DoneGetProperty(keyName);

	return hr;
}

HRESULT __stdcall CNLEService::SetInt64(LPCWSTR keyName, NLElonglong llValue)
{
	return this->Set(keyName, (long long&)llValue);
}

HRESULT __stdcall CNLEService::GetInt64(LPCWSTR keyName, NLElonglong *pValue)
{
	_ASSERT(m_pProperties);
	RETURN_IF_FAILED(WillGetProperty(keyName), E_NLE_ACCESS_DENIED);

	HRESULT hr = m_pProperties->GetInt64(keyName, pValue);

	DoneGetProperty(keyName);

	return hr;
}

HRESULT __stdcall CNLEService::SetString(LPCWSTR keyName, LPCWSTR szValue)
{
	std::wstring str(szValue);
	return this->Set(keyName, str);
}

HRESULT __stdcall CNLEService::GetString(LPCWSTR keyName, LPWSTR szValue, int size)
{
	_ASSERT(m_pProperties);
	RETURN_IF_FAILED(WillGetProperty(keyName), E_NLE_ACCESS_DENIED);

	HRESULT hr = m_pProperties->GetString(keyName, szValue, size);
  
	DoneGetProperty(keyName);

	return hr;
}

HRESULT __stdcall CNLEService::SetDouble(LPCWSTR keyName, DOUBLE dValue)
{
	return this->Set(keyName, (double&)dValue);
}

HRESULT __stdcall CNLEService::GetDouble(LPCWSTR keyName, DOUBLE *pdValue)
{
	_ASSERT(m_pProperties);
	RETURN_IF_FAILED(WillGetProperty(keyName), E_NLE_ACCESS_DENIED);

	HRESULT hr = m_pProperties->GetDouble(keyName, pdValue);

	DoneGetProperty(keyName);

	return hr;
}

HRESULT __stdcall CNLEService::SetRect(LPCWSTR keyName, const NLERect &rcValue)
{
	return this->Set(keyName, (NLERect&)rcValue);
}

HRESULT __stdcall CNLEService::SetRectF(LPCWSTR keyName, const NLERectF& rcValue)
{
	return this->Set(keyName, (NLERectF&)rcValue);
}

HRESULT __stdcall CNLEService::GetRect(LPCWSTR keyName, NLERect *pNLERectValue)
{
	_ASSERT(m_pProperties);
	RETURN_IF_FAILED(WillGetProperty(keyName), E_NLE_ACCESS_DENIED);

	HRESULT hr = m_pProperties->GetRect(keyName, pNLERectValue); 
	
	DoneGetProperty(keyName);

	return hr;
}

HRESULT __stdcall CNLEService::GetRectF(LPCWSTR keyName, NLERectF *pNLERectFValue)
{
	_ASSERT(m_pProperties);
	RETURN_IF_FAILED(WillGetProperty(keyName), E_NLE_ACCESS_DENIED);

	HRESULT hr = m_pProperties->GetRectF(keyName, pNLERectFValue);

	DoneGetProperty(keyName);
	return hr;
}

HRESULT __stdcall CNLEService::SetPoint(LPCWSTR keyName, const NLEPoint& ptValue)
{
	return this->Set(keyName, (NLEPoint&)ptValue);
}

HRESULT __stdcall CNLEService::SetPointF(LPCWSTR keyName, const NLEPointF& ptValue)
{
	return this->Set(keyName, (NLEPointF&)ptValue);
}

HRESULT __stdcall CNLEService::GetPoint(LPCWSTR keyName, NLEPoint *pPointValue)
{
	_ASSERT(m_pProperties);
	RETURN_IF_FAILED(WillGetProperty(keyName), E_NLE_ACCESS_DENIED);

	HRESULT hr = m_pProperties->GetPoint(keyName, pPointValue);

	DoneGetProperty(keyName);

	return hr;
}

HRESULT __stdcall CNLEService::GetPointF(LPCWSTR keyName, NLEPointF *pPointValue)
{
	_ASSERT(m_pProperties);
	RETURN_IF_FAILED(WillGetProperty(keyName), E_NLE_ACCESS_DENIED);

	HRESULT hr = m_pProperties->GetPointF(keyName, pPointValue);

	DoneGetProperty(keyName);

	return hr;
}

HRESULT __stdcall CNLEService::SetSize(LPCWSTR keyName, const NLESize& oSize)
{
	return this->Set(keyName, (NLESize&)oSize);
}

HRESULT __stdcall CNLEService::SetSizeF(LPCWSTR keyName, const NLESizeF& oSize)
{
	return this->Set(keyName, (NLESizeF&)oSize);
}

HRESULT __stdcall CNLEService::GetSize(LPCWSTR keyName, NLESize *pSize)
{
	_ASSERT(m_pProperties);
	RETURN_IF_FAILED(WillGetProperty(keyName), E_NLE_ACCESS_DENIED);

	HRESULT hr = m_pProperties->GetSize(keyName, pSize);

	DoneGetProperty(keyName);

 	return hr;
}

HRESULT __stdcall CNLEService::GetSizeF(LPCWSTR keyName, NLESizeF *pSize)
{
	_ASSERT(m_pProperties);
	RETURN_IF_FAILED(WillGetProperty(keyName), E_NLE_ACCESS_DENIED);

	HRESULT hr = m_pProperties->GetSizeF(keyName, pSize);  

	DoneGetProperty(keyName);

	return hr;
}

HRESULT __stdcall CNLEService::SetRange(LPCWSTR keyName, const NLERange& range)
{
	_ASSERT(m_pProperties);
	HRESULT hr = E_FAIL;
	CNLEValue val;
	val.Set(range);
	RETURN_IF_FAILED(WillSetProperty(keyName, val), E_NLE_ACCESS_DENIED);
	if(SUCCEEDED(hr = this->Set(keyName, range) ) )	
	{
		DoneSetProperty(keyName);
	}
	return hr;
}

HRESULT __stdcall CNLEService::GetRange(LPCWSTR keyName, NLERange *pRange)
{
	_ASSERT(m_pProperties);
	RETURN_IF_FAILED(WillGetProperty(keyName), E_NLE_ACCESS_DENIED);

	HRESULT hr = E_FAIL;
	
	if (SUCCEEDED(hr = m_pProperties->GetRange(keyName, pRange)))
	{
		DoneGetProperty(keyName);
	}
	return hr;
}


HRESULT __stdcall CNLEService::SetBlob(LPCWSTR keyName, const NLEBlob& oBlob)
{	
	HRESULT hr = E_FAIL;
	CNLEValue val;
	val.Set(oBlob);
	RETURN_IF_FAILED(WillSetProperty(keyName, val), E_NLE_ACCESS_DENIED);
	if (SUCCEEDED(hr = this->Set(keyName, val)))
	{
		DoneSetProperty(keyName);
	}
	return hr;
}

HRESULT __stdcall CNLEService::GetBlob(LPCWSTR keyName, NLEBlob* pBlob)
{
	_ASSERT(m_pProperties);
	RETURN_IF_FAILED(WillGetProperty(keyName), E_NLE_ACCESS_DENIED);

	HRESULT hr = E_FAIL;
	
	hr = m_pProperties->GetBlob(keyName, pBlob);
   
	DoneGetProperty(keyName);

	return hr;
}

HRESULT __stdcall CNLEService::SetInterface(LPCWSTR keyName, IUnknown *pUnk)
{
	CNLEPtr<IUnknown> ptr(pUnk);
	return this->Set(keyName, ptr);
}

HRESULT __stdcall CNLEService::GetInterface(LPCWSTR keyName, IUnknown **ppUnk)
{
	_ASSERT(m_pProperties);
	RETURN_IF_FAILED(WillGetProperty(keyName), E_NLE_ACCESS_DENIED);

	HRESULT hr = m_pProperties->GetInterface(keyName,ppUnk);

	DoneGetProperty(keyName);

	return hr;
}

HRESULT __stdcall CNLEService::SetGUID(LPCWSTR keyName, const GUID& guid)
{
	return this->Set(keyName, (GUID&)guid);
}

HRESULT __stdcall CNLEService::GetGUID(LPCWSTR keyName, GUID *pGuid)
{
	_ASSERT(m_pProperties);
	RETURN_IF_FAILED(WillGetProperty(keyName), E_NLE_ACCESS_DENIED);

	HRESULT hr = m_pProperties->GetGUID(keyName, pGuid);  

	DoneGetProperty(keyName);

	return hr;
}

HRESULT __stdcall CNLEService::SetRational(LPCWSTR keyName, const NLERational& ral)
{
	return this->Set(keyName, (NLERational&)ral);
}

HRESULT __stdcall CNLEService::GetRational(LPCWSTR keyName, NLERational *pRal)
{
	_ASSERT(m_pProperties);
	RETURN_IF_FAILED(WillGetProperty(keyName), E_NLE_ACCESS_DENIED);

	HRESULT hr = m_pProperties->GetRational(keyName, pRal);   

	DoneGetProperty(keyName);

	return hr;
}

HRESULT __stdcall CNLEService::CopyValue(INLEProperties* pDst, LPCWSTR keyNameFrom, LPCWSTR keyNameTo /*= NULL*/)
{
	_ASSERT(m_pProperties);
	HRESULT hr = m_pProperties->CopyValue(pDst, keyNameFrom, keyNameTo);
	return hr;
}

HRESULT __stdcall CNLEService::CopyValues(INLEProperties* pDst, ARRAY_WSTR arrKeyNames)
{
	_ASSERT(m_pProperties);
	HRESULT hr = m_pProperties->CopyValues(pDst, arrKeyNames);
	return hr;
}

HRESULT __stdcall CNLEService::CopyValues(INLEProperties* pDst, ARRAY_PAIRWSTR arrKeyNames)
{
	_ASSERT(m_pProperties);
	HRESULT hr = m_pProperties->CopyValues(pDst, arrKeyNames);
	return hr;
}

HRESULT __stdcall CNLEService::CopyAll(INLEProperties* pDst)
{
	_ASSERT(m_pProperties);
	HRESULT hr = m_pProperties->CopyAll(pDst);
	return hr;
}

//////////////////////////////////////////////////////////////////////////
HRESULT __stdcall CNLEService::Delete(LPCWSTR keyName)
{
	_ASSERT(m_pProperties);
	HRESULT hr = m_pProperties->Delete(keyName);
	if (SUCCEEDED(hr) )
	{
		NLEPropertyListeners lstListenser = GetListeners(keyName);
		if (lstListenser.size() > 0)
		{
			NLEPropertyListeners::reverse_iterator it = lstListenser.rbegin();
			while (it != lstListenser.rend() )
			{
				PropSetingClass* pObj = it->pObj;
				pObj->NonDelegatingRelease();
				++it;
			}
		}
	}
	return hr;
}

HRESULT __stdcall CNLEService::DeleteAll()
{
	_ASSERT(m_pProperties);
	HRESULT hr = m_pProperties->DeleteAll();
	return hr;
}

HRESULT __stdcall CNLEService::Dump(INLELogger* pLog /*= NULL*/)
{
	_ASSERT(m_pProperties);
	HRESULT hr = m_pProperties->Dump(pLog);
	return hr;
}

HRESULT __stdcall CNLEService::GetKey(int nIdex, LPCWSTR *ppwszKeyName)
{
	_ASSERT(m_pProperties);
	HRESULT hr = m_pProperties->GetKey(nIdex, ppwszKeyName);
	return hr;
}



HRESULT CNLEService::SetParent(IUnknown *pParent)
{	
	m_pParent = pParent;

	return S_OK;
}

IUnknown* CNLEService::GetParent()
{	
	return m_pParent;
}




HRESULT __stdcall CNLEService::GetProperties(INLEProperties **ppProperties)
{
	if (ppProperties)
	{
		_ASSERT(m_pProperties);
		HRESULT hr = ( (INLEService *)this)->QueryInterface(IID_INLEProperties, (LPVOID *)ppProperties);
		_ASSERT(SUCCEEDED(hr) && *ppProperties);
		return hr;
	}
	else
	{
		return E_INVALIDARG;
	}
	return S_OK;
}


HRESULT __stdcall CNLEService::GetReconstructProperties(INLEProperties **ppProperties)
{
	if (ppProperties)
	{
		_ASSERT(m_pProperties);
		HRESULT hr = m_pProperties->QueryInterface(IID_INLEProperties, (LPVOID *)ppProperties);
		_ASSERT(SUCCEEDED(hr) && *ppProperties);
		return hr;
	}
	else
	{
		return E_INVALIDARG;
	}
	return S_OK;
}

using namespace NLEKey::Base;
using namespace NLEKey::Render;
///优先属性表
static const LPCWSTR arrPreferKey[] =
{
	NLEKey::Timelapse::kFileList,
	NLEKey::Timelapse::kSequenceMode,
	NLEKey::Timelapse::kImageType,
	NLEKey::Timelapse::kFramerate,
	NLEKey::Timelapse::kSize,
	NLEKey::Timelapse::kCache,
    NLEKey::SourceRender::kSourceRender_CurrentAudioStreamIndex,
    NLEKey::SourceRender::kSourceRender_CurrentVideoStreamIndex,

	kStrFileFullPath,			// 1、设置文件路径之后，会有一套按source出来的参数
	kRender_Scale,				// 2、再改变变速
	kRender_Framerate,			// 3、再改变帧率{2和3的位置可以互换}
	kRender_RangFrameNumber,	// 4、设定渲染范围
	kRender_Position,			// 5、设定在父片段里的位置

    NLEKey::Freeze::kFreeze_Position,
    NLEKey::Freeze::kFreeze_Scale,
    NLEKey::Freeze::kFreeze_Enabled,
};
static const int nKeyCount = sizeof(arrPreferKey) / sizeof(arrPreferKey[0]);

HRESULT __stdcall CNLEService::SetReconstructProperties(INLEProperties *pProperties)
{
	HRESULT hr = E_FAIL;
	NLEPropDecorator  pThisProperties;
	hr = GetProperties(&pThisProperties); CHECK(hr);
			
	NLEComPtr<INLEProperties> pPropCopy = NLECommon::CreateProperties();
	pProperties->CopyAll(pPropCopy);

	//先设置这几个属性，因它们有先后顺序
	for (int i = 0; i < nKeyCount; i++)
	{
		LPCWSTR pKeyName = arrPreferKey[i];
		if (SUCCEEDED(hr = pPropCopy->CopyValue(pThisProperties, pKeyName)))
		{
			pPropCopy->Delete(pKeyName);
		}
	}

	hr = pPropCopy->CopyAll(pThisProperties); CHECK(hr);

	return hr;
}
