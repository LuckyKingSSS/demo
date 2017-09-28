/////////////////////////////////////////////
// ImageRender.h
// 提供设备无关位图数据接口


#pragma once

#include "Image.h"

#ifndef IMAGE_FILTER_DEFINE
#define IMAGE_FILTER_DEFINE

// 图像滤镜ID
enum IMAGE_FILTER_ID
{
	IFI_NONE,		
	IFI_ZOOM_BLUR,				// Zoom模糊 nRadius - 模糊深度
	IFI_MOTION_BLUR,			// 运动模糊 nRadius - 模糊半径  nParam1 - 模糊方向（0 - 向左， 1 - 向右）
	IFI_GAUSS_BLUR,				// 高斯模糊 nRadius - 模糊半径 
	IFI_SHADOW,					// 添加阴影 nRadius - 不透明度  nParam1 - 阴影颜色（COLORREF） nParam2 - 平滑程度 nParam3 - X方向偏移 nParam4 - Y方向偏移
//	IFI_SOLIDFRAME,				// 边框

	IFI_INVERT			= 300,	// 图像反色
	IFI_GRAY_SCALE,				// 灰度图像
	IFI_THRESHOLD,				// 阀值 nRadius (0 - 255)
	IFI_FLIP,					// 垂直翻转
	IFI_FLIP_HORZ,				// 水平翻转
	IFI_EMBOSS,					// 浮雕 nRadius (0 - 100)
	IFI_SPLASH,					// 斑点 nRadius (0 - 255)
	IFI_MOSAIC,					// 马赛克 nRadius (0 - 255)
	IFI_OIL_PAINT,				// 油画 nRadius (0 - 10)
	IFI_3DGRID,					// 3D网格 nParam1 - 网格尺寸（1 - ） nParam2 - 网格深度（1 - ） 
	IFI_WHIRL_PINCH,			// 旋转挤压 nParam1 - 旋转（0 - 360） nParam2 - 挤压（0 - 100） 
	IFI_GRADIENT_RADIAL,		// 放射型渐变 暂不使用
	IFI_GAMMA,					// gamma调节 nRadius (0 - 255)
	IFI_ROTATE90,				// 顺时针旋转90度/rotate 90'
	IFI_RIBBON,					// 带形 nParam1 : 振幅[0..100]，此值为一百分比  nParam2 : 频率>=0，每10为一个pi
	IFI_HALFTONE_M3,			// 半影调 无参数
	IFI_BRIGHTNESS,				// 调节亮度 使用nParam1参数（0 - 200） 
	IFI_CONTRAST,				// 调节对比度 使用nParam1参数（0 - 200） 
	IFI_COLORTONE,				// 单色调 nParam1 - R分量 nParam2 - G分量 nParam3 - B分量
	IFI_HUE_SATURATION,			// 色调饱和度/hue saturation  nParam1 - 色调(-180, 180) nParam2 - 饱和度(-100, 100) 
	IFI_CREATEHALO,				// 制作光晕
	IFI_OLD_PHOTO,				// 老照片 nRadius (0 - 10) 暂时未使用任何参数
	IFI_SOLIDFRAME,				// 边框

	IFI_STRETCH = 500,			// nQuality 取值见IMAGE_INTERPOLATION定义 nParam1 - 缩放后图像的宽度 nParam2 - 缩放后图像的高度
	IFI_ROTATE,					// 旋转 nAngle - (0 - 360)

	IFI_COUNT
};

// Image Filter Parameter structure
struct IMAGE_FILTER_PARAM
{
	IMAGE_FILTER_ID	uFilterID;		// 滤镜ID
	int				nRadius;		// 半径
	int				nAngle;			// 角度
	int				nQuality;		// 品质
	int				nParam1;
	int				nParam2;
	int				nParam3;
	int				nParam4;
};

typedef IMAGE_FILTER_PARAM* LPIMAGE_FILTER_PARAM;

 

struct FILTERPARAM 
{
	IMAGE_FILTER_ID uFilterID;
    wchar_t szIDCaption[64];  //显示的标题
    int bRadius ;  wchar_t RadiusText[32]; int RadiusMax, RadiusMin;
    int bAngle ;   wchar_t AngleText[32];  int AngleMax,  AngleMin;
    int bQuality;  wchar_t QualityText[32];int QualityMax, QualityMin;
    int bParam1;   wchar_t Param1Text[32]; int Param1Max, Param1Min;
    int bParam2;   wchar_t Param2Text[32]; int Param2Max, Param2Min;
    int bParam3;   wchar_t Param3Text[32]; int Param3Max, Param3Min;
    int bParam4;   wchar_t Param4Text[32]; int Param4Max, Param4Min;
};

typedef FILTERPARAM* LPFILTERPARAM;

#endif

// 图像滤镜回掉函数定义
typedef int (__stdcall * IMAGE_FILTER_CALLBACK)(void* pUserObj, UINT uMesg, WPARAM wParam, LPARAM lParam);

// 图像滤镜处理
int __stdcall IRImageFitler(HDIBIMAGE hImageDst, HDIBIMAGE hImageSrc, LPIMAGE_FILTER_PARAM pParam, IMAGE_FILTER_CALLBACK fnCallback = 0, void* pUserObj = 0);

int __stdcall IRGetFilterParam(IMAGE_FILTER_ID uFilterID, LPFILTERPARAM pParam); // 取得Filter参数

int  __stdcall IRGetFilterCount();   //取得Ｆｉｌｔｅｒ的个数；

int __stdcall IRGetFilterItem(int Index , LPFILTERPARAM pParam);  //取得第　Ｉｎｄｅｘ项

