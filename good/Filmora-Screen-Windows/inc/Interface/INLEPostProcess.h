#pragma once

/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  INLEPostProcess.h
 *@brief                                
 *@version 1.0                          
 *@author  wangyx                       
 *@date    7/14/2016 16:36
 */

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
#include "INLEFrame.h"
#include "INLEProperties.h"
#include "NLEComPtr.h"

//{F447FBA6-9F23-B9DA-99FE-4A0CD1CFEF66}
DEFINE_GUID(CLSID_NLEPostRequest, 0xF447FBA6, 0x9F23, 0xB9DA, 0x99, 0xFE, 0x4A, 0x0C, 0xD1, 0xCF, 0xEF, 0x66);                   
//{9692CAFB-4B6D-C1DF-978E-AAE734A223E8}
DEFINE_GUID(IID_INLEPostRequest, 0x9692CAFB, 0x4B6D, 0xC1DF, 0x97, 0x8E, 0xAA, 0xE7, 0x34, 0xA2, 0x23, 0xE8);

typedef struct PostTransform
{
	float		fRotate;// 0~360
	NLEPointF	ptfTranslate;	//平移
	NLESizeF	sfScale; //缩放
}PostTransform;

class INLEPostRequest:public IUnknown
{
public:
	virtual void				__stdcall SetProperties(NLEComPtr<INLEProperties> pProp) = 0;
	virtual NLEComPtr<INLEProperties>	 __stdcall GetProperties() = 0;

	virtual void				 __stdcall SetCurrentFrame(NLEComPtr<INLEFrame> pFrame) = 0;
	virtual NLEComPtr<INLEFrame> __stdcall GetCurrentFrame() = 0;

	virtual void				 __stdcall SetGraphics(NLEComPtr<INLEFrame> pGraphics) = 0;
	virtual NLEComPtr<INLEFrame> __stdcall GetGraphics() = 0;

	virtual void				 __stdcall SetOffset(NLEFrameIndex nOffset) = 0;
	virtual NLEFrameIndex		 __stdcall GetOffset() = 0;
	
	virtual void				 __stdcall SetMatrix(NLETransformMatrix &matTransform) = 0;
	virtual NLETransformMatrix*  __stdcall GetMatrix() = 0;
	
	virtual void				__stdcall  SetTransform(PostTransform &postTransform) = 0;
	virtual const PostTransform*  __stdcall GetTransform() = 0;

	virtual void				__stdcall SetAlpha(float btAlpha) = 0;
	virtual float				__stdcall GetAlpha() = 0;

	virtual void				 __stdcall CopyFrom(INLEPostRequest* pRequest) = 0;

	virtual NLEComPtr<INLEPostRequest> __stdcall Clone() = 0;

	virtual void				__stdcall Reset() = 0;
};

//{C5AF0AA9-4C71-17DE-C1AC-405495FAE991}
DEFINE_GUID(CLSID_NLEVideoPostProcess, 0xC5AF0AA9, 0x4C71, 0x17DE, 0xC1, 0xAC, 0x40, 0x54, 0x95, 0xFA, 0xE9, 0x91);                   

//{30F3A658-E633-5C10-F0CC-729FD51C40F4}
DEFINE_GUID(CLSID_NLEAudioPostProcess, 0x30F3A658, 0xE633, 0x5C10, 0xF0, 0xCC, 0x72, 0x9F, 0xD5, 0x1C, 0x40, 0xF4);

//{06073364-F75F-EC48-28BE-1F236A55CBD6}
DEFINE_GUID(IID_INLEPostProcess, 0x06073364, 0xF75F, 0xEC48, 0x28, 0xBE, 0x1F, 0x23, 0x6A, 0x55, 0xCB, 0xD6);

class INLEPostProcess :public IUnknown
{
public:
	virtual HRESULT __stdcall WillProcess(INLEPostRequest *pRequest) = 0;
	virtual HRESULT __stdcall DoProcess(INLEPostRequest *pRequest) = 0;
	virtual HRESULT __stdcall Cleanup() = 0;
};
