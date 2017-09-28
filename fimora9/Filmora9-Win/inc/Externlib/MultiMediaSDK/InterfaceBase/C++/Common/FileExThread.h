// FileExThread.h

#pragma once
#ifndef WIN32
#include "Linux32_Typedef.h"
#endif
#include "FileEx.h"
#include "BaseThread.h"

#define SUPPORT_MULTI_THREAD
#include "BufferStream.h"
#include "SingleLock.h"

#define BLOCK_SIZE		(4 * 1024)
#define BUFFER_SIZE		(5 * 1024 * 1024)

class FileExThread : public FileEx, public BaseThread
{
public:

	FileExThread()
		: m_pBlock(0)
		, m_i64CurrPos(0)
		, m_Buffer(BUFFER_SIZE + BLOCK_SIZE * 2)
		, m_bISO(0)
	{
	}

	virtual ~FileExThread(void)
	{
		Close();
	}

	BOOL Open(const wchar_t * pFileName, const char * pMode = "rb")
	{
		BOOL br = FileEx::Open(pFileName, pMode);
		if (!br)
			return  FALSE;
		
		if (JudgeISOFileW(pFileName))
		{
			m_bISO = TRUE;

			return TRUE;
		}
		m_pBlock = new BYTE[BLOCK_SIZE];

		//Start();
		m_bISO = FALSE;
		
		return br;
	}

	void Close()
	{
		Stop();
		FileEx::Close();
		m_Buffer.Reset();

		delete [] m_pBlock;
		m_pBlock = 0;

		m_i64CurrPos = 0;
	}

	BOOL EnableMultiThread(BOOL bEnable)
	{
		if (m_bISO)
			return TRUE;

		BOOL bOldFlag = IsActive();
		if (bEnable)
		{
			if(!IsActive())
			{
				m_Buffer.Reset();
				Start();
#ifdef WSLOG
				WSLOG(L"FileExThread::StartThread...");
#endif
			}
		}
		else
		{
#ifdef WSLOG
			WSLOG(L"FileExThread::StopThread.");
#endif
			Stop();
			m_Buffer.Reset();
			FileEx::Seek(m_i64CurrPos, SEEK_SET);
		}

		return bOldFlag;
	}

	DWORD Read(void* pData, DWORD dwLength)
	{
		DWORD dwRead = 0;
		if (!IsActive())
		{
			dwRead = FileEx::Read(pData, dwLength);
		}
		else
		{
			int nOnce = dwLength;
			if (nOnce > (2 * 1024 * 1024))
				nOnce = 2 * 1024 * 1024;

			while(dwRead < dwLength)
			{
				if (nOnce > (int)(dwLength - dwRead))
					nOnce = (int)(dwLength - dwRead);

				while(m_Buffer.GetValidSize() < nOnce)
				{
					if (IsEof())
						break;
					Sleep(1);
				}

				dwRead += (DWORD)m_Buffer.GetData((LPBYTE)pData + dwRead, nOnce);
				if (IsEof())
					break;
			}

		}

		m_i64CurrPos += dwRead;
		
		return dwRead;
	}

	int Seek(__int64 i64Offset, int origin = SEEK_CUR)
	{
		CSingleLock sl_seek(&m_csSeek);

		int ret = 0;
		if (!IsActive())
		{
			ret = FileEx::Seek(i64Offset, origin);
			m_i64CurrPos = FileEx::GetPos();
			m_Buffer.Reset();
		}
		else
		{
			__int64 pos = 0;
			if (SEEK_CUR == origin)
			{
				pos = m_i64CurrPos + i64Offset;
			}
			else if (SEEK_SET == origin)
			{
				pos = i64Offset;
			}
			else
			{
				pos = GetLength() + i64Offset;
			}

			if (pos < m_i64CurrPos)
			{
				ret = FileEx::Seek(pos, SEEK_SET);
				m_i64CurrPos = FileEx::GetPos();
				m_Buffer.Reset();
			}
			else if (pos > m_i64CurrPos)
			{
				int nBufRest = m_Buffer.GetValidSize();
				__int64 delta = pos - m_i64CurrPos;
				if (delta <= nBufRest)
				{
					BYTE* buf1 = 0;
					BYTE* buf2 = 0;
					int len1 = 0;
					int len2 = 0;
					m_Buffer.LockData((int)delta, &buf1, &len1, &buf2, &len2);
					m_Buffer.UnlockData(buf1, len1, buf2, len2);
					m_i64CurrPos += delta;
				}
				else
				{
					ret = FileEx::Seek(pos, SEEK_SET);
					m_i64CurrPos = FileEx::GetPos();
					m_Buffer.Reset();
				}
			}
			
		}

		return ret;
	}

	__int64 GetPos()
	{
		return m_i64CurrPos;
	}

protected:
	virtual void svc()
	{

#ifdef WSLOG
		WSLOG(L"FileExThread::svc()...");
#endif
		for (;;)
		{
			//IFPAUSESTATE_WAIT;
			IFABORTEVENT_BREAK(0);
			// do somthing

			if ((m_Buffer.GetValidSize() >= (BUFFER_SIZE)) || IsEof())
			{
				Sleep(1);
				continue;
			}

			CSingleLock sl_seek(&m_csSeek);

			DWORD dwRead = FileEx::Read(m_pBlock, BLOCK_SIZE);
			if (0 == dwRead)
			{
				Sleep(1);
			}
			else
			{
				m_Buffer.PutData(m_pBlock, dwRead);
			}
		} 
	}

	
private:

	BufferStream	m_Buffer;

	LPBYTE			m_pBlock;

	__int64			m_i64CurrPos;

	CCritical		m_csSeek;

	BOOL			m_bISO;

};
