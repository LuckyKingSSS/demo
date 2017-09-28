#pragma once

#ifndef _H_INLEACTIONCAM_H_
#define _H_INLEACTIONCAM_H_

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>

#include "INLETimelineBase.h"

// {5B326A84-B1C7-4F24-95BD-332D1919BAFB}
DEFINE_GUID(CLSID_NLEActionCam, 0x5b326a84, 0xb1c7, 0x4f24, 0x95, 0xbd, 0x33, 0x2d, 0x19, 0x19, 0xba, 0xfb);
// {2A51721C-91B0-4FE3-8410-03A48FDC08FB}
DEFINE_GUID(IID_INLEActionCam, 0x2a51721c, 0x91b0, 0x4fe3, 0x84, 0x10, 0x3, 0xa4, 0x8f, 0xdc, 0x8, 0xfb);

// {7BD098B5-6BCD-4B8A-8D10-128C4BEF92B4}
DEFINE_GUID(IID_INLEACMarkup, 0x7bd098b5, 0x6bcd, 0x4b8a, 0x8d, 0x10, 0x12, 0x8c, 0x4b, 0xef, 0x92, 0xb4);
// {E3537659-2E4C-41D0-8840-B99A6F0D169E}
DEFINE_GUID(IID_INLEACTimeshift, 0xe3537659, 0x2e4c, 0x41d0, 0x88, 0x40, 0xb9, 0x9a, 0x6f, 0xd, 0x16, 0x9e);
// {C3967105-3028-4CCD-A0A1-712A745645EA}
DEFINE_GUID(IID_INLEACFreeze, 0xc3967105, 0x3028, 0x4ccd, 0xa0, 0xa1, 0x71, 0x2a, 0x74, 0x56, 0x45, 0xea);

/**@ingroup GroupTimeline
*@brief 运动相机组件
*@{
*/

/**
*@brief 标签基接口
*/
class INLEACMarkup :public IUnknown
{
public:
	virtual HRESULT __stdcall GetEffectedClipIdx(int &clipIdx) = 0;
	virtual HRESULT __stdcall SetEffectedClipIdx(int clipIdx) = 0;

	virtual HRESULT __stdcall SetPos(NLEFrameIndex pos) = 0;
	virtual HRESULT __stdcall GetPos(NLEFrameIndex &pos) = 0;

	virtual HRESULT __stdcall SetLen(NLEFrameIndex len) = 0;
	virtual HRESULT __stdcall GetLen(NLEFrameIndex &len) = 0;
};

/**
*@brief 时间位移接口
*/
class INLEACTimeshift :public IUnknown
{
public:
	virtual HRESULT __stdcall GetReplay(int &cnt) = 0;
	virtual HRESULT __stdcall SetReplay(int cnt) = 0;
};

/**
*@brief 定帧接口
*/
class INLEACFreeze :public IUnknown
{
public:
};

/**
*@brief 概览接口。管理整个tl的基本信息{包括有多少markup、freeze，在哪里等等}
*/
class INLEActionCam :public IUnknown
{
public:
	/**
	*@brief  	设置/获取原始片段{原始片段，可以用来抓取缩略图}
	*@param	pClip 设置的原始片段
	*@param	ppv 获取的原始片段
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*@note		注意事项
	*/
	virtual HRESULT __stdcall SetOrignalClip(INLETimelineBase *pClip) = 0;
	virtual HRESULT __stdcall GetOrignalClip(INLETimelineBase **ppv) = 0;

	/**
	*@brief  	新建/获取/删除指定位置的时间位移标签
	*@param	pos 位置{当获取标签时，只要pos落在标签范围内即可}
	*@param	ppv 时间位移标签接口。如果指定位置的时间位移标签不存在，则返回NULL
	*@param	len 所占长度
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*@note		注意事项
	*/
	virtual HRESULT __stdcall NewTimeshiftByPos(NLEFrameIndex pos, NLEFrameIndex len, int replayCnt = 2, int effectedClipIdx = 0) = 0;
	virtual HRESULT __stdcall GetTimeshiftByPos(NLEFrameIndex pos, INLEACTimeshift **ppv) = 0;
	virtual HRESULT __stdcall DeleteTimeshiftByPos(NLEFrameIndex pos) = 0;

	/**
	*@brief  	新建/获取/删除指定位置的定帧标签
	*@param	pos 位置{当获取标签时，只要pos落在标签范围内即可}
	*@param	ppv 定帧标签接口。如果指定位置的定帧标签不存在，则返回NULL
	*@param	len 所占长度
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*@note		注意事项
	*/
	virtual HRESULT __stdcall NewFreezeByPos(NLEFrameIndex pos, NLEFrameIndex len, int effectedClipIdx = 0) = 0;
	virtual HRESULT __stdcall GetFreezeByPos(NLEFrameIndex pos, INLEACFreeze **ppv) = 0;
	virtual HRESULT __stdcall DeleteFreezeByPos(NLEFrameIndex pos) = 0;

	/**
	*@brief  	由播放器回调的播放进度，计算UI需要显示的播放进度
	*@param	play_pos 播放进度。-1 - 表示重设内部计数{比如，时间位移标签的回放次数}，此时ac_pos返回值无意义
	*@param	ac_pos 界面显示进度
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*@note		注意事项
	*/
	virtual HRESULT __stdcall PlayToAC(NLEFrameIndex play_pos, NLEFrameIndex &ac_pos) = 0;
};

/**
*@}
*/

#endif // _H_INLEACTIONCAM_H_