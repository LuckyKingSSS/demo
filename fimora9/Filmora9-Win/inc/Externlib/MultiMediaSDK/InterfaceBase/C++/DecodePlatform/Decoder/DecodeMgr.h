// DecodeMgr.h
#pragma once
#ifndef WIN32
#include "Linux32_Typedef.h"
#endif
#include "CodecFrame.h"
#include "FileInfo.h"
#include "DecodePlugin.h"

#ifdef __cplusplus
extern "C" 
{
#endif

BOOL __stdcall WSInitDecodeMgr();
void __stdcall WSUninitDecodeMgr();


struct DECOER_ITEM;
typedef DECOER_ITEM* HDECODER;

HDECODER __stdcall WSDecoderInit(const StreamInfo* pStreamInfo, const EXTRA_DATA* pExtraData);
int __stdcall WSDecoderPutData(HDECODER hDecoder, MEDIA_BLOCK* pBlock); // pBlock为0表示流结束
MEDIA_FRAME* __stdcall WSDecoderGetNextFrame(HDECODER hDecoder);
void __stdcall WSDecoderReset(HDECODER hDecoder);
void __stdcall WSDecoderClose(HDECODER hDecoder);


// 返回-1表示没有dwFourCC对应的Parser
int __stdcall WSParseVideoInfo(DWORD dwFourCC, MEDIA_BLOCK* pBlock, PARSE_CONTEXT* pContext, WSVideoInfo* pVidInfo, int* pnBitrate, int* pnReserved = 0);
int __stdcall WSParseAudioInfo(DWORD dwFourCC, MEDIA_BLOCK* pBlock, PARSE_CONTEXT* pContext, WSAudioInfo* pAudInfo, int* pnBitrate, int* pnReserved = 0);

int __stdcall WSDecoderGetCount();
const DecodePluginItem* __stdcall WSDecoderGetItem(int nIndex);


#ifdef __cplusplus
}
#endif
