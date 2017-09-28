////////////////////////////////////////////////////////
// DSound.h
//
// direct sound audio output device
////////////////////////////////////////////////////////

#ifndef _DIRECTSOUND_OUT_DEVICE_H__INCLUDE_
#define _DIRECTSOUND_OUT_DEVICE_H__INCLUDE_

#ifndef DEFAULT_BUFFER_LENGTH
#define DEFAULT_BUFFER_LENGTH	1000
#endif
#include <mmsystem.h>
#include <dsound.h>
#include <assert.h>
#pragma comment(lib, "dsound.lib")
//#include <xTraceU.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(i) {if (i) i->Release(); i = NULL;}
#endif

class CDirectSound
{
 public:

    virtual ~CDirectSound();
    CDirectSound();

	BOOL Create(int nSamplesPerSec, int nChannels, int nBitsPerSample);
	void Clear();

    DWORD  Write(LPBYTE lpData, DWORD dwBytes);

    void Stop();
    void Play();

	DWORD GetPlayTime(); // milli-second
	BOOL IsPlaying() { return m_bPlaying;}
	
	DWORD  GetBufferSize() { return m_dwBufferSize;}
	DWORD  GetFreeBufferSize();


 protected:
    DWORD  _Write(LPBYTE lpData, DWORD dwBytes);

    LPDIRECTSOUND       m_pDS;   // directsound device
    LPDIRECTSOUNDBUFFER m_pDSB;  // directsound buffer

    DWORD				m_dwBufferSize;        // length of sound buffer in bytes
	DWORD				m_dwWriteCursor;
	__int64				m_qWriteBytes;
	DWORD				m_dwBytesPerSec;


	BOOL				m_bPlaying;

};

inline CDirectSound::~CDirectSound()
{
	Clear();
}

inline CDirectSound::CDirectSound()
: m_pDS(NULL)
, m_pDSB(NULL)
, m_dwBufferSize(0)
, m_dwWriteCursor(0)
, m_bPlaying(FALSE)
, m_qWriteBytes(0)
, m_dwBytesPerSec(0)
{
}

inline BOOL CDirectSound::Create(int nSamplesPerSec, int nChannels, int nBitsPerSample)
{
	assert(NULL == m_pDSB);
	m_dwBufferSize = nSamplesPerSec * nChannels * (nBitsPerSample >> 3) * DEFAULT_BUFFER_LENGTH / 1000;

    HRESULT hr = DS_OK;
	hr = DirectSoundCreate (NULL,&m_pDS,NULL);
	if (DS_OK != hr) {
		return FALSE;
	}
    
	HWND hWnd = GetDesktopWindow();
	hr = m_pDS->SetCooperativeLevel (hWnd, DSSCL_NORMAL);

	m_dwBytesPerSec = nSamplesPerSec * nChannels * (nBitsPerSample >> 3);

	WAVEFORMATEX wfx = {sizeof(WAVEFORMATEX)};
	LPWAVEFORMATEX lpWF = &wfx;
    lpWF->wFormatTag      = WAVE_FORMAT_PCM;
    lpWF->nSamplesPerSec  = nSamplesPerSec;
    lpWF->nChannels       = nChannels;
    lpWF->wBitsPerSample  = nBitsPerSample;        // for PCM 8 or 16
    lpWF->nBlockAlign     = (lpWF->wBitsPerSample * lpWF->nChannels) >> 3;
    lpWF->nAvgBytesPerSec = m_dwBytesPerSec; //lpWF->nSamplesPerSec * lpWF->nBlockAlign;

    DSBUFFERDESC        dSBD = { sizeof (DSBUFFERDESC)};  // directsound buffer description

    dSBD.dwBufferBytes = m_dwBufferSize;
    dSBD.lpwfxFormat   = lpWF;
	dSBD.dwFlags       =		DSBCAPS_CTRLPAN | 
								DSBCAPS_CTRLVOLUME | 
								DSBCAPS_CTRLFREQUENCY |
								DSBCAPS_GETCURRENTPOSITION2 |
								DSBCAPS_STICKYFOCUS |
								DSBCAPS_GLOBALFOCUS ;

    hr = m_pDS->CreateSoundBuffer (&dSBD, &m_pDSB, NULL);
    if (DS_OK != hr) {
		SAFE_RELEASE(m_pDS);
        return FALSE;
	}


	return TRUE;
}

inline void CDirectSound::Clear()
{
	SAFE_RELEASE(m_pDSB);
	SAFE_RELEASE(m_pDS);

	m_dwBufferSize  = 0;
	m_dwWriteCursor = 0;
	m_bPlaying     = FALSE;
}
	
inline DWORD CDirectSound::GetPlayTime() // milli-second
{
	if (!m_pDSB)
		return 0;

	DWORD dwCurrentPlayCursor = 0;
	DWORD dwCurrentWriteCursor = 0;  
	HRESULT hr = m_pDSB->GetCurrentPosition(&dwCurrentPlayCursor, &dwCurrentWriteCursor);


	DWORD dwPlayTime = 0;
	
	if (m_dwWriteCursor > dwCurrentPlayCursor)
	{
		dwPlayTime = (DWORD)((m_qWriteBytes - m_dwWriteCursor + dwCurrentPlayCursor) * 1000 / m_dwBytesPerSec + 0.5);
	}
	else
	{
		dwPlayTime = (DWORD)((m_qWriteBytes - m_dwBufferSize + dwCurrentPlayCursor - m_dwWriteCursor) * 1000 / m_dwBytesPerSec + 0.5);
	}
	

	return dwPlayTime;
}
	
inline DWORD  CDirectSound::GetFreeBufferSize()
{
	if (!m_pDSB)
		return 0;

	DWORD dwCurrentPlayCursor = 0;
	DWORD dwCurrentWriteCursor = 0;  
	HRESULT hr = m_pDSB->GetCurrentPosition(&dwCurrentPlayCursor, &dwCurrentWriteCursor);

	DWORD dwFreeSize = m_dwBufferSize;

	if (m_qWriteBytes > 0)
	{
		if (m_dwWriteCursor > dwCurrentPlayCursor)
		{
			dwFreeSize = m_dwBufferSize - (m_dwWriteCursor - dwCurrentPlayCursor);
		}
		else
		{
			dwFreeSize = dwCurrentPlayCursor - m_dwWriteCursor;
		}
	}

//	XTRACE(L"GCP: %d %d %d %d\n", dwCurrentPlayCursor, dwCurrentWriteCursor, m_dwWriteCursor, dwFreeSize);
	return dwFreeSize;
	
}

inline void CDirectSound::Stop()
{
	assert(m_pDSB);
	if (m_pDSB)
		m_pDSB->Stop ();
	m_bPlaying		= FALSE;
}
    
inline void CDirectSound::Play()
{
	if (m_bPlaying)
		return;
	assert(m_pDSB);

	if (m_pDSB)
		m_pDSB->Play (0, 0, DSBPLAY_LOOPING);
	m_bPlaying		= TRUE;
//	XTRACE("CDirectSound::Play()\n");

	DWORD dwCurrentPlayCursor = 0;
	DWORD dwCurrentWriteCursor = 0;  
	HRESULT hr = m_pDSB->GetCurrentPosition(&dwCurrentPlayCursor, &dwCurrentWriteCursor);

}

inline DWORD CDirectSound::Write(LPBYTE lpData, DWORD dwBytes)
{
	assert(lpData && dwBytes);
	if (!(lpData && dwBytes>0))
		return 0;

	if (dwBytes < GetFreeBufferSize())
		return _Write(lpData, dwBytes);

	DWORD dwWrited = 0;
	LPBYTE pTmp = lpData;

	int nRest = dwBytes;
	int nOnce = m_dwBufferSize / 2;
	while(nRest)
	{
		if (nOnce > nRest)
			nOnce = nRest;
		nOnce =  _Write(pTmp, nOnce);
		dwWrited += nOnce;
		pTmp += nOnce;
		nRest -= nOnce;
	}

	return dwWrited;
	
}

inline DWORD CDirectSound::_Write(LPBYTE lpData, DWORD dwBytes)
{
	assert(dwBytes < m_dwBufferSize * 3 / 4);
	while (dwBytes > GetFreeBufferSize())
	{
		Sleep(1);
	}

    DWORD  len1 = 0, len2 = 0;
    LPBYTE lpbuf1,lpbuf2;
    HRESULT hr = m_pDSB->Lock (
                      m_dwWriteCursor,
                      dwBytes,
                      (void **)&lpbuf1, &len1,
                      (void **)&lpbuf2, &len2, 
					  0 // DSBLOCK_FROMWRITECURSOR 
					  );
    if (DS_OK != hr) {
        return 0;
    }

	if (len2 == 0)
	{
		assert(dwBytes == len1);
		memcpy(lpbuf1, lpData, dwBytes);
		m_dwWriteCursor += dwBytes;
		m_qWriteBytes += dwBytes;
	}
	else
	{
		assert(dwBytes - len1 == len2);
		memcpy(lpbuf1, lpData, len1);
		memcpy(lpbuf2, lpData + len1, dwBytes - len1);
		m_dwWriteCursor = len2;
		m_qWriteBytes += dwBytes;
	}

    hr = m_pDSB->Unlock (lpbuf1,len1,lpbuf2,len2);

	if (!m_bPlaying)
	{
		if (m_dwWriteCursor > (m_dwBufferSize * 1 / 2))
			Play();
	}
	
	m_dwWriteCursor %= m_dwBufferSize;

	return dwBytes;
}


#endif //#define _DIRECTSOUND_OUT_DEVICE_H__INCLUDE_
