#include <Guiddef.h>                                                    
#include <objbase.h>     


#ifndef _NLE_FONT_LIBRARY_TYPE_
#define _NLE_FONT_LIBRARY_TYPE_


// {81BEBF39-9189-425f-85B8-C0EC058E4953}
DEFINE_GUID(CLSID_NLEFontLibrary,
	0x81bebf39, 0x9189, 0x425f, 0x85, 0xb8, 0xc0, 0xec, 0x5, 0x8e, 0x49, 0x53);
 
// {5AC81DBF-6854-4aed-97E2-55A576A96B1D}
DEFINE_GUID(IID_INLEFontLibrary,
	0x5ac81dbf, 0x6854, 0x4aed, 0x97, 0xe2, 0x55, 0xa5, 0x76, 0xa9, 0x6b, 0x1d);


class INLEFontLibrary :public IUnknown
{
public:
	/**
	*@brief     加载字体
	*@param  wszFontPath 字体路径
	*@param  nType DType加载字体模式
	*@return
	*/
	virtual BOOL __stdcall AddFont(const wchar_t* wszFontPath, int nType) = 0;
	/**
	*@brief     获取字体个数 
	*@return 字体个数
	*/
	virtual int  __stdcall GetFontCount() = 0;
	/**
	*@brief     获取字体名称
	*@param  nIndex  索引
	*@param  pFontName  字体名
	*@return
	*/
	virtual BOOL __stdcall GetFontName(int nIndex, wchar_t* pFontName) = 0;
};

#endif