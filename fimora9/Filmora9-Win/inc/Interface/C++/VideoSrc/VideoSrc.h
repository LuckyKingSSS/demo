// VideoSrc.h

#pragma once

#include "DecodeParam.h"
#include "FileInfo.h"
#include "CodecDefine.h"
#include "CodecFrame.h"

class VideoDecoder;
class AudioDecoder;
namespace DShowDecoder
{
typedef VideoDecoder* PVIDEODECODER;
typedef AudioDecoder* PAUDIODECODER;

// 取得Media信息
MEDIA_FILE_INFO* __stdcall WGetMediaFileInfo(const wchar_t* pFileName);
void __stdcall WReleaseMediaFileInfo(MEDIA_FILE_INFO* pMediaInfo);
IMediaStream *__stdcall WVDCreate(const wchar_t * pFileName, const DecParam* pDecParam, const MEDIA_FILE_INFO *pMediaInfo, int* pError);
IMediaStream * __stdcall WADCreate(const wchar_t * pFileName, const DecParam* pDecParam, const MEDIA_FILE_INFO *pMediaInfo, int* pError);
}
