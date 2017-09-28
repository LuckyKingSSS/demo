// AudioSlideParam.pas

{
	   音频合成的处理参数
}

unit AudioSlideParams;

interface

uses
  Windows, DecoderParam, CodecDefine;

type

  AudioSlideParam = record
    nSampleRate       : Integer;
    nChannels         : Integer;
    nBitsPerSample    : Integer;              // 采样深度，目前仅支持16位
    nInterpolation    :	AUDIO_INTERPOLATION;  // 采样率转换插值算法
  end;
  PAudioSlideParam = ^AudioSlideParam;

  VolumEffect = record			  // 音量特效
	  bUseEffect    : BOOL;		  // 是否使用特效
	  dStartTime    : Double;		// 特效起止时间，单位：秒，以输出时间尺度为为准，有如下关系：
		dEndTime      : Double;		//				0.0 <= dStartTime < dEndTime <= dTimeEnd - dTimeStart
		nStartVoulm   : Integer;	// 开始音量
		nEndVoulm     : Integer;	// 结束音量
		nEffectID     : Integer;	// 过渡算法ID
  end;
  PVolumEffect = ^VolumEffect;

  AudioProcessParam	= record		  // 音频处理参数，注意：nVolum与Start/EndEffect效果有叠加，即若nVolum = 90,
                								  // StartEffect.nStartVoulm = 80,则最终开始音量为  原音量×90%×80% ＝ 原音量×72%
    Time          : CLIP_TIME;	  // 时间设置
    nVolum        : Integer;			// 音量,0～200, 100保持原音量
    StartEffect   : VolumEffect;	// 音频开始部分过渡效果（暂不使用）
    EndEffect     : VolumEffect;	// 音频结尾部分过渡效果（暂不使用）
    DecMod        : DEC_MODEL;    // 解码方式
    audDecParam   : DecParam;			// 音频解码参数
 end;
  PAudioProcessParam = ^AudioProcessParam;

implementation

end.