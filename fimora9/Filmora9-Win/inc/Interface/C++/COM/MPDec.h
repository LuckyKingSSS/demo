#pragma once
#include "CommonInterface.h"
#include "DecodeParam.h"

#ifndef MP_DECODER
#define MP_DECODER

DEFINE_GUID(IID_IMPDec, 
			0x9f134f68, 0x9670, 0x44e1, 0x8a, 0x73, 0xc7, 0x52, 0xf2, 0x9d, 0x8b, 0x2c);

EXTERN_C const IID IID_IMPDec;
MACRO_MIDL_INTERFACE(IMPDec, "9F134F68-9670-44e1-8A73-C752F29D8B2C")
: public IUnknown
{
public:
	virtual STDMETHODIMP GetVideoStream(IMediaStream **ppMediaStream) = 0;
	virtual STDMETHODIMP GetAudioStream(IMediaStream **ppMediaStream) = 0;
};

typedef MEDIA_FRAME * (__cdecl *CreateYV12Frame_Internal_API)(int width, int height, int bAlpha, int bBlackImage);
typedef	IMPDec *	(__stdcall *WSCreateMPDecoderEx_InternalFrame_API) (const wchar_t * pFileName,
														  MEDIA_FILE_INFO* pMediaInfo,
														  const DecParam* pVideoDecParam,
														  const DecParam* pAudioDecParam,  
														  int* pError,
														  int nCreateTag,
														  CreateYV12Frame_Internal_API fnCreateYV12Frame,
														  int nUseFFMpeg);

#endif