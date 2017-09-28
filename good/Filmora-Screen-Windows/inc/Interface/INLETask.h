#pragma once
/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  INLETask.h
 *@brief 执行任务单元
 *@version 1.0                   
 *@author  wangyx                       
 *@date    7/1/2016 10:29
 */

#ifndef _H_INLETASK_H_
#define _H_INLETASK_H_

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
                                                                                
//{77D4E634-663A-884D-0868-7C8588636364}
DEFINE_GUID(CLSID_NLETaskDelegate, 0x77D4E634, 0x663A, 0x884D, 0x08, 0x68, 0x7C, 0x85, 0x88, 0x63, 0x63, 0x64);
//{21C97086-97C4-0AFB-2DE2-D0CC68C2D9A3}
DEFINE_GUID(IID_INLETaskDelegate, 0x21C97086, 0x97C4, 0x0AFB, 0x2D, 0xE2, 0xD0, 0xCC, 0x68, 0xC2, 0xD9, 0xA3);

//{2C25A154-F06C-493E-BFA6-988EC7F4557D}
DEFINE_GUID(CLSID_NLETask, 0x2C25A154, 0xF06C, 0x493E, 0xBF, 0xA6, 0x98, 0x8E, 0xC7, 0xF4, 0x55, 0x7D);
//{68DD06EF-6B01-3745-3411-FE066EACF571}
DEFINE_GUID(IID_INLETask, 0x68DD06EF, 0x6B01, 0x3745, 0x34, 0x11, 0xFE, 0x06, 0x6E, 0xAC, 0xF5, 0x71);


typedef enum enumTaskState
{
	/**
	*@brief 初始化完成后的状态
	*/
	NLETaskStateReady,

	/**
	*@brief 正在运行
	*/
	NLETaskStateRunning,
	/**
	*@brief 暂停状态
	*/
	NLETaskStatePaused,

	/**
	*@brief 停止状态
	*/
	NLETaskStateStopped,

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
	*@brief  	任务执行体，任务执行过程中会不定期的回调该接口，可用于判断线程运行是否正常，统计线程运行时间等
	*@param[in]	pParam 执行时所需要参数
	*@return 	HRESULT __stdcall
	*/
	virtual HRESULT __stdcall OnRuning(LPVOID pParam = NULL) = 0;

	/**
	*@brief  	通知任务当前执行状态
	*@param[in]	state 状态值
	*@param[in]	uProgress 进度，[0, 100]
	*@return 	标准返回
	*@see 		NLETaskState
	*/
	virtual HRESULT __stdcall OnStatus(NLETaskState state, UINT uProgress, LPVOID pParam = NULL) = 0;
};


/**@ingroup GroupExtension
*@brief 任务体接口
*可查询出 INLESerialize 、 INLEProperties 接口
*/
class INLETask :public IUnknown
{
public:

	/**
	*@brief  	初始化
	*@param[in]	TaskID
	*@param[in]	pDelegate
	*@param[in]	pParam 任务执行所需要的参数，与INLETaskDelegate的OnRuning参数匹配
	*@return 	HRESULT __stdcall
	*@see 		INLETaskDelegate,OnRuning
	*@note		pDelegate代理象不能增加引用计数，否则会引发系统性灾难!!!!!!!!!!
	*@TODO		使用一种方法避免pDelegate增加引用计数.
	*/
	virtual HRESULT __stdcall Initialize(UINT uID, INLETaskDelegate*pDelegate, LPVOID pParam) = 0;

	/**
	*@brief  	获得任务ID
	*@return 	ID
	*@see 		SetTaskID
	*@note		注意事项
	*/
	virtual UINT	__stdcall GetTaskID() = 0;
	/**
	*@brief  	启动任务
	*@return 	HRESULT __stdcall
	*@note		当uID为默认值时表示所有任务
	*/
	virtual HRESULT __stdcall Start(void) = 0;

	/**
	*@brief  	暂停任务
	*@return 	HRESULT __stdcall
	*@note
	*	- 当uID为默认值时表示所有任务
	*/
	virtual HRESULT __stdcall Pause(void) = 0;

	/**
	*@brief  	停止任务
	*@param	uID 任务ID
	*@return 	HRESULT __stdcall
	*@see 		Pause/Start
	*@note		任务停止后仍可再启动，除非已完成。
	*/
	virtual HRESULT __stdcall Stop(void) = 0;

	/**
	 *@brief 是否为可执行的
	 *@return TRUE:可执行 FALSE:不可执行
	 */
	virtual NLEBOOL __stdcall Executable() = 0;
};



#endif //_H_INLETASK_H_
