// DemuxMgr.h
#pragma once
#ifndef WIN32
#include "Linux32_Typedef.h"
#endif
#include "DemuxPlugin.h"
#include "StreamReader.h"
#include "FileInfo.h"

#ifdef __cplusplus
extern "C" 
{
#endif

BOOL __stdcall WSInitDemuxMgr();
void __stdcall WSUninitDemuxMgr();


typedef void* HDEMUX;

HDEMUX __stdcall WSDemuxCreate(STREAM_READER reader, DWORD dwFourCC, DEMUX_STYLE flag = DS_FULL_BLOCK, const DEMUX_PARAM* pParam = 0);
HDEMUX __stdcall WSDemuxFileCreate(const wchar_t* pPathName, DWORD dwFourCC, DEMUX_STYLE flag = DS_FULL_BLOCK, const DEMUX_PARAM* pParam = 0, const MEDIA_FILE_INFO *pMediaInfo = 0); // dwFourCC: 0 - 自动判断 否则则使用指定的Demuxer打开

DWORD __stdcall WSDemuxGetMaxBlockSize(HDEMUX hDemux);
BOOL __stdcall WSDemuxGetBlock(HDEMUX hDemux, MEDIA_BLOCK* pBlock, STREAM_GET_FLAG flag = SGF_ALL_STREAM);
__int64 __stdcall WSDemuxSeek(HDEMUX hDemux, __int64 pos, int flag, unsigned int uStreamID); // pos - time(1/10000000 second)

EXTRA_DATA* __stdcall WSDemuxGetExtraData(HDEMUX hDemux, unsigned int uStreamID);
void __stdcall WSDemuxReleaseExtraData(EXTRA_DATA*  pExtraData);

void __stdcall WSDemuxDestroy(HDEMUX hDemux);

BOOL __stdcall WSDemuxEnableMultiThread(HDEMUX hDemux, BOOL bEnable); // 返回先前的状态


MEDIA_BLOCK* __stdcall WSDemuxCreateBlock(AV_STREAM_TYPE type, int data_size);
void __stdcall WSDemuxDestroyBlock(MEDIA_BLOCK* pBlock);


#ifdef __cplusplus
}
#endif
