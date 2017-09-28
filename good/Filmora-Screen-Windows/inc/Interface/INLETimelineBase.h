#pragma once

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
#include "INLEProperties.h"
#include "INLERender.h"
#include <vector>
#include "INLEService.h"
#include "INLEIterator.h"
#include "INLEEditBase.h"

//{CD61E0E4-0074-3FD5-4915-951546E34A03}
DEFINE_GUID(CLSID_NLETimelineBase, 0xCD61E0E4, 0x0074, 0x3FD5, 0x49, 0x15, 0x95, 0x15, 0x46, 0xE3, 0x4A, 0x03);                   
//{29FDB4EE-DE39-151E-ED38-06C8CFB45CDA}
DEFINE_GUID(IID_INLETimelineBase, 0x29FDB4EE, 0xDE39, 0x151E, 0xED, 0x38, 0x06, 0xC8, 0xCF, 0xB4, 0x5C, 0xDA); 

/**
 *@ingroup GroupTimeline
 *@brief Timeline基础接口
 *	- 是否有音视频等，通过查询到 INLEProperties 接口后得到。
 *	- 复制Timeline，可通过查询 INLEClonable 接口
 *	- 保存Timeline，可通过查询 INLESerialize 接口
 *	- 渲染Timline，可通过GetRender得到音视频渲染INLERender
 *	- 通过Index获得Clip接口
 */
class INLETimelineBase :public INLEEditBase
{
public:
	/**
	 *@brief  	新加入Clip到层级为nLevel的最后
	 *@param	pTimeline 新Clip对象	
	 *@return 	标准返回值
	 *@see 		RemoveClip
	 *@note		如果重复加入，只有一个引用，但仍会返回成功。
	 */
	virtual HRESULT __stdcall AddClip(INLETimelineBase *pTimeline) = 0;

	/**
	 *@brief  	获取Clip个数。不递归计算枝叶
	 *@param	pCount
	 *@return 	标准返回值
	 *@note		为空时返回0
	 */
	virtual HRESULT __stdcall GetClipCount(INT* pCount) = 0;
	
	/**
	 *@brief  	交换iIndexFrom和iIndexTo的位置。
	 *@param	iIndexFrom 前
	 *@param	iIndexTo 后
	 *@return 	标准返回值
	 */
	virtual HRESULT __stdcall MoveClip(UINT iIndexFrom, UINT iIndexTo = -1) = 0;

	/**
	 *@brief  	移除Clip
	 *@param	pTimline
	 *@return 	标准返回值
	 */
	virtual HRESULT __stdcall RemoveClip(INLETimelineBase* pClip) = 0;

	/**
	 *@brief  	移除所有Clip
	 *@return 	标准返回值
	 */
	virtual HRESULT __stdcall RemoveAll() = 0;

	/**
	 *@brief  	获取iIndex对应的Clip
	 *@param	guid Clip的唯一标识符
	 *@param	ppTimeline Clip指针地址
	 *@return 	标准返回值
	 */
	virtual HRESULT __stdcall GetClip(GUID guid, INLETimelineBase **ppTimeline) = 0;
		
	/**
	*@brief  	获取rng对应的Clip
	*@param	rng 时间线上面的位置，单位：帧数, 
	*@param	vecClips Clip的guid列表
	*@return 	标准返回值
	*/
	virtual HRESULT __stdcall GetClips(NLERange &rng, std::vector<GUID>& vecClips) = 0;

	/**
	*@brief  	获取遍历器
	*@param	pIterator 遍历接口
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*@note		注意事项
	*/
	virtual HRESULT __stdcall GetIterator(INLEIterator **pIterator) = 0;

	/**
	*@brief 获取GUID，GUID是timeline的全局标识，可通过GUID查询timeline。
	*@param pGuid 保存获取到的GUID值
	*@return 标准返回值
	*/
	virtual HRESULT __stdcall GetGUID(GUID *pGuid) = 0;
};
