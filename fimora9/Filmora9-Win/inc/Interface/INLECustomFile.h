///////////////////////////////////////////////////////////
//  INLECustomFile.h
//  Implementation of the Interface INLECustomFile
//  Created on:      09-三月-2017 13:37:52
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(H_NLE_CUSTOM_FILE_H)
#define H_NLE_CUSTOM_FILE_H

#include "Linux32_Typedef.h"
#include "objbase.hpp"
#include "COMMacro.h"

typedef struct 
{
	INT nWidth;
	INT nHeight;
	INT nPitch;
	INT nFramerateNum;
	INT nFramerateDen;
}CustomFileInfo;

typedef enum
{
	CustomFileMode_Read = 1,
	CustomFileMode_Writer = 2,
	CustomFileMode_WRBoth = 3,
}CustomFileMode;

	// {0FD7EDC7-80D1-4665-994C-E8A20AB5EA90}
DEFINE_GUID(IID_INLECustomFile,
	0xfd7edc7, 0x80d1, 0x4665, 0x99, 0x4c, 0xe8, 0xa2, 0xa, 0xb5, 0xea, 0x90);
EXTERN_C const IID IID_INLECustomFile;
MACRO_MIDL_INTERFACE(INLECustomFile, "0FD7EDC7-80D1-4665-994C-E8A20AB5EA90")
	: public IUnknown
{
	virtual HRESULT SetPath(const WCHAR* pwszFullPath) = 0;
	virtual const WCHAR* GetPath() = 0;
	/**
	 *@brief open file
	 *@note  MUST, SetPath firstly
	 *@sa	 SetPath	
	 */
	virtual HRESULT Open(CustomFileMode fm = CustomFileMode_Read) = 0;
	virtual void Close(INT bRemove) =0;
	virtual HRESULT SetInfo(const CustomFileInfo &oCustomInfo) =0;
	virtual const CustomFileInfo* GetInfo()  = 0;
	/**
	 *@brief 当前缓存文件的长大
	 */
	virtual LONGLONG GetCurrentSize() =0;	
	/**
	 *@brief 当前缓存文件里面的帧总数
	 */
	virtual INT GetFrameCount() = 0;
	virtual HRESULT WriteSample(INT iFrmNumber, BYTE* pData, INT nDataSize, LONGLONG llPts) = 0;
	virtual HRESULT ReadSample(INT iFrmNumber, BYTE* pBuffer, INT nBufferSize, LONGLONG *pllPts) = 0;
	
};

//定义组件
// {C229989C-62F5-4CF3-941F-A2EAA1D9FA1A}
MIDL_DEFINE_GUID(CLSID, CLSID_CNLECustomFile, 0xc229989c, 0x62f5, 0x4cf3, 0x94, 0x1f, 0xa2, 0xea, 0xa1, 0xd9, 0xfa, 0x1a);
EXTERN_C const CLSID CLSID_CNLECustomFile;

#endif // !H_NLE_CUSTOM_FILE_H
