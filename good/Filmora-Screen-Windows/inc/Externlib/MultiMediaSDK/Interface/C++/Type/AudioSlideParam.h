// AudioSlideParam.h

/*
	   音频合成的处理参数
*/

#pragma once

#include "DecodeParam.h"
#include "CodecDefine.h"


struct AudioSlideParam
{
	int					nSampleRate;
	int					nChannels;
	int					nBitsPerSample; // 采样深度，目前仅支持16位
	AUDIO_INTERPOLATION	nInterpolation;	// 采样率转换插值算法
};

struct VolumEffect			// 音量特效
{
	int	bUseEffect;		// 是否使用特效
	double	dStartTime;		// 特效起止时间，单位：秒，以输出时间尺度为为准，有如下关系：
	double	dEndTime;		//				0.0 <= dStartTime < dEndTime <= dTimeEnd - dTimeStart
	int		nStartVoulm;	// 开始音量
	int		nEndVoulm;		// 结束音量
	int		nEffectID;		// 过渡算法ID
};

struct AudioProcessParam			// 音频处理参数，注意：nVolum与Start/EndEffect效果有叠加，即若nVolum = 90,  
{									// StartEffect.nStartVoulm = 80,则最终开始音量为  原音量×90%×80% ＝ 原音量×72%
	CLIP_TIME		Time;			// 时间设置
	int				nVolum;			// 音量,0～200, 100保持原音量
	VolumEffect		StartEffect;	// 音频开始部分过渡效果（暂不使用）
	VolumEffect		EndEffect;		// 音频结尾部分过渡效果（暂不使用）
	DEC_MODEL		DecMod;			// 解码方式
	DecParam		decParam;		// 音频解码参数
};
