/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  NLEBasePropComPtr.h
 *@brief                                
 *@version 1.0                          
 *@author  wangyx                       
 *@date    4/8/2016 11:48
 */

#include "NLEComPtr.h"
#include "INLEProperties.h"
#include "NLEKey.h"
#include <assert.h>

using namespace NLEKey::Base;

class NLEBasePropComptr :public NLEComPtr<INLEProperties>
{
public:
	NLEBasePropComptr(){}

	NLEBasePropComptr(INLEProperties* p) :NLEComPtr(p) {	}

	NLEBasePropComptr(NLEComPtr<INLEProperties> p) :NLEComPtr(p) {	}
	~NLEBasePropComptr() {	}

public:

	NLEBasePropComptr* operator->() const throw()
	{
		assert(p != NULL);
		return  (NLEBasePropComptr*)this;
	}

public:
	__inline HRESULT SetLevel(INT nLevel) { return p->SetInt(kLevel, nLevel);}

	__inline HRESULT GetLevel(INT *pLevel) { return p->GetInt(kLevel, pLevel); }

	__inline HRESULT SetName(LPCWSTR strName) { return p->SetString(kName, strName); }
	
	__inline HRESULT GetName(LPWSTR strName, int size) { return p->GetString(kName, strName, size); }

	__inline HRESULT SetPosition(int pos) { return p->SetInt(NLEKey::Render::kRender_Position, pos); }
	
	__inline HRESULT GetPosition(int * pPos) { return p->GetInt(NLEKey::Render::kRender_Position, pPos); }

    /**@brief 若为-1则表示一直都存在*/
    __inline HRESULT SetFrameCount(INT  nFrameCount) { return p->SetInt(NLEKey::Render::kRender_TotalFrameCount, nFrameCount); }

	__inline HRESULT GetFrameCount(INT *pFrameCount) { return p->GetInt(NLEKey::Render::kRender_TotalFrameCount, pFrameCount); }
};
