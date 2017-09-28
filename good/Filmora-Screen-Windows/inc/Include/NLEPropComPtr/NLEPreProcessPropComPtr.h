#pragma once
/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  NLEPreProcessPropComPtr.h
 *@brief                                
 *@version 1.0                          
 *@author  wangyx                       
 *@date    4/20/2016 16:38
 */

#include "NLEComPtr.h"
#include "INLEProperties.h"
#include "NLEKey.h"

using namespace NLEKey::PreProcess;

class NLEPreProcessPropComPtr :public NLEComPtr<INLEProperties>
{
public:
	__inline HRESULT SetShiftSpeed(NLERational oSpeed) { return p->SetRational(kPreProcess_ShiftSpeed, oSpeed); }
	__inline HRESULT GetShiftSpeed(NLERational *pSpeed){ return p->GetRational(kPreProcess_ShiftSpeed, pSpeed); }

public:
	NLEPreProcessPropComPtr()
	{
	}
	NLEPreProcessPropComPtr(INLEProperties* p) :NLEComPtr(p)
	{
	}

	NLEPreProcessPropComPtr(NLEComPtr<INLEProperties> p) :NLEComPtr(p)
	{
	}
	~NLEPreProcessPropComPtr()
	{

	}

public:

	NLEPreProcessPropComPtr* operator->() const throw()
	{
		return  (NLEPreProcessPropComPtr*)this;
	}
};



