/**
 * Copyright (c) 2016 Wondershare.inc
 * All rights reserved.
 *
 *@file  NLESelfPropComPtr.h
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

using namespace NLEKey::Render;

#define OrgRender(prop)  ((NLESelfPropComPtr)(prop) )

class NLESelfPropComPtr :public NLEComPtr<INLEProperties>
{
public:
	NLESelfPropComPtr()
	{
	}
	NLESelfPropComPtr(INLEProperties* p) :NLEComPtr(p)
	{
	}

	NLESelfPropComPtr(NLEComPtr<INLEProperties> p) :NLEComPtr(p)
	{
	}
	~NLESelfPropComPtr()
	{

	}

public:

	NLESelfPropComPtr* operator->() const throw()
	{
		return  (NLESelfPropComPtr*)this;
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

	__inline HRESULT  SetPoint(LPCWSTR keyName, const NLEPoint& ptValue)
	{
		assert(p); return p->SetPoint(keyName, ptValue);
	}

	__inline HRESULT  GetPoint(LPCWSTR keyName, NLEPoint *pPointValue)
	{
		assert(p); return p->GetPoint(keyName, pPointValue);
	}

	__inline HRESULT  SetRange(LPCWSTR keyName, const NLERange& ptValue)
	{
		assert(p); return p->SetRange(keyName, ptValue);
	}

	__inline HRESULT  GetRange(LPCWSTR keyName, NLERange *pRangeValue)
	{
		assert(p); return p->GetRange(keyName, pRangeValue);
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

	/**
	*@brief 复制键组的值，只要有一个不存在或不能Copy都会返回失败
	*@param arrKeyNames 原型std::vector<LPCWSTR>，要复制的键组。快速使用方式：{ kName1, kName2, kName3 }
	*/
	HRESULT CopyValues(INLEProperties* pDst, ARRAY_WSTR arrKeyNames)
	{
		assert(p); return p->CopyValues(pDst, arrKeyNames);
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
};