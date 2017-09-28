// YUVImage.h
//
//

#pragma once

#ifndef _YUVIMAGE_DEF
#define _YUVIMAGE_DEF

// YUV图像格式定义
enum YUV_IMAGE_FORMAT
{
	YIF_444,
	YIF_422,
	YIF_YV12 // 420
};

// YUV图像数据结构定义
struct tagYUVImage
{
#ifdef __cplusplus
	YUV_IMAGE_FORMAT format;
#else
	unsigned int format;
#endif

	unsigned char*	y;	 // Y数据指针
	unsigned char*	u;	 // U数据指针
	unsigned char*	v;	 // V数据指针
	unsigned char*	a;	 // Alpha 通道（不透明度） 

	int width;   // 图像宽度
	int height;  // 图像高度

	int y_pitch; // Y数据每行所占字节数
	int u_pitch; // U数据每行所占字节数
	int v_pitch; // V数据每行所占字节数
} ;

typedef struct tagYUVImage YUVImage;

#endif


