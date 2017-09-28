/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  INLEFrameOperate.h
 *@brief                                
 *@version 1.0                          
 *@author  yaoyj                       
 *@date    3/11/2016 13:56
 */

#ifndef _H_INLEFRAMEOPERATE_H_
#define _H_INLEFRAMEOPERATE_H_

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
          
struct MEDIA_FRAME;
struct NLEMediaInfo;

//{36EF1805-BAB8-8CEF-767C-F49D578A1CD3}
DEFINE_GUID(CLSID_INLEFrameOperate, 0x36EF1805, 0xBAB8, 0x8CEF, 0x76, 0x7C, 0xF4, 0x9D, 0x57, 0x8A, 0x1C, 0xD3);                   
//{8022AAE8-CE6F-BC0D-5C58-1DCC63E85C9B}
DEFINE_GUID(IID_INLEFrameOperate, 0x8022AAE8, 0xCE6F, 0xBC0D, 0x5C, 0x58, 0x1D, 0xCC, 0x63, 0xE8, 0x5C, 0x9B);                         
class INLEFrameOperate :public IUnknown
{
public:	
    virtual HRESULT STDMETHODCALLTYPE SetFormat(NLEFrameFormat format) = 0;

    virtual HRESULT __stdcall SetMediaInfo(NLEMediaInfo mediaInfo) = 0;

	virtual HRESULT __stdcall GetFrameIndex(int *pFrameIndex) = 0;

	virtual HRESULT __stdcall SetFrameIndex(int nFrameIndex) = 0;

	/**
	*@brief 设置帧号
	*/
	virtual HRESULT __stdcall SetMediaFrameNumber(INT nFrmNumber) = 0;

    /**
    *@brief 用 MEDIA_FRAME 结构中数据填充IFrame帧数据
    *@param[in] pFrame 从底层编解码库得到的帧数据    
    *@return 标准错误码
    */
    virtual HRESULT STDMETHODCALLTYPE Load(MEDIA_FRAME *pFrame) = 0;

    /**    
    *@brief 生成一个指定的视频帧，当pData为NULL时则生成一个指定格式默认视频帧内存
    *@param [in]nWidth 视频宽度
    *@param [in]nHeight 视频高度
    *@param [in]format 视频格式
    *@param [in]pData 视频图像数据，对于BGRA, BGR格式，数据逐行存放;
    *           对于YUV数据，则先存放所有的Y分量数据，然后是所有的U分量数据，最后是所有的V分量数据
    *@return 标准错误码
    */
    virtual HRESULT STDMETHODCALLTYPE InitVideo(int nWidth, int nHeight, 
        NLEFrameFormat format = NLE_FRAME_FORMAT_BGRA, void *pData = NULL, NLElonglong pts = 0, NLElonglong duration = 400000) = 0;

    /**
    @brief 生成一个指定长度的音频静音帧
    @param [in]nSampleRate 采样率
    @param [in]nChannels 声道数
    @param [in]nBitsPerSample 位深度    
    @param [in]nSize 数据大小，以字节为单位
    */
    virtual HRESULT STDMETHODCALLTYPE CreateAudio(int nSampleRate, int nChannels, int nBitsPerSample, int nSize) = 0;

    /**
    @brief 混音
    */
    virtual HRESULT STDMETHODCALLTYPE MixAudio(void *pData, int nSize) = 0;
    

	/**
	 *@brief  	设置数据
	 *@param	nSize
	 *@return 	HRESULT STDMETHODCALLTYPE
	 *@see 		参考资料
	 *@note		nSize必须小于或等于GetBufferSize()
	 */
	virtual HRESULT STDMETHODCALLTYPE SetDataSize(const UINT nSize) = 0;

	/**
	*@brief  	获取内部数据缓冲字节数
	*@return 	帧数据字节数
	*@see 		GetData
	*@note
	*/
	virtual UINT STDMETHODCALLTYPE GetBufferSize() = 0;

	/**
	 *@brief  	SetOpaque，设置是否为不透明
	 *@param	bOpaque 是否为不透明
	 *@return 	HRESULT STDMETHODCALLTYPE
	 *@see 		参考资料
	 *@note		注意事项
	 */
	virtual HRESULT STDMETHODCALLTYPE SetOpaque(BOOL bOpaque) = 0;

	/**
	 *@brief 设置是否为同步点
	 */
	virtual HRESULT __stdcall SetSyncPoint(NLEFrameIndex frmIndexSyncPoint) = 0;
};                                                                              



#endif //_H_INLEFRAMEOPERATE_H_
