// DecodePlugin.h

#pragma once

#include "ParserPlugins.h"
#include "DemuxParam.h"
#include "FileInfo.h"
#include "CodecFrame.h"



typedef void* HDECODEPLUGIN;

// HDECODEPLUGIN __stdcall Init(const StreamInfo* pStreamInfo, const EXTRA_DATA* pExtraData);
typedef HDECODEPLUGIN (__stdcall* DECODER_INIT_API)(const StreamInfo*, const EXTRA_DATA* pExtraData);

// void __stdcall PutData(HDECODEPLUGIN hDecoder, MEDIA_BLOCK* pBlock); // pBlock为0表示流结束
typedef int (__stdcall* DECODER_PUTDATA_API)(HDECODEPLUGIN, const MEDIA_BLOCK*);

// MEDIA_FRAME* __stdcall GetNextFrame(HDECODEPLUGIN hDecoder);
typedef MEDIA_FRAME* (__stdcall* DECODER_GETNEXTFRAME_API)(HDECODEPLUGIN);

//void __stdcall Reset(HDECODEPLUGIN hDecoder);
typedef void (__stdcall* DECODER_RESET_API)(HDECODEPLUGIN);

//void __stdcall Close(HDECODEPLUGIN hDecoder);
typedef void (__stdcall* DECODER_CLOSE_API)(HDECODEPLUGIN);

struct DecodePluginItem
{
	wchar_t		szID[128];
	DWORD		dwFourCC;
	AV_STREAM_TYPE dwType;
	int			nPriority;

	PARSE_VIDEO_DATA_API		fnVideoParse;
	PARSE_AUDIO_DATA_API		fnAudioParse;

	DECODER_INIT_API			fnInit;
	DECODER_PUTDATA_API			fnPutData;
	DECODER_GETNEXTFRAME_API	fnGetNextFrame;
	DECODER_RESET_API			fnReset;
	DECODER_CLOSE_API			fnClose;	
};

EXPORT_API const DecodePluginItem* __stdcall GetDecoderInfoArray(); // 解码插件必须导出该函数 
typedef const DecodePluginItem* (__stdcall* GET_DECODERINFO_ARRAY_API)();

