
#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
                                                                                
//{0E49A56E-29CE-3A08-3B7F-B139DC94D01E}
DEFINE_GUID(CLSID_MemoryPool, 0x0E49A56E, 0x29CE, 0x3A08, 0x3B, 0x7F, 0xB1, 0x39, 0xDC, 0x94, 0xD0, 0x1E);                   
//{01BB24AB-B15D-45EC-2A03-9DE9D0E0C3E0}
DEFINE_GUID(IID_IMemoryPool, 0x01BB24AB, 0xB15D, 0x45EC, 0x2A, 0x03, 0x9D, 0xE9, 0xD0, 0xE0, 0xC3, 0xE0);                        

/**@ingroup GroupSDK
 * @brief 内存池组件
 *	- 解决内存碎片问题，提高内存利用率
 *	- 本模块可复用到其它应用中
 */
class IMemoryPool :public IUnknown
{
public:
	
	/**
	 *@brief  	TODO
	 *@param	pTodo
	 *@return 	HRESULT STDMETHODCALLTYPE
	 *@see 		参考资料
	 *@note		注意事项
	 */
	virtual HRESULT STDMETHODCALLTYPE TODO(void* pTodo) = 0;
};                                                                              
