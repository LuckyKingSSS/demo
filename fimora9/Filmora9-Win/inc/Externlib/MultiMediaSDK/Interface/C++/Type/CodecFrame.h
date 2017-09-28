// CodecFrame.h

//DecodeParam.h
/*! \file CodecFrame.h
* \brief	定义音视频数据参数
*/


#pragma once
#include <string.h> // memset
#include "FileInfo.h"

#ifndef _YUVIMAGE_DEF
#define _YUVIMAGE_DEF

/*!
* \enum YUV_IMAGE_FORMAT
* \brief  YUV图像格式定义 
*/
// YUV图像格式定义
enum YUV_IMAGE_FORMAT
{
	YIF_444,
	YIF_422,
	YIF_YV12 // 420
};

/*!
* \struct YUVImage
* \brief  YUV图像格式定义 
* \see YUV_IMAGE_FORMAT
*/

// YUV图像数据结构定义
struct YUVImage
{
	/*! YUV 的格式*/
	YUV_IMAGE_FORMAT format;

/*! Y数据指针*/
	LPBYTE	y;

	/*! U数据指针*/
	LPBYTE	u;

	/*! V数据指针*/
	LPBYTE	v;

	/*! Alpha 通道（不透明度） */
	LPBYTE	a;

	/*! 图像宽度*/
	int width;  

	/*! 图像高度*/
	int height; 

	/*! Y数据每行所占字节数*/
	int y_pitch;

	/*! U数据每行所占字节数*/
	int u_pitch; // 

	/*! V数据每行所占字节数*/
	int v_pitch;
};
#endif

// 图像格式定义
#ifndef IMAGEFORMAT
#define IMAGEFORMAT
/*!
* \enum IMAGE_FORMAT
* \brief  视频图像的格式 
*/
enum IMAGE_FORMAT
{
	/*! RGB类型*/
	IF_RGB,

	/*! YUV类型*/
	IF_YUV,

	/*! 压缩数据*/
	IF_COMPRESSED,
};
#endif

enum  FRAME_DATA_TYPE
{
	DATA_COMPRESS =0,
	DATA_UNCOMPRESS,
	DATA_CONTROL
};

// 图像数据结构定义
//JYY CHANGE 这里的结构体都是用C++方式定义，但是有些插件MPEG系列都是用C的方式，编译不过，
//我只有用C的方式重定义了个头文件MediaType,防止重复定义加上了这些宏
#ifndef MEDIAFRAME  
#define MEDIAFRAME
/*!
* \struct VideoImage
* \brief  视频图像参数
* \see IMAGE_FORMAT
*/
struct VideoImage
{
	/*! 图像格式*/
	IMAGE_FORMAT	format;

	/*!
	* \union 
	* \brief 格式类型
	*/
	union
	{
		/*!
		* \brief IF_YUV类型参数
		* \see YUV_IMAGE_FORMAT
		*/
		struct
		{
			/*! YUV格式类型*/
			YUV_IMAGE_FORMAT yuv_format;

			/*! Y数据指针*/
			LPBYTE	y;

			/*! U数据指针*/
			LPBYTE	u;	 

			/*! V数据指针*/
			LPBYTE	v;	

			/*! Alpha 通道（不透明度） */
			LPBYTE	a;	
			
			/*! 图像宽度*/
			int width;  
			
			/*! 图像高度*/
			int height; 

			/*! Y数据每行所占字节数*/
			int y_pitch; 

			/*! U数据每行所占字节数*/
			int u_pitch;

			/*! V数据每行所占字节数*/
			int v_pitch; 

			int a_pitch;
		};

		/*!
		* \brief IF_RBG类型参数
		*/
		struct
		{
			/*! 位图信息*/
			BITMAPINFOHEADER	biHeader;

			/*! 位图数据*/
			LPBYTE				lpBits;
		};

		/*!
		* \brief 压缩数据
		*/
		struct // 压缩数据
		{
			/*! 压缩数据首地址*/
			BYTE*	pData;

			/*! 压缩数据大小*/
			int		nBytes;

			/*! 缓冲区大小*/
			int		nBufSize; // pData 缓冲区大小
		};
	};

	/*! 指示标志 低位为1表示关键帧*/
	int	flag;//
};

#ifndef SUBPICFRAME  
#define SUBPICFRAME

/*!
* \struct SubPicFrame
* \brief  字幕数据参数
* \see YUV_IMAGE_FORMAT
*/
struct SubPicFrame
{
	/*! 字幕类型*/
	YUV_IMAGE_FORMAT format;

	/*! Y数据指针*/
	LPBYTE	y;				

	/*! U数据指针*/
	LPBYTE	u;	 			

	/*! V数据指针*/
	LPBYTE	v;	 			

	/*! Alpha 通道（不透明度）*/
	LPBYTE	a;	 			 

	/*! 对应于视频中起始坐标*/
	int dx;		 			

	/*! 对应于视频中起始坐标*/
	int dy;

	/*! 字幕图像宽度*/
	int width;   			

	/*! 字幕图像高度*/
	int height;  			

	/*! 字幕中有效起始坐标*/
	int sx;					

	/*! 字幕中有效起始坐标*/
	int sy;

	/*! 字幕中有效图像大小，对应于sx、sy*/
	int validwidth;			

	/*! validwidth <= width   validheight <= height*/
	int validheight;			

	/*! Y数据每行所占字节数*/
	int y_pitch; 			

	/*! U数据每行所占字节数*/
	int u_pitch; 			

	/*! V数据每行所占字节数*/
	int v_pitch; 			

	/*! 起始时间戳，单位1/10000000 second*/
	__int64	StartPts;	

	/*! 显示持续时间 1/10000000 second*/
	int		duration;	
};
#endif

/*!
* \struct AudioFrame
* \brief  音频数据参数
*/
struct AudioFrame 
{
	/*! 采样率*/
	int		nSampleRate;

	/*! 声道数*/
	int		nChannels;

	/*! 一次采样所使用的BIT*/
	int		nBitsPerSample;

	/*! 音频数据指针*/
	BYTE*	pData;

	/*! 音频数据量*/
	int		nBytes;

	/*! pData 缓冲区大小*/
	int		nBufSize; 
};

/*!
* \struct MEDIA_FRAME
* \brief  Frame信息
* \see AV_STREAM_TYPE VideoImage AudioFrame SubPicFrame
*/
struct MEDIA_FRAME 
{
	/*! 数据类型*/
	AV_STREAM_TYPE dwType;
	union
	{
		/*! 视频数据*/
		VideoImage	video;

		/*! 音频数据*/
		AudioFrame	audio;

		/*! 字幕数据*/
		SubPicFrame subPic;
	};

	/*! Frame的显示时间戳 单位1/10000000 second*/
	__int64	pts;		

	/*! Frame的显示解码戳 单位1/10000000 second*/
	__int64 dts;		

	/*! Frame的持续时间 单位1/10000000 second*/
	int		duration;	

};
#endif


#ifdef __cplusplus

/*!
* \fn __inline const YUVImage* VideoImage_2_YUVImage(const VideoImage* pVidImage)
* \brief VideoImage格式转换成YUVImage格式输出
* \param pVidImage [in] 输入 VideoImage格式
* \reval 输出YUVImage格式
* \see VideoImage YUVImage
*/
__inline const YUVImage* VideoImage_2_YUVImage(const VideoImage* pVidImage)
{
	if ((0 == pVidImage) || (IF_YUV != pVidImage->format))
		return 0;

	return (const YUVImage*)&pVidImage->yuv_format;
}

/*!
* \fn __inline const YUVImage* MediaFrame_2_YUVImage(const MEDIA_FRAME* frame)
* \brief MEDIA_FRAME格式转换成YUVImage格式输出
* \param frame [in] 输入 MEDIA_FRAME格式
* \reval 输出YUVImage格式
* \see MEDIA_FRAME YUVImage
*/
__inline const YUVImage* MediaFrame_2_YUVImage(const MEDIA_FRAME* frame)
{
	if ((0 == frame) || (IF_YUV != frame->video.format))
		return 0;

	return (const YUVImage*)&frame->video.yuv_format;
}

__inline void ReleaseRGBAFrame(MEDIA_FRAME* frame)
{
	if (0 == frame)
		return;

	delete[] frame->video.lpBits;

	delete frame;

}

__inline MEDIA_FRAME* CreateRGBAFrame(int width, int height, int bAlpha = true, int bBlackImage = true)
{
	MEDIA_FRAME* frame = new MEDIA_FRAME;

	memset(frame, 0, sizeof(MEDIA_FRAME));
	frame->pts = -1;
	frame->dts = -1;
	frame->duration = 0;
	frame->dwType = ST_VIDEO;
	frame->video.format = IF_RGB;
	frame->video.biHeader.biBitCount = 32;
	frame->video.biHeader.biHeight = height;
	frame->video.biHeader.biWidth = width;
	frame->video.biHeader.biSizeImage = frame->video.biHeader.biHeight * frame->video.biHeader.biWidth * frame->video.biHeader.biBitCount / 8;

	if (bAlpha)
	{
		int nsize = frame->video.biHeader.biSizeImage *1.2;

		frame->video.lpBits = new BYTE[nsize];
		memset(frame->video.lpBits, 0xFF, nsize);
	}
	return frame;
}


/*!
* \fn __inline MEDIA_FRAME* CreateYV12Frame(int width, int height, int bAlpha = false, int bBlackImage = true)
* \brief 根据图像的With, height创建YIF_YV12格式的MEDIA_FRAME
* \param width [in] 图像的宽
* \param height [in] 图像的高 
* \param bAlpha [in] 透明度
* \bBlackImage [in] true表明初始化化的图像为黑色
* \reval 输出MEDIA_FRAME格式
* \see MEDIA_FRAME
*/
__inline MEDIA_FRAME* CreateYV12Frame(int width, int height, int bAlpha = true, int bBlackImage = true)
{
	MEDIA_FRAME* frame = new MEDIA_FRAME;

	memset(frame, 0, sizeof(MEDIA_FRAME));
	frame->pts = -1;
	frame->dts = -1;
	frame->duration = 0;
	frame->dwType = ST_VIDEO;
	frame->video.format = IF_YUV;
	frame->video.yuv_format = YIF_YV12;
	frame->video.width = width;
	frame->video.height = height;
	frame->video.y_pitch = width;
	frame->video.u_pitch = (width + 1) / 2;
	frame->video.v_pitch = (width + 1) / 2;
	if (bAlpha)
	{
		frame->video.a_pitch = width;
	}
	

	int y_sz = width * height;
	int u_sz = ((width + 1) / 2) * ((height + 1) / 2);

	int size = y_sz + u_sz * 2;

//	frame->video.y = new BYTE[y_sz + 128];
//	frame->video.u = new BYTE[u_sz + 128];
//	frame->video.v = new BYTE[u_sz + 128];
//	if (bAlpha)
//	{
//		frame->video.a = new BYTE[y_sz + 128];
//	}

	if (bAlpha)
		size += y_sz;

	frame->video.y = new BYTE[size + 512];
	frame->video.v = frame->video.y + y_sz; 
	frame->video.u = frame->video.v + u_sz; 
	if (bAlpha)
		frame->video.a = frame->video.u + u_sz; 

	if (bBlackImage)
	{
		memset(frame->video.y, 16, y_sz);
		memset(frame->video.u, 128, u_sz);
		memset(frame->video.v, 128, u_sz);
		if (bAlpha)
		{
			memset(frame->video.a, 255, y_sz);
		}
	}

	return frame;
}

/*!
* \fn __inline void ReleaseYV12Frame(MEDIA_FRAME* frame)
* \brief 释放YIF_YV12格式的MEDIA_FRAME
* \param frame [in] YIF_YV12格式的MEDIA_FRAME
*/
__inline void ReleaseYV12Frame(MEDIA_FRAME* frame)
{
	if (0 == frame)
		return;

	delete [] frame->video.y;

	delete frame;

}

/*!
* \fn __inline MEDIA_FRAME* CreateAudioFrame(int sample_rate, int channels, int bits_per_sample, int nFrameSize = (32 * 1024))
* \brief 根据音频数据的采样率, 声道数，以及采样位数， 创建音频MEDIA_FRAME
* \param sample_rate [in] 音频的采用率
* \param channels [in] 声道数 
* \param bits_per_sample [in] 采样位数
* \param nFrameSize [in] 初始化缓冲区大小
* \reval 输出音频 MEDIA_FRAME格式
*/
__inline MEDIA_FRAME* CreateAudioFrame(int sample_rate, int channels, int bits_per_sample, int nFrameSize = (32 * 1024))
{
	MEDIA_FRAME* frame = new MEDIA_FRAME;

	memset(frame, 0, sizeof(MEDIA_FRAME));

	frame->pts = -1;
	frame->dts = -1;
	frame->duration = 0;
	frame->dwType = ST_AUDIO;

	frame->audio.pData = new BYTE[nFrameSize];
	memset(frame->audio.pData, 0, nFrameSize);
	frame->audio.nBufSize = nFrameSize;
	frame->audio.nSampleRate = sample_rate;
	frame->audio.nChannels = channels;
	frame->audio.nBitsPerSample = bits_per_sample;

	return frame;
}

/*!
* \fn __inline void ReleaseAudioFrame(MEDIA_FRAME* frame)
* \brief 释放音频 MEDIA_FRAME
* \param frame [in] 音频Frame
*/
__inline void ReleaseAudioFrame(MEDIA_FRAME* frame)
{
	if (0 == frame)
		return;

	delete [] frame->audio.pData;

	delete frame;

}

/*!
* \fn __inline void CopyVideoFrame(MEDIA_FRAME* pDst, const MEDIA_FRAME* pSrc, int bCopyTimeStamp = true,int bAlpha = true )
* \brief 拷贝视频MediaFrame
* \param pDst [in] 目标视频MEDIA_FRAME
* \param pSrc [in] 源视频MEDIA_FRAME 
* \param bCopyTimeStamp [in] 是否拷贝时间戳
* \warning 必须是YIF_YV12格式的视频数据。 目标视频MEDIA_FRAME的宽高要等于源视频MEDIA_FRAME的宽高，否则会导致内存越界
*/
__inline void CopyVideoFrame(MEDIA_FRAME* pDst, const MEDIA_FRAME* pSrc, int bCopyTimeStamp = true,int bAlpha = true)
{
	if (0 == pDst || 0 == pSrc)
		return;

	if (bCopyTimeStamp)
	{
		pDst->pts = pSrc->pts;
		pDst->dts = pSrc->dts;
		pDst->duration = pSrc->duration;
	}

	int y_pitch = pDst->video.y_pitch;
	int u_pitch = pDst->video.u_pitch;
	int v_pitch = pDst->video.v_pitch;
	int a_pitch = pDst->video.a_pitch;

	int y_pitch_src = pSrc->video.y_pitch;
	int u_pitch_src = pSrc->video.u_pitch;
	int v_pitch_src = pSrc->video.v_pitch;
	int a_pitch_src = pSrc->video.a_pitch;

	int width = pDst->video.width;
	int height = pDst->video.height;

	if (y_pitch != y_pitch_src)
	{
		for (int i = 0; i < height; ++i)
		{
			memcpy(pDst->video.y + y_pitch * i, pSrc->video.y + y_pitch_src * i, y_pitch);
			if(pSrc->video.a != NULL  && pDst->video.a != NULL)
			{
				memcpy(pDst->video.a + a_pitch * i, pSrc->video.a + a_pitch_src * i, a_pitch);
			}
		}
	}
	else
	{
		memcpy(pDst->video.y, pSrc->video.y, y_pitch * height);
		if(pSrc->video.a  != NULL && pDst->video.a != NULL)
		{
			memcpy(pDst->video.a, pSrc->video.a, a_pitch * height);
		}
	}
	

	if ( (u_pitch != u_pitch_src) || (v_pitch != v_pitch_src))
	{
		for (int i = 0; i < (height + 1) / 2; ++i)
		{
			memcpy(pDst->video.u + u_pitch * i, pSrc->video.u + u_pitch_src * i, u_pitch);
			memcpy(pDst->video.v + v_pitch * i, pSrc->video.v + v_pitch_src * i, v_pitch);
		}
	}
	else
	{
		memcpy(pDst->video.u, pSrc->video.u, u_pitch * ((height + 1) / 2) );
		memcpy(pDst->video.v, pSrc->video.v, v_pitch * ((height + 1) / 2) );
	}	


}

__inline void CopyAudioFrame(MEDIA_FRAME* pDst, const MEDIA_FRAME* pSrc, int bCopyTimeStamp = true)
{
	if (0 == pDst || 0 == pSrc)
		return;
	
	if (bCopyTimeStamp)
	{
		pDst->pts = pSrc->pts;
		pDst->dts = pSrc->dts;
		pDst->duration = pSrc->duration;
	}
	
	pDst->audio.nBitsPerSample = pSrc->audio.nBitsPerSample;
	pDst->audio.nChannels = pSrc->audio.nChannels;
	pDst->audio.nSampleRate = pSrc->audio.nSampleRate;
	
	if (pSrc->audio.nBytes > pDst->audio.nBufSize)
	{
		delete[] pDst->audio.pData;
		
		pDst->audio.pData = new BYTE[pSrc->audio.nBytes + 1024];
		pDst->audio.nBufSize = pSrc->audio.nBytes + 1024;
	}
	
	memcpy(pDst->audio.pData, pSrc->audio.pData, pSrc->audio.nBytes);
	pDst->audio.nBytes = pSrc->audio.nBytes;
}


#endif
