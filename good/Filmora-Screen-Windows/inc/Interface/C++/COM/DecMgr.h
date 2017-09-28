/*! \file DecMgr.h
 * \brief							解码器接口调用说明
 * Detailed
 *应用层可以通过调用DecMgr模块获取IDecMgr的接口，通过该接口的GetMediaInfo方法获取媒体信息接口IMediaInfo，
 *该接口提供了获取媒体信息的方法。为了获取ID3或媒体的其它信息，那么可以调用IMediaInfo接口中的QueryInerface方法，查询是否支持该接口。
 *为了获取音视频流接口IMediaStream，使用IDecMgr接口的CreateMediaDecoder方法，创建媒体解码器IMediaDecoder接口，
 *通过QueryInterface方法查询IVideoOutput, IAudioOutput接口，它们提供了获取音视频解码器接口IMediaStream的方法GetVideoOutput， GetAudioOutput。
 *IMediaDecoder接口提供了动态切换流的方法ChangeStream。如果是外挂字幕，通过IMediaStream接口的QueryInterface方法，
 *获取外挂字幕解码设置接口ISubPicSetting，通过该接口获取各种设置的方法。
 */


#pragma once
#include <scombase.h>
#include "COMMacro.h"
#include "TextDef.h"
#include "DecodeParam.h"
#include "CodecDefine.h"
#include "CommonInterface.h"

/*!
 * \class ISubPicStreamInfo
 * \brief 获取字幕的信息接口 
 */

// {FB73C029-B0BD-4cbb-A0CE-971E64D4BDD4}
DEFINE_GUID(IID_ISubPicStreamInfo, 
			0xfb73c029, 0xb0bd, 0x4cbb, 0xa0, 0xce, 0x97, 0x1e, 0x64, 0xd4, 0xbd, 0xd4);
EXTERN_C const IID IID_ISubPicStreamInfo;
MACRO_MIDL_INTERFACE(ISubPicStreamInfo, "FB73C029-B0BD-4cbb-A0CE-971E64D4BDD4")
: public IUnknown
{
public:
	/*! \fn virtual STDMETHODIMP_(SUPPIC_TYPE) GetType(void)
	* \brief 获取字幕的类型， 外挂还是内嵌
	* \retval 字幕的类型
	*/
	virtual STDMETHODIMP_(SUPPIC_TYPE) GetType(void) = 0;

	/*! \fn virtual STDMETHODIMP_(UINT) GetID(void)
	* \brief 获取字幕的ID
	* \retval 字幕的ID
	*/
	virtual STDMETHODIMP_(UINT) GetID(void) = 0;

	/*! \fn virtual STDMETHODIMP_(UINT) GetFourCC(void)
	* \brief 获取字幕的FourCC
	* \retval 字幕的FourCC
	*/
	virtual STDMETHODIMP_(UINT) GetFourCC(void) = 0;

	/*! \fn virtual STDMETHODIMP GetLanguage(BSTR * ppLanguage)
	* \brief 获取字幕的语言
	* \param ppLanguage [int/out] 字幕的语言
	* \retval HRESULT 获取结果
	*\ \warning 输出的ppLanguage，必须使用SysFreeString释放，否则会造成内存泄露 
	*/
	virtual STDMETHODIMP GetLanguage(BSTR * ppLanguage) = 0;

	/*! \fn virtual STDMETHODIMP GetDescription(BSTR * ppDescription)
	* \brief 获取字幕的描述
	* \param ppDescription [int/out] 字幕的描述
	* \retval HRESULT 获取结果
	* \warning 输出的ppDescription，必须使用SysFreeString释放，否则会造成内存泄露 
	*/
	virtual STDMETHODIMP GetDescription(BSTR * ppDescription) = 0;
};

/*!
* \class ISubPicStreamInfos
* \brief 字幕信息枚举接口 
*/

// {4291F08A-95C4-45c3-AF5B-1CC55A3F2CF7}
DEFINE_GUID(IID_ISubPicStreamInfos, 
			0x4291f08a, 0x95c4, 0x45c3, 0xaf, 0x5b, 0x1c, 0xc5, 0x5a, 0x3f, 0x2c, 0xf7);
EXTERN_C const IID IID_ISubPicStreamInfos;
MACRO_MIDL_INTERFACE(ISubPicStreamInfos, "4291F08A-95C4-45c3-AF5B-1CC55A3F2CF7")
: public IUnknown
{
public:
	/*! \fn virtual STDMETHODIMP_(int) GetCount(void)
	* \brief 获取字幕的数量
	* \retval 字幕的数量
	*/
	virtual STDMETHODIMP_(int) GetCount(void) = 0;

	/*! \fn virtual STDMETHODIMP GetItem(int nIndex, ISubPicStreamInfo** ppItem)
	* \brief 获取指定的字幕信息接口
    * \param nIndex [in] 字幕的索引号
	* \param ppItem [out] 字幕信息接口 
	* \retval 获取的结果
	* \see ISubPicStreamInfo
	* \warning ppItem使用完毕后，一定要调用Release方法释放，避免内存泄露。
	*/
	virtual STDMETHODIMP GetItem(int nIndex, ISubPicStreamInfo** ppItem) = 0;
};


/*!
* \class IStreamInfos
* \brief 流信息枚举接口 
*/

// {5DD650C8-40A3-4d0d-B460-0BA5804A1AAD}
DEFINE_GUID(IID_IStreamInfos, 
			0x5dd650c8, 0x40a3, 0x4d0d, 0xb4, 0x60, 0xb, 0xa5, 0x80, 0x4a, 0x1a, 0xad);
EXTERN_C const IID IID_IStreamInfos;
MACRO_MIDL_INTERFACE(IStreamInfos, "5DD650C8-40A3-4d0d-B460-0BA5804A1AAD")
: public IUnknown
{
public:
	/*! \fn virtual STDMETHODIMP_(int) GetCount(void)
	* \brief 获取流的数量
	* \retval 流的数量
	*/
	virtual STDMETHODIMP_(int) GetCount(void) = 0;

	/*! \fn virtual STDMETHODIMP GetItem(int nIndex, IStreamInfo** ppItem)
	* \brief 获取指定的流信息接口
	* \param nIndex [in] 流的索引号
	* \param ppItem [out] 流信息接口 
	* \retval 获取的结果
	* \see IStreamInfo
	* \warning ppItem使用完毕后，一定要调用Release方法释放，避免内存泄露。
	*/
	virtual STDMETHODIMP GetItem(int nIndex, IStreamInfo** ppItem) = 0;
};

/*!
* \class IProgramInfo
* \brief 节目信息接口，用于获取媒体文件中的节目信息 
*/

// {F5FA588B-55AF-4fa9-8913-323CFF39C231}
DEFINE_GUID(IID_IProgramInfo, 
			0xf5fa588b, 0x55af, 0x4fa9, 0x89, 0x13, 0x32, 0x3c, 0xff, 0x39, 0xc2, 0x31);
EXTERN_C const IID IID_IProgramInfo;
MACRO_MIDL_INTERFACE(IProgramInfo, "F5FA588B-55AF-4fa9-8913-323CFF39C231")
: public IUnknown
{
public:

	/*! \fn virtual STDMETHODIMP_(UINT) GetNumber(void)
	* \brief 获取节目在Number,暂无实际意义
	* \retval 节目的Number
	*/
	virtual STDMETHODIMP_(UINT) GetNumber(void) = 0;

	/*! \fn virtual STDMETHODIMP_(UINT) GetID(void)
	* \brief 获取节目的ID，ID是节目的唯一标示
	* \retval 节目的ID
	*/
	virtual STDMETHODIMP_(UINT) GetID(void) = 0;

	/*! \fn virtual STDMETHODIMP_(double) GetMediaLength(void)
	* \brief 获取节目的时间长度，单位是秒
	* \retval 节目的时长
	* \note   取节目中各路流，时长的最大值
	*/
	virtual STDMETHODIMP_(double) GetMediaLength(void) = 0;

	/*! \fn virtual STDMETHODIMP_(int)	  GetBitrate()
	* \brief 获取节目的码率
	* \retval 节目的码率
	* \note 取各路流码率之和
	*/
	virtual STDMETHODIMP_(int)	  GetBitrate() = 0;	
	
	/*! \fn virtual STDMETHODIMP_(UINT) GetTimeStampReferenceStreamID(void)
	* \brief 获取节目时间戳参考流ID，
	* \retval 时间戳参考流ID
	*/
	virtual STDMETHODIMP_(UINT) GetTimeStampReferenceStreamID(void) = 0;

	/*! \fn virtual STDMETHODIMP GetVideoStreamInfos(IStreamInfos** ppVideoStreamInfos)
	* \brief 获取视频流信息枚举器接口，
	* \param ppVideoStreamInfos [out] 视频流枚举器接口
	* \retval 获取结果
	* \see IStreamInfos
	*/
	virtual STDMETHODIMP GetVideoStreamInfos(IStreamInfos** ppVideoStreamInfos) = 0;

	/*! \fn virtual STDMETHODIMP GetAudioStreamInfos(IStreamInfos** ppAudioStreamInfos)
	* \brief 获取音频流信息枚举器接口，
	* \param ppAudioStreamInfos [out] 音频流信息枚举器接口
	* \retval 获取结果
	* \see IStreamInfos
	*/
	virtual STDMETHODIMP GetAudioStreamInfos(IStreamInfos** ppAudioStreamInfos) = 0;

	/*! \fn virtual STDMETHODIMP GetSubPicStreamInfos(ISubPicStreamInfos** ppSubPicStreamInfos)
	* \brief 获取字幕流信息枚举器接口，
	* \param ppSubPicStreamInfos [out] 字幕流信息枚举器接口
	* \retval 获取结果
	* \see ISubPicStreamInfos
	*/
	virtual STDMETHODIMP GetSubPicStreamInfos(ISubPicStreamInfos** ppSubPicStreamInfos) = 0;
};

/*!
* \class IProgramInfos
* \brief 节目信息枚举器接口 
*/

// {E04C85FD-D887-4716-8D7A-D18A17CE60EC}
DEFINE_GUID(IID_IProgramInfos, 
			0xe04c85fd, 0xd887, 0x4716, 0x8d, 0x7a, 0xd1, 0x8a, 0x17, 0xce, 0x60, 0xec);
EXTERN_C const IID IID_IProgramInfos;
MACRO_MIDL_INTERFACE(IProgramInfos, "E04C85FD-D887-4716-8D7A-D18A17CE60EC")
: public IUnknown
{
public:

	/*! \fn virtual STDMETHODIMP_(int) GetCount(void)
	* \brief 获取节目的数量
	* \retval 节目的数量
	*/
	virtual STDMETHODIMP_(int) GetCount() = 0;

	/*! \fn virtual STDMETHODIMP GetItem(int nIndex, IProgramInfo** ppItem)
	* \brief 获取指定的节目信息接口
	* \param nIndex [in] 节目的索引号
	* \param ppItem [out] 节目信息接口 
	* \retval 获取的结果
	* \see IProgramInfo
	*/
	virtual STDMETHODIMP GetItem(int nIndex, IProgramInfo** ppItem) = 0;	
};

/*!
* \class IMetaDataInfo
* \brief 获取MetaData信息接口 
*/

// {80DEF872-887D-4426-B31B-771011CF3427}
DEFINE_GUID(IID_IMetaDataInfo, 
            0x80def872, 0x887d, 0x4426, 0xb3, 0x1b, 0x77, 0x10, 0x11, 0xcf, 0x34, 0x27);
EXTERN_C const IID IID_IMetaDataInfo;
MACRO_MIDL_INTERFACE(IMetaDataInfo, "80DEF872-887D-4426-B31B-771011CF3427")
: public IUnknown
{
public:

	/*! \fn virtual STDMETHODIMP GetArtist(BSTR *pArtist)
	* \brief 获取艺术家信息
	* \param pArtist [out] 艺术家信息 
	* \retval 获取的结果
	*/
    virtual STDMETHODIMP GetArtist(BSTR *pArtist) = 0;

	/*! \fn virtual STDMETHODIMP GetTitle(BSTR *pTitle)
	* \brief 获取标题信息
	* \param pTitle [out] 标题信息 
	* \retval 获取的结果
	*/
    virtual STDMETHODIMP GetTitle(BSTR *pTitle) = 0;

	/*! \fn virtual STDMETHODIMP GetTrackNumber(BSTR *pTrackNumber)
	* \brief 获取音轨号
	* \param pTrackNumber [out] 音轨号
	* \retval 获取的结果
	*/
    virtual STDMETHODIMP GetTrackNumber(BSTR *pTrackNumber) = 0;

	/*! \fn virtual STDMETHODIMP GetAlbum(BSTR *pAlbum)
	* \brief 获取专辑名称
	* \param pAlbum [out] 专辑名称
	* \retval 获取的结果
	*/
    virtual STDMETHODIMP GetAlbum(BSTR *pAlbum) = 0;

	/*! \fn virtual STDMETHODIMP GetDate(BSTR *pDate)
	* \brief 获取年代
	* \param pDate [out] 年代
	* \retval 获取的结果
	*/
    virtual STDMETHODIMP GetDate(BSTR *pDate) = 0;

	/*! \fn virtual STDMETHODIMP GetGenre(BSTR *pGenre)
	* \brief 获取风格
	* \param pGenre [out] 风格
	* \retval 获取的结果
	*/
    virtual STDMETHODIMP GetGenre(BSTR *pGenre) = 0;
   
	/*! \fn virtual STDMETHODIMP GetPublisher(BSTR *pPublisher)
	* \brief 获取发行商
	* \param pPublisher [out] 发行商
	* \retval 获取的结果
	*/
	virtual STDMETHODIMP GetPublisher(BSTR *pPublisher) = 0;
};


/*!
* \class ICaptureMediaInfo
* \brief 获取录制文件信息接口，通过IMediaInfo查询得到 
*/

// {8C376301-E8AD-4037-A9FB-5C73C290AB9E}
DEFINE_GUID(IID_ICaptureMediaInfo, 
			0x8c376301, 0xe8ad, 0x4037, 0xa9, 0xfb, 0x5c, 0x73, 0xc2, 0x90, 0xab, 0x9e);
EXTERN_C const IID IID_ICaptureMediaInfo;
MACRO_MIDL_INTERFACE(ICaptureMediaInfo, "8C376301-E8AD-4037-A9FB-5C73C290AB9E")
: public IUnknown
{
public:
	/*! \fn virtual STDMETHODIMP_(UINT) GetFourCC()
	* \brief 获取流类型
	* \param nStreamID [in] 流ID
	* \param pType [out] 流类型，参考NLEScreenRecordingStreamType
	* \retval 文件FourCC
	*/
	virtual STDMETHODIMP GetStreamType(int nStreamID, unsigned int *pType) = 0;
};

/*!
* \class IMediaInfo
* \brief 获取媒体文件信息接口 
*/

// {10CA4A98-A525-42a5-9282-DD64406261B2}
DEFINE_GUID(IID_IMediaInfo, 
			0x10ca4a98, 0xa525, 0x42a5, 0x92, 0x82, 0xdd, 0x64, 0x40, 0x62, 0x61, 0xb2);
EXTERN_C const IID IID_IMediaInfo;
MACRO_MIDL_INTERFACE(IMediaInfo, "10CA4A98-A525-42a5-9282-DD64406261B2")
: public IUnknown
{
public:

	/*! \fn virtual STDMETHODIMP_(UINT) GetFourCC()
	* \brief 获取文件FourCC
	* \retval 文件FourCC
	*/
	virtual STDMETHODIMP_(UINT) GetFourCC() = 0;

	/*! \fn virtual STDMETHODIMP GetFileName(BSTR * pFileName)
	* \brief 获取文件名
	* \param pFileName [out] 文件名
	* \retval 获取的结果
	*/
	virtual STDMETHODIMP GetFileName(BSTR * pFileName) = 0;

	/*! \fn virtual STDMETHODIMP GetDescription(BSTR * ppDescription)
	* \brief 获取文件的描述信息
	* \param ppDescription [out] 文件的描述信息
	* \retval 获取的结果
	*/
	virtual STDMETHODIMP GetDescription(BSTR * ppDescription) = 0;

	/*! \fn virtual STDMETHODIMP_(double) GetMediaLength(void)
	* \brief 获取媒体文件的播放时长
	* \retval 媒体文件的播放时长
	* \note 媒体文件播放时长，取媒体文件中包含的各路节目，时长的最大值
	*/
	virtual STDMETHODIMP_(double) GetMediaLength(void) = 0;

	/*! \fn virtual STDMETHODIMP_(int) GetBitrate(void)
	* \brief 获取媒体文件的码率
	* \retval 媒体文件的码率
	* \note 媒体文件的码率，取各路节目码率之和
	*/
	virtual STDMETHODIMP_(int) GetBitrate(void) = 0;

	/*! \fn virtual STDMETHODIMP GetIPrograms(IProgramInfos **ppProgramInfos)
	* \brief 获取节目信息枚举器接口
	* \retval 获取结果
	* \see IProgramInfos
	*/
	virtual STDMETHODIMP GetIPrograms(IProgramInfos **ppProgramInfos) = 0;

	/*! \fn virtual STDMETHODIMP GetIPrograms(IProgramInfos **ppProgramInfos)
	* \brief 获取隔行逐行扫描信息
	* \retval 获取结果
	* \see IProgramInfos
	*/
	virtual STDMETHODIMP_(AV_SCAN_TYPE) GetScanType() = 0;

};

// {89D89E0A-02F1-47a1-8756-323A9098937E}
DEFINE_GUID(IID_ISubPicSetting, 
			0x89d89e0a, 0x2f1, 0x47a1, 0x87, 0x56, 0x32, 0x3a, 0x90, 0x98, 0x93, 0x7e);
EXTERN_C const IID IID_ISubPicSetting;

/*!
* \class ISubPicSetting
* \brief 字幕显示设置接口
*/

MACRO_MIDL_INTERFACE(ISubPicSetting, "89D89E0A-02F1-47a1-8756-323A9098937E")
: public IUnknown
{
public:

	/*! \brief 设置字幕显示图像的大小
	* \param nWidth [in] 字幕的宽
	* \param nHeight [in] 字幕的高
	* \retval 设置结果
	*/
	virtual STDMETHODIMP SetRect(int nWidth, int nHeight) = 0;

	/*! \brief 获取字幕图像的大小
	* \param pWidth [out] 字幕的宽
	* \param pHeight [out] 字幕的高
	* \retval 获取结果
	*/
	virtual STDMETHODIMP GetRect(int * pWidth, int * pHeight) = 0;

	/*! \brief 设置字幕显示效果，是否使用缺省模式
	* \param bUse [int] 1表示使用，0表示不使用
	* \retval 设置结果
	*/
	virtual STDMETHODIMP SetUseDefaultStyle(int bUse) = 0;

	/*! \brief 获取当前字幕显示效果，是否采用缺省模式
	* \retval 1表示采用，0表示未采用
	*/
	virtual STDMETHODIMP_(int) GetUseDefaultStyle(void) = 0;

	/*! \brief 设置字幕显示的字体
	* \param fontName [int] 字体名称
	* \retval 设置结果
	*/
	virtual STDMETHODIMP SetFontName(const BSTR fontName) = 0;

	/*! \brief 获取当前字幕显示的字体
	* \param pFontName [out] 字体名称
	* \retval 获取结果
	*/
	virtual STDMETHODIMP GetFontName(BSTR * pFontName) = 0;

	/*! \brief 设置字幕显示的字号
	* \param nFontSize [int] 字号
	* \retval 设置结果
	*/
	virtual STDMETHODIMP SetFontSize(int nFontSize) = 0;

	/*! \brief 获取当前字幕显示的字号
	* \retval 字号
	*/
	virtual STDMETHODIMP_(int) GetFontSize(void) = 0;

	/*! \brief 设置当前字幕显示的字体样式
	* \param nStyle  [in] 字体样式,取值范围PCSFontStyleRegular | PCSFontStyleBold | PCSFontStyleItalic | PCSFontStyleUnderline
	* \retval 设置结果
	*/
	virtual STDMETHODIMP SetStyle(UINT nStyle) = 0;

	/*! \brief 获取当前字幕显示的字体样式
	* \retval 字体样式
	*/
	virtual STDMETHODIMP_(UINT) GetStyle(void) = 0;

	/*! \brief 设置当前字幕显示的字体颜色
	* \param fontColor  [in] 字体颜色
	* \retval 设置结果
	*/
	virtual STDMETHODIMP SetFontColor(COLORREF fontColor) = 0;

	/*! \brief 获取当前字幕显示的字体颜色
	* \retval 字体颜色
	*/
	virtual STDMETHODIMP_(COLORREF) GetFontColor(void) = 0;

	/*! \brief 设置字幕显示的字体是否使用阴影
	* \param bUse  [in] 1表示使用，0表示不使用
	* \retval 设置结果
	*/
	virtual STDMETHODIMP SetUseShadow(int bUse) = 0;

	/*! \brief 获取字幕显示的字体是否使用阴影
	* \retval 1表示使用，0表示未使用
	*/
	virtual STDMETHODIMP_(int) GetUseShadow(void) = 0;

	/*! \brief 设置文字阴影的参数
	* \param paramShadow [in] 文字阴影的参数
	* \retval 设置结果
	* \note 只有当设置使用文字阴影后，才有效
	*/
	virtual STDMETHODIMP_(int) SetParamShadow(SHADOWPARAM paramShadow) = 0;

	/*! \brief 获取当前字幕显示的文字阴影参数
	* \retval 文字阴影的参数
	* \see SHADOWPARAM
	*/
	virtual STDMETHODIMP_(SHADOWPARAM) GetParamShadow(void) = 0;

	/*! \brief 设置光晕效果
	* \param nValue [in] 光晕值，0为无光晕效果
	* \param dwColor [int] 光晕颜色值
	* \retval 设置结果
	*/
	virtual STDMETHODIMP SetHalation(int nValue, COLORREF dwColor) = 0;

	/*! \brief 获取当前设置的光晕效果
	* \param pValue [out] 光晕值
	* \param pColor [out] 光晕颜色值
	* \retval 获取结果
	*/
	virtual STDMETHODIMP GetHalation(int *pValue, COLORREF * pColor) = 0;

	/*! \brief 设置旋转角度
	* \param nAngle [in] 旋转角度
	* \retval 设置结果
	*/
	virtual STDMETHODIMP SetAngle(int nAngle) = 0;

	/*! \brief 获取旋转角度
	* \retval 旋转角度
	*/
	virtual STDMETHODIMP_(int) GetAngle(void) = 0;

	/*! \brief 设置字幕图片视频图片上的位置
	* \param nXCoordinate [in] 字幕图片左上角在视频图片上的X坐标
	* \param nYCoordinate [in] 字幕图片左上角在视频图片上的Y坐标
	* \retval 设置结果
	*/
	virtual STDMETHODIMP SetLeftTopPos(int nXCoordinate, int nYCoordinate) = 0;

	/*! \brief 获取当前字幕图片视频图片上的位置
	* \param pXCoordinate [out] 字幕图片左上角在视频图片上的X坐标
	* \param pYCoordinate [out] 字幕图片左上角在视频图片上的Y坐标
	* \retval 获取结果
	*/
	virtual STDMETHODIMP GetLeftTopPos(int * pXCoordinate, int * pYCoordinate) = 0;
};

/*!
* \class IMediaDecoder
* \brief 媒体解码器接口，通过QueryInterface方法查询IVideoOutput, IAudioOutput接口，它们提供了获取音视频流(IMediaStream)的输出方法
* \see IVideoOutput IAudioOutput 接口
*/

// {C8BD5D61-172A-4ca2-98B7-DE5F1E995D72}
DEFINE_GUID(IID_IMediaDecoder, 
			0xc8bd5d61, 0x172a, 0x4ca2, 0x98, 0xb7, 0xde, 0x5f, 0x1e, 0x99, 0x5d, 0x72);
EXTERN_C const IID IID_IMediaDecoder;
MACRO_MIDL_INTERFACE(IMediaDecoder, "C8BD5D61-172A-4ca2-98B7-DE5F1E995D72")
: public IUnknown
{
public:
};


/*!
* \class IDecMgr
* \brief 解码管理器接口，通过它可以获取解码器接口IMediaDecoder，以及文件信息接口IMediaInfo
* \see IMediaDecoder IMediaInfo
*/

// {5AD2BE35-3038-4c6a-8C7F-3698526A898F}
DEFINE_GUID(IID_IDecMgr, 
			0x5ad2be35, 0x3038, 0x4c6a, 0x8c, 0x7f, 0x36, 0x98, 0x52, 0x6a, 0x89, 0x8f);
EXTERN_C const IID IID_IDecMgr;
MACRO_MIDL_INTERFACE(IDecMgr, "5AD2BE35-3038-4c6a-8C7F-3698526A898F")
: public IUnknown
{
public:

	/*! \fn virtual STDMETHODIMP_(IMediaDecoder *) CreateMediaDecoder(const BSTR pMediaPath, FILE_TYPE fileType, const DecParam * pVideoDecParam, const DecParam * pAudioDecParam, DEC_MODEL DecMod)
	* \brief 根据媒体文件路径及其解码参数，创建解码器接口IMediaDecoder
	* \param pMediaPath [int] 媒体文件路径
	* \param fileType [int] 媒体文件类型 分为音视频文件，与外挂字幕
	* \param pVideoDecParam [in] 视频解码参数
	* \param pAudioDecParam [in] 音频解码参数
	* \param DecMod [in] 解码方式选择
	* \retval 解码器接口 IMediaDecoder
	* \see FILE_TYPE DecParam DEC_MODEL IMediaDecoder
	*/
	virtual STDMETHODIMP_(IMediaDecoder *) CreateMediaDecoder(const BSTR pMediaPath, FILE_TYPE fileType, const DecParam * pVideoDecParam, const DecParam * pAudioDecParam, DEC_MODEL DecMod) = 0;


	/*! \fn virtual STDMETHODIMP GetMediaInfo(const BSTR pMediaPath, FILE_TYPE fileType, DEC_MODEL DecMod, IMediaInfo** ppIMediaInfo)
	* \brief 根据媒体文件路径及参数，获取媒体文件信息接口IMediaInfo
	* \param pMediaPath [int] 媒体文件路径
	* \param fileType [int] 媒体文件类型 分为音视频文件，与外挂字幕
	* \param DecMod [in] 解码方式选择
	* \param ppIMediaInfo 媒体信息接口
	* \retval 获取结果
	* \see FILE_TYPE  DEC_MODEL IMediaInfo
	*/
	virtual STDMETHODIMP GetMediaInfo(const BSTR pMediaPath, FILE_TYPE fileType, DEC_MODEL DecMod, IMediaInfo** ppIMediaInfo) = 0;


	virtual STDMETHODIMP_(IMediaDecoder *) CreateMediaDecoder(const BSTR pMediaPath, FILE_TYPE fileType, const DecParam * pVideoDecParam, const DecParam * pAudioDecParam, DEC_MODEL DecMod,IMediaInfo* pIMediaInfo) = 0;
	
};



// {E71EAD89-1CCD-4c34-97A8-7D45B5F16613}
//定义组件对象DecMgr
MIDL_DEFINE_GUID(CLSID, CLSID_CDecMgr, 0xe71ead89, 0x1ccd, 0x4c34, 0x97, 0xa8, 0x7d, 0x45, 0xb5, 0xf1, 0x66, 0x13);

EXTERN_C const CLSID CLSID_CDecMgr;
