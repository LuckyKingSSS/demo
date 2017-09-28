{$Z4}
unit MediaDecode;

interface
uses
  Windows, Classes, MediaCommon;

const
  CLSID_CDecMgr: TGUID = '{e71ead89-1ccd-4c34-97a8-7d45b5f16613}';
  CLSID_CSlideDecode: TGUID = '{21c7bb8b-657a-46fb-86e5-a439960ad004}';
  IID_ISlideDecoder: TGUID = '{3DDBBF17-D26F-4fd5-90B4-347C519C3A0D}';
  IID_IDecMgr: TGUID = '{5AD2BE35-3038-4c6a-8C7F-3698526A898F}';
  IID_IMediaDecoder: TGUID = '{C8BD5D61-172A-4ca2-98B7-DE5F1E995D72}';
  IID_ISubPicSetting: TGUID = '{89D89E0A-02F1-47a1-8756-323A9098937E}';
  IID_IMediaInfo: TGUID = '{10CA4A98-A525-42a5-9282-DD64406261B2}';
  IID_IMetaDataInfo: TGUID = '{80DEF872-887D-4426-B31B-771011CF3427}';
  IID_IProgramInfos: TGUID = '{E04C85FD-D887-4716-8D7A-D18A17CE60EC}';
  IID_IProgramInfo: TGUID = '{F5FA588B-55AF-4fa9-8913-323CFF39C231}';
  IID_ISubPicStreamInfos: TGUID = '{4291F08A-95C4-45c3-AF5B-1CC55A3F2CF7}';
  IID_ISubPicStreamInfo: TGUID = '{FB73C029-B0BD-4cbb-A0CE-971E64D4BDD4}';
  
//  MEDIA_CAPABILITIES
const
  MCAPS_CANT_SEEK = $0001;        // 不支持Seek操作
  MCAPS_UNKNOWN_LENGTH = $0002;   // 媒体播放时长未知

const
  SD_OPTFLAG_CLIP_START = 1;
  SD_OPTFLAG_CLIP_END = 0;
// DECODE_EX_PARAM_MESSAGE
// SetExDecodeParam函数使用的uMsg
const
// DECODE_EX_PARAM_MESSAGE
// SetExDecodeParam函数使用的uMsg
  DEM_DEINTERLACE = 1000;          // 隔行扫描插值算法： wParam - DEINTERLACE_STYLE
  CHANGE_AUIDO_STREAM = 1010;      // 动态改变音频流：　wParam　新音频流的Stream ID
  CHANGE_SUBTITLE_STREAM = 1011;   // 动态改变字幕流： wParam 新字幕流的Stream ID
  CHANGE_VIDEO_STREAM = 10012;     // 动态改变视频字幕流： wParam 新字幕流的Stream ID

  KEEP_ORIGINAL_VOLUME = 100;  

type
  HVIDEODECODER  = IMediaStream;  // Video解码器句柄定义
  HAUDIODECODER  = IMediaStream;  // Audio解码器句柄定义

  FILE_TYPE =
  (
    MEDIA_FILE,
    SUB_PIC
  );

  SUPPIC_TYPE =
  (
	  SPT_Internal,		//内嵌字幕
	  SPT_External		//外置字幕
  );
  
  DEINTERLACE_STYLE =
  (
    DIS_NONE,
    DIS_ODD,                // 重复奇数行
    DIS_EVEN,               // 重复偶数行
    DIS_ODD_INTERPOLATE,    // 使用奇数行插值
    DIS_EVEN_INTERPOLATE,   // 使用偶数行插值
    DIS_AUTO                // 自动处理（暂不支持）
  );

  DEC_MODEL =                 // 定义解码方式
  (
    AUTO_MODEL,       // 自动处理
    DSHOW_MODEL,      // DShow解码器
    FFDEC_MODEL,      // FFMPEG解码
    DVD_MODEL,        // DVD解码器
    AVDEC_MODEL,      // AVDec解码器 WS_AVDec.dll 基于WS Video Decoder Platform
    SWF_MODEL,        // Flash解码器
    MTXML_MODEL,      // mtx文件解码器
    IMAGE_MODEL       // 图片解码器
  );


  PDecParam = ^TDecParam;
  TDecParam = record
    uMediaType          : UINT;             // ' FWS'  ' BOV'  ' GPM'
    uProgramID          : UINT;             // uPID of ProgramInfo
    nTrack              : Longint;          // 视频轨道编号 StreamID
    nSubTrack           : Longint;          // 字幕编号 StreamID（适用于Video解码器）
    bDirectCopy         : BOOL;             // 直接读取流数据
    uDeintelace         : DEINTERLACE_STYLE;// 隔行扫描插值算法
    wParam              : WPARAM;
    lParam              : LPARAM;
    bUseThreadDecode    : BOOL; // 使用线程解码缓冲 播放器创建解码器时需要将该参数置FALSE才能做到实时Deintelace
                                // 多个CPU系统上打开线程可以提升CPU使用率
  end;

  PVideoDecParam = PDecParam;
  TVideoDecParam = TDecParam;
  TAudioDecParam = TDecParam;
  PAudioDecParam = PDecParam;

  PWatermarkParam = ^TWatermarkParam;
  TWatermarkParam = record
    hModule       : HMODULE;
    szID          : array [0..511] of WideChar;  // 水印资源ID
    Rect          : TRect;                       // 相对于剪裁后的图像分辨率中的位置
    nOpacity      : Longint;                     // 水印不透明度 (0 - 100)  0 - 完全透明  100 - 完全不透明
    lpbi          : PBITMAPINFO;                 // lpbi 非0则不使用hModule，支持32位透明图像，以及Y41A自定义格式
    lpBits        : PBYTE;                       // lpbi和lpBits需要应用程序负责其生命期
    nAlign        : Longint;                     // 水印对齐(nAlign > 0时 设置水印对齐(nAlign), 否则设置水印位置(Rect))
    // nPos 1 LeftTop, 2 MiddleTop, 3 RightTop, 4 Left, 5 Middle, 6 Right,  7 LeftBottom, 8 MiddleBottom, 9 RightBottom
  end;

  TSDVideoCallBack = function (pUserObj: Pointer; pClipObj: Pointer; nOptFlag: UINT): HVIDEODECODER; stdcall;
  TSDAudioCallBack = function (pUserObj: Pointer; pClipObj: Pointer; nOptFlag: UINT): HAUDIODECODER; stdcall;

  PVideoSlideParam = ^TVideoSlideParam;
  TVideoSlideParam = record
    nWidth          : Longint;                // 分辨率宽度
    nHeight         : Longint;
    dFrameRate      : Double;                 // 帧率 -1表示自动选择
    dwCompression   : DWORD;                  // 图像格式，目前只支持'21VY'
    nBitsDepth      : Longint;                // 图像位深度 YV12为12位
    Watermark       : TWatermarkParam;        // 水印参数
  end;

  PVideoProcessParam = ^TVideoProcessParam;
  TVideoProcessParam = record
    Time          : CLIP_TIME;     // 时间设置
    CropRect      : TRect;         // 剪裁区域，为浮点数相对坐标
    nBrightness   : Longint;       // 亮度，-100～100，0保持原亮度
    nContrast     : Longint;       // 对比度，-100～100，0保持原对比度
    nSaturation   : Longint;       // 饱和度,-100～100，0保持原饱和度
    uExFilterID   : UINT;          // 扩展滤镜ID
    uFlipFlag     : UINT;          // 图像翻转设置
    DecMod        : DEC_MODEL;     // 解码方式
    vidDecParam   : TDecParam;     // 解码参数
    uDeintelace   : DEINTERLACE_STYLE;    // 去除隔行扫描方式
    nResizeStyle  : IMAGE_RESIZE_METHOD;  // 图像缩放方式
    nInterpolation: IMAGE_INTERPOLATION;  // 图像插值算法
  end;

  TAudioSlideParam = record
    nSampleRate       : Longint;
    nChannels         : Longint;
    nBitsPerSample    : Longint;              // 采样深度，目前仅支持16位
    nInterpolation    : AUDIO_INTERPOLATION;  // 采样率转换插值算法
  end;
  PAudioSlideParam = ^TAudioSlideParam;

  PVolumEffect = ^TVolumEffect;
  TVolumEffect = record           // 音量特效
    bUseEffect    : BOOL;         // 是否使用特效
    dStartTime    : Double;       // 特效起止时间，单位：秒，以输出时间尺度为为准，有如下关系：
    dEndTime      : Double;       //  0.0 <= dStartTime < dEndTime <= dTimeEnd - dTimeStart
    nStartVoulm   : Longint;      // 开始音量
    nEndVoulm     : Longint;      // 结束音量
    nEffectID     : Longint;      // 过渡算法ID
  end;

  PAudioProcessParam = ^TAudioProcessParam;
  TAudioProcessParam = record     // 音频处理参数，注意：nVolum与Start/EndEffect效果有叠加，即若nVolum = 90,
                                  // StartEffect.nStartVoulm = 80,则最终开始音量为  原音量×90%×80% ＝ 原音量×72%
    Time          : CLIP_TIME;    // 时间设置
    nVolum        : Longint;      // 音量,0～200, 100保持原音量
    StartEffect   : TVolumEffect; // 音频开始部分过渡效果（暂不使用）
    EndEffect     : TVolumEffect; // 音频结尾部分过渡效果（暂不使用）
    DecMod        : DEC_MODEL;    // 解码方式
    audDecParam   : TDecParam;    // 音频解码参数
  end;
  
  IMediaDecoder = interface(IUnknown)
    ['{C8BD5D61-172A-4ca2-98B7-DE5F1E995D72}']
  end;
  
  HMEDIADECODER  = IMediaDecoder;  // 媒体文件解码器句柄定义

  ISubPicStreamInfo = interface(IUnknown)
    ['{FB73C029-B0BD-4cbb-A0CE-971E64D4BDD4}']
    function GetType(): SUPPIC_TYPE; stdcall;
    function GetID(): UINT; stdcall;
    function GetFourCC(): UINT; stdcall;
    function GetLanguage(var ppLanguage: WideString): HRESULT; stdcall;
    function GetDescription(var ppDescription: WideString): HRESULT; stdcall;
  end;
  
  ISubPicStreamInfos = interface(IUnknown)
    ['{4291F08A-95C4-45c3-AF5B-1CC55A3F2CF7}']
    function GetCount(): Integer; stdcall;
    function GetItem(nIndex: Integer; var pSubPicStreamInfo: ISubPicStreamInfo): HRESULT; stdcall;
  end;
  

  IStreamInfos = interface(IUnknown)
    ['{5DD650C8-40A3-4d0d-B460-0BA5804A1AAD}']
    function GetCount(): Integer; stdcall;
    function GetItem(nIndex: Integer; var ppItem: IStreamInfo): HRESULT; stdcall;
  end;
  
  IProgramInfo = interface(IUnknown)
    ['{F5FA588B-55AF-4fa9-8913-323CFF39C231}']
    function GetNumber(): UINT; stdcall;
    function GetID(): UINT; stdcall;
    function GetMediaLength(): Double; stdcall;
    function GetBitrate(): Integer; stdcall;
    function GetTimeStampReferenceStreamID(): UINT; stdcall;
    function GetVideoStreamInfos(var ppVideoStreamInfos: IStreamInfos): HRESULT; stdcall;
    function GetAudioStreamInfos(var ppAudioStreamInfos: IStreamInfos): HRESULT; stdcall;
    function GetSubPicStreamInfos(var pSubPicStreamInfos: ISubPicStreamInfos): HRESULT; stdcall;
  end;
  
  IProgramInfos = interface(IUnknown)
    ['{E04C85FD-D887-4716-8D7A-D18A17CE60EC}']
    function GetCount(): Integer; stdcall;
    function GetItem(nIndex: Integer; var pProgramInfo: IProgramInfo): HRESULT; stdcall;
  end;

  IMetaDataInfo = interface(IUnknown)
    ['{80DEF872-887D-4426-B31B-771011CF3427}']
    function GetArtist(var pArtist: WideString): HRESULT; stdcall;
    function GetTitle(var pTitle: WideString): HRESULT; stdcall;
    function GetTrackNumber(var pTrackNumber: WideString): HRESULT; stdcall;
    function GetAlbum(var pAlbum: WideString): HRESULT; stdcall;
    function GetDate(var pDate: WideString): HRESULT; stdcall;
    function GetGenre(var pGenre: WideString): HRESULT; stdcall;
    function GetPublisher(var pPublisher: WideString): HRESULT; stdcall;
  end;
  
  IMediaInfo = interface(IUnknown)
    ['{10CA4A98-A525-42a5-9282-DD64406261B2}']
    function GetFourCC(): UINT; stdcall;
    function GetFileName(pFileName: PWideString): HRESULT; stdcall;
    function GetDescription(var ppDescription: WideString): HRESULT; stdcall;
    function GetMediaLength(): Double; stdcall;
    function GetBitrate(): Integer; stdcall;
    function GetIPrograms(var pProgramInfos: IProgramInfos): HRESULT; stdcall;
  end;

  IDecMgr = interface(IUnknown)
    ['{5AD2BE35-3038-4c6a-8C7F-3698526A898F}']
    function CreateMediaDecoder(const pMediaPath: WideString; fileType: FILE_TYPE;
      const pVidParam : TVideoDecParam; const pAudParam : TAudioDecParam;
      DecMod: DEC_MODEL): Pointer; stdcall;
    function GetMediaInfo(const pMediaPath: WideString; fileType: FILE_TYPE;
      DecMod: DEC_MODEL; var pIMediaInfo: IMediaInfo): HRESULT; stdcall;
  end;

  ISlideDecoder = interface(IUnknown)
    ['{3DDBBF17-D26F-4fd5-90B4-347C519C3A0D}']
    function SetVideoParam(const pParam: TVideoSlideParam; pUserObj: Pointer; fnCBVideo: TSDVideoCallBack): HRESULT; stdcall;
    function GetVideoParam(): PVideoSlideParam;
    function AddVideoClip(pMediaStream: IMediaStream; const pVideoInfo: TWSVideoInfo; const pClipTime: CLIP_TIME; pUserClipObj: Pointer): HRESULT; stdcall;
    function SetAudioParam(const pParam: TAudioSlideParam; pUserObj: Pointer; fnCBAudio: TSDAudioCallBack): HRESULT; stdcall;
    function GetAudioParam(): PAudioSlideParam; stdcall;
    function AddAudioClip(pMediaStream: IMediaStream; const pAudioInfo: TWSAudioInfo; const pClipTime: CLIP_TIME; pUserClipObj: Pointer): HRESULT; stdcall;
  end;

  IVideoDecoder = interface
    function GetVideoDecoder: HVIDEODECODER;
    function GetVideoInfo: PWSVideoInfo;

    property VideoDecoder: HVIDEODECODER read GetVideoDecoder;
    property VideoInfo: PWSVideoInfo read GetVideoInfo;
  end;

  IAudioDecoder = interface
    function GetAudioDecoder: HAUDIODECODER;
    function GetAudioInfo: PWSAudioInfo;

    property AudioDecoder: HAUDIODECODER read GetAudioDecoder;
    property AudioInfo: PWSAudioInfo read GetAudioInfo;
  end;

  PMediaDecodeAPI = ^TMediaDecodeAPI;
  TMediaDecodeAPI = record
  private
    
  public
    function DecMgrInit(): BOOL;
    procedure DecMgrUninit();
    function WSGetMediaFileInfo(const pFileName : UnicodeString; DecMod : DEC_MODEL = AUTO_MODEL) : IMediaInfo;
    procedure WSReleaseMediaFileInfo(pMediaInfo: IMediaInfo);
  end;
  TMediaDecodeReason = (mdrForPlay, mdrForPreview, mdrForEncode);

  TMediaDecoder = class(TInterfacedObject, IVideoDecoder, IAudioDecoder)
  private
    FMediaDecoder: IMediaDecoder;
    FError: Integer;
    FVideoInfo: TWSVideoInfo;
    FAudioInfo: TWSAudioInfo;
    FDecodeReason: TMediaDecodeReason;
    FOnRelease: TNotifyEvent;
  protected
    function GetMediaDecoder(): HMEDIADECODER;
  public
    constructor Create(const FileName: WideString; const pVidParam: TVideoDecParam; const pAudParam: TAudioDecParam; ADecModel: DEC_MODEL = AUTO_MODEL);
    destructor Destroy; override;

    function GetVideoDecoder: HVIDEODECODER;
    function GetVideoInfo: PWSVideoInfo;

    function GetAudioDecoder: HAUDIODECODER;
    function GetAudioInfo() : PWSAudioInfo;

    procedure ChangeAudioTrack(ATrackId: Integer);
    procedure ChangeSubtitle(ATrackId: Integer);

    property Handle: HMEDIADECODER read GetMediaDecoder;
    property VideoDecoder: HVIDEODECODER read GetVideoDecoder;
    property VideoInfo: PWSVideoInfo read GetVideoInfo;
    property AudioDecoder: HAUDIODECODER read GetAudioDecoder;
    property AudioInfo: PWSAudioInfo read GetAudioInfo;
    property Error: Integer read FError;
    property DecodeReason: TMediaDecodeReason read FDecodeReason write FDecodeReason;
    property OnRelease: TNotifyEvent read FOnRelease write FOnRelease; 

  end;
  
  TSlideDecoderEx = class;

  TVideoSlideClipEvent = function (Sender: TSlideDecoderEx; nOptFlag: UINT): HVIDEODECODER of object;
  TVideoSlideClipProc = function (Sender: TSlideDecoderEx; nOptFlag: UINT): HVIDEODECODER;

  TAudioSlideClipEvent = function (Sender: TSlideDecoderEx; nOptFlag: UINT): HAUDIODECODER of object;
  TAudioSlideClipProc = function (Sender: TSlideDecoderEx; nOptFlag: UINT): HAUDIODECODER;

  TSlideDecoderEx = class(TInterfacedObject, IVideoDecoder, IAudioDecoder)
  private
    FSlideDecoder: ISlideDecoder;
    FVideoSlideParam: TVideoSlideParam;
    FAudioSlideParam: TAudioSlideParam;
    FDecoderList: TInterfaceList;
    FFileList: TStringList;
    FVideoClipEvent: TVideoSlideClipProc;
    FAudioClipEvent: TAudioSlideClipProc;
    FDecodeReason: TMediaDecodeReason;
    function DoVideoCallback(pClipObj: Pointer; nOptFlag: UINT): HVIDEODECODER;
    function DoAudioCallback(pClipObj: Pointer; nOptFlag: UINT): HAUDIODECODER;
  public
    constructor Create(const pVidParam : TVideoSlideParam; const pAudParam: TAudioSlideParam); overload;
    constructor Create(const pVidParam : TVideoSlideParam; AVideoClipEvent: TVideoSlideClipProc;
      const pAudParam: TAudioSlideParam; AAudioClipEvent: TAudioSlideClipProc); overload;
    constructor Create(const pVidParam : TVideoSlideParam; AVideoClipEvent: TVideoSlideClipEvent;
      const pAudParam: TAudioSlideParam; AAudioClipEvent: TAudioSlideClipEvent); overload;
    destructor Destroy; override;

    function AddVideoSource(const AVideo: IVideoDecoder; const pClipTime: CLIP_TIME): BOOL; overload;
    function AddVideoDecoder(hVideoProcess: HVIDEODECODER; const pClipTime: CLIP_TIME): BOOL; overload;
    // 添加视频剪辑，使用回调方式（节约内存，推荐）
    function AddVideoClip(const pVideoInfo: TWSVideoInfo; const pClipTime: CLIP_TIME; pClipObj: Pointer): Pointer;
    // 这个接口主要是为了兼容性而设
    function AddVideoSource(const FileName: UnicodeString; const pParam : TVideoProcessParam): BOOL; overload;

    function AddAudioSource(const AAudio: IAudioDecoder; const pClipTime: CLIP_TIME): BOOL; overload;
    function AddAudioDecoder(hAudioProcess: HAUDIODECODER; const pClipTime: CLIP_TIME): BOOL; overload;
    // 添加音频剪辑，使用回调方式（节约内存，推荐）
    function AddAudioClip(const pAudioInfo: TWSAudioInfo; const pClipTime: CLIP_TIME; pClipObj: Pointer): Pointer;
    // 这个接口主要是为了兼容性而设
    function AddAudioSource(const FileName: UnicodeString; const pParam : TAudioProcessParam): BOOL; overload;

    function GetVideoDecoder: HVIDEODECODER;
    function GetVideoInfo: PWSVideoInfo;

    function GetAudioDecoder: HAUDIODECODER;
    function GetAudioInfo: PWSAudioInfo;

    property VideoSlideParam: TVideoSlideParam read FVideoSlideParam;
    property AudioSlideParam: TAudioSlideParam read FAudioSlideParam;
    property VideoClipEvent: TVideoSlideClipProc read FVideoClipEvent;
    property AudioClipEvent: TAudioSlideClipProc read FAudioClipEvent;
    property DecodeReason: TMediaDecodeReason read FDecodeReason write FDecodeReason;
  end;
var
  gDecMgr: IDecMgr;
  VADecoderAPI: TMediaDecodeAPI;

implementation
{ TMediaDecoder }

procedure TMediaDecoder.ChangeAudioTrack(ATrackId: Integer);
var
  pAudStream: IMediaStream;
begin
  pAudStream := GetAudioDecoder;
  if Assigned(pAudStream) then
  begin
    pAudStream.ChangeStream(ATrackId);
    pAudStream._Release;
  end;
end;

procedure TMediaDecoder.ChangeSubtitle(ATrackId: Integer);
var
  pVidStream: IMediaStream;
begin
  pVidStream := GetVideoDecoder;
  if Assigned(pVidStream) then
  begin
    pVidStream.ChangeStream(ATrackId);
    pVidStream._Release;
  end;
end;

constructor TMediaDecoder.Create(const FileName: WideString;
  const pVidParam: TVideoDecParam; const pAudParam: TAudioDecParam; ADecModel: DEC_MODEL);
begin
  if not Assigned(gDecMgr) then
  begin
    WSCoCreateInstance(CLSID_CDecMgr, nil, 0, IID_IDecMgr, gDecMgr);
  end;
  if Assigned(gDecMgr) then
  begin
    FMediaDecoder := IMediaDecoder(gDecMgr.CreateMediaDecoder(FileName, MEDIA_FILE, pVidParam, pAudParam, ADecModel));
  end;
end;

destructor TMediaDecoder.Destroy;
begin
  if Assigned(FMediaDecoder) then
    FMediaDecoder._Release;
  inherited;
end;

function TMediaDecoder.GetAudioDecoder: HAUDIODECODER;
var
  pAudOutput: IAudioOutput;
begin
  FMediaDecoder.QueryInterface(IID_IAudioOutput, pAudOutput);
  if Assigned(pAudOutput) then
  begin
    pAudOutput.GetAudioOutput(Result);
    pAudOutput._Release;
  end;
end;

function TMediaDecoder.GetAudioInfo: PWSAudioInfo;
var
  pAudStream: IMediaStream;
  pStreamInfo: IStreamInfo;
begin
  pAudStream := GetAudioDecoder;
  if Assigned(pAudStream) then
  begin
    pAudStream.GetStreamInfo(pStreamInfo);
    Result := pStreamInfo.GetAudioInfo;
    pAudStream._Release;
  end;
end;

function TMediaDecoder.GetMediaDecoder: HMEDIADECODER;
begin
  Result := FMediaDecoder;
  Result._AddRef;
end;

function TMediaDecoder.GetVideoDecoder: HVIDEODECODER;
var
  pVidOutput: IVideoOutput;
begin
  FMediaDecoder.QueryInterface(IID_IVideoOutput, pVidOutput);
  if Assigned(pVidOutput) then
  begin
    pVidOutput.GetVideoOutput(Result);
    pVidOutput._Release;
  end;
end;

function TMediaDecoder.GetVideoInfo: PWSVideoInfo;
var
  pVidStream: IMediaStream;
  pStreamInfo: IStreamInfo;
begin
  pVidStream := GetVideoDecoder;
  if Assigned(pVidStream) then
  begin
    pVidStream.GetStreamInfo(pStreamInfo);
    Result := pStreamInfo.GetVideoInfo;
    pVidStream._Release;
  end;
  
end;
function DefaultSDVideoCallBackProc(pUserObj: Pointer; pClipObj: Pointer; nOptFlag: UINT): HVIDEODECODER; stdcall;
begin
  Result:=TSlideDecoderEx(pUserObj).DoVideoCallback(pClipObj, nOptFlag);
end;

function DefaultSDAudioCallBackProc(pUserObj: Pointer; pClipObj: Pointer; nOptFlag: UINT): HAUDIODECODER; stdcall;
begin
  Result:=TSlideDecoderEx(pUserObj).DoAudioCallback(pClipObj, nOptFlag);
end;

{ TVideoSlideDecoderEx }

function TSlideDecoderEx.AddAudioClip(const pAudioInfo: TWSAudioInfo;
  const pClipTime: CLIP_TIME; pClipObj: Pointer): Pointer;
var
  pAudOut: IAudioOutput;
  pAudStream: IMediaStream;
begin
  pAudOut := nil;
  pAudStream := nil;
  if Assigned(FSlideDecoder) then
  begin
    FSlideDecoder.QueryInterface(IID_IAudioOutput, pAudOut);
    if Assigned(pAudOut) then
    begin
      pAudOut.GetAudioOutput(pAudStream);
      if Assigned(pAudStream) then
        FSlideDecoder.AddAudioClip(pAudStream, pAudioInfo, pClipTime, pClipObj);
      pAudOut._Release;
    end;  
  end;
end;

function TSlideDecoderEx.AddAudioDecoder(hAudioProcess: HAUDIODECODER;
  const pClipTime: CLIP_TIME): BOOL;
begin

end;

function TSlideDecoderEx.AddAudioSource(const FileName: UnicodeString;
  const pParam: TAudioProcessParam): BOOL;
begin

end;

function TSlideDecoderEx.AddAudioSource(const AAudio: IAudioDecoder;
  const pClipTime: CLIP_TIME): BOOL;
begin

end;

function TSlideDecoderEx.AddVideoClip(const pVideoInfo: TWSVideoInfo;
  const pClipTime: CLIP_TIME; pClipObj: Pointer): Pointer;
var
  pVidOut: IVideoOutput;
  pVidStream: IMediaStream;
begin
  pVidOut := nil;
  pVidStream := nil;
  FSlideDecoder.QueryInterface(IID_IVideoOutput, pVidOut);
  if Assigned(pVidOut) then
  begin
    pVidOut.GetVideoOutput(pVidStream);
    if Assigned(pVidStream) then
    begin
      FSlideDecoder.AddVideoClip(pVidStream, pVideoInfo, pClipTime, pClipObj);
      pVidStream._Release;
    end;
    pVidOut._Release;
  end;
end;

function TSlideDecoderEx.AddVideoDecoder(hVideoProcess: HVIDEODECODER;
  const pClipTime: CLIP_TIME): BOOL;
begin

end;

function TSlideDecoderEx.AddVideoSource(const FileName: UnicodeString;
  const pParam: TVideoProcessParam): BOOL;
begin

end;

constructor TSlideDecoderEx.Create(const pVidParam: TVideoSlideParam;
  const pAudParam: TAudioSlideParam);
begin
  Create(pVidParam, TVideoSlideClipProc(nil), pAudParam, TAudioSlideClipProc(nil));
end;

constructor TSlideDecoderEx.Create(const pVidParam: TVideoSlideParam;
  AVideoClipEvent: TVideoSlideClipProc; const pAudParam: TAudioSlideParam;
  AAudioClipEvent: TAudioSlideClipProc);
begin
  inherited Create;
  FVideoSlideParam := pVidParam;
  FAudioSlideParam := pAudParam;
  FSlideDecoder := nil;
  WSCoCreateInstance(CLSID_CSlideDecode, nil, 0, IID_ISlideDecoder, FSlideDecoder);
  FDecoderList:=TInterfaceList.Create;
  FVideoClipEvent := AVideoClipEvent;
  FAudioClipEvent := AAudioClipEvent;
end;

constructor TSlideDecoderEx.Create(const pVidParam: TVideoSlideParam;
  AVideoClipEvent: TVideoSlideClipEvent; const pAudParam: TAudioSlideParam;
  AAudioClipEvent: TAudioSlideClipEvent);
begin
  inherited Create;
  FVideoSlideParam := pVidParam;
  FAudioSlideParam := pAudParam;
  FSlideDecoder := nil;
  WSCoCreateInstance(CLSID_CSlideDecode, nil, 0, IID_ISlideDecoder, FSlideDecoder);
  FDecoderList := TInterfaceList.Create;
  @FVideoClipEvent := @AVideoClipEvent;
  @FAudioClipEvent := @AAudioClipEvent;
end;

function TSlideDecoderEx.AddVideoSource(const AVideo: IVideoDecoder;
  const pClipTime: CLIP_TIME): BOOL;
begin

end;

destructor TSlideDecoderEx.Destroy;
begin
  if Assigned(FSlideDecoder) then
    FSlideDecoder._Release;
  inherited;
end;

function TSlideDecoderEx.DoAudioCallback(pClipObj: Pointer;
  nOptFlag: UINT): HAUDIODECODER;
var
  pEvent: TMethod;
begin
  if Assigned(FAudioClipEvent) then
  begin
    pEvent.Code := @FAudioClipEvent;
    pEvent.Data := pClipObj;
    Result := TAudioSlideClipEvent(pEvent)(Self, nOptFlag);
  end else
    Result := nil;
end;

function TSlideDecoderEx.DoVideoCallback(pClipObj: Pointer;
  nOptFlag: UINT): HVIDEODECODER;
var
  pEvent: TMethod;
begin
  if Assigned(FVideoClipEvent) then
  begin
    pEvent.Code := @FVideoClipEvent;
    pEvent.Data := pClipObj;
    Result := TVideoSlideClipEvent(pEvent)(Self, nOptFlag);
  end else
    Result := nil;
end;

function TSlideDecoderEx.GetAudioDecoder: HAUDIODECODER;
var
  pAudOut: IAudioOutput;
  pAudStream: IMediaStream;
begin
  pAudOut := nil;
  pAudStream := nil;
  Result := nil;
  if Assigned(FSlideDecoder) then
  begin
    FSlideDecoder.QueryInterface(IID_IAudioOutput, pAudOut);
    if Assigned(pAudOut) then
    begin
      pAudOut.GetAudioOutput(Result);
      pAudOut._Release;
    end;
  end;
end;

function TSlideDecoderEx.GetAudioInfo: PWSAudioInfo;
var
  pAudStream: IMediaStream;
  pStreamInfo: IStreamInfo;
begin
  pAudStream := GetAudioDecoder;
  pStreamInfo := nil;
  if Assigned(pAudStream) then
  begin
    pAudStream.GetStreamInfo(pStreamInfo);
    if Assigned(pStreamInfo) then
    begin
      Result := pStreamInfo.GetAudioInfo;
      pStreamInfo._Release;
    end;
    pAudStream._Release;
  end;
end;

function TSlideDecoderEx.GetVideoDecoder: HVIDEODECODER;
var
  pVidOut: IVideoOutput;
  pVidStream: IMediaStream;
begin
  pVidOut := nil;
  pVidStream := nil;
  Result := nil;
  if Assigned(FSlideDecoder) then
  begin
    FSlideDecoder.QueryInterface(IID_IVideoOutput, pVidOut);
    if Assigned(pVidOut) then
    begin
      pVidOut.GetVideoOutput(Result);
      pVidOut._Release;
    end;
  end;
end;

function TSlideDecoderEx.GetVideoInfo: PWSVideoInfo;
var
  pVidStream: IMediaStream;
  pStreamInfo: IStreamInfo;
begin
  pVidStream := GetVideoDecoder;
  pStreamInfo := nil;
  if Assigned(pVidStream) then
  begin
    pVidStream.GetStreamInfo(pStreamInfo);
    if Assigned(pStreamInfo) then
    begin
      Result := pStreamInfo.GetVideoInfo;
      pStreamInfo._Release;
    end; 
  end;
end;

{ TMediaDecodeAPI }

function TMediaDecodeAPI.DecMgrInit: BOOL;
begin
  Result := True;
  if not Assigned(gDecMgr) then
    WSCoCreateInstance(CLSID_CDecMgr, nil, 0, IID_IDecMgr, gDecMgr);
  if not Assigned(gDecMgr) then
    Result := False;
end;

procedure TMediaDecodeAPI.DecMgrUninit;
begin
  if Assigned(gDecMgr) then
    gDecMgr._Release;
  gDecMgr := nil;
end;

function TMediaDecodeAPI.WSGetMediaFileInfo(const pFileName: UnicodeString;
  DecMod: DEC_MODEL): IMediaInfo;
begin
  if Assigned(gDecMgr) then
    gDecMgr.GetMediaInfo(pFileName, MEDIA_FILE, DecMod, Result);
end;

procedure TMediaDecodeAPI.WSReleaseMediaFileInfo(pMediaInfo: IMediaInfo);
begin
  if Assigned(pMediaInfo) then
    pMediaInfo._Release;
end;

end.
