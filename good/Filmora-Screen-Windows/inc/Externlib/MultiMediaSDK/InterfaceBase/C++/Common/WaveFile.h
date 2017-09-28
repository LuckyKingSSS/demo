// WaveFile.h : header file

#pragma once
#ifndef WIN32
#include "Linux32_Typedef.h"
#endif
#include "FileEx.h"

static  unsigned short bswap_16(unsigned short x)
{
    return (x>>8) | (x<<8);
}

static unsigned int bswap_32(unsigned int x)
{
    x= ((x<<8)&0xFF00FF00) | ((x>>8)&0x00FF00FF);
    return (x>>16) | (x<<16);
}

#ifdef ARCH_PPC
#define be2me_16(x) bswap_16(x)
#define be2me_32(x) bswap_32(x)
#else
#define be2me_16(x) (x)
#define be2me_32(x) (x)
#endif

class WaveFile
{
public:
    WaveFile() : m_nDataSize(0) {}
    ~WaveFile() { Close(); }

	void Close()
	{
		if (m_File.Handle())
		{
			if (m_nDataSize > 0)
			{
				__int64 iPos = m_File.Seek(4, SEEK_SET);
				
				int nFileSize = m_nDataSize + m_nWavHdrSize;
				DWORD dwWrited = 0;
				int nFileSizeReverse = be2me_32(nFileSize);
				dwWrited = m_File.Write( &nFileSizeReverse, sizeof(nFileSize));  
				iPos = m_File.Seek(m_nWavHdrSize - 4, SEEK_SET);
				DWORD dwDataSizeReverse = be2me_32(m_nDataSize);
				dwWrited = m_File.Write( &dwDataSizeReverse, sizeof(m_nDataSize));  
			}

			m_File.Close();
		}

		m_nDataSize = 0;
	}

	BOOL Open(LPCTSTR lpszPathName)
	{
		Close();

		USES_CONVERSION;
		
		return m_File.Open(T2W(lpszPathName), "wb");
	}

	DWORD WriteData(BYTE* pData, DWORD dwSize)
	{
		if (0 == pData || 0 == dwSize)
			return 0;

		DWORD dwWrited = 0;
		dwWrited = m_File.Write(pData, dwSize);  

		m_nDataSize += dwWrited;

		return dwWrited;
	}

	BOOL WriteHeader(LPWAVEFORMATEX lpWfx)
	{
		if (0 == lpWfx)
			return FALSE;

        m_nWavHdrSize = 44;
		DWORD	m_dwDataSize = 0;
		DWORD   m_WaveHeaderSize   =   38;   
		DWORD   m_WaveFormatSize   =   16;  

		DWORD dwWrited = 0;
		
		m_File.Seek(0, SEEK_SET);   
		m_File.Write("RIFF", 4);   
		unsigned   int   fileSize = 0;
		unsigned   int   fileSizeReverse = be2me_32(fileSize);   
		m_File.Write(&fileSizeReverse, sizeof(fileSize));   
		m_File.Write("WAVE", 4);   
		m_File.Write("fmt ", 4); 
		DWORD dwWaveFormatSize = be2me_32(m_WaveFormatSize);   
		m_File.Write(&dwWaveFormatSize, sizeof(m_WaveFormatSize));
		WORD  wFormatTagReverse = be2me_16(lpWfx->wFormatTag);  
		m_File.Write(&wFormatTagReverse, sizeof(lpWfx->wFormatTag)); 
		WORD nChannelsReverse = be2me_16(lpWfx->nChannels);  
		m_File.Write(&nChannelsReverse, sizeof(lpWfx->nChannels));  
		DWORD dwnSamplesPerSec = be2me_32(lpWfx->nSamplesPerSec); 
		m_File.Write(&dwnSamplesPerSec, sizeof(lpWfx->nSamplesPerSec)); 
		DWORD dwnAvgBytesPerSec = be2me_32(lpWfx->nAvgBytesPerSec);  
		m_File.Write(&dwnAvgBytesPerSec, sizeof(lpWfx->nAvgBytesPerSec));
		WORD nBlockAlignReverse = be2me_16(lpWfx->nBlockAlign);   
		m_File.Write(&nBlockAlignReverse, sizeof(lpWfx->nBlockAlign)); 
		WORD wBitsPerSampleReverse = be2me_16(lpWfx->wBitsPerSample);  
		m_File.Write(&wBitsPerSampleReverse, sizeof(lpWfx->wBitsPerSample)); 
        if (lpWfx->cbSize)
        {
            WORD wSize = be2me_16(lpWfx->cbSize);
            m_File.Write(&wSize, sizeof(lpWfx->cbSize));
            m_nWavHdrSize += 2;
        }
		m_File.Write("data", 4); 
		DWORD dwDataSizeReverse = be2me_32(m_dwDataSize);  
		m_File.Write(&dwDataSizeReverse, sizeof(m_dwDataSize));   

		return TRUE;
	}
	
	FILE* Handle()
	{
		return m_File.Handle();
	}

private:
	FileEx	m_File;
	DWORD	m_nDataSize;
    DWORD   m_nWavHdrSize;            

};
