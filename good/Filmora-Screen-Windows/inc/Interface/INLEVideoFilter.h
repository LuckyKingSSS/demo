
#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>

#ifndef _NLE_FILTER_PROPERTY_TYPE_
#define _NLE_FILTER_PROPERTY_TYPE_
/**
@brief 滤镜参数类型
*/
typedef enum enumNLEFilterPropertyType
{
	NLE_FILTER_PROPERTY_TYPE_INVALID,
	NLE_FILTER_PROPERTY_TYPE_BOOLEAN,
	NLE_FILTER_PROPERTY_TYPE_INT,
	NLE_FILTER_PROPERTY_TYPE_DOUBLE,
	NLE_FILTER_PROPERTY_TYPE_FPOINT,
	NLE_FILTER_PROPERTY_TYPE_FSIZE,
	NLE_FILTER_PROPERTY_TYPE_FRECT,
	NLE_FILTER_PROPERTY_TYPE_FCOLOR,
}NLEFilterPropertyType;

                                                                                
//{A5BD36B8-CE3C-6CB3-2F48-6DF82AD348E0}
DEFINE_GUID(CLSID_NLEVideoFilter, 0xA5BD36B8, 0xCE3C, 0x6CB3, 0x2F, 0x48, 0x6D, 0xF8, 0x2A, 0xD3, 0x48, 0xE0);                   
//{AB0818A4-A6E3-32AF-D63C-075CAEEE297E}
DEFINE_GUID(IID_INLEVideoFilter, 0xAB0818A4, 0xA6E3, 0x32AF, 0xD6, 0x3C, 0x07, 0x5C, 0xAE, 0xEE, 0x29, 0x7E);   
/**
 *@brief 视频Filter对应的Timeline
 */
class INLEVideoFilter :public IUnknown
{
public:
	virtual INT __stdcall GetFilterPropertyCount() = 0;
	virtual HRESULT __stdcall GetFilterProperty(int nIndex, INLEProperties **pProperties) = 0;
};

#endif