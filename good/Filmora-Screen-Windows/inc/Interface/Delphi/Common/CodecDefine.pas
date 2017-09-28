unit CodecDefine;

{$MINENUMSIZE 4}

interface

uses
  Windows;

type

{$IFNDEF _DEC_MODEL_DEFINE}
{$DEFINE _DEC_MODEL_DEFINE}
  DEC_MODEL =         // 定义解码方式
  (
    AUTO_MODEL =0,
    DSHOW_MODEL,
    FFMPEG_MODEL,
    DVD_MODEL,			// 仅针对VOB文件
	AVDEC_MODEL,			// WS_AVDec.dll 基于WS Video Decoder Platform
  IMAGE_MODEL       // 图片解码器
  );
{$ENDIF}  //  _DEC_MODEL_DEFINE


{$IFNDEF _AUDIO_INTERPOLATION_DEFINE}
{$DEFINE _AUDIO_INTERPOLATION_DEFINE}
  AUDIO_INTERPOLATION =       // 音频采样率转换插值算法
  (
    SIMPLE_LINEAR = 0,        // 一次线性插值
    QUADRATIC_LINEAR  	      // 二次线性插值(暂未实现)
  );
{$ENDIF}  //  _AUDIO_INTERPOLATION_DEFINE


{$IFNDEF IMAGE_INTERPOLATION_DEFINE}
{$DEFINE IMAGE_INTERPOLATION_DEFINE}
  IMAGE_INTERPOLATION =       // 图像插值算法
  (
	  IIN_AUTO,					        // 自动选择插值算法
    IIN_FAST,			            // Nearest value (邻近插值)
    IIN_BI_LINER,		          // Bi-Liner Interpolation (双线性插值)
    IIN_TRI_CONVOLUTE	        // Tri-Convolute Interpolation (卷积插值)
  );
{$ENDIF}  //  IMAGE_INTERPOLATION_DEFINE


{$IFNDEF IMAGE_RESIZE_METHOD_DEFINE}
{$DEFINE IMAGE_RESIZE_METHOD_DEFINE}
  IMAGE_RESIZE_METHOD =
  (
    IRM_FULLSCREEN,		        // stretch to full screen
    IRM_ORIGINAL_SCALE,       // keep scale, fit to size of output
    IRM_16_9,			            // resize to 16:9, fit to size of output
    IRM_4_3,			              // resize to 4:3, fit to size of output
 	IRM_LETTERBOX,
	IRM_PAN_SCAN
 );
{$ENDIF}  //  IMAGE_RESIZE_METHOD_DEFINE

{$IFNDEF DEINTERLACE_INTERPOLATION_DEF}
{$DEFINE DEINTERLACE_INTERPOLATION_DEF}
 DEINTERLACE_STYLE =
(
	DIS_NONE,				
	DIS_ODD,				// 重复奇数行
	DIS_EVEN,				// 重复偶数行
	DIS_ODD_INTERPOLATE,	// 使用奇数行插值
	DIS_EVEN_INTERPOLATE,	// 使用偶数行插值
	DIS_AUTO				// 自动处理（暂不支持）
);
{$ENDIF}  //  DEINTERLACE_INTERPOLATION_DEF


{$IFNDEF _DOUBLE_RECT_DEFINE}
{$DEFINE _DOUBLE_RECT_DEFINE}
  D_RECT = record            // 定义位置相对坐标，参考值为输出图像分辨率
    dTop      : Double;      // 顶部坐标，为相对坐标，取值 0.0～1.0，下同
    dBottom   : Double;
    dLeft     : Double;
    dRight    : Double;
  end;
{$ENDIF}  //  _DOUBLE_RECT_DEFINE


{$IFNDEF _CLIP_TIME_DEFINE}
{$DEFINE _CLIP_TIME_DEFINE}
  CLIP_TIME = record
    dStart     : Double;      // 相对输出（音）视频的起止时间，单位：秒，若 剪切长度 != 输出长度，则会产生拉伸效果
    dEnd       : Double;      // 此值为零则无拉伸效果
    dTrimFrom  : Double;      // 剪切时间段之起点，单位：秒，下同
    dTrimTo    : Double;      // 剪切时间段之终点，0值表示尾端不进行剪切
  end;
{$ENDIF}  //  _CLIP_TIME_DEFINE

  MediaType =
  (
	  AUDIO_TYPE,
	  VIDEO_TYPE
  );

implementation

end.