/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  INLEAudioPack.h
 *@brief                                
 *@version 1.0                          
 *@author  yaoyj                       
 *@date    6/17/2016 12:11
 */
#ifndef _H_INLEAUDIOPACK_H_
#define _H_INLEAUDIOPACK_H_

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            

#include "NLEComPtr.h"
#include "INLERender.h"
#include "ILadspaPlugin.h"

//{9275F483-05F9-3480-2F6A-5884FA81E43F}
DEFINE_GUID(CLSID_INLEAudioPack, 0x9275F483, 0x05F9, 0x3480, 0x2F, 0x6A, 0x58, 0x84, 0xFA, 0x81, 0xE4, 0x3F);
//{6A50C5C5-1399-6A18-9A25-D4D6B9F5C0B5}
DEFINE_GUID(IID_INLEAudioPack, 0x6A50C5C5, 0x1399, 0x6A18, 0x9A, 0x25, 0xD4, 0xD6, 0xB9, 0xF5, 0xC0, 0xB5);
class INLEAudioPack :public IUnknown
{
public:

    virtual HRESULT __stdcall  SetInputRender(NLEComPtr<INLERender> pRender) = 0;

	virtual HRESULT __stdcall  SetAudioInfo(NLERational dRenderFramerate, int channels, int samplerate, int bitsPerSample, NLERational dSourceFramerate) = 0;

	virtual HRESULT __stdcall SetAudioFrameSize(const int nByteSize) = 0;
	virtual HRESULT __stdcall GetAudioFrameSize(int *nByteSize) = 0;

	virtual HRESULT __stdcall SetAudioProcessor(NLEComPtr<INLEAudioProcessor> pProcessor) = 0;
};

#endif //_H_INLEAUDIOPACK_H_
                                                                           

