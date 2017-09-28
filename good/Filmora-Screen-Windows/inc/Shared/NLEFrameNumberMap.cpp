
#include "stdafx.h"
#include "NLEFrameNumberMap.h"
#include "NLEType.h"
#include "INLELogger.h"
#include "NLEComPtr.h"
#include "NLECommon.h"

NLEFrameIndex NLEFrameNumberMap::GetSourceFrameIndex(const NLEFrameIndex frmIndexRender)
{
    return frmIndexRender *m_ralFromTo.num / double(m_ralFromTo.den);

	ARRAY_FRMINDEX_MAP& indexMap = m_arrFrameIndiesMap;
	NLE_ASSERT(indexMap.size() == m_ralFromTo.den);

	const INT frmCountSrc = (INT)m_ralFromTo.num;
	const INT frmCountRdr = (INT)m_ralFromTo.den;
		
	int nClipCount = (frmIndexRender + 1) / frmCountRdr;
	int nRemainerRdr = (frmIndexRender + 1) % frmCountRdr;

	NLEFrameIndex frmIndexSource;
	if (nRemainerRdr > 0)
	{
		NLEFrameIndex frmIdxRemainer = nRemainerRdr - 1;
		frmIndexSource = frmCountSrc * nClipCount + indexMap[frmIdxRemainer] ;
	}
	else
	{
		frmIndexSource = frmCountSrc * nClipCount - 1;
	}
	return frmIndexSource;
}

NLEFrameIndex NLEFrameNumberMap::GetRenderFrameIndex(const NLEFrameIndex frmIndexSource)
{
    return frmIndexSource * m_ralFromTo.den / double(m_ralFromTo.num);

	ARRAY_FRMINDEX_MAP& indexMap = m_arrFrameIndiesMap;
	NLE_ASSERT(indexMap.size() == m_ralFromTo.den);

	const INT frmCountSrc = (INT)m_ralFromTo.num;
	const INT frmCountRdr = (INT)m_ralFromTo.den;

	int nClipCount	 = (frmIndexSource + 1) / frmCountSrc;
	int nRemainerSrc = (frmIndexSource + 1) % frmCountSrc;
	NLEFrameIndex frmIndexRdr;
	if (nRemainerSrc > 0)
	{
		NLEFrameIndex frmIdxRmnr = nRemainerSrc - 1;
     NLEFrameIndex frmIdxRdrRemainer = 0;
		for (int i = 0;i < frmCountRdr; i++)
		{
			if (indexMap[i] >= frmIdxRmnr)
			{
				frmIdxRdrRemainer = i;
				break;
			}
		}
		frmIndexRdr = frmCountRdr * nClipCount + frmIdxRdrRemainer;
	}
	else
	{
		frmIndexRdr = frmCountRdr * nClipCount - 1;
	}
	return frmIndexRdr;

}
 

/**
 *@brief  	生成Render--->Source映射表
 *@param	newRalFromTo Scale值，
 *-			例如(5,2)，即把5帧变为2帧(加快);(3, 5)，即把4帧变成10帧(变慢)
 *@return 	HRESULT 标准返回
 *@see 		GetRenderFrameIndex GetSourceFrameIndex
 *@note		例如:
 *-			(5,2)生成的映射表为[0]=0,[1] = 3
 *-			(3,5)生成的映射表为[0]=0,[1]=1,[2]=1, [3]=2,[4]=2
 */
HRESULT NLEFrameNumberMap::CreateFrameIndexMapTable(const NLERational& newRalFromTo)
{

    return S_OK;

	NLE_ASSERT(newRalFromTo.den != 0 && newRalFromTo.num != 0);
	if (newRalFromTo.den == 0 || newRalFromTo.num == 0)
	{
		LOGGER_ERROR_(L"Invalid parmater, num:%lld, den:%lld !!!", newRalFromTo.num, newRalFromTo.den);
		return E_INVALIDARG;
	}
	if (m_ralFromTo == newRalFromTo)
	{
		return S_OK; 
	}

	const INT frmCountSrc = (INT)newRalFromTo.num;
	const INT frmCountRdr = (INT)newRalFromTo.den;
    NLE_ASSERT(frmCountSrc == newRalFromTo.num);
    NLE_ASSERT(frmCountRdr == newRalFromTo.den);

	ARRAY_FRMINDEX_MAP& oMap = m_arrFrameIndiesMap; 
	oMap.clear();
    
	oMap.resize(frmCountRdr);
	
	NLE_ASSERT((INT)oMap.size() == frmCountRdr);

    int i = 0;
	if (frmCountRdr == 1) 
	{
		//e.g (5,1) --> [0] = 4,丢弃后4帧
		oMap[0] = 0;
	}
	else
	{
		DOUBLE dInterval = (frmCountSrc - 1) * 1.0 / (frmCountRdr - 1);
		for (i = 0; i <= (frmCountRdr - 1); i++)
		{
			oMap[i] = (NLEFrameIndex)(dInterval * i + 0.5);
		}
	}
	NLE_ASSERT(m_arrFrameIndiesMap[0] == 0);
	NLE_ASSERT(m_arrFrameIndiesMap[frmCountRdr - 1] <= frmCountSrc - 1);

#ifdef WIN32_PRINT
	LOGGER_INFO_(L"-------Scale[ %lld --->%lld ]", newRalFromTo.num, newRalFromTo.den);
	for (int i = 0; i < (int)oMap.size(); i++)
	{
		LOGGER_INFO_(L"idxMap[%d]:%d", i, oMap[i]);
	}
	LOGGER_INFO_(L"-------Scale[ %lld --->%lld ]", newRalFromTo.num, newRalFromTo.den);
#endif
	return S_OK;
}

HRESULT NLEFrameNumberMap::SetFromTo(NLERational &ralFromTo)
{
	if (m_ralFromTo == ralFromTo)
	{
		return S_OK;
	}
	HRESULT hr = S_OK;
    NLECommon::Math::ReduceRational(ralFromTo, &ralFromTo);
	if( FAILED(hr = this->CreateFrameIndexMapTable(ralFromTo) ) )	
	{
		LOGGER_ERROR_(L"Failed to create frame index table, size:(%lld / %lld )", ralFromTo.num, ralFromTo.den);
		return hr;
	}
	m_ralFromTo = ralFromTo;
	NLE_ASSERT(ralFromTo.den > 0);
	return S_OK;
}

NLEFrameNumberMap::~NLEFrameNumberMap()
{
	m_ralFromTo = 0;
}

NLEFrameNumberMap::NLEFrameNumberMap()
{

}

//////////////////////////////////////////////////////////////////////////
void NLEFrameNumberMap::Test()
{	
	NLERational ralScale(9, 19);
	SetFromTo(ralScale);
	ARRAY_FRMINDEX_MAP &idxMap = m_arrFrameIndiesMap;

	NLEFrameIndex s1, r1, s2, r2;
	r1 = 101;
	s1 = GetSourceFrameIndex(r1);
	r2 = GetRenderFrameIndex(s1);
	NLE_ASSERT(r2 == r1);
	s2 = GetSourceFrameIndex(r2);
	NLE_ASSERT(s2 == s1);
}

