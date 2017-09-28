/* ### WS@H Project:Multimedia_Platform ### */

#pragma once
#include <typedef.hpp>
#include <vector>
using namespace std;

#define AV_TIME_BASE            1000000

enum E_AV_SEEK_TYPE
{
	E_AV_SEEK_KEYFRAME, 
	E_AV_SEEK_FILEPOS, 
	E_AV_SEEK_TIMEPOS,	
	E_AV_SEEK_FRAMEPOS	
};

enum E_AV_SCAN_TYPE
{
	E_AV_SCAN_TYPE_PROGRESSIVE, ///< 逐行模式

	E_AV_SCAN_TYPE_INTERLACED, ///< 交织模式

	E_AV_SCAN_TYPE_UNKNOWN,	///< 模式未知

	E_AV_SCAN_TYPE_RESERVED	///< 保留字段

};

enum E_AV_STREAM_TYPE
{
	/*!非媒体流*/
	E_ST_NONE,

	/*! 视频Stream*/
	E_ST_VIDEO,		// 

	/*! 音频Stream*/
	E_ST_AUDIO,	

	E_ST_SUBPIC_Internal,
	/*! 字幕Stream*/
	E_ST_SUBPIC_External,

	/*! 图像Stream*/
	E_ST_IMAGE,
};


enum E_DEMUXDATA_TYPE
{
	E_DEMUXDATA_COMPRESS,
	E_DEMUXDATA_UNCOMPRESS
};

enum E_DEMUXWE_TYPE
{
	E_DEMUXWE_SINGLE,
	E_DEMUXWE_MULTI
};

enum SEEK_CAPACITY
{
	SEEK_CAPACITY_UNKNOW    =    0 ,
	SEEK_CAPACITY_NONE      =    1 ,
	SEEK_CAPACITY_TIME      =    2 ,
	SEEK_CAPACITY_FILEPOS   =    4 ,

};

typedef struct _ST_AVRational{
	int num; ///< numerator
	int den; ///< denominator
} ST_AVRational;

struct ST_AudioInfo
{
	/*! 采样率 */
	int sample_rate;

	/*! 声道数 */
	int channels;

	/*! 每个sample占用位数 */
	int bitspersample;	

	/*! 每秒字节数 */
	int bytespersecond;	

	/*! 时间长度 */
	double length;	

	/*! 媒体特性，由MEDIA_CAPABILITIES各值或运算而成，见MEDIA_CAPABILITIES定义 */
	DWORD dwCapsFlag;		
};


// 视频信息
struct ST_VideoInfo
{
	/*! 宽度*/
	int width;

	/*! 高度 */
	int height;

	/*! 帧率 */
	double frame_rate;	

	/*! x方向比例 */
	int xratio;	

	/*! y方向比例 */
	int yratio;	

	/*! 最大帧率 */
	double frame_rate_max;

	/*! 帧率模型 */
	int nIsVBR;

	UINT            nVideoProfile;

	UINT            nVideoLevel;

	UINT            nMaxBFrames;
};

struct ST_StreamInfo
{
	/*! 流类型*/
	E_AV_STREAM_TYPE	uStreamType;

	UINT			uStreamIndex;

	/*! 流ID*/
	UINT			uStreamID;

	/*! 流CodecID*/
	DWORD			dwCodecID;

	wchar_t	szDescription[128];

	/*! 流的时间长度*/
	double			dStreamLength;

	/*! 流的码率*/
	int				nBitrate;

	/*! 流的视频信息， 当uStreamType为ST_VIDEO时有效*/
	ST_VideoInfo 	vidInfo;

	/*! 流的音频信息， 当uStreamType为ST_AUDIO时有效*/
	ST_AudioInfo 	audInfo;	

	/*! 当前流的第一个时间戳*/
	__int64			ptsStart;	

	/*! 当前流的第一个时间戳和该路节目所有流中ptsStart最小的那个ptsStart的差值*/
	__int64			ptsOffset;	

	/*! 当前流的隔行或逐行扫描 */
	E_AV_SCAN_TYPE	scanType;

	/*! 当前流的所对应的ExtraData的长度*/
	UINT            nExtraDataLength;
	/*! 当前流的所对应的ExtraData的地址*/
	unsigned char   *pExtraDataBuffer;

	ST_AVRational    sample_ratio_video;




	ST_StreamInfo()
	{
		memset(this,0,sizeof(ST_StreamInfo));
		uStreamType = E_ST_NONE;
		uStreamID   = 0xFFFFFFFF;
		dwCodecID    = 0;
		dStreamLength = 0.0;
		nBitrate    = 0;
		ptsOffset   = 0;
		ptsStart    = 0;
		scanType    = E_AV_SCAN_TYPE_UNKNOWN;
		nExtraDataLength = 0;
		pExtraDataBuffer = 0;
	}
};


struct ST_ProgramInfo
{
	/*! 节目流ID*/
	UINT				uPID;

	/*! 节目的时长*/
	double				dMediaLength;

	/*! 节目流的码率*/
	int					nBitrate;

	/*! 节目流中视频流的数目*/
	int					nVideoStreamCount;

	/*! 节目流中音频流的数目*/
	int					nAudioStreamCount;

	/*! 节目流中字幕流的数目*/
	int					nSubPicStreamCount;

	/*! 节目流中视频信息*/
	ST_StreamInfo*			pVidStreamInfos;

	/*! 节目流中音频信息*/
	ST_StreamInfo*			pAudStreamInfos;

	/*! 节目流中字幕流信息*/
	ST_StreamInfo*	pSubPicInfos;
};


struct ST_MEDIA_FILE_INFO
{
	/*! 文件名*/
	wchar_t			szName[512];

	/*! 媒体文件时间长度(取各个节目流中，最长的那路)*/
	double			dMediaLength;

	/*! 媒体文件的码率，(各路节目流码率之和)*/
	int				nBitrate;

	/*! 媒体文件的节目数*/
	int				nProgramCount;

	BOOL            HaveNoProgram;

	/*! 媒体文件的节目信息*/
	ST_ProgramInfo*	pProgramInfos;

};

struct KeyFrameIndex
{
	int index;
	LONGLONG pts;
	LONGLONG offset;
};

struct KeyFrameIndexArr
{
	__int64 nCount;
	vector<KeyFrameIndex> keyFrameIdArr;
};


struct ST_VideoFrame 
{
	int width;
	int height;
	BYTE *y;
	BYTE *u;
	BYTE *v;
	int y_pitch;
	int u_pitch;
	int v_pitch;

};

struct ST_AudioFrame 
{
	int samplerate;
	int bitdepth;
	int channel;
	BYTE *pdata;
	int pdatasize;
	int maxlen;

};

struct ST_MediaFrame
{
	E_AV_STREAM_TYPE type;
	long long pts;
	long long dts;
	long duration;
	union
	{
		ST_VideoFrame video;
		ST_AudioFrame audio;

	};


};

ST_MediaFrame* CreateVideoFrame(int width,int height);
void ReleaseVideoFrame(ST_MediaFrame* frame);
ST_MediaFrame* CreateAudioFrame(int samplerate,int channel);
void ReleaseAudioFrame(ST_MediaFrame* frame);


BOOL __stdcall IsCodecSupport(DWORD id);
UINT UnFixtMediaStreamID(UINT id);