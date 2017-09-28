
#ifndef _NLE_TIPTEXT_PROPERTY_TYPE_
#define _NLE_TIPTEXT_PROPERTY_TYPE_

#include <Guiddef.h>                                                     
#include <objbase.h>       



// {3985BF28-E025-4905-B651-673FE33CE2C8}
DEFINE_GUID(CLSID_INLECaptionTipTextObj,
	0x3985bf28, 0xe025, 0x4905, 0xb6, 0x51, 0x67, 0x3f, 0xe3, 0x3c, 0xe2, 0xc8);


// {9B45CD7D-136B-4144-B9F9-C1F799B2777D}
DEFINE_GUID(IID_INLECaptionTipTextObj,
	0x9b45cd7d, 0x136b, 0x4144, 0xb9, 0xf9, 0xc1, 0xf7, 0x99, 0xb2, 0x77, 0x7d);



class INLECaptionTipTextObj :public IUnknown
{
public:
	/**
	*@brief     设置图片路径参数
	*@param pimgObj 图片路径信息
	*@return
	*/
	virtual bool __stdcall GetShapeInterface(void* pObj) = 0;
	/**
	*@brief     设置图片路径参数
	*@param pimgObj 图片路径信息
	*@return
	*/
	virtual bool __stdcall GetTextInterface(void* pObj) = 0;
};


#endif