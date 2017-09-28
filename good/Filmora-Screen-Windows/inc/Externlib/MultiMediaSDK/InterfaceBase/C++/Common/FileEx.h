// FileEx.h

#pragma once
#ifndef WIN32
#include "Linux32_Typedef.h"
#endif
#include "strconv.h"
#include <wchar.h>
#include <string>
#include "strconv.h"
#ifndef WIN32
	#define _ftelli64 ftello
	#define _fseeki64 fseeko
#endif

#ifdef WIN32

#ifndef STRCMP
#define STRCMP

#define strcasecmp  stricmp
#define strncasecmp strnicmp

#endif

#endif


class FileEx
{
public:

	BOOL JudgeISOFileA(const char *pFileName)
	{
		const char *pExtPos = strrchr(pFileName, '.');
		if (NULL == pExtPos)
			return FALSE;

		char  cExtName[32] = {'\0'};
		strcpy(cExtName, pExtPos);
		if (strncasecmp(cExtName, ".iso:", 5) == 0)
			return TRUE;
		else
			return FALSE;
	}

	BOOL JudgeISOFileW(const wchar_t *pFileName)
	{
	    std::wstring file(pFileName);
		if (std::wstring::npos == file.rfind(L'.'))
			return FALSE;

        USES_CONVERSION;
        char *pszExtName = W2A(pFileName);
		if (strncasecmp(pszExtName, ".iso:", 5) == 0)
			return TRUE;
		else
			return FALSE;
	}

	FileEx()
		: m_pFile(0)
		, m_pFileName(0)
		, m_bISO(FALSE)
	{
	}

	virtual ~FileEx(void)
	{
		Close();
	}

	virtual BOOL Open(const wchar_t * pFileName, const char * pMode = "rb")
	{
		Close();

		if (JudgeISOFileW(pFileName))
		{
			m_pFileName = new wchar_t[wcslen(pFileName) + 1];
			wcscpy(m_pFileName, pFileName);
			m_bISO = TRUE;

			return TRUE;
		}

		m_bISO = FALSE;

		USES_CONVERSION;
#ifdef WIN32		
		m_pFile = _wfopen(pFileName, A2W(pMode));
#else
		m_pFile = fopen(W2A(pFileName), pMode);	
#endif

		if (m_pFile)
		{
			m_pFileName = new wchar_t[wcslen(pFileName) + 1];
			wcscpy(m_pFileName, pFileName);
		}

		return (0 != m_pFile);
	}

	virtual void Close()
	{
		if (m_pFile)
		{
			fclose(m_pFile);
			m_pFile = 0;
		}

		delete [] m_pFileName;
		m_pFileName = 0;
	}
	
	virtual void Flush()
	{
		if (m_pFile)
			fflush(m_pFile);
	}

	virtual DWORD Write(const void* pData, DWORD dwLength)
	{
		if (0 == m_pFile)
			return 0;

		DWORD dwWrited = (DWORD)fwrite(pData, 1, dwLength, m_pFile);
		return dwWrited;
	}

	virtual DWORD Read(void* pData, DWORD dwLength)
	{
		if (0 == m_pFile)
			return 0;

		DWORD dwRead = (DWORD)fread(pData, 1, dwLength, m_pFile);
		return dwRead;
	}

	virtual __int64 GetLength()
	{
		if (m_bISO)
		{
			return 1;
		}

		if (0 == m_pFile)
			return 0;

		__int64 iLength = 0;

		__int64 iCur = _ftelli64(m_pFile);
		_fseeki64(m_pFile, 0, SEEK_END);
		iLength = _ftelli64(m_pFile);
		_fseeki64(m_pFile, iCur, SEEK_SET);
	
		return iLength;;
	}

	virtual int Seek(__int64 i64Offset, int origin = SEEK_CUR) // successful to return 0 
	{
		if (m_bISO)
			return  0;

		int ret = -1;

		ret = _fseeki64(m_pFile, i64Offset, origin);

		return ret;
	}

	virtual __int64 GetPos()
	{
		if (m_bISO)
			return 0;

		__int64 iCur = 0;

		iCur = _ftelli64(m_pFile);

		return iCur;
	}

	virtual BOOL EnableMultiThread(BOOL bEnable)
	{
		return FALSE;
	}

	BOOL IsEof()
	{
		return (feof(m_pFile));
	}
	
	FILE* Handle()
	{
		return m_pFile;
	}

	const wchar_t* GetFileName() const
	{
		return m_pFileName;
	}

private:
	FILE*		m_pFile;
	wchar_t*	m_pFileName;
	BOOL		m_bISO;
};
