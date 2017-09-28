#pragma once
/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  ILadspaPlugin.h
 *@brief 音频特效插件。                               
 *@version 1.0                          
 *@author  wangyx                       
 *@date    5/6/2016 16:15
 */

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
#include "NLEComPtr.h"
                                                                                
//{3E486ACA-44FB-6AB5-9388-57850FE860ED}
DEFINE_GUID(CLSID_LadspaPlugin, 0x3E486ACA, 0x44FB, 0x6AB5, 0x93, 0x88, 0x57, 0x85, 0x0F, 0xE8, 0x60, 0xED);                   
//{1393812A-A68D-8664-B2F3-0220FDB7F712}
DEFINE_GUID(IID_ILadspaPlugin, 0x1393812A, 0xA68D, 0x8664, 0xB2, 0xF3, 0x02, 0x20, 0xFD, 0xB7, 0xF7, 0x12);                    


struct Variant
{
	int port_idx;
	int is_out;
	float lower;
	float defaultValue;
	float current;
	float upper;
	int multiply;
};

/**
*@ingroup ExternLib
*@brief 音频插件处理接口使用步骤如下:
*	- 1.LoadRuntimeLibrary 加载插件运行时库
*	- 2.GetSurpportInputChannelCount/GetSurpportOutputChannelCount检查支持的in/out声道数
*	- 3.GetParamValue/SetParamValue 设置参数
*	- 4.SetInputData 设置输入数据指针
*	- 5.Run 执行处理
*	- 6.获取处理后的结果，可没有下一次Run之前可多次获取。
*/
class ILadspaPlugin :public IUnknown
{
public:

	/**
	 *@brief  	加载动态库
	 *@param	dllPath 全路径
	 *@param	sampleRate 处理数据的采样率，输入和输出的采样率
	 *@return 	标准返回	 
	 *@note		注意事项
	 */
	virtual  HRESULT __stdcall LoadRuntimeLibrary(LPCWSTR pszDllFullPath, unsigned int nInOutSamplerate = 44100) = 0;
		
	/**
	 *@brief 激活运行时库，必须在加载(LoadRuntimeLibrary)之后
	 *@param bActive 是否激活
	 *@see	 LoadRuntimeLibrary	
	 */
	virtual HRESULT __stdcall Active(BOOL bActive = TRUE) = 0;

	/**
	 *@brief  	获取该插件支持的输入音频数据的声道数
	 *@param	pCount
	 */
	virtual  HRESULT __stdcall GetSurpportInputChannelCount(INT *pCount) = 0;

	/**
	*@brief  	获取支持的输出声道数
	*@param		pCount
	*/
	virtual  HRESULT __stdcall GetSurpportOutputChannelCount(INT *pCount) = 0;

	/**
	*@brief  	获取参数个数
	*@param		pCount
	*/
	virtual  HRESULT __stdcall GetParamCount(UINT *pCount) = 0;


	/**
	 *@brief  	获取第nIndex个参数名称
	 *@param	nIndex 序号
	 *@param	ppName 名称
	 *@return 	HRESULT __stdcall
	 */
	virtual  HRESULT __stdcall GetParamName(const int nIndex, const char** ppName) = 0;

	/**
	 *@brief  	获取参数值
	 *@param	name 参数名称
	 *@param	pValue 参数值
	 *@return 	HRESULT __stdcall
	 */
	virtual  HRESULT __stdcall GetParamValue(const char* name, Variant* pValue) = 0;

	/**
	 *@brief  	设置参数
	 *@param	name 名称
	 *@param	value 参数值
	 *@return 	HRESULT __stdcall
	 */
	virtual  HRESULT __stdcall SetParamValue(const char* name, float value) = 0;


	/**
	 *@brief  	设置输入数据
	 *@param	pInData 数据地址
	 *@param	nSize 数据大小
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料
	 *@note		1)输入数据的声道数必须与GetSurpportInputChannelCount值相同
	 */
	virtual HRESULT __stdcall SetInputData(BYTE  *pInData, INT nSize) = 0;

	/**
	 *@brief  	Run
	 *@return 	HRESULT __stdcall
	 */
	virtual HRESULT __stdcall Run() = 0;

	/**
	 *@brief  	GetOutputData
	 *@param	pOutData
	 *@param	pSize
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料
	 *@note		注意事项:
	 *-			1)输出数据的声道数与GetSurpportOutputChannelCount相同;
	 *-			2)如果没有再次执行Run，则仍为上次执行的数据
	 */
	virtual HRESULT __stdcall GetOutputData(BYTE **pOutData, INT *pSize) = 0;

	/**
	 *@brief 当前是否激活状态
	 */
	virtual HRESULT __stdcall GetState(BOOL* bActive) = 0;

	
	/**
	 *@brief  	设置Plugin的ID值，用于唯一标识该插件
	 *@param	uID ID值，LadspaPluginID_xxx
	 *@return 	HRESULT __stdcall
	 *@see 		GetPluginID
	 *@note		此接口是可选调用的
	 */
	virtual HRESULT __stdcall SetPluginID(const UINT uID) = 0;

	/**
	 *@brief  	获取该插件的ID
	 *@param	pID ID，ID值，见LadspaPluginID_xxx
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料
	 *@note		若未设置，则为LadspaPluginID_Unknown
	 */
	virtual HRESULT __stdcall GetPluginID(UINT* pID) = 0;
};

typedef ILadspaPlugin INLEAudioProcessor;

typedef enum
{
	LadspaPluginID_Unknown,
	LadspaPluginID_Shifter,
	LadspaPluginID_VolumeAmplifer,
	LadspaPluginID_Mixer,
	LadspaPluginID_Max,
}LadspaPluginID;

//{5F8E04E3-69E3-E253-4A7E-AFE96CC6214E}
DEFINE_GUID(IID_IPitchShifter, 0x5F8E04E3, 0x69E3, 0xE253, 0x4A, 0x7E, 0xAF, 0xE9, 0x6C, 0xC6, 0x21, 0x4E);


#define LIBNAME_LENGTH_MAX
/**
 *@brief 变速接口
 */
class IPitchShifter :public IUnknown
{
public:
	/**
	 *@brief  	当前倍数
	 *@param	oRal
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料
	 *@note		注意事项
	 */
	virtual HRESULT __stdcall SetScale(const NLERational &oRal) = 0;
	 
	/**
	 *@brief  	设置倍数，倍数取数据伸缩长度，例如0.5，则数据长度为原来的1/2,即为变快。
	 *@param	pRal
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料
	 *@note		注意事项
	 */
	virtual HRESULT __stdcall GetScale(NLERational* pRal) = 0;

	/**
	 *@brief 获取音调值
	 */
	virtual HRESULT __stdcall GetSemitons(INT *pSemitons) = 0;
	/**
	 *@brief 设置音调值, [-12, 12]
	 */
	virtual HRESULT __stdcall SetSemitons(INT nSemitons) = 0;
};
