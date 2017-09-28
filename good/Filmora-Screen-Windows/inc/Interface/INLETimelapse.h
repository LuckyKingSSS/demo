#pragma once

#ifndef _H_INLETIMELAPSE_H_
#define _H_INLETIMELAPSE_H_

#include "NLEType.h"
#include "NLEGraphics.h"
#include <Guiddef.h>                                                            
#include <objbase.h>   

// {A09503B8-4E05-4091-96C6-4A384A0B7D3C}
DEFINE_GUID(CLSID_NLETimelapse,
	0xa09503b8, 0x4e05, 0x4091, 0x96, 0xc6, 0x4a, 0x38, 0x4a, 0xb, 0x7d, 0x3c);
// {2B21C1D5-8903-42CC-9182-632B918E6CD4}
DEFINE_GUID(IID_INLETimelapse,
	0x2b21c1d5, 0x8903, 0x42cc, 0x91, 0x82, 0x63, 0x2b, 0x91, 0x8e, 0x6c, 0xd4);

/**@ingroup GroupTimeline
*@brief Timelapse组件
*@{
*/

class INLETimelapseClip : public IUnknown
{
public:
	virtual HRESULT GetFramerate(NLERational &framerate) = 0;
	virtual HRESULT SetFramerate(NLERational &framerate) = 0;

	virtual HRESULT GetRange(NLERange &rng) = 0;

	virtual HRESULT GetImageCount(int &cnt) = 0;
	virtual HRESULT HideImageByIdx(int idx, bool hidden = true) = 0;
};

/**
*@brief Timelapse组件
*/
class INLETimelapse :public IUnknown
{
public:
	virtual HRESULT GetClipCount(int &cnt) = 0;
	virtual HRESULT GetClip(int idx, INLETimelapseClip **ppv) = 0;

	virtual HRESULT InsertClip(int idx, WCHAR *path) = 0;
	virtual HRESULT RemoveClip(int idx) = 0;

	virtual HRESULT SetClipSize(NLESize &size) = 0;

	virtual HRESULT GotoClip(int idx, NLERange *rng) = 0;
};

/**
*@}
*/

#endif // _H_INLETIMELAPSE_H_