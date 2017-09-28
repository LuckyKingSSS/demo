
#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
                                                                                
//{0E49A56E-29CE-3A08-3B7F-B139DC94D01E}
DEFINE_GUID(CLSID_NLEMemoryPool, 0x0E49A56E, 0x29CE, 0x3A08, 0x3B, 0x7F, 0xB1, 0x39, 0xDC, 0x94, 0xD0, 0x1E);                   
//{01BB24AB-B15D-45EC-2A03-9DE9D0E0C3E0}
DEFINE_GUID(IID_INLEMemoryPool, 0x01BB24AB, 0xB15D, 0x45EC, 0x2A, 0x03, 0x9D, 0xE9, 0xD0, 0xE0, 0xC3, 0xE0);                        

/**@ingroup GroupSDK
 * @brief 内存池组件
 *	- 解决内存碎片问题，提高内存利用率
 *	- 本模块可复用到其它应用中
 */
class INLEMemoryPool :public IUnknown
{
public:
	/**
	*@brief 初始化内存池
	*@return 标准返回值
	*/
	virtual HRESULT __stdcall Init() = 0;

	/**
	*@brief 设置内存池信息，控制内存池的内存分配策略
	*@param nMinBlockSize 一次申请的内存块的最小大小，单位是byte,默认为512KB
	*@param nMaxBlockSize 一次申请的内存块的最大大小，单位是byte,默认为100MB
	*@return 标准返回值
	*/
	virtual HRESULT __stdcall SetInfo(int nMinBlockSize = 512 * 1024, int nMaxBlockSize = 100 * 1024 * 1024) = 0;

	/**
	*@brief 申请内存块
	*@param nSize 指定需要申请的内存块的大小，单位是byte
	*@param ppBuf [out]返回申请到的内存块的首地址，若失败，则返回为NULL
	*@return 标准返回值
	*/
	virtual HRESULT __stdcall Alloc(int nSize, void **ppBuf) = 0;

	/**
	*@brief 释放内存块
	*@param pBuf 需要释放的内存块的首地址
	*@return 标准返回值
	*/
	virtual HRESULT __stdcall Free(void *pBuf) = 0;
};                                                                              
