/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  INLESpeedShift.h
 *@brief                                
 *@version 1.0                          
 *@author  yaoyj                       
 *@date    4/24/2017 14:28
 */

#ifndef _H_INLESPEEDSHIFT_H_
#define _H_INLESPEEDSHIFT_H_
                                                         
#include <Guiddef.h>                                                            
#include <objbase.h>    
#include "INLEGraphics.h"
#include "NLEType.h" 

/**
@brief 平滑渐变的过渡方式
*/
enum eNLESpeedShiftAlgorithm
{
	NLE_SPEED_SHIFT_ALGORITHM_LINEAR = 0, //一次线性渐变

	NLE_SPEED_SHIFT_ALGORITHM_SINECOSINE, //正弦余弦曲线渐变

	NLE_SPEED_SHIFT_ALGORITHM_PARABOLA, //二次抛物线渐变
};

//{3E258317-44F1-D6F7-D312-6B1F7FE08071}
DEFINE_GUID(CLSID_NLESpeedShift, 0x3E258317, 0x44F1, 0xD6F7, 0xD3, 0x12, 0x6B, 0x1F, 0x7F, 0xE0, 0x80, 0x71);                   
//{9223415C-5F2B-6EA1-6FBA-8DEB85C6D886}
DEFINE_GUID(IID_INLESpeedShift, 0x9223415C, 0x5F2B, 0x6EA1, 0x6F, 0xBA, 0x8D, 0xEB, 0x85, 0xC6, 0xD8, 0x86);                         
/**
@brief 分段变速，在变速区间切换时采用平滑渐变方式过渡。
变速过程中帧率不变，通过压缩或拉长时间实现变速.
变速方案设计的主要思路为：
以变速前的帧数为x轴，速率的倍数的倒数为y轴，绘制速度的变化率与帧数的曲线，则对曲线的积分就是变速后对应的帧数。
*/
class INLESpeedShift :public IUnknown
{
public:
	/**
	*@brief 设置平滑的算法
	*@param[in] mode 平滑的算法模式
	*
	@return 标准错误
	*/
	virtual HRESULT STDMETHODCALLTYPE SetShiftMode(eNLESpeedShiftAlgorithm mode) = 0;

	/**
	@brief 设置源帧率
	*/
	virtual HRESULT STDMETHODCALLTYPE SetOriginFramerate(NLERational originFramerate) = 0;

	/**
	@brief 设置变速的总区间
	*/
	//virtual HRESULT STDMETHODCALLTYPE SetOriginRange(NLERange range) = 0;

	/**
	@brief 设置渲染帧率
	*/
	virtual HRESULT STDMETHODCALLTYPE SetRenderFramerate(NLERational renderFramerate) = 0;
	
	/**
	@brief 根据变速速率与原始播放时间的关系曲线，设置经过该曲线的关键点，如果第index个点存在，则修改
	@param point 曲线经过的关键点		

	@return 标准错误
	@remark 点的坐标x值为变速前帧序号，y值为变速的倍数
	*/
	virtual HRESULT STDMETHODCALLTYPE AddShiftPoint(NLEShiftPoint point) = 0;

	/**
	@brief 获取曲线上添加的关键点的个数
	*/
	virtual HRESULT STDMETHODCALLTYPE GetShiftPointCount(int *pCount) = 0;

	/**
	@brief 删除指定的关键点
	@param index 曲线上设置的关键点的序号
	@return 标准错误
	@remark 点的坐标x值为变速前帧序号，y值为变速的倍数
	*/
	virtual HRESULT STDMETHODCALLTYPE DeleteShiftPoint(int index) = 0;

	/**
	@brief 修改关键点的值，修改曲线
	@param index 曲线上设置的关键点的序号
	@return 标准错误
	@remark 点的坐标x值为变速前帧序号，y值为变速的倍数
	*/
	virtual HRESULT STDMETHODCALLTYPE ModifyShiftPoint(NLEShiftPoint point, int index) = 0;	

	/**
	@brief 获取变速速率与原始播放时间的关系曲线上的关键点列表
	@param pPoints 曲线经过的点的列表
	@param nMaxPointCount pPoints最多可保存的点的个数
	@param pRealPointCount 实际保存的点的个数

	@return 标准错误
	@remark 点的坐标x值为变速前帧序号，y值为变速的倍数
	*/
	virtual HRESULT STDMETHODCALLTYPE GetShiftPoints(NLEShiftPoint *pPoints, int nMaxPointCount, int *pRealPointCount) = 0;

	/**
	@brief 获取变速后指定的帧在原始流中的帧位置
	@param nFrameIndex 变速后的帧位置
	@param pOriginFrameIndex [out] 原始流中的帧位置

	@return 标准错误
	*/
	virtual HRESULT STDMETHODCALLTYPE GetOriginFrameIndex(int nFrameIndex, int *pOriginFrameIndex) = 0;

	/**
	@brief 获取原始流某一帧在变速之后的流中对应的帧位置
	@param nOriginFrameIndex 原始流的帧位置
	@param pFrameIndex [out] 变速后的流中的帧位置

	@return 标准错误
	*/
	virtual HRESULT STDMETHODCALLTYPE GetRenderFrameIndex(int nOriginFrameIndex, int *pFrameIndex) = 0;

	/**
	@brief 获取变速后的总帧数
	@param pFrameCount [out] 保存总帧数
	@return 标准错误码
	*/
	virtual HRESULT STDMETHODCALLTYPE GetRenderFrameCount(int *pFrameCount) = 0;
};                                                                              



#endif //_H_INLESPEEDSHIFT_H_
