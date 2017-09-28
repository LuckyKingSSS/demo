#pragma once

#include "NLEComPtr.h"
#include "INLEProperties.h"
#include "NLEKey.h"
#include <assert.h>
#include "NLEMacroDefine.h"
#include "NLEComError.h"
#include "INLEVideoFilter.h"

using namespace NLEKey::Filter;


class NLEFilterParamComPtr :public NLEComPtr<INLEProperties>
{
public:
	__inline HRESULT SetFriendName(LPCWSTR lpwszFriendName) { return p->SetString(Property::kFriendName, lpwszFriendName); }
	__inline HRESULT GetFriendName(LPWSTR pwszBuffer, int nSize = 38) { return p->GetString(Property::kFriendName, pwszBuffer, nSize); }

	__inline HRESULT SetVariantName(LPCWSTR lpwszVarName) { return p->SetString(Property::kVariantName, lpwszVarName); }
	__inline HRESULT GetVariantName(LPWSTR pwszBuffer, int nSize = 38) { return p->GetString(Property::kVariantName, pwszBuffer, nSize); }
	
	__inline NLEFilterPropertyType GetType() { INT nType = 0; p->GetInt(Property::kType, &nType);  return (NLEFilterPropertyType)(nType); }
	__inline HRESULT SetType(NLEFilterPropertyType type) { return p->SetInt(Property::kType, type);  }

	__inline HRESULT GetMinMax(CNLEValue *pMin, CNLEValue *pMax) 
	{
		HRESULT hr = E_NLE_UNEXPECTED;
		if (pMin) hr =  p->GetValue(Property::kValue_Min, *pMin);
		if (pMax) hr =  p->GetValue(Property::kValue_Max, *pMax);
		return hr;
	}

	__inline HRESULT SetMinMax(CNLEValue &valMin, CNLEValue &valMax)
	{		
		RETURN_IF_FAILED(p->SetValue(Property::kValue_Min, valMin), E_NLE_UNEXPECTED);
		return p->SetValue(Property::kValue_Max, valMax);
	}
	
	__inline HRESULT GetDefault(CNLEValue *pDefault)
	{		
		return  p->GetValue(Property::kValue_Default, *pDefault);
	}

	__inline HRESULT SetDefault(CNLEValue &valDefault)
	{
		return  p->SetValue(Property::kValue_Default, valDefault);
	}

	__inline HRESULT GetCurrent(CNLEValue *pCurrent)
	{
		return p->GetValue(Property::kValue_Current, *pCurrent);
	}

	__inline HRESULT SetCurrent(CNLEValue &valCurrent)
	{
		return p->SetValue(Property::kValue_Current, valCurrent);
	}
public:
	NLEFilterParamComPtr(){}

	NLEFilterParamComPtr(INLEProperties* p) :NLEComPtr(p) {	}

	NLEFilterParamComPtr(NLEComPtr<INLEProperties> p) :NLEComPtr(p) {	}
	~NLEFilterParamComPtr() {	}

public:

	NLEFilterParamComPtr* operator->() const throw()
	{
		assert(p != NULL);
		return  (NLEFilterParamComPtr*)this;
	}
};


class NLEFilterInfoComPtr :public NLEComPtr<INLEProperties>
{
public:
	__inline HRESULT SetFilterID(LPCWSTR lpwszFilterGUID) { return p->SetString(kFilterID, lpwszFilterGUID); }
	__inline HRESULT GetFilterID(LPWSTR pwszBuffer, int nSize = 38) { return p->GetString(kFilterID, pwszBuffer, nSize); }

	__inline HRESULT SetFriendName(LPCWSTR lpwszFriendName) { return p->SetString(kFilterFriendName, lpwszFriendName); }
	__inline HRESULT GetFriendName(LPWSTR pwszBuffer, int nSize = 38) { return p->GetString(kFilterFriendName, pwszBuffer, nSize); }

	__inline HRESULT SetLibName(LPCWSTR lpwszLibName) { return p->SetString(kFilterLibName, lpwszLibName); }
	__inline HRESULT GetLibName(LPWSTR pwszBuffer, int nSize = 38) { return p->GetString(kFilterLibName, pwszBuffer, nSize); }

	__inline HRESULT SetParamCount(INT nCount) { return p->SetInt(Property::kCount, nCount); }
	__inline INT	 GetParamCount() { INT nCount = 0; p->GetInt(Property::kCount, &nCount); return nCount; }

	__inline HRESULT AddParam(INT nIndex, NLEFilterParamComPtr pParam) { return p->SetInterface(Property::kParamsArray[nIndex], pParam);}
	__inline NLEFilterParamComPtr GetParam(INT nIndex)
	{
		_ASSERT(p);
		NLEComPtr<IUnknown> pUnk = NULL; NLEFilterParamComPtr pParam = NULL;
		HRESULT hr = p->GetInterface(Property::kParamsArray[nIndex],&pUnk); CHECK(hr);
		hr = pUnk->QueryInterface(IID_INLEProperties, (LPVOID *)&pParam); CHECK(hr);
		return pParam;
	}
	__inline NLEFilterParamComPtr GetParam(LPCWSTR pwszFirendNameIn)
	{
		_ASSERT(p);
		INT nCount = GetParamCount();
		for (int i = 0;i < nCount; i++)
		{
			NLEFilterParamComPtr pParam = this->GetParam( (INT)i);
			WCHAR pwszFriendName[100] = { 0 };
			pParam->GetFriendName(pwszFriendName, 100);
			if (wcscmp(pwszFirendNameIn, pwszFriendName) == 0)
			{
				return pParam;
			}
		}
		return NULL;
	}

public:
	NLEFilterInfoComPtr(){}

	NLEFilterInfoComPtr(INLEProperties* p) :NLEComPtr(p) {	}

	NLEFilterInfoComPtr(NLEComPtr<INLEProperties> p) :NLEComPtr(p) {	}
	~NLEFilterInfoComPtr() {	}

public:

	NLEFilterInfoComPtr* operator->() const throw()
	{
		assert(p != NULL);
		return  (NLEFilterInfoComPtr*)this;
	}
};