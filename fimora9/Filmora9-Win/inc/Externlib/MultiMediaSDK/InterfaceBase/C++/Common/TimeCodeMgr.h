//
// TimeCodeMgr.h
//

#pragma once 
#include <list>
using namespace std;

enum SEARCH_DIRECTION
{
	FRONT = 0,
	NEXT,
	NEARPOS
};

class TimeCodeMgr
{
	struct struTimeCodeMgr
	{
		__int64 pts;
		__int64 pos;
		int		nBlockLen;
	};
public:

	TimeCodeMgr()
	{
		m_listTimeCode.clear();	
	}

	virtual ~ TimeCodeMgr(void)
	{
		m_listTimeCode.clear();
	}

	virtual void Reset()
	{
		m_listTimeCode.clear();
	}

	int GetTimeCodeListSize()
	{
		return (int) m_listTimeCode.size();
	}

	__int64 GetPTSByIndex(int nIndex)
	{
		if(nIndex >= 0 && nIndex < (int) m_listTimeCode.size())
		{
			list<struTimeCodeMgr>::iterator it = m_listTimeCode.begin();
			while(nIndex--) it++;

			return it->pts;
		}

		return -1;
	}

	//删除index以及之前的BLOCK
	void EraseBlockByIndex(int nIndex)
	{
		if(nIndex >= 0 && nIndex < (int) m_listTimeCode.size())
		{
			list<struTimeCodeMgr>::iterator iter = m_listTimeCode.begin();
			list<struTimeCodeMgr>::iterator iterNext = iter;

			for (int i = 0; i <= nIndex; i++)
			{
				iterNext = iter;
				iterNext++;

				m_listTimeCode.erase(iter);
				iter = iterNext;
			}


			return ;
		}
	}


	__int64 GetPTS(int nSize)
	{
		__int64 pts = -1;
		list<struTimeCodeMgr>::iterator iterTimeCode;
		if (0 == nSize)  // 0表示获取头个包的PTS，但不将头个包的信息移出
		{
			iterTimeCode = m_listTimeCode.begin();
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
					if (-1 == pts && iterTimeCode->pts > -1)
						pts = iterTimeCode->pts;
					list<struTimeCodeMgr>::iterator iterTemp = iterTimeCode;
					iterTimeCode++;
					m_listTimeCode.erase(iterTemp);
				}
				else if (nDataSum == nSize)
				{
					if (-1 == pts && iterTimeCode->pts > -1)
						pts = iterTimeCode->pts;

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


////返回所在包的PTS， 该包所在链表的位置(pos)，return 该位置在所在包的相对位置，aim 表示方向， 0表示前一个，1表示后一个， 2表示最近的
	__int64 SearchPTS(const __int64 SearchPos, __int64 &pts, __int64 &pos, SEARCH_DIRECTION dir)   
	{
		__int64 nSize = 0;
		list<struTimeCodeMgr>::iterator it;
		int nDataSum = 0;

		it = m_listTimeCode.begin();
		if (it == m_listTimeCode.end())
		{
			pts = -1;
			pos = -1;
			return 0;
		}
		else
		{
			nSize = SearchPos + 1 - it->pos;   //实际的长度
		}
		while (it != m_listTimeCode.end())
		{
			TimeCodeMgr::struTimeCodeMgr item = *it;
			nDataSum += it->nBlockLen;
			if ((it->pos == SearchPos && it->pts > -1) || (it->pos == SearchPos && nSize <= nDataSum))
			{
				pts = it->pts;
				pos = it->pos;
				return 0;
			}

			if (nDataSum < nSize)  
			{
				it++;
				if (it == m_listTimeCode.end())
				{
					it--;
					item = *it;
					pts = it->pts;
					pos = it->pos;
					it++;
				}
				//item = *it;
			}
			else
			{
				switch (dir)
				{
				case FRONT:
					{
						pts = it->pts;
						pos = it->pos;
						break;
						//return (nSize - nDataSum + iterTimeCode->nBlockLen);
					}
				case NEXT: 
					{
						it++;
						if (it != m_listTimeCode.end())
						{
							item = *it;
							pts = it->pts;
							pos = it->pos;
						}
						else
						{
							pts = -1;
							pos = -1;
						}
						break;
					}
				case NEARPOS:
					{
						if (nDataSum - nSize > 	it->nBlockLen / 2)
						{
							it++;
							if (it != m_listTimeCode.end())
							{
								item = *it;
								pts = it->pts;
								pos = it->pos;
							}
							else
							{
								it--;
								item = *it;
								pts = it->pts;
								pos = it->pos;
							}
						}
						else
						{
							pts = it->pts;
							pos = it->pos;
						}
						break;
					}
				default:
					{
						pts = -1;
						pos = -1;
						break;
					}
				} //switch (aim)
				return 0;
			}
		}

		return 0;
	}


	__int64 SearchPTSReverse(const __int64 SearchDataCount, __int64 &pts, __int64 &pos)   
	{
		std::list<struTimeCodeMgr>::iterator iter;
		std::list<struTimeCodeMgr>::iterator iter_tmp;
		int nDataSum = 0;


		if (m_listTimeCode.size() > 0)
		{
			iter_tmp =m_listTimeCode.begin();
			iter = iter_tmp;
			if (SearchDataCount > iter->pos + 1 + iter->nBlockLen)
			{
				iter = m_listTimeCode.end();
				--iter;

				while (iter != iter_tmp)
				{
					if ( (iter->pos + 1) <= SearchDataCount )
					{
						pts = iter->pts;
						pos = iter->pos;

						break;
					}

					--iter;
				}
			}
			else
			{
				pts = iter->pts;
				pos = iter->pos;
			}
		}
		else
		{
			pts = -1;
			pos = -1;

			return 0;
		}

		return 0;
	}


   BOOL EraseBlock(const __int64 SearchPos)
   {
	   list<struTimeCodeMgr>::iterator it = m_listTimeCode.begin();
	  
	   if (it == m_listTimeCode.end())
	   {
			return	FALSE;
	   }
	   while (it != m_listTimeCode.end())
	   {
		    const struTimeCodeMgr* item = &(*it);
			if (item->pos < SearchPos)
			{
				list<struTimeCodeMgr>::iterator itTemp = it;
				it++;
				m_listTimeCode.erase(itTemp);
			}
	   }
	   return TRUE;	
   }
   __int64 SearchPTS(const __int64 SearchPos, const __int64 nLen, __int64 &pos) 
   {
		pos = -1;
		list<struTimeCodeMgr>::iterator it = m_listTimeCode.begin();
		if (it == m_listTimeCode.end())
		{
			return -1;
		}
		if (it->pos > SearchPos)
			return -1;

		while(it != m_listTimeCode.end())
		{
			const struTimeCodeMgr* item = &(*it);
			if ((item->pos == SearchPos && item->pts > -1) || (item->pos == SearchPos && nLen <= item->nBlockLen))
			{
				pos = item->pos;
				return item->pts;
			}

			//继续向下找
			if ( item->pos + item->nBlockLen > SearchPos)  //SearchPos在该block内
			{
				it++;
				if (it != m_listTimeCode.end())
				{
                    const struTimeCodeMgr* item = &(*it);
					if (SearchPos + nLen > item->pos)
					{
						pos = item->pos;
						return item->pts;
					}
					else
					{
						return -1;
					}
				}
				else
				{
					return -1;
				}
			}
			else
			{
				it++;
			}
		}
		return -1;
   }

	int PutPTS(int nLen, __int64 pts)
	{
		//计算pos
		struTimeCodeMgr blockTimeCode;
		list<struTimeCodeMgr>::iterator iterTimeCode = m_listTimeCode.begin();
		if (iterTimeCode == m_listTimeCode.end())
			blockTimeCode.pos = 0;
		else
		{
			iterTimeCode = m_listTimeCode.end();
			iterTimeCode--;
			blockTimeCode.pos = iterTimeCode->pos + iterTimeCode->nBlockLen;
		}
		if (-1 != pts || m_listTimeCode.size() == 0)
		{
			blockTimeCode.pts = pts;
			blockTimeCode. nBlockLen = nLen;
			m_listTimeCode.push_back(blockTimeCode);
			return nLen;
		}
		else  //包跟上一个合并
		{
			//list<TimeCodeMgr>::iterator iterTimeCode;
			iterTimeCode = m_listTimeCode.end();
			iterTimeCode--;
			iterTimeCode->nBlockLen += nLen;
			return nLen;
		}
		return nLen;
	}


	//获得nPos所在包的PTS
	__int64 GetPTSByPos(int nPos)
	{
		__int64 pts = -1;
		list<struTimeCodeMgr>::iterator iterTimeCode;
		if (0 == nPos)  // 0表示获取头个包的PTS，但不将头个包的信息移出
		{
			iterTimeCode = m_listTimeCode.begin();
            if (iterTimeCode != m_listTimeCode.end())
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
		list<struTimeCodeMgr>::iterator iterTimeCode;
		if (0 == nSize)  // 0表示获取头个包的PTS，但不将头个包的信息移出
		{
			return;
		}
		else
		{
			int nDataSum = 0;
			iterTimeCode = m_listTimeCode.begin();
            if (bErasePTS && iterTimeCode != m_listTimeCode.end())
                iterTimeCode->pts = -1;

			while (iterTimeCode != m_listTimeCode.end())
			{
				nDataSum += iterTimeCode->nBlockLen;
				if (nDataSum < nSize)  
				{
					list<struTimeCodeMgr>::iterator iterTemp = iterTimeCode;
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

	void EraseDataByPos(__int64 nPos, BOOL bErasePTS)
	{
		list<struTimeCodeMgr>::iterator iterTimeCode;
		if (0 == nPos)  // 0表示获取头个包的PTS，但不将头个包的信息移出
		{
			return;
		}
		else
		{
			iterTimeCode = m_listTimeCode.begin();
			if (bErasePTS)
				iterTimeCode->pts = -1;

			while (iterTimeCode != m_listTimeCode.end())
			{
				__int64 pos = iterTimeCode->pos + iterTimeCode->nBlockLen;
				if (pos < nPos)  
				{
					list<struTimeCodeMgr>::iterator iterTemp = iterTimeCode;
					iterTimeCode++;
					m_listTimeCode.erase(iterTemp);
				}
				else if (pos == nPos)
				{
					m_listTimeCode.erase(iterTimeCode);

					break;
				}
				else
				{
					break;
				}
			}
		}

	}

	void EraseDataByCount(int nCount)
	{
		list<struTimeCodeMgr>::iterator iterTimeCode = m_listTimeCode.begin();

		while ((iterTimeCode != m_listTimeCode.end()) && (nCount > 0))
		{
			list<struTimeCodeMgr>::iterator iterTemp = iterTimeCode;
			iterTimeCode++;
			nCount--;
			m_listTimeCode.erase(iterTemp);
		}

	}

private:

	list<struTimeCodeMgr>	m_listTimeCode;
};
