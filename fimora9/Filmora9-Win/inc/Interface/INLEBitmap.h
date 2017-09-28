#pragma once

/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  INLEBitmap.h
 *@brief                                
 *@version 1.0                          
 *@author  wangyx                       
 *@date    6/21/2016 15:41
 */

#ifndef _H_INLEBITMAP_H_
#define _H_INLEBITMAP_H_


#include "NLEType.h"
#include "NLEGraphics.h"
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
#include "NLEBitmapDebug.h"

//{4781BE6F-F5F6-9AE8-E46B-00B7C6A0B756}
DEFINE_GUID(CLSID_NLEBitmap, 0x4781BE6F, 0xF5F6, 0x9AE8, 0xE4, 0x6B, 0x00, 0xB7, 0xC6, 0xA0, 0xB7, 0x56);                   
//{8987FA77-0A04-E93A-D02B-AB0FED808CDA}
DEFINE_GUID(IID_INLEBitmap, 0x8987FA77, 0x0A04, 0xE93A, 0xD0, 0x2B, 0xAB, 0x0F, 0xED, 0x80, 0x8C, 0xDA);                         
typedef UINT NLEColor;

class INLEBitmap :public IUnknown
{
public:
	/**
	 *@brief  	填充Bitmap内存，当pAddr为NULL时，创建内存
	 *@param	nWidth 宽
	 *@param	nHeight 高
	 *@param	pAddr 用于填充的数据指针，当前暂不支持,传入NULL即可。
	 *@param	nBitCount 
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料	 
	 */
	virtual HRESULT __stdcall FillContent(INT nWidth, INT nHeight, LPBYTE pAddr, INT nBitCount)  = 0;
	
	/**
	 *@brief  	使用clrValue填充所有像素
	 *@param	clrValue 用于填充的像素值，包括透明度
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料
	 *@note		注意事项
	 */
	virtual HRESULT __stdcall FillColor(NLEColor clrValue) = 0;

	virtual INT __stdcall GetWidth()	= 0;
	virtual INT __stdcall GetHeight()	= 0;
	virtual INT __stdcall GetPitch()	= 0;	
	virtual INT __stdcall GetBitCount() = 0;

	///获取图像数据大小,单位字节。
	virtual INT __stdcall GetSize() = 0;
	///获取图像数据指针
	virtual LPBYTE __stdcall GetBits() const = 0;


	/**
	 *@brief  	显示图像，使用XTraceMonitor.exe工具查看
	 *@return 	HRESULT __stdcall	
	 */
	virtual HRESULT __stdcall Show() = 0;

	virtual HRESULT __stdcall ResetROI() = 0;
	virtual HRESULT __stdcall GetROI(NLERectF &roi) = 0;
	virtual HRESULT __stdcall SetROI(NLERectF &roi) = 0;
	virtual LPBYTE __stdcall GetROIBits() = 0;

	virtual BOOL	__stdcall IsOpaque() = 0;

	BITMAP_DEBUG_INFO
}; 

typedef INLEBitmap INLEImage;
 
//{BA401EFC-6FF7-D61E-9CEC-7C67E3C73804}
DEFINE_GUID(IID_INLEImageSerialize, 0xBA401EFC, 0x6FF7, 0xD61E, 0x9C, 0xEC, 0x7C, 0x67, 0xE3, 0xC7, 0x38, 0x04);                         
class INLEImageSerialize :public IUnknown
{
public:
	virtual HRESULT __stdcall LoadFromFile(LPCWSTR pFullFileName) = 0;
	virtual HRESULT __stdcall SaveToFile(LPCWSTR pFullFileName, LPCWSTR pClsName = L"image/bmp") = 0;
};                                                                              

#endif //_H_INLEBITMAP_H_


