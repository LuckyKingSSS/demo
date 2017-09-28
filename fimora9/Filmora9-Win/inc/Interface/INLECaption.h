#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h> 
#include "NLECaptionDefine.h"
#include "NLECommon.h"


#ifndef _NLE_CAPTION_TYPE_
#define _NLE_CAPTION_TYPE_

typedef HRESULT(__stdcall *CAPTIONOBJ_BEFORECHANGE_CALLBACK)(void* pUserObj);

//{1DC8C767-8F40-4926-4A50-B1DE90323EF5}
DEFINE_GUID(CLSID_INLECaption, 
	0x1DC8C767, 0x8F40, 0x4926, 0x4A, 0x50, 0xB1, 0xDE, 0x90, 0x32, 0x3E, 0xF5);
//{712E9C55-5014-4d11-9067-E86AD1747CE0}
DEFINE_GUID(IID_INLECaption, 
	0x712e9c55, 0x5014, 0x4d11, 0x90, 0x67, 0xe8, 0x6a, 0xd1, 0x74, 0x7c, 0xe0);
/**
*@brief Caption对应的Timeline
*/
class INLECaption :public IUnknown
{
public:
	/**
	*@brief        创建文档各种类型的对象
	*@param        objType 对象类型 （文字，图片，图形，视频）
	*@param        X，Y 指定最初的创建位置
	*@param        DesWidth， 文档视图区域的宽度信息
	*@param        objData， 获取所创建的对象
	*@return       
	*/
	virtual void __stdcall CreateNewObj(NLECaptionObjType objType,  int DesWidth, int DesHeight, NLECaptionObjData* objData) = 0;
	/**
	*@brief        获取当前选取的对象，多选时优先返回文字，其次图形，视频优先级最低。
	*@param        objData 当前选取的对象
	*@return
	*/
	virtual void __stdcall GetSelObjPara(NLECaptionObjData* objData) = 0;
	/**
	*@brief        删除所选取的对象
	*@param         
	*@return
	*/
	virtual void __stdcall DeleteSelObjPara() = 0;
	/**
	*@brief        返回所有的对象个数
	*@param
	*@return int 对象个数
	*/
	virtual int  __stdcall GetParaObjCount() = 0;
	/**
	*@brief        根据索引获取对象
	*@param nIndex 索引
	*@param objData 当前索引的对象
	*@return int 对象个数
	*/
	virtual void __stdcall GetObjParaByIndex(int nIndex, NLECaptionObjData* objData) = 0;

	/**
	*@brief        鼠标按下事件
	*@param x, y 索引位置信息
	*@param ss Ctrl ，Shift等状态建信息
	*@param mb 鼠标按键信息
	*@return bool 
	*/
	virtual bool __stdcall MouseDownEvent(int x, int y, NLEShiftState ss, NLEMouseButton mb) = 0;
	/**
	*@brief        鼠标移动事件
	*@param x, y 索引位置信息
	*@param ss Ctrl ，Shift等状态建信息
	*@param mt 返回鼠标所在区域，用于应用层更新鼠标指针
	*@return bool
	*/
	virtual bool __stdcall MouseMoveEvent(int x, int y, NLEShiftState ss, NLEMouseType* mt) = 0;
	/**
	*@brief        鼠标弹起事件
	*@param x, y 索引位置信息
	*@param ss Ctrl ，Shift等状态建信息
	*@param mb  鼠标按键信息
	*@return bool
	*/
	virtual bool __stdcall MouseUpEvent(int x, int y, NLEShiftState ss, NLEMouseButton mb) = 0;


	virtual bool __stdcall MouseDoubleClickEvent(int x, int y, NLEShiftState ss, NLEMouseButton mb, NLEMouseType* mt) = 0;
	/**
	*@brief        键盘按键事件
	*@param key 键值
	*@param ss Ctrl ，Shift等状态建信息
	*@param strText  所输入的文本
	*@param bHandled  是否标记为已处理，如果未处理，应用层判断是否需要处理
	*@return bool
	*/
	virtual bool __stdcall KeyPressEvent(wchar_t key, NLEShiftState ss, BSTR strText, bool* bHandled) = 0;
	/**
	*@brief        设置文档编辑or非编辑状态
	*@param   bEdit 编辑or非编辑状态
	*@param   bClearChangeState 是否重新标记文档状态被更新标识
	*@return 
	*/
	virtual void	__stdcall SetDocState(bool bEdit, bool bClearChangeState) = 0;
	/**
	*@brief       获取文档状态
	*@param   bEdit 编辑or非编辑状态 
	*@return 
	*/
	virtual void	__stdcall GetDocState(bool* bEdit, bool* bTextEdit) = 0;
	/**
	*@brief      设置对象对齐模式
	*@param   alignType  上，下，左，右，居中
	*@return
	*/
	virtual void	__stdcall SetObjAlignType(NLEObjAlignType alignType) = 0;
	/**
	*@brief      设置撤销恢复记录ID，用来将一系列操作归纳到一步操作
	*@param    
	*@return
	*/
	virtual void	__stdcall PrepareUndoGroupID() = 0;
	/**
	*@brief      撤销
	*@param
	*@return
	*/
	virtual void	__stdcall UnDo() = 0;
	/**
	*@brief      恢复
	*@param
	*@return
	*/
	virtual void	__stdcall ReDo() = 0;
	/**
	*@brief     是否能够通过Delete键删除对象，一般简易模式禁用此功能
	*@param
	*@return
	*/
	virtual void	__stdcall CanDeleteObj(bool bCanDelete) = 0;
	/**
	*@brief     是否能启用记录撤销恢复功能，一般简易模式禁用此功能
	*@param
	*@return
	*/
	virtual void	__stdcall CanUnDoReDo(bool bCanUnDoReDo) = 0;
	/**
	*@brief     获取文档是否被修改标识，用于高级模式退出提示，以及撤销恢复的后续操作
	*@param
	*@return
	*/
	virtual void	__stdcall GetDocHasChanged(bool* bChanged) = 0;
	/**
	*@brief     获取撤销恢复状态，Mac下需要通过此控制菜单可用状态
	*@param
	*@return
	*/
	virtual void	__stdcall GetUndoRedoState(bool* bCanUndo, bool *bCanRedo) = 0;
	/**
	*@brief     设置输出大小
	*@param  目标宽高
	*@return
	*/
	virtual void	__stdcall SetDesSize(int nDesWidth, int nHeight) = 0;

	/**
	*@brief     获取一帧渲染图片
	*@param
	*@return
	*/
	virtual NLEComPtr<INLEFrame> __stdcall GetFrame(INLEFrame* pBKFrame) = 0;


	virtual bool __stdcall LoadFromFile(BSTR strFilePath) = 0;
	virtual bool __stdcall SaveToFile(BSTR strFilePath) = 0;
	virtual HRESULT __stdcall SetObjBeforeChangeCallBack(CAPTIONOBJ_BEFORECHANGE_CALLBACK pCallBack, void *pUserObj) = 0;
	virtual void __stdcall SetDrawFrameType(bool bClientDraw) = 0;
};              

#endif