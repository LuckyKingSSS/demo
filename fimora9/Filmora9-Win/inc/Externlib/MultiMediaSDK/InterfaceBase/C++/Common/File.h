#pragma once
#ifndef WIN32
#include "Linux32_Typedef.h"
#endif

class File
{
public:

	File(BOOL bWriteMode = FALSE)
		: m_hFile(0)
		, m_bWriteMode(bWriteMode)
	{
	}

	virtual ~File(void)
	{
		Close();
	}

	BOOL Open(const wchar_t * pFileName)
	{
		Close();

		HANDLE hFile = 0;

		if(m_bWriteMode)
		{
		
			hFile = CreateFileW(pFileName,
				GENERIC_WRITE,
				FILE_SHARE_READ,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
		}
		else
		{
			hFile = CreateFile(pFileName,
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
				NULL);
		}

		if(hFile != INVALID_HANDLE_VALUE)
		{
			m_hFile = hFile;
		}

		return (0 != m_hFile);
	}

	void Close()
	{
		if (m_hFile)
		{
			CloseHandle(m_hFile);
			m_hFile = 0;
		}
	}

	DWORD Write(const LPBYTE pData, DWORD dwLength)
	{
		if (0 == m_hFile)
			return 0;

		DWORD dwWrited = 0;
		WriteFile(m_hFile, pData, dwLength, &dwWrited, NULL);
		return dwWrited;
	}

	DWORD Read(LPBYTE pData, DWORD dwLength)
	{
		if (0 == m_hFile)
			return 0;

		DWORD dwRead = 0;
		ReadFile(m_hFile, pData, dwLength, &dwRead, NULL);
		return dwRead;
	}

	DWORD GetLength()
	{
	
		DWORD FileSizeHigh = 0;
		DWORD dwLow = GetFileSize(m_hFile, &FileSizeHigh);
		//__int64 iLen = FileSizeHigh;
		//iLen <<= 32;
		//iLen += dwLow;
		return dwLow;;
	}

	DWORD Seek(int nOffset, DWORD dwOrigin = FILE_CURRENT)
	{
		return SetFilePointer(m_hFile, nOffset, NULL, dwOrigin);
	}

	DWORD GetPos()
	{
		return SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT);
	}

private:
	HANDLE	m_hFile;
	BOOL 	m_bWriteMode;
};
