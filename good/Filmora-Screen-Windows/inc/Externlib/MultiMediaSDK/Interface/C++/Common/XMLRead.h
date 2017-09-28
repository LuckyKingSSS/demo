#ifndef H_H_H_XML_READ___
#define H_H_H_XML_READ___


#include "FileInfo.h"
#include "CodecDefine.h"
#include <set>
#include <vector>
#include <map>
#include <string>
#include <list>


using namespace std;

#if defined (_WIN32)

	#define AL_APIENTRY __cdecl

	#if defined(AL_BUILD_LIBRARY) 
	#define AL_API __declspec(dllexport)
	#else
	#define AL_API __declspec(dllimport)
	#endif

	//typedef unsigned int       DWORD;
#else
	typedef unsigned int       DWORD;

	#define AL_APIENTRY
	#define AL_API extern
	#pragma export on

#endif
//2维数据定义 主要用于分辨率
struct Int2DStruct 
{
	int value1;
	int value2;
	bool operator != (Int2DStruct &right)
	{
		return  ((value1 != right.value1) || (value2 = right.value2));
	}
};
struct Param
{
	int paramType;
	int ParamdataType;
	void * value;
};
struct ParamItem
{
	wstring paramItemNam;
	Param  param;

};

enum paramType
{
	PT_LISAN_SINGLE,
	PT_LISAN_COMBIN,
	PT_LIANXU_SINGLE,
	PT_LIANXU_COMBIN,
	PT_NULL,	//空字符串
};
enum ParamDataType
{
	PD_NULL = -1, //数据类型非法
	PD_INT,
	PD_CHAR,
	PD_DOUBLE	
};
enum AVParamPRI //video audio的优先级
{
	AParam_Master,
	VParam_Master,
	AParam_Slave,
	VParam_Slave
};

using namespace std;
typedef std::vector<wstring> STRING_VECTOR;
typedef std::vector<int>   INT_VECTOR	;
typedef std::vector<double>DOUBLE_VECTOR	;
typedef std::vector<Int2DStruct> INT2D_VECTOR	;
typedef std::map<std::wstring,Param> map_ParamItem;

struct rule_ConditionItem
{
	wstring Operation;
	int expectation;
	map_ParamItem map_ConditionParamItem;
	rule_ConditionItem()
	{
		expectation =1;
		map_ConditionParamItem.clear();
	}
	~rule_ConditionItem()
	{
		map_ConditionParamItem.clear();
	}
};
struct rule_ResultItem
{
	int expectation;
	map_ParamItem map_ResultParamItem;
	rule_ResultItem()
	{
		expectation = 1;
		map_ResultParamItem.clear();
	}
	~rule_ResultItem()
	{
		map_ResultParamItem.clear();
	}
};
struct Rule_Item
{
	rule_ConditionItem conditionItem;
	rule_ResultItem	 resultItem;
};
typedef vector<Rule_Item> rules_vector;
struct AudioFourcc
{
	map_ParamItem audioParam;
	rules_vector audioRules_vector;
	AudioFourcc()
	{
		audioParam.clear();
		audioRules_vector.clear();
	}
	~AudioFourcc()
	{
		audioRules_vector.clear();
		audioParam.clear();
	}

};
typedef std::map<wstring,AudioFourcc> map_AudioFourcc;
struct VideoFourcc
{
	map_ParamItem videoParam;
	rules_vector videoRules_vector;
	VideoFourcc()
	{
		videoParam.clear();
		videoRules_vector.clear();
	}
	~VideoFourcc()
	{
		videoRules_vector.clear();
		videoParam.clear();
	}
};
struct AudioFourccItem
{
	wstring audioFccName;
	AudioFourcc audiofcc;
};
typedef std::map<wstring,VideoFourcc> map_VideoFourcc;
struct VideoFourccItem
{
	wstring videoFccName;
	VideoFourcc videofcc;
};

struct FileFourcc
{
	map_AudioFourcc map_audio;
	map_VideoFourcc map_video;
	~FileFourcc()
	{
		map_audio.clear();
		map_video.clear();
	}
};

struct FileFourccItem
{
	wstring fileFccName;// 文件格式fourcc
	FileFourcc fileFcc;
};

typedef  std::map<wstring,FileFourcc> map_FileFourcc;
struct userInputParam  
{
	wchar_t fileFourccName[260];
	wchar_t audioFourccName[260];
	wchar_t videoFourccName[260];
	int a_bitrate;
	int channle;
	int samplerate;
	int v_bitrate;
	double framerate;
	int resolution_widht;
	int resolution_height;
	userInputParam()
	{
		memset(fileFourccName,0,sizeof(fileFourccName));
		memset(audioFourccName,0,sizeof(audioFourccName));
		memset(videoFourccName,0,sizeof(videoFourccName));
		a_bitrate = 0;
		channle = 0;
		samplerate = 0;
		framerate = 0;
		resolution_widht = 0;
		resolution_height = 0;
	}
};

enum errResultKey
{
	KEY_INVALIDE = -1,
	CHANNEL_KEY,
	A_BITRATE_KEY,
	SAMPLERATE_KEY,
	FRAMERATE_KEY,
	V_BITRATE_KEY,
	RESOLUTION_KEY,

	FORMATEFILE_FORCC_KEY,
	VIDEO_FORCC_KEY,
	AUDIO_FORCC_KEY,
};

enum errParamFormatType
{
	EPF_INVALIDE = -1,
	EPF_LISAN_SINGLE,
	EPF_LISAN_COMBIN,
	EPF_LIANXU_SINGLE,
	EPF_LIANXU_COMBIN,
	EPF_NULL,	//空字符串
	EPF_PARITY,
};

enum errParamDateType
{
	EPD_INVALIDE = -1,
	EPD_INT,
	EPD_CHAR,
	EPD_DOUBLE,
	EPD_INT2D,// 分辨率是个特殊情况 是二维的
};
#define  MAX_ELEMENT_SIZE 100
struct retErrCheckResult
{
	errResultKey errKey;
	errParamFormatType epfType;
	errParamDateType   epdType;
	int nElementSize;
	int nElement[MAX_ELEMENT_SIZE];
	double dElement[MAX_ELEMENT_SIZE];
	Int2DStruct n2Element[MAX_ELEMENT_SIZE];
	int nWHPARITY[2];

	retErrCheckResult()
	{
		errKey = KEY_INVALIDE;
		epfType =  EPF_INVALIDE;
		epdType = EPD_INVALIDE;
		nElementSize =0;

	}
	retErrCheckResult &operator = (const retErrCheckResult &rightside)
	{
		nElementSize = rightside.nElementSize;
		for (int i = 0;i<rightside.nElementSize;i++)
		{
			nElement[i] = rightside.nElement[i];
			dElement[i] = rightside.dElement[i];
			n2Element[i] = rightside.n2Element[i];

		}
		return *this;

	}

};

AL_API bool AL_APIENTRY InitXml(wchar_t* xmlpath);
AL_API int  AL_APIENTRY UnintXml();
AL_API int  AL_APIENTRY CheckInputParam(userInputParam &userInputFileFourcc,retErrCheckResult* errResult = NULL);
AL_API int  AL_APIENTRY CheckInputParam(userInputParam &userInputFileFourcc);
AL_API void  AL_APIENTRY setUserParam(userInputParam &userInputFileFourcc,wchar_t  *userParamStr,void * value);

//FOR LOADING THE DLL 
enum WS_XML_PARSE_TYPE
{
	XML_PARSE_FILE_DEMUX = 0,
	XML_PARSE_FILE_DECODE,
	XML_PARSE_FILE_ENCODE,
	XML_PARSE_FILE_MUX,
	XML_PARSE_FILE_CHECK_ENCODE_PARAM
};

enum DECODE_DEMUX_MODE_XML_TYPE
{
	DECODE_DEMUX_MODE_XML_AVDEC = 0,
	DECODE_DEMUX_MODE_XML_MPDEC,
	DECODE_DEMUX_MODE_XML_DESHOW,
	DECODE_DEMUX_MODE_XML_QT
};

 struct usrLoadFileDemuxDllParam
 {
	 int			nPriority;
	 wchar_t		szDllName[128];
	 wchar_t		szID[128];
	 DWORD		    dwFourCC;
	 wchar_t		szExtName[128]; // File Extention Name，exp:L".mp4;.mov;.3gp"
	
 };

 struct usrLoadFileDecodeDllParam
 {
	 int			nPriority;
	 wchar_t		szDllName[128];
	 DWORD			dwFourCC;
	 AV_STREAM_TYPE dwType;
	 wchar_t		szID[128];	
 };

 struct usrLoadFileEncodeDllParam
 {
	 int			nPriority;
	 wchar_t		szDllName[128];
	 DWORD			dwFourCC;	
	 EncMediaType	dwType;
	 wchar_t		szID[128];
 };

 struct usrLoadFileMuxDllParam
 {
	 int			nPriority;
	 wchar_t		szDllName[128];
	 DWORD			dwFourCC;
	 wchar_t		szID[128];
	 wchar_t		szExtName[10];

 };


 //loss copy params
 template<typename DefType>
 struct LossErrCheckResult
{
	errResultKey errKey;
	DefType		 dtRightValue;
	LossErrCheckResult()
	{
		dtRightValue = 0;
		errKey = KEY_INVALIDE;
	}
};


 //定义每个参数的类型，以及结果
 template<typename DefType>
 struct ParamClass_WS
 {

	 DefType			dtValue;
	 int				nIsRight;
	 LossErrCheckResult<DefType> errResult;
	 ParamClass_WS()
	 {
		 dtValue = 0;
		 nIsRight = 1;
	 }


 };


 struct LossInputVideoInfo
 {
	ParamClass_WS<DWORD>	dwVideoForcc;
	wchar_t					dwInputVideoForcc[10];

	ParamClass_WS<int>		nWidth;		
	ParamClass_WS<int>		nHeight;	

	ParamClass_WS<double>	dFrameRate;	

	ParamClass_WS<int>		nBitrate;	
	ParamClass_WS<int>		nMaxBitrate;
	ParamClass_WS<int>		nPeakBitRate;

	//
	ParamClass_WS<unsigned int>   dwProfile;
	ParamClass_WS<unsigned int>   dwComplexityLevel;
	ParamClass_WS<int>			   nBFrame;
	ParamClass_WS<int>			   nProgressive;
	ParamClass_WS<double>		   dMaxFrameRate;
	ParamClass_WS<int>			   nIsVBR;


	LossInputVideoInfo()
	{
		memset(dwInputVideoForcc,0,sizeof(wchar_t) * 10);
	}

 };

 struct LossInputAudioInfo
 {
	ParamClass_WS<DWORD>	dwAudioForcc;
	wchar_t					dwInputAudioForcc[10];

	ParamClass_WS<int>		nSampleRate;
	ParamClass_WS<int>		nChannels;	
	ParamClass_WS<int>		nBitrate;	
	
	ParamClass_WS<int>		nMaxBitrate;
	ParamClass_WS<int>		nBitsPerSample;
	LossInputAudioInfo()
	{
		memset(dwInputAudioForcc,0,sizeof(wchar_t) * 10);
	}
 };

 struct LossOutputFormateKind
 {
	DWORD	dwFileForcc;
	DWORD	dwVideoForcc;
	DWORD	dwAudioForcc;
	LossOutputFormateKind()
	{
		dwFileForcc = dwAudioForcc = dwVideoForcc = 0;
	}
 };

 struct LossInputFormatInfo
 {
	 wchar_t     filePostFixName[10];
	ParamClass_WS<DWORD>    dwFormatForcc;
	LossInputVideoInfo		videoInfo;
	LossInputAudioInfo		audioInfo;

	

 };

 //输出编码格式，以及相关参数

 struct LossOutputFormatInfo  
 {
	 wchar_t     filePostFixName[10];
	 int		 nVideoIsLoss;
	 int		 nAudioIsLoss;

	 LossOutputFormateKind lossInputFormatKinds[64];

	 ParamClass_WS<DWORD> fileFourccName;
	 ParamClass_WS<DWORD> audioFourccName;
	 wchar_t			 audioOutputFourccName[10];

	 ParamClass_WS<DWORD> videoFourccName;
	 wchar_t			  videoOutPutFourccName[10];

	 ParamClass_WS<int> a_bitrate;
	 ParamClass_WS<int> channle;
	 ParamClass_WS<int> samplerate;

	 ParamClass_WS<int> v_bitrate;
	 ParamClass_WS<double> framerate;
	 ParamClass_WS<int> resolution_widht;
	 ParamClass_WS<int> resolution_height;

	 //
	 wchar_t hardwareDivceName[200];

	 LossOutputFormatInfo()
	 {
		 memset(audioOutputFourccName,0,sizeof(wchar_t) * 10);
		 memset(videoOutPutFourccName,0,sizeof(wchar_t) * 10);
		 memset(hardwareDivceName,0,sizeof(wchar_t) * 200);

		 nAudioIsLoss = 1;
		 nVideoIsLoss = 1;
	 }
 };


//weightvalue
 struct WeightValueIndex
 {
	 int  nWeightValue;
	 int  nIndex;
 };


#endif