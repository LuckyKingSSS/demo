
/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  NLERepositoryInfo.h
 *@brief   封装音频FilterInfo的存储信息                             
 *@version 1.0                          
 *@author  wangyx                       
 *@date    7/27/2016 14:16
 */

#ifndef _H_NLEREPOSITORYINFOCOMPTR_H_
#define _H_NLEREPOSITORYINFOCOMPTR_H_

#include "NLEComPtr.h"
#include "INLEProperties.h"
#include "NLEKey.h"
#include <assert.h>
#include "NLEType_c.h"

#include "NLEFilterInfoComPtr.h"

class NLEFilterRepositoryInfoPtr :public NLEComPtr<INLEProperties>
{
public:
	NLEFilterRepositoryInfoPtr(){}

	NLEFilterRepositoryInfoPtr(INLEProperties* p) :NLEComPtr(p) {	}

	NLEFilterRepositoryInfoPtr(NLEComPtr<INLEProperties> p) :NLEComPtr(p) {	}
	~NLEFilterRepositoryInfoPtr() {	}

public:

	NLEFilterRepositoryInfoPtr* operator->() const throw()
	{
		assert(p != NULL);
		return  (NLEFilterRepositoryInfoPtr*)this;
	}

public:
	LPCWSTR GetDirectoryKeyName(RepositoryType type)
	{
		LPCWSTR lpszKeyName = NULL;
		switch (type)
		{
		case RepositoryType_Mask:
		case RepositoryType_VideoFilter:
			lpszKeyName = NLEKey::Repository::kEffectEngineDir;
			break;
		case RepositoryType_AudioFilter:
			lpszKeyName = NLEKey::Repository::kAudioPluginsDir;
			break;
		default:
			_ASSERT(FALSE); break;
		}
		return lpszKeyName;
	}
public:
	//动态库等运行时资源的主目录
	__inline HRESULT GetRuntimeDirectory(RepositoryType type,LPWSTR pszBuffer, INT nBufferSize = 256)
	{ 
		LPCWSTR lpszKeyName = GetDirectoryKeyName(type); _ASSERT(p);
		return p->GetString(lpszKeyName, pszBuffer, nBufferSize);
	}
	__inline HRESULT SetRuntimeDirectory(LPCWSTR pszDirectory, RepositoryType type)
	{
		LPCWSTR lpszKeyName = GetDirectoryKeyName(type); _ASSERT(p);
		return p->SetString(lpszKeyName, pszDirectory);
	}
	
	__inline HRESULT SetFilterCount(const INT nCount)
	{
		_ASSERT(p);
		return p->SetInt(NLEKey::Filter::kFilterCount, nCount);
	}

	__inline const INT GetFilterCount()
	{
		_ASSERT(p); INT nCount = 0;
		HRESULT hr = p->GetInt(NLEKey::Filter::kFilterCount, &nCount); CHECK(hr);
		return nCount;
	}

	__inline HRESULT AddFilterInfo(LPCWSTR pszFilterID, NLEFilterInfoComPtr pFilterInfo)
	{
		_ASSERT(p); _ASSERT(pszFilterID);
		return p->SetInterface(pszFilterID, pFilterInfo);
	}

	__inline NLEFilterInfoComPtr GetFilterInfo(LPCWSTR pszFilterGUID)
	{
		_ASSERT(p); _ASSERT(pszFilterGUID);
		NLEComPtr<IUnknown> pUnk = NULL; NLEComPtr<INLEProperties> pProp = NULL;
		HRESULT hr = p->GetInterface(pszFilterGUID, &pUnk); CHECK(hr);
		hr = pUnk->QueryInterface(IID_INLEProperties, (LPVOID *)&pProp); CHECK(hr);
		return pProp;
	}

	__inline NLEFilterInfoComPtr GetFilterInfo(const int nIndex)
	{
		NLEComPtr<INLEProperties> pProp(this->p);
		LPCWSTR pGUIDKey = NULL;
		if (nIndex >= GetFilterCount() )
		{
			return NULL;
		}
		HRESULT hr = pProp->GetKey(nIndex, &pGUIDKey); CHECK(hr);
		return this->GetFilterInfo(pGUIDKey);
	}
};

#endif //_H_NLEREPOSITORYINFOCOMPTR_H_