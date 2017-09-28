#pragma once

/**
 * Copyright (c) 2013 Wondershare.inc
 * All rights reserved.
 *
 *@file    INLEVideoRender.h
 *@brief   Video渲染接口
 *@version 1.0
 *@author  wangyx 
 *@date    2016/03/02 15:36
 */
#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>
#include "INLEFrame.h"

                                                                                
//{40CCF070-19E5-061C-8F98-84BB4D75B0DE}
DEFINE_GUID(CLSID_NLERender, 0x40CCF070, 0x19E5, 0x061C, 0x8F, 0x98, 0x84, 0xBB, 0x4D, 0x75, 0xB0, 0xDE);                   
//{05D645DD-3385-6B8A-920F-8B7D6CD011FD}
DEFINE_GUID(IID_INLERender, 0x05D645DD, 0x3385, 0x6B8A, 0x92, 0x0F, 0x8B, 0x7D, 0x6C, 0xD0, 0x11, 0xFD);

//{EE5A61EE-C349-8A6D-A503-CA1FDEEA809D}
DEFINE_GUID(IID_INLECloneRender, 0xEE5A61EE, 0xC349, 0x8A6D, 0xA5, 0x03, 0xCA, 0x1F, 0xDE, 0xEA, 0x80, 0x9D);                         

//{97E5BD2C-2A56-16B2-A14C-9072DFB2A6D5}
DEFINE_GUID(IID_INLEOrignalRender, 0x97E5BD2C, 0x2A56, 0x16B2, 0xA1, 0x4C, 0x90, 0x72, 0xDF, 0xB2, 0xA6, 0xD5);                         

/**
 *@brief 渲染总接口
 *@note 必须第一个调用Seek, 在释放INLERender之前必须要调用Cleanup()来清除资源
 */
class INLERender :public IUnknown
{
public:
	/**
	 *@brief  	为渲染线作准备
	 *@param	pFrame 要Render的帧
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料
	 *@note		注意事项如下：
	 *-			1.此pFrame与 Render 的参数pFrame是同一个对象,
	 *-			2.故可在Prepare中加入一些标识，如果有需要的话;
	 *			
	 */
	virtual HRESULT __stdcall Prepare(INLEFrame*  pFrame) = 0;

	/**
	 *@brief  	执行渲染线
	 *@param	pFrame
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料
	 *@note		注意事项
	 */
	virtual HRESULT __stdcall Render(INLEFrame*  pFrame) = 0;


	/**
	 *@brief  	渲染线定位 nFrameIndex 所在的帧位置
	 *@param	nFrameIndex 帧位置, 表示需要定位到视频的第几帧
	 *@param    pRealFrameIndex 保存实际seek的帧位置
	 *@return 	标准错误
	 */
	virtual HRESULT __stdcall Seek(NLEFrameIndex nFrameIndex, NLEFrameIndex *pRealFrameIndex = NULL) = 0;

	/**
	 *@brief  	渲染线前进nFrameCount帧
	 *@param[in] nFrameCount 前进的帧数，负数时表示后退
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料
	 *@note		注意事项
	 */
	virtual HRESULT __stdcall Forward(INT nFrameCount, INT *pRealFrameCount) = 0;

	/**
	 *@brief  	当前位置
	 *@param	
	 *@return 	int __stdcall
	 *@see 		参考资料
	 *@note		注意事项
	 */
	virtual NLEFrameIndex __stdcall GetOffset(void) = 0;

	/**
	 *@brief  	清理并复位
	 *@return 	HRESULT
	 */
	virtual HRESULT __stdcall Cleanup() = 0;

	/**
	*@brief  	扩展接口
	*@param[in]	uCmd 命令ID，见RENDER_CMD_xxx
	*@param[in][out] wParam 参数1
	*@param[in][out] lParam 参数2
	*@return 	标准返回
	*@see 		参考资料
	*@note		注意事项
	*/
	virtual HRESULT __stdcall SendCommand(UINT uCmd, NLEWParam wParam = NULL, NLELParam lParam = NULL) = 0;
}; 


//{74F57103-09A3-97D0-9885-E5B07C852CA1}
DEFINE_GUID(IID_INLEAudioRender, 0x74F57103, 0x09A3, 0x97D0, 0x98, 0x85, 0xE5, 0xB0, 0x7C, 0x85, 0x2C, 0xA1);  
             
//{26CF902D-C1DD-EEED-472C-5CC5D242CACD}
DEFINE_GUID(IID_INLEVideoRender, 0x26CF902D, 0xC1DD, 0xEEED, 0x47, 0x2C, 0x5C, 0xC5, 0xD2, 0x42, 0xCA, 0xCD);


typedef enum
{
	/**
	 *@brief 获取第nIndex帧音频帧大小(字节数)
	 *@note  wParam NLEFrameIndex类型，帧号
	 *@note  lParam INT* 类型，帧大小
	 */
	RENDER_CMD_AUDIO_FRAME_SIZE = 0x1000,

	/**
	 *@brief 开始渲染
	 *@note  wParam NLEFrameIndex类型，当前渲染位置(在父结点中的位置)
	 *@note  lParam 保留
	 */
	 RENDER_CMD_ENTER_RENDERING,
	 /**
	  *@brief 结束渲染
	  *@note  wParam NLEFrameIndex类型，当前渲染位置(在父结点中的位置)
	  *		  在Cleanup之后收到此命令
	  *@note  lParam 保留
	  */
	  RENDER_CMD_LEAVE_RENDERING,

	  REDNER_CMD_CHILDREN_START,
	  REDNER_CMD_CHILDREN_END,

	  /**
	   *@brief 是否启用倒放
	   *@note wParam NLEBOOL类型，TRUE：启用， FALSE：不启用
	   *@note lParam 保留
	   */
	   RENDER_CMD_BACKPLAY_AVALIABLE,

	   RENDER_CMD_ENTER_SEEKING,
	   /**
	   *@brief 结束渲染
	   *@note  wParam NLEFrameIndex类型，当前渲染位置(在父结点中的位置)
	   *		  在Cleanup之后收到此命令
	   *@note  lParam 保留
	   */
	   RENDER_CMD_LEAVE_SEEKING,

	   /**
		*@brief 渲染到播放器还是编码器?
		*@WPARAM	GUID* 取值如下：
		*-			IID_INLEPlayer 播放器
		*-			IID_INLEncoder 编码器
		*@LPARAM   WCHAR*,关于Sink的描述(用于调试)
		*/
		RENDER_CMD_SINK_GUID,

		/**
		 *@brief 将要更改属性
		 */
		RENDER_CMD_WILL_CHANGE_PROPERTIES,

	   /**
		*@brief 获取当前的mark游标
		*@WPARAM	NLEFrameIndex * ，用于返回值
		*@LPARAM   
		*/
		RENDER_CMD_GET_MARK_OFFSET,

}RenderCmd;
