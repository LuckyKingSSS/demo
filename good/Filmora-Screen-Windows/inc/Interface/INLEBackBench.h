
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
#include "INLETask.h"
                            

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
	 *@brief 获取第nIndex个任务的ID号
	 */
	virtual UINT	__stdcall GetTaskID(INT nIndex) = 0;

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

public:
	/**
	 *@brief 创建一个唯一的ID
	 */
	virtual UINT CreateUniqTaskID() = 0;
};                                                                              

#endif //_H_INLEBACKBENCH_H_
