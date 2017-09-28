/*
* Copyright (c) 2013 Wondershare.inc
* All rights reserved.
*
*@file    INLEFrame.h
*@brief   定义音频/视频/文字数据帧接口
*@version 1.0
*@author  wangyx
*@date    2016/03/02 14:11
*/

#pragma once
#ifndef _H_INLEFRAME_H_
#define _H_INLEFRAME_H_


#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
#include <stdint.h>
#include <vector>
#include "NLEComPtr.h"
#include "INLEBitmap.h"


/** @ingroup GroupTimeline
*  主要包括 INLEFrame
*/


//{BF73FF76-BC29-4A28-9E43-CFE69C8522FD}
DEFINE_GUID(CLSID_NLEFrame, 0xBF73FF76, 0xBC29, 0x4A28, 0x9E, 0x43, 0xCF, 0xE6, 0x9C, 0x85, 0x22, 0xFD);

//{D5A682CB-7397-FC57-E739-096AF5B3FD27}
DEFINE_GUID(IID_INLEFrame, 0xD5A682CB, 0x7397, 0xFC57, 0xE7, 0x39, 0x09, 0x6A, 0xF5, 0xB3, 0xFD, 0x27);       
/**
 *@brief 视频帧/音频帧接口
 *	- INLEProperties 帧属性，用于查询帧信息,如视频的宽高等
 *	- INLECloable 复制接口，能够复制帧
 */
class INLEFrame :public IUnknown
{
public:
	/**
	*@brief  	设置时间戳及时长
	*@param	pts 时间戳，单位100ns(100ns = 1/10000000s)
	*@param	duration 时长,单位100ns
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*@note		注意事项
	*/
    virtual HRESULT __stdcall SetTimestamp(NLElonglong pts, NLElonglong duration) = 0;
	
    /**
	*@brief  	获取时间戳及时长
	*@param	pPts 时间戳，单位100ns(100ns = 1/10000000s)
	*@param	pDuration 时长,单位100ns
	*@return 	HRESULT __stdcall
	*@see 		SetTimestamp
	*/
    virtual HRESULT __stdcall GetTimestamp(NLElonglong *pPts, NLElonglong *pDuration) = 0;

    virtual HRESULT __stdcall GetMediaInfo(NLEMediaInfo *pInfo) = 0;
	
	virtual NLEMediaType __stdcall GetType() = 0;
    	
    virtual HRESULT	__stdcall SetType(NLEMediaType type) = 0;

	/**
	*@brief  	获取帧数据字节数
	*@return 	帧数据字节数
	*@see 		GetData
	*/
	virtual UINT __stdcall GetDataSize() = 0;

	/**
	*@brief  	获取帧数据地址
	*@return 	帧数据地址
	*@see 		数据大小通过 GetDataSize 得到。
	*/
	virtual const LPVOID __stdcall GetData() = 0;

    /**
    @brief 将frame中的数据重置为默认值，视频帧重置为黑帧，音频帧置为静音
    */
	virtual HRESULT __stdcall Reset(void) = 0;

    /**
    @brief 复制pFromFrame中的数据过来
    @param pFromFrame 源frame
    */
	virtual HRESULT __stdcall CopyFrom(INLEFrame *pFromFrame) = 0;

	/**
	 *@brief 获得图像接口，若为音频，返回NULL
	 */
	virtual NLEComPtr<INLEImage> __stdcall GetImage() = 0;

	/**
	 *@brief 获取标记
	 */
	virtual HRESULT __stdcall GetMediaFrameNumber(INT* pFrmNumber) = 0;

	/**
	 *@brief 绘制文本
	 */
	virtual HRESULT __stdcall DrawText(LPCWSTR pwszText, INT x = 0, INT y = 0) = 0;

	/**
	 *@brief 检测是否为同步点
	 */
	virtual HRESULT __stdcall GetSyncPoint(NLEFrameIndex *pFrmIdxSyncPoint) = 0;
};

#define FRAME_IMAGE(pFrame) ((pFrame)->GetImage())
#define FRAME_WIDTH(pFrame) ( FRAME_IMAGE(pFrame)->GetWidth() )
#define FRAME_HEIGHT(pFrame) ( FRAME_IMAGE(pFrame)->GetHeight() )
#define FRAME_SIZE(pFrame) NLESize( FRAME_WIDTH(pFrame), FRAME_HEIGHT(pFrame) )

#endif //_H_INLEFRAME_H_
