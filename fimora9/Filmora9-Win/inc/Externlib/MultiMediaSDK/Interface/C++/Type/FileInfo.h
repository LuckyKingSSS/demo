// FileInfo.h 

/*! \file FileInfo.h
* \brief	文件信息
*/
#pragma once

#include "MetaDataInfo.h"
#include "WSVideoInfo.h"
#include "WSAudioInfo.h"


#ifndef STREAM_TYPE_DEF
#define STREAM_TYPE_DEF

/*!
* \enum AV_STREAM_TYPE
* \brief 流的类型 
*/
// Stream类型定义
enum AV_STREAM_TYPE
{
	/*!非媒体流*/
	ST_NONE,
	
	/*! DVD导航数据*/
	ST_NAV,			 
	
	/*! 视频Stream*/
	ST_VIDEO,		// 
	
	/*! 音频Stream*/
	ST_AUDIO,	
	
	/*! 字幕Stream*/
	ST_SUBPIC,
	
	/*! 图像Stream*/
	ST_IMAGE,
};

enum AV_SCAN_TYPE
{
	AV_SCAN_TYPE_PROGRESSIVE, ///< 逐行模式
	AV_SCAN_TYPE_INTERLACED, ///< 交织模式
	AV_SCAN_TYPE_UNKNOWN,	///< 模式未知
	AV_SCAN_TYPE_RESERVED	///< 保留字段

};
#endif

/*!
* \struct StreamInfo
* \brief 流的信息
* \see WSVideoInfo WSAudioInfo AV_STREAM_TYPE
*/
struct StreamInfo
{
	/*! 流类型*/
	AV_STREAM_TYPE	uStreamType;

	/*! 流ID*/
	UINT			uStreamID;

	/*! 流FourCC*/
	DWORD			dwFourCC;

	/*! 0 - 正常  -1 找不到dwFourCC对应的解码器*/
	int				nState;	

	/*! 流名称*/
	wchar_t			szName[64];

	/*! 流的描述信息*/
	wchar_t			szDescription[128];

	/*! 流的时间长度*/
	double			dStreamLength;

	/*! 流的码率*/
	int				nBitrate;

	/*! 流的视频信息， 当uStreamType为ST_VIDEO时有效*/
	WSVideoInfo 	vidInfo;


	/*! 流的视频原始信息， 当uStreamType为ST_VIDEO时有效*/
	WSVideoInfo 	vidInfoSrc;

	/*! 流的音频信息， 当uStreamType为ST_AUDIO时有效*/
	WSAudioInfo 	audInfo;	

	/*! 当前流的第一个时间戳*/
	__int64			ptsStart;	

	/*! 当前流的第一个时间戳和该路节目所有流中ptsStart最小的那个ptsStart的差值*/
	__int64			ptsOffset;	

	/*! 当前流的隔行或逐行扫描 */
	AV_SCAN_TYPE	scanType;

	/*! 当前流的原始隔行或逐行扫描 */
	AV_SCAN_TYPE	scanTypeSrc;

	/*! 保留字段 暂时用作 mov sprite 的roate */
	DWORD			dwReserved1;

	/*! 保留字段*/
	DWORD			dwReserved2;
	
};

/*!
* \enum SUPPIC_TYPE
* \brief  字幕的类型，内嵌还是外挂
*/
enum SUPPIC_TYPE
{
	/*! 内嵌字幕*/
	SPT_Internal,
	
	/*! 外置字幕*/
	SPT_External,
};

/*!
* \struct SubPicStreamInfo
* \brief  字幕的信息
* \see SUPPIC_TYPE
*/
struct SubPicStreamInfo
{
	/*! 字幕的类型*/
	SUPPIC_TYPE		uSPType;

	/*! 字幕流的ID*/
	UINT			uStreamID;

	/*! 字幕流的 FourCC*/
	DWORD			dwFourCC;

	/*! 字幕流的语言*/
	wchar_t			szLang[64];

	/*! 字幕流的描述*/
	wchar_t			szDescription[128];
};


/*!
* \struct ProgramInfo
* \brief  节目信息
* \see StreamInfo SubPicStreamInfo
*/
struct ProgramInfo
{
	/*! 字幕的Numer 暂无意义*/
	DWORD				dwNumber;

	/*! 节目流ID*/
	UINT				uPID;

	/*! 节目的时长*/
	double				dMediaLength;

	/*! 节目流的码率*/
	int					nBitrate;

	/*! 节目流中视频流的数目*/
	int					nVideoStreamCount;

	/*! 节目流中音频流的数目*/
	int					nAudioStreamCount;

	/*! 节目流中视频信息*/
	StreamInfo*			pVidStreamInfos;

	/*! 节目流中音频信息*/
	StreamInfo*			pAudStreamInfos;

	/*! 时间戳参考音频流ID*/
	UINT				uTimeStampReferenceStreamID;

	/*! 节目流中字幕流的数目*/
	int					nSubPicStreamCount;

	/*! 节目流中字幕流信息*/
	SubPicStreamInfo*	pSubPicInfos;
};

struct MEDIA_FILE_INFO;
typedef void (__stdcall* MEDIA_INFO_RELEASE_API)(MEDIA_FILE_INFO* pMediaFileInfo);

/*!
* \struct 	MEDIA_FILE_INFO
* \brief 媒体文件信息
*  一个媒体文件中存在1路或多路节目，一个节目中存在1路或多路（也可能不存）音视频和字幕流
* \see MetaDataInfo ProgramInfo
*/
struct MEDIA_FILE_INFO
{
	/*! 媒体文件FourCC*/
	DWORD			dwFourCC;

	/*! 文件名*/
	wchar_t			szName[512];

	/*! 文件的描述信息*/
	wchar_t			szDescription[128];

	/*! MetaData 信息如(ID3信息)*/
    MetaDataInfo    metaDataInfo;

	/*! 媒体文件时间长度(取各个节目流中，最长的那路)*/
	double			dMediaLength;

	/*! 媒体文件的码率，(各路节目流码率之和)*/
	int				nBitrate;

	/*! 媒体文件的节目数*/
	int				nProgramCount;
	/*! 媒体文件的节目信息*/
	ProgramInfo*	pProgramInfos;

	/*! 隔行或逐行扫描 */
	AV_SCAN_TYPE	scanType;

	/*! 释放MediaInfo数据内存的函数指针*/
	MEDIA_INFO_RELEASE_API	fnRelease;
};


// MediaInfo.dll
MEDIA_FILE_INFO* __stdcall MFICreate();
ProgramInfo* __stdcall MFIAddProgram(MEDIA_FILE_INFO* pMediaFileInfo, DWORD dwNumber, UINT uPID);
StreamInfo* __stdcall MFIAddStream(ProgramInfo*, AV_STREAM_TYPE uStreamType, UINT uStreamID, DWORD dwFourCC);
SubPicStreamInfo* __stdcall MFIAddSubPicStream(ProgramInfo*, SUPPIC_TYPE uStreamType, UINT uStreamID, DWORD dwFourCC);
int __stdcall MFIEraseStream(ProgramInfo* pProgram, AV_STREAM_TYPE uStreamType, UINT uStreamID);
void __stdcall MFIRelease(MEDIA_FILE_INFO* pMediaFileInfo);

MEDIA_FILE_INFO* __stdcall MFIClone(const MEDIA_FILE_INFO *pMediaInfoSrc);


#ifdef __cplusplus

/*!
* \fn __inline ProgramInfo* FindProgram(MEDIA_FILE_INFO* pMediaInfo, UINT uProgramID)
* \brief 从MEDIA_FILE_INFO中，找到指定节目流ID的，节目流信息指针。
* \param pMediaInfo [in] 媒体文件信息
* \param uProgramID [in] 节目流ID
* \retval 节目流指针
* \see  MEDIA_FILE_INFO ProgramInfo
* \note 如果没有找到，那么返回第一路节目流
*/
__inline ProgramInfo* FindProgram(MEDIA_FILE_INFO* pMediaInfo, UINT uProgramID)
{
	// 如果找不到，取第一个Program
	ProgramInfo* pProgram = pMediaInfo->pProgramInfos;

	for (int i = 0; i < pMediaInfo->nProgramCount; ++i)
	{
		if (pMediaInfo->pProgramInfos[i].uPID == uProgramID)
		{
			pProgram = &pMediaInfo->pProgramInfos[i];
			break;
		}
	}

	return pProgram;
}


/*!
* \fn __inline StreamInfo* FindVideoStream(ProgramInfo* pProgram, UINT uStreamID)
* \brief 从ProgramInfo中，找到指定视频流ID的，视频流信息指针。
* \param pProgram [in] 节目流信息
* \param uStreamID [in] 视频流ID
* \retval 视频流指针
* \see  ProgramInfo StreamInfo
* \note 如果没有找到，那么返回第一路视频流
*/
__inline StreamInfo* FindVideoStream(ProgramInfo* pProgram, UINT uStreamID)
{
	// 如果找不到，取第一个Program
	StreamInfo* pStream = pProgram->pVidStreamInfos;

	for (int i = 0; i < pProgram->nVideoStreamCount; ++i)
	{
		if (pProgram->pVidStreamInfos[i].uStreamID == uStreamID)
		{
			pStream = &pProgram->pVidStreamInfos[i];
			break;
		}
	}

	return pStream;
}

/*!
* \fn __inline StreamInfo* FindAudioStream(ProgramInfo* pProgram, UINT uStreamID)
* \brief 从ProgramInfo中，找到指定音频流ID的，音频流信息指针。
* \param pProgram [in] 节目流信息
* \param uStreamID [in] 音频流ID
* \retval 音频流指针
* \see  ProgramInfo StreamInfo
* \note 如果没有找到，那么返回第一路音频流
*/
__inline StreamInfo* FindAudioStream(ProgramInfo* pProgram, UINT uStreamID)
{
	// 如果找不到，取第一个Program
	StreamInfo* pStream = pProgram->pAudStreamInfos;

	for (int i = 0; i < pProgram->nAudioStreamCount; ++i)
	{
		if (pProgram->pAudStreamInfos[i].uStreamID == uStreamID)
		{
			pStream = &pProgram->pAudStreamInfos[i];
			break;
		}
	}

	return pStream;
}

/*!
* \fn __inline SubPicStreamInfo* FindSubPicStream(ProgramInfo* pProgram, UINT uStreamID)
* \brief 从ProgramInfo中，找到指定字幕流ID的，字幕流信息指针。
* \param pProgram [in] 节目流信息
* \param uStreamID [in] 字幕流ID
* \retval 字幕流指针
* \see  ProgramInfo StreamInfo
* \note 如果没有找到，那么返回第一路字幕流
*/
__inline SubPicStreamInfo* FindSubPicStream(ProgramInfo* pProgram, UINT uStreamID)
{
	SubPicStreamInfo* pStream = 0;
	for (int i = 0; i <  pProgram->nSubPicStreamCount; i++)
	{
		if (pProgram->pSubPicInfos[i].uStreamID == uStreamID)
		{
			pStream = &pProgram->pSubPicInfos[i];
			break;
		}
	}
	return pStream;
}

#endif
