//
// TimeCodeBufferStream.h
//

#pragma once 
#include <list>
#include "BufferStream.h"
using namespace std;

struct struTimeCode
{
    __int64 pts;
    int		nBlockLen;		
};

class TimeCodeBufferStream : public  BufferStream
{
public:

	TimeCodeBufferStream(int nInitLength = 1024) : BufferStream(nInitLength) 
	{
		m_listTimeCode.clear();	
	}

	virtual ~ TimeCodeBufferStream(void)
	{
		m_listTimeCode.clear();
	}

	virtual void Reset()
	{
		BufferStream::Reset();
		m_listTimeCode.clear();
	}

//包的时间戳为第一个数据帧头的时间戳
	__int64 GetPTS(int nSize)
	{
		__int64 pts = -1;
		if(m_listTimeCode.size() == 0)
			return pts;

		list<struTimeCode>::iterator iterTimeCode;
		if (0 == nSize)  // 0表示获取头个包的PTS，但不将头个包的信息移出
		{
			iterTimeCode = m_listTimeCode.begin();
			pts = iterTimeCode->pts;
		}
		else
		{
			int nDataSum = 0;
			iterTimeCode = m_listTimeCode.begin();
			pts = iterTimeCode->pts;   //取它所在BLOCK的时间磋
			iterTimeCode->pts = -1; //取了哪个那么就将它的PTS设置成-1，避免混乱；
			//nDataSum = iterTimeCode->nBlockLen;
			//iterTimeCode++;

			while (iterTimeCode != m_listTimeCode.end())
			{
				nDataSum += iterTimeCode->nBlockLen;
				if (nDataSum < nSize)  
				{
					//if (-1 == pts && iterTimeCode->pts > -1)
						//pts = iterTimeCode->pts;
					list<struTimeCode>::iterator iterTemp = iterTimeCode;
					iterTimeCode++;
					m_listTimeCode.erase(iterTemp);
				}
				else if (nDataSum == nSize)
				{
					//if (-1 == pts && iterTimeCode->pts > -1)
						//pts = iterTimeCode->pts;

					m_listTimeCode.erase(iterTimeCode);
					break;
				}
				else
				{
					iterTimeCode->nBlockLen = nDataSum - nSize;
					break;
				}
			}
		}
		
		return pts;
	}

//获得nPos所在包的PTS
	__int64 GetPTSByPos(int nPos)
	{
		__int64 pts = -1;
		list<struTimeCode>::iterator iterTimeCode;
		if (0 == nPos)  // 0表示获取头个包的PTS，但不将头个包的信息移出
		{
			iterTimeCode = m_listTimeCode.begin();
			pts = iterTimeCode->pts;
		}
		else
		{
			int nDataSum = 0;
			iterTimeCode = m_listTimeCode.begin();

			while (iterTimeCode != m_listTimeCode.end())
			{
				nDataSum += iterTimeCode->nBlockLen;
				if (nDataSum < nPos)  
				{
					iterTimeCode++;
				}
				else if (nDataSum >= nPos)
				{
					pts = iterTimeCode->pts;

					break;
				}
			}
		}
		
		return pts;
	}
	
	void EraseData(int nSize, BOOL bErasePTS)
	{
		list<struTimeCode>::iterator iterTimeCode;
		if (0 == nSize)  // 0表示获取头个包的PTS，但不将头个包的信息移出
		{
			return;
		}
		else
		{
			int nDataSum = 0;
			iterTimeCode = m_listTimeCode.begin();
			if (bErasePTS)
				iterTimeCode->pts = -1;

			while (iterTimeCode != m_listTimeCode.end())
			{
				nDataSum += iterTimeCode->nBlockLen;
				if (nDataSum < nSize)  
				{
					list<struTimeCode>::iterator iterTemp = iterTimeCode;
					iterTimeCode++;
					m_listTimeCode.erase(iterTemp);
				}
				else if (nDataSum == nSize)
				{
					m_listTimeCode.erase(iterTimeCode);

					break;
				}
				else
				{
					iterTimeCode->nBlockLen = nDataSum - nSize;

					break;
				}
			}
		}
		
	}

	int PutData(LPBYTE pData, int nLen, __int64 pts)
	{
		BufferStream::PutData(pData, nLen);
		
		struTimeCode blockTimeCode;
		list<struTimeCode>::iterator iterTimeCode = m_listTimeCode.begin();

		//if (-1 != pts || m_listTimeCode.size() == 0)
		{
			blockTimeCode.pts = pts;
			blockTimeCode. nBlockLen = nLen;
			m_listTimeCode.push_back(blockTimeCode);
			return nLen;
		}
		/*
		else  //包跟上一个合并
		{
			//list<TimeCodeMgr>::iterator iterTimeCode;
			iterTimeCode = m_listTimeCode.end();
			iterTimeCode--;
			iterTimeCode->nBlockLen += nLen;
			return nLen;
		}
		return nLen;
		*/
	}

private:
	list<struTimeCode>	m_listTimeCode;
};
