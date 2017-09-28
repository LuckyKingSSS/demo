#pragma once

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h> 
#include "INLEService.h"

//{8C0E7949-611F-8AAA-B0D3-D0D2221202F6}
DEFINE_GUID(CLSID_NLECodec, 0x8C0E7949, 0x611F, 0x8AAA, 0xB0, 0xD3, 0xD0, 0xD2, 0x22, 0x12, 0x02, 0xF6);                   
//{18CE6ED3-05F1-1397-BC75-27A20D703A9E}
DEFINE_GUID(IID_INLECodec, 0x18CE6ED3, 0x05F1, 0x1397, 0xBC, 0x75, 0x27, 0xA2, 0x0D, 0x70, 0x3A, 0x9E);                         

class INLEStream;


class INLECodec :public IUnknown
{
public:    
	
	/**
	@brief 初始化
	@param pFileName 文件名，完整路径

	@return 标准错误码
	*/
	virtual HRESULT STDMETHODCALLTYPE Init(LPCWSTR pFileName) = 0;

	/**
	@brief 获取文件名
	@param pFileName[out] 文件名，完整路径
	@param nMaxFileNameLength[in] 文件名的最大长度
	*/
	virtual HRESULT STDMETHODCALLTYPE GetFileName(LPWSTR pOutFileName, int nMaxFileNameLength) = 0;

	/**
	@brief 获取媒体信息，属性名为 NLEKey::Codec::xxx
	@ppProperties [out] 媒体信息，用属性表保存

	@return 标准错误码
	*/
	virtual HRESULT STDMETHODCALLTYPE GetMediaInfo(INLEProperties **ppProperties) = 0;

	/**
	@brief 获取音频流，视频流
	@param ppAudioStream [out] 保存音频流
	@param ppVideoStream [out] 保存视频流

	@return 标准错误码
	*/
	virtual HRESULT STDMETHODCALLTYPE GetStreams(INLEStream **ppAudioStream, INLEStream **ppVideoStream) = 0;


	/**
	 *@brief  	EnableStream 是否使用Stream
	 *@param	streamType Stream类型,NLE_MEDIA_TYPE_VIDEO or NLE_MEDIA_TYPE_AUDIO
	 *@param	bEnable TRUE：使用, FALSE:不使用
	 *@return 	HRESULT 标准返回
	 *@see 		GetStreams
	 *@note		当流为不使用状态时，GetStreams就获取不到
	 */
	virtual HRESULT STDMETHODCALLTYPE EnableStream(const NLEMediaType streamType, BOOL bEnable = TRUE) = 0;



	virtual int STDMETHODCALLTYPE GetStreamCount(void) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE GetStreamInfo(int nStreamIndex, INLEProperties **ppProp) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetStream(int nStreamIndex, INLEStream **ppStream) = 0;
};

