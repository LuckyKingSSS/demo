// VideoSlideParam.h

/*
	   视频合成的处理参数
*/

#pragma once

#include "DecodeParam.h"
#include "CodecDefine.h"


#ifndef WATERMARK_PARAM
#define WATERMARK_PARAM

struct WatermarkParam
{
	HMODULE			hModule;
	wchar_t			szID[512];				// 水印资源ID
	RECT			Rect;					// 水印位置
	int				nOpacity;				// 水印不透明度 (0 - 100)  0 - 完全透明  100 - 完全不透明
	LPBITMAPINFO	lpbi;					// lpbi 非0则不使用hModule，支持32位透明图像，以及Y41A自定义格式
	LPBYTE			lpBits;					// lpbi和lpBits需要应用程序负责其生命期
	int             nAlign;                 // 水印对齐(nAlign > 0时 设置水印对齐(nAlign), 否则设置水印位置(Rect))
};

#endif

#ifndef FLOAT_RECT_DEF
#define FLOAT_RECT_DEF
typedef struct tagFRECT
{
	float   left;
	float   top;
	float   right;
	float   bottom;
} FRECT, *PFRECT;
#endif

#ifndef USERWATERMARK_PARAM
#define USERWATERMARK_PARAM
struct UserWatermarkParam
{
	LPBITMAPINFO lpbi;
	LPBYTE       lpBits;
	FRECT        fRect;
	int          nOpacity;
};
#endif

struct VideoSlideParam
{
	int						nWidth;			// 分辨率宽度
	int						nHeight;		// 分辨率高度
	double					dFrameRate;		// 帧率 0表示自动选择
	DWORD					dwCompression;	// 图像格式，目前只支持'21VY'
	int						nBitsDepth;		// 图像位深度
	WatermarkParam			Watermark;		// 水印参数
};

struct VideoProcessParam
{
	CLIP_TIME		Time;				// 时间设置
	RECT			CropRect;			// 剪裁区域，为浮点数相对坐标
	int				nBrightness;		// 亮度，-100～100，0保持原亮度
	int				nContrast;			// 对比度，-100～100，0保持原对比度
	int				nSaturation;		// 饱和度,-100～100，0保持原饱和度
	UINT			uExFilterID;		// 扩展滤镜ID
	UINT			uFlipFlag;			// 图像翻转设置
	DEC_MODEL		DecMod;				// 解码方式
	DecParam		decParam;			// 解码参数
	DEINTERLACE_STYLE uDeintelace;		// 去除隔行扫描方式
	IMAGE_RESIZE_METHOD		nResizeStyle;	// 图像缩放方式
	IMAGE_INTERPOLATION		nInterpolation;	// 图像插值算法
};

