/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  IVideoStabilizer.h
 *@brief                                
 *@version 1.0                          
 *@author  yaoyj                       
 *@date    6/28/2016 13:47
 */

#ifndef _H_IVIDEOSTABILIZER_H_
#define _H_IVIDEOSTABILIZER_H_

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            

#include <stdint.h>
#include "NLEGraphics.h"
                                                                                
//{F04D160D-B15A-DEA8-B606-4141BDD67E95}
DEFINE_GUID(CLSID_VideoStabilizer, 0xF04D160D, 0xB15A, 0xDEA8, 0xB6, 0x06, 0x41, 0x41, 0xBD, 0xD6, 0x7E, 0x95);                   
//{24EA62D8-4126-380A-2C85-AB445E17431B}
DEFINE_GUID(IID_IVideoStabilizer, 0x24EA62D8, 0x4126, 0x380A, 0x2C, 0x85, 0xAB, 0x44, 0x5E, 0x17, 0x43, 0x1B);                         

typedef enum eStabilizerMode
{
	STABILIZER_MODE_NO_ACTUAL_TIME = 0, //非实时模式
	STABILIZER_MODE_ACTUAL_TIME = 1, //实时模式
}eStabilizerMode;


class IVideoStabilizer :public IUnknown
{
public:
	/**
	*@brief 初始化稳像模块
	*@param[in] width 图像宽度
	*@param[in] height 图像高度
	*@param[in] mode 稳像的模式，目前只支持非实时模式
	*@param[in] iRenderMode 是否为输出？ 1：是 0:不是
	*@return 标准错误
	*/
	virtual HRESULT STDMETHODCALLTYPE Initialize(int width, int height, eStabilizerMode mode = STABILIZER_MODE_NO_ACTUAL_TIME, int iRenderMode = 0, float fSacle = 1.0) = 0;

	/**
	*@brief 清除之前处理已得到的稳像参数数据，重新初始化稳像模块
	*/
	virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;

	/**
	*@brief 获取稳像处理后可输出的图像的边界尺寸
	*@param[out] pOutSize 获取稳像后输出的图像尺寸	
	*@return 标准错误
	*/
	virtual HRESULT STDMETHODCALLTYPE GetOutSize(NLESize *pOutSize) = 0;

	/**
	*@brief 处理一帧图像，向稳像处理模块送入一帧图像
	*@param[in] pInData 需要处理的图像数据，格式为BGRA，字节序为 B G R A B G R A B G ...
	*@param[in] nWidth 图像宽度
	*@param[in] nHeight 图像高度
	*@param[in] nFrameIndex 图像序号
	*@return 标准错误
	*/
	virtual HRESULT STDMETHODCALLTYPE PushFrame(uint8_t *pInData, int nWidth, int nHeight, int nFrameIndex) = 0;

	/**
	*@brief 停止送入图像
	*/
	virtual HRESULT STDMETHODCALLTYPE EndPushFrame(void) = 0;

	/**
	*@brief 获取处理后的一帧图像数据
	*@param[in] pInData 原始图像数据，默认格式为BGRA，字节序为 B G R A B G R A B G ...
	*@param[out] pOutData 输出的图像数据，函数内部会填充这个buffer, buffer的内存由使用者分配，处理后的图像尺寸小于或等于原图像尺寸
	*@param[in] nFrameIndex 需要处理的图像的编号
	*@return 标准错误
	*/
	virtual HRESULT STDMETHODCALLTYPE GetOutFrame(uint8_t *pInData, uint8_t *pOutData, int nFrameIndex) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetParams(int* pWidth, int* pHeight, eStabilizerMode* pMode, int* pRenderMode, float* pSacle = NULL) =0;

	
	/**
	 *@brief  	GetBufferCount，获得最小缓冲帧队列大小
	 *@param[int] nRenderMode 是否为输出模式？ 1:是， 0:不是
	 *@param[out] pnCount 最小缓存队列大小
	 *@return 	HRESULT STDMETHODCALLTYPE
	 *@see 		参考资料
	 *@note		必须在初始化(Initialize)后才能调用
	 */
	virtual HRESULT STDMETHODCALLTYPE GetMinBufferCount(int *pnCount) = 0;
};                                                                              


#endif //_H_IVIDEOSTABILIZER_H_
