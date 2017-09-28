#ifndef             AVCHDPARSERINTERFACE_H_83948ugehgoeirugnlkwlkjiikw_
#define             AVCHDPARSERINTERFACE_H_83948ugehgoeirugnlkwlkjiikw_
#include			<scombase.h>
#include			"COMMacro.h"
#define				AVCHD_PARSE_FILE_DETAIL

enum   eAVCHDStreamType
{
	STREAM_UNKNOWN,
    STREAM_PRIMARY_VIDEO,
	STREAM_PRIMARY_AUDIO,
	STREAM_SUBTITLE,
	STREAM_INTERACTIVE
};

enum  eAVCHDCodec
{
	CODEC_UNKNOWN,
	CODEC_AVC,
	CODEC_MP2,
	CODEC_VC1,
	CODEC_LPCM,
	CODEC_AC3,
	CODEC_DTS,
	CODEC_BITMAP

};

typedef   struct  tag_sAVCHDStreamAttr
{
	eAVCHDStreamType        m_streamtype;
	eAVCHDCodec             m_streamcodec;
    int                     m_streamPID;
	//only   video.
	int                     m_width;
	int                     m_height;
	//only audio.
	int                     m_samplerate;
	int                     m_channel;
	int                     m_bitpersample;
}sAVCHDStreamAttr;
typedef   struct  tag_sMTSDetail
{
    int  m_nStreams;
	sAVCHDStreamAttr        m_streamList[10];
}sMTSDetail;
//interface iid {CDA63F94-AA12-4ca1-9A97-EC9D2CF45E16}
DEFINE_GUID(IID_IAVCHDParser, 
			0xCDA63F94, 0xAA12, 0x4CA1, 0x9A, 0x97, 0xEC, 0x9D, 0x2C, 0xF4, 0x5E, 0x16);
EXTERN_C const IID IID_IAVCHDParser;
MACRO_MIDL_INTERFACE(IAVCHDParser, "CDA63F94-AA12-4CA1-9A97-EC9D2CF45E16")
: public IUnknown
{
public:
	 virtual STDMETHODIMP          SetSourceDirectory(const BSTR  path)=0;

	 virtual STDMETHODIMP          ParseDirectory()=0;

	 virtual STDMETHODIMP_(int)    GetFileNumber()=0;

	 virtual STDMETHODIMP_(BSTR)          GetFilePath(const int nIndex)=0; 

	 virtual STDMETHODIMP_(sMTSDetail*)          GetFileDetail(const int nIndex)=0;
};

// class id {36278F4B-1109-424d-B86A-7F21393556C1}
MIDL_DEFINE_GUID(CLSID, CLSID_CAVCHDParser, 0x36278F4B, 0x1109, 0x424d, 0xB8, 0x6A, 0x7F, 0x21, 0x39, 0x35, 0x56, 0xC1);
EXTERN_C const CLSID CLSID_CAVCHDParser;
#endif