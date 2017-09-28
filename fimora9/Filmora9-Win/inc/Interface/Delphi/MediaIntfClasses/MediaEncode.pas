{$Z4}
unit MediaEncode;

interface
uses
  Windows, Classes, SysUtils, MediaCommon, MediaDecode, MediaEncodeMgr, MediaProcess;
const
  CLSID_CMediaConverter: TGUID = '{7e55d7f9-454a-4d4c-bd2d-8fec187b657f}';
  CLSID_CLossLessCopy: TGUID = '{235fec1a-b7e6-4fab-b89f-0743344e484a}';
  IID_IMediaConverter: TGUID = '{9684898A-3297-4da3-9264-6AAA5ED56600}';
  IID_IMediaEncParam: TGUID = '{635AF5AC-3D40-4ca6-83B2-392E316345E0}';
  SPLIT_BASE_ON_TIME = 1;
  SPLIT_BASE_ON_SIZE = 2;
type

  FileInfoType =
  (
    ID3
  );

  PFileInfoParam = ^TFileInfoParam;
  TFileInfoParam = record
    formateType: FileInfoType;
    nBufSize: Integer;
    pBuffer: PBYTE;
  end;

  PStreamParam = ^TStreamParam;
  TStreamParam = record
    dwFourCC: UINT;
    nBufSize: Integer;
    pBuffer: PBYTE;
  end;
  TimeData = record
    hour : Longint;
    minute : Longint;
    second : Longint;
    millisec : Longint;
  end;

  PMediaProgressInfo = ^TMediaProgressInfo;
  TMediaProgressInfo = record
    dTotalLength    : double;       // 总帧数量 (单位秒）
    dCurrTime       : double;       // 当前帧　（单位秒）
    nPercent        : Longint;      // 百分比（0 -100）
    nPercentPercent : Longint;      // 万分比（0 - 10000）
    pImageFrame     : PMEDIA_FRAME; // 图像数据
    tmPrepaid       : TimeData;     // 当前任务已用时间 (系统时间)
    tmRemain        : TimeData;     // 当前任务剩余时间 (系统时间)
    dTransedTime    : Double;       //当前任务已转换片断时间 (秒）
    dTransRate      : Double;       //转换速率，单位：每秒转换的片断长度
  end;
  

  IMediaEncParam = interface(IUnknown)
    ['{635AF5AC-3D40-4ca6-83B2-392E316345E0}']
    function SetFourCC(dwFourCC: UINT): HRESULT; stdcall;
    function GetFourCC(): UINT; stdcall;
    function SetUseTwoPass(bUse: Integer): HRESULT; stdcall;
    function GetUseTwoPass(): Integer; stdcall;
    function SetFileParam(const FileInfoParam: PFileInfoParam): HRESULT; stdcall;
    function GetFileParam(): PFileInfoParam; stdcall;
    function SetEncParam(StreamType: AV_STREAM_TYPE; const pStreamParam: PStreamParam): HRESULT; stdcall;
    function GetEncParam(): PStreamParam; stdcall;
    function GetEstimateDiskSpace(): Int64; stdcall;
  end;
  
  IMediaConverter = interface(IUnknown)
    ['{9684898A-3297-4da3-9264-6AAA5ED56600}']
    function SetOutputFile(const pOutputPath: WideString): HRESULT; stdcall;
    function GetEncodeParam(var pMediaEncParam: IMediaEncParam): HRESULT; stdcall;
    function SetClipTime(dBeginTime: Double; dLength: Double): HRESULT; stdcall;
    function GetClipTime(var dBeginTime, dLength: Double): HRESULT; stdcall;
    function SetCallBack(fnCallback: TMULTI_CALLBACK; pUserObj: Pointer): HRESULT; stdcall;
    function Start(): HRESULT; stdcall;
    function Pause(): HRESULT; stdcall;
    function Resume(): HRESULT; stdcall;
    function Stop(): HRESULT; stdcall;
    function SetSplitParam(nStyle: Integer; dwLimit: UINT): HRESULT; stdcall;
    function GetSplitParam(var nStyle: Integer; var dwLimit: UINT): HRESULT; stdcall;
    function GetTargetCount(): Integer; stdcall;
    function GetTargetFileName(nIndex: Integer; var pFileName: WideString): HRESULT; stdcall;
  end;

  TMediaEncodeAPI = record
  public
    procedure Init;
    function WEGetEstimateDiskSpaceEx(const pParam : TENCODEPARAM; dLength : double) : INT64;
  end;

  TMediaEncoder = class;

  HMEDIAENCODER = IMediaConverter;
  TEncoderStatus = (esNone, esHaveFile, esEncoding, esPause, esFinish);
  TMediaEncodingState = (mesUnknown, mesEncoding, mesFinished, mesUserAbort,
    mesCallBackAbort, mesError, mesTwoPassStart, mesTwoPassEncoding,
    mesTwoPassFinished);

  TMediaEncodeProgressEvent = function (Sender: TMediaEncoder; EncodingState: TMediaEncodingState;
    AProgress, AProgressMax: Integer; AProgressInfo: PMediaProgressInfo): Boolean of object;

  TEncodeSplitStyle = (ssNone=0, ssByTime=SPLIT_BASE_ON_TIME, ssBySize=SPLIT_BASE_ON_SIZE);

  TMediaEncoder = class
  private
    FHandle: HMEDIAENCODER;
    FEncodeParam: TEncodeParam;
    FEncodeParamEx: TEncodeParamEx;
    FVideoDecoder: IVideoDecoder;
    FAudioDecoder: IAudioDecoder;
    FOutputFile: UnicodeString;
    FStatus: TEncoderStatus;
    FAborting: Boolean;
    FOnEncodeProgress: TMediaEncodeProgressEvent;
    FDirectCopy: Boolean;
    function DoProgress(EncodingState: TMediaEncodingState;
      AProgress, AProgressMax: Integer; AProgressInfo: PMediaProgressInfo): Boolean;
    procedure SetEncodeParam(pParam : PEncodeParam);
    function GetEncodeParam: PEncodeParam;
    procedure SetOutputFileProc(const Value: UnicodeString);
    function GetEncodeParamEx: PEncodeParamEx;
    procedure SetEncodeParamEx(const Value: PEncodeParamEx);
  public
    constructor Create(const AFileName: UnicodeString; AWidth: Integer=0; AHeight: Integer=0); overload;
    constructor Create(const AFileName: UnicodeString; const AVidDecParam: TVideoDecParam;
      const AAudDecParam: TAudioDecParam; ADecModel: DEC_MODEL=AUTO_MODEL; ADirectCopy: Boolean = False); overload;
    constructor Create(const AVideo: IVideoDecoder; const AAudio: IAudioDecoder); overload; virtual;
    destructor Destroy; override;
    
    function SetOutputFile(const pOutFileName : UnicodeString): Boolean;
    function Start(dBeginTime: Double; dLength: Double;
               hMsgReceiverWnd : HWND; fnCallback : TMULTI_CALLBACK; pUserObj : Pointer) : Integer; overload;

    function Start(dBeginTime: Double; dLength: Double;
               hMsgReceiverWnd : HWND) : Integer; overload;
    function Pause(): Integer;
    function Resume(): Integer;
    function Stop(wParam : WPARAM = 0; lParam : LPARAM = 0) : Integer;
    function Abort(wParam : WPARAM = 0; lParam : LPARAM = 0) : Integer;
    procedure Wait(uMilliSecondsTimeOut : UINT = $FFFFFFFF);
    function EnablePreview(bEnable : Boolean): Boolean;
    function SetThreadPriority(nPriority: Integer): BOOL;
    function SetFullSpeed(Value: Boolean): BOOL;
    class function GetEstimateDiskSpaceEx(const pParam : TEncodeParam; dLength : double) : INT64;
    function SetSplitParam(nStyle: TEncodeSplitStyle; dwLimit: DWORD): BOOL;
    function GetTargetCount(): Integer;
    function GetTargetFileName(pFileName: PWideChar; nIndex: Integer): BOOL; overload;
    function GetTargetFileName(nIndex: Integer): UnicodeString; overload;

    property Handle: HMEDIAENCODER read FHandle;
    property EncodeParam: PEncodeParam read GetEncodeParam write SetEncodeParam;
    property EncodeParamEx: PEncodeParamEx read GetEncodeParamEx write SetEncodeParamEx;
    property OutputFile: UnicodeString read FOutputFile write SetOutputFileProc;
    property VideoDecoder: IVideoDecoder read FVideoDecoder;
    property AudioDecoder: IAudioDecoder read FAudioDecoder;
    property Status: TEncoderStatus read FStatus;
    property OnEncodeProgress: TMediaEncodeProgressEvent read FOnEncodeProgress write FOnEncodeProgress;
  end;


  TMediaProcessEncoder = class(TMediaEncoder)
  private
    FAudioProcessor: TAudioMediaProcessor;
    FVideoProcessor: TVideoMediaProcessor;
    FResizeParam: RESIZE_PARAM;
    FpResizeParam: PRESIZE_PARAM;
    FFrameRate: double;
    FSampleRate, FChannels, FBitsPerSample: Integer;
    FEvenFlag: BOOL;
  public
    constructor Create(const AFileName: UnicodeString; const AVidDecParam: TVideoDecParam;
      const AAudDecParam: TAudioDecParam; ADecModel: DEC_MODEL=AUTO_MODEL;
      const pResizeParam : PRESIZE_PARAM = nil; dFrameRate : double = -1;
      nSampleRate : Integer = 0; nChannels : Integer = 0;
      nBitsPerSample : Integer = 0; bEvenFlag : BOOL = FALSE); overload;
    constructor Create(const AVideo: IVideoDecoder; const AAudio: IAudioDecoder); override;
    constructor Create(AVideo: TVideoMediaProcessor; AAudio: TAudioMediaProcessor); overload;

    property VideoProcessor: TVideoMediaProcessor read FVideoProcessor;
    property AudioProcessor: TAudioMediaProcessor read FAudioProcessor;
  end;

  TMediaSlideEncoderEx = class(TMediaEncoder)
  private
    FVideoSlideDecoder: TSlideDecoderEx;
    FAudioSlideDecoder: TSlideDecoderEx;
  public
    constructor Create(pVidParam: PVideoSlideParam; pAudParam: PAudioSlideParam); overload;
    constructor Create(pVidParam: PVideoSlideParam; pAudParam: PAudioSlideParam;
      AVideoClipEvent: TVideoSlideClipEvent; AAudioClipEvent: TAudioSlideClipEvent); overload;

    property VideoSlideDecoder: TSlideDecoderEx read FVideoSlideDecoder;
    property AudioSlideDecoder: TSlideDecoderEx read FAudioSlideDecoder;
  end;

  var
    MediaEncodeAPI: TMediaEncodeAPI;
implementation

{ TMediaEncoder }

function OnEncodeCallBack(pUserObj: Pointer; uMsg: UINT; wParam: WPARAM; lParam: LPARAM): BOOL; stdcall;
// 编码时的预览使用回调函数
// uMsg -    0 进度消息  wParam - 进度(0-100)  lParam - ProgressInfo*
//           1 转换完成  wParam - 0 正常完成  1 用户终止 2 外部回掉终止 3 遭遇错误
//         100 第二次进度开始
//           101 进度消息  wParam - 进度(0-10000)  lParam - 0
//         110 第二次进度结束 在第二次进度期间 不能调用WEPause和WEStop函数
// 返回FALSE 则停止转换
var
  EncodingState: TMediaEncodingState;
begin
  with TMediaEncoder(pUserObj) do
  begin
    case uMsg of
      0:
        begin
          EncodingState := mesEncoding;
        end;
      1:
        case wParam of
          0: EncodingState := mesFinished;
          1: EncodingState := mesUserAbort;
          2: EncodingState := mesCallBackAbort;
          3: EncodingState := mesError;
        else
          EncodingState:=mesUnknown;
        end;
      100: EncodingState := mesTwoPassStart;
      101: EncodingState := mesTwoPassEncoding;
      110: EncodingState := mesTwoPassFinished;
    else
      EncodingState:=mesUnknown;
    end;
    Result:=DoProgress(EncodingState, wParam div 100, 100, PMediaProgressInfo(lParam));
  end;
end;

function TMediaEncoder.Abort(wParam: WPARAM; lParam: LPARAM): Integer;
begin

end;

constructor TMediaEncoder.Create(const AFileName: UnicodeString; AWidth,
  AHeight: Integer);
begin

end;

constructor TMediaEncoder.Create(const AFileName: UnicodeString;
  const AVidDecParam: TVideoDecParam; const AAudDecParam: TAudioDecParam;
  ADecModel: DEC_MODEL; ADirectCopy: Boolean);
var
  pMediaDecoder: TMediaDecoder;
begin
  FDirectCopy := ADirectCopy;
  try
    pMediaDecoder := TMediaDecoder.Create(AFileName, AVidDecParam, AAudDecParam, ADecModel);
  except
    pMediaDecoder := nil;
    if Assigned(Classes.ApplicationHandleException) then
      Classes.ApplicationHandleException(Self);
  end;
  if Assigned(pMediaDecoder) and (pMediaDecoder.Handle=nil) then
      FreeAndNil(pMediaDecoder);
  if Assigned(pMediaDecoder) then
    Create(pMediaDecoder, pMediaDecoder);
end;

constructor TMediaEncoder.Create(const AVideo: IVideoDecoder;
  const AAudio: IAudioDecoder);
var
  hVideo: HVIDEODECODER;
  hAudio: HAUDIODECODER;
  pVidInput: IVideoInput;
  pAudInput: IAudioInput;
begin
  hVideo := nil;
  hAudio := nil;
  if Assigned(AVideo) then
    hVideo := AVideo.VideoDecoder;
  if Assigned(AAudio) then
    hAudio := AAudio.AudioDecoder;
  if not FDirectCopy then
    WSCoCreateInstance(CLSID_CMediaConverter, nil, 0, IID_IMediaConverter, FHandle)
  else
    WSCoCreateInstance(CLSID_CLossLessCopy, nil, 0, IID_IMediaConverter, FHandle);
  if Assigned(FHandle) then
  begin
    pVidInput := nil;
    FHandle.QueryInterface(IID_IVideoInput, pVidInput);
    if Assigned(pVidInput) and Assigned(hVideo) then
    begin
      pVidInput.SetVideoInput(hVideo);
      pVidInput._Release;
    end;
    pAudInput := nil;
    FHandle.QueryInterface(IID_IAudioInput, pAudInput);
    if Assigned(pAudInput) and Assigned(hAudio) then
    begin
      pAudInput.SetAudioInput(hAudio);
      pAudInput._Release;
    end;
  end;
  if Assigned(hVideo) then
    hVideo._Release;
  if Assigned(hAudio) then
    hAudio._Release;
end;

destructor TMediaEncoder.Destroy;
begin

  inherited;
end;

function TMediaEncoder.DoProgress(EncodingState: TMediaEncodingState; AProgress,
  AProgressMax: Integer; AProgressInfo: PMediaProgressInfo): Boolean;
begin

end;

function TMediaEncoder.EnablePreview(bEnable: Boolean): Boolean;
begin

end;

function TMediaEncoder.GetEncodeParam: PEncodeParam;
begin

end;

function TMediaEncoder.GetEncodeParamEx: PEncodeParamEx;
begin

end;

class function TMediaEncoder.GetEstimateDiskSpaceEx(const pParam: TEncodeParam;
  dLength: double): INT64;
begin

end;

function TMediaEncoder.GetTargetCount: Integer;
begin
  Result := 0;
  if Assigned(FHandle) then
    Result := FHandle.GetTargetCount;
end;

function TMediaEncoder.GetTargetFileName(pFileName: PWideChar;
  nIndex: Integer): BOOL;
begin
//  if Assigned(FHandle) then
//  begin
//    StrCopy(
//    FHandle.GetTargetFileName(nIndex, pFileName);
//  end;
end;

function TMediaEncoder.GetTargetFileName(nIndex: Integer): UnicodeString;
begin
  if Assigned(FHandle) then
  begin
    FHandle.GetTargetFileName(nIndex, Result);
  end;
end;

function TMediaEncoder.Pause: Integer;
begin
  Result := 0;
  if Assigned(FHandle) then
  begin
    if FHandle.Pause = S_OK then
      Result := 1;
  end;
end;

function TMediaEncoder.Resume: Integer;
begin
  Result := 0;
  if Assigned(FHandle) then
  begin
    if FHandle.Resume = S_OK then
      Result := 1;
  end;
end;

procedure TMediaEncoder.SetEncodeParam(pParam: PEncodeParam);
var
  pEncParam: IMediaEncParam;
  VidStreamParam: TStreamParam;
  AudStreamParam: TStreamParam;
begin
  if not Assigned(pParam) then
    Exit;
  if Assigned(FHandle) then
  begin
    FHandle.GetEncodeParam(pEncParam);
    if Assigned(pEncParam) then
    begin
      pEncParam.SetFourCC(pParam.dwFourCC);
      if not pParam.bVideoDisable then
      begin
        VidStreamParam.dwFourCC := pParam.Video.dwFourCC;
        VidStreamParam.nBufSize := sizeof(TVideoParam);
        GetMem(VidStreamParam.pBuffer, sizeof(TVideoParam));
        CopyMemory(VidStreamParam.pBuffer, @pParam.Video, sizeof(TVideoParam));
        FreeMem(VidStreamParam.pBuffer, sizeof(TVideoParam));
      end;

      if not pParam.bAudioDisable then
      begin
        AudStreamParam.dwFourCC := pParam.Audio.dwFourCC;
        AudStreamParam.nBufSize := sizeof(TAudioParam);
        GetMem(AudStreamParam.pBuffer, sizeof(TAudioParam));
        CopyMemory(AudStreamParam.pBuffer, @pParam.Audio, sizeof(TAudioParam));
        FreeMem(AudStreamParam.pBuffer, sizeof(TAudioParam));
      end;
      pEncParam._Release;
    end;
  end;
end;

procedure TMediaEncoder.SetEncodeParamEx(const Value: PEncodeParamEx);
begin
end;

function TMediaEncoder.SetFullSpeed(Value: Boolean): BOOL;
begin

end;

function TMediaEncoder.SetOutputFile(
  const pOutFileName: UnicodeString): Boolean;
begin
  Result := False;
  if Assigned(FHandle) then
    Result := FHandle.SetOutputFile(pOutFileName) = S_OK;  
end;

procedure TMediaEncoder.SetOutputFileProc(const Value: UnicodeString);
begin
  if SetOutputFile(Value) then
    FOutputFile := Value;
end;

function TMediaEncoder.SetSplitParam(nStyle: TEncodeSplitStyle;
  dwLimit: DWORD): BOOL;
begin
  if Assigned(FHandle) then
  begin
    FHandle.SetSplitParam(Ord(nStyle), dwLimit);
  end;
end;

function TMediaEncoder.SetThreadPriority(nPriority: Integer): BOOL;
begin
end;

function TMediaEncoder.Start(dBeginTime, dLength: Double;
  hMsgReceiverWnd: HWND): Integer;
begin
  Result := Start(dBeginTime, dLength, hMsgReceiverWnd, @OnEncodeCallBack, Self);
end;

function TMediaEncoder.Start(dBeginTime, dLength: Double; hMsgReceiverWnd: HWND;
  fnCallback: TMULTI_CALLBACK; pUserObj: Pointer): Integer;
begin
  Result:=-1;
  if FStatus in [esHaveFile, esNone] then
  begin
    FAborting:=False;
    FHandle.SetClipTime(dBeginTime, dLength);
    FHandle.SetCallBack(fnCallback, pUserObj);
    if Assigned(FHandle) then
    begin
      if FHandle.Start = S_OK then Result := 0;
      if Result = 0 then
      begin
        FStatus := esEncoding;
      end;
    end;
  end;
end;

function TMediaEncoder.Stop(wParam: WPARAM; lParam: LPARAM): Integer;
begin
  Result := 0;
  if (FStatus in [esEncoding, esPause]) and Assigned(FHandle) then
  begin
    FAborting := True;
    if Assigned(FHandle) then
    begin
      if FHandle.Stop = S_OK then
        Result := 1;
    end;
    FStatus := esNone;
    FAborting := False;
  end;
end;

procedure TMediaEncoder.Wait(uMilliSecondsTimeOut: UINT);
begin
end;

{ TMediaProcessEncoder }

constructor TMediaProcessEncoder.Create(const AFileName: UnicodeString;
  const AVidDecParam: TVideoDecParam; const AAudDecParam: TAudioDecParam;
  ADecModel: DEC_MODEL; const pResizeParam: PRESIZE_PARAM; dFrameRate: double;
  nSampleRate, nChannels, nBitsPerSample: Integer; bEvenFlag: BOOL);
begin
  FpResizeParam:=pResizeParam;
  if Assigned(pResizeParam) then
  begin
    FResizeParam:=pResizeParam^;
    FpResizeParam:=@FResizeParam;
  end;
  FFrameRate:=dFrameRate;
  FSampleRate:=nSampleRate;
  FChannels:=nChannels;
  FBitsPerSample:=nBitsPerSample;
  FEvenFlag:=bEvenFlag;
  inherited Create(AFileName, AVidDecParam, AAudDecParam, ADecModel);
end;

constructor TMediaProcessEncoder.Create(const AVideo: IVideoDecoder;
  const AAudio: IAudioDecoder);
var
  pVideoIntf: IVideoDecoder;
  pAudioIntf: IAudioDecoder;
begin
  FVideoProcessor := TVideoMediaProcessor.Create(AVideo.VideoDecoder);
  FAudioProcessor := TAudioMediaProcessor.Create(AAudio.AudioDecoder);
  pVideoIntf := FVideoProcessor;
  pAudioIntf := FAudioProcessor;
  inherited Create(pVideoIntf, pAudioIntf);
end;

constructor TMediaProcessEncoder.Create(AVideo: TVideoMediaProcessor;
  AAudio: TAudioMediaProcessor);
var
  pVideoIntf: IVideoDecoder;
  pAudioIntf: IAudioDecoder;
begin
  FVideoProcessor:=AVideo;
  FAudioProcessor:=AAudio;
  pVideoIntf:=AVideo;
  pAudioIntf:=AAudio;
  inherited Create(pVideoIntf, pAudioIntf);
end;

{ TMediaSlideEncoderEx }

constructor TMediaSlideEncoderEx.Create(pVidParam: PVideoSlideParam;
  pAudParam: PAudioSlideParam);
begin
  FVideoSlideDecoder := TSlideDecoderEx.Create(pVidParam^, pAudParam^);
  FAudioSlideDecoder := FVideoSlideDecoder;
  inherited Create(FVideoSlideDecoder, FAudioSlideDecoder);
end;

constructor TMediaSlideEncoderEx.Create(pVidParam: PVideoSlideParam;
  pAudParam: PAudioSlideParam; AVideoClipEvent: TVideoSlideClipEvent;
  AAudioClipEvent: TAudioSlideClipEvent);
begin
  FVideoSlideDecoder := TSlideDecoderEx.Create(pVidParam^, pAudParam^);
  FAudioSlideDecoder := FVideoSlideDecoder;
  inherited Create(FVideoSlideDecoder, FAudioSlideDecoder);
end;

{ TMediaEncodeAPI }

procedure TMediaEncodeAPI.Init;
begin

end;

function TMediaEncodeAPI.WEGetEstimateDiskSpaceEx(const pParam: TENCODEPARAM;
  dLength: double): INT64;
var
  nVideoBitrate: Integer;
  nAudioBitrate: Integer;
  nBitrate: Integer;
  EncParam: TEncodeParam;
begin
  EncParam := pParam;
  nVideoBitrate := pParam.Video.nBitrate;
  nAudioBitrate := pParam.Audio.nBitrate;
  if (pParam.Video.dwFourCC = MakeFourCC('DV50'))
    or (pParam.dwFourCC = MakeFourCC('dv  '))
  then
  begin
    if EncParam.Video.dFrameRate < 25 then
      EncParam.Video.dFrameRate := 25;
    nVideoBitrate :=
      Round(EncParam.Video.nWidth * EncParam.Video.nHeight * EncParam.Video.dFrameRate * 8 / 5);
  end;
  if EncParam.Video.bIsVBR then
    nVideoBitrate := EncParam.Video.nMaxBitrate * 6 div 10 + EncParam.Video.nBitrate * 4 div 10;

  if EncParam.Audio.bIsVBR then
    nAudioBitrate := EncParam.Audio.nMaxBitrate * 6 div 10 + EncParam.Audio.nBitrate * 4 div 10;

  nBitrate := 0;
  if not EncParam.bVideoDisable then
    nBitrate := nBitrate + nVideoBitrate;
  if not EncParam.bAudioDisable then
    nBitrate := nBitrate + nAudioBitrate;
  Result := Round(nBitrate  * dLength / 8);
end;

end.
