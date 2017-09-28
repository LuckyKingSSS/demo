#pragma once


#include "NLEKey.h"
using namespace NLEKey::Render;
using namespace NLEKey::Base;

#include "NLEBasePropComPtr.h"
#include "NLERenderPropComPtr.h"
#include "NLESelfPropComPtr.h"
#include "NLESourcePropComPtr.h"
#include "NLEPreProcessPropComPtr.h"

class NLEPropComPtr :
	virtual public NLEBasePropComptr, 
	virtual public NLERenderPropComPtr,
	virtual public NLESelfPropComPtr,
	virtual public NLESourcePropComPtr,
	virtual public NLEPreProcessPropComPtr
{
public:
	NLEPropComPtr()
	{

	}

	NLEPropComPtr(NLEComPtr<INLEProperties> p) :
		NLEBasePropComptr(p), 
		NLERenderPropComPtr(p),
		NLESelfPropComPtr(p),
		NLESourcePropComPtr(p),
		NLEPreProcessPropComPtr(p)
	{	
	}

	~NLEPropComPtr() 
	{
	}

public:

	NLEPropComPtr* operator->() const throw()
	{
		return  (NLEPropComPtr*)this;
	}
public:

	/***@brief 返回Render接口*/
	NLERenderPropComPtr* Render()
	{
		return (NLERenderPropComPtr *)this;
	}

	/**返回原始接口，即INLEProperties*/
	NLESelfPropComPtr* Self()
	{
		return (NLESelfPropComPtr *)this;
	}

	/***@返回基础接口*/
	NLEBasePropComptr* Base()
	{
		return (NLEBasePropComptr *)this;
	}

	/***@返回源信息接口*/
	NLESourcePropComPtr *Source()
	{
		return (NLESourcePropComPtr *)this;
	}

};