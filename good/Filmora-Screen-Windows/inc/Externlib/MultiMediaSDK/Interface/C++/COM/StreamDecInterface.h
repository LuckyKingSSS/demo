#pragma once

#include <scombase.h>
#include "COMMacro.h"
#include "CommonInterface.h"
#include "DecodeParam.h"
#include "StreamDevice.h"

#ifndef WIN32
typedef void * HWND;
#endif

#ifndef STREAM_DECODER
#define  STREAM_DECODER

DEFINE_GUID(IID_IStreamDec, 
			0x9c20239e, 0x4717, 0x44e9, 0x8d, 0x4e, 0x70, 0x6f, 0x92, 0xbe, 0xc6, 0x8a);

EXTERN_C const IID IID_IStreamDec;
MACRO_MIDL_INTERFACE(IStreamDec, "9C20239E-4717-44e9-8D4E-706F92BEC68A")
: public IUnknown
{
public:
	virtual STDMETHODIMP GetOutPutVideoDevice(IOutPutDevices **ppVideoDev) = 0;
	virtual STDMETHODIMP GetOutPutAudioDevice(IOutPutDevices **ppAudioDev) = 0;

	virtual STDMETHODIMP SetUsedVideoDevice(const BSTR inDevice) = 0;
	virtual STDMETHODIMP GetUsedVideoDevice(BSTR *DevName) = 0;

	virtual STDMETHODIMP SetUsedAudioDeviceAndInputPin(const BSTR inDevice, const BSTR inPutPin) = 0; //MAC不用管inPutPin
	virtual STDMETHODIMP GetUsedAudioDeviceAndInputPin(BSTR *inDevName, BSTR *inPutPin) = 0;

	virtual STDMETHODIMP SetVideoResolution(const int nWidth, const int nHeight) = 0;
	virtual STDMETHODIMP GetVideoResolution( int &nWidth,  int &nHeight) = 0;

	virtual STDMETHODIMP GetAudioParamInfo(int &nSampleRate, int &nChannels) = 0;

	virtual STDMETHODIMP SetCallBack(MULTI_CALLBACK pCallBack, void *pUserObj) = 0; //MAC用回调函数传递消息
	virtual STDMETHODIMP SetCallBackWindow(HWND hWnd) = 0;	//WIN用窗口传递消息

	virtual STDMETHODIMP FlushDevice() = 0;	//刷新设备 

	virtual STDMETHODIMP StartDecoder(int nDisableVideo, int nDisableAudio) = 0;

	virtual STDMETHODIMP GetVideoStream(IMediaStream **ppMediaStream) = 0;
	virtual STDMETHODIMP GetAudioStream(IMediaStream **ppMediaStream) = 0;

	virtual STDMETHODIMP StopDecoder() = 0;
};
#endif

// {1000A2FF-5302-4183-90B2-844061175A16}
//定义组件对象StreamDecoder

MIDL_DEFINE_GUID(CLSID, CLSID_CSTREAM_DECODER, 0x1000a2ff, 0x5302, 0x4183, 0x90, 0xb2, 0x84, 0x40, 0x61, 0x17, 0x5a, 0x16);

EXTERN_C const CLSID CLSID_CSTREAM_DECODER;
