/*
 *  MediaConvert.hpp
 *  TestTemp
 *
 *  Created by  wanggh on 1/14/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "MediaConverterExParam.h"

enum ConvertState
{
    CS_NONE,		//
    CS_RUNNING,		// 
    CS_COMPLETE,	// 
    CS_ERROR      	// 
};

struct ConvertProgressInfo
{
	int cbSize					; //struct size
	ConvertState eConvState		; // 	
	
	int nProgress				;
	int nCurIndex				; // current index
	//double dCurLength			; // 
	//double dCurTransedLength   	; // 
	
	int nCount					;
	double dTotalLength     	; // 
	double dTotalTransedLength	; // 
	
	double dElapsedTime			; // seconds
	double dRemainedTime		; // seconds
	
	double dCurTransRate		; // Transed length per second
};

DEFINE_GUID(IID_IMediaConvItem, 
			0x50cf4c1a, 0x070c, 0x4ee1, 0x96, 0x6d, 0xec, 0x4e, 0xd1, 0x3d, 0x54, 0xa5);
EXTERN_C const IID IID_IMediaConvItem;
MACRO_MIDL_INTERFACE(IMediaConvItem, "50CF4C1A-070C-4ee1-966D-EC4ED13D54A5")
: public IUnknown
{
/*
	STDMETHOD(LoadDefaultParam)() = 0;
	STDMETHOD(ClearConvParam)() = 0;	*/

	STDMETHOD(SetConvParam)(const char* key, const char* value) = 0;
	STDMETHOD(SetConvParam)(const char* key, /*out*/const char* buff, /*in, out*/int buff_size) = 0;
	STDMETHOD(GetConvParam)(const char* key, /*out*/char* buff/*, / *in, out* /int* buff_size*/) = 0;
	STDMETHOD(CreateVideoAndAudioEffect)() = 0;	
	
/*
	STDMETHOD(SetConvParamEx)(const char* key, const char* buff, int buff_size) = 0;
	STDMETHOD(GetConvParamEx)(const char* key, / *out* /char* buff, / *in, out* /int* buff_size) = 0;*/

};

DEFINE_GUID(IID_IMediaConv,
			0x3ef663ca, 0x8181, 0x4e97, 0x8b, 0x3e, 0x45, 0x50, 0x7b, 0xc1, 0x78, 0xa8);
EXTERN_C const IID IID_IMediaConv;
MACRO_MIDL_INTERFACE(IMediaConv/*IMediaConverter*/, "3EF663CA-8181-4e97-8B3E-45507BC178A8")
: public IUnknown
{
	/*STDMETHOD(Init)(const BSTR strComXmlPath) = 0;*/
	STDMETHOD(Init)(const char *pstrComXmlPath) = 0;
	STDMETHOD(AddItem)(IMediaConvItem* pObj) = 0;
	STDMETHOD(GetItem)(int index, IMediaConvItem** ppObj) = 0;

	/*STDMETHOD(ClearItem)() = 0;*/
	STDMETHOD(ClearAllItems)() = 0;
	STDMETHOD(ClearItem)(int nIndex) = 0;
	STDMETHOD(ClearItem)(IMediaConvItem *pObj) = 0;

	STDMETHOD(SetSlide)(bool bSlide) = 0;	/*chenyz add*/
	STDMETHOD_(BOOL,IsSlide)() = 0;			/*chenyz add*/

	/*STDMETHOD(SetEncParam)(const char* key, const char* value) = 0;*///chenyz add
	
	STDMETHOD(Start)() = 0;
	STDMETHOD(Pause)() = 0;
	STDMETHOD(Resume)() = 0;
	STDMETHOD(Abort)() = 0;
	STDMETHOD(Stop)() = 0;
	STDMETHOD(GetTargetCountAndFileNames)(int *pIndex, BSTR **pppFileNames) = 0;

	STDMETHOD(GetProgress)(ConvertProgressInfo& convInfo) = 0;

	STDMETHOD(SetUserCallBack)(MULTI_CALLBACK fnUserCallBack, LPVOID pUserObj) = 0;

	//int GetPreview(...); // delay to decide
};

// {5B53122D-57AD-4f7b-B548-AA6AA9D1E47E}
MIDL_DEFINE_GUID(CLSID, CLSID_CMediaConvItem, 0x5b53122d, 0x57ad, 0x4f7b, 0xb5, 0x48, 0xaa, 0x6a, 0xa9, 0xd1, 0xe4, 0x7e);
EXTERN_C const CLSID CLSID_CMediaConvItem;

// {AC4C9158-3E9D-48b6-80C2-86CF1B1BCBA8}
MIDL_DEFINE_GUID(CLSID, CLSID_CMediaConv, 0xac4c9158, 0x3e9d, 0x48b6, 0x80, 0xc2, 0x86, 0xcf, 0x1b, 0x1b, 0xcb, 0xa8);
EXTERN_C const CLSID CLSID_CMediaConv;



