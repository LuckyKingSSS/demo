#pragma once
#include <EncodeParam.h>
#include "COMMacro.h"
#include "ICaptureSource.h"

/*!
* \class ICaptureEngine
* \brief 获取媒体文件信息接口 
*/
// {85B03E98-8309-4eda-B1DE-E602356164E5}
DEFINE_GUID(IID_ICaptureEngine, 
			0x85b03e98, 0x8309, 0x4eda, 0xb1, 0xde, 0xe6, 0x2, 0x35, 0x61, 0x64, 0xe5);
EXTERN_C const IID IID_ICaptureEngine;
MACRO_MIDL_INTERFACE(ICaptureEngine, "85B03E98-8309-4eda-B1DE-E602356164E5")
: public IUnknown
{
public:
	/*! \fn virtual STDMETHODIMP Init(void)
	* \brief 录制引擎初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP Init() = 0;

	/*! \fn virtual STDMETHODIMP Start(void)
	* \brief 开始录制
	* \retval 获取结果
	*/
	virtual STDMETHODIMP Start() = 0;

	/*! \fn virtual STDMETHODIMP Stop(void)
	* \brief 停止录制
	* \retval 获取结果
	*/
	virtual STDMETHODIMP Stop() = 0;

	/*! \fn virtual STDMETHODIMP SetScreenCapturer(IUnknown* pVideoCaptureSource, BOOL bCaptureCursor)
	* \brief 设置视频流接口
	* \param  pVideoCaptureSource [in / out] 视频流接口
	* \param  bCaptureCursor [in / out] 是否录制鼠标
	* \retval 获取结果
	*/
	virtual STDMETHODIMP SetScreenCapturer(IUnknown* pVideoCaptureSource, BOOL bCaptureCursor) = 0;

	/*! \fn virtual STDMETHODIMP SetAudioCapturer(IUnknown* pVideoCaptureSource, BOOL bCaptureCursor)
	* \brief 设置音频流接口
	* \param  pAudioCaptureSource [in / out] 音频流接口
	* \retval 获取结果
	*/
	virtual STDMETHODIMP SetAudioCapturer(IUnknown* pAudioCaptureSource) = 0;

	/*! \fn virtual STDMETHODIMP SetEncodeParam(EncodeParam* pEncParam)
	* \brief 设置编码参数
	* \param  pEncParam [in / out] 编码参数
	* \retval 获取结果
	*/
	virtual STDMETHODIMP SetEncodeParam(EncodeParam* pEncParam) = 0;

	/*! \fn virtual STDMETHODIMP SetOutputFile(wchar_t *pszFilePath)
	* \brief 设置输出文件路径
	* \param  pszFilePath [in / out] 输出文件路径
	* \retval 获取结果
	*/
	virtual STDMETHODIMP SetOutputFile(wchar_t *pszFilePath) = 0;

	/*! \fn virtual STDMETHODIMP SetCropRect(RECT rect)
	* \brief 设置裁剪区域
	* \param  rect [in / out] 裁剪区域
	* \retval 获取结果
	*/
	virtual STDMETHODIMP SetCropRect(RECT rect) = 0;

	/*! \fn virtual STDMETHODIMP SetCaptureCallback(CaptureCallback captureCallback, void* pUserData)
	* \brief 设置录制回调
	* \param  captureCallback [in / out] 回调函数指针
	* \param  pUserData [in / out] 用户数据指针
	* \retval 获取结果
	*/
	virtual STDMETHODIMP SetCaptureCallback(CaptureCallback captureCallback, void* pUserData) = 0;

	/*! \fn virtual STDMETHODIMP AddClickAnimationItem(HDIBIMAGE hAnimationItem, int duration)
	* \brief 设置鼠标动画图像的一帧
	* \param  hAnimationItem [in / out] 鼠标动画图像帧
	* \param  duration [in / out] 鼠标动画图像帧持续时长
	* \retval 获取结果
	*/
	virtual STDMETHODIMP AddClickAnimationItem(HDIBIMAGE hAnimationItem, int duration) = 0;

	/*! \fn virtual STDMETHODIMP AddClickVoiceFile(wchar_t* pszNoiseFilePath)
	* \brief 设置鼠标单击声音
	* \param  pszNoiseFilePath [in / out] 鼠标单击声音文件路径
	* \retval 获取结果
	*/
	virtual STDMETHODIMP AddClickVoiceFile(wchar_t* pszNoiseFilePath) = 0;

	/*! \fn virtual STDMETHODIMP CursorClick(int xPos, int yPos)
	* \brief 响应鼠标单击事件
	* \param  xPos [in / out] 鼠标单击事件x坐标
	* \param  yPos [in / out] 鼠标单击事件y坐标
	* \retval 获取结果
	*/
	virtual STDMETHODIMP CursorClick(int xPos, int yPos) = 0;
};

// {F7AFB65B-D0EF-4778-9F0B-E6236DE49591}
//定义组件对象CaptureEngine
MIDL_DEFINE_GUID(CLSID, CLSID_CCaptureEngine, 0xf7afb65b, 0xd0ef, 0x4778, 0x9f, 0xb, 0xe6, 0x23, 0x6d, 0xe4, 0x95, 0x91);

EXTERN_C const CLSID CLSID_CCaptureEngine;

#define ENC_SET_COMMON \
	AV_STREAM_TYPE	uStreamType;	\
	union							\
	{								\
		VideoParam video;		    \
		AudioParam audio;		    \
		int reserved[RESERVEDSIZE]; \
	};								\

typedef struct _StreamEncodeSetting
{
	ENC_SET_COMMON
}StreamEncodeSetting;

typedef struct _VideoEffectSetting
{
	RECT  cropRect;
	UINT  uScreenID;
}VideoEffectSetting;

typedef struct _VideoUnRegisterSetting
{
	BYTE* pPngData;
	int	  nPngDataSize;
}VideoUnRegisterSetting;

typedef struct _CaptureStreamSetting
{
	ENC_SET_COMMON
	union
	{
		VideoEffectSetting vidEffect;
		VideoUnRegisterSetting vidUnreg;
		int effect_reserved[RESERVEDSIZE]; //保留字段 为了分配固定大小结构
	};
}CaptureStreamSetting;

enum EncoderExtadataType
{
	eetNONE = 0,
	eetSPSPPS = 1,
	eetVOS = 2,
};

typedef struct _EncoderExtradata
{
	EncoderExtadataType  uExtradataType;
	enum NLEScreenRecordingStreamType eRecordType;
	union
	{
		struct {
			BYTE *pVOSData;
			int nVOSSize;
		};
		struct {
			BYTE *pPPS;
			int nPPSSize;
			BYTE *pSPS;
			int nSPSSize;
		};
		int reserved[RESERVEDSIZE];
	};
}EncoderExtradata;

/*!
* \class IESStreamPropertySetting
* \brief 获取流属性 
*/
// {1F933277-16A6-4acd-A611-5CD23A8C208D}
DEFINE_GUID(IID_IESStreamPropertySetting, 
			0x1f933277, 0x16a6, 0x4acd, 0xa6, 0x11, 0x5c, 0xd2, 0x3a, 0x8c, 0x20, 0x8d);
EXTERN_C const IID IID_IESStreamPropertySetting;
MACRO_MIDL_INTERFACE(IESStreamPropertySetting, "1F933277-16A6-4acd-A611-5CD23A8C208D")
: public IUnknown
{
public:
	virtual STDMETHODIMP					SetProperty(enum CaptureStreamPropertyEnum propertyEnum, CaptureStreamProperty* pProperty) = 0;
};


/*!
* \class IID_IESStreamWriter
* \brief 录制流参数设置接口 
*/
// {7524E3D0-FC36-4b88-B637-0394FDEDB84A}
DEFINE_GUID(IID_ICaptureESStream, 
			0x7524e3d0, 0xfc36, 0x4b88, 0xb6, 0x37, 0x3, 0x94, 0xfd, 0xed, 0xb8, 0x4a);
EXTERN_C const IID IID_ICaptureESStream;
MACRO_MIDL_INTERFACE(ICaptureESStream, "7524E3D0-FC36-4b88-B637-0394FDEDB84A")
: public IUnknown
{
public:
	/*! \fn virtual STDMETHODIMP SetInputStream(ICaptureStream *pInputStream)
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					WriteFrame(MEDIA_FRAME *pFrame, int* pInfo) = 0;

	/*! \fn virtual STDMETHODIMP SetInputStream(ICaptureStream *pInputStream)
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					SetExtradata(EncoderExtradata *pExtradata) = 0;

	/*! \fn virtual STDMETHODIMP GetExtradata(ICaptureStream *pInputStream)
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					GetExtradata(EncoderExtradata **ppExtradata) = 0;

};

/*!
* \class IID_ICaptureMuxerPlugin
* \brief 录制流参数设置接口 
*/
// {E2C4FDD4-CC4C-49e5-A48B-E7D6C0F7E923}
DEFINE_GUID(IID_ICaptureMuxerPlugin,
			0xe2c4fdd4, 0xcc4c, 0x49e5, 0xa4, 0x8b, 0xe7, 0xd6, 0xc0, 0xf7, 0xe9, 0x23);
EXTERN_C const IID IID_ICaptureMuxerPlugin;
MACRO_MIDL_INTERFACE(ICaptureMuxerPlugin, "E2C4FDD4-CC4C-49e5-A48B-E7D6C0F7E923")
: public IUnknown
{
public:
	/*! \fn virtual STDMETHODIMP Init()
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					Init() = 0;

	/*! \fn virtual STDMETHODIMP AddStream(StreamEncodeSetting *pEncodeSetting, int nStreamID)
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					AddStreamSetting(StreamEncodeSetting *pStreamEncodeSetting) = 0;

	/*! \fn virtual STDMETHODIMP SetOutputFormat(DWORD dwFileFormat)
	* \brief 流初始化
	* \retval 获取结果
	*/

	virtual STDMETHODIMP					SetOutputFormat(DWORD dwFormat) = 0;

	/*! \fn virtual STDMETHODIMP SetOutputFileFormat(DWORD dwFileFormat, wchar_t *pszOutputFile)
	* \brief 流初始化
	* \retval 获取结果
	*/

	virtual STDMETHODIMP					SetOutputFile(wchar_t *pszOutputFile) = 0;

	/*! \fn virtual STDMETHODIMP ConfigSetting()
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					ConfigSetting() = 0;

};

/*!
* \class IID_IEncodeFilter
* \brief 录制流参数设置接口 
*/
// {40B8BE2F-E9F4-4ea4-940F-C5ED6C9EE0FE}
DEFINE_GUID(IID_IEncodeFilter, 
			0x40b8be2f, 0xe9f4, 0x4ea4, 0x94, 0xf, 0xc5, 0xed, 0x6c, 0x9e, 0xe0, 0xfe);
EXTERN_C const IID IID_IEncodeFilter;
MACRO_MIDL_INTERFACE(IEncodeFilter, "40B8BE2F-E9F4-4ea4-940F-C5ED6C9EE0FE")
: public IUnknown
{
public:
	/*! \fn virtual STDMETHODIMP Init()
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					Init() = 0;

	/*! \fn virtual STDMETHODIMP Start()
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					Start() = 0;

	/*! \fn virtual STDMETHODIMP Stop()
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					Close() = 0;

	/*! \fn virtual STDMETHODIMP SetInputStream(ICaptureStream *pInputStream)
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					SetInputStream(ICaptureStreamEx *pInputStream) = 0;

	/*! \fn virtual STDMETHODIMP GetInputStream(ICaptureStream *pInputStream)
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					GetInputStream(ICaptureStreamEx **ppInputStream) = 0;

	/*! \fn virtual STDMETHODIMP			SetOutputStream(ICaptureStream **ppOutputStream)
	* \brief 开始获取数据
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					SetOutputStream(ICaptureESStream *pESStream) = 0;

	/*! \fn virtual STDMETHODIMP			SetEncodeSetting(StreamEncodeSetting *pEncodeSetting)
	* \brief 锁定一帧数据
	* \retval 返回帧数据
	*/
	virtual STDMETHODIMP					SetEncodeSetting(StreamEncodeSetting *pStreamEncodeSetting) = 0;

	/*! \fn virtual STDMETHODIMP			GetEncodeSetting(StreamEncodeSetting **pEncodeSetting)
	* \brief 锁定一帧数据
	* \retval 返回帧数据
	*/
	virtual STDMETHODIMP					GetEncodeSetting(StreamEncodeSetting *pStreamEncodeSetting) = 0;

	/*! \fn virtual STDMETHODIMP			SetFileFormat(DWORD dwFileFormat)
	* \brief 锁定一帧数据
	* \retval 返回帧数据
	*/
	virtual STDMETHODIMP					SetFileFormat(DWORD dwFileFormat) = 0;

	/*! \fn virtual STDMETHODIMP			GetFileFormat(DWORD dwFileFormat)
	* \brief 锁定一帧数据
	* \retval 返回帧数据
	*/
	virtual STDMETHODIMP					GetFileFormat(DWORD *pDWFileFormat) = 0;

	/*! \fn virtual STDMETHODIMP GetExtradata(EncoderExtradata **ppExtradata)
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					GetExtradata(EncoderExtradata **ppExtradata) = 0;

	/*! \fn virtual STDMETHODIMP Pause()
	* \brief 暂停
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					Pause() = 0;

	/*! \fn virtual STDMETHODIMP Resume()
	* \brief 恢复
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					Resume() = 0;


	/*! \fn virtual STDMETHODIMP SetRecordType()
	* \brief 设置录制源类型
	* \param  eRecordType [in] 源类型
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					SetRecordType(enum NLEScreenRecordingStreamType eRecordType) = 0;


	/*! \fn virtual STDMETHODIMP GetRecordType()
	* \brief 获取录制源类型
	* \param  pRecordType [out] 源类型
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					GetRecordType(enum NLEScreenRecordingStreamType* pRecordType) = 0;

	/*! \fn virtual STDMETHODIMP Abort()
	* \brief 中断录制
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					Abort() = 0;
};

/*!
* \class ICaptureMultiplexer
* \brief 录制流参数设置接口 
*/
// {FF2BD56F-FEDF-49c6-AE3D-146054157C5F}
DEFINE_GUID(IID_ICaptureMultiplexer,
			0xff2bd56f, 0xfedf, 0x49c6, 0xae, 0x3d, 0x14, 0x60, 0x54, 0x15, 0x7c, 0x5f);
EXTERN_C const IID IID_ICaptureMultiplexer;
MACRO_MIDL_INTERFACE(ICaptureMultiplexer, "FF2BD56F-FEDF-49c6-AE3D-146054157C5F")
: public IUnknown
{
public:

	/*! \fn virtual STDMETHODIMP NewESStreammWriter(int nStreamID, IESStreamWriter **pESStreamWriter)
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					NewESStream(StreamEncodeSetting *pStreamEncodeSetting, EncoderExtradata *pExtraData, ICaptureESStream **ppESStream) = 0;

	/*! \fn virtual STDMETHODIMP NewESStreammWriter(int nStreamID, IESStreamWriter **pESStreamWriter)
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					GetFileFormat(DWORD *pFileFormat) = 0;

	/*! \fn virtual STDMETHODIMP Open()
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					Open() = 0;

	/*! \fn virtual STDMETHODIMP Close()
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					Close() = 0;


};

/*!
* \class IID_ICaptureMuxerMgr
* \brief 录制流参数设置接口 
*/
// {C737C439-B481-4753-A8BD-14EEABB437B5}
DEFINE_GUID(IID_ILocalFileMuxerMgr,
			0xc737c439, 0xb481, 0x4753, 0xa8, 0xbd, 0x14, 0xee, 0xab, 0xb4, 0x37, 0xb5);
EXTERN_C const IID IID_ILocalFileMuxerMgr;
MACRO_MIDL_INTERFACE(ILocalFileMuxerMgr, "C737C439-B481-4753-A8BD-14EEABB437B5")
: public IUnknown
{
public:
	/*! \fn virtual STDMETHODIMP Init()
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					Init() = 0;

	/*! \fn virtual STDMETHODIMP NewStream(ICaptureStream *pInputStream)
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					AddStreamSetting(StreamEncodeSetting *pStreamEncodeSetting) = 0;

	/*! \fn virtual STDMETHODIMP SetOutputFileFormat(DWORD dwFileFormat, wchar_t *pszOutputFile)
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					SetOutputFile(DWORD dwFileFormat, wchar_t *pszOutputFile) = 0;

	/*! \fn virtual STDMETHODIMP GetMultiplexer(ICaptureMultiplexer **ppCaptureMultiplexer)
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					GetMultiplexer(ICaptureMultiplexer **ppCaptureMultiplexer) = 0;
};


// {F150A0CE-A353-450e-AE8A-D62065D36B95}
//定义组件对象DecMgr
MIDL_DEFINE_GUID(CLSID, CLSID_CLocalFileMuxerMgr, 0xf150a0ce, 0xa353, 0x450e, 0xae, 0x8a, 0xd6, 0x20, 0x65, 0xd3, 0x6b, 0x95);

EXTERN_C const CLSID CLSID_CLocalFileMuxerMgr;

/*!
* \class ICaptureEngineEx
* \brief 获取媒体文件信息接口 
*/
// {0E486A5F-7120-4ac7-BF1F-2BAF30D21146}
DEFINE_GUID(IID_ICaptureEngineEx, 
			0xe486a5f, 0x7120, 0x4ac7, 0xbf, 0x1f, 0x2b, 0xaf, 0x30, 0xd2, 0x11, 0x46);
EXTERN_C const IID IID_ICaptureEngineEx;
MACRO_MIDL_INTERFACE(ICaptureEngineEx, "0E486A5F-7120-4ac7-BF1F-2BAF30D21146")
: public IUnknown
{
public:
	/*! \fn virtual STDMETHODIMP Init(void)
	* \brief 录制引擎初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP Init() = 0;

	/*! \fn virtual STDMETHODIMP Start(void)
	* \brief 开始录制
	* \retval 获取结果
	*/
	virtual STDMETHODIMP Start() = 0;

	/*! \fn virtual STDMETHODIMP Stop(void)
	* \brief 停止录制
	* \retval 获取结果
	*/
	virtual STDMETHODIMP Stop() = 0;

	/*! \fn virtual STDMETHODIMP AddCaptureStream(IUnknown* pVideoCaptureSource)
	* \brief 设置视频流接口
	* \param  pVideoCaptureSource [in / out] 视频流接口
	* \param  bCaptureCursor [in / out] 是否录制鼠标
	* \retval 获取结果
	*/
	virtual STDMETHODIMP AddCaptureStream(IUnknown* pCaptureStream, CaptureStreamSetting *pCaptureStreamSetting) = 0;

	/*! \fn virtual STDMETHODIMP SetCaptureMultiplexer(CaptureCallback captureCallback, void* pUserData)
	* \brief 设置录制回调
	* \param  captureCallback [in / out] 回调函数指针
	* \param  pUserData [in / out] 用户数据指针
	* \retval 获取结果
	*/

	virtual STDMETHODIMP SetCaptureMultiplexer(ICaptureMultiplexer* pCaptureMultiplexer) = 0;

	/*! \fn virtual STDMETHODIMP SetCaptureCallback(CaptureCallback captureCallback, void* pUserData)
	* \brief 设置录制回调
	* \param  captureCallback [in / out] 回调函数指针
	* \param  pUserData [in / out] 用户数据指针
	* \retval 获取结果
	*/

	virtual STDMETHODIMP SetOutputFile(wchar_t* pszFileName, DWORD dwFileFormat) = 0;

	/*! \fn virtual STDMETHODIMP SetCaptureCallback(CaptureCallback captureCallback, void* pUserData)
	* \brief 设置录制回调
	* \param  captureCallback [in / out] 回调函数指针
	* \param  pUserData [in / out] 用户数据指针
	* \retval 获取结果
	*/
	virtual STDMETHODIMP SetCaptureCallback(CaptureCallback captureCallback, void* pUserData) = 0;

	/*! \fn virtual STDMETHODIMP Pause()
	* \brief 暂停
	* \retval 获取结果
	*/
	virtual STDMETHODIMP Pause() = 0;

	/*! \fn virtual STDMETHODIMP Resume()
	* \brief 恢复
	* \retval 获取结果
	*/
	virtual STDMETHODIMP Resume() = 0;

};

// {A016905B-2C21-4704-95FD-391C8AB26BB3}
//定义组件对象DecMgr
MIDL_DEFINE_GUID(CLSID, CLSID_CCaptureEngineEx, 0xa016905b, 0x2c21, 0x4704, 0x95, 0xfd, 0x39, 0x1c, 0x8a, 0xb2, 0x6b, 0xb3);

EXTERN_C const CLSID CLSID_CCaptureEngineEx;

#ifdef __cplusplus
extern "C" {
#endif
void CaptureInit(int isAppStore);
#ifdef __cplusplus
};
#endif