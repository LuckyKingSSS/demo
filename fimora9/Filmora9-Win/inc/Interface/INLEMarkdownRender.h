#pragma once

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            

#include "MarkdownCommon.h"

//{AD9FE01F-7E3C-8BFD-B4DE-27ED170C64E0}
DEFINE_GUID(CLSID_INLEMarkdownRender, 0xAD9FE01F, 0x7E3C, 0x8BFD, 0xB4, 0xDE, 0x27, 0xED, 0x17, 0x0C, 0x64, 0xE0);
//{19737095-9168-74A5-2560-B6BB681994F3}
DEFINE_GUID(IID_INLEMarkdownVideoRender, 0x19737095, 0x9168, 0x74A5, 0x25, 0x60, 0xB6, 0xBB, 0x68, 0x19, 0x94, 0xF3);

//{80A33B5C-F40E-AFA1-6675-58D5CB76E77A}
DEFINE_GUID(IID_INLEMarkdownAudioRender, 0x80A33B5C, 0xF40E, 0xAFA1, 0x66, 0x75, 0x58, 0xD5, 0xCB, 0x76, 0xE7, 0x7A);

class INLEMarkdownRender :public IUnknown
{
public:
	virtual HRESULT __stdcall AttachRenderNode(void* pRenderNode) = 0;
	virtual HRESULT __stdcall DettachRenderNode() = 0;
	virtual HRESULT __stdcall SetLastEditInfo(MarkdownInfo oEditInfo) = 0;
};

