
/*! \file IFOInterface.h
* \brief	IFO，ISO信息获取接口
*/

#pragma once
#include <scombase.h>
#include "COMMacro.h"

/*!
* \class IIFOInfo
* \brief 获取IFO信息，该接口通过查询IMediaInfo接口获取。 
*/


// {5D71E358-66C1-4248-BB77-EB39F910E1FC}
DEFINE_GUID(IID_IIFOInfo, 
			0x5d71e358, 0x66c1, 0x4248, 0xbb, 0x77, 0xeb, 0x39, 0xf9, 0x10, 0xe1, 0xfc);
EXTERN_C const IID IID_IIFOInfo;
MACRO_MIDL_INTERFACE(IIFOInfo, "5D71E358-66C1-4248-BB77-EB39F910E1FC")
: public IUnknown
{
public:

	/*! \fn virtual STDMETHODIMP_(int)    GetHaveCSSProtect()
	* \brief 获取影碟是否有CSS保护
	* \retval  -1 表示 不确定, 0 表示 没有, 1 表示 有
	*/
	virtual STDMETHODIMP_(int) GetHaveCSSProtect() = 0;

	/*! \fn virtual STDMETHODIMP_(int)    GetTitleCount()
	* \brief 获取IFO中Title的数目
	* \retval Title的数目
	*/
	virtual STDMETHODIMP_(int) GetTitleCount() = 0;

	/*! \fn virtual STDMETHODIMP_(int)    GetChapterCount(int nTitleIndex)
	* \brief 获取指定的Title章节数
	* \retval Title的章节数
	*/
	virtual STDMETHODIMP_(int) GetChapterCount(int nTitleIndex) = 0;

	/*! \fn virtual STDMETHODIMP_(double) GetTitleBeginTime(int nTitleIndex)
	* \brief 获取指定的Title起始时间
	* \param nTitleIndex [in] Title的索引号
	* \retval Title的起始时间
	*/
	virtual STDMETHODIMP_(double) GetTitleBeginTime(int nTitleIndex) = 0;

	/*! \fn virtual STDMETHODIMP_(double) GetTitleLength(int nTitleIndex)
	* \brief 获取指定的Title播放时长
	* \param nTitleIndex [in] Title的索引号
	* \retval Title的播放时长
	*/
	virtual STDMETHODIMP_(double) GetTitleLength(int nTitleIndex) = 0;

	/*! \fn virtual STDMETHODIMP_(double) GetChapterBeginTime(int nTitleIndex, int nChapterIndex)
	* \brief 获取指定的章节起始时间
	* \param nTitleIndex [in] Title的索引号
	* \param nChapterIndex [in] 章节的索引号
	* \retval 章节起始时间
	*/
	virtual STDMETHODIMP_(double) GetChapterBeginTime(int nTitleIndex, int nChapterIndex) = 0;

	/*! \fn virtual STDMETHODIMP_(double) GetChapterLength(int nTitleIndex, int nChapterIndex)
	* \brief 获取指定的章节播放时长
	* \param nTitleIndex [in] Title的索引号
	* \param nChapterIndex [in] 章节的索引号
	* \retval 章节的播放时长
	*/
	virtual STDMETHODIMP_(double) GetChapterLength(int nTitleIndex, int nChapterIndex) = 0;

	/*! \fn virtual STDMETHODIMP_(int)    GetTitleAudioTrackCount(int nTitleIndex)
	* \brief 获取指定的Title音轨数
	* \param nTitleIndex [in] Title的索引号
	* \retval Title的音轨数
	*/
	virtual STDMETHODIMP_(int) GetTitleAudioTrackCount(int nTitleIndex) = 0;

	/*! \fn virtual STDMETHODIMP		  GetAudioTrackDesc(int nTitleIndex, int nAudioTrackidx, BSTR *ppDescription)
	* \brief 获取指定音轨的描述信息
	* \param nTitleIndex [in] Title的索引号
	* \param nAudioTrackidx [in] 音轨的索引号
	* \param ppDescription [in / out] 音轨的描述信息
	* \retval 获取的结果
	*/
	virtual STDMETHODIMP GetAudioTrackDesc(int nTitleIndex, int nAudioTrackidx, BSTR *ppDescription) = 0;

	/*! \fn virtual STDMETHODIMP_(int)    GetTitleSubTitleCount(int nTitleIndex)
	* \brief 获取指定的Title字幕数
	* \param nTitleIndex [in] Title的索引号
	* \retval Title的字幕数
	*/
	virtual STDMETHODIMP_(int) GetTitleSubTitleCount(int nTitleIndex) = 0;

	/*! \fn virtual STDMETHODIMP		  GetSubTitleDesc(int nTitleIndex, int nSubTitleidx, BSTR *ppDescription)
	* \brief 获取指定字幕的描述信息
	* \param nTitleIndex [in] Title的索引号
	* \param nSubTitleidx [in] 字幕的索引号
	* \param ppDescription [in / out] 字幕的描述信息
	* \retval 获取的结果
	*/
	virtual STDMETHODIMP GetSubTitleDesc(int nTitleIndex, int nSubTitleidx, BSTR *ppDescription) = 0;

	/*! \fn virtual STDMETHODIMP_(WORD)   GetAudioTrackLang(int nTitleIndex, int nAudioTrackidx)
	* \brief 获取指定音轨的语言编号(ISO 639 language codes.)
	* \param nTitleIndex [in] Title的索引号
	* \param nAudioTrackidx [in] 音轨的索引号
	* \retval 语言编号
	*/
	virtual STDMETHODIMP_(WORD) GetAudioTrackLang(int nTitleIndex, int nAudioTrackidx) = 0;

	/*! \fn virtual STDMETHODIMP          GetAudioFormatDesc(int nTitleIndex, int nAudioTrackidx, BSTR *ppDescription)
	* \brief 获取指定音轨的音频编码("AC3", "MPEG1", "MPEG2x", "LPCM", "DTS")
	* \param nTitleIndex [in] Title的索引号
	* \param nAudioTrackidx [in] 音轨的索引号
	* \param ppDescription [in / out] 音频编码
	* \retval 获取结果
	*/
	virtual STDMETHODIMP GetAudioFormatDesc(int nTitleIndex, int nAudioTrackidx, BSTR *ppDescription) = 0;

	/*! \fn virtual STDMETHODIMP_(int)    GetAudioTrackID(int nTitleIndex, int nAudioTrackidx)
	* \brief 获取指定音轨的ID号(AC3 [0x80, 0x87]，  DTS [0x88, 0x8F]， MPEG Audio [0xC0, 0xCF], LPCM [0xA0, 0xAF])
	* \param nTitleIndex [in] Title的索引号
	* \param nAudioTrackidx [in] 音轨的索引号
	* \retval 音轨的ID号
	*/
	virtual STDMETHODIMP_(int) GetAudioTrackID(int nTitleIndex, int nAudioTrackidx) = 0;

	/*! \fn virtual STDMETHODIMP_(int)     GetAudioChannel(int nTitleIndex, int nAudioTrackidx)
	* \brief 获取指定音轨的声道数
	* \param nTitleIndex [in] Title的索引号
	* \param nAudioTrackidx [in] 音轨的索引号
	* \retval 音轨的声道数
	*/
	virtual STDMETHODIMP_(int) GetAudioChannel(int nTitleIndex, int nAudioTrackidx) = 0;

	/*! \fn virtual STDMETHODIMP_(WORD)	  GetSubLangCode(int nTitleIndex, int nSubTitleidx)
	* \brief 获取指定字幕的语言编号(ISO 639 language codes)
	* \param nTitleIndex [in] Title的索引号
	* \param nSubTitleidx [in] 字幕的索引号
	* \retval 字幕的语言编号
	*/
	virtual STDMETHODIMP_(WORD) GetSubLangCode(int nTitleIndex, int nSubTitleidx) = 0;

	/*! \fn virtual STDMETHODIMP_(int)	  GetSubTitleTrackID(int nTitleIndex, int nSubTitleidx)
	* \brief 获取指定字幕的ID号([0x20, 0x3F])
	* \param nTitleIndex [in] Title的索引号
	* \param nSubTitleidx [in] 字幕的索引号
	* \retval 字幕的ID号
	*/
	virtual STDMETHODIMP_(int) GetSubTitleTrackID(int nTitleIndex, int nSubTitleidx) = 0;

	/*! \fn virtual STDMETHODIMP_(int)     GetTitleAvailable(int nTitleIndex)
	* \brief 获取指定的Title是否是有效的
	* \param nTitleIndex [in] Title的索引号
	* \retval 0表示无效，1表示有效
	*/
	virtual STDMETHODIMP_(int) GetTitleAvailable(int nTitleIndex) = 0;
};

/*!
* \class IISOInfo
* \brief 获取ISO文件中的卷标名，以及IFO的数目 
*/

// {490F7DE8-FC4E-42af-9570-1433DB5FA847}
DEFINE_GUID(IID_ISOInfo, 
			0x490f7de8, 0xfc4e, 0x42af, 0x95, 0x70, 0x14, 0x33, 0xdb, 0x5f, 0xa8, 0x47);
EXTERN_C const IID IID_ISOInfo;
MACRO_MIDL_INTERFACE(IISOInfo, "490F7DE8-FC4E-42af-9570-1433DB5FA847")
: public IUnknown
{
public:

	/*! \fn virtual STDMETHODIMP_(int) GetIFOCount()
	* \brief 获取ISO中IFO文件的数目
	* \retval IFO文件的数目
	*/
	virtual STDMETHODIMP_(int) GetIFOCount() = 0;

	/*! \fn virtual STDMETHODIMP	   GetISOVolumeName(BSTR *ppVolumeName)
	* \brief 获取ISO的卷标
	* \param ppVolumeName [in / out] ISO的卷标
	* \retval ISO的卷标
	*/
	virtual STDMETHODIMP	   GetISOVolumeName(BSTR *ppVolumeName) = 0;	
};

/*!
* \class ICreateISOInfo
* \brief 通过ISO的文件名,创建IISOInfo接口实例
*/

// {B66AB703-15E7-4e39-94E9-CEA64BBD5534}
DEFINE_GUID(IID_ICreateISOInfo, 
			0xb66ab703, 0x15e7, 0x4e39, 0x94, 0xe9, 0xce, 0xa6, 0x4b, 0xbd, 0x55, 0x34);
EXTERN_C const IID IID_ICreateISOInfo;
MACRO_MIDL_INTERFACE(ICreateISOInfo, "B66AB703-15E7-4e39-94E9-CEA64BBD5534")
: public IUnknown
{
public:

	/*! \fn virtual STDMETHODIMP_(IISOInfo *) CreateISOInfo(const BSTR pISOPath)
	* \brief 通过ISO的文件名,创建IISOInfo接口实例
	* \param pISOPath [in] ISO文件路径
	* \retval IISOInfo接口实例
	* \see IISOInfo
	*/
	virtual STDMETHODIMP_(IISOInfo *) CreateISOInfo(const BSTR pISOPath) = 0;	
};

// {A13F85D0-420C-4cf0-BD56-B6C1F101A99F}
MIDL_DEFINE_GUID(CLSID, CLSID_CCreateISOInfo, 0xa13f85d0, 0x420c, 0x4cf0, 0xbd, 0x56, 0xb6, 0xc1, 0xf1, 0x1, 0xa9, 0x9f);
EXTERN_C const CLSID CLSID_CCreateISOInfo;