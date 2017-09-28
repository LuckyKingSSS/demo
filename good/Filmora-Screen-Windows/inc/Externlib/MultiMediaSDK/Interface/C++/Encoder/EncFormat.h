// EncFormat.h: 定义编码器插件接口
#pragma once

#include "EncodeParam.h"
#include "CodecFrame.h"


struct ENCODER_ITEM;	// 编码器信息
typedef void* HENCODER;	// 编码器句柄定义

// 获取编码器信息
extern "C" const ENCODER_ITEM* __stdcall GetEncoders();
typedef const ENCODER_ITEM* (__stdcall* GET_ENCODERS_API)();



// 根据dwFourCC标志创建编码器，返回值为编码器句柄
// HENCODER __stdcall EncoderInit(DWORD dwFourCC, const EncodeParam* pEncParam);
typedef HENCODER (__stdcall* ENCODER_INIT_API)(DWORD dwFourCC, const EncodeParam* pEncParam);

// 编码，参数： 1.hEnc - 编码器句柄；  2.pFrameIn - 输入音频数据（无压缩PCM数据）或视频数据(YV12格式图像，限定为一帧)；  
//              3.ppFrameOut - 压缩后数据，由编码器管理内存；  
//				4.ppPacketInfo - 用于存放pFrameOut压缩数据的信息，仅用于音频
// 返回值： 压缩数据长度，单位 byte
// int __stdcall Encode(HENCODER hEnc, const MEDIA_FRAME* pFrameIn, MEDIA_FRAME** ppFrameOut, int** ppPacketInfo);
typedef int (__stdcall* ENCODE_API)(HENCODER hEnc, const MEDIA_FRAME* pFrameIn, MEDIA_FRAME** ppFrameOut, int** ppPacketInfo);

// 获取VosData数据块的指针和长度，仅用于AAC音频编码器
// void __stdcall GetVosData(HENCODER hEnc, BYTE** ppVosData, int* pVosLen);
typedef void (__stdcall* GET_VOSDATA_API)(HENCODER hEnc, BYTE** ppVosData, int* pVosLen);

// 获取PPSData, SPSData两个数据块的指针和长度，仅用于H264视频编码器
// void __stdcall GetXPSData(HENCODER hEnc, BYTE** ppPPSData, int* pPPSLen, BYTE** ppSPSData, int* pSPSLen);
typedef void (__stdcall* GET_XPSDATA_API)(HENCODER hEnc, BYTE** ppPPSData, int* pPPSLen, BYTE** ppSPSData, int* pSPSLen);

//为了解决残留在编码中的最后部分数据未能编码，询问编码器还需要多少数据才能编码
typedef int (__stdcall * GET_LAST_NEED_PCMDATA_NUM_API) (HENCODER hEnc);
// 销毁编码器
// void __stdcall DestroyEncoder(HENCODER hEnc);
typedef void (__stdcall* ENCODER_CLOSE_API)(HENCODER hEnc);

// 校正音频编码参数，从编码器支持的参数中获取最与传入值最相近的一组参数从传入结构体指针中返回
// int __stdcall ReviseAudioParam(AudioParam* pAudioParam);
typedef BOOL (__stdcall* REVISE_AUDIO_PARAM_API)(AudioParam* pAudioParam);

// 校正视频编码参数，从编码器支持的参数中获取最与传入值最相近的一组参数从传入结构体指针中返回
// int __stdcall ReviseVideoParam(VideoParam* pVideoParam);
typedef BOOL (__stdcall* REVISE_VIDEO_PARAM_API)(VideoParam* pVideoParam);


//
typedef BOOL (__stdcall* ENCODE_FIRSTEND_API)(HENCODER hEnc,EncodeParam* pEncParam);
typedef BOOL (__stdcall* SET_ENCODE_PARAMEX_API)(EncodeParamEx* pEncParamEx);


struct ENCODER_ITEM
{
	DWORD						dwFourCC;
	wchar_t						szName[64];
	wchar_t						szDescription[128];	
	EncMediaType				Type;			// Audio: 0, Video: 1;

	ENCODER_INIT_API			fnInit;
	ENCODE_API					fnEncode;
	GET_LAST_NEED_PCMDATA_NUM_API	fnGetLastNeedPCMDataNum;
	ENCODER_CLOSE_API			fnClose;

	GET_VOSDATA_API				fnGetVos;
	GET_XPSDATA_API				fnGetXPS;

	REVISE_AUDIO_PARAM_API		fnReviseAudio;
	REVISE_VIDEO_PARAM_API		fnReviseVideo;

	ENCODE_FIRSTEND_API         fnEncodefirstEnd;
	SET_ENCODE_PARAMEX_API      fnSetEncodeParamEx; 
};


