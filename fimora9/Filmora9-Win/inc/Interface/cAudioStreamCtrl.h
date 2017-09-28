

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Thu Mar 02 11:38:32 2017
 */
/* Compiler settings for cAudioStreamCtrl.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */

#include "stdint.h"
#include <Guiddef.h>
#include <objbase.h>

#ifndef __cAudioStreamCtrl_h__
#define __cAudioStreamCtrl_h__

// {F6A83839-7B1B-49C1-8E31-00B4214D108D}
DEFINE_GUID(IID_IAudioStreamCtrl, 
0xf6a83839, 0x7b1b, 0x49c1, 0x8e, 0x31, 0x0, 0xb4, 0x21, 0x4d, 0x10, 0x8d);

#ifdef __cplusplus
extern "C"{
#endif 

// 音频流的公共控制接口

#ifndef __IAudioStreamCtrl_INTERFACE_DEFINED__
#define __IAudioStreamCtrl_INTERFACE_DEFINED__

class IAudioStreamCtrl : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE GetSamplerateList( 
		/* [out][in] */ int32_t *samplerateList,
		/* [out][in] */ int32_t *cnt) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE GetSamplerate( 
		/* [out][in] */ int32_t *samplerate) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE SetSamplerate( 
		/* [in] */ int32_t samplerate) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE GetChannelsList( 
		/* [out][in] */ int32_t *channelsList,
		/* [out][in] */ int32_t *cnt) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE GetChannels( 
		/* [out][in] */ int32_t *channels) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE SetChannels( 
		/* [in] */ int32_t channels) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE GetBitsPerSampleList( 
		/* [out][in] */ int32_t *bitsPerSampleList,
		/* [out][in] */ int32_t *cnt) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE GetBitsPerSample( 
		/* [out][in] */ int32_t *bitsPerSample) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE SetBitsPerSample( 
		/* [in] */ int32_t bitsPerSample) = 0;
	
};

#endif 	/* __IAudioStreamCtrl_INTERFACE_DEFINED__ */

#ifdef __cplusplus
}
#endif

#endif


