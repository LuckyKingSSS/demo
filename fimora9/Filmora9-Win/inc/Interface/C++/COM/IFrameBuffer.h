/* ### WS@H Project:Multimedia_Platform ### */
#ifndef            IFRAMEBUFFER_H_
#define            IFRAMEBUFFER_H_

#include "typedef.hpp"
#include <scombase.h>
#include "COMMacro.h"
class         CWSSample;

DEFINE_GUID(IID_IFrameBuffer, 
			0x6B2DCCFD, 0x012F, 0x4a84, 0x8E, 0x43, 0x18, 0x57, 0x0F, 0xE7, 0x5B, 0xB6);
EXTERN_C const IID IID_IFrameBuffer;

MACRO_MIDL_INTERFACE(IFrameBuffer, "6B2DCCFD-012F-4a84-8E43-18570FE75BB6")
: public IUnknown
{
public:
		virtual STDMETHODIMP					  SetCacheDirectory(const BSTR   szDirectory,const BSTR   szCacheFile)=0;//reserved.
		virtual STDMETHODIMP_(CWSSample*)         GetDataSample()=0;//single thread call //reserved.
		virtual STDMETHODIMP_(CWSSample*)         GetEmptySample(const ULONG  nRequireBytes)=0;//reserved.
		virtual STDMETHODIMP                      PushEmptySample(CWSSample*     pSample)=0;	//reserved.
		virtual STDMETHODIMP                      PushDataSample(CWSSample*      pSample)=0;//reserved.

		virtual STDMETHODIMP_(CWSSample*)         GetDataSampleMT(const ULONG  nMillSecond)=0;//multi-thread safe
		virtual STDMETHODIMP_(CWSSample*)         GetEmptySampleMT(const ULONG   nMillSecond,const ULONG  nRequireBytes)=0;
		virtual STDMETHODIMP                      PushEmptySampleMT(CWSSample*     pSample)=0;
		virtual STDMETHODIMP                      PushDataSampleMT(CWSSample*    pSample)=0;
		virtual STDMETHODIMP                      AddNewEmptySampleMT(CWSSample* pSample)=0;
		virtual STDMETHODIMP                      DecreaseSamples(const ULONG  nSamples)=0;//reduce the count of samples 
	
		virtual STDMETHODIMP_(ULONG)              GetDataSampleCount()=0;
		virtual STDMETHODIMP_(ULONG)              GetEmptySampleCount()=0;
		virtual STDMETHODIMP_(ULONG)              GetBufferSize()=0;//Sample count belongs to this buffer.
		virtual STDMETHODIMP                      SetBufferSize(const ULONG nSamples)=0;//reserved.
		virtual STDMETHODIMP                      AbandonDataSamples()=0;//Clear the Data Samples and put them into empty Sample array.
		virtual STDMETHODIMP                      FreeAllSamples()=0;
		virtual STDMETHODIMP_(LONG)               GetReferenceCount()=0;	

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//only use in preview mode with MPDec  for temporay.
		//user should not return Sample to buffer, because it is allways the same sample.
		virtual STDMETHODIMP_(CWSSample*)         GetDataSamplePassBy()=0;
		virtual STDMETHODIMP_(LONGLONG)           DirectSeekFromPlayer(const LONGLONG  nPos)=0;
	    ///////////////////////////////////////////////////////////////////////////////////
};

#endif