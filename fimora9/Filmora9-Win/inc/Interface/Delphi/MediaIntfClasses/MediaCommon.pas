{$Z4}
unit MediaCommon;

interface
uses
  Windows, SysUtils;
const
  IID_IStreamInfos: TGUID = '{5DD650C8-40A3-4d0d-B460-0BA5804A1AAD}';
  IID_IMediaStream: TGUID = '{E6C172B0-6D74-438f-81E0-433D92B2A627}';
  IID_IVideoInput:  TGUID = '{8F310739-7304-4fb9-B4C4-FF7B3B48D98E}';
  IID_IVideoOutput: TGUID = '{30D4D994-A15B-4c2e-8277-66D0A8AA1B4F}';
  IID_IAudioInput:  TGUID = '{89314327-F161-4483-A0EE-AFA455E825EA}';
  IID_IAudioOutput: TGUID = '{29B857F5-9C9F-4586-85EA-BDE2F1F178F1}';

type
{$IFNDEF UNICODE}
  UnicodeString = WideString;
{$ENDIF}

  AV_STREAM_TYPE =
  (
	  ST_NONE,
	  ST_NAV,			// DVD导航数据
	  ST_VIDEO,		// 视频Stream
	  ST_AUDIO,		// 音频Stream
	  ST_SUBPIC,
	  ST_IMAGE
  );

  YUV_IMAGE_FORMAT =
  (
	  YIF_444,
	  YIF_422,
	  YIF_YV12 // 420
  );

  IMAGE_FORMAT =
  (
    IF_RGB,
    IF_YUV,
    IF_COMPRESSED
  );

  DEINTERLACE_STYLE =
  (
	  DIS_NONE,
  	DIS_ODD,				// 重复奇数行
	  DIS_EVEN,				// 重复偶数行
	  DIS_ODD_INTERPOLATE,	// 使用奇数行插值
	  DIS_EVEN_INTERPOLATE,	// 使用偶数行插值
	  DIS_AUTO				// 自动处理（暂不支持）
  );
  
  IMAGE_INTERPOLATION =
  (
    IIN_AUTO,
    IIN_FAST,
    IIN_BI_LINER,
    IIN_TRI_CONVOLUTE
  );

  IMAGE_TYPE =
  (
  	IMAGE_BMP,
	  IMAGE_JPG,
	  IMAGE_PNG
  );
  
  IMAGE_RESIZE_METHOD =
  (
	  IRM_FULLSCREEN,		// stretch to full screen
  	IRM_ORIGINAL_SCALE, // keep scale, fit to size of output
	  IRM_16_9,			// resize to 16:9, fit to size of output
	  IRM_4_3,			// resize to 4:3, fit to size of output
	  IRM_LETTERBOX,
	  IRM_PAN_SCAN
  );
  
  AUDIO_INTERPOLATION =       // 音频采样率转换插值算法
  (
    SIMPLE_LINEAR = 0,        // 一次线性插值
    QUADRATIC_LINEAR          // 二次线性插值(暂未实现)
  );



  PFRECT = ^FRECT;
  FRECT = record
    left: Double;
    top: Double;
    right: Double;
    bottom: Double;
  end;
  
  PWSAudioInfo = ^TWSAudioInfo;
  TWSAudioInfo = record
    sample_rate: Integer;
    channels: Integer;
    bitspersample: Integer;
    bytespersecond: Integer;
    length: Double;
    dwCapsFlag: DWORD;
  end;

  PWSVideoInfo = ^TWSVideoInfo;
  TWSVideoInfo = record
    width: Integer;
    height: Integer;
    frame_rate: Double;
    total_frames: Integer;
    xratio: Integer;
    yratio: Integer;
    dwCompression: DWORD;
    nBitsDepth: Integer;
    dwCapsFlag: DWORD;
  end;

  PRESIZE_PARAM = ^TRESIZE_PARAM;
  TRESIZE_PARAM = record
    width: Integer;
    height: Integer;
    uInterpolation: IMAGE_INTERPOLATION;
    uStyle: IMAGE_RESIZE_METHOD;
  end;
  RESIZE_PARAM = TRESIZE_PARAM;

  PCaptureImageInfo = ^TCaptureImageInfo;
  TCaptureImageInfo = record
    uResizeParam: TRESIZE_PARAM;
    uDeInterlace: DEINTERLACE_STYLE;
    uImageType: IMAGE_TYPE;
  end;
  
  CLIP_TIME = record
    dStart     : Double;      // 相对输出（音）视频的起止时间，单位：秒，若 剪切长度 != 输出长度，则会产生拉伸效果
    dEnd       : Double;      // 此值为零则无拉伸效果
    dTrimFrom  : Double;      // 剪切时间段之起点，单位：秒，下同
    dTrimTo    : Double;      // 剪切时间段之终点，0值表示尾端不进行剪切
  end;

  // YUV图像数据结构定义
  PYUVImage = ^TYUVImage;
  TYUVImage = record
    format: YUV_IMAGE_FORMAT;
    y: PBYTE;   // Y数据指针
    u: PBYTE;   // U数据指针
    v: PBYTE;   // V数据指针
    a: PBYTE;   // Alpha 通道（不透明度）
    width: Longint;   // 图像宽度
    height: Longint;  // 图像高度
    y_pitch: Longint; // Y数据每行所占字节数
    u_pitch: Longint; // U数据每行所占字节数
    v_pitch: Longint; // V数据每行所占字节数
  end;

  TVideoImage = record
    case format: IMAGE_FORMAT of
      IF_YUV:
        (
          YUVImage: TYUVImage;
          flag: Longint;      //  1 - 关键帧
        );
      IF_RGB:
        (
          biHeader: BITMAPINFOHEADER;
          lpBits: PBYTE;
        );
      IF_COMPRESSED: // 压缩数据
        (
          pData: PBYTE;
          nBytes: Longint;
          nBufSize: Longint; // pData 缓冲区大小
        );
  end;

  TAudioFrame = record
    nSampleRate     : Longint;
    nChannels       : Longint;
    nBitsPerSample  : Longint;
    pData           : PBYTE;
    nBytes          : Longint;
    nBufSize        : Longint; // pData 缓冲区大小
  end;

  TSubPicFrame = record
    format: YUV_IMAGE_FORMAT;
    y: PBYTE;               // Y数据指针
    u: PBYTE;               // U数据指针
    v: PBYTE;               // V数据指针
    a: PBYTE;               // Alpha 通道（不透明度）
    dx: Longint;            // 对应于视频中起始坐标
    dy: Longint;
    width: Longint;         // 字幕图像宽度
    height: Longint;        // 字幕图像高度
    sx: Longint;            // 字幕中有效起始坐标
    sy: Longint;
    validwidth: Longint;    // 字幕中有效图像大小，对应于sx、sy
    validheight: Longint;   // validwidth <= width   validheight <= height
    y_pitch: Longint;       // Y数据每行所占字节数
    u_pitch: Longint;       // U数据每行所占字节数
    v_pitch: Longint;       // V数据每行所占字节数

    StartPts: Int64;        // 1/10000000 second
    duration: Int64;        // 显示持续时间 1/10000000 second
  end;

  PPMEDIA_FRAME = ^PMEDIA_FRAME;
  PMEDIA_FRAME = ^TMEDIA_FRAME;
  TMEDIA_FRAME = record
    case dwTypeL: AV_STREAM_TYPE of
    ST_VIDEO:
      (
        video: TVideoImage;
        pts: int64;
        dts: Int64;    // 1/10000000 second
        duration: Integer;
      );
    ST_AUDIO:
      (
        audio: TAudioFrame;
      );
    ST_SUBPIC:
      (
        subPic: TSubPicFrame;
      )
  end;

  
  HVIEW = HWND;

  TMULTI_CALLBACK = function (pUserObj: Pointer; uMsg: UINT; wParam: WPARAM; lParam: LPARAM): BOOL; stdcall;

  IStreamInfo = interface(IUnknown)
    ['{58FE100C-DCD4-49db-9930-326174CC0ECC}']
    function GetType(): AV_STREAM_TYPE; stdcall;
    function GetID(): UINT; stdcall;
    function GetFourCC(): UINT; stdcall;
    function GetState(): UINT; stdcall;
    function GetName(var pStreamName: UnicodeString): HRESULT; stdcall;
    function GetDescription(var ppDescription: UnicodeString): HRESULT; stdcall;
    function GetLength(): Double; stdcall;
    function GetBitRate(): Integer; stdcall;
    function GetAudioInfo(): PWSAudioInfo; stdcall;
    function GetVideoInfo(): PWSVideoInfo; stdcall;
    function GetStartPTS(): Int64; stdcall;
    function GetPTSOffset(): Int64; stdcall;
  end;

  IMediaStream = interface(IUnknown)
    ['{E6C172B0-6D74-438f-81E0-433D92B2A627}']
    function GetStreamInfo(var pStreamInfo: IStreamInfo): HRESULT; stdcall;
    function ChangeStream(nStreamID: Integer): HRESULT; stdcall;
    function Seek(dTime: Double; accurate: Integer): HRESULT; stdcall;
    function GetSample(): PMEDIA_FRAME; stdcall;
    function Forward(): HRESULT; stdcall;
    function GetOffset(): Double; stdcall;
  end;

  IVideoInput = interface(IUnknown)
    ['{8F310739-7304-4fb9-B4C4-FF7B3B48D98E}']
    function SetVideoInput(MediaStream: IMediaStream): HRESULT; stdcall;
  end;

  IVideoOutput = interface(IUnknown)
    ['{30D4D994-A15B-4c2e-8277-66D0A8AA1B4F}']
    function GetVideoOutput(var pMediaStream: IMediaStream): HRESULT; stdcall;
  end;

  IAudioInput = interface(IUnknown)
    ['{89314327-F161-4483-A0EE-AFA455E825EA}']
    function SetAudioInput(MediaStream: IMediaStream): HRESULT; stdcall;
  end;

  IAudioOutput = interface(IUnknown)
    ['{29B857F5-9C9F-4586-85EA-BDE2F1F178F1}']
    function GetAudioOutput(var pMediaStream: IMediaStream): HRESULT; stdcall;
  end;


  
  function WSCoCreateInstance(const clsid: TGUID; unkOuter: IUnknown;
    dwClsContext: Longint; const iid: TGUID; out pv): HResult; stdcall;
  function SetCOMProfile(filename: PChar): HRESULT; stdcall;
  
  function MakeFourCC(const Str: AnsiString): DWORD; 
  function StringFromFourCC(dwFourCC: PDWORD): AnsiString; overload;
  function StringFromFourCC(dwFourCC: DWORD): AnsiString; overload;
  function DIBWIDTHBYTES(cx: DWORD; bit: DWORD):DWORD;
  procedure RaiseLoadLibError(const szLibName: string);

implementation
uses SysConst;

function MakeFourCC(const Str: AnsiString): DWORD;
var
  nSize: Longint;
begin
  Result:=$20202020;
  nSize:=Length(Str);
  if nSize>SizeOf(Result) then nSize:=SizeOf(Result);
  Move(Str[1], Result, nSize);
end;

function StringFromFourCC(dwFourCC: DWORD): AnsiString;
begin
  SetString(Result, PAnsiChar(@dwFourCC), 4);
end;

function StringFromFourCC(dwFourCC: PDWORD): AnsiString;
begin
  if (dwFourCC<>nil) and (DWORD(dwFourCC)<$80000000) then
  begin
    Result:=StringFromFourCC(dwFourCC^);
  end else
    Result:='';
end;

function DIBWIDTHBYTES(cx: DWORD; bit: DWORD):DWORD;
begin
  Result := ((cx * bit + 31) and not 31) shr 3;
end;

procedure RaiseLoadLibError(const szLibName: string);
var
  LastError: Integer;
  Error: EOSError;
begin
  LastError:=GetLastError;
  if LastError<>0 then
  begin
    Error:=EOSError.CreateResFmt(@SOSError, [LastError,
      SysErrorMessage(LastError)]);
    Error.Message:=Error.Message+sLineBreak+szLibName;
    raise Error;
  end;
end;

  function WSCoCreateInstance; external 'COMSupport.dll' name 'WSCreateInstance';
  function SetCOMProfile; external 'COMSupport.dll' name 'SetCOMProfile';


end.
