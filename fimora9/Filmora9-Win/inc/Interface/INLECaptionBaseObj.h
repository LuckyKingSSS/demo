#pragma once 
#include <Guiddef.h>
#include "NLECaptionDefine.h" 
#include "INLEBitmap.h"

// {0BFB9B00-3376-4f0e-AED0-6E63077F13EB}
DEFINE_GUID(CLSID_INLECaptionBaseObj,
	0xbfb9b00, 0x3376, 0x4f0e, 0xae, 0xd0, 0x6e, 0x63, 0x7, 0x7f, 0x13, 0xeb);

// {6BBF5726-EAE4-47b1-B157-1CDC0EC41AF9}
DEFINE_GUID(IID_INLECaptionBaseObj, 0x6bbf5726, 0xeae4, 0x47b1, 0xb1, 0x57, 0x1c, 0xdc, 0xe, 0xc4, 0x1a, 0xf9);

class INLECaptionBaseObj :public IUnknown
{
public:
	/**
	*@brief     获取对象ID
	*@param
	*@return 返回对象ID
	*/
	virtual int	__stdcall GetObjID() = 0;

	/**
	*@brief     获取所属对象，主要是用在TipText上
	*@param
	*@return 返回对象ID
	*/
	virtual void*	__stdcall GetOwnerObj() = 0;
	/**
	*@brief     设置所属对象，主要是用在TipText上
	*@param
	*@return 返回对象ID
	*/
	virtual void	__stdcall SetOwnerObj(void* pOwnerObj) = 0;
	/**
	*@brief     设置对象时间信息
	*@param  startTime起始时间点，  duration  持续时长
	*@return
	*/
	virtual void __stdcall SetTimeInfo(float startTime, float duration, bool bEnableUnDoReDo) = 0;
	/**
	*@brief     返回对象时间信息
	*@param  startTime起始时间点，  duration  持续时长
	*@return
	*/
	virtual void __stdcall GetTimeInfo(float* startTime, float* duration) = 0;
	/**
	*@brief     设置动画入场比例
	*@param  nEffectIn  入场比例
	*@return
	*/
	virtual void __stdcall SetFrameRate(NLERational rat) = 0;

	virtual void __stdcall SetEffectInRatio(float nEffectIn) = 0;
	/**
	*@brief     获取动画入场比例
	*@param  nEffectIn  入场比例
	*@return
	*/
	virtual void __stdcall GetEffectInRatio(float* nEffectIn) = 0;
	/**
	*@brief     设置动画出场比例
	*@param  nEffectOut  出场比例
	*@return
	*/
	virtual void __stdcall SetEffectOutRatio(float nEffectOut) = 0;
	/**
	*@brief     获取动画出场比例
	*@param  nEffectOut  出场比例
	*@return
	*/
	virtual void __stdcall GetEffectOutRatio(float* nEffectOut) = 0;
	/**
	*@brief     设置对象Z轴顺序
	*@param  nZOrder  Z轴顺序，bActive是否被选中，用于撤销恢复时选中对象
	*@return
	*/
	virtual void __stdcall SetZOrder(int nZOrder, bool bActive) = 0;
	/**
	*@brief     获取对象Z轴顺序
	*@param  nZOrder  Z轴顺序，
	*@return
	*/
	virtual void __stdcall GetZOrder(int* nZOrder) = 0;
	/**
	*@brief     获取对象坐标位置
	*@param  xPos，yPos 坐标信息
	*@return
     */
	virtual void __stdcall GetPos(float* xPos, float* yPos) = 0;
	/**
	*@brief     设置对象坐标位置
	*@param  xPos，yPos 坐标信息
	*@return
	*/
	virtual void __stdcall SetPos(float xPos, float yPos) = 0;

	/**
	*@brief     设置对象坐标位置,提供给应用层调用
	*@param  xPos，yPos 坐标信息
	*@return
	*/
	virtual void __stdcall SetObjPosition(float xPos, float yPos)=0;
	/**
	*@brief     获取对象角度
	*@param nAngle 角度值
	*@return
	*/
	virtual void __stdcall GetAngle(float* fAngle) = 0;
	/**
	*@brief     设置对象角度
	*@param nAngle 角度值
	*@return
	*/
	virtual void __stdcall SetAngle(float fAngle) = 0;
	/**
	*@brief     获取对象中心点坐标
	*@param xPos，yPos中心点坐标
	*@return
	*/
	virtual void __stdcall GetCenterPos(float* xPos, float* yPos) = 0;
	/**
	*@brief     设置对象中心点坐标
	*@param xPos，yPos中心点坐标
	*@return
	*/
	virtual void __stdcall SetCenterPos(float xPos, float yPos) = 0;

	/**
	*@brief     获取对象缩放值
	*@param hScale，vScale 水平和垂直值
	*@return
	*/
	virtual void __stdcall GetScale(float* hScale, float* vScale) = 0;
	/**
	*@brief     设置对象缩放值
	*@param hScale，vScale 水平和垂直值
	*@return
	*/
	virtual void __stdcall SetScale(float hScale, float vScale) = 0;

	/**
	*@brief     设置所属文档对象
	*@param   pOwner CaptionTimeline文档对象指针
	*@return
	*/

	virtual void __stdcall SetOwnerDoc(void* pOwner) = 0;
	/**
	*@brief     获取对象信息数据，返回给应用层获取对象类型，指针等信息
	*@param   objData 对象打包数据
	*@return
	*/
	virtual void __stdcall GetObjOwnerData(NLECaptionObjData* objData) = 0;
	/**
	*@brief     t调整对象布局，当文档大小变化时，需要重新调整对象的坐标信息
	*@param   nStreamWidth，nStreamHeight 新文档渲染大小
	*@return
	*/
	virtual void __stdcall AdjustPos(int nStreamWidth, int nStreamHeight) = 0;
	/**
	*@brief        鼠标按下事件
	*@param x, y 索引位置信息
	*@param ss Ctrl ，Shift等状态建信息
	*@param mb 鼠标按键信息
	*@return bool
	*/
	virtual bool  __stdcall OnMouseDown(int x, int y, NLEShiftState ss, NLEMouseButton mb) = 0;
	/**
	*@brief        鼠标移动事件
	*@param x, y 索引位置信息
	*@param ss Ctrl ，Shift等状态建信息
	*@param mt 返回鼠标所在区域，用于应用层更新鼠标指针
	*@return bool
	*/
	virtual bool  __stdcall OnMouseMove(int x, int y, NLEShiftState ss, NLEMouseType* mt, bool bDragSize) = 0;
	/**
	*@brief        鼠标弹起事件
	*@param x, y 索引位置信息
	*@param ss Ctrl ，Shift等状态建信息
	*@param mb  鼠标按键信息
	*@return bool
	*/
	virtual bool  __stdcall OnMouseUp(int x, int y, NLEShiftState ss, NLEMouseButton mb) = 0;

	virtual bool __stdcall OnMouseDoubleClick(int x, int y, NLEShiftState ss, NLEMouseButton mb) = 0;
	/**
	*@brief        键盘按键事件
	*@param key 键值
	*@param ss Ctrl ，Shift等状态建信息
	*@param strText  所输入的文本
	*@param bHandled  是否标记为已处理，如果未处理，应用层判断是否需要处理
	*@return bool
	*/
	virtual bool  __stdcall OnKeyPress(wchar_t key, BSTR strText, NLEShiftState ss) = 0;
	/**
	*@brief        获取鼠标所在对象的区域
	*@param x, y 索引位置信息
	*@param mt 返回鼠标所在区域，用于应用层更新鼠标指针
	*@return bool
	*/
	virtual bool __stdcall GetMouseInAreaType(int x, int y, NLEMouseType* mt) = 0;
	/**
	*@brief     绘制对象
	*@param pFrame 背景图
	*@param bPreview 是否是动画预览模式
	*@param nCurFrame 当前帧
	*@param nTotalFrame 总帧数
	*@param fFrameRate 帧率
	*@param bPaintEditFrame false 渲染对象  true 渲染对象编辑框
	*@return
	*/
	virtual void __stdcall Paint(INLEBitmap* pFrame, bool bPreview, DWORD nCurFrame, DWORD nTotalFrame,
		double fFrameRate, bool bPaintEditFrame) = 0;
	/**
	*@brief       清除Edit， Focused状态
	*@return
	*/
	virtual void __stdcall GetObjPointsInfo(NLECaptionObjPointsInfo* pPointsInfo, int FrameWidth, int FrameHeight) = 0;
	/**
	*@brief       重置Focuse，Edit状态
	*@return
	*/
	virtual void __stdcall ClearState() = 0;
	/**
	*@brief       设置Focused状态
	*@return
	*/
	virtual void __stdcall SetFocused(bool bFocused) = 0;
	/**
	*@brief       获取Focused状态
	*@return
	*/
	virtual void __stdcall GetFocused(bool* bFocused) = 0;
	/**
	*@brief       获取Edit状态
	*@return
	*/
	virtual void __stdcall GetEdited(bool* bEdited) = 0;
	/**
	*@brief       Clone对象，返回封装后的数据结构
	*@return
	*/
	virtual NLECaptionObjData* __stdcall  CloneObj() = 0;
	/**
	*@brief       执行对象Clone具体操作
	*@return
	*/
	virtual void* __stdcall  Clone(void* pOwner) = 0;
	/**
	*@brief       释放对象无用数据，用于当文档不在渲染时间段时，释放内存
	*@return
	*/
	virtual void __stdcall FreeUnUseMemory() = 0;
	/**
	*@brief       准备渲染，在此处初始化对象相关资源
	*@param nCurFrame 当前帧
	*@param RalFrameRate 帧率
	*@return
	*/
	virtual void __stdcall PrepareRender(int nCurFrame, NLERational RalFrameRate) = 0;
	/**
	*@brief       准备动画相关数据
	*@return
	*/
	virtual void __stdcall PrepareAnimation() = 0;
	/**
	*@brief      设置拖动参数，当多选时需要对每个选取的对象设置拖动参数
	*@param x,y 当前鼠标坐标
	*@param cx，cy 旋转中心点
	*@param nAngle 当前角度
	*@return
	*/
	virtual void __stdcall SetDragParam(int x, int y, int cx, int cy, float nAngle, NLEMouseType dt) = 0;
	/**
	*@brief      获取拖动时的角度值，用于界面更新拖动旋转时，实时更新界面
	*@param nDragAngle 角度值
	*@return
	*/
	virtual void __stdcall GetDragAngle(float* nDragAngle) = 0;
	/**
	*@brief      设置分组信息
	*@param nGroup 分组编号
	*@return
	*/
	virtual void __stdcall SetGroupID(int nGroup) = 0;
	/**
	*@brief      设置Motion编号
	*@param strMotionID 编号
	*@return
	*/
	virtual void __stdcall SetMotionID(BSTR	strMotionID) = 0;
	/**
	*@brief      设置Transition编号
	*@param strTransitionID 编号
	*@return
	*/
	virtual void __stdcall SetTransitionID(BSTR strTransitionID) = 0;
	/**
	*@brief
	*@param
	*@param
	*@return 将转场，Motion，文字动画，都清掉
	*/
	virtual void __stdcall RemoveAllMotion() = 0;
	/**
	*@brief
	*@param
	*@param
	*@return 返回是否有设置进场和出场的动画
	*/
	virtual bool __stdcall HasInOutAnimation() = 0;
	/**
	*@brief      序列化
	*@param doc tinyxml文档
	*@param node 所属节点
	*@return
	*/

	virtual void __stdcall Serialize(void* doc, void* node) = 0;
	/**
	*@brief      反序列化
	*@param node 所属节点
	*@return
	*/
	virtual void __stdcall LoadFromNode(void* node, std::wstring strResFilePath) = 0;
	/**
	*@brief      获取文字内容
	*@param str 文本信息
	*@return
	*/
	virtual BOOL __stdcall GetText(BSTR* str) = 0;
	/**
	*@brief    重置底层撤销恢复的计步信息
	*@param str 文本信息
	*@return
	*/
	virtual void __stdcall SetUnDoReDoStep() = 0;
};

