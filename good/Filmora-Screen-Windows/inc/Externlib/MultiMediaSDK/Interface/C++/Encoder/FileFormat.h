// FileFormat.h
#pragma once

#include "EncodeParam.h"
#include "CodecFrame.h"


struct FILEFORMAT_ITEM;		// 文件格式信息
typedef void* HFILEFORMAT;	// 媒体文件句柄定义


// 获取文件格式信息
extern "C" const FILEFORMAT_ITEM* __stdcall GetFileFormats(); 
typedef const FILEFORMAT_ITEM* (__stdcall* GET_FILEFORMATS_API)();



// 根据dwFourCC值创建相应格式的文件，并写入文件头信息，返回值为文件句柄
// HFILEFORMAT __stdcall CreateMediaFile(const wchar_t* pFileName, const EncodeParam* pEncParam);
typedef HFILEFORMAT (__stdcall* CREATE_MEDIA_FILE_API)(const wchar_t* pFileName, const EncodeParam* pEncParam);

// 写入音频数据
// int __stdcall WriteAudioPacket(HFILEFORMAT hFile, const MEDIA_FRAME* pFrame, int* pPacketInfo);
typedef int (__stdcall* WRITE_AUDIO_PACKET_API)(HFILEFORMAT hFile, const MEDIA_FRAME* pFrame, int* pPacketInfo);

// 写入图像数据
// int __stdcall WriteVideoPacket(HFILEFORMAT hFile, const MEDIA_FRAME* pFrame);
typedef int (__stdcall* WRITE_VIDEO_PACKET_API)(HFILEFORMAT hFile, const MEDIA_FRAME* pFrame);

// 仅用于AAC音频编码器，在关闭文件前调用一次
// void __stdcall WriteVosData(HFILEFORMAT hFile, BYTE* pVosData, int nVosLen);
typedef void (__stdcall* WRITE_VOSDATA_API)(HFILEFORMAT hFile, BYTE* pVosData, int nVosLen);

// 仅用于H264视频编码器，关闭文件前调用一次
// void __stdcall WritezXPSData(HFILEFORMAT hFile, BYTE* pPPSData, int nPPSLen, BYTE* pSPSData, int nSPSLen);
typedef void (__stdcall* WRITE_XPSDATA_API)(HFILEFORMAT hFile, BYTE* pPPSData, int nPPSLen, BYTE* pSPSData, int nSpsLen);

// 更新文件头信息，关闭文件
//void __stdcall CloseMediaFile(HFILEFORMAT hFile, ENCODE_CALLBACK pCallBack, void* pUserObj);
typedef void (__stdcall* CLOSE_MEDIA_FILE_API)(HFILEFORMAT hFile, ENCODE_CALLBACK pCallBack, void* pUserObj);


struct ENCODER_MODE
{
	DWORD dwFourCC;
	int  bIsInline;
};

// FILE_FORMAT_FLAG
#define FFF_TIMESTAMPSUPPORTED	0x0001

struct FILEFORMAT_ITEM
{
	DWORD dwFourCC;
	wchar_t szExtName[10];
	wchar_t szFormatName[64];
	wchar_t szDescription[128];

	ENCODER_MODE* pAudioEncoders;
	ENCODER_MODE* pVideoEncoders;

	CREATE_MEDIA_FILE_API	fnCreateFile;
	WRITE_AUDIO_PACKET_API	fnWriteAudio;
	WRITE_VIDEO_PACKET_API	fnWriteVideo;
	CLOSE_MEDIA_FILE_API	fnCloseFile;

	WRITE_VOSDATA_API		fnWriteVosData;
	WRITE_XPSDATA_API		fnWriteXPSData;

	UINT					flag;
};
