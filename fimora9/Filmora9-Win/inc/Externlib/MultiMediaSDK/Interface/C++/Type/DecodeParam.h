//DecodeParam.h
/*! \file DecodeParam.h
* \brief							解码器设置信息
*/

#pragma once
#include "CodecDefine.h"
#ifndef MEDIA_FILE_TYPE
#define MEDIA_FILE_TYPE

/*!
*\enum FILE_TYPE
*\ brief 媒体文件类型，媒体文件还是外挂字幕
*/
enum FILE_TYPE 
{
	/*! 媒体文件*/
	MEDIA_FILE,		

	/*! 外挂字幕*/
	SUB_PIC,
};
#endif 

// DECODE_EX_PARAM_MESSAGE
// SetExDecodeParam函数使用的uMsg
#define DEM_DEINTERLACE			1000	// 隔行扫描插值算法： wParam - DEINTERLACE_STYLE
#define CHANGE_AUIDO_STREAM		1010	//动态改变音频流：　wParam　新音频流的Stream ID
#define CHANGE_SUBTITLE_STREAM	1011	//动态改变字幕流： wParam 新字幕流的Stream ID
#define CHANGE_VIDEO_STREAM		10012	//动态改变视频字幕流： wParam 新字幕流的Stream ID

#ifndef VIDEO_DEC_PARAM
#define VIDEO_DEC_PARAM

/*! 
* \struct DecParam
* \brief  解码参数
*/
struct DecParam
{
	/*! 结构体的大小用于做版本控制*/
	int					nSize;

	/*! 媒体文件的FourCC*/
	UINT				uMediaType;			

	/*! 节目流ID*/
	UINT				uProgramID;			

	/*! 音频流ID*/
	int					nTrack;				

	/*! 字幕流ID*/
	int					nSubTrack;			

	/*! 直接读取流数据*/
	int					bDirectCopy;		

	/*! 所使用的隔行扫描插值算法*/
	DEINTERLACE_STYLE	uDeintelace;	

	/*! 解码参数*/
	WPARAM				wParam;

	/*! 解码参数*/
	LPARAM				lParam;				

	/*! 是否使用解码多线程，1表示使用， 0表示不使用*/
	int					bUseThreadDecode;	
	    				                 	

	/*! 外挂字幕的宽*/
	int					nWidth;			

	/*! 外挂字幕的高*/
	int					nHeight;
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