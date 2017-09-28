// ParserPlugins.h 

#pragma once


#include "WSVideoInfo.h"
#include "WSAudioInfo.h"
#include "DemuxParam.h"

enum PARSE_MESSAGE
{
	PM_INIT,
	PM_PARSE,
	PM_END
};

struct PARSE_CONTEXT
{
	PARSE_MESSAGE	msg;		// Parse 命令
	void*			handle;		// 由具体的Parser实现者分配或销毁
	DWORD			dwFourCC;	// 当前流FourCC
};

typedef void (__stdcall* PARSE_VIDEO_DATA_API)(const MEDIA_BLOCK* pBlock, PARSE_CONTEXT* pContext, WSVideoInfo* pVidInfo, int* pnBitrate, int* pnReserved);
typedef void (__stdcall* PARSE_AUDIO_DATA_API)(const MEDIA_BLOCK* pBlock, PARSE_CONTEXT* pContext, WSAudioInfo* pAudInfo, int* pnBitrate, int* pnReserved);
