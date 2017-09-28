
/*! \file CommonInterface.h
* \brief							公共的一些接口文件，很多模块都会用到的
* Detailed
* IVideoInput，IAudioInput，IVideoOutput，IAudioOutput接口用于设置视频流，音频流接口IMediaStream的输入以及获取输出。
* 除IVideoComposition接口外，其它接口都可以通过QueryInterface方法，查询这些接口。若某接口支持音频的处理，那么可以查询出IAudioInput接口，
* 该接口的SetAudioInput方法，用于设置音频流接口IMediaStream的输入， 对应的可以查询出IAudioOutput接口，该接口的GetAudioOutput方法，
* 用于获取音频流接口IMediaStream的输出。若支持视频数据处理，那么可以查询出IVideoInput接口，该接口的SetVideoInput的方法，
* 用于设置视频流接口的输入。对应的可以查询出IVideoOutput接口，该接口提供了GetVideoOutput方法，用于获取视频流接口IMediaStream的输出
*/


#pragma once
#include <scombase.h>
#include "COMMacro.h"
#include "FileInfo.h"
#include "CodecFrame.h"

/*!
* \class IStreamInfo
* \brief 获取流信息 
*/

// {58FE100C-DCD4-49db-9930-326174CC0ECC}
DEFINE_GUID(IID_IStreamInfo, 
			0x58fe100c, 0xdcd4, 0x49db, 0x99, 0x30, 0x32, 0x61, 0x74, 0xcc, 0xe, 0xcc);
EXTERN_C const IID IID_IID_IStreamInfo;
MACRO_MIDL_INTERFACE(IStreamInfo, "58FE100C-DCD4-49db-9930-326174CC0ECC")
: public IUnknown
{
public:

	/*! \fn virtual STDMETHODIMP_(AV_STREAM_TYPE) GetType(void)
	* \brief 获取流类型
	* \retval 流类型
	* \see AV_STREAM_TYPE
	*/
	virtual STDMETHODIMP_(AV_STREAM_TYPE) GetType(void) = 0;

	/*! \fn virtual STDMETHODIMP_(UINT) GetID(void)
	* \brief 获取流ID
	* \retval 流ID
	*/
	virtual STDMETHODIMP_(UINT) GetID(void) = 0;

	/*! \fn virtual STDMETHODIMP_(UINT) GetFourCC(void)
	* \brief 获取流FourCC
	* \retval 流FourCC
	*/
	virtual STDMETHODIMP_(UINT) GetFourCC(void) = 0;

	/*! \fn virtual STDMETHODIMP_(int) GetState(void)
	* \brief  0 - 正常  -1 找不到dwFourCC对应的解码器
	* \retval 解码器状态
	*/
	virtual STDMETHODIMP_(int) GetState(void) = 0;

	/*! \fn virtual STDMETHODIMP GetName(BSTR * ppStreamName)
	* \brief  获取流的名称
	* \param  ppStreamName [in / out] 流的名称
	* \retval 获取结果
	*/
	virtual STDMETHODIMP GetName(BSTR * ppStreamName) = 0;

	/*! \fn virtual STDMETHODIMP GetDescription(BSTR * ppDescription)
	* \brief  获取流的描述信息
	* \param  ppDescription [in / out] 流的描述信息
	* \retval 获取结果
	*/
	virtual STDMETHODIMP GetDescription(BSTR * ppDescription) = 0;

	/*! \fn virtual STDMETHODIMP_(double) GetLength(void)
	* \brief  获取流的播放时长
	* \retval 流的播放时长
	*/
	virtual STDMETHODIMP_(double) GetLength(void) = 0;

	/*! \fn virtual STDMETHODIMP_(int) GetBitRate(void)
	* \brief  获取流的码率
	* \retval 流的码率
	*/
	virtual STDMETHODIMP_(int) GetBitRate(void) = 0;

	/*! \fn virtual STDMETHODIMP_(const WSAudioInfo *) GetAudioInfo(void)
	* \brief  获取流的音频信息
	* \retval 流的音频信息
	* \see WSAudioInfo
	*/
	virtual STDMETHODIMP_(const WSAudioInfo *) GetAudioInfo(void) = 0;

	/*! \fn virtual STDMETHODIMP_(const WSVideoInfo *) GetVideoInfo(void)
	* \brief  获取流的视频信息
	* \retval 流的视频信息
	* \see WSVideoInfo
	*/
	virtual STDMETHODIMP_(const WSVideoInfo *) GetVideoInfo(void) = 0;

	/*! \fn virtual STDMETHODIMP_(__int64) GetStartPTS(void)
	* \brief  获取流的开始时间戳
	* \retval 流的开始时间戳
	*/
	virtual STDMETHODIMP_(__int64) GetStartPTS(void) = 0;

	/*! \fn virtual STDMETHODIMP_(__int64) GetPTSOffset(void)
	* \brief  获取当前流与节目的开始时间戳之差
	* \retval 当前流与节目的开始时间戳之差
	*/
	virtual STDMETHODIMP_(__int64) GetPTSOffset(void) = 0;
};

/*!
* \class IStreamInfoSrc
* \brief 获取流信息原始信息 
*/

// {A9AF814A-6F97-4a81-9453-4070EEC761BB}
DEFINE_GUID(IID_IStreamInfoSrc, 
			0xa9af814a, 0x6f97, 0x4a81, 0x94, 0x53, 0x40, 0x70, 0xee, 0xc7, 0x61, 0xbb);
EXTERN_C const IID IID_IID_IStreamInfoSrc;
MACRO_MIDL_INTERFACE(IStreamInfoSrc, "A9AF814A-6F97-4a81-9453-4070EEC761BB")
: public IUnknown
{
	/*! \fn virtual STDMETHODIMP_(const WSVideoInfo *) GetVideoSrcInfo(void)
	* \brief  获取流的原始视频信息
	* \retval 流的视频信息
	* \see WSVideoInfo
	*/
	virtual STDMETHODIMP_(const WSVideoInfo *) GetVideoSrcInfo(void) = 0;

	/*! \fn virtual STDMETHODIMP GetIPrograms(IProgramInfos **ppProgramInfos)
	* \brief 获取隔行逐行扫描信息
	* \retval 获取结果
	* \see IProgramInfos
	*/
	virtual STDMETHODIMP_(AV_SCAN_TYPE) GetScanType() = 0;

	/*! \fn virtual STDMETHODIMP GetIPrograms(IProgramInfos **ppProgramInfos)
	* \brief 获取隔行逐行扫描的原始信息
	* \retval 获取结果
	* \see IProgramInfos
	*/
	virtual STDMETHODIMP_(AV_SCAN_TYPE) GetSrcScanType() = 0;

	/*! \fn virtual STDMETHODIMP_(int) GetSrcRotate() = 0;
	* \brief 获取扫描的原始视频旋转信息
	* \retval 获取结果
	* \see IProgramInfos
	*/
	virtual STDMETHODIMP_(int) GetSrcRotate() = 0;

};
/*!
* \class IMediaStream
* \brief 获取流信息 
*/
// {E6C172B0-6D74-438f-81E0-433D92B2A627}
DEFINE_GUID(IID_IMediaStream, 
			0xe6c172b0, 0x6d74, 0x438f, 0x81, 0xe0, 0x43, 0x3d, 0x92, 0xb2, 0xa6, 0x27);
EXTERN_C const IID IID_IMediaStream;
MACRO_MIDL_INTERFACE(IMediaStream, "E6C172B0-6D74-438f-81E0-433D92B2A627")
: public IUnknown
{
public:

	/*! \fn virtual STDMETHODIMP GetStreamInfo(IStreamInfo** ppStreamInfo)
	* \brief  获取当前流的信息
	* \param ppStreamInfo [in / out] 输出流信息接口
	* \retval 获取结果
	* \see IStreamInfo 
	*/
	virtual STDMETHODIMP GetStreamInfo(IStreamInfo** ppStreamInfo) = 0;

	/*! \fn virtual STDMETHODIMP ChangeStream(int nStreamID)
	* \brief  变换一路流
	* \param nStreamID [in] 新流ID
	* \retval 变换结果
	*/
	virtual STDMETHODIMP ChangeStream(int nStreamID) = 0;

	/*! \fn virtual STDMETHODIMP_(double) Seek(double dTime, int accurate)
	* \brief  对流进行Seek
	* \param dTime [in] seek的时间点
	* \param accurate [in] 是否精确seek
	* \retval seek的结果
	*/
	virtual STDMETHODIMP_(double) Seek(double dTime, int accurate) = 0;

	/*! \fn virtual STDMETHODIMP_(MEDIA_FRAME *) GetSample(void)
	* \brief  获取当前的帧(视频) 或 Sample(音频)
	* \retval 获取的结果
	* \see MEDIA_FRAME
	* \note 若返回为NULL，那么表明文件解码完成
	*/
	virtual STDMETHODIMP_(MEDIA_FRAME *) GetSample(void) = 0;

	/*! \fn virtual STDMETHODIMP_(int) Forward(void)
	* \brief  前进下一帧(视频) 或 下一个Sample (音频)
	* \retval 1 表示以前进一帧，0表示解码结束
	*/
	virtual STDMETHODIMP_(int) Forward(void) = 0;

	/*! \fn virtual STDMETHODIMP_(double) GetOffset(void)
	* \brief  获取当前解码进度·
	* \retval 解码进度(当前解码时长)
	*/
	virtual STDMETHODIMP_(double) GetOffset(void) = 0;
};

/*!
* \class IVideoInput
* \brief 设置视频流的输入
*/
// {8F310739-7304-4fb9-B4C4-FF7B3B48D98E}
DEFINE_GUID(IID_IVideoInput, 
			0x8f310739, 0x7304, 0x4fb9, 0xb4, 0xc4, 0xff, 0x7b, 0x3b, 0x48, 0xd9, 0x8e);
EXTERN_C const IID IID_IVideoInput;
MACRO_MIDL_INTERFACE(IVideoInput, "8F310739-7304-4fb9-B4C4-FF7B3B48D98E")
: public IUnknown
{
public:

	/*! \fn virtual STDMETHODIMP SetVideoInput(IMediaStream* pMediaStream) 
	* \brief  设置视频流的输入·
	* \param pMediaStream [in] 视频流
	* \retval 设置结果
	* \see IMediaStream
	*/
	virtual STDMETHODIMP SetVideoInput(IMediaStream* pMediaStream) = 0;
};

/*!
* \class IVideoOutput
* \brief 获取视频流的输出
*/
// {30D4D994-A15B-4c2e-8277-66D0A8AA1B4F}
DEFINE_GUID(IID_IVideoOutput, 
			0x30d4d994, 0xa15b, 0x4c2e, 0x82, 0x77, 0x66, 0xd0, 0xa8, 0xaa, 0x1b, 0x4f);
EXTERN_C const IID IID_IVideoOutput;
MACRO_MIDL_INTERFACE(IVideoOutput, "30D4D994-A15B-4c2e-8277-66D0A8AA1B4F")
: public IUnknown
{
public:

	/*! \fn virtual STDMETHODIMP GetVideoOutput(IMediaStream** ppMediaStream)
	* \brief  获取视频流的输出·
	* \param ppMediaStream [in / out] 视频流
	* \retval 获取结果
	* \see IMediaStream
	*/
   virtual STDMETHODIMP GetVideoOutput(IMediaStream** ppMediaStream) = 0;
};

/*!
* \class IAudioInput
* \brief 设置音频流的输入
*/
// {89314327-F161-4483-A0EE-AFA455E825EA}
DEFINE_GUID(IID_IAudioInput, 
			0x89314327, 0xf161, 0x4483, 0xa0, 0xee, 0xaf, 0xa4, 0x55, 0xe8, 0x25, 0xea);
EXTERN_C const IID  IID_IAudioInput;
MACRO_MIDL_INTERFACE(IAudioInput, "89314327-F161-4483-A0EE-AFA455E825EA")
: public IUnknown
{
public:

	/*! \fn virtual STDMETHODIMP SetAudioInput(IMediaStream* pMediaStream)
	* \brief  设置音频流的输入·
	* \param pMediaStream [in] 音频流
	* \retval 设置结果
	* \see IMediaStream
	*/
	virtual STDMETHODIMP SetAudioInput(IMediaStream* pMediaStream) = 0;
};

/*!
* \class IAudioOutput
* \brief 获取音频流的输出
*/
// {29B857F5-9C9F-4586-85EA-BDE2F1F178F1}
DEFINE_GUID(IID_IAudioOutput, 
			0x29b857f5, 0x9c9f, 0x4586, 0x85, 0xea, 0xbd, 0xe2, 0xf1, 0xf1, 0x78, 0xf1);
EXTERN_C const IID IID_IAudioOutput;
MACRO_MIDL_INTERFACE(IAudioOutput, "29B857F5-9C9F-4586-85EA-BDE2F1F178F1")
: public IUnknown
{
public:

	/*! \fn  virtual STDMETHODIMP GetAudioOutput(IMediaStream** ppMediaStream)
	* \brief  获取音频流的输出·
	* \param ppMediaStream [in / out] 音频流
	* \retval 获取结果
	* \see IMediaStream
	*/
   virtual STDMETHODIMP GetAudioOutput(IMediaStream** ppMediaStream) = 0;	
};


/************************************************************************/
/*						DIBImage接口以及组件						    */
/************************************************************************/
// {2E1ADF80-6F97-47f5-A3AA-9EB80EE12D3F}

/*!
* \class IDibImage
* \brief 获取位图信息
*/
DEFINE_GUID(IID_IDibImage, 
			0x2e1adf80, 0x6f97, 0x47f5, 0xa3, 0xaa, 0x9e, 0xb8, 0xe, 0xe1, 0x2d, 0x3f);
EXTERN_C const IID IID_IDibImage;
MACRO_MIDL_INTERFACE(IDibImage, "2E1ADF80-6F97-47f5-A3AA-9EB80EE12D3F")
: public IUnknown
{
public:

	/*! \fn  virtual STDMETHODIMP_(int) GetWidth(void)
	* \brief  获取位图宽度·
	* \retval 位图宽度
	*/
	virtual STDMETHODIMP_(int) GetWidth(void) = 0;

	/*! \fn  virtual STDMETHODIMP_(int) GetHeight(void)
	* \brief  获取位图高度·
	* \retval 位图高度
	*/
	virtual STDMETHODIMP_(int) GetHeight(void) = 0;

	/*! \fn  virtual STDMETHODIMP_(int) GetBitCount(void) = 0
	* \brief  Get the number of bits that define each pixel and the maximum number of colors in the bitmap·
	* \retval bits count
	*/
	virtual STDMETHODIMP_(int) GetBitCount(void) = 0;

	/*! \fn  virtual STDMETHODIMP_(int) GetImageSize(void)
	* \brief 获取位图的大小
	* \retval 位图的大小
	*/
	virtual STDMETHODIMP_(int) GetImageSize(void) = 0;

	/*! \fn  virtual STDMETHODIMP_(int) GetPitch(void)
	* \brief 获取位图的每一行的数据量
	* \retval 位图的每一行的数据量
	*/
	virtual STDMETHODIMP_(int) GetPitch(void) = 0;

	/*! \fn  virtual STDMETHODIMP_(const LPBITMAPINFO) GetBitmapInfo(void)
	* \brief 获取位图信息
	* \retval 位图信息
	*/
	virtual STDMETHODIMP_(const LPBITMAPINFO) GetBitmapInfo(void) = 0;

	/*! \fn  virtual STDMETHODIMP_(const LPBYTE) GetBits(void)
	* \brief 获取位图数据的首地址
	* \retval 位图数据的首地址
	*/
	virtual STDMETHODIMP_(const LPBYTE) GetBits(void) = 0;

	/*! \fn  virtual STDMETHODIMP_(int) GetHeaderSize(void)
	* \brief 获取位图信息的结构体大小
	* \retval 位图信息的结构体大小
	*/
	virtual STDMETHODIMP_(int) GetHeaderSize(void) = 0;
};