
#pragma once

/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  INLETransition.h
 *@brief                                
 *@version 1.0                          
 *@author  wangyx                       
 *@date    7/13/2016 11:11
 */

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
#include "INLEBitmap.h"
                                                                                
//{CCF49509-2EDF-949A-04FE-1C41D03D8F65}
DEFINE_GUID(CLSID_NLETransition, 0xCCF49509, 0x2EDF, 0x949A, 0x04, 0xFE, 0x1C, 0x41, 0xD0, 0x3D, 0x8F, 0x65);                   
//{CC63F6BE-1969-07CC-BC9B-9DD02F42C6AD}
DEFINE_GUID(IID_INLETransition, 0xCC63F6BE, 0x1969, 0x07CC, 0xBC, 0x9B, 0x9D, 0xD0, 0x2F, 0x42, 0xC6, 0xAD);   

/**
*@ingroup GroupExtern
*@brief 转场算法接口,调用流程必须如下:
*	- 1.SetTransID
*	- 2.SetOutSize
*	- 3.SetDuration
*	- 4.SetPosition
*	- 5.DoTransition
*@note 可参考TransitionTest.cpp
*/
class INLETransition :public IUnknown
{
public:	

	/**
	 *@brief  	设置ID
	 *@param	szTransID ID字符
	 *@return 	HRESULT __stdcall
	 *@see 		GetTransID
	 *@note		设置时会重置相关资源
	 */
	virtual HRESULT __stdcall SetTransID(LPCWSTR szTransID)		= 0;
	virtual LPCWSTR __stdcall GetTransID(LPCWSTR* ppszTranID)	= 0;	
	
	/**
	*@brief  	SetOutSize
	*@param	nWidth
	*@param	nHeight
	*@return 	HRESULT __stdcall
	*@see 		SetTransID
	*@note		在此之前必须先调用SetTransID
	*/
	virtual HRESULT __stdcall SetOutSize(const INT nWidth, const INT nHeight) = 0;
	virtual HRESULT __stdcall GetOutSize(INT *pWidth, INT *pHeight) = 0;

	/**
	 *@brief  	SetDuration
	 *@param	nTotal 一般设置为100
	 *@see 		SetOutSize
	 *@note		必须先调用SetOutSize
	 */
	virtual HRESULT __stdcall SetDuration(const UINT nTotal) = 0;
	virtual HRESULT __stdcall GetDuration(UINT *pTotal) = 0;
	
	/**
	 *@brief  	SetPosition
	 *@param	nCurrent 当前值，与Duration相对应，在Duration范围内
	 *@return 	HRESULT __stdcall
	 *@see 		SetDuration
	 *@note		必须先调用SetDuration
	 */
	virtual HRESULT __stdcall SetPosition(const UINT nCurrent) = 0;
	virtual HRESULT __stdcall GetPosition(UINT *pCurrent) = 0;
	
		
	/**
	 *@brief  	DoTransition
	 *@param	pFrom 入场帧，不能为NULL
	 *@param	pTo 出场帧，不能为NULL
	 *@param	pOut 输出Image,不得为NULL
	 *@return 	HRESULT __stdcall	 
	 *-			1. pOut可与pFrom或pTo相同，执行完成后覆盖之
	 *-			2. pFrom/pTo/pOut的大小必须相同，否则不执行
	 */
	virtual HRESULT __stdcall DoTransition(INLEImage* pFrom, INLEImage* pTo, INLEImage* pOut) = 0;
};              
