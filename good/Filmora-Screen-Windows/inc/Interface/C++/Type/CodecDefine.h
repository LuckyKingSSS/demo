// CodecDefine.h

//DecodeParam.h
/*! \file CodecDefine.h
* \brief							解码定义信息
*/

#pragma once

#ifndef _DEC_MODEL_DEFINE
#define _DEC_MODEL_DEFINE
/*!
* \enum DEC_MODEL
* \brief 定义解码方式
*/
enum DEC_MODEL		// 定义解码方式
{
	/*! 自动处理*/
	AUTO_MODEL = 0,	
	
	/*! DShow解码器，仅WINDOWS有效*/
	DSHOW_MODEL,

	/*! FFMPEG解码， 以不使用*/
	FFDEC_MODEL,

	/*! DVD解码器*/
	DVD_MODEL,	

	/*! AVDec解码器 WS_AVDec.dll 基于WS Video Decoder Platform*/
	AVDEC_MODEL,

	/*! Flash解码器， 以不再使用*/
	SWF_MODEL,		

	/*!  mtx文件解码器*/
	MTXML_MODEL,

	/*! 图片解码器*/
	IMAGE_MODEL,  

	/*! QuickTime解码器*/
	QTML_MODEL,	

	/*! 外挂字幕解码器*/
	SUBPIC_MODEL,	

	/*! 用于初始化音视频模式用*/
	NO_SET_MODEL,  
	MPDEC_MODEL,	//MPlayer解码器

	SYSTEMDEC_MODEL,//SYSTEM 解码器
};

#endif	// _DEC_MODEL_DEFINE


#ifndef _AUDIO_INTERPOLATION_DEFINE
#define _AUDIO_INTERPOLATION_DEFINE

/*!
* \enum AUDIO_INTERPOLATION
* \brief 音频采样率转换插值算法
*/
enum AUDIO_INTERPOLATION	// 音频采样率转换插值算法
{
	/*! 一次线性插值*/
	SIMPLE_LINEAR = 0,

	/*! 二次线性插值(暂未实现)*/
	QUADRATIC_LINEAR,
};

#endif // _AUDIO_INTERPOLATION_DEFINE


#ifndef IMAGE_INTERPOLATION_DEFINE
#define IMAGE_INTERPOLATION_DEFINE
/*!
* \enum IMAGE_INTERPOLATION
* \brief 图像插值算法
*/
enum IMAGE_INTERPOLATION	// 图像插值算法
{
	/*! 自动选择插值算法*/
	IIN_AUTO,			

	/*! Nearest value (邻近插值)*/
	IIN_FAST,			

	/*! Bi-Liner Interpolation (双线性插值)*/
	IIN_BI_LINER,		

	/*! Tri-Convolute Interpolation (卷积插值)*/
	IIN_TRI_CONVOLUTE	
};

#endif	// IMAGE_INTERPOLATION_DEFINE


#ifndef IMAGE_RESIZE_METHOD_DEFINE
#define IMAGE_RESIZE_METHOD_DEFINE
/*!
* \enum IMAGE_RESIZE_METHOD
* \brief 图像scale设置
*/
enum IMAGE_RESIZE_METHOD
{
	/*! stretch to full screen*/
	IRM_FULLSCREEN,		

	/*! keep scale, fit to size of output*/
	IRM_ORIGINAL_SCALE, 

	/*! resize to 16:9, fit to size of output*/
	IRM_16_9,			

	/*! resize to 4:3, fit to size of output*/
	IRM_4_3,			

	/*! resize to LETTERBOX, fit to size of output*/
	IRM_LETTERBOX,

	/*! resize to PAN_SCAN, fit to size of output*/
	IRM_PAN_SCAN
};
#endif

#ifndef DEINTERLACE_INTERPOLATION_DEF
#define DEINTERLACE_INTERPOLATION_DEF

/*!
* \enum DEINTERLACE_STYLE
* \brief 除去隔行扫描方法
*/
enum DEINTERLACE_STYLE
{
	/*! 不做处理*/
	DIS_NONE,	

	/*! 重复奇数行*/
	DIS_ODD,				

	/*! 重复偶数行*/
	DIS_EVEN,			

	/*! 使用奇数行插值*/
	DIS_ODD_INTERPOLATE,

	/*! 使用偶数行插值*/
	DIS_EVEN_INTERPOLATE,	

	/*! 自动处理（暂不支持）*/
	DIS_AUTO,
};

#endif // DEINTERLACE_INTERPOLATION_DEF


#ifndef _DOUBLE_RECT_DEFINE
#define _DOUBLE_RECT_DEFINE

struct D_RECT				// 定义位置相对坐标，参考值为输出图像分辨率
{
	double		dTop;		// 顶部坐标，为相对坐标，取值 0.0～1.0，下同
	double		dBottom;
	double		dLeft;
	double		dRight;
};

#endif // _DOUBLE_RECT_DEFINE

#ifndef _CLIP_TIME_DEFINE
#define _CLIP_TIME_DEFINE

/*!
* \struct CLIP_TIME
* \brief 定义剪辑信息
*/
struct CLIP_TIME
{
	/*! 相对输出（音）视频的起止时间，单位：秒，若 剪切长度 != 输出长度，则会产生拉伸效果*/
	double		dStart;		

	/*! 此值为零则无拉伸效果*/
	double		dEnd;		 

	/*! 剪切时间段之起点，单位：秒，下同*/
	double		dTrimFrom;	 

	/*!  剪切时间段之终点，0值表示尾端不进行剪切*/
	double		dTrimTo;	
};

#endif // _CLIP_TIME_DEFINE

/*!
* \enum EncMediaType
* \brief 编码类型信息
*/
enum EncMediaType
{
	/*! 编码音频*/
	ENC_AUDIO_TYPE,

	/*! 编码视频*/
	ENC_VIDEO_TYPE,
};

/*!
* \enum ACCELERATE_DECODEC
* \brief 解码类型信息
*/
enum ACCELERATE_DECODEC_TYPE
{
	ACCELERATE_GPU_UNUSE = 0,
	ACCELERATE_GPU_AUTO = 1,//
	ACCELERATE_GPU_H264 = 2, //unsupported
	ACCELERATE_GPU_MPEG2 = 4,//unsupported
	ACCELERATE_GPU_MPEG4 = 8,//unsupported
	ACCELERATE_GPU_VC1	 = 16,//unsupported
	ACCELERATE_GPU_VP9	 = 32,//unsupported

};