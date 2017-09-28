
#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
#include "INLETask.h"
                                                                                
//{878897F7-A654-F219-E514-5036E2387E08}
DEFINE_GUID(CLSID_ThreadPool, 0x878897F7, 0xA654, 0xF219, 0xE5, 0x14, 0x50, 0x36, 0xE2, 0x38, 0x7E, 0x08);                   
//{19A9CCEE-24E0-BBCA-5140-F5F587FFC220}
DEFINE_GUID(IID_IThreadPool, 0x19A9CCEE, 0x24E0, 0xBBCA, 0x51, 0x40, 0xF5, 0xF5, 0x87, 0xFF, 0xC2, 0x20);       
/**
 *@brief 线程池接口
 */
class IThreadPool :public IUnknown
{
public:	

	/**
	 *@brief  	最大分配的任务数
	 *@param	nMaxCount 最大任务数
	 *@return 	HRESULT __stdcall
	 */
	virtual HRESULT __stdcall GetCapacity(INT *pMaxCount) = 0;


	/**
	 *@brief  	取得一个任务
	 *@param	ppTask 任务对象接口
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料
	 *@note		注意事项
	 */
	virtual HRESULT __stdcall FetchTask(INLETask **ppTask) = 0;
	

	/**
	 *@brief  	归还任务
	 *@param	pTask 要归还的任务
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料
	 *@note		注意事项
	 */
	virtual HRESULT __stdcall ReturnTask(INLETask* pTask) = 0;
};                                                                      
