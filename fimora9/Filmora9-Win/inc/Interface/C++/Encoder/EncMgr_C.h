// 功能： 实现媒体文件的创建及编码

#pragma once

#include "EncodeParam.h"
#include "WSAudioInfo.h"
#include "WSVideoInfo.h"
#include "CodecFrame.h"
//#include "EncMgr.h"
#ifdef __cplusplus
extern "C" {
#endif


#ifdef USE_MUTEX_LOCK
	#define _USE_SINGLE_LOCK	// 使用线程锁
#endif

typedef void* HMEDIAFILE; // 媒体文件句柄定义

int __stdcall FCInitPlugins();		// 应用程序启动时调用
void __stdcall FCUninitPlugins();	// 应用程序退出时调用
FileFormatInfo*  __stdcall FCGetFileFormatInfos(); // 获取已加载支持的所有文件格式，需在FCInitPlugins()调用成功后才可调用
//IFileFormatInfos*  __stdcall FCGetFileFormatInfosImp();

// 创建媒体文件 目前pVidInfo->dwFourCC = '21VY' pVidInfo->nBitsDepth = 12的输入数据 
HMEDIAFILE __stdcall FCCreateMediaFile(const wchar_t* pFileName, const EncodeParam* pEncParam, 
				  const WSVideoInfo* pVidInfo, const WSAudioInfo* pAudInfo,const EncodeParamEx* pEncParamEx=NULL);


// 关闭媒体文件
void __stdcall FCCloseMediaFile(HMEDIAFILE hFileEnc, ENCODE_CALLBACK pCallBack = 0, void* pUserObj = 0);

// 写入音频数据（无压缩PCM数据）
int __stdcall FCWriteAudioData(HMEDIAFILE hFileEnc, const MEDIA_FRAME* pFrame);

// 写入一帧图像数据 目前只支持YV12图像
int __stdcall FCWriteVideoFrame(HMEDIAFILE hFileEnc, const MEDIA_FRAME* pFrame);

// 通知二次编码的第一次数据传输结束
int __stdcall FCEncodeFirstEnd(HMEDIAFILE hFileEnc,EncodeParam* pEncParam);

//设置扩展编码参数
//int __stdcall FCSetEncParamEx(HMEDIAFILE hFileEnc,EncodeParamEx* pEncParamEx);

int __stdcall FCGetEncodeParam(HMEDIAFILE hFileEnc,EncodeParam* pEncParam) ;

#ifdef __cplusplus
}
#endif

