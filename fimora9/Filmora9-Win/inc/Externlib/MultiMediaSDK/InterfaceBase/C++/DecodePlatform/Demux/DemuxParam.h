// DemuxParam.h

#pragma once


#ifndef EXPORT_API
#ifdef __cplusplus
	#define EXPORT_API extern "C"
#else
	#define EXPORT_API
#endif
#endif

#ifndef WIN32
#include "Linux32_Typedef.h"
#endif
#include "ExtraData.h"
#include "FileInfo.h"

// 流读取控制标识
enum STREAM_GET_FLAG
{
	SGF_ALL_STREAM,		// 读取所有的Stream
	SGF_APPOINT_STREAM,	// 读取指定的Stream: pBlock->uStreamID
};

// 数据分离方式
enum DEMUX_STYLE
{
	DS_FULL_BLOCK,	// 分离完整的Block数据	
	//DS_DEC_ONLY,	// 分离出解码器需要的数据
};


#define TIMESTAMP_SCALE		(10000000)

// Block数据结构定义
struct MEDIA_BLOCK 
{
	AV_STREAM_TYPE	uStreamType;	// Stream类型
	UINT			uStreamID;		// Stream ID
	DWORD			dwFourCC;		// 流FourCC

	// 时间戳 将Block数据里的时间戳换算为单位为1.0 / TIMESTAMP_SCALE second 的时间戳
	__int64			pts;			// Unit: 1.0 / TIMESTAMP_SCALE second
	__int64			dts;			// Unit: 1.0 / TIMESTAMP_SCALE second

	BYTE*			pBlockData;		// Block数据
	DWORD			dwBlockSize;	// Block数据缓冲区大小
	DWORD			dwHdrSize;		// Block头长度 pBlockData + dwHdrSize 为解码器需要的数据
	DWORD			dwValidLength;	// 从dwHdrSize开始的有效数据长度 

	int				flag;			// 低位为1，表示该Block包含关键帧
};

