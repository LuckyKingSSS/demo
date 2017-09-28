#pragma once

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h> 
#include "NLEComPtr.h"

// {FB897F04-8D26-4433-BD8A-8666386F48F9}
DEFINE_GUID(CLSID_NLEMediaEncoder,
	0xfb897f04, 0x8d26, 0x4433, 0xbd, 0x8a, 0x86, 0x66, 0x38, 0x6f, 0x48, 0xf9);
// {59EC05A8-958C-4066-A2E2-EAFF25DDD5D0}
DEFINE_GUID(IID_INLEMediaEncoder,
	0x59ec05a8, 0x958c, 0x4066, 0xa2, 0xe2, 0xea, 0xff, 0x25, 0xdd, 0xd5, 0xd0);

class INLETimelineBase;
class INLEFrame;
typedef enum NLEEncoderCallbackType
{
	NLE_ENCODER_CALLBACK_TYPE_PROGRESS = 0, //进度
	NLE_ENCODER_CALLBACK_TYPE_STATE = 1, //编码器状态
}NLEEncoderCallbackType;

typedef enum NLEEncoderState
{
	NLE_ENCODER_STATE_NORMAL_FINISH = 0, //正常结束
	NLE_ENCODER_STATE_STOPPED_BY_USER = 1, //用户终止
	NLE_ENCODER_STATE_STOPPED_BY_CALLBACK = 2, //通过回调函数终止（回调函数返回非S_OK的值）
	NLE_ENCODER_STATE_OCCUR_ERROR = 3, //编码器遇到错误

	// 用于指示多段落并行编码状态
	NLE_ENCODER_STATE_PROGRESS = 4, //正在进行

	NLE_ENCODER_STATE_SLOWWAIT = 5, //处理太慢{给用户决定是否继续等待}
}NLEEncoderState;

#define ENC_PROGRESS_MAX	10000
typedef struct NLEEncoderCallbackData
{
	int nProgress; //进度，值为0 - 10000
	NLEEncoderState state;
	NLEComPtr<INLEFrame> image; // 转换过程图像
}NLEEncoderCallbackData;

/**
编码器的回调函数，当回调函数返回值不为S_OK时终止编码
*/
typedef HRESULT(*NLEEncoderCallback)(NLEEncoderCallbackType callbackType, NLEEncoderCallbackData data, void *pContext);

/**
@brief 编码器，将timeline编码输出到文件

@mark 输出格式的参数和编码器的参数是通过INLEMediaEncoder的属性来设置的，需要先查询到INLEProperties接口，
可设置如下属性：
hr = exportProp->SetInt(NLEKey::Codec::kFourCC, pInfo->fileFourCC); CHECK(hr); //文件格式
hr = exportProp->SetInt(NLEKey::Codec::Video::kFourCC, pInfo->videoFourCC); CHECK(hr); //视频编码器
hr = exportProp->SetInt(NLEKey::Codec::kVideo_Disable, 0); CHECK(hr); //是否关闭视频流
hr = exportProp->SetInt(NLEKey::Codec::Video::kWidth, pInfo->renderSize.Width); CHECK(hr); //宽度
hr = exportProp->SetInt(NLEKey::Codec::Video::kHeight, pInfo->renderSize.Height); CHECK(hr); //高度
hr = exportProp->SetDouble(NLEKey::Codec::Video::kFrameRate, pInfo->dFramerate); CHECK(hr); //帧率
hr = exportProp->SetInt(NLEKey::Codec::Video::kBitrate, pInfo->nVideoBitrate); CHECK(hr); //视频比特率

hr = exportProp->SetInt(NLEKey::Codec::kAudio_Disable, 0); CHECK(hr); //是否关闭音频流
hr = exportProp->SetInt(NLEKey::Codec::Audio::kFourCC, pInfo->audioFourCC); CHECK(hr); //音频编码器
exportProp->SetInt(NLEKey::Codec::Audio::kSampleRate, pInfo->nSamplerate); //采样率
exportProp->SetInt(NLEKey::Codec::Audio::kChannels, pInfo->nChannels); //声道数
exportProp->SetInt(NLEKey::Codec::Audio::kBitsPerSample, pInfo->nBitsPerSample); //采样位数
exportProp->SetInt(NLEKey::Codec::Audio::kBitrate, pInfo->nAudioBitrate); //音频比特率
exportProp->SetInt(NLEKey::Codec::Audio::kIsVBR, 0); //是否为可变比特率

详细说明见： NLEKey.h中的 NLEKey::Codec 定义
*/

class INLEMediaEncoder :public IUnknown
{
public:

	/**
	@brief 设置编码输入源

	@param [in]pTimeline 输入源，可通过pTimeline获取音频，视频的render，从而获取所需要的音频，视频流,该timeline必须为一个复合timeline
	@param [in]callback 设置给编码器的回调函数，用于显示编码状态和编码进度
	@param [in]pContext 回调函数使用的私有数据

	@return 标准错误码
	@remark 编码处理过程中可能会修改pTimeline的属性，影响timeline的行为，建议用户克隆一个新的timeline来作为输入源
	*/
	virtual HRESULT STDMETHODCALLTYPE SetInput(NLEComPtr<INLETimelineBase> pTimeline, NLEEncoderCallback callback = NULL, void *pContext = NULL) = 0;

	/**
	@brief 设置编码输出文件名
		
	@param [in]pOutputPath 输出文件的完整路径

	@return 标准错误码
	*/
	virtual HRESULT STDMETHODCALLTYPE SetOutputFile(const BSTR pOutputPath) = 0;

	/**
	@brief 开始编码

	@return 标准错误码
	*/
	virtual HRESULT STDMETHODCALLTYPE Start(void) = 0;
	
	/**
	@brief 暂停编码

	@return 标准错误码
	*/
	virtual HRESULT STDMETHODCALLTYPE Pause(void) = 0;
	
	/**
	@brief 继续编码

	@return 标准错误码
	*/
	virtual HRESULT STDMETHODCALLTYPE Resume(void) = 0;
	
	/**
	@brief 停止编码

	@return 标准错误码
	*/
	virtual HRESULT STDMETHODCALLTYPE Stop(void) = 0;

	/**
	@brief 获取文件预估空间占用
	@param pSize 返回预估的输出文件大小，单位是字节
	*/
	virtual HRESULT STDMETHODCALLTYPE GetEstimateDiskSpace(NLElonglong *pSize) = 0;
};                                                                              

// {C47A3CB7-11F7-465A-A53B-06A975D6A42C}
DEFINE_GUID(CLSID_NLEMediaEncoderEx,
	0xc47a3cb7, 0x11f7, 0x465a, 0xa5, 0x3b, 0x6, 0xa9, 0x75, 0xd6, 0xa4, 0x2c);

// {AF599763-F529-4CDA-B24D-8F5A908D1910}
DEFINE_GUID(IID_INLEMediaEncoderEx,
	0xaf599763, 0xf529, 0x4cda, 0xb2, 0x4d, 0x8f, 0x5a, 0x90, 0x8d, 0x19, 0x10);

typedef struct NLEEncoderCallbackExData
{
	int nParagIdx; // 段落编号。-1 - 表示总体进度；[0, nParagCnt -1] - 表示段落进度
	int nProgress;
	NLEEncoderState state;
}NLEEncoderCallbackExData;

/**
编码器的回调函数，当回调函数返回值不为S_OK时终止编码
*/
typedef HRESULT(*NLEEncoderCallbackEx)(NLEEncoderCallbackType callbackType, NLEEncoderCallbackExData data, void *pContext);

class INLEMediaEncoderEx : public IUnknown
{
public:
	/**
	@brief 设置编码输入源

	@param [in]pTimeline 输入源，可通过pTimeline获取音频，视频的render，从而获取所需要的音频，视频流
	@param [in]nParagCnt 需要并行的段落个数
	@param [in]callback 设置给编码器的回调函数，用于显示编码状态和编码进度
	@param [in]pContext 回调函数使用的私有数据

	@return 标准错误码
	@remark 编码处理过程中可能会修改pTimeline的属性，影响timeline的行为，建议用户克隆一个新的timeline来作为输入源
	*/
	virtual HRESULT STDMETHODCALLTYPE SetInput(NLEComPtr<INLETimelineBase> pTimeline, int nParagCnt, NLEEncoderCallbackEx callback = NULL, void *pContext = NULL) = 0;

	/**
	@brief 设置编码输出文件名

	@param [in]pOutputPath 输出文件的完整路径

	@return 标准错误码
	*/
	virtual HRESULT STDMETHODCALLTYPE SetOutputFile(const BSTR pOutputPath) = 0;

	/**
	@brief 开始编码

	@return 标准错误码
	*/
	virtual HRESULT STDMETHODCALLTYPE Start(void) = 0;

	/**
	@brief 暂停编码

	@return 标准错误码
	*/
	virtual HRESULT STDMETHODCALLTYPE Pause(void) = 0;

	/**
	@brief 继续编码

	@return 标准错误码
	*/
	virtual HRESULT STDMETHODCALLTYPE Resume(void) = 0;

	/**
	@brief 停止编码

	@return 标准错误码
	*/
	virtual HRESULT STDMETHODCALLTYPE Stop(void) = 0;
};
