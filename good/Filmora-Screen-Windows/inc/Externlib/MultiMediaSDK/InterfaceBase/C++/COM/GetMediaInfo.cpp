#include "stdafx.h"
#include "GetMediaInfo.h"


CSubPicStreamInfo::CSubPicStreamInfo()
				  :m_pSubPicInfo(NULL)
{

}

CSubPicStreamInfo::~CSubPicStreamInfo()
{
	SAFE_DELETE(m_pSubPicInfo);
}

STDMETHODIMP_(SUPPIC_TYPE) CSubPicStreamInfo::GetType(void)
{
	CheckPointer(m_pSubPicInfo, SPT_Internal);

	return m_pSubPicInfo->uSPType;
}

STDMETHODIMP_(UINT) CSubPicStreamInfo::GetID(void)
{
	CheckPointer(m_pSubPicInfo, 0);

	return m_pSubPicInfo->uStreamID;
}

STDMETHODIMP_(UINT) CSubPicStreamInfo::GetFourCC(void)
{
	CheckPointer(m_pSubPicInfo, 0);

	return m_pSubPicInfo->dwFourCC;
}

STDMETHODIMP CSubPicStreamInfo::GetLanguage(BSTR * ppLanguage)
{
	CheckPointer(ppLanguage, E_POINTER);
	*ppLanguage = NULL;
	CheckPointer(m_pSubPicInfo, E_FAIL);

	*ppLanguage = SysAllocString(m_pSubPicInfo->szLang);
	return S_OK;
}

STDMETHODIMP CSubPicStreamInfo::GetDescription(BSTR * ppDescription) 
{
	CheckPointer(ppDescription, E_POINTER);
	*ppDescription = NULL;
	CheckPointer(m_pSubPicInfo, E_FAIL);

	*ppDescription = SysAllocString(m_pSubPicInfo->szDescription);

	return S_OK;
}

STDMETHODIMP CSubPicStreamInfo::SetSubPicStreamInfo(const SubPicStreamInfo *pSubPicStreamInfo)
{
	CheckPointer(pSubPicStreamInfo, E_POINTER);
	SAFE_DELETE(m_pSubPicInfo);
	m_pSubPicInfo = new SubPicStreamInfo;
	memcpy(m_pSubPicInfo, pSubPicStreamInfo, sizeof(SubPicStreamInfo));

	return S_OK;
}

STDMETHODIMP CSubPicStreamInfos::AddBySubPicStreamInfo(const SubPicStreamInfo *pSubPicStreamInf)
{
	CheckPointer(pSubPicStreamInf, E_POINTER);

	CSubPicStreamInfo *pSubPicStream = new CSubPicStreamInfo;
	pSubPicStream->SetSubPicStreamInfo(pSubPicStreamInf);
	Add(pSubPicStream);

	return S_OK;
}

STDMETHODIMP CStreamInfos::AddByStreamInfo(const StreamInfo *pStreamInfo)
{
	CheckPointer(pStreamInfo, E_POINTER);

	CStreamInfo *pStream = new CStreamInfo;
	pStream->SetStreamInfo(pStreamInfo);
	if (pStreamInfo->vidInfoSrc.width != 0 && pStreamInfo->vidInfoSrc.height != 0)
		pStream->SetVideoSrcInfo(&pStreamInfo->vidInfoSrc);
	Add(pStream);

	return S_OK;
}


void FreeProgramInfo(ProgramInfo *pProgramInfo)
{
	if (!pProgramInfo)
	{
		return;
	}

	SAFE_DELETE_ARRAY(pProgramInfo->pVidStreamInfos);
	SAFE_DELETE_ARRAY(pProgramInfo->pAudStreamInfos);
	SAFE_DELETE_ARRAY(pProgramInfo->pSubPicInfos);

	SAFE_DELETE(pProgramInfo);
}

ProgramInfo * CloneProgramInfo(const ProgramInfo *pProgramInfo)
{
	if (!pProgramInfo)
	{
		return NULL;
	}

	ProgramInfo *pClone = new ProgramInfo;
	memcpy(pClone, pProgramInfo, sizeof(ProgramInfo));
	if (pProgramInfo->nVideoStreamCount > 0)
	{
		pClone->pVidStreamInfos = new StreamInfo[pProgramInfo->nVideoStreamCount];
		memcpy(pClone->pVidStreamInfos, pProgramInfo->pVidStreamInfos, sizeof(StreamInfo) * pProgramInfo->nVideoStreamCount);
	}

	if (pProgramInfo->nAudioStreamCount > 0)
	{
		pClone->pAudStreamInfos = new StreamInfo[pProgramInfo->nAudioStreamCount];
		memcpy(pClone->pAudStreamInfos, pProgramInfo->pAudStreamInfos, sizeof(StreamInfo) * pProgramInfo->nAudioStreamCount);
	}

	if (pProgramInfo->nSubPicStreamCount > 0)
	{
		pClone->pSubPicInfos = new SubPicStreamInfo[pProgramInfo->nSubPicStreamCount];
		memcpy(pClone->pSubPicInfos, pProgramInfo->pSubPicInfos, sizeof(SubPicStreamInfo) * pProgramInfo->nSubPicStreamCount);
	}

	return pClone;
}

CProgramInfo::CProgramInfo()
			 :m_pProgramInfo(NULL)
			 ,m_pVideoStreamInfos(NULL)
			 ,m_pAudioStreamInfos(NULL)
			 ,m_pSubPicStreamInfos(NULL)
{

}

CProgramInfo::~CProgramInfo()
{
	FreeProgramInfo(m_pProgramInfo);
	SAFE_RELEASE(m_pVideoStreamInfos);
	SAFE_RELEASE(m_pAudioStreamInfos);
	SAFE_RELEASE(m_pSubPicStreamInfos);
}

STDMETHODIMP_(UINT) CProgramInfo::GetNumber(void)
{
	CheckPointer(m_pProgramInfo, 0);

	return m_pProgramInfo->dwNumber;
}

STDMETHODIMP_(UINT) CProgramInfo::GetID(void)
{
	CheckPointer(m_pProgramInfo, 0);

	return m_pProgramInfo->uPID;
}

STDMETHODIMP_(double) CProgramInfo::GetMediaLength(void)
{
	CheckPointer(m_pProgramInfo, 0.0);

	return m_pProgramInfo->dMediaLength;
}

STDMETHODIMP_(int) CProgramInfo::GetBitrate()
{
	CheckPointer(m_pProgramInfo, 0);

	return m_pProgramInfo->nBitrate;
}

STDMETHODIMP_(UINT) CProgramInfo::GetTimeStampReferenceStreamID(void)
{
	CheckPointer(m_pProgramInfo, 0);

	return m_pProgramInfo->uTimeStampReferenceStreamID;
}

STDMETHODIMP CProgramInfo::GetVideoStreamInfos(IStreamInfos** ppVideoStreamInfos)
{
	CheckPointer(ppVideoStreamInfos, E_POINTER);
	*ppVideoStreamInfos = NULL;
	CheckPointer(m_pVideoStreamInfos, E_FAIL);

	*ppVideoStreamInfos = m_pVideoStreamInfos;
	m_pVideoStreamInfos->AddRef();

	return S_OK;
}

STDMETHODIMP CProgramInfo::GetAudioStreamInfos(IStreamInfos** ppAudioStreamInfos)
{
	CheckPointer(ppAudioStreamInfos, E_POINTER);
	*ppAudioStreamInfos = NULL;
	CheckPointer(m_pAudioStreamInfos, E_FAIL);

	*ppAudioStreamInfos = m_pAudioStreamInfos;
	m_pAudioStreamInfos->AddRef();

	return S_OK;
}

STDMETHODIMP CProgramInfo::GetSubPicStreamInfos(ISubPicStreamInfos** ppSubPicStreamInfos)
{
	CheckPointer(ppSubPicStreamInfos, E_POINTER);
	*ppSubPicStreamInfos = NULL;
	CheckPointer(m_pSubPicStreamInfos, E_FAIL);

	*ppSubPicStreamInfos = m_pSubPicStreamInfos;
	m_pSubPicStreamInfos->AddRef();

	return S_OK;
}

STDMETHODIMP CProgramInfo::SetProgramInfo(const ProgramInfo *pProgramInfo)
{
	FreeProgramInfo(m_pProgramInfo);
	m_pProgramInfo = CloneProgramInfo(pProgramInfo);
	
	SAFE_RELEASE(m_pVideoStreamInfos);
	if (pProgramInfo->nVideoStreamCount > 0)
	{
		m_pVideoStreamInfos = new CStreamInfos;
		m_pVideoStreamInfos->AddRef();
	}
	for (int nIndex = 0; nIndex < pProgramInfo->nVideoStreamCount; nIndex++)
	{
		m_pVideoStreamInfos->AddByStreamInfo(&(pProgramInfo->pVidStreamInfos[nIndex]));
	}

	SAFE_RELEASE(m_pAudioStreamInfos);
	if (pProgramInfo->nAudioStreamCount > 0)
	{
		m_pAudioStreamInfos = new CStreamInfos;
		m_pAudioStreamInfos->AddRef();
	}
	for (int nIndex = 0; nIndex < pProgramInfo->nAudioStreamCount; nIndex++)
	{
		m_pAudioStreamInfos->AddByStreamInfo(&(pProgramInfo->pAudStreamInfos[nIndex]));
	}

	SAFE_RELEASE(m_pSubPicStreamInfos);
	if (pProgramInfo->nSubPicStreamCount > 0)
	{
		m_pSubPicStreamInfos = new CSubPicStreamInfos;
		m_pSubPicStreamInfos->AddRef();
	}
	for (int nIndex = 0; nIndex < pProgramInfo->nSubPicStreamCount; nIndex++)
	{
		m_pSubPicStreamInfos->AddBySubPicStreamInfo(&(pProgramInfo->pSubPicInfos[nIndex]));
	}

	return S_OK;
}


STDMETHODIMP CProgramInfos::AddByMediaInfo(const MEDIA_FILE_INFO *pMediaInfo)
{
	CheckPointer(pMediaInfo, E_POINTER);
	for (int nProgramIndex = 0; nProgramIndex < pMediaInfo->nProgramCount; nProgramIndex++)
	{
		CProgramInfo *pProgramInfo = new CProgramInfo;
		pProgramInfo->SetProgramInfo(&(pMediaInfo->pProgramInfos[nProgramIndex]));
		Add(pProgramInfo);
	}

	return S_OK;
}



CMediaInfo::CMediaInfo()
			:m_pMediaInfo(NULL)
			,m_pProgramInfos(NULL)
{
	
}

CMediaInfo::~CMediaInfo()
{
	if (m_pMediaInfo)
	{
		MFIRelease(m_pMediaInfo);
		m_pMediaInfo = NULL;
	}
	
	SAFE_RELEASE(m_pProgramInfos);
}


STDMETHODIMP_(UINT) CMediaInfo::GetFourCC()
{
	CheckPointer(m_pMediaInfo, E_FAIL);

	return m_pMediaInfo->dwFourCC;
}

STDMETHODIMP CMediaInfo::GetFileName(BSTR *pFileName)
{
	CheckPointer(pFileName, E_POINTER);
	CheckPointer(m_pMediaInfo, E_FAIL);
	
	*pFileName = SysAllocString(m_pMediaInfo->szName);

	return S_OK;
}
STDMETHODIMP CMediaInfo::GetDescription(BSTR * ppDescription)
{
	CheckPointer(ppDescription, E_POINTER);
	CheckPointer(m_pMediaInfo, E_FAIL);

	*ppDescription = SysAllocString(m_pMediaInfo->szDescription);

	return S_OK;
}

STDMETHODIMP_(double) CMediaInfo::GetMediaLength(void)
{
	CheckPointer(m_pMediaInfo, 0.0);

	return m_pMediaInfo->dMediaLength;
}

STDMETHODIMP_(int) CMediaInfo::GetBitrate(void)
{
	CheckPointer(m_pMediaInfo, 0);

	return m_pMediaInfo->nBitrate;
}


STDMETHODIMP CMediaInfo::GetIPrograms(IProgramInfos **ppProgramInfos)
{
	CheckPointer(ppProgramInfos, E_POINTER);
	*ppProgramInfos = NULL;
	CheckPointer(m_pMediaInfo, E_FAIL);
	CheckPointer(m_pProgramInfos, E_FAIL);

	*ppProgramInfos = m_pProgramInfos;
	m_pProgramInfos->AddRef();

	return S_OK;
}

STDMETHODIMP_(AV_SCAN_TYPE) CMediaInfo::GetScanType()
{
	return m_pMediaInfo->scanType;
}

STDMETHODIMP CMediaInfo::GetArtist(BSTR *pArtist)
{
	CheckPointer(pArtist, E_FAIL);
	CheckPointer(m_pMediaInfo, E_FAIL);

	*pArtist = SysAllocString(m_pMediaInfo->metaDataInfo.szArtist);

	return S_OK;
}

STDMETHODIMP CMediaInfo::GetTitle(BSTR *pTitle)
{
	CheckPointer(pTitle, E_FAIL);
	CheckPointer(m_pMediaInfo, E_FAIL);

	*pTitle = SysAllocString(m_pMediaInfo->metaDataInfo.szTitle);

	return S_OK;
}

STDMETHODIMP CMediaInfo::GetTrackNumber(BSTR *pTrackNumber)
{
	CheckPointer(pTrackNumber, E_FAIL);
	CheckPointer(m_pMediaInfo, E_FAIL);

	*pTrackNumber = SysAllocString(m_pMediaInfo->metaDataInfo.szTrackNumber);

	return S_OK;
}

STDMETHODIMP CMediaInfo::GetAlbum(BSTR *pAlbum)
{
	CheckPointer(pAlbum, E_FAIL);
	CheckPointer(m_pMediaInfo, E_FAIL);

	*pAlbum = SysAllocString(m_pMediaInfo->metaDataInfo.szAlbum);

	return S_OK;
}

STDMETHODIMP CMediaInfo::GetDate(BSTR *pDate)
{
	CheckPointer(pDate, E_FAIL);
	CheckPointer(m_pMediaInfo, E_FAIL);

	*pDate = SysAllocString(m_pMediaInfo->metaDataInfo.szDate);

	return S_OK;
}

STDMETHODIMP CMediaInfo::GetGenre(BSTR *pGenre)
{
	CheckPointer(pGenre, E_FAIL);
	CheckPointer(m_pMediaInfo, E_FAIL);

	*pGenre = SysAllocString(m_pMediaInfo->metaDataInfo.szGenre);

	return S_OK;
}

STDMETHODIMP CMediaInfo::GetPublisher(BSTR *pPublisher)
{
	CheckPointer(pPublisher, E_FAIL);
	CheckPointer(m_pMediaInfo, E_FAIL);

	*pPublisher = SysAllocString(m_pMediaInfo->metaDataInfo.szPublisher);

	return S_OK;
}
void STDMETHODCALLTYPE CMediaInfo::Fill(const MEDIA_FILE_INFO *pMediaInfo)
{
	if (!pMediaInfo)
	{
		return;
	}

	if (m_pMediaInfo)
	{
		MFIRelease(m_pMediaInfo);
	}
	
	m_pMediaInfo = MFIClone(pMediaInfo);
	SAFE_RELEASE(m_pProgramInfos);
	m_pProgramInfos = new CProgramInfos;
	m_pProgramInfos->AddRef();
	m_pProgramInfos->AddByMediaInfo(pMediaInfo);
}