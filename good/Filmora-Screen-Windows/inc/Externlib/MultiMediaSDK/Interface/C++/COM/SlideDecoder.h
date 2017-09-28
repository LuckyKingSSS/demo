/*! \file SlideDecoder.h
* \brief							视频连接接口调用说明
* Detailed
* 视频连接的主要功能是，对输入的多个文件，进行时间戳的累加，同时对齐音视频，使得输出的音视频时间戳是连续的，同步的。
* 视频连接接口分为两个，ISlideDecoder是严格按照TrimFrom, TrimeTo的时间剪切或补数据，实现音视频的对齐，以及时间戳的累加，
* 但是这种实现方式有个缺陷是：如果时间获取错误，那么会丢失数据，或补黑帧。
* 为了解决这个问题，实现了ISlideDecoderEX接口，该接口实现了音视频相互参考，而不依赖于TrimFrom, TrimeTo的时间，解决了由于时间不
* 准确而导致的问题。 
* 建议使用ISlideDecoderEX接口
*/

#pragma once
#include "CommonInterface.h"
#include "scombase.h"
#include "COMMacro.h"
#include "VideoSlideParam.h"
#include "AudioSlideParam.h"
#include "WSVideoInfo.h"
#include "WSAudioInfo.h"


//////////////////////////////////////////////////////////////////////////
//2009-03-02 Add Begin
//第二次进行文件合并的优化
/************************************************************************
功能：视频回调函数
参数：pUserObj [OUT]设置回调函数是使用指定的返回对象指针
pUserClipObj [OUT]对应的Clip对象指针
nOptFlag [OUT]操作标志，nOptFlag=1需要打开，nOptFlag=0需要关闭	
返回：返回VideoProcess对象句柄
说明：定义回调函数接口，供合并文件的时候与控制者在需要打开和关闭的时候进行
交互操作，提高效率。需要注意的是返回的VideoProcess由外部维护生存期
************************************************************************/
typedef IMediaStream *(__stdcall * SDEXVideoCallBack)(LPVOID pUserObj, LPVOID pUserClipObj, UINT nOptFlag);

/************************************************************************
功能：音频回调函数
参数：同上视频参数
返回：返回AudioProcess对象句柄
说明：定义回调函数接口，供合并文件的时候与控制者在需要打开和关闭的时候进行
交互操作，提高效率。需要注意的是返回的AudioProcess由外部维护生存期
************************************************************************/
typedef IMediaStream *(__stdcall * SDEXAudioCallBack)(LPVOID pUserObj, LPVOID pUserClipObj, UINT nOptFlag);


/*!
* \class ISlideDecoder
* \brief 视频连接接口 
*/

// {3DDBBF17-D26F-4fd5-90B4-347C519C3A0D}
DEFINE_GUID(IID_ISlideDecoder, 
			0x3ddbbf17, 0xd26f, 0x4fd5, 0x90, 0xb4, 0x34, 0x7c, 0x51, 0x9c, 0x3a, 0xd);
EXTERN_C const IID IID_ISlideDecoder;
MACRO_MIDL_INTERFACE(ISlideDecoder, "3DDBBF17-D26F-4fd5-90B4-347C519C3A0D")
: public IUnknown
{
public:
	
	/*! \fn virtual STDMETHODIMP SetVideoParam(const VideoSlideParam * pParam, LPVOID pUserObj, SDEXVideoCallBack fnCBVideo)
	* \brief 设置视频参数，以及回调函数，该回调函数用于，应用层创建视频解码器，传给底层
	* \param pParam [in] 视频参数，为输入的视频数据的参数信息
	* \param pUserObj [in] 回调函数的使用者参数
	* \param fnCBVideo [in] 回调函数函数指针
	* \retval HRESULT 设置的结果
	* \warning 要求视频输入的图像数据满足VideoSlideParam参数设置，否则会引发音视频不同步或其他问题
	*/
	virtual STDMETHODIMP SetVideoParam(const VideoSlideParam * pParam, LPVOID pUserObj, SDEXVideoCallBack fnCBVideo) = 0;

	/*! \fn virtual STDMETHODIMP_(const VideoSlideParam *) GetVideoParam(void)
	* \brief 获取设置进的视频参数
	* \retval 设置进的视频参数
	*/
	virtual STDMETHODIMP_(const VideoSlideParam *) GetVideoParam(void) = 0;

	/*! \fn virtual STDMETHODIMP AddVideoClip(IMediaStream *pMediaStream, const WSVideoInfo * pVideoInfo, const CLIP_TIME * pClipTime, LPVOID pUserClipObj)
	* \brief 置入一个视频频源及其处理参数（含剪裁、亮度对比度调节、特效设置等），多次调用即可置入多个视频源
	* \param pMediaStream [in] Slide创建的视频解码器接口
	* \param pVideoInfo [in] 输入视频数据参数
	* \param pClipTime [in] 剪切信息
	* \param pUserClipObj [in] 剪切标识
	* \retval HRESULT 设置的结果
	*/
	virtual STDMETHODIMP AddVideoClip(IMediaStream *pMediaStream, const WSVideoInfo * pVideoInfo, const CLIP_TIME * pClipTime, LPVOID pUserClipObj) = 0;

	/*! \fn virtual STDMETHODIMP SetAudioParam(const AudioSlideParam * pParam, LPVOID pUserObj, SDEXAudioCallBack fnCBAudio)
	* \brief 设置音频参数，以及回调函数，该回调函数用于，应用层创建音频解码器，传给底层
	* \param pParam [in] 音频参数，为输入的音频数据的参数信息
	* \param pUserObj [in] 回调函数的使用者参数
	* \param fnCBAudio [in] 回调函数函数指针
	* \retval HRESULT 设置的结果
	* \warning 要求音频输入的音频数据满足AudioSlideParam参数设置，否则会引发音视频不同步或其他问题
	*/
	virtual STDMETHODIMP SetAudioParam(const AudioSlideParam * pParam, LPVOID pUserObj, SDEXAudioCallBack fnCBAudio) = 0; 

	/*! \fn virtual STDMETHODIMP_(const AudioSlideParam *) GetAudioParam(void)
	* \brief 获取设置进的音频参数
	* \retval 设置进的音频参数
	*/
	virtual STDMETHODIMP_(const AudioSlideParam *) GetAudioParam(void) = 0;

	/*! \fn virtual STDMETHODIMP AddAudioClip(IMediaStream *pMediaStream, const WSAudioInfo * pAudioInfo, const CLIP_TIME * pClipTime, LPVOID pUserClipObj)
	* \brief 置入一个音频源及其处理参数（含剪裁、亮度对比度调节、特效设置等），多次调用即可置入多个音频源
	* \param pMediaStream [in] Slide创建的音频解码器接口
	* \param pAudioInfo [in] 输入音频数据参数
	* \param pClipTime [in] 剪切信息
	* \param pUserClipObj [in] 剪切标识
	* \retval HRESULT 设置的结果
	*/
	virtual STDMETHODIMP AddAudioClip(IMediaStream *pMediaStream, const WSAudioInfo * pAudioInfo, const CLIP_TIME * pClipTime, LPVOID pUserClipObj) = 0;
};


/*!
* \class IClipTimeItem
* \brief 设置剪切信息 
*/
DEFINE_GUID(IID_IClipTimeItem, 
					0x390235be, 0xc389, 0x4d58, 0xb1, 0x4b, 0x72, 0x47, 0x4d, 0x96, 0xe7, 0x58);
EXTERN_C const IID IID_IClipTimeItem;
MACRO_MIDL_INTERFACE(IClipTimeItem, "390235BE-C389-4d58-B14B-72474D96E758") 
: public IUnknown
{
public:
	/*! \fn virtual STDMETHODIMP SetClipTime(const CLIP_TIME * pClipTime)
	* \brief 设置剪切信息
	* \param pClipTime [in] 剪切参数
	* \retval 设置结果
	* \see CLIP_TIME
	*/
	virtual STDMETHODIMP SetClipTime(const CLIP_TIME * pClipTime) = 0;

	/*! \fn virtual STDMETHODIMP_(const CLIP_TIME *) GetClipTime()
	* \brief 获取当前设置的剪切信息
	* \retval 剪切参数
	* \see CLIP_TIME
	*/
	virtual STDMETHODIMP_(const CLIP_TIME *) GetClipTime() = 0;

	/*! \fn virtual STDMETHODIMP SetFileLength(const double dFileLength)
	* \brief 设置视频文件的原始总时长，
	* \param dFileLength [in] 文件时长
	* \retval 设置结果
	* \note 当TrimTo 等于 dFileLength时，表明没有对文件尾进行剪切，那么不参考TrimTo的时间，
	*       如果TrimTo < dFileLength，表明对文件尾进行了剪切，那么在做视频连接时根据，根据TrimTo的信息进行剪辑
	* \warning 非常重要，如果不设置，如果时长获取比实际的小，那么按照TrimTo做剪辑，会导致丢失数据
	*/
	virtual STDMETHODIMP SetFileLength(const double dFileLength) = 0;
	
	/*! \fn virtual STDMETHODIMP_(double) GetFileLength()
	* \brief 获取当前设置的文件时长
	* \retval 当前设置的文件时长
	*/
	virtual STDMETHODIMP_(double) GetFileLength() = 0;

	/*! \fn virtual STDMETHODIMP SetUserClipObject(LPVOID pUserObj)
	* \brief 设置Clip用户句柄
	* \param pUserObj [in] Clip用户句柄
	* \retval 设置的结果
	*/
	virtual STDMETHODIMP SetUserClipObject(LPVOID pUserObj) = 0;

	/*! \fn virtual STDMETHODIMP_(LPVOID) GetUserClipObject()
	* \brief 获取当前设置的Clip用户句柄
	* \retval 当前设置的Clip用户句柄
	*/
	virtual STDMETHODIMP_(LPVOID) GetUserClipObject() = 0;
};

/*!
* \class IClipTime
* \brief IClipTimeItem枚举器
*/
// {8088E13D-9B88-4f48-86C6-1F0A83AA0FE0}
DEFINE_GUID(IID_IClipTime, 
					0x8088e13d, 0x9b88, 0x4f48, 0x86, 0xc6, 0x1f, 0xa, 0x83, 0xaa, 0xf, 0xe0);
EXTERN_C const IID IID_IClipTime;
MACRO_MIDL_INTERFACE(IClipTime, "8088E13D-9B88-4f48-86C6-1F0A83AA0FE0")
:public IUnknown
{
	public:

	/*! \fn virtual STDMETHODIMP Add(IClipTimeItem **ppClipTimetem)
	* \brief 添加一个Clip信息项
	* \param ppClipTimetem [in /out] 创建一个IClipTimeItem接口实例输出
	* \retval 获取结果
	* \warning 使用完毕后，务必调用release方法释放资源
	* \see IClipTimeItem
	*/
	virtual STDMETHODIMP Add(IClipTimeItem **ppClipTimetem) = 0;

	/*! \fn virtual STDMETHODIMP_(int) GetCount(void)
	* \brief 获取添加进的Clip项的数量
	* \retval 添加进的Clip项的数量
	*/
	virtual STDMETHODIMP_(int) GetCount(void) = 0;

	/*! \fn virtual STDMETHODIMP GetItem(int nIndex, IClipTimeItem **ppClipTimetem)
	* \brief 获取一个指定的Clip信息项
	* \param nIndex [in] 指定的信息项的索引
	* \param ppClipTimetem [in /out] 获取与nIndex对应的IClipTimeItem接口实例输出
	* \retval 获取结果
	* \note 获取失败，那么输出*ppClipTimetem为NULL 
	* \warning 使用完毕后，务必调用release方法释放资源
	* \see IClipTimeItem
	*/
	virtual STDMETHODIMP GetItem(int nIndex, IClipTimeItem **ppClipTimetem) = 0;

	/*! \fn virtual STDMETHODIMP Remove(IClipTimeItem* pClipTimetem, int *pIndex)
	* \brief 移除一个指定的Clip信息项
	* \param pClipTimeItem [in] 要移除的Clip信息项接口实例指针
	* \param pIndex [in /out] 返回该实例指针在列表中的索引号
	* \retval 移除结果
	*/
	virtual STDMETHODIMP Remove(IClipTimeItem *pClipTimeItem, int *pIndex) = 0;

	/*! \fn virtual STDMETHODIMP Clear()
	* \brief 移除所有的Clip信息项
	* \retval 移除结果
	*/
	virtual STDMETHODIMP Clear() = 0;
};

/*!
* \class ISlideDecoderEX
* \brief 改进后的视频连接接口
* Detailed 输入的音视频解码器均由回调函数创建，通过QueryInerface查询IVideoOutput, IAudioOutput接口，获取音视频流接口IMediaStream的输出，
* 该模块接口只配合Convert模块使用， 若做其它用途，请与研发中心联系。该模块输入的音频视频流，应经过DataProcess模块，经过图像缩放，重采样，声道变换，平滑时间戳处理。
* 否则可能导致输出音视频不同步。
*/
DEFINE_GUID(IID_ISlideDecoderEX, 
			0xafd00486, 0x9c35, 0x4e93, 0x80, 0x87, 0xa7, 0x3e, 0x78, 0x34, 0x53, 0xe7);
EXTERN_C const IID IID_ISlideDecoderEX;
MACRO_MIDL_INTERFACE(ISlideDecoderEX, "AFD00486-9C35-4e93-8087-A73E783453E7")
: public IUnknown
{
public:
	/*! \fn virtual STDMETHODIMP SetParam(const VideoSlideParam * pVideoParam, const AudioSlideParam * pAudioParam)
	* \brief 设置音视频参数
	* \param pVideoParam [in] 视频参数
	* \param pAudioParam [in] 音频参数
	* \retval 设置结果
	* \note 音视频参数不能同时为空， 若pVideoParam为空那么表明输出纯音频流， 若pAudioParam为空，那么输出纯视频流
	* \see VideoSlideParam AudioSlideParam 
	*/
	virtual STDMETHODIMP SetParam(const VideoSlideParam * pVideoParam, const AudioSlideParam * pAudioParam) = 0;

	/*! \fn virtual STDMETHODIMP GetClipTimeSetting(IClipTime **ppClipTime)
	* \brief 获取Clip信息设置枚举器
	* \param ppClipTime [in / Out] Clip信息枚举器接口实例
	* \retval 获取结果
	* \warning 使用完毕后，请调用Release方法，释放接口
	* \see IClipTime 
	*/
	virtual STDMETHODIMP GetClipTimeSetting(IClipTime **ppClipTime) = 0;

	/*! \fn virtual STDMETHODIMP SetVideoCallBack(SDEXVideoCallBack fnCBVideo, LPVOID pUserObj)
	* \brief 设置创建视频流IMediaStream接口的回调函数函数指针，以及创建者句柄
	* \param fnCBVideo [in] 创建视频流IMediaStream接口的回调函数函数指针
	* \param pUserObj [in] 创建者句柄
	* \retval 设置结果e 
	*/
	virtual STDMETHODIMP SetVideoCallBack(SDEXVideoCallBack fnCBVideo, LPVOID pUserObj) = 0;

	/*! \fn virtual STDMETHODIMP_(const VideoSlideParam *) GetVideoParam(void)
	* \brief 获取当前的视频参数
	* \retval 视频参数e
	* \see VideoSlideParam
	*/
	virtual STDMETHODIMP_(const VideoSlideParam *) GetVideoParam(void) = 0;

	/*! \fn virtual STDMETHODIMP SetAudioCallBack(SDEXAudioCallBack fnCBAudio, LPVOID pUserObj)
	* \brief 设置创建音频流IMediaStream接口的回调函数函数指针，以及创建者句柄
	* \param fnCBAudio [in] 创建音频流IMediaStream接口的回调函数函数指针
	* \param pUserObj [in] 创建者句柄
	* \retval 设置结果e 
	*/
	virtual STDMETHODIMP SetAudioCallBack(SDEXAudioCallBack fnCBAudio, LPVOID pUserObj) = 0;

	/*! \fn virtual STDMETHODIMP_(const AudioSlideParam *) GetAudioParam(void)
	* \brief 获取当前的音频参数
	* \retval 音频参数e
	* \see AudioSlideParam
	*/
	virtual STDMETHODIMP_(const AudioSlideParam *) GetAudioParam(void) = 0;
};

//定义组件对象MediaPlayer
MIDL_DEFINE_GUID(CLSID, CLSID_CSlideDecode, 0x21c7bb8b, 0x657a, 0x46fb, 0x86, 0xe5, 0xa4, 0x39, 0x96, 0xa, 0xd0, 0x4);

EXTERN_C const CLSID CLSID_CSlideDecode;