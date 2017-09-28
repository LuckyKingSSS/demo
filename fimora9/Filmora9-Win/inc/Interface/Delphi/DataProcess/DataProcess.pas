unit DataProcess;

{
功能：
支持图像亮度、对比度、饱和度，音量调节； 
支持灰度、底片、浮雕等特效； 
支持支持图像剪裁（去处四周黑边）； 
支持图像翻转功能；
支持BMP、JPG抓图； 

支持水印放任意位置，支持水印透明度调节。
}

{$WARNINGS OFF}
{$Z4}

interface

uses
  Windows, EncParam, CodecDefine;

// 扩展滤镜ID定义 
// EX_FILTER_ID
const 
	EX_FILTER_GRAY			  = $0001;
	EX_FILTER_EMBOSS		  = $0002;
	EX_FILTER_NEGATIVE		= $0004;
	EX_FILTER_BLACK_WHITE	= $0008;
	EX_FILTER_WHITE_BLACK	= $0010;
	EX_FILTER_OLD_PHOTO		= $0020;

// FLIP_FLAG
const
	FLIP_HORIZONTAL			  = $0001;
	FLIP_VERTICAL			    = $0002;


//typedef void * HVIDEODECODER;
//typedef void * HAUDIODECODER;
type
  HVIDEODECODER = Pointer;
  HAUDIODECODER = Pointer;
  
  RESIZE_PARAM = record
    width           :Integer;
    height          :Integer;
    uInterpolation  :IMAGE_INTERPOLATION;
    uStyle          :IMAGE_RESIZE_METHOD;
  end;
  PRESIZE_PARAM = ^RESIZE_PARAM;

// liuqi 2008/7/24
// 该数据结构在CodecDefine中已经定义。（需要调用CodecDefine头文件）
/////////////////////////////////////////////
//{$IFNDEF DEINTERLACE_INTERPOLATION_DEF}
//{$DEFINE DEINTERLACE_INTERPOLATION_DEF}
// DEINTERLACE_STYLE =
//(
//	DIS_NONE,
//	DIS_ODD,				// 重复奇数行
//	DIS_EVEN,				// 重复偶数行
//	DIS_ODD_INTERPOLATE,	// 使用奇数行插值
//	DIS_EVEN_INTERPOLATE,	// 使用偶数行插值
//	DIS_AUTO				// 自动处理（暂不支持）
//);
//{$ENDIF}  //  DEINTERLACE_INTERPOLATION_DEF
//////////////////////////////////////////////

//创建数据处理视频解码器，传源视频解码器
// dFrameRate - >= 0 使用帧率转换和时间戳校正功能（同时时间戳归0处理）   = 0 使用原始帧率     < 0 不使用帧率转换和时间戳校正功能 
//HVIDEODECODER PCVideoCreate(HVIDEODECODER hSrcVideo, const RESIZE_PARAM* pResizeParam = 0, double dFrameRate = -1);
function PCVideoCreate(hSrcVideo: HVIDEODECODER; const pResizeParam : PRESIZE_PARAM = nil; dFrameRate : double = -1): HVIDEODECODER; stdcall;

//销毁数据处理视频解码器
//void PCVideoDestroy(HVIDEODECODER hVideo);
procedure PCVideoDestroy(hVideo: HVIDEODECODER); stdcall;

//创建数据处理音频解码器，传源音频解码器
// bEvenFlag - TRUE 使用时间戳校正功能（同时时间戳归0处理）
//HAUDIODECODER PCAudioCreate(HAUDIODECODER hSrcAudio, int nSampleRate = 0, int nChannels = 0, int nBitsPerSample = 0, BOOL bEvenFlag = FALSE);
function PCAudioCreate(hSrcAudio: HAUDIODECODER; nSampleRate : Integer = 0; nChannels : Integer = 0; nBitsPerSample : Integer = 0; bEvenFlag : BOOL = FALSE): HAUDIODECODER; stdcall;

//销毁数据处理音频解码器
//void PCAudioDestroy(HAUDIODECODER hAudio);
procedure PCAudioDestroy(hAudio: HAUDIODECODER); stdcall;

//设置剪裁区域,rc——剪裁区域,全0为不进行剪裁操作
//void PCSetCropRect(HVIDEODECODER hVideo, const RECT* pCropRect);
procedure PCSetCropRect(hVideo: HVIDEODECODER; const pCropRect: PRect); stdcall;

//设置光亮度，-100～100， 0保持原亮度
//void PCSetVideoBrightness(HVIDEODECODER pVideo, int nValue);
procedure PCSetVideoBrightness(hVideo: HVIDEODECODER; nValue: Integer); stdcall;

//设置对比度，-100～100， 0保持原对比度
//void __stdcall PCSetContrast(HVIDEODECODER hVideo, int nValue);
procedure PCSetContrast(hVideo: HVIDEODECODER; nValue: Integer); stdcall;

//设置饱和度,-100~100, 0保持原饱和度
//void PCSetVideoSaturation(HVIDEODECODER pVideo, int nValue);
procedure PCSetVideoSaturation(hVideo: HVIDEODECODER; nValue: Integer); stdcall;

// 指定水印资源ID
// pRect - 相对于剪裁后的图像分辨率中的位置
// nOpacity - 水印不透明度 (0 - 100)  0 - 完全透明  100 - 完全不透明
//void __stdcall PCSetWatermarkSrcID(HVIDEODECODER hVideo, HMODULE hModule, const wchar_t * pID, const LPRECT pRect = 0, int nOpacity = 100);
procedure PCSetWatermarkSrcID(hVideo: HVIDEODECODER; hModule: Cardinal; const pID: PWideChar; const pRect: PRECT = nil; nOpacity: Integer = 100); stdcall;

//void __stdcall PCSetWatermarkSrcIDAlign(HVIDEODECODER hVideo, HMODULE hModule, const wchar_t * pID, int nPos = 0, int nOpacity = 100);
procedure PCSetWatermarkSrcIDAlign(hVideo: HVIDEODECODER; hModule: Cardinal; const pID: pWideChar; nPos: Integer = 0; nOpacity: Integer = 100); stdcall;
//void __stdcall PCDisableWatermark(HVIDEODECODER hVideo, BOOL bDisable);
procedure PCDisableWatermark(hVideo: HVIDEODECODER; bDisable: BOOL); stdcall;

//void __stdcall PCSetWatermarkImage(HVIDEODECODER hVideo, LPBITMAPINFO lpbi, LPBYTE lpBits, const LPRECT pRect = 0, int nOpacity = 100); // 支持32位透明图像 pRect - 相对于剪裁后的图像分辨率中的位置
procedure PCSetWatermarkImage(hVideo: HVIDEODECODER; lpbi: PBITMAPINFO; lpBits: PBYTE; const pRect: PRECT = nil; nOpacity: Integer = 100); stdcall;

//void __stdcall PCSetWatermarkImageAlign(HVIDEODECODER hVideo, LPBITMAPINFO lpbi, LPBYTE lpBits, int nPos = 0, int nOpacity = 100); // 支持32位透明图像 nPos - 相对于剪裁后的图像分辨率中的位置
procedure PCSetWatermarkImageAlign(hVideo: HVIDEODECODER; lpbi: PBITMAPINFO; lpBits: PBYTE; nPos: Integer = 0; nOpacity: Integer = 100); stdcall;


{$IFNDEF FLOAT_RECT_DEF}
{$DEFINE FLOAT_RECT_DEF}
type
  FRECT = record
    left    :Double;
    top     :Double;
    right   :Double;
    bottom  :Double;
  end;
  PFRECT = ^FRECT;
{$ENDIF}

// 支持32位透明图像 pFRect - 相对于剪裁缩放后的图像分辨率中的百分比位置  1表示100%
// 设置lpbi、lpBits为0，表示取消水印图片
// 设置lpbi、lpBits为0xFFFFFFFF，表示保留原来的水印图片，只改变水印位置。
//void __stdcall PCSetUserWatermarkImage(HVIDEODECODER hVideo, LPBITMAPINFO lpbi, LPBYTE lpBits, const PFRECT pFRect = 0, int nOpacity = 100);
procedure PCSetUserWatermarkImage(hVideo: HVIDEODECODER; lpbi: PBITMAPINFO; lpBits: PBYTE; const pFRect: PFRECT = 0; nOpacity: Integer = 100); stdcall;

//音量,0~200, 100保持原音量
//void PCSetVolum(HAUDIODECODER pAudio, int nValue);
procedure PCSetVolum(hAudio: HAUDIODECODER; nValue: Integer); stdcall;

// 抓图 
// pFileName： 文件名
// nFormat： 文件格式 0 - BMP  1 - JPG 
//BOOL __stdcall PCCaptureImage(HVIDEODECODER hVideo, wchar_t * pFileName, int nFormat, bWaterMark);
function PCCaptureImage(hSrcVideo: HVIDEODECODER; pFileName: PWideChar; nFormat : Integer; bWaterMark : BOOL): BOOL; stdcall;

	
// 设置扩展滤镜参数 uFlag取EX_FILTER_ID各值或运算 推荐使用单一Filter
//void __stdcall PCSetExFilterFlag(HVIDEODECODER hVideo, UINT uFlag);
procedure PCSetExFilterFlag(hVideo: HVIDEODECODER; uFlag: UINT); stdcall;

// FLIP_FLAG
//#define FLIP_HORIZONTAL		0x0001
//#define FLIP_VERTICAL			0x0002
// 图像翻转设置
// uFlag - 由FLIP_FLAG或运算而成
//void __stdcall PCSetFlipFlag(HVIDEODECODER hVideo, UINT uFlag);
procedure PCSetFlipFlag(hVideo: HVIDEODECODER; uFlag: UINT); stdcall;

// 去除隔行扫描功能
// 如果在DecParam参数中指定了Deintelace功能，这里不应该再重复处理
//void __stdcall PCSetVideoDeintelace(HVIDEODECODER hVideo, DEINTERLACE_STYLE uDeintelace);
procedure PCSetVideoDeintelace(hVideo: HVIDEODECODER; uDeintelace: DEINTERLACE_STYLE); stdcall;

//定位到dTime 
//void __stdcall PCVideoSeek(HVIDEODECODER hVideo, double dTime, BOOL accurate = TRUE);
procedure PCVideoSeek(hVideo: HVIDEODECODER; dTime: double; accurate : BOOL = TRUE); stdcall;

//void __stdcall PCAudioSeek(HAUDIODECODER hAudio, double dTime);
procedure PCAudioSeek(hAudio: HAUDIODECODER; dTime: double); stdcall;


implementation

const
  DLLNAME = 'WS_DataProcess.dll';

function  PCVideoCreate; external DLLNAME  index 1;
procedure PCVideoDestroy; external DLLNAME index 2;
function  PCAudioCreate; external DLLNAME  index 3;
procedure PCAudioDestroy; external DLLNAME index 4;
procedure PCSetCropRect; external DLLNAME  index 5;
procedure PCSetVideoBrightness; external DLLNAME Name 'PCSetVideoBrightness';
procedure PCSetContrast; external DLLNAME Name 'PCSetContrast';
procedure PCSetVideoSaturation; external DLLNAME Name 'PCSetVideoSaturation';
procedure PCSetWatermarkSrcID; external DLLNAME index 9;
procedure PCSetWatermarkSrcIDAlign; external DLLNAME Name 'PCSetWatermarkSrcIDAlign';
procedure PCSetVolum; external DLLNAME Name 'PCSetVolum';
procedure PCDisableWatermark; external DLLNAME index 11;
function  PCCaptureImage; external DLLNAME index 12;
procedure PCSetExFilterFlag; external DLLNAME Name 'PCSetExFilterFlag';
procedure PCSetFlipFlag; external DLLNAME Name 'PCSetFlipFlag';
procedure PCSetWatermarkImage; external DLLNAME Name 'PCSetWatermarkImage';
procedure PCSetWatermarkImageAlign; external DLLNAME Name 'PCSetWatermarkImageAlign';
Procedure PCSetUserWatermarkImage; external DLLNAME Name 'PCSetUserWatermarkImage';
procedure PCSetVideoDeintelace; external DLLNAME Name 'PCSetVideoDeintelace';
procedure PCVideoSeek; external DLLNAME Name 'PCVideoSeek';
procedure PCAudioSeek; external DLLNAME Name 'PCAudioSeek';

end.
 