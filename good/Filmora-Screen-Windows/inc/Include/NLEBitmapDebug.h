#pragma once

#define DEBUG_NLEBITMAP_CONTENT 


struct NLEBitmapDebugInfo
{
	unsigned int nBitCount;	//8/32
	unsigned int nWidth;
	unsigned int nHeight;
	unsigned int nPitch;
	unsigned char* pData;
	LONGLONG *ptsPtr;
	NLEBitmapDebugInfo()
	{
		nWidth = nHeight = nPitch = 0; pData = NULL; ptsPtr = NULL; nBitCount = 32;
	}
	NLEBitmapDebugInfo(int w, int h, int stride, unsigned char* data)
	{
		nWidth = w; nHeight = h; nPitch = stride; pData = data;
	}
};

#ifdef DEBUG_NLEBITMAP_CONTENT
#define BITMAP_DEBUG_INFO NLEBitmapDebugInfo m_oBmpDebugInfo; \
void UpdateDebugInfo() \
{	\
	m_oBmpDebugInfo.nWidth = this->GetWidth(); \
	m_oBmpDebugInfo.nHeight = this->GetHeight(); \
	m_oBmpDebugInfo.nPitch = this->GetPitch(); \
	m_oBmpDebugInfo.pData = this->GetBits(); \
	m_oBmpDebugInfo.nBitCount = this->GetBitCount(); \
}
#else 
#define BITMAP_DEBUG_INFO 
#endif
