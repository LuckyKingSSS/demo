#pragma once

#include "FileInfo.h"
#include "CommonInterface.h"

class CNLEStreamInfo : public scombase::CUnknown, public IStreamInfo, public IStreamInfoSrc
{
public:
    CNLEStreamInfo()
		:m_pStreamInfo(NULL)
		, m_pVideoInfoSrc(NULL)
	{
	}

    ~CNLEStreamInfo()
	{
		SAFE_DELETE(m_pStreamInfo);
		SAFE_DELETE(m_pVideoInfoSrc);
	}

public:
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv)
	{
		if ( riid==__uuidof(IStreamInfo))
			return scombase::GetInterface((IStreamInfo *)this, ppv);
		else if (riid == __uuidof(IStreamInfoSrc))
			return scombase::GetInterface((IStreamInfoSrc *)this, ppv);
		else
			return CUnknown::NonDelegatingQueryInterface(riid, ppv);
	};
	DECLARE_IUNKNOWN

public:
	virtual STDMETHODIMP_(AV_STREAM_TYPE) GetType(void)
	{
		CheckPointer(m_pStreamInfo, ST_NONE);

		return m_pStreamInfo->uStreamType;
	}
	virtual STDMETHODIMP_(UINT) GetID(void)
	{
		CheckPointer(m_pStreamInfo, 0);

		return m_pStreamInfo->uStreamID;
	}

	virtual STDMETHODIMP_(UINT) GetFourCC(void)
	{
		CheckPointer(m_pStreamInfo, 0);

		return m_pStreamInfo->dwFourCC;
	}

	virtual STDMETHODIMP_(int) GetState(void)
	{
		CheckPointer(m_pStreamInfo, -1);

		return m_pStreamInfo->nState;
	}

	virtual STDMETHODIMP GetName(BSTR * ppStreamName)
	{
		CheckPointer(ppStreamName, E_POINTER);
		CheckPointer(m_pStreamInfo, E_FAIL);
		*ppStreamName = SysAllocString(m_pStreamInfo->szName);

		return S_OK;
	}

	virtual STDMETHODIMP GetDescription(BSTR * ppDescription)
	{
		CheckPointer(ppDescription, E_POINTER);
		CheckPointer(m_pStreamInfo, E_FAIL);
		*ppDescription = SysAllocString(m_pStreamInfo->szDescription);

		return S_OK;
	}

	virtual STDMETHODIMP_(double) GetLength(void)
	{
		CheckPointer(m_pStreamInfo, 0.0);

		return m_pStreamInfo->dStreamLength;
	}

	virtual STDMETHODIMP_(int) GetBitRate(void)
	{
		CheckPointer(m_pStreamInfo, 0);

		return m_pStreamInfo->nBitrate;
	}

	virtual STDMETHODIMP_(const WSAudioInfo *) GetAudioInfo(void)
	{
		CheckPointer(m_pStreamInfo, NULL);

		return &(m_pStreamInfo->audInfo);
	}

	virtual STDMETHODIMP_(const WSVideoInfo *) GetVideoInfo(void)
	{
		CheckPointer(m_pStreamInfo, NULL);

		return &(m_pStreamInfo->vidInfo);
	}

	virtual STDMETHODIMP_(const WSVideoInfo *) GetVideoSrcInfo(void)
	{
		CheckPointer(m_pVideoInfoSrc, NULL);

		return m_pVideoInfoSrc;
	}

	virtual STDMETHODIMP_(__int64) GetStartPTS(void)
	{
		CheckPointer(m_pStreamInfo, 0);

		return m_pStreamInfo->ptsStart;
	}

	virtual STDMETHODIMP_(__int64) GetPTSOffset(void)
	{
		CheckPointer(m_pStreamInfo, 0);

		return m_pStreamInfo->ptsOffset;
	}


	virtual STDMETHODIMP_(AV_SCAN_TYPE) GetScanType()
	{
		CheckPointer(m_pStreamInfo, AV_SCAN_TYPE_UNKNOWN);

		return m_pStreamInfo->scanType;
	}

	virtual STDMETHODIMP_(AV_SCAN_TYPE) GetSrcScanType()
	{
		CheckPointer(m_pStreamInfo, AV_SCAN_TYPE_UNKNOWN);

		return m_pStreamInfo->scanTypeSrc;
	}
	virtual STDMETHODIMP_(int) GetSrcRotate()
	{
		CheckPointer(m_pStreamInfo, 0);

		return m_pStreamInfo->dwReserved1;
	}
	virtual STDMETHODIMP SetStreamInfo(const StreamInfo *pStreamInfo)
	{
		CheckPointer(pStreamInfo, E_POINTER);

		SAFE_DELETE(m_pStreamInfo);
		m_pStreamInfo = new StreamInfo;
		memcpy(m_pStreamInfo, pStreamInfo, sizeof(StreamInfo));

		SAFE_DELETE(m_pVideoInfoSrc);
		m_pVideoInfoSrc = new WSVideoInfo;
		memcpy(m_pVideoInfoSrc, &pStreamInfo->vidInfo, sizeof(WSVideoInfo));

		return S_OK;
	}
	virtual STDMETHODIMP SetVideoSrcInfo(const WSVideoInfo *pVideoSrcInfo)
	{
		CheckPointer(pVideoSrcInfo, E_POINTER);

		SAFE_DELETE(m_pVideoInfoSrc);
		m_pVideoInfoSrc = new WSVideoInfo;
		memcpy(m_pVideoInfoSrc, pVideoSrcInfo, sizeof(WSVideoInfo));

		return S_OK;
	}
	STDMETHODIMP_(StreamInfo *) GetStreamInfo(void)
	{
		CheckPointer(m_pStreamInfo, 0);
		return m_pStreamInfo;
	}

private:
	StreamInfo *m_pStreamInfo;
	WSVideoInfo *m_pVideoInfoSrc;
};
