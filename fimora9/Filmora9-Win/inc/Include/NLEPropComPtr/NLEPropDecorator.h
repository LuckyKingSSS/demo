/**
 * Copyright (c) 2016 Wondershare.inc
 * All rights reserved.
 *
 *@file  NLEPropDecorator.h
 *@brief   INLEProperties智能指针
 *@version 1.0
 *@author  wangyx
 *@date    4/8/2016 17:19
 */

#pragma once

#include "NLEComPtr.h"
#include "INLEProperties.h"
#include "NLEKey.h"
#include <assert.h>

class NLEPropDecorator :public NLEComPtr<INLEProperties>
{
public:
	typedef struct PropHelper
	{
		NLEPropDecorator* _pHandler;
		LPCWSTR _kn;
		CNLEValue _val;
		PropHelper()
		{
			_kn = NULL;
			_pHandler = NULL;
		}
		PropHelper(const PropHelper &hlp)
		{
			_kn = hlp._kn;
			_pHandler = hlp._pHandler;
			_val = hlp._val;
		}	
	public:
		PropHelper& operator=(PropHelper hlp)
		{
#ifdef WIN32
			HRESULT hr = this->Assign<PropHelper>(hlp);
#else
            HRESULT hr = this->Assign(hlp);
#endif
			return *this;
		}

		template<typename T>
		T& operator=(T &v)
		{
#ifdef WIN32
			HRESULT hr = this->Assign<T>(v);
#else
			HRESULT hr = this->Assign(v);
#endif
			return v;
		}

		template<typename T> operator T()
		{
			CNLEValue nleVal;
			NLEPropDecorator* pPropObj = (NLEPropDecorator*)_pHandler;
			HRESULT hr = pPropObj->GetValue(_kn, nleVal);
			return nleVal;
		}

	protected:
		template<typename T>
		HRESULT Assign(T &v)
		{
			CNLEValue nleVal;
			nleVal.Set<T>(v);
			NLEPropDecorator* pPropObj = (NLEPropDecorator*)_pHandler;
			HRESULT hr = pPropObj->SetValue(_kn, nleVal); CHECK(hr);
			return hr;
		}

#ifdef WIN32
		template<>
      HRESULT Assign<WCHAR*>(WCHAR* &v)
#else
        HRESULT Assign(WCHAR* &v)
#endif
		{
			std::wstring str = v;
			CNLEValue nleVal;
			nleVal.Set(str);
			NLEPropDecorator* pPropObj = (NLEPropDecorator*)_pHandler;
			HRESULT hr = pPropObj->SetValue(_kn, nleVal); CHECK(hr);
			return hr;
		}

#ifdef WIN32
		template<>
      HRESULT Assign<PropHelper>(PropHelper &hlp)
#else
		  HRESULT Assign(PropHelper &hlp)
#endif
		{
			CNLEValue hlpVal;
			NLEPropDecorator* pThat = (NLEPropDecorator*)hlp._pHandler;
			NLEPropDecorator* pThis = (NLEPropDecorator*)_pHandler;
			NLE_ASSERT(pThis && pThat);
			NLE_ASSERT(hlp._kn);

			HRESULT hr = S_OK;
			if (SUCCEEDED(hr = pThat->GetValue(hlp._kn, hlpVal)))
			{
				hr = pThis->SetValue(_kn, hlpVal); CHECK(hr);
			}
			CHECK(hr);
			return hr;
		}		
	}PropHelper;
public:
	NLEPropDecorator()
	{
		Init();
	}
	NLEPropDecorator(INLEProperties* p) :NLEComPtr(p)
	{
		Init();
	}

	NLEPropDecorator(NLEComPtr<INLEProperties> p) :NLEComPtr(p)
	{
		Init();
	}
	~NLEPropDecorator()
	{

	}

public:

	NLEPropDecorator* operator->() const throw()
	{
		return  (NLEPropDecorator*)this;
	}

	__inline PropHelper operator[](LPCWSTR keyName)
	{
		PropHelper hlp;
		hlp._kn = keyName;
		hlp._pHandler = this;
		return hlp;
	}

public:
	__inline HRESULT  SetValue(LPCWSTR keyName, CNLEValue& val)
	{
		assert(p); return p->SetValue(keyName, val);
	}

	__inline HRESULT  GetValue(LPCWSTR keyName, CNLEValue& val)
	{
		assert(p); return p->GetValue(keyName, val);
	}

	__inline HRESULT GetCount(INT *pCount)
	{
		assert(p); return p->GetCount(pCount);
	}

	__inline HRESULT  SetInt(LPCWSTR keyName, INT nValue)
	{
		assert(p); return p->SetInt(keyName, nValue);
	}

	__inline HRESULT  GetInt(LPCWSTR keyName, INT *pValue)
	{
		assert(p); return p->GetInt(keyName, pValue);
	}

	__inline HRESULT  SetInt64(LPCWSTR keyName, INT64 nValue)
	{
		assert(p); return p->SetInt64(keyName, nValue);
	}

	__inline HRESULT  GetInt64(LPCWSTR keyName, INT64 *pValue)
	{
		assert(p); return p->GetInt64(keyName, pValue);
	}

	__inline HRESULT  SetString(LPCWSTR keyName, LPCWSTR szValue)
	{
		assert(p); return p->SetString(keyName, szValue);
	}

	__inline HRESULT  GetString(LPCWSTR keyName, LPWSTR szValue, int size)
	{
		assert(p); return p->GetString(keyName, szValue, size);
	}

	__inline HRESULT  SetDouble(LPCWSTR keyName, DOUBLE dValue)
	{
		assert(p); return p->SetDouble(keyName, dValue);
	}

	__inline HRESULT  GetDouble(LPCWSTR keyName, DOUBLE *pdValue)
	{
		assert(p); return p->GetDouble(keyName, pdValue);
	}

	__inline HRESULT  SetRect(LPCWSTR keyName, NLERect rcValue)
	{
		assert(p); return p->SetRect(keyName, rcValue);
	}

	__inline HRESULT  GetRect(LPCWSTR keyName, NLERect *pNLERectValue)
	{
		assert(p); return p->GetRect(keyName, pNLERectValue);
	}

	__inline HRESULT  SetRectF(LPCWSTR keyName, const NLERectF& rcValue)
	{
		assert(p); return p->SetRectF(keyName, rcValue);
	}

	__inline HRESULT  GetRectF(LPCWSTR keyName, NLERectF *pNLERectFValue)
	{
		assert(p); return p->GetRectF(keyName, pNLERectFValue);
	}

	__inline HRESULT  SetRange(LPCWSTR keyName, const NLERange& ptValue)
	{
		assert(p); return p->SetRange(keyName, ptValue);
	}

	__inline HRESULT  GetRange(LPCWSTR keyName, NLERange *pRangeValue)
	{
		assert(p); return p->GetRange(keyName, pRangeValue);
	}

	__inline HRESULT  SetPoint(LPCWSTR keyName, const NLEPoint& ptValue)
	{
		assert(p); return p->SetPoint(keyName, ptValue);
	}

	__inline HRESULT  GetPoint(LPCWSTR keyName, NLEPoint *pPointValue)
	{
		assert(p); return p->GetPoint(keyName, pPointValue);
	}

	__inline HRESULT  SetPointF(LPCWSTR keyName, const NLEPointF& ptValue)
	{
		assert(p); return p->SetPointF(keyName, ptValue);
	}

	__inline HRESULT  GetPointF(LPCWSTR keyName, NLEPointF *pPointValue)
	{
		assert(p); return p->GetPointF(keyName, pPointValue);
	}

	__inline HRESULT  SetSize(LPCWSTR keyName, const NLESize& oSize)
	{
		assert(p); return p->SetSize(keyName, oSize);
	}

	__inline HRESULT  GetSize(LPCWSTR keyName, NLESize *pSize)
	{
		assert(p); return p->GetSize(keyName, pSize);
	}

	__inline HRESULT  SetSizeF(LPCWSTR keyName, const NLESizeF& oSize)
	{
		assert(p); return p->SetSizeF(keyName, oSize);
	}

	__inline HRESULT  GetSizeF(LPCWSTR keyName, NLESizeF *pSize)
	{
		assert(p); return p->GetSizeF(keyName, pSize);
	}

	__inline HRESULT SetColorF(LPCWSTR keyName, NLEColorF color)
	{
		assert(p); return p->SetColorF(keyName, color);
	}

	__inline HRESULT GetColorF(LPCWSTR keyName, NLEColorF *pColor)
	{
		assert(p); return p->GetColorF(keyName, pColor);
	}

	__inline HRESULT  SetBlob(LPCWSTR keyName, const NLEBlob& oBlob)
	{
		assert(p); return p->SetBlob(keyName, oBlob);
	}

	__inline HRESULT  GetBlob(LPCWSTR keyName, NLEBlob *pBlob)
	{
		assert(p); return p->GetBlob(keyName, pBlob);
	}


	__inline HRESULT  SetInterface(LPCWSTR keyName, IUnknown *pUnk)
	{
		assert(p); return p->SetInterface(keyName, pUnk);
	}

	__inline HRESULT  GetInterface(LPCWSTR keyName, IUnknown **ppUnk)
	{
		assert(p); return p->GetInterface(keyName, ppUnk);
	}

	__inline HRESULT  SetGUID(LPCWSTR keyName, const GUID& guid)
	{
		assert(p); return p->SetGUID(keyName, guid);
	}

	__inline HRESULT  GetGUID(LPCWSTR keyName, GUID *pGuid)
	{
		assert(p); return p->GetGUID(keyName, pGuid);
	}

	__inline HRESULT  SetRational(LPCWSTR keyName, const NLERational& ral)
	{
		assert(p); return p->SetRational(keyName, ral);
	}

	__inline HRESULT  GetRational(LPCWSTR keyName, NLERational *pRal)
	{
		assert(p); return p->GetRational(keyName, pRal);
	}

	/***@brief 复制对应键的值*/
	__inline HRESULT CopyValue(INLEProperties* pDst, LPCWSTR keyName)
	{
		assert(p); return p->CopyValue(pDst, keyName);
	}

	__inline HRESULT CopyValue(INLEProperties* pDst, LPCWSTR keyNameFrom, LPCWSTR keyNameTo)
	{
		assert(p); return p->CopyValue(pDst, keyNameFrom, keyNameTo);
	}

	/**
	*@brief 复制键组的值，只要有一个不存在或不能Copy都会返回失败
	*@param arrKeyNames 原型std::vector<LPCWSTR>，要复制的键组。快速使用方式：{ kName1, kName2, kName3 }
	*/
	__inline HRESULT CopyValues(INLEProperties* pDst, ARRAY_WSTR arrKeyNames)
	{
		assert(p); return p->CopyValues(pDst, arrKeyNames);
	}

	__inline HRESULT CopyValues(INLEProperties* pDst, ARRAY_PAIRWSTR arrKeyNames)
	{
		assert(p); return p->CopyValues(pDst, arrKeyNames);
	}

	HRESULT CopyAll(INLEProperties* pDst)
	{
		assert(p); return p->CopyAll(pDst);
	}

	__inline HRESULT __stdcall Delete(LPCWSTR keyName)
	{
		assert(p); return p->Delete(keyName);
	}

	__inline HRESULT __stdcall DeleteAll()
	{
		assert(p); return p->DeleteAll();
	}

	__inline HRESULT __stdcall Dump(INLELogger* pLog = NULL)
	{
		assert(p); return p->Dump(pLog);
	}

	__inline HRESULT __stdcall GetKey(int nIdex, LPCWSTR *ppwszKeyName)
	{
		assert(p); return p->GetKey(nIdex, ppwszKeyName);
	}

	__inline INT GetRenderBytesPerSecond()
	{
		if (m_nBytesPerSecond > 0)
		{
			return m_nBytesPerSecond;
		}
		INT nSamplereate = 0;
		this->GetInt(NLEKey::Render::kRender_SampleRate, &nSamplereate); NLE_ASSERT(nSamplereate > 0);
		INT nChannels = 0;
		this->GetInt(NLEKey::Render::kRender_Channels, &nChannels); NLE_ASSERT(nChannels);
		INT nBitsPerSample = 0;
		this->GetInt(NLEKey::Render::kRender_BitsPerSample, &nBitsPerSample); NLE_ASSERT(nBitsPerSample);
		m_nBytesPerSecond = nSamplereate * nChannels * nBitsPerSample / 8;
		return GetRenderBytesPerSecond();
	}
private:
	void Init()
	{
		m_nBytesPerSecond = 0;
	}
	INT m_nBytesPerSecond;
};
