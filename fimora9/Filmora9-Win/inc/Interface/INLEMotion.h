#pragma once

#include <vector>
using namespace std;

#include <Guiddef.h>
#include <objbase.h>

#include "NLEType_c.h"
#include "NLEGraphics.h"
#include "INLEBitmap.h"

// {113963BC-E24B-4306-871D-5D0AACA91F68}
DEFINE_GUID(CLSID_NLEMotion, 0x113963bc, 0xe24b, 0x4306, 0x87, 0x1d, 0x5d, 0xa, 0xac, 0xa9, 0x1f, 0x68);

// {C4FDE7A1-97B0-42FF-B494-FC274E2390F8}
DEFINE_GUID(IID_INLEMotionAux, 0xc4fde7a1, 0x97b0, 0x42ff, 0xb4, 0x94, 0xfc, 0x27, 0x4e, 0x23, 0x90, 0xf8);

// {1C7E71CE-2A17-4DDD-BFC9-9982540EC637}
DEFINE_GUID(IID_INLEMotionActor, 0x1c7e71ce, 0x2a17, 0x4ddd, 0xbf, 0xc9, 0x99, 0x82, 0x54, 0xe, 0xc6, 0x37);

// {76E0FBC3-DB7D-4F23-B715-44E6609D84CF}
DEFINE_GUID(IID_INLECurver, 0x76e0fbc3, 0xdb7d, 0x4f23, 0xb7, 0x15, 0x44, 0xe6, 0x60, 0x9d, 0x84, 0xcf);

// {6F58D348-9F2A-4573-BB7D-10F0C6D492DE}
DEFINE_GUID(IID_INLETwneener, 0x6f58d348, 0x9f2a, 0x4573, 0xbb, 0x7d, 0x10, 0xf0, 0xc6, 0xd4, 0x92, 0xde);

// {D356B8A6-F42B-4BD9-8B7F-0222BCB87059}
DEFINE_GUID(IID_INLEDrawer, 0xd356b8a6, 0xf42b, 0x4bd9, 0x8b, 0x7f, 0x2, 0x22, 0xbc, 0xb8, 0x70, 0x59);

typedef struct _NLEImage
{
	void* data;

	NLEFrameFormat format; // 必须是 NLE_FRAME_FORMAT_BGRA
	int width;
	int height;
} NLEImage;

typedef struct _NLEMotionMorphology
{
	float frameIdxRel; // 帧序，指明（时间位置）。第一个关键帧，帧序必须为0。外部使用的时候，帧序取值为相对值[0, 1.0)；内部实现，则使用绝对值
	int pointIdx; // 点序，指明（空间位置）。第一个关键帧，点序必须为0

	float alpha; // 透明度。取值范围[0.0, 1.0]

	int opt; // 0 - 旋转的时候保持尺寸不变；1 - 放大尺寸，保证旋转的时候仍全部可见
	float angle; // 旋转的角度。负数顺时针方向，正数反时针方向
	float x_scale, y_scale; // 缩放因子。取值范围[0.0, 15.0]

} NLEMotionMorphology;

typedef int NLECurveType;

// 内置的曲线类型
#define NLE_CURVE_BEZIER	0
#define NLE_CURVE_BSPLINE	1
#define NLE_CURVE_NUM		2

/**
*@ingroup GroupCurver
*@brief INLECurver接口
*/
class INLECurver :
	public IUnknown
{
public:
	virtual HRESULT __stdcall GenerateCurve(NLECurveType type, vector<tagNLEPointF>* ctrlPoints, vector<tagNLEPointF>* curvePoints, int pointCnt) = 0;
};

/**
*@ingroup GroupTwneener
*@brief INLETwneener接口
*/
class INLETwneener :
	public IUnknown
{
public:
	virtual HRESULT __stdcall LoadFrom(int frameCount, vector<tagNLEPointF>* curvePoints, vector<NLEMotionMorphology>* keyMorphs) = 0;
	virtual HRESULT __stdcall GetByFrameIdx(int frameIdx, tagNLEPointF* point, NLEMotionMorphology* morph) = 0;
	virtual void __stdcall Clear() = 0;
};

/**
*@ingroup GroupDrawer
*@brief INLEDrawer接口
*/
class INLEDrawer :
	public IUnknown
{
public:
	virtual HRESULT __stdcall Draw(NLEImage* dst, NLEImage* src, tagNLEPointF* point, NLEMotionMorphology* morph) = 0;
	virtual HRESULT __stdcall Draw(INLEBitmap* dst, INLEBitmap* src, tagNLEPointF* point, NLEMotionMorphology* morph) = 0;
#ifdef _DEBUG
	virtual void __stdcall DrawCurve(NLEImage* image, vector<tagNLEPointF>* curvePoints, vector<tagNLEPointF>* ctrlPoints, vector<NLEMotionMorphology>* keyMorphs) = 0;
#endif // _DEBUG
};

enum NLEMotionLoopMode
{
	NLE_MOTIONLOOP_NONE,
	NLE_MOTIONLOOP_CIRCLE,
	NLE_MOTIONLOOP_REVERSE,
};

#define MAX_MOTION_NAME_LEN	256

#define MAX_CTRL_POINTS	32
#define MIN_CTRL_POINTS	4

#define MAX_KEY_MORPHS	32
#define MIN_KEY_MORPHS	1

#define MIN_CURVE_POINTS	2
#define MIN_FRAME_COUNT		1

#define MAX_SCALE		15.0

class INLEMotionAux : public IUnknown
{
public:
	/**
	*@brief  	设置Twneener接口
	*@param	pTwneener Twneener接口
	*@return 	标准返回值
	*/
	virtual HRESULT __stdcall SetTwneener(INLETwneener* pTwneener) = 0;
	/**
	*@brief  	设置贴图接口
	*@param	pDrawer 贴图接口
	*@return 	标准返回值
	*/
	virtual HRESULT __stdcall SetDrawer(INLEDrawer* pDrawer) = 0;

	/**
	*@brief  	注册曲线接口。内置的曲线类型，禁止注册；自定义曲线类型，后注册的会覆盖先注册的。
	*@param	pCurver 曲线接口
	*@return 	标准返回值
	*/
	virtual HRESULT __stdcall RegCurver(NLECurveType type, INLECurver* pCurver) = 0;
	/**
	*@brief  	反注册曲线接口
	*@param	pCurver 曲线接口
	*@return 	标准返回值
	*/
	virtual HRESULT __stdcall UnregCurver(NLECurveType type) = 0;
	/**
	*@brief  	重置曲线接口。反注册所有已注册曲线接口，仅保留内置的
	*@param	pCurver 曲线接口
	*/
	virtual void __stdcall ResetCurver() = 0;
};

class INLEMotionActor : public IUnknown
{
public:
	/**
	*@brief  	判断是否已经加载动画数据
	*@param	loaded 接受是否加载的布尔值。TRUE - 已加载
	*@return 	标准返回值
	*/
	virtual HRESULT __stdcall IsLoaded(BOOL* loaded) = 0;
	/**
	*@brief  	从文件加载动画
	*@param	filePath 文件全路径
	*@param	animName 动画名称
	*@return 	标准返回值
	*/
	virtual HRESULT __stdcall LoadFromFile(WCHAR* filePath, WCHAR* animName) = 0;
	/**
	*@brief  	从内存加载动画
	*@param	xmlBuffer xml缓冲
	*@param	len 缓冲长度
	*@param	animName 动画名称
	*@return 	标准返回值
	*/
	virtual HRESULT __stdcall LoadFromMem(char* xmlBuffer, size_t len, WCHAR* animName) = 0;
	/**
	*@brief  	按参数加载动画。适合于产生模板。
	*@param	frameCount 动画帧数。取值范围[MIN_FRAME_COUNT, ∞)
	*@param	pointCount 曲线的点数。取值范围[MIN_CURVE_POINTS, ∞)
	*@param	type 曲线类型
	*@param	ctrlPoints 控制点数组。控制点坐标取相对值[0, 1.0)。数组大小取值范围[MIN_CTRL_POINTS, MAX_CTRL_POINTS]
	*@param	size 动画画面的尺寸
	*@param	offset 动画的偏移量
	*@param	scale 动画画面的缩放比例
	*@param	lastPos 动画的最终位置{实际上与offset类似，都是计算动画的偏移的。只是文字使用方便才增加的。}。取相对值[0, 1.0)
	*@param	loop 循环模式
	*@param	keyMorphs 关键状态数组
	*@return 	标准返回值
	*/
	virtual HRESULT __stdcall LoadFrom(int frameCount, int pointCount,
		NLECurveType type, vector<tagNLEPointF>* ctrlPoints, tagNLESizeF* size, tagNLEPointF* offset, tagNLEPointF* scale, tagNLEPointF* lastPos, NLEMotionLoopMode loop,
		vector<NLEMotionMorphology>* keyMorphs) = 0;

	/**
	*@brief  	以现有的参数重新加载一次。适合于产生实例。
	*@return 	标准返回值
	*@see 		LoadFromFile
	*/
	virtual HRESULT __stdcall Reload() = 0;

	/**
	*@brief  	保存动画到文件。适合于产生模板。
	*@param	filePath 文件全路径
	*@param	animName 动画名称
	*@return 	标准返回值
	*@see 		LoadFromFile
	*/
	virtual HRESULT __stdcall SaveToFile(WCHAR *filePath = NULL, WCHAR* animName = NULL) = 0;

	/**
	*@brief  	保存动画到缓冲。适合于保存实例。
	*@param	xmlBuffer xml缓冲
	*@param	len 缓冲长度。输入表示缓冲区大小，输出表示实际占用的大小
	*@param	animName 动画名称
	*@return 	标准返回值
	*@see 		LoadFromFile
	*/
	virtual HRESULT __stdcall SaveToMem(char* xmlBuffer, size_t* len, WCHAR* animName = NULL) = 0;

	/**
	*@brief  	加载动画
	*@param	animName 动画名称
	*@param	reload 是否重新加载模板文件。默认false，不重新加载
	*@return 	标准返回值
	*/
	virtual HRESULT __stdcall LoadByID(WCHAR* animName, bool reload = false) = 0;
	/**
	*@brief  	保存动画{适合于产生自定义模板}
	*@param	animName 动画名称
	*@return 	标准返回值
	*@see 		LoadFromFile
	*/
	virtual HRESULT __stdcall SaveByID(WCHAR* animName) = 0;

	/**
	*@brief  	设置/获取动画循环模式
	*@param	mode 循环模式
	*@return 	标准返回值
	*/
	virtual HRESULT __stdcall SetLoopMode(NLEMotionLoopMode mode) = 0;
	virtual HRESULT __stdcall GetLoopMode(NLEMotionLoopMode* mode) = 0;

	/**
	*@brief  	设置/获取动画路径的尺寸
	*@param	size 动画路径的尺寸。取值范围(0, ∞)；特别取值1.0，表示相对值
	*@return 	标准返回值
	*/
	virtual HRESULT __stdcall SetSize(tagNLESizeF* size) = 0;
	virtual HRESULT __stdcall GetSize(tagNLESizeF* size) = 0;

	/**
	*@brief  	设置/获取动画画面的缩放比例
	*@param	scale 动画画面的缩放比例。取值范围[0, ∞)
	*@return 	标准返回值
	*/
	virtual HRESULT __stdcall SetScale(tagNLEPointF* scale) = 0;
	virtual HRESULT __stdcall GetScale(tagNLEPointF* scale) = 0;

	/**
	*@brief  	设置/获取动画的偏移。默认偏移为(0, 0)
	*@param	offset 动画的偏移
	*@return 	标准返回值
	*/
	virtual HRESULT __stdcall SetOffset(tagNLEPointF* offset) = 0;
	virtual HRESULT __stdcall GetOffset(tagNLEPointF* offset) = 0;

	/**
	*@brief  	设置/获取动画的最终位置{实际上与offset类似，都是计算动画的偏移的。只是文字使用方便才增加的。}
	*@param	lastPos 动画的最终位置。取相对值[0, 1.0)；设置时，取值NULL - 表示重置动画的最终位置
	*@return 	标准返回值
	*/
	virtual HRESULT __stdcall SetLastPos(tagNLEPointF* lastPos) = 0;
	virtual HRESULT __stdcall GetLastPos(tagNLEPointF* lastPos) = 0;

	/**
	*@brief  	设置/获取曲线的类型
	*@param	type 曲线类型
	*@return 	标准返回值
	*/
	virtual HRESULT __stdcall SetCurveType(NLECurveType type) = 0;
	virtual HRESULT __stdcall GetCurveType(NLECurveType* type) = 0;

	/**
	*@brief  	设置/获取曲线的控制点
	*@param	ctrlPoints 控制点数组。控制点坐标取相对值[0, 1.0)。数组大小取值范围[MIN_CTRL_POINTS, MAX_CTRL_POINTS]
	*@return 	标准返回值
	*/
	virtual HRESULT __stdcall SetCtrlPoints(vector<tagNLEPointF>* ctrlPoints) = 0;
	virtual HRESULT __stdcall GetCtrlPoints(vector<tagNLEPointF>* ctrlPoints) = 0;

	/**
	*@brief  	设置/获取动画的关键状态
	*@param	keyMorphs 关键状态数组。数组大小取值范围[MIN_KEY_MORPHS, MAX_KEY_MORPHS]
	*@return 	标准返回值
	*@see 		LoadFrom
	*/
	virtual HRESULT __stdcall SetKeyMorphs(vector<NLEMotionMorphology>* keyMorphs) = 0;
	virtual HRESULT __stdcall GetKeyMorphs(vector<NLEMotionMorphology>* keyMorphs) = 0;

	/**
	*@brief  	设置/获取动画的帧数
	*@param	frameCount 动画帧数。取值范围[MIN_FRAME_COUNT, ∞)
	*@return 	标准返回值
	*/
	virtual HRESULT __stdcall SetFrameCount(int frameCount) = 0;
	virtual HRESULT __stdcall GetFrameCount(int* frameCount) = 0;

	/**
	*@brief  	设置/获取曲线的点数组
	*@param	curvePoints 保存曲线点的缓冲。绝对值坐标。数组大小取值范围[MIN_CURVE_POINTS, ∞)
	*@return 	标准返回值
	*@see 		LoadFrom
	*/
	virtual HRESULT __stdcall GetCurvePoints(vector<tagNLEPointF>* curvePoints) = 0;

	/**
	*@brief  	获取frameIdx指定时间的状态
	*@param	frameIdx 第几帧，取值范围[0, frameCount)
	*@param	point 所在的中心点坐标
	*@return 	标准返回值
	*@see 		LoadFrom
	*/
	virtual HRESULT __stdcall GetPointByFrameIdx(int frameIdx, tagNLEPointF* point) = 0;

	/**
	*@brief  	获取frameIdx指定时间的状态
	*@param	frameIdx 第几帧，取值范围[0, frameCount)
	*@param	morph 状态。其中的帧序和点序，是离它最近的前一个关键状态的帧序和点序
	*@return 	标准返回值
	*@see 		LoadFrom
	*/
	virtual HRESULT __stdcall GetMorphByFrameIdx(int frameIdx, NLEMotionMorphology* morph) = 0;

	/**
	*@brief  	将src按照frameIdx指定时间的状态贴到dst上去
	*@param	frameIdx 帧序号，取值范围[0, frameCount)
	*@param	dst 目标图像
	*@param	src 源图像
	*@return 	标准返回值
	*@see 		LoadFrom
	*/
	virtual HRESULT __stdcall DoMotion(int frameIdx, INLEBitmap* dst, INLEBitmap* src) = 0;

	/**
	*@brief  	清理加载的数据
	*/
	virtual void __stdcall Clear() = 0;
};