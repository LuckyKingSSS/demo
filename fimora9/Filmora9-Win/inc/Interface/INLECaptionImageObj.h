#include <Guiddef.h>                                                            
#include <objbase.h>    
#include "NLECaptionDefine.h"

#ifndef _NLE_IMAGE_PROPERTY_TYPE_
#define _NLE_IMAGE_PROPERTY_TYPE_

// {6AED858B-1CBA-4fd9-810F-C71812FABC45}
DEFINE_GUID(CLSID_INLECaptionImageObj,
	0x6aed858b, 0x1cba, 0x4fd9, 0x81, 0xf, 0xc7, 0x18, 0x12, 0xfa, 0xbc, 0x45);

// {01741C6A-F0D4-46ad-A1B1-A8C6C951B589}
DEFINE_GUID(IID_INLECaptionImageObj,
	0x1741c6a, 0xf0d4, 0x46ad, 0xa1, 0xb1, 0xa8, 0xc6, 0xc9, 0x51, 0xb5, 0x89);


class INLECaptionImageObj :public IUnknown
{
public:	
	/**
	*@brief     设置图片路径参数
	*@param pimgObj 图片路径信息
	*@return
	*/
	virtual void __stdcall SetImageParam(const NLEImageParam* pimgObj) = 0;
	/**
	*@brief     设置图片路径参数
	*@param pimgObj 图片路径信息
	*@return
	*/
	virtual bool __stdcall SetImagePath(BSTR imagePath) = 0;
};


#endif