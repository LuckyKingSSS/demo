#pragma once

#include "NLEType.h"
#include <Guiddef.h>
#include <objbase.h>

#include "INLEProperties.h"
#include "INLEVideoFilter.h"
                                                                                
//{8CF6DEAE-A9AD-64D0-7D52-DF9006209D4B}
DEFINE_GUID(CLSID_NLEMarkdown, 0x8CF6DEAE, 0xA9AD, 0x64D0, 0x7D, 0x52, 0xDF, 0x90, 0x06, 0x20, 0x9D, 0x4B);                   
//{AC8050CA-49F8-BAAC-909A-7152FD14AFB5}
DEFINE_GUID(IID_INLEMarkdown, 0xAC8050CA, 0x49F8, 0xBAAC, 0x90, 0x9A, 0x71, 0x52, 0xFD, 0x14, 0xAF, 0xB5);    

//编辑区位置
typedef NLEFrameIndex	NLEMarkPosition;
typedef NLElonglong		NLEMarkLength;
typedef NLEFrameIndex	NLEMarkDuration;
typedef NLERange		NLEMarkRange;

//渲染位置
typedef NLEFrameIndex	NLERenderPosition;
typedef NLEFrameIndex	NLERenderDuration;
typedef NLERange		NLERenderRange; 

/**
 *@brief 标记段接口，可查询出以下接口： 
 *-	INLELightRoomProcessor(TODO)
 *-	INLECubeLUT(TODO)
 */
class INLEMarkdown : public IUnknown
{
public:

	/**
	 *@brief  	修改标记点范围，若为停顿标记段，则不能设置
	 *@param	rngMark
	 *@return 	HRESULT __stdcall
	 *@see 		AddMarkdown
	 */
	virtual HRESULT __stdcall SetRange(NLEMarkRange rngMark) = 0;

	/**
	 *@brief  	获取标记点时间段
	 *@param	pRngMark，停顿标记段帧数为1
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料
	 *@note		注意事项
	 */
	virtual HRESULT __stdcall GetRange(NLEMarkRange* pRngMark) = 0;

	/**
	 *@brief  	SetReverse，设置倒放
	 *@param	bEnable，是否开启倒放
	 *@param	bKeepAudio，是否保留音频，仅在bEnable为TRUE时有意义，取值如下：
	 *-TRUE		音频倒放
	 *-FALSE	音频静音
	 *@return 	HRESULT __stdcall 
	 *@see 		参考资料
	 *@note		注意事项
	 */
	virtual HRESULT __stdcall SetReverse(NLEBOOL bEnable, NLEBOOL bKeepAudio = TRUE) = 0;
	virtual HRESULT __stdcall GetReverse(NLEBOOL *pEnable, NLEBOOL* pKeepAudio) = 0;

	/**
	 *@brief  	SetSpeed，设置速度
	 *@param	ralSpeed 速度值，>= 0, 意义如下:
	 *-			< 1 慢放倍数
	 *-			= 1 原速
	 *-			> 1 快放位数
	 *@return 	HRESULT __stdcall
	 *@see 		GetSpeed
	 */
	virtual HRESULT __stdcall SetSpeed(NLERational ralSpeed) = 0;
	virtual HRESULT __stdcall GetSpeed(NLERational* pSpeed) = 0;

	

	/**
	 *@brief  	SetReplay，设置重播次数
	 *@param	nTimes 次数，>= 0
	 *@return 	HRESULT __stdcall
	 *@see 		GetReplay
	 */
	virtual HRESULT __stdcall SetReplay(INT nTimes) = 0;
	virtual HRESULT __stdcall GetReplay(INT* pTimes) = 0;


	virtual HRESULT __stdcall SetSpeedEnable(NLEBOOL bEnable) = 0;
	virtual HRESULT __stdcall GetSpeedEnable(NLEBOOL& bEnable) = 0;

	virtual HRESULT __stdcall SetReplayEnable(NLEBOOL bEnable) = 0;
	virtual HRESULT __stdcall GetReplayEnable(NLEBOOL& bEnable) = 0;	
};


#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
                                                                                
//{3227CBCF-71B0-AE3F-5E20-D4FF06C3057E}
DEFINE_GUID(CLSID_INLEFreeze, 0x3227CBCF, 0x71B0, 0xAE3F, 0x5E, 0x20, 0xD4, 0xFF, 0x06, 0xC3, 0x05, 0x7E);
//{DD49F36D-BC1E-A645-3C33-5AEB3DAF8FD4}
DEFINE_GUID(IID_INLEFreeze, 0xDD49F36D, 0xBC1E, 0xA645, 0x3C, 0x33, 0x5A, 0xEB, 0x3D, 0xAF, 0x8F, 0xD4);

typedef enum 
{
	///仅第一次起用
	FreezeAvaliableType_FirstRound,	
	///仅最后一次起作用(次数>1时)
	FreezeAvaliableType_LastRound,
	///所有次数都起作用(次数>1时)
	FreezeAvaliableType_AllRounds,
}FreezeAvaliableType;

class INLEFreeze :public IUnknown
{
public:	
	/**
	*@brief  	SetFreeze，设置停顿的帧数
	*@param		nFreezeMarkDuration，帧数
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*@note		注意事项
	*/
	virtual HRESULT __stdcall SetFreeze(NLEMarkDuration nFreezeMarkDuration) = 0;
	virtual HRESULT __stdcall GetFreeze(NLEMarkDuration* pFreezeMarkDuration) = 0;

	virtual HRESULT __stdcall GetPosition(NLEMarkPosition* pPosition) = 0;
	
	/**
	 *@brief 设置定帧起作用的类型
	 */
	virtual HRESULT __stdcall SetAvaliableType(FreezeAvaliableType eAvaliabeType) = 0;
	virtual HRESULT __stdcall GetAvaliableType(FreezeAvaliableType* pAvaliableType) = 0;
};

//{7EDBD92C-636A-886F-7AB9-12A940BA709D}
DEFINE_GUID(CLSID_NLEMarkBook, 0x7EDBD92C, 0x636A, 0x886F, 0x7A, 0xB9, 0x12, 0xA9, 0x40, 0xBA, 0x70, 0x9D);
//{B54FC124-2B1F-DAC4-D9A0-F4BCAB1A1947}
DEFINE_GUID(IID_INLEMarkBook, 0xB54FC124, 0x2B1F, 0xDAC4, 0xD9, 0xA0, 0xF4, 0xBC, 0xAB, 0x1A, 0x19, 0x47);

/**
*@brief 标记蓝本接口，可查询出以下接口：
*- INLERepair 视频修复
*- INLELightRoomProcessor 色彩调节
*- IID_INLECubeLUT 光影调节
*/
class INLEMarkBook :public IUnknown
{
public:		
	/**
	 *@brief  	加入标记段
	 *@param	[out] ppMarkdown 标记段
	 *@param	[in ] rngMark 标记段范围，取值如下：
	 *			- 若帧数为1，则为停顿标记段
	 *			- 若帧数大于1，则为普通标记段
	 *@return 	HRESULT __stdcall
	 *@see 		RemoveMarkdown
	 *@note		默认为最短长度2帧，若不足则返回失败
	 */
	virtual HRESULT __stdcall AddMarkdown(NLEMarkRange rngMark, INLEMarkdown** ppMarkdown) = 0;

	/**
	 *@brief  	移除标记段
	 *@param	pMarkdown 标记段
	 *@return 	HRESULT __stdcall
	 *@see 		AddMarkdown
	 */
	virtual HRESULT __stdcall RemoveMarkdown(INLEMarkdown* pMarkdown) = 0;

	/**
	 *@brief  	GetMarkdownCount，标记段个数
	 *@param	pCount
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料
	 *@note		注意事项
	 */
	virtual HRESULT __stdcall GetMarkdownCount(INT *pCount) = 0;

	/**
	 *@brief  	GetMarkdown，获取第nIndex个Markdown对象
	 *@param	nIndex
	 *@param	pMarkdown
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料
	 *@note		注意事项
	 */
	virtual HRESULT __stdcall GetMarkdownFromIndex(INT nIndex, INLEMarkdown** pMarkdown) = 0;

	virtual HRESULT __stdcall AddFreeze(NLEMarkPosition nMarkPos, INLEFreeze** pFreeze) = 0;

	virtual HRESULT __stdcall RemoveFreeze(INLEFreeze* pFreeze) = 0;

	virtual HRESULT __stdcall GetFreezeCount(INT* pCount) = 0;

	virtual HRESULT __stdcall GetFreezeFromIndex(INT nIndex, INLEFreeze** pFreeze) = 0;

	
	/**
	 *@brief  	GetMarkPosition，将渲染时的位置转换为标记点位置，
	 *@param	posRender
	 *@param	pPosMark
	 *@return 	HRESULT __stdcall
	 *@see 		GetRenderPosition
	 *@note		GetRenderPosition与之相反
	 */
	//virtual HRESULT __stdcall GetMarkPosition(NLERenderPosition posRender, NLEMarkPosition *pPosMark) = 0;

	virtual HRESULT __stdcall GetRenderPosition(NLEMarkPosition posMark, NLERenderPosition* pPosRender) = 0;

	/**
	 *@brief  	GetMarkDuration，获取Mark总长度，帧数
	 *@param	pMarkFrameCount，总帧数
	 *@return 	HRESULT __stdcall
	 */
	virtual HRESULT __stdcall GetMarkDuration(NLEMarkDuration *pMarkFrameCount) = 0;

	/**
	*@brief  	GetRenderDuration，获取渲染总长度，帧数
	*@param		pRenderFrameCount，总帧数
	*@note		在属性发生变化时都要刷新该值，例如设置某个Markdown的Range
	*/
	virtual HRESULT __stdcall GetRenderDuration(NLERenderDuration *pRenderFrameCount) = 0;

	/**
	*@brief  	SetRenderMode，设置显示模式
	*@param		lMode(0:normal,1:LeftRightTwo,2:LeftRightHalf,3:TopBottomHalf,4:Original)
	*@note		
	*/
	virtual HRESULT __stdcall SetRenderMode(LONG lMode) = 0;

	/**
	*@brief  	SetRenderMode，获取当前显示模式
	*@param		plMode 模式值
	*@note
	*/
	virtual HRESULT __stdcall GetRenderMode(LONG *plMode) = 0;

	/**
	*@brief  	GetWardPosition,获取下一个前进/后退的点
	*@param	[in ] nCur 当前位置，可以不是标准的对齐点
	*@param	[out] pNext 下一个
	*@param	[in ] bForward 是否是为前进，TRUE：前进 FALSE：后退
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*@note		若nCur为最后一帧且bForward为TRUE，则返回0；
	*@note		若nCur为第0帧且bForward为FALSE,则返回最后一帧
	*/
	virtual HRESULT __stdcall GetWardPosition(NLEMarkPosition nCur, NLEMarkPosition *pNext, NLEBOOL bForward) = 0;
};