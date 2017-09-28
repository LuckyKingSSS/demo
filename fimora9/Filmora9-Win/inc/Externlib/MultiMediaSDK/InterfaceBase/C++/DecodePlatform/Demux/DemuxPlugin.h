// DemuxPlugin.h

#pragma once
#ifndef WIN32
#include "Linux32_Typedef.h"
#endif
#include "DemuxParam.h"
#include "StreamReader.h"
#include "FileInfo.h"
typedef void* HDEMUXPLUGIN;
typedef void* HFILEREADER;


struct DEMUX_PARAM
{
	int		nProgramIndex; // DVD里面的Title编号
	DWORD	dwReserved;
};


// HDEMUXPLUGIN __stdcall DemuxCreate(STREAM_READER reader, DWORD dwFourCC, DEMUX_STYLE flag)
typedef HDEMUXPLUGIN	(__stdcall* DEMUX_CREATE_API)(STREAM_READER reader, DWORD dwFourCC, DEMUX_STYLE flag, const DEMUX_PARAM* pParam, const MEDIA_FILE_INFO *pMediaInfo);

//DWORD __stdcall WSDemuxGetMaxBlockSize(HDEMUXPLUGIN hDemux); // 告知使用者需要为每个Block分配多大的内存
typedef DWORD (__stdcall* DEMUX_GETMAXBLOCKSIZE_API)(HDEMUXPLUGIN);

//BOOL __stdcall WSDemuxGetBlock(HDEMUXPLUGIN hDemux, MEDIA_BLOCK* pBlock, STREAM_GET_FLAG flag);  // pBlock(pBlockData) 由使用者分配内存
typedef BOOL	(__stdcall* DEMUX_GETBLOCK_API)(HDEMUXPLUGIN, MEDIA_BLOCK*, STREAM_GET_FLAG flag);

//EXTRA_DATA* __stdcall WSDemuxGetExtraData(HDEMUXPLUGIN hDemux, unsigned int uStreamID);
typedef EXTRA_DATA*	(__stdcall* DEMUX_GETEXTRADATA_API)(HDEMUXPLUGIN, unsigned int uStreamID);


// DEMUX_SEEK_FLAG
#define DEMUX_SEEK_FLAG_TIME		0x0001			
#define DEMUX_SEEK_FLAG_FRAME		0x0002
#define DEMUX_SEEK_FLAG_BLOCK		0x0004

//__int64 __stdcall WSDemuxSeek(HDEMUXPLUGIN hDemux, __int64 pos, int flag, unsigned int uStreamID); // pos - time(1/10000000 second) 
typedef __int64 (__stdcall* DEMUX_SEEK_API)(HDEMUXPLUGIN, __int64, int, unsigned int);

//void __stdcall WSDemuxDestroy(HDEMUXPLUGIN hDemux);
typedef void	(__stdcall* DEMUX_DESTROY_API)(HDEMUXPLUGIN);


struct DemuxInfo
{
	wchar_t						szID[128];
	DWORD						dwFourCC;
	int							nPriority;

	DEMUX_CREATE_API 			fnCreate;
	DEMUX_GETMAXBLOCKSIZE_API	fnGetMaxBlockSize;
	DEMUX_GETBLOCK_API			fnGetBlock;
	DEMUX_SEEK_API				fnSeek;
	DEMUX_DESTROY_API			fnDestroy;

	DEMUX_GETEXTRADATA_API		fnGetExtraData;
};

EXPORT_API const DemuxInfo* __stdcall GetDemuxInfoArray(); // Demux插件必须导出该函数
typedef const DemuxInfo* (__stdcall* GET_DEMUXINFO_ARRAY_API)();


