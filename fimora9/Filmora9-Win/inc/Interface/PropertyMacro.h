#pragma once

#include "objbase.h"

#ifndef _H_PROPERTYMACRO_H_
#define _H_PROPERTYMACRO_H_

typedef scombase::CUnknown PropSetingClass;

typedef HRESULT(PropSetingClass::*FunctionSetInt)(LPCWSTR keyName, int iValue);
typedef HRESULT(PropSetingClass::*FunctionSetInt64)(LPCWSTR keyName,  INT64 llValue);
typedef HRESULT(PropSetingClass::*FunctionSetDouble)(LPCWSTR keyName, DOUBLE iValue);
typedef HRESULT(PropSetingClass::*FunctionSetString)(LPCWSTR keyName, LPCWSTR pString);
typedef HRESULT(PropSetingClass::*FunctionSetPoint)(LPCWSTR keyName, const NLEPoint& point);
typedef HRESULT(PropSetingClass::*FunctionSetPointF)(LPCWSTR keyName, const NLEPointF& point);
typedef HRESULT(PropSetingClass::*FunctionSetSize)(LPCWSTR keyName, const NLESize& size);
typedef HRESULT(PropSetingClass::*FunctionSetSizeF)(LPCWSTR keyName, const NLESizeF& sizeF);
typedef HRESULT(PropSetingClass::*FunctionSetRect)(LPCWSTR keyName, const NLERect& rect);
typedef HRESULT(PropSetingClass::*FunctionSetRectF)(LPCWSTR keyName, const NLERectF& rect);
typedef HRESULT(PropSetingClass::*FunctionSetData)(LPCWSTR keyName, LPVOID pData, UINT uSize, BOOL bCopy);
typedef HRESULT(PropSetingClass::*FunctionSetUnknown)(LPCWSTR keyName, IUnknown *pUnk);
typedef HRESULT(PropSetingClass::*FunctionSetGUID)(LPCWSTR keyName, const GUID& guid);
typedef HRESULT(PropSetingClass::*FunctionSetRational)(LPCWSTR keyName, const NLERational& rational);
typedef HRESULT(PropSetingClass::*FunctionSetValue)(LPCWSTR keyName, CNLEValue& iValue);
typedef HRESULT(PropSetingClass::*AnyFunction)(LPCWSTR keyName, void * iValue);

typedef struct ProperyListener
{
	LPCWSTR keyName;
	AnyFunction listener;
	PropSetingClass* pObj;
	ProperyListener(LPCWSTR keyName, AnyFunction listener, PropSetingClass* pObj = NULL)
	{
		this->keyName = keyName;
		this->listener = listener;
		this->pObj = pObj;
	}
	~ProperyListener()
	{
		this->keyName = NULL;
		this->listener = NULL;
		this->pObj = NULL;
	}
}NLEPropetyListener;


#include <vector>
typedef std::vector<NLEPropetyListener> NLEPropertyListeners;


#define DECLARE_IMP_SET \
template<typename T> HRESULT __stdcall Set(LPCWSTR keyName, T& v) \
{ \
	if(NULL == keyName) return E_INVALIDARG; \
	CNLEValue val; \
	val.Set(v); \
	return this->SetValue(keyName, val); \
}

#define DECLARE_IMP_GET \
template<typename T> HRESULT __stdcall Get(LPCWSTR keyName, T& v)  \
{ \
	if(NULL == keyName) return E_INVALIDARG; \
	CNLEValue val; \
	HRESULT hr = this->GetValue(keyName, val); \
	if (SUCCEEDED(hr)) \
				{	\
		val.Get(v); \
				} \
	return hr; \
}

#define DECLARE_IMP_DEL \
template<typename T> HRESULT __stdcall Del(LPCWSTR keyName)  \
{	\
	return this->DelValue(keyName);	\
}

#define ATTRIBUTE_SET(n, t, k, v)	(n)->Set<t>(L#k, v);
#define ATTRIBUTE_GET(n, t, k, v)	(n)->Get<t>(L#k, v);
#define ATTRIBUTE_DEL(n, t, k)		(n)->Del<t>(L#k);

#endif //_H_PROPERTYMACRO_H_
