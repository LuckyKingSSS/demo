#pragma once

#include "wstypedef.h"
//WMV格式的扩展参数
struct WMV_VideoParamEx
{
   int m_Quality ;                //质量等级   0-4 4为最好 默认为0
   int m_VBRType;                  // 可变长编码类型 m_VBRType 0 1 默认为0 动态码率VBR 0 平均码率ABR 1(暂时不支持) 
   int m_MaxBuffer;               //缓冲大小   0~10秒 默认为0
   int m_IFrameInterval;          //默认为25
};
//AVC Profiles 
enum WS_CodeID_H264_PROFILE 
{
	WS_CodeID_H264_PROFILE_UNKNOWN		 =0,
	WS_CodeID_H264_PROFILE_AVC_BASELINE    =66,
	WS_CodeID_H264_PROFILE_AVC_MAIN        =77,
	WS_CodeID_H264_PROFILE_AVC_HIGH        =100,
};

enum WS_CodeID_H264_LEVEL 
{
	WS_CodeID_H264_LEVEL_UNKNOWN			 =0,
	WS_CodeID_H264_LEVEL_AVC_1             =10,
	WS_CodeID_H264_LEVEL_AVC_1b            =9,
	WS_CodeID_H264_LEVEL_AVC_11            =11,
	WS_CodeID_H264_LEVEL_AVC_12            =12,
	WS_CodeID_H264_LEVEL_AVC_13            =13,
	WS_CodeID_H264_LEVEL_AVC_2             =20,
	WS_CodeID_H264_LEVEL_AVC_21            =21,
	WS_CodeID_H264_LEVEL_AVC_22            =22,
	WS_CodeID_H264_LEVEL_AVC_3             =30,
	WS_CodeID_H264_LEVEL_AVC_31            =31,
	WS_CodeID_H264_LEVEL_AVC_32            =32,
	WS_CodeID_H264_LEVEL_AVC_4             =40,
	WS_CodeID_H264_LEVEL_AVC_41            =41,
	WS_CodeID_H264_LEVEL_AVC_42            =42,
	WS_CodeID_H264_LEVEL_AVC_5             =50,
	WS_CodeID_H264_LEVEL_AVC_51            =51,
};
struct H264_VideoParamEx
{
	WS_CodeID_H264_PROFILE   dwProfile;        //0表示不采用外部设置；
	WS_CodeID_H264_LEVEL   dwComplexityLevel;//0表示不采用外部设置；
};

struct VC1_VideoParamEx
{
	unsigned int   dwProfile;
	unsigned int   dwComplexityLevel;
	unsigned int   dwQP;
	unsigned int   dwMaxKeyFrameDistance;
	unsigned int   dwNumOfBFrames;
	unsigned int   dwRateControlMode;
	unsigned int   dwVBVBufferInBytes;
	double          dPeakBitRate;
};

/* XVID高级参数设置 */
struct Xvid_EncodeParamEx
{
    int Quality;
    int VHQMODE;
};