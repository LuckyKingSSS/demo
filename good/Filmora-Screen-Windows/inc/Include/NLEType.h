#ifndef _H_NLETYPE_H_
#define _H_NLETYPE_H_

#if defined(WIN32)

#ifdef NLETYPE_EXPORTS
#define NLETYPE_API __declspec( dllexport ) 
#else
#define NLETYPE_API __declspec( dllimport )
#endif

#else //WIN32
#define NLETYPE_API
#endif //WIN32

//用于检测内存泄漏问题
//#define DETECTION_MEMORY_LEAKS 1 

#if DETECTION_MEMORY_LEAKS 
#include "vld.h"
#endif

#include <stdint.h>
#include <objbase.h>
#include "NLEType_c.h"




#ifndef _NLE_FUNC_DATA_DELETE_DEFINE_
#define _NLE_FUNC_DATA_DELETE_DEFINE_

typedef int (*NLE_FUNC_DATA_DELETE)(void *);

#endif

#ifndef _NLE_FRAME_INDEX_DEFINE__
#define _NLE_FRAME_INDEX_DEFINE__
/**
 *@brief 帧序号，范围>=0
 */
typedef int NLEFrameIndex;

typedef int NLEBOOL;

//#define FRAMEINDEX_MIN	-1
//#define NLEFrameIndexInvalid	-9999

#endif

#ifndef _NLE_EVENT_DEFINE_
#define _NLE_EVENT_DEFINE_

typedef struct tagNLEEVENT
{
	UINT eventID;
	WCHAR szDescription[100];
	tagNLEEVENT(UINT eid, LPCWSTR szDesp = NULL, void* pObj = NULL)
	{
		eventID = eid;
		if (szDesp) wcsncpy_s(szDescription, szDesp, 100);
		pObj = pObj;
	}
	void* pObj;
}NLEEVENT;

#endif

#ifndef _NLE_PROFILE_DEFINE_
#define _NLE_PROFILE_DEFINE_
typedef struct tagNLEProfile
{
	LPCWSTR szDescription;
	UINT uWidth;
	UINT uHeight;
	//
}NLEProfile;

#endif //_NLE_PROFILE_DEFINE_

#ifndef _NLE_RATIONAL_DEFINE_
#define _NLE_RATIONAL_DEFINE_

/**
 *@brief 有理数定义，适用于帧率换算,例如25fps，可表示为(1/25)
 */
class NLERational :public tagNLERational
{	
public:
	NLERational() 
	{
		num = 0; 
		den = 1;
	}
	NLERational(NLElonglong num, NLElonglong den)
	{
		this->num = num;
		this->den = den;
	}
	NLERational(double dval)
	{
		this->den = (INT)1E6;
		this->num = (int)(this->den * dval + 0.5);
	}

	NLERational(tagNLERational _cRational)
	{
		this->num = _cRational.num;
		this->den = _cRational.den;
	}

	NLERational &operator = (const NLERational &oRal)
	{
		this->num = oRal.num;
		this->den = oRal.den;
		return *this;
	}

	NLERational operator * (const NLERational &oRal)
	{
		NLERational ralResult;
		ralResult.num = this->num * oRal.num;
		ralResult.den = this->den * oRal.den;
        
		return ralResult;
	}

	NLERational operator / (const NLERational &oRal)
	{
		NLERational ralResult;
		ralResult.num = this->num * oRal.den;
		ralResult.den = this->den * oRal.num;
		return ralResult;
	}

	BOOL operator == (const NLERational &that) const
	{
		return (this->num * that.den == this->den * that.num);
	}

	BOOL operator != (const NLERational &that) const
	{
		return !(*this == that);
	}

	BOOL operator > (const NLERational &that)
	{
		return (this->num * that.den - this->den * that.num) > 0;
	}

	double Value()
	{
		return (this->num * 1.0f) / (this->den);
	}
	
	operator double()
	{
		return this->Value();
	}
};

typedef NLERational NLEFramerate;

#endif

#ifndef _NLE_BLOB_DATA_DEFINE_
#define _NLE_BLOB_DATA_DEFINE_

class NLETYPE_API NLEBlob
{
public:
	NLEBlob();
	NLEBlob(LPBYTE pData, UINT uSize, BOOL bCopy = FALSE, LPVOID pExtend = NULL);
	NLEBlob(const NLEBlob &v);
	~NLEBlob();
	NLEBlob& operator =(const NLEBlob& oBlob);
	void Clear();

public:
	UINT	uSize;
	LPBYTE	pData;
	BOOL	bCopy;	 //reserved filed
	LPVOID	pExtend; //reserved filed 
};

#endif//_NLE_BLOB_DATA_DEFINE


#ifndef _NLE_FRAME_FOMAT_DEFINE_
#define _NLE_FRAME_FOMAT_DEFINE_
/**
@brief 预留支持其他格式的视频帧，音频帧
*/
typedef enum enumNLEFrameFormat
{
    /**
    视频帧格式
    */
    /**
    BGRA, 32位，每个分量占8位。
    字节布局从低位到高位依次为
    0  1  2  3  4  5  6  7  ...
    B  G  R  A  B  G  R  A  ...
    */
    NLE_FRAME_FORMAT_BGRA, 
    /**
    BGR, 24位，每个分量占8位
    字节布局从低位到高位依次为
    0  1  2  3  4  5  6  7  ...
    B  G  R  B  G  R  B  G  ...
    */
    NLE_FRAME_FORMAT_BGR,
    NLE_FRAME_FORMAT_YUV420,

    /**
    音频帧格式
    */
    NLE_FRAME_FORMAT_PCM_S8 = 65, //8位
    NLE_FRAME_FORMAT_PCM_S16, //16位整型
    NLE_FRAME_FORMAT_PCM_S32, //32位整型

	/*! 压缩数据*/
	NLE_FRAME_FORMAT_COMPRESSED = 129,
}NLEFrameFormat;
#endif //_NLE_FRAME_FOMAT_DEFINE_


#ifndef _NLE_MEDIA_STREAM_TYPE_DEFINE_
#define _NLE_MEDIA_STREAM_TYPE_DEFINE_

typedef enum enumNLEMediaType
{
    /**非媒体流*/
	NLE_MEDIA_TYPE_NONE,
    
    /**DVD导航数据流*/
    NLE_MEDIA_TYPE_NAVIGATION,

    /**视频流*/
	NLE_MEDIA_TYPE_VIDEO, 

    /**音频流*/
	NLE_MEDIA_TYPE_AUDIO,    

    /**字幕流*/
    NLE_MEDIA_TYPE_SUBPIC,

    /**图片数据流*/
    NLE_MEDIA_TYPE_IMAGE,

	/**鼠标信息流*/
	NLE_MEDIA_TYPE_MOUSE,

    /**键盘信息流*/
    NLE_MEDIA_TYPE_KEYBOARD,
    
}NLEMediaType;

#endif


typedef enum enumNLEFileType
{
	NLE_FILE_TYPE_UNKNOWN = 0, //不支持的文件类型
	NLE_FILE_TYPE_VIDEO = 1, //视频文件，至少包含一个视频流
	NLE_FILE_TYPE_AUDIO = 2, //音频文件，至少包含一个音频流，且不包含视频流
	NLE_FILE_TYPE_IMAGE = 3, //图片文件
}NLEFileType;


#ifndef NLEVideoInfo
typedef struct NLEVideoInfo
{
    RECT                rcSource;
    RECT                rcTarget;
    int                 nBitrate;

    int64_t             AvgTimePerFrame; //Each unit of reference time is 100 nanoseconds
    
    NLEFrameFormat      format;
    int                 nWidth;
    int                 nHeight;
	BOOL				isOpaque;	//是否为不透明
} NLEVideoInfo;
#endif

#ifndef NLEAudioInfo
typedef struct NLEAudioInfo
{
    NLEFrameFormat      format;
    WORD  nChannels;
    DWORD nSampleRate;
    DWORD nAvgBytesPerSec;
    WORD  nBlockAlign;
    WORD  wBitsPerSample;
} NLEAudioInfo;
#endif

#ifndef NLEMediaInfo
typedef struct  NLEMediaInfo {
    NLEMediaType type;
	int64_t   nFrameIndex; //仅NLE Platform内部使用，外部程序请勿使用nFrameIndex属性
	int	      nMediaFrameNumber;	//媒体源原始帧号
	NLEFrameIndex  frmIdxSyncPoint;
    union
    {
        NLEVideoInfo video;
        NLEAudioInfo audio;
    };
	
} NLEMediaInfo;
#endif

#ifndef NLEShiftPoint
typedef struct NLEShiftPoint
{
	double x;
	double y;
	NLEShiftPoint()
	{
		x = y = 0.0;
	}
	NLEShiftPoint(double dx, double dy)
	{
		x = dx;
		y = dy;
	}
}NLEShiftPoint;
#endif


#ifndef NLETransformMatrix
typedef double NLETransformMatrix[3][3];
#endif //

#ifndef NLERenderMode
typedef enum
{
	//完全填充，Resize到填充区域大小,但仍保持原长宽比例，其余部分透明黑色填充。
	RenderMode_Fill,

	//直接贴图，Frame的中心与AnchorPoint重合
	RenderMode_Direct,

	//拉伸贴图
	RenderMode_Stretch,
}NLERenderMode;
#endif
#include <vector>
typedef std::vector<LPCWSTR> ARRAY_WSTR;
typedef std::pair<LPCWCHAR, LPCWCHAR> PAIRWSTR;
typedef std::vector<PAIRWSTR> ARRAY_PAIRWSTR;
typedef std::vector<GUID> GUIDArray;

#ifndef NLEWParam
typedef LONGLONG NLEWParam;
#endif

#ifndef NLELParam
typedef LONGLONG NLELParam;
#endif

#ifndef NLESession
typedef INT NLESession;
#endif

#endif //_H_NLETYPE_H_
