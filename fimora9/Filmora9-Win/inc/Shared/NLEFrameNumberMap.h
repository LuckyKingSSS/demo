
#include "stdafx.h"
#include <vector>
#include "NLEType.h"

#ifndef _H_NLEFRAMENUMBERMAP_H_
#define _H_NLEFRAMENUMBERMAP_H_

typedef std::vector<NLEFrameIndex> ARRAY_FRMINDEX_MAP;

class NLEFrameNumberMap
{
public:
	NLEFrameNumberMap();
	
	~NLEFrameNumberMap();

public:
	NLEFrameIndex GetSourceFrameIndex(const NLEFrameIndex frmIndexRender);
	NLEFrameIndex GetRenderFrameIndex(const NLEFrameIndex frmIndexSource);
	HRESULT CreateFrameIndexMapTable(const NLERational& newRalFromTo);
	HRESULT SetFromTo(NLERational &ralFromTo);

public:
	void Test();
private:
	/**@brief 最终Scale倍数,（30, 25）即30帧变25帧(变慢), ()*/
	NLERational m_ralFromTo;

	/**@brief 帧序号映射表,使用方法 nRenderFrameIndex = m_arrFrameIndiesMap[nSourceTotalFrameIndex] */
	ARRAY_FRMINDEX_MAP m_arrFrameIndiesMap;
};


#endif //_H_NLEFRAMENUMBERMAP_H_

