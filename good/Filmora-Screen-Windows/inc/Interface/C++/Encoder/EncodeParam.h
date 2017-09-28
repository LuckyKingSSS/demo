#pragma once

#include "MetaDataInfo.h"
#include "CodecDefine.h"
#include "EncodeParamEx.h"

//扩展编码参数
#define RESERVEDSIZE 1024
struct VideoParamEx
{
    DWORD	dwFourCC;	// 视频编码器 FourCC ： '1VMW'/'2VMW'/'3VMW' /'DIVX'/'462H'
    union 
    {
        WMV_VideoParamEx m_wmvParamEx;
        VC1_VideoParamEx m_vc1ParamEx;
        Xvid_EncodeParamEx m_xvidParamEx;
		H264_VideoParamEx m_h264ParamEx;
		AppleProRes_EncodeParamEx m_proresParamEx;
        int reserved[RESERVEDSIZE]; //保留字段 为了分配固定大小结构
    };
};

struct AudioParamEx
{ 
    DWORD	dwFourCC;	// 音频编码器 FourCC ： '1AMW'/'2AMW'/'3AMW' /' 3PM'  
    union 
    {
        int reserved[RESERVEDSIZE/2]; //保留字段 为了分配固定大小结构
    };
};

struct EncodeParamEx
{
    DWORD dwFourCC;			    // 文件格式FourCC
    int     useflg;             //有效标志  0  未使用 1 视频扩展参数  2音频扩展参数 3 音视频扩展参数
    int m_TwoPass;              //二次编码标志  0 一次编码; 1 二次编码只送一次数据; 2 二次编码送二次数据      默认为0 
    VideoParamEx videoEx;		// 视频扩展参数
    AudioParamEx audioEx;		// 音频扩展参数
};

struct VideoParam
{
	DWORD	dwFourCC;	// 视频编码器 FourCC ： '1VMW'/'2VMW'/'3VMW' /'DIVX'/'462H'
	int		nWidth;		// 分辨率宽度
	int		nHeight;	// 分辨率高度
	double  dFrameRate;	// 帧率 0表示自动选择
	int		nBitrate;	// 码率 bps （恒定码率、可变最小码率）
	int	bIsVBR;		// 是否使用变码率
	int		nMaxBitrate;// 最大码率
	IMAGE_RESIZE_METHOD nResizeStyle;	// 图像缩放方式
	IMAGE_INTERPOLATION nInterpolation;	// 图像插值算法
	DWORD	dwCompression;				// 图像格式
	int		nBitsDepth;					// 图像位深度

	//=============== MPEG 编码器参数 ===========================//
	// 使用Mpeg编码请参考 #include "mpeg_param.h"                // 
	DWORD   dwFormat;   // 编码格式                              // 
	DWORD   dwNorm;     // 电视制式                              //  
	DWORD   dwAspect;   // 比例                                  // 
	//int    bFieldEnc;  // 是否使用交错模式                      //
	//=============== MPEG 编码器参数 ===========================//

	int	nVideoRotate;			//File Rotate angle for play,value{90,180,270}
	
    // 视频扩展参数
    EncodeParamEx VideoParamEx;
};

struct AudioParam
{
	DWORD	dwFourCC;	// 音频编码器 FourCC ： '1AMW'/'2AMW'/'3AMW' /' 3PM'
	int		nSampleRate;// 采样率 Hz
	int		nChannels;	// 声道数量
	int		nBitrate;	// 码率 bps （恒定码率、可变最小码率）
	int	bIsVBR;		// 是否使用变码率
	int		nMaxBitrate;// 最大码率
	int		nBitsPerSample;	// Number of bits per sample of mono data

    // 音频扩展参数
    EncodeParamEx AudioParamEx;
};

// 编码参数
struct EncodeParam
{
	VideoParam video;		    // 视频参数
	AudioParam audio;		    // 音频参数
	int bVideoDisable;		    // 禁用视频
	int bAudioDisable;		    // 禁用音频

	DWORD dwFourCC;			    // 文件格式FourCC
    MetaDataInfo metaDataInfo;  // ID3等文件信息
};

struct EncoderInfo
{
	DWORD		dwFourCC;
	wchar_t		szEncName[64];
	wchar_t		szDescription[128];
};

struct FileFormatInfo
{
	DWORD			dwFourCC;
	wchar_t			szExtName[10];
	wchar_t			szFormatName[64];
	wchar_t			szDescription[128];
	int				nAudioEncs;
	int				nVideoEncs;
	EncoderInfo**	ppAudioEncInfos;
	EncoderInfo**	ppVideoEncInfos;
};

// 编码时的预览使用回调函数
typedef BOOL (__stdcall * ENCODE_CALLBACK)(void* pUserObj, UINT uMsg, WPARAM wParam, LPARAM lParam);
// uMsg -    0 进度消息  wParam - 进度(0-10000)  lParam - ProgressInfo*
//           1 转换完成  wParam - 0 正常完成  1 用户终止 2 外部回掉终止 3 遭遇错误
//         100 第二次进度开始 
//           101 进度消息  wParam - 进度(0-10000)  lParam - 0
//         110 第二次进度结束 在第二次进度期间 不能调用WEPause和WEStop函数
// 返回FALSE 则停止转换

