/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  INLEMediaStream.h
 *@brief                                
 *@version 1.0                          
 *@author  wangyx                       
 *@date    3/4/2016 17:12
 */

#ifndef _H_INLEMEDIASTREAM_H_
#define _H_INLEMEDIASTREAM_H_

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
#include "INLEService.h"
            
class INLEProperties;
class INLEFrame;

//{B1BC2317-7841-CB26-4B7C-738FF523625B}
DEFINE_GUID(CLSID_INLEStream, 0xB1BC2317, 0x7841, 0xCB26, 0x4B, 0x7C, 0x73, 0x8F, 0xF5, 0x23, 0x62, 0x5B);                   
//{41202E1A-8B8F-3214-8515-05755DF2A43F}
DEFINE_GUID(IID_INLEStream, 0x41202E1A, 0x8B8F, 0x3214, 0x85, 0x15, 0x05, 0x75, 0x5D, 0xF2, 0xA4, 0x3F);                         
class INLEStream : public IUnknown
{
public:
    
    virtual NLEMediaType STDMETHODCALLTYPE GetStreamType(void) = 0;

	/**
	*@brief  获取当前的帧(视频) 或 Sample(音频)
	*@return 标准错误	
	*@remark 若返回的ppFrame为NULL，那么表明文件解码完成
	*/
	virtual HRESULT STDMETHODCALLTYPE GetFrame(INLEFrame **ppFrame) = 0;

	/**
	@brief 获取当前的解码位置

	@return 解码进度，单位是100纳秒(100ns)
	*/
	virtual NLElonglong STDMETHODCALLTYPE GetOffset(void) = 0;

	/**
	@brief 对流进行seek操作

	@param position[in] 要seek的时间点，单位是100纳秒(100ns)
	@param accurate [in] 是否精确seek

	@return seek的结果
	*/
	virtual NLElonglong STDMETHODCALLTYPE Seek(NLElonglong position, int accurate) = 0;

	/**
	* @brief  前进或后退指定数量的帧
    * @param  [in]nFrameCount 当值 >0，表示前进的帧数；<0,表示后退的帧数（暂未实现），=0，不处理
	* @return S_OK 表示操作成功，E_FAIL表示解码结束
	*/
    virtual HRESULT STDMETHODCALLTYPE Forward(INT nFrameCount = 1, INT *pRealFrameCount = NULL) = 0;

	/**
	* @brief  变换一路流
	* @param nStreamID [in] 新流ID
	* @return 变换结果
	*/
	virtual HRESULT STDMETHODCALLTYPE ChangeStream(int nStreamID) = 0;

    /**
    * @brief 设置每个音频帧的时长，单位为字节数。
    */
    virtual HRESULT STDMETHODCALLTYPE SetAudioFrameSize(int nSize) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetAudioFrameSize(int *pSize) = 0;
	/**	
	*@brief 渲染当前帧(视频) 或 Sample(音频)
	*@return 标准错误
	*
	*/
	virtual HRESULT STDMETHODCALLTYPE Render(INLEFrame *pFrame) = 0;
};                                                                              



#endif //_H_INLEMEDIASTREAM_H_
