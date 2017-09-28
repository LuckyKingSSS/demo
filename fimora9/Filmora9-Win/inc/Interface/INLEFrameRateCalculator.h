/* ### WS@H Project:NLE ### */
/***********************************************************************/
/**
* Copyright(c) Wondershare software Corporation
* All rights reserved.
*
* @file		NLEFrameRateCalculator.h
* @brief	用于计算视频帧率
* @version	1.0
* @author	wangyx
* @date		2015/4/30
*/
/***********************************************************************/

#ifndef NLE_FRAMERATE_CALCULATOR_H
#define NLE_FRAMERATE_CALCULATOR_H

#include <objbase.h>

typedef enum 
{
	FrameRateCalcModeFloor, //向下舍入
	FrameRateCalcModeCeil,  //向上舍入
	FrameRateCalcModeRound  //四舍五入
}FrameRateCalcMode;

/**
* @name IFrameRateCalculator
* @{
* @brief 定义IFrameRateCalculator的公共接口
* @note 非线程安全，如果需要线程安全请使用PTSToFrameNumber/FrameNumberToPTS/SortTimeByFrameRate
*
*/

// {FE3353AC-6667-4e99-9403-7539B3A533C4}
DEFINE_GUID(IID_IFrameRateCalculator, 
			0xfe3353ac, 0x6667, 0x4e99, 0x94, 0x3, 0x75, 0x39, 0xb3, 0xa5, 0x33, 0xc4);
EXTERN_C const IID IID_IFrameRateCalculator;
//MACRO_MIDL_INTERFACE(IFrameRateCalculator, "FE3353AC-6667-4e99-9403-7539B3A533C4")
class IFrameRateCalculator : public IUnknown
{
public:
	/*
	 *@brief 设置帧率值及起始偏移
	 *@param fps [in] float 帧率值，例如25, 23.97。
	 *@param startOffset [in] LONGLONG 起始帧偏移，通常为0
	 *@return 设置成功，返回S_OK，对不支持帧率等，将返回E_FAIL
	 */
	virtual HRESULT __stdcall SetFrameRate(float fps, NLElonglong startOffset = 0) = 0;
	/*
	 *@brief 由时间戳获取到对应的帧号
	 *@param frmTime [in] LONGLONG 时间戳
	 *@return 返回帧号,若无此帧，则返回-1
	 */
	virtual int __stdcall GetFrameNumber(NLElonglong frmTime) = 0;
	
	/*
	 *@brief 由帧号得到对应的时间戳
	 *@param nFrmNumber [in] int 帧号
	 *@return 返回时间戳，若对应帧则返回-1
	 */
	virtual	NLElonglong __stdcall GetFrameTime(int nFrmNumber) = 0;

	/*
	 *@brief 修正时间戳，对齐到已设置的FrameRate，使用四舍五入方法。
	 *@param frmTime 输入时间
	 *@return 修正过的时间
	 *@sa SetFrameRate
	 *@note 如果为25帧（40Ms），ModifyFrameTime(41)结果为40, ModifyFrameTime(65)结果为80
	 */
	virtual NLElonglong __stdcall ModifyFrameTime(NLElonglong frmTime) = 0;
};
/** @} */

extern "C" HRESULT __stdcall  GetShareFrameRateCalculator(IFrameRateCalculator** ppvobj);

/**@brief 得到最接近的帧率值*/
//extern "C" const float GetRealFrameRate(const float fps);

/**
@brief 把帧数转换为时间，单位是100納秒(100ns)
*/
extern "C" NLElonglong __stdcall FrameNumberToPTS(int nFrame, double dFrameRate);

/**
@brief 把时间转换为帧数，单位是100納秒(100ns)
*/
extern "C" int __stdcall PTSToFrameNumber(NLElonglong llTime, double dFrameRate);

/**
@brief 单位是100納秒(100ns)
*/
extern "C" NLElonglong __stdcall SortTimeByFrameRate(NLElonglong llTime, double dFrameRate);

extern "C" const BOOL IsFrameCalculatorSupport(float fps);

#endif // NLE_FRAMERATE_CALCULATOR_H
