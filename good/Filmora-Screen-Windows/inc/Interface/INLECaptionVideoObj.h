#include <Guiddef.h>                                                            
#include <objbase.h>    
#include "NLECaptionDefine.h"

#ifndef _NLE_VIDEO_PROPERTY_TYPE_
#define _NLE_VIDEO_PROPERTY_TYPE_

// {6CA774E2-42C3-49f3-A3EC-596D4AC7DA34}
DEFINE_GUID(CLSID_INLECaptionVideoObj,
	0x6ca774e2, 0x42c3, 0x49f3, 0xa3, 0xec, 0x59, 0x6d, 0x4a, 0xc7, 0xda, 0x34);

// {571AE109-D8F6-49bf-8EB2-FC472A4A82A1}
DEFINE_GUID(IID_INLECaptionVideoObj,
	0x571ae109, 0xd8f6, 0x49bf, 0x8e, 0xb2, 0xfc, 0x47, 0x2a, 0x4a, 0x82, 0xa1);


class INLECaptionVideoObj :public IUnknown
{
public:
	/**
	*@brief     设置视频路径参数
	*@param pvideoObj 图片路径信息
	*@return
	*/
	virtual void __stdcall SetVideoParam(NLEVideoObj* pvideoObj) = 0;


	virtual void __stdcall GetVideoParam(NLEVideoObj*  pvideoObj) = 0;
	virtual bool __stdcall SetVideoPath(BSTR videoPath) = 0;
	virtual bool __stdcall SetColor1(DWORD Color1) = 0;

	virtual bool __stdcall SetGradientEnable(int nEnable) = 0;
	virtual bool __stdcall SetGradientColor1(DWORD Color1) = 0;
	virtual bool __stdcall SetGradientColor2(DWORD Color2) = 0;
	virtual bool __stdcall SetGradientAngle(int nGradientAngle) = 0;
	/**
	*@brief     获取视频帧
	*@param nCurFrame 当前帧
	*@param RalFrameRate 帧率
	*@param bPreview 是否预览模式 
	*@return
	*/
	virtual bool __stdcall GetSample(DWORD nCurFrame, NLERational RalFrameRate, bool bPreview) = 0;
};


#endif