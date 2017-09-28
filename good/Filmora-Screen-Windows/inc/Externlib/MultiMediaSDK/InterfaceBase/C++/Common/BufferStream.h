// BufferStream.h

#pragma once

#define SUPPORT_MULTI_THREAD

#ifdef SUPPORT_MULTI_THREAD
#include "SingleLock.h"
#endif

class BufferStream
{
public:
	BufferStream(int nInitLength = 1024)
		: m_pBuffer(0)
		, m_nBufferSize(nInitLength)
		, m_nValidSize(0)
		, m_itBegin(0)
		, m_itEnd(0)
	{
		if (0 == m_nBufferSize)
			m_nBufferSize = 1024;

		m_pBuffer = new BYTE[m_nBufferSize];
	}

	virtual ~BufferStream(void)
	{
		delete [] m_pBuffer;
		m_pBuffer = 0;
		m_nBufferSize = 0;
		m_nValidSize = 0;
	}

	 void Reset()
	{
#ifdef SUPPORT_MULTI_THREAD
		CSingleLock sl(&m_cs);
#endif
		m_nValidSize = 0;
		m_itBegin = 0;
		m_itEnd = 0;
	}

	 int GetFreeSize() const
	{
#ifdef SUPPORT_MULTI_THREAD
		CSingleLock sl(&((BufferStream*)this)->m_cs);
#endif
		return m_nBufferSize - m_nValidSize;
	}

	virtual int GetValidSize() const
	{
#ifdef SUPPORT_MULTI_THREAD
		CSingleLock sl(&((BufferStream*)this)->m_cs);
#endif
		return m_nValidSize;
	}

	 BOOL LockData(int nSize, BYTE** ppBuf1, int* pnSize1, BYTE** ppBuf2, int* pnSize2, BOOL bAccurate = FALSE) // 直接取得内存数据指针，当前指针前移
	{
#ifdef SUPPORT_MULTI_THREAD
		m_csLockData.Lock();
#endif

#ifdef SUPPORT_MULTI_THREAD
		CSingleLock sl(&m_cs);
#endif
		int nVSZ = m_nValidSize;

		if (nSize > nVSZ)
		{
			if (bAccurate)
				return FALSE;

			nSize = nVSZ;
		}

		if (m_itEnd > m_itBegin)
		{
			*ppBuf1 = m_pBuffer + m_itBegin;
			*pnSize1 = nSize;

			if (ppBuf2 && pnSize2)
			{
				*ppBuf2 = 0;
				*pnSize2 = 0;
			}
		}
		else if (m_itEnd <= m_itBegin)
		{
			int len1 = m_nBufferSize - m_itBegin;
			int len2 = m_itEnd;
			if (len1 >= nSize)
			{
				*ppBuf1 = m_pBuffer + m_itBegin;
				*pnSize1 = nSize;

				*ppBuf2 = 0;
				*pnSize2 = 0;
			}
			else
			{
				*ppBuf1 = m_pBuffer + m_itBegin;
				*pnSize1 = len1;

				if (ppBuf2 && pnSize2)
				{
					*ppBuf2 = m_pBuffer;
					*pnSize2 = nSize - len1;
				}
			}
		}

		return TRUE;
	}

	 BOOL UnlockData(BYTE* pBuf1, int nSize1, BYTE* pBuf2, int nSize2) // 直接取得内存数据指针，当前指针前移
	{
#ifdef SUPPORT_MULTI_THREAD
		CSingleLock sl(&m_cs);
#endif

		int nSize = nSize1 + nSize2;
		
		if (nSize2 > 0)
		{
			m_itBegin = nSize2;
			if (m_itBegin == m_itEnd)
			{
				m_itBegin = 0;
				m_itEnd = 0;
			}
		}
		else
		{
			m_itBegin += nSize1;
			if (m_itBegin >= m_nBufferSize)
			{
				m_itBegin = 0;
				if (m_itEnd >= m_nBufferSize)
					m_itEnd = 0;
			}
		}

		m_nValidSize -= nSize;

#ifdef SUPPORT_MULTI_THREAD
		m_csLockData.Unlock();
#endif

		return TRUE;
	}

	int GetData(LPBYTE pData, int nSize, BOOL bAccurate = FALSE) // bAccurate 为TRUE表示数据必须足够
	{
		if (0 == nSize)
			return 0;

#ifdef SUPPORT_MULTI_THREAD
		CSingleLock sl(&m_cs);
#endif
		int nVSZ = m_nValidSize;

		if (nSize > nVSZ)
		{
			if (bAccurate)
				return 0;

			nSize = nVSZ;
		}

		if (m_itEnd > m_itBegin)
		{
			memcpy(pData, m_pBuffer + m_itBegin, nSize);
			m_itBegin += nSize;
			if (m_itBegin >= m_nBufferSize)
			{
				m_itBegin = 0;
				m_itEnd = 0;
			}
		}
		else// if (m_itEnd < m_itBegin)
		{
			int len1 = m_nBufferSize - m_itBegin;
			int len2 = m_itEnd;
			if (len1 >= nSize)
			{
				memcpy(pData, m_pBuffer + m_itBegin, nSize);
				m_itBegin += nSize;
				if (m_itBegin >= m_nBufferSize)
					m_itBegin = 0;
			}
			else
			{
				memcpy(pData, m_pBuffer + m_itBegin, len1);
				memcpy(pData + len1, m_pBuffer, nSize - len1);
				m_itBegin = nSize - len1;
			}
		}

		m_nValidSize -= nSize;

		return nSize;
	}

	int PutData(LPBYTE pData, int nLen)
	{
		if (0 == nLen)
			return 0;

//		int nFree = GetFreeSize();
#ifdef SUPPORT_MULTI_THREAD
		CSingleLock sl(&m_cs);
#endif
		int nFree = m_nBufferSize - m_nValidSize;

		if (nLen > nFree)
		{
			//return 0;
			// 重新分配内存
			int nInc = ((m_nBufferSize + nLen) * 2) + 1024;
			LPBYTE pMemData = new BYTE[m_nBufferSize + nInc];

			if (m_itEnd > m_itBegin)
			{
				int len = m_itEnd - m_itBegin;
				memcpy(pMemData, m_pBuffer + m_itBegin, len);
				m_itBegin = 0;
				m_itEnd = m_itBegin + len;
			}
			else if (m_itEnd < m_itBegin)
			{
				int len1 = m_nBufferSize - m_itBegin;
				int len2 = m_itEnd;
				memcpy(pMemData, m_pBuffer + m_itBegin, len1);
				memcpy(pMemData + len1, m_pBuffer, m_itEnd);
				m_itBegin = 0;
				m_itEnd = len1 + len2;
			}

			delete [] m_pBuffer;
			m_pBuffer = pMemData;
			m_nBufferSize += nInc;
		}

		if ((m_nBufferSize - m_itEnd) >= nLen)
		{
			memcpy(m_pBuffer + m_itEnd, pData, nLen);
			m_itEnd += nLen;
		}
		else
		{
			int len = m_nBufferSize - m_itEnd;
			memcpy(m_pBuffer + m_itEnd, pData, len);
			memcpy(m_pBuffer, pData + len, nLen - len);
			m_itEnd = nLen - len;
		}

		m_nValidSize += nLen;

		return nLen;
	}

protected:
	LPBYTE		m_pBuffer;		// data buffer
	int			m_nBufferSize;
	int			m_nValidSize;

#ifdef SUPPORT_MULTI_THREAD
	CCritical		m_cs;
	CCritical		m_csLockData;
#endif

	int			m_itBegin;	// 
	int			m_itEnd;		// 
};
