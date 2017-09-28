#pragma once

#include "DecMgr.h"
#include "FileInfo.h"
#include "CommonClass.h"

class CSubPicStreamInfo : public scombase::CUnknown, public ISubPicStreamInfo
{
public:
	CSubPicStreamInfo();
	virtual ~CSubPicStreamInfo();

public:
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv)
	{
		if ( riid==__uuidof(ISubPicStreamInfo))
			return scombase::GetInterface((ISubPicStreamInfo *)this, ppv);
		else
			return scombase::CUnknown::NonDelegatingQueryInterface(riid, ppv);
	};
	DECLARE_IUNKNOWN

public:
	virtual STDMETHODIMP_(SUPPIC_TYPE) GetType(void);
	virtual STDMETHODIMP_(UINT) GetID(void);
	virtual STDMETHODIMP_(UINT) GetFourCC(void);
	virtual STDMETHODIMP GetLanguage(BSTR * ppLanguage);
	virtual STDMETHODIMP GetDescription(BSTR * ppDescription) ;

	virtual STDMETHODIMP SetSubPicStreamInfo(const SubPicStreamInfo *pSubPicStreamInfo);

public:
	SubPicStreamInfo *m_pSubPicInfo;
};

class CSubPicStreamInfos : public scombase::CUnknownList<CSubPicStreamInfo, ISubPicStreamInfo, ISubPicStreamInfos>
{
public:
	virtual STDMETHODIMP AddBySubPicStreamInfo(const SubPicStreamInfo *pSubPicStreamInf);
};


class CStreamInfos : public scombase::CUnknownList<CStreamInfo, IStreamInfo, IStreamInfos>
{
public:
	virtual STDMETHODIMP AddByStreamInfo(const StreamInfo *pStreamInfo);
};

class CProgramInfo : public scombase::CUnknown, public IProgramInfo
{
public:
	CProgramInfo();
	virtual ~CProgramInfo();

public:
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv)
	{
		if ( riid==__uuidof(IProgramInfo))
			return scombase::GetInterface((IProgramInfo *)this, ppv);
		else
			return scombase::CUnknown::NonDelegatingQueryInterface(riid, ppv);
	};
	DECLARE_IUNKNOWN

public:
	virtual STDMETHODIMP_(UINT) GetNumber(void);
	virtual STDMETHODIMP_(UINT) GetID(void);
	virtual STDMETHODIMP_(double) GetMediaLength(void);
	virtual STDMETHODIMP_(int)	  GetBitrate();	
	virtual STDMETHODIMP_(UINT) GetTimeStampReferenceStreamID(void);
	virtual STDMETHODIMP GetVideoStreamInfos(IStreamInfos** ppVideoStreamInfos);
	virtual STDMETHODIMP GetAudioStreamInfos(IStreamInfos** ppAudioStreamInfos);
	virtual STDMETHODIMP GetSubPicStreamInfos(ISubPicStreamInfos** ppSubPicStreamInfos);

	virtual STDMETHODIMP SetProgramInfo(const ProgramInfo *pProgramInfo);

private:
	ProgramInfo *m_pProgramInfo;
	CStreamInfos *m_pVideoStreamInfos;
	CStreamInfos *m_pAudioStreamInfos;
	CSubPicStreamInfos *m_pSubPicStreamInfos;
};



class CProgramInfos: public scombase::CUnknownList<CProgramInfo, IProgramInfo, IProgramInfos> 
{
public:
	virtual STDMETHODIMP AddByMediaInfo(const MEDIA_FILE_INFO *pMediaInfo);
};

class CMediaInfo: public scombase::CUnknown, public IMediaInfo,public IMetaDataInfo
{
public:
	CMediaInfo();
	virtual ~CMediaInfo();

public:
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv)
	{
		if ( riid==__uuidof(IMediaInfo))
			return scombase::GetInterface((IMediaInfo *)this, ppv);
		else if ( riid==__uuidof(IMetaDataInfo))
			return scombase::GetInterface((IMetaDataInfo *)this, ppv);
		else
			return scombase::CUnknown::NonDelegatingQueryInterface(riid, ppv);
	};
	DECLARE_IUNKNOWN

public:
	//IMediaInfo
	virtual STDMETHODIMP_(UINT) GetFourCC();
	virtual STDMETHODIMP GetFileName(BSTR * pFileName);
	virtual STDMETHODIMP GetDescription(BSTR * ppDescription);
	virtual STDMETHODIMP_(double) GetMediaLength(void);
	virtual STDMETHODIMP_(int) GetBitrate(void);
	virtual STDMETHODIMP GetIPrograms(IProgramInfos **ppProgramInfos);
	virtual STDMETHODIMP_(AV_SCAN_TYPE) GetScanType();

	// IMetaDataInfo
	virtual STDMETHODIMP GetArtist(BSTR *pArtist);
	virtual STDMETHODIMP GetTitle(BSTR *pTitle);
	virtual STDMETHODIMP GetTrackNumber(BSTR *pTrackNumber);
	virtual STDMETHODIMP GetAlbum(BSTR *pAlbum);
	virtual STDMETHODIMP GetDate(BSTR *pDate);
	virtual STDMETHODIMP GetGenre(BSTR *pGenre);
	virtual STDMETHODIMP GetPublisher(BSTR *pPublisher);
	//
	void	STDMETHODCALLTYPE	  Fill(const MEDIA_FILE_INFO *pMediaInfo);

private:
    void ReleaseProgramInfo();	

protected:
	MEDIA_FILE_INFO *m_pMediaInfo;
	CProgramInfos	*m_pProgramInfos; 
};