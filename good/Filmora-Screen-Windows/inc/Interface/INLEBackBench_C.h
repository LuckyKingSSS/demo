
#ifndef _H_INLEBACKBENCH_H_
#define _H_INLEBACKBENCH_H_

/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  INLEBackBench.h
 *@brief                                
 *@version 1.0                          
 *@author  wangyx                       
 *@date    3/3/2016 15:15
 */

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
                                                       
                                                                                
//{77D4E634-663A-884D-0868-7C8588636364}
DEFINE_GUID(CLSID_NLETaskDelegate, 0x77D4E634, 0x663A, 0x884D, 0x08, 0x68, 0x7C, 0x85, 0x88, 0x63, 0x63, 0x64);                   
//{21C97086-97C4-0AFB-2DE2-D0CC68C2D9A3}
DEFINE_GUID(IID_INLETaskDelegate, 0x21C97086, 0x97C4, 0x0AFB, 0x2D, 0xE2, 0xD0, 0xCC, 0x68, 0xC2, 0xD9, 0xA3);

typedef enum enumTaskState
{
	/**
	 *@brief 正在运行
	 */
	NLETaskStateRunning,
	/**
	*@brief 暂停状态
	*/
	NLETaskStatePaused,
	/**
	 *@brief 完成状态
	 */
	NLETaskStateFinished,

	/**
	 *@brief 出错状态
	 */
	NLETaskStateError,
}NLETaskState;

#define NLETASK_ID_RESERVED 0xFFFFFFFF

/**@ingroup GroupExtension
 *@brief 任务执行代理者
 *@see 代理模式(iOS)
 */
class INLETaskDelegate :public IUnknown
{
public:

	/**
	 *@brief  	任务执行体
	 *@param[in]	pParam 执行时所需要参数
	 *@return 	HRESULT __stdcall	 
	 */
   EXTERN_C NLE_API HRESULT OnRuning(void* pObj, LPVOID pParam = NULL);

	/**
	 *@brief  	通知任务当前执行状态
	 *@param[in]	state 状态值
	 *@param[in]	fProgress 进度，[0, 100]
	 *@return 	标准返回
	 *@see 		NLETaskState
	 */
   EXTERN_C NLE_API HRESULT OnStatus(void* pObj, NLETaskState state, UINT fProgress);
};

//{2C25A154-F06C-493E-BFA6-988EC7F4557D}
DEFINE_GUID(CLSID_NLETask, 0x2C25A154, 0xF06C, 0x493E, 0xBF, 0xA6, 0x98, 0x8E, 0xC7, 0xF4, 0x55, 0x7D);                   
//{68DD06EF-6B01-3745-3411-FE066EACF571}
DEFINE_GUID(IID_INLETask, 0x68DD06EF, 0x6B01, 0x3745, 0x34, 0x11, 0xFE, 0x06, 0x6E, 0xAC, 0xF5, 0x71);

/**@ingroup GroupExtension
 *@brief 任务体接口
 *可查询出 INLESerialize 、 INLEProperties 接口
 */
class INLETask :public IUnknown
{
public:

	/**
	 *@brief  	初始化
	 *@param[in]	pDelegate
	 *@param[in]	pParam 任务执行所需要的参数，与INLETaskDelegate的OnRuning参数匹配
	 *@return 	HRESULT __stdcall
	 *@see 		INLETaskDelegate,OnRuning	 	
	 */
   EXTERN_C NLE_API HRESULT Initialize(void* pObj, UINT uID, INLETaskDelegate*pDelegate, LPVOID pParam);
	
	/**
	 *@brief  	获得任务ID
	 *@return 	ID
	 *@see 		SetTaskID
	 *@note		注意事项
	 */
	virtual UINT	__stdcall GetTaskID() = 0;
	/**
	*@brief  	启动任务
	*@param[in] uID 任务ID，
	*@return 	HRESULT __stdcall
	*@note		当uID为默认值时表示所有任务
	*/
	virtual HRESULT __stdcall Start(UINT uID = NLETASK_ID_RESERVED) = 0;

	/**
	*@brief  	暂停任务
	*@param	uID 任务ID
	*@return 	HRESULT __stdcall
	*@note
	*	- 当uID为默认值时表示所有任务
	*/
	virtual HRESULT __stdcall Pause(UINT uID = NLETASK_ID_RESERVED) = 0;

	/**
	*@brief  	停止任务
	*@param	uID 任务ID
	*@return 	HRESULT __stdcall
	*@see 		Pause/Start
	*@note		任务停止后仍可再启动，除非已完成。
	*/
	virtual HRESULT __stdcall Stop(UINT uID = NLETASK_ID_RESERVED) = 0;
};                                                                              


//{18792432-1B35-F0C6-A2D3-FF11897BC559}
DEFINE_GUID(CLSID_NLEBackBench, 0x18792432, 0x1B35, 0xF0C6, 0xA2, 0xD3, 0xFF, 0x11, 0x89, 0x7B, 0xC5, 0x59);                   
//{5D77A0F8-E584-6BF5-BABB-1F8365667962}
DEFINE_GUID(IID_INLEBackBench, 0x5D77A0F8, 0xE584, 0x6BF5, 0xBA, 0xBB, 0x1F, 0x83, 0x65, 0x66, 0x79, 0x62);                         
/**@ingroup GroupExtension 
 *@brief 后台处理平台，可管理后台所有任务
 * 与 INLETask 配合使用
 */
class INLEBackBench :public IUnknown
{
public:
	/**
	 *@brief  	添加任务到后台
	 *@param[in] pTask 任务
	 *@return 	标准返回
	 *@see 		INLETask
	 *@note		加入时状态为 NLETaskStatePaused
	 */
	virtual HRESULT __stdcall AddTask(INLETask *pTask) = 0;
	/**
	 *@brief  	移除Task
	 *@param[in]	uID
	 *@return 	标准返回
	 *@see 		AddTask
	 */
	virtual HRESULT __stdcall RemoveTask(UINT uID) = 0;
		
	/**
	 *@brief  	获得Task对象指针
	 *@param[in]	uID
	 *@param[out]	ppTask
	 *@return 	标准返回
	 */
	virtual HRESULT __stdcall GetTask(UINT uID, INLETask **ppTask) = 0;

	/**
	 *@brief  	Task数目
	 *@return 	数目
	 */
	virtual UINT	__stdcall GetTaskCount() = 0;


	/**
	 *@brief  	启动任务
	 *@param[in] uID 任务ID，
	 *@return 	HRESULT __stdcall
	 *@note		当uID为默认值时表示所有任务
	 */
	virtual HRESULT __stdcall Start(UINT uID = NLETASK_ID_RESERVED) = 0;

	/**
	*@brief  	暂停任务
	*@param	uID 任务ID
	*@return 	HRESULT __stdcall
	*@note		
	*	- 当uID为默认值时表示所有任务	
	*/
	virtual HRESULT __stdcall Pause(UINT uID = NLETASK_ID_RESERVED) = 0;

	/**
	 *@brief  	停止任务
	 *@param	uID 任务ID
	 *@return 	HRESULT __stdcall
	 *@see 		Pause/Start
	 *@note		任务停止后仍可再启动，除非已完成。
	 */
	virtual HRESULT __stdcall Stop(UINT uID = NLETASK_ID_RESERVED) = 0;
};                                                                              

#endif //_H_INLEBACKBENCH_H_
