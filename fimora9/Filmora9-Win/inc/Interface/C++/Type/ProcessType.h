/**
@file   ProcessType.h 
@brief  效果处理各种类型，以及结构体定义
*/
#ifndef __PROCESS_TYPE_HEAD_FILE__
#define __PROCESS_TYPE_HEAD_FILE__

#include "CodecDefine.h"

#ifndef IMAGE_RESIZE_METHOD_DEFINE
#define IMAGE_RESIZE_METHOD_DEFINE

/**
@enum IMAGE_RESIZE_METHOD
@brief 图像类型
*/

enum IMAGE_RESIZE_METHOD
{
    IRM_FULLSCREEN,		///<stretch to full screen 
    IRM_ORIGINAL_SCALE, ///<keep scale, fit to size of output
    IRM_16_9,			///<resize to 16:9, fit to size of output
    IRM_4_3,			///<resize to 4:3, fit to size of output
    IRM_LETTERBOX,		///<lefterbox
    IRM_PAN_SCAN		///<pan scan
};
#endif

#ifndef RESIZE_PARAM_DEFINE
#define RESIZE_PARAM_DEFINE
/**
@struct RESIZE_PARAM
@brief 图像缩放交织类型结构体
*/
struct RESIZE_PARAM
{
    int					width;	///< 图像宽
    int					height;	///< 图像高
    IMAGE_INTERPOLATION	uInterpolation; ///< 交织类型
    IMAGE_RESIZE_METHOD uStyle; ///<图像scale类型
};

#endif

#ifndef DEINTERLACE_INTERPOLATION_DEF
#define DEINTERLACE_INTERPOLATION_DEF
/**
@enum DEINTERLACE_STYLE
@brief 视频交织类型
*/
enum DEINTERLACE_STYLE
{
    DIS_NONE,				///< 不使用
    DIS_ODD,				///< 重复奇数行
    DIS_EVEN,				///< 重复偶数行
    DIS_ODD_INTERPOLATE,	///< 使用奇数行插值
    DIS_EVEN_INTERPOLATE,	///< 使用偶数行插值
    DIS_AUTO,				///< 自动处理（暂不支持）
};

#endif // DEINTERLACE_INTERPOLATION_DEF

#ifndef FLOAT_RECT_DEF
#define FLOAT_RECT_DEF
/**
@struct tagFRECT
@brief 定义矩形
*/
typedef struct tagFRECT
{
    float   left;	///< 矩形左上顶点 X 坐标
    float   top;	///<矩形左上顶点 Y坐标
    float   right;	///<矩形右下顶点 X 坐标
    float   bottom;	///<矩形右下顶点 Y 坐标
} FRECT, *PFRECT;
#endif

// 扩展滤镜ID定义 
// EX_FILTER_ID


#define EX_FILTER_GRAY			0x0001	///<灰色
#define EX_FILTER_EMBOSS			0x0002	///<浮雕
#define EX_FILTER_NEGATIVE		0x0004	///<负片
#define EX_FILTER_BLACK_WHITE		0x0008	///<黑白
#define EX_FILTER_WHITE_BLACK		0x0010	///<白黑
#define EX_FILTER_OLD_PHOTO		0x0020	///<老照片

// FLIP_FLAG
#define FLIP_HORIZONTAL			0x0001 ///< 水平翻转
#define FLIP_VERTICAL			0x0002 ///< 垂直翻转

#endif // __PROCESS_TYPE_HEAD_FILE__
