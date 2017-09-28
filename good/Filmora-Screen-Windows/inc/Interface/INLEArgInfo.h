#pragma once

#ifndef _H_INLEEDITINFO_H_
#define _H_INLEEDITINFO_H_

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            

enum enumNLEArgInfoType
{
	NLEArgInfoType_IntegerRange,			//带范围的整形
	NLEArgInfoType_DoubleRange,				 //带范围的整形
	NLEArgInfoType_Integer,					//整形
	NLEArgInfoType_Double,					//浮点型
	NLEArgInfoType_EasingFuntion,			//缓动动画 
	NLEArgInfoType_String,					 //字符串编辑
	NLEArgInfoType_FileName,			     //文件名
	NLEArgInfoType_Rotate                    //旋转 0， 90， 180， 270
}NLEArgInfoType;

//{3BBA874F-B9E4-5959-F7FF-B5D284D2544D}
DEFINE_GUID(CLSID_NLEArgInfo, 0x3BBA874F, 0xB9E4, 0x5959, 0xF7, 0xFF, 0xB5, 0xD2, 0x84, 0xD2, 0x54, 0x4D);
//{39F82120-1BA6-CE6F-58BD-C47FFCB958B0}
DEFINE_GUID(IID_INLEArgInfo, 0x39F82120, 0x1BA6, 0xCE6F, 0x58, 0xBD, 0xC4, 0x7F, 0xFC, 0xB9, 0x58, 0xB0);

class INLEArgInfo :public IUnknown
{
public:	
	virtual HRESULT __stdcall GetArgInfoType(NLEArgInfoType *pArgInfoType) = 0;
	virtual HRESULT __stdcall GetArgCount(UINT32* pCount) = 0;
	virtual HRESULT __stdcall GetArgCount(UINT32* pCount) = 0;
	virtual HRESULT __stdcall GetArgKey(UINT32 index, NLEATTRI* pAttri) = 0;
	virtual HRESULT __stdcall GetArgyName(UINT32 index, BSTR* pName) = 0;
	virtual HRESULT __stdcall GetArgType(UINT32 index, NLEVariantType* pType) = 0;
	virtual BOOL	__stdcall IsSupportedAnimation(UINT32 index) = 0;
	virtual HRESULT __stdcall GetIntMinMaxValue(UINT32 index, int* pMin, int *pMax) = 0;
	virtual HRESULT __stdcall GetDoubleMinMaxValue(UINT32 index, double* pMin, double *pMax) = 0;
};

#endif //_H_INLEEDITINFO_H_
                                                                            


