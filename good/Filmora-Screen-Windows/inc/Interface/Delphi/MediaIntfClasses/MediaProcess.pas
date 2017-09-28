unit MediaProcess;

interface
uses
  Windows, Classes, MediaCommon, WSTextArt, MediaDibImage, MediaDecode, WSSubPic;
const
  CLSID_CVideoCompositor:     TGUID = '{375daf31-a4b7-40b0-9f2b-3b90ac944536}';
  IID_IVideoCompositor:       TGUID = '{A32480A3-582E-4785-AA98-B46B2C317252}';
  IID_IVideoCompositonItem:   TGUID = '{7060C309-9996-49ff-B5E3-74093A6ABB98}';
  CLSID_CVideoResizer:        TGUID = '{fc2c8098-6c82-492a-926c-cbb964d12d3f}';
  IID_IVideoResizer:          TGUID = '{D7F70D4F-AAD6-4d92-9CE4-B95641E2D865}';
  CLSID_CVideoRotation:       TGUID = '{807dea6d-2e28-41b4-89a8-2d330cba4dfc}';
  IID_IVideoRotation:         TGUID = '{37300932-932C-42a6-852F-B3C9CFD84C83}';
  CLSID_CVideoCopy:           TGUID = '{c2aa0e9e-3e1a-48a9-b288-99a59da0cb9d}';
  IID_IVideoCopy:             TGUID = '{C2B0EEA2-6E70-45df-A06B-B096146EFB56}';
  CLSID_CVideoEffect:         TGUID = '{9f97a1bf-884d-4c14-b9cc-4da7cdf35bc7}';
  IID_IVideoEffect:           TGUID = '{AC3DDFA6-1974-42d5-880E-C0CEAA194D93}';
  CLSID_CVideoCrop:           TGUID = '{465a9d1d-55d6-4bdd-9232-9b035e796667}';
  IID_IVideoCrop:             TGUID = '{2081D016-BF75-4029-8E17-49A53C2F3541}';
  CLSID_CVideoSlider:         TGUID = '{315ab12e-abfb-4327-a38a-2b0b886df849}';
  IID_IVideoSlider:           TGUID = '{52752A39-9BD7-4333-BA14-6982882255FB}';
  CLSID_CAudioSlider:         TGUID = '{911b921b-b28a-44d1-a0ed-4bdecf18a3bc}';
  IID_IAudioSlider:           TGUID = '{F7591170-F4C6-46ea-BA3B-6FF1C8942EEC}';
  CLSID_CAudioFade:           TGUID = '{68cce4e2-e906-47a8-9820-f710890d0a2a}';         
  IID_IAudioFade:             TGUID = '{E2EA8730-B88E-48a9-AE1E-09E9A5ECC99C}';
  CLSID_CVolumeSetting:       TGUID = '{e2737827-e04a-4969-aabb-badbd0cfe40f}';
  IID_IVolumeSetting:         TGUID = '{742F6246-6D45-4d96-9032-463749B59F67}';
  CLSID_CAudioProcess:        TGUID = '{c1242906-5995-4ed4-b1ab-9faee1cfcce7}';
  IID_IAudioProcess:          TGUID = '{CDC6C251-70B0-42d0-B800-978AC93428A7}';

const
  EX_FILTER_GRAY        = $0001;
  EX_FILTER_EMBOSS      = $0002;
  EX_FILTER_NEGATIVE    = $0004;
  EX_FILTER_BLACK_WHITE = $0008;
  EX_FILTER_WHITE_BLACK = $0010;
  EX_FILTER_OLD_PHOTO   = $0020;

// FLIP_FLAG
const
  FLIP_HORIZONTAL       = $0001;
  FLIP_VERTICAL         = $0002;

  KEEP_ORIGINAL_VOLUME = MediaDecode.KEEP_ORIGINAL_VOLUME;

type
  tagFRECT = record
    left: Single;
    top: Single;
    right: Single;
    bottom: Single;
  end;
  TFRECT = tagFRECT;
  PFRECT = ^tagFRECT;
  
  TUserWatermarkParam = record
    lpbi: PBITMAPINFO;
    lpBits: PBYTE;
    fRect: TFRECT;
    nOpacity: Longint;
  end;
  PUserWatermarkParam = ^TUserWatermarkParam;

  IAudioProcess = interface(IUnknown)
    ['{CDC6C251-70B0-42d0-B800-978AC93428A7}']
    function SetSampleRate(nSampleRate: Integer): HRESULT; stdcall;
    function GetSampleRate(): Integer; stdcall;
    function SetChannels(nChannels: Integer): HRESULT; stdcall;
    function GetChannels(): Integer; stdcall;
  end;
  
  IVolumeSetting = interface(IUnknown)
    ['{742F6246-6D45-4d96-9032-463749B59F67}']
    function SetVolume(nVolume: Integer): HRESULT; stdcall;
    function GetVolume(): Integer; stdcall;
  end;
  
  IAudioFade = interface(IUnknown)
    ['{E2EA8730-B88E-48a9-AE1E-09E9A5ECC99C}']
    function SetFadeInOutTime(dFadeInStartTime: Double; dFadeInLength: Double;
      dFadeOutStartTime: Double; dFadeOutLength: Double): HRESULT; stdcall;
    function GetFadeInOutTime(var dFadeInStartTime: Double; var dFadeInLength: Double;
      var dFadeOutStartTime: Double; var dFadeOutLength: Double): HRESULT; stdcall;
  end;
  
  IAudioSlider = interface(IUnknown)
    ['{F7591170-F4C6-46ea-BA3B-6FF1C8942EEC}']
    function SetAlignHead(bAlignHead: Integer): HRESULT; stdcall;
    function GetAlignHead(): Integer; stdcall;
  end;
  
  IVideoSlider = interface(IUnknown)
    ['{52752A39-9BD7-4333-BA14-6982882255FB}']
    function SetOutputFrameRate(dFrameRate: Double): HRESULT; stdcall;
    function GetOutputFrameRate(): Double; stdcall;
    function SetAlignHead(bAlignHead: Integer): HRESULT; stdcall;
    function GetAlignHead(): Integer; stdcall;
  end;

  IVideoCrop = interface(IUnknown)
    ['{2081D016-BF75-4029-8E17-49A53C2F3541}']
    function SetCropRect(const cropRect: TRECT): HRESULT; stdcall;
    function GetCropRect(): TRECT; stdcall;
  end;

  IVideoEffect = interface(IUnknown)
    ['{AC3DDFA6-1974-42d5-880E-C0CEAA194D93}']
    function SetFilterFlag(uFlag: UINT): HRESULT; stdcall;
    function GetFilterFlag(): UINT; stdcall;
    function SetBrightness(nValue: Integer): HRESULT; stdcall;
    function GetBrightness(): Integer; stdcall;
    function SetSaturation(nValue: Integer): HRESULT; stdcall;
    function GetSaturation(): Integer; stdcall;
    function SetDeintelace(uDeintelace: DEINTERLACE_STYLE): HRESULT; stdcall;
    function GetDeintelace(): DEINTERLACE_STYLE; stdcall;
    function GetContrast(): Integer; stdcall;
    function SetContrast(nValue: Integer): HRESULT; stdcall;
  end;

  IVideoCopy = interface(IUnknown)
    ['{C2B0EEA2-6E70-45df-A06B-B096146EFB56}']
    function SetNeedCopyFrame(bNeed: BOOL): HRESULT; stdcall;
    function GetNeedCopyFrame(): BOOL; stdcall;
  end;
  
  IVideoRotation = interface(IUnknown)
    ['{37300932-932C-42a6-852F-B3C9CFD84C83}']
    function SetFlipFlag(uFlag: UINT): HRESULT; stdcall;
    function GetFlipFlag(): UINT; stdcall;
    function SetRotation(nRotation: Integer; nDrag: Integer; flg: Integer): HRESULT; stdcall;
    function GetRotation(var nRotation: Integer; var nDrag: Integer; var flg: Integer): HRESULT; stdcall;
  end;
  
  IVideoResizer = interface(IUnknown)
    ['{D7F70D4F-AAD6-4d92-9CE4-B95641E2D865}']
    function SetOutputSize(nWidth: Integer; nHeight: Integer): HRESULT; stdcall;
    function GetOutputSize(var nWidth: Integer; var nHeight: Integer): HRESULT; stdcall;
    function SetInterpolationMode(interpolationMode: IMAGE_INTERPOLATION): HRESULT; stdcall;
    function GetInterpolationMode(): IMAGE_INTERPOLATION; stdcall;
    function SetResizeMode(resizeMode: IMAGE_RESIZE_METHOD): HRESULT; stdcall;
    function GetResizeMode(): IMAGE_RESIZE_METHOD; stdcall;
  end;
  
  IVideoCompositonItem = interface(IUnknown)
    ['{7060C309-9996-49ff-B5E3-74093A6ABB98}']
    function SetRect(const Rect: FRECT): HRESULT; stdcall;
    function GetRect(): FRECT; stdcall;
    function SetOpacity(nOpacity: Integer): HRESULT; stdcall;
    function SetStartTime(startTime: Int64): HRESULT; stdcall;
    function GetStartTime(): int64; stdcall;
    function SetDuration(duration: int64): HRESULT; stdcall;
    function GetDuration(): int64; stdcall;
  end;
  
  IVideoCompositor = interface(IUnknown)
    ['{A32480A3-582E-4785-AA98-B46B2C317252}']
    function AddByMediaStream(pMediaStream: IMediaStream): Pointer; stdcall;
    function AddByFileName(const pFileName: WideString): Pointer; stdcall;
    function AddByBitmap(pDIBImage: IDibImage): Pointer; stdcall;
    function GetCount(): Integer; stdcall;
    function GetItem(nIndex: Integer; var pVideoCompositonItem: IVideoCompositonItem): HRESULT; stdcall;
    function Remove(pVideoCompositonItem: IVideoCompositonItem; var nIndex: Integer): HRESULT; stdcall;
  end;

  TEffectStream = class
  protected
    FDataProcessList: TList;
    function EnableDataProcess(const clsid: TGUID; unkOuter: IUnknown;
      dwClsContext: Longint; const iid: TGUID; var pv: IUnknown): HRESULT;
    constructor Create;
  public
    destructor Destroy; override;
    procedure SetAllNULL(); virtual; abstract;
  end;

  TAudioEffectStream = class(TEffectStream)
  private
    FAudProcessor: IUnknown;
    FVolumeSetting: IUnknown;
    FAudFade: IUnknown;
    FAudioSlider: IUnknown;
  public
    procedure SetAllNULL(); override;
    function EnableAudioProcessor(): HRESULT;
    function EnableVolumeSetting(): HRESULT;
    function EnableAudioFade(): HRESULT;
    function EnableAudioSlider(): HRESULT;

    procedure SetVolume(nVolume: Integer);
    procedure SetSampleRate(nSampleRate: Integer);
    procedure SetChannels(nChannels: Integer);
    procedure SetFadeInOutTime(dFadeInStartTime: Double; dFadeInLength: Double;
      dFadeOutStartTime: Double; dFadeOutLength: Double);

    procedure GetStream(pInStream: IMediaStream; var ppOutStream: IMediaStream);
  end;

  TVideoEffectStream = class(TEffectStream)
  private
    FPCText: IUnknown;
    FVideoCrop: IUnknown;
    FVideoEffectDeintelace: IUnknown;
    FVideoEffect: IUnknown;
    FVideoRotation: IUnknown;
    FVideoResizer: IUnknown;
    FVideoCompositor: IUnknown;
    FVideoSubPicCompositor: IUnknown;
    FVideoSlider: IUnknown;
    FVideoCopy: IUnknown;
  public
    procedure SetAllNULL(); override;
    function EnableVideoCrop(): HRESULT;
    function EnableVideoEffectDeintelace(): HRESULT;
    function EnableVideoEffect(): HRESULT;
    function EnableVideoRotation(): HRESULT;
    function EnableVideoResizer(): HRESULT;
    function EnableVideoCompositor(): HRESULT;
    function EnableVideoSubPicCompositor(): HRESULT;
    function EnablePCText(): HRESULT;
    function EnableVideoSlider(): HRESULT;
    function EnableVideoCopy(): HRESULT;

    procedure SetResizeParam(pResizeParam: PRESIZE_PARAM);
    procedure GetOutputSize(var nWidth: Integer; var nHeight: Integer);
    procedure SetInterpolationMode(interpolation: IMAGE_INTERPOLATION);
    procedure SetResizeMode(irm: IMAGE_RESIZE_METHOD);
    procedure SetCropRect(const CropRect: TRect);
    procedure GetCropRect(var pCropRect: TRect);
    procedure SetFilterFlag(uFlag: UINT);
    procedure SetRotation(nRotation: Integer; nDrag: Integer; flg: Integer);
    procedure SetFlipFlag(uFlag: UINT);
    procedure SetBrightness(nValue: Integer);
    procedure SetSaturation(nValue: Integer);
    procedure SetDeintelace(uDeintelace: DEINTERLACE_STYLE);
    procedure SetContrast(nValue: Integer);
    procedure SetFramerate(dFramerate: Double);
    procedure AddWaterMarkFile(pFileName: WideString; nScreenWidth: Integer; nScreenHeight: Integer; var pVideoCompositionItem: IVideoCompositonItem);
    procedure AddWaterMarkText(
      const pText: WideString;
      nFontSize: Integer;
      crFont: COLORREF;
      strFontName: WideString;
      nScreenWidth: Integer;
      nScreenHeight: Integer;
      var pVideoCompositionItem: IVideoCompositonItem);
    procedure AddSubTitle(pSubPicStream: IMediaStream; pVidParam: PDecParam; pVideoCompositonItem: IVideoCompositonItem);
    function RemoveCompositionItem(pItem: IVideoCompositonItem): Integer;
    function RemoveSubPicCompositionItem(pItem: IVideoCompositonItem): Integer;
    procedure GetStream(pInStream: IMediaStream; var ppOutStream: IMediaStream);
  end;

  TVideoSettingRec = record
    FVideoCropRect: TRect;
    Brightness: Longint;
    Contrast: Longint;
    Saturation: Longint;
  end;

  TVideoMediaProcessor = class(TInterfacedObject, IVideoDecoder)
  private
    FHandle: HVIDEODECODER;
    FSrcDecoder: IVideoDecoder;
    FOwnedDecoder: Boolean;
    FFlipFlag: UINT;
    FVideoSettingRec: TVideoSettingRec;
    FVideoInfo: TWSVideoInfo;
    FVideoEffectStream: TVideoEffectStream;
    FWatermarkDibImage: IDibImage;
    FSubPicItem: IVideoCompositonItem;
    function GetVideoDecoder: HVIDEODECODER;
    procedure SetVideoSettingRec(const Value: TVideoSettingRec);
    procedure SetFlipFlagProc(Value: UINT);
  public
    constructor Create(AVideoDecoder: HVIDEODECODER; AOwned: Boolean=True); overload;
    constructor Create(const hSrcVideo: IVideoDecoder; pResizeParam : PRESIZE_PARAM = nil; dFrameRate : double = -1); overload;
    destructor Destroy; override;

    function GetVideoInfo: PWSVideoInfo;

    procedure SetCropRect(const pCropRect: TRect);
    procedure SetCropRectEx(const pCropRect: TRect; irm: IMAGE_RESIZE_METHOD);
    procedure SetBrightness(nValue: Integer);
    procedure SetContrast(nValue: Integer);
    procedure SetSaturation(nValue: Integer);
    procedure DisableWatermark(bDisable: Boolean);
    procedure SetWatermarkSrcID(hModule: Cardinal; const pID: PWideChar; const pRect: PRECT = nil; nOpacity: Integer = 100);
    procedure SetWatermarkSrcIDAlign(hModule: HMODULE; pID: PWideChar; nPos: Integer = 0; nOpacity: Integer = 100);
    procedure SetWatermarkImage(lpbi: PBITMAPINFO; lpBits: PBYTE; const pRect: PRECT = nil; nOpacity: Integer = 100);
    procedure SetWatermarkImageAlign(lpbi: PBITMAPINFO; lpBits: PBYTE; nPos: Integer = 0; nOpacity: Integer = 100);
    procedure SetUserWatermarkImage(lpbi: PBITMAPINFO; lpBits: PBYTE; AFRect: PFRECT = nil; nOpacity: Integer = 100); overload;
    procedure SetUserWatermarkImage(hDibImage: HDIBIMAGE; AFRect: PFRECT = nil; nOpacity: Integer = 100); overload;
    procedure SetExFilterFlag(uFlag: UINT);
    procedure SetFlipFlag(uFlag: UINT);
    procedure SetRotation(nAngle: UINT; nDrag: Integer = 0);
    procedure SetDeintelace(uDeintelace: DEINTERLACE_STYLE);
    procedure SetUserSubPicture(pUserSubPic: PUserSubPicRec; nOpacity: Integer = 100);
    function CaptureImage(const AFileName: UnicodeString; nFormat : Integer; bWaterMark : Boolean): Boolean;
    function SetSubPicEx(const pUserSubPicEx: TUserSubPicEx; nOpacity: Integer): Boolean; // 如果返回False表示没有实现

    procedure SetResizeParam(pResizeParam: PRESIZE_PARAM);
    property Handle: HVIDEODECODER read GetVideoDecoder;
    property VideoSettingRec: TVideoSettingRec read FVideoSettingRec write SetVideoSettingRec;
    property FlipFlag: UINT read FFlipFlag write SetFlipFlagProc;
    property VideoInfo: PWSVideoInfo read GetVideoInfo;
    property SrcDecoder: IVideoDecoder read FSrcDecoder;
    property VideoEffectStream: TVideoEffectStream read FVideoEffectStream;
  end;

  TAudioMediaProcessor = class(TInterfacedObject, IAudioDecoder)
  private
    FHandle: HAUDIODECODER;
    FSrcDecoder: IAudioDecoder;
    FAudioInfo: TWSAudioInfo;
    FAudioEffectStream: TAudioEffectStream;
    function GetAudioDecoder: HAUDIODECODER;
  public
    constructor Create(AAudieoDecoder: HAUDIODECODER); overload;
    constructor Create(const hSrcAudio: IAudioDecoder; nSampleRate : Integer = 0; nChannels : Integer = 0; nBitsPerSample : Integer = 0; bEvenFlag : BOOL = FALSE); overload;
    destructor Destroy; override;

    function GetAudioInfo: PWSAudioInfo;

    procedure SetVolum(nValue: Integer);
    procedure SetSamplerate(nSamplerate: Integer);
    procedure SetChannel(nChannel: Integer);
    
    property Handle: HAUDIODECODER read GetAudioDecoder;
    property AudioInfo: PWSAudioInfo read GetAudioInfo;
    property SrcDecoder: IAudioDecoder read FSrcDecoder;

    property AudioEffectStream: TAudioEffectStream read FAudioEffectStream;
  end;
  
  function CalcRelativePos(const ARect: TRect; AWidth, AHeight: Integer): TFRect; overload;
  function CalcRelativePos(ALeft, ATop, AWidth, AHeight, AToWidth, AToHeight: Integer): TFRect; overload;

implementation
function CalcRelativePos(ALeft, ATop, AWidth, AHeight, AToWidth, AToHeight: Integer): TFRect;
var
  ratiox, ratioy: Double;
begin
  if (AToWidth=0) or (AToHeight=0) then
  begin
    Result.left:=0;
    Result.top:=0;
    Result.right:=0;
    Result.bottom:=0;
    Exit;
  end;
  ratiox:=1/AToWidth;
  ratioy:=1/AToHeight;
  Result.left:=ratiox*ALeft;
  Result.top:=ratioy*ATop;
  Result.right:=Result.left+ratiox*AWidth;
  Result.bottom:=Result.top+ratioy*AHeight;
end;

function CalcRelativePos(const ARect: TRect; AWidth, AHeight: Integer): TFRect;
begin
  with ARect do
    Result:=CalcRelativePos(Left, Top, Right-Left, Bottom-Top, AWidth, AHeight);
end;

{ TEffectStream }

constructor TEffectStream.Create;
begin
  FDataProcessList := TList.Create;
end;

destructor TEffectStream.Destroy;
var
  I: Integer;
  pUnknown: IUnknown;
begin
  SetAllNULL;
  for I := 0 to FDataProcessList.Count - 1 do
  begin
    pUnknown := IUnknown(FDataProcessList[I]);
    pUnknown._Release;
  end;
  inherited;
end;

function TEffectStream.EnableDataProcess(const clsid: TGUID;
  unkOuter: IInterface; dwClsContext: Integer; const iid: TGUID;
  var pv: IUnknown): HRESULT;
begin
  Result := S_FALSE;
  if not assigned(pv) then
  begin
    Result := WSCoCreateInstance(clsid, unkOuter, dwClsContext, iid, pv);
    if Assigned(pv) then
    begin
      pv._AddRef;
      FDataProcessList.Add(Pointer(pv));
    end;
  end
  else
  begin
    Result := S_OK;
  end;
end;

{ TAudioEffectStream }

function TAudioEffectStream.EnableAudioFade: HRESULT;
begin
  Result := EnableDataProcess(CLSID_CAudioFade, nil, 0, IID_IAudioFade, FAudFade);
end;

function TAudioEffectStream.EnableAudioProcessor: HRESULT;
begin
  Result := EnableDataProcess(CLSID_CAudioProcess, nil, 0, IID_IAudioProcess, FAudProcessor);
end;

function TAudioEffectStream.EnableAudioSlider: HRESULT;
begin
  Result := EnableDataProcess(CLSID_CAudioSlider, nil, 0, IID_IAudioSlider, FAudioSlider);
end;

function TAudioEffectStream.EnableVolumeSetting: HRESULT;
begin
  Result := EnableDataProcess(CLSID_CVolumeSetting, nil, 0, IID_IVolumeSetting, FVolumeSetting);
end;

procedure TAudioEffectStream.GetStream(pInStream: IMediaStream;
  var ppOutStream: IMediaStream);
var
  I: Integer;
  pAudInput: IAudioInput;
  pStream: IMediaStream;
  pAudOutput: IAudioOutput;
  pDataProcess: IUnknown;
begin
  ppOutStream := pInStream;
  if not Assigned(pInStream) then
    Exit;
  ppOutStream._AddRef;
  for I := 0 to FDataProcessList.Count - 1 do
  begin
    pDataProcess := IUnknown(FDataProcessList[I]);
    pDataProcess.QueryInterface(IID_IAudioInput, pAudInput);
    if Assigned(pAudInput) then
    begin
      pAudInput.SetAudioInput(ppOutStream);
      pDataProcess.QueryInterface(IID_IAudioOutput, pAudOutput);
      if Assigned(pAudOutput) then
      begin
        pAudOutput.GetAudioOutput(pStream);
        if Assigned(pStream) then
        begin
          if Assigned(ppOutStream) then
            ppOutStream._Release;
          ppOutStream := pStream;
        end;
        pAudOutput._Release;
      end;
      pAudInput._Release;
    end;
  end;
end;

procedure TAudioEffectStream.SetAllNULL;
var
  I: Integer;
  pDateProcess: IUnknown;
  pAudioInput: IAudioInput;
begin
  for I := 0 to FDataProcessList.Count - 1 do
  begin
    if Assigned(FDataProcessList[I]) then
    begin
      pDateProcess := IUnknown(FDataProcessList[I]);
      pDateProcess.QueryInterface(IID_IAudioInput, pAudioInput);
      if Assigned(pAudioInput) then
        pAudioInput.SetAudioInput(nil);
    end;
  end;
end;

procedure TAudioEffectStream.SetChannels(nChannels: Integer);
var
  pAudProcessor: IAudioProcess;
begin
  pAudProcessor := FAudProcessor as IAudioProcess;
  if Assigned(pAudProcessor) then
    pAudProcessor.SetChannels(nChannels);
end;

procedure TAudioEffectStream.SetFadeInOutTime(dFadeInStartTime, dFadeInLength,
  dFadeOutStartTime, dFadeOutLength: Double);
var
  pAudFade: IAudioFade;
begin
  pAudFade := FAudFade as IAudioFade;
  if Assigned(pAudFade) then
    pAudFade.SetFadeInOutTime(dFadeInStartTime, dFadeInLength, 
      dFadeOutStartTime, dFadeOutLength)
end;

procedure TAudioEffectStream.SetSampleRate(nSampleRate: Integer);
var
  pAudProcessor: IAudioProcess;
begin
  pAudProcessor := FAudProcessor as IAudioProcess;
  if Assigned(pAudProcessor) then
    pAudProcessor.SetSampleRate(nSampleRate);
end;

procedure TAudioEffectStream.SetVolume(nVolume: Integer);
var
  pVolumeSetting: IVolumeSetting;
begin
  pVolumeSetting := FVolumeSetting as IVolumeSetting;
  if Assigned(pVolumeSetting) then
    pVolumeSetting.SetVolume(nVolume);
end;

{ TVideoEffectStream }

procedure TVideoEffectStream.AddSubTitle(pSubPicStream: IMediaStream;
  pVidParam: PDecParam; pVideoCompositonItem: IVideoCompositonItem);
begin

end;

procedure TVideoEffectStream.AddWaterMarkFile(pFileName: WideString;
  nScreenWidth, nScreenHeight: Integer;
  var pVideoCompositionItem: IVideoCompositonItem);
begin
  
end;

procedure TVideoEffectStream.AddWaterMarkText(const pText: WideString;
  nFontSize: Integer; crFont: COLORREF; strFontName: WideString; nScreenWidth,
  nScreenHeight: Integer; var pVideoCompositionItem: IVideoCompositonItem);
begin
  
end;

function TVideoEffectStream.EnablePCText: HRESULT;
begin
  Result := EnableDataProcess(CLSID_CPCSText, nil, 0, IID_IPCText, FPCText);
end;

function TVideoEffectStream.EnableVideoCompositor: HRESULT;
begin
  Result := EnableDataProcess(CLSID_CVideoCompositor, nil, 0, IID_IVideoCompositor, FVideoCompositor);
end;

function TVideoEffectStream.EnableVideoCopy: HRESULT;
begin
  Result := EnableDataProcess(CLSID_CVideoCopy, nil, 0, IID_IVideoCopy, FVideoCopy);
end;

function TVideoEffectStream.EnableVideoCrop: HRESULT;
begin
  Result := EnableDataProcess(CLSID_CVideoCrop, nil, 0, IID_IVideoCrop, FVideoCrop);
end;

function TVideoEffectStream.EnableVideoEffect: HRESULT;
begin
  Result := EnableDataProcess(CLSID_CVideoEffect, nil, 0, IID_IVideoEffect, FVideoEffect);
end;

function TVideoEffectStream.EnableVideoEffectDeintelace: HRESULT;
begin
  Result := EnableDataProcess(CLSID_CVideoEffect, nil, 0, IID_IVideoEffect, FVideoEffectDeintelace);
end;

function TVideoEffectStream.EnableVideoResizer: HRESULT;
begin
  Result := EnableDataProcess(CLSID_CVideoResizer, nil, 0, IID_IVideoResizer, FVideoResizer);
end;

function TVideoEffectStream.EnableVideoRotation: HRESULT;
begin
  Result := EnableDataProcess(CLSID_CVideoRotation, nil, 0, IID_IVideoRotation, FVideoRotation);
end;

function TVideoEffectStream.EnableVideoSlider: HRESULT;
begin
  Result := EnableDataProcess(CLSID_CVideoSlider, nil, 0, IID_IVideoSlider, FVideoSlider);
end;

function TVideoEffectStream.EnableVideoSubPicCompositor: HRESULT;
begin
  Result := EnableDataProcess(CLSID_CVideoCompositor, nil, 0, IID_IVideoCompositor, FVideoSubPicCompositor);
end;

procedure TVideoEffectStream.GetCropRect(var pCropRect: TRect);
var
  pVideoCrop: IVideoCrop;
begin
  pVideoCrop := FVideoCrop as IVideoCrop;
  if Assigned(pVideoCrop) then
    pCropRect := pVideoCrop.GetCropRect;
end;

procedure TVideoEffectStream.GetOutputSize(var nWidth, nHeight: Integer);
var
  pVideoResizer: IVideoResizer;
begin
  pVideoResizer := FVideoResizer as IVideoResizer;
  if Assigned(pVideoResizer) then
    pVideoResizer.GetOutputSize(nWidth, nHeight);
end;

procedure TVideoEffectStream.GetStream(pInStream: IMediaStream;
  var ppOutStream: IMediaStream);
var
  pVidInput: IVideoInput;
  pVidOutput: IVideoOutput;
  pStream: IMediaStream;
  pDataProcess: IUnknown;
  I: Integer;
begin
  ppOutStream := pInStream;
  if not Assigned(pInStream) then
  begin
    Exit;
  end;
  ppOutStream._AddRef;
  for I := 0 to FDataProcessList.Count - 1 do
  begin
    pDataProcess := IUnknown(FDataProcessList[I]);
    pDataProcess.QueryInterface(IID_IVideoInput, pVidInput);
    if Assigned(pVidInput) then
    begin
      pVidInput.SetVideoInput(ppOutStream);
      pDataProcess.QueryInterface(IID_IVideoOutput, pVidOutput);
      if Assigned(pVidOutput) then
      begin
        pVidOutput.GetVideoOutput(pStream);
        if Assigned(pVidOutput) then
        begin
          if Assigned(pStream) then
          begin
            ppOutStream._Release;
            ppOutStream := pStream;
          end;  
          pVidOutput._Release;
        end;
      end;
      pVidInput._Release;
    end;
  end;
end;

function TVideoEffectStream.RemoveCompositionItem(
  pItem: IVideoCompositonItem): Integer;
var
  pVideoCompositor: IVideoCompositor;
begin
  Result := -1;
  pVideoCompositor := FVideoCompositor as IVideoCompositor;
  if Assigned(pVideoCompositor) then
    pVideoCompositor.Remove(pItem, Result);
end;

function TVideoEffectStream.RemoveSubPicCompositionItem(
  pItem: IVideoCompositonItem): Integer;
var
  pVideoCompositor: IVideoCompositor;
begin
  Result := -1;
  pVideoCompositor := FVideoSubPicCompositor as IVideoCompositor;
  if Assigned(pVideoCompositor) then
    pVideoCompositor.Remove(pItem, Result);
end;

procedure TVideoEffectStream.SetAllNULL;
var
  I, nIndex: Integer;
  pDateProcess: IUnknown;
  pVideoInput: IVideoInput;
  rect: TRect;
  pVideoCompositonItem: IVideoCompositonItem;
  pVideoCrop: IVideoCrop;
  pVideoEffect: IVideoEffect;
  pVideoRotation: IVideoRotation;
  pVideoResizer: IVideoResizer;
  pVideoCompositor: IVideoCompositor;
begin
  for I := 0 to FDataProcessList.Count - 1 do
  begin
    if Assigned(FDataProcessList[I]) then
    begin
      pDateProcess := IUnknown(FDataProcessList[I]);
      pDateProcess.QueryInterface(IID_IVideoInput, pVideoInput);
      if Assigned(pVideoInput) then
        pVideoInput.SetVideoInput(nil);
    end;
  end;
  rect.Left := 0;
  rect.Top := 0;
  rect.Right := 0;
  rect.Bottom := 0;
  if Assigned(FVideoCrop) then
  begin
    pVideoCrop := FVideoCrop as IVideoCrop;
    pVideoCrop.SetCropRect(rect);
  end;
  if Assigned(FVideoEffect) then
  begin
    pVideoEffect := FVideoEffect as IVideoEffect;
    pVideoEffect.SetBrightness(0);
    pVideoEffect.SetContrast(0);
    pVideoEffect.SetDeintelace(DIS_NONE);
    pVideoEffect.SetFilterFlag(0);
    pVideoEffect.SetSaturation(0);
  end;
  if Assigned(FVideoRotation) then
  begin
    pVideoRotation := FVideoRotation as IVideoRotation;
    pVideoRotation.SetFlipFlag(0);
    pVideoRotation.SetRotation(0, 0, 0);
  end;
  if Assigned(FVideoResizer) then
  begin
    pVideoResizer := FVideoResizer as IVideoResizer;
    pVideoResizer.SetInterpolationMode(IIN_AUTO);
  end;
  if Assigned(FVideoCompositor) then
  begin
    pVideoCompositor := FVideoCompositor as IVideoCompositor;
    for I := pVideoCompositor.GetCount - 1 downto 0 do
    begin
      pVideoCompositor.GetItem(I, pVideoCompositonItem);
      pVideoCompositor.Remove(pVideoCompositonItem, nIndex);
    end;
  end;
end;

procedure TVideoEffectStream.SetBrightness(nValue: Integer);
var
  pVideoEffect: IVideoEffect;
begin
  pVideoEffect := FVideoEffect as IVideoEffect;
  if Assigned(pVideoEffect) then
    pVideoEffect.SetBrightness(nValue);
end;

procedure TVideoEffectStream.SetContrast(nValue: Integer);
var
  pVideoEffect: IVideoEffect;
begin
  pVideoEffect := FVideoEffect as IVideoEffect;
  if Assigned(pVideoEffect) then
    pVideoEffect.SetContrast(nValue);
end;

procedure TVideoEffectStream.SetCropRect(const CropRect: TRect);
var
  pVideoCrop: IVideoCrop;
begin
  pVideoCrop := FVideoCrop as IVideoCrop;
  if Assigned(pVideoCrop) then
    pVideoCrop.SetCropRect(CropRect);
end;

procedure TVideoEffectStream.SetDeintelace(uDeintelace: DEINTERLACE_STYLE);
var
  pVideoEffectDeintelace: IVideoEffect;
begin
  pVideoEffectDeintelace := FVideoEffectDeintelace as IVideoEffect;
  if Assigned(pVideoEffectDeintelace) then
    pVideoEffectDeintelace.SetDeintelace(uDeintelace);
end;

procedure TVideoEffectStream.SetFilterFlag(uFlag: UINT);
var
  pVideoEffect: IVideoEffect;
begin
  pVideoEffect := FVideoEffect as IVideoEffect;
  if Assigned(pVideoEffect) then
    pVideoEffect.SetFilterFlag(uFlag);
end;

procedure TVideoEffectStream.SetFlipFlag(uFlag: UINT);
var
  pVideoRotation: IVideoRotation;
begin
  pVideoRotation := FVideoEffect as IVideoRotation;
  if Assigned(pVideoRotation) then
    pVideoRotation.SetFlipFlag(uFlag);
end;

procedure TVideoEffectStream.SetFramerate(dFramerate: Double);
var
  pVideoSlider: IVideoSlider;
begin
  pVideoSlider := FVideoSlider as IVideoSlider;
  if Assigned(pVideoSlider) then
    pVideoSlider.SetOutputFrameRate(dFramerate);
end;

procedure TVideoEffectStream.SetInterpolationMode(
  interpolation: IMAGE_INTERPOLATION);
var
  pVideoResizer: IVideoResizer;
begin
  pVideoResizer := FVideoResizer as IVideoResizer;
  if Assigned(pVideoResizer) then
    pVideoResizer.SetInterpolationMode(interpolation);
end;

procedure TVideoEffectStream.SetResizeMode(irm: IMAGE_RESIZE_METHOD);
var
  pVideoResizer: IVideoResizer;
begin
  pVideoResizer := FVideoResizer as IVideoResizer;
  if Assigned(pVideoResizer) then
    pVideoResizer.SetResizeMode(irm);
end;

procedure TVideoEffectStream.SetResizeParam(pResizeParam: PRESIZE_PARAM);
var
  pVideoResizer: IVideoResizer;
begin
  pVideoResizer := FVideoResizer as IVideoResizer;
  if Assigned(pResizeParam) and Assigned(pVideoResizer) then
  begin
    pVideoResizer.SetOutputSize(pResizeParam.width, pResizeParam.height);
    pVideoResizer.SetInterpolationMode(pResizeParam.uInterpolation);
    pVideoResizer.SetResizeMode(pResizeParam.uStyle);
  end;
end;

procedure TVideoEffectStream.SetRotation(nRotation, nDrag, flg: Integer);
var
  pVideoRotation: IVideoRotation;
begin
  pVideoRotation := FVideoRotation as IVideoRotation;
  if Assigned(pVideoRotation) then
    pVideoRotation.SetRotation(nRotation, nDrag, flg);
end;

procedure TVideoEffectStream.SetSaturation(nValue: Integer);
var
  pVideoEffect: IVideoEffect;
begin
  pVideoEffect := FVideoEffect as IVideoEffect;
  if Assigned(pVideoEffect) then
    pVideoEffect.SetSaturation(nValue);
end;

{ TVideoMediaProcessor }

function TVideoMediaProcessor.CaptureImage(const AFileName: UnicodeString;
  nFormat: Integer; bWaterMark: Boolean): Boolean;
begin

end;

constructor TVideoMediaProcessor.Create(AVideoDecoder: HVIDEODECODER;
  AOwned: Boolean);
begin
  inherited Create;
  FVideoEffectStream := TVideoEffectStream.Create;
  FHandle := AVideoDecoder;
  FOwnedDecoder:=AOwned;
end;

constructor TVideoMediaProcessor.Create(const hSrcVideo: IVideoDecoder;
  pResizeParam: PRESIZE_PARAM; dFrameRate: double);
begin
end;

destructor TVideoMediaProcessor.Destroy;
begin
  inherited;
end;

procedure TVideoMediaProcessor.DisableWatermark(bDisable: Boolean);
begin

end;

function TVideoMediaProcessor.GetVideoDecoder: HVIDEODECODER;
var
  pOutStream: IMediaStream;
begin
  if Assigned(FVideoEffectStream) then
  begin
    FVideoEffectStream.GetStream(FHandle, pOutStream);
  end;
  Result := pOutStream;
end;

function TVideoMediaProcessor.GetVideoInfo: PWSVideoInfo;
var
  pVidDecoder: IMediaStream;
  pStreamInfo: IStreamInfo;
begin
  pVidDecoder := GetVideoDecoder;
  if Assigned(pVidDecoder) then
  begin
    pVidDecoder.GetStreamInfo(pStreamInfo);
    if Assigned(pStreamInfo) then
    begin
      Result := pStreamInfo.GetVideoInfo;
      pStreamInfo._Release;
    end;
    pVidDecoder._Release;
  end;
end;

procedure TVideoMediaProcessor.SetBrightness(nValue: Integer);
begin
  if Assigned(FVideoEffectStream) then
  begin
    FVideoEffectStream.SetBrightness(nValue);
  end;
end;

procedure TVideoMediaProcessor.SetContrast(nValue: Integer);
begin
  if Assigned(FVideoEffectStream) then
  begin
    FVideoEffectStream.SetContrast(nValue);
  end;
end;

procedure TVideoMediaProcessor.SetCropRect(const pCropRect: TRect);
begin
  if Assigned(FVideoEffectStream) then
  begin
    FVideoEffectStream.SetCropRect(pCropRect);
  end;
end;

procedure TVideoMediaProcessor.SetCropRectEx(const pCropRect: TRect;
  irm: IMAGE_RESIZE_METHOD);
begin
  if Assigned(FVideoEffectStream) then
  begin
    FVideoEffectStream.SetResizeMode(irm);
    FVideoEffectStream.SetCropRect(pCropRect);
  end;
end;

procedure TVideoMediaProcessor.SetDeintelace(uDeintelace: DEINTERLACE_STYLE);
begin
  if Assigned(FVideoEffectStream) then
  begin
    FVideoEffectStream.SetDeintelace(uDeintelace);
  end;
end;

procedure TVideoMediaProcessor.SetExFilterFlag(uFlag: UINT);
begin
  if Assigned(FVideoEffectStream) then
  begin
    FVideoEffectStream.SetFilterFlag(uFlag);
  end;
end;

procedure TVideoMediaProcessor.SetFlipFlag(uFlag: UINT);
begin
  FFlipFlag := uFlag;
  if Assigned(FVideoEffectStream) then
  begin
    FVideoEffectStream.SetFlipFlag(uFlag);
  end;
end;

procedure TVideoMediaProcessor.SetFlipFlagProc(Value: UINT);
begin
  FFlipFlag := Value;
  if Assigned(FVideoEffectStream) then
  begin
    FVideoEffectStream.SetFlipFlag(Value);
  end;
end;

procedure TVideoMediaProcessor.SetResizeParam(pResizeParam: PRESIZE_PARAM);
begin
  if Assigned(FVideoEffectStream) then
  begin
    FVideoEffectStream.SetResizeParam(pResizeParam);
  end;  
end;

procedure TVideoMediaProcessor.SetRotation(nAngle: UINT; nDrag: Integer);
begin
  if Assigned(FVideoEffectStream) then
    FVideoEffectStream.SetRotation(nAngle, nDrag, 0);
end;

procedure TVideoMediaProcessor.SetSaturation(nValue: Integer);
begin
  if Assigned(FVideoEffectStream) then
    FVideoEffectStream.SetSaturation(nValue);
end;

function TVideoMediaProcessor.SetSubPicEx(const pUserSubPicEx: TUserSubPicEx;
  nOpacity: Integer): Boolean;
begin

end;

procedure TVideoMediaProcessor.SetUserSubPicture(pUserSubPic: PUserSubPicRec;
  nOpacity: Integer);
begin

end;

procedure TVideoMediaProcessor.SetUserWatermarkImage(hDibImage: HDIBIMAGE;
  AFRect: PFRECT; nOpacity: Integer);
var
  pVideoCompositor: IVideoCompositor;
  nIndex: Integer;
begin
  if FWatermarkDibImage <> hDibImage then
  begin
    if Assigned(FWatermarkDibImage) then
      FWatermarkDibImage._Release;
    FWatermarkDibImage := hDibImage;
    FWatermarkDibImage._AddRef;
    pVideoCompositor := FVideoEffectStream.FVideoCompositor as IVideoCompositor;
    if Assigned(FSubPicItem) then
    begin
      pVideoCompositor.Remove(FSubPicItem, nIndex);
      FSubPicItem._Release;
      FSubPicItem := nil;
    end;
    FSubPicItem := IVideoCompositonItem(pVideoCompositor.AddByBitmap(FWatermarkDibImage));
  end;
end;

procedure TVideoMediaProcessor.SetUserWatermarkImage(lpbi: PBITMAPINFO;
  lpBits: PBYTE; AFRect: PFRECT; nOpacity: Integer);
begin

end;

procedure TVideoMediaProcessor.SetVideoSettingRec(
  const Value: TVideoSettingRec);
begin

end;

procedure TVideoMediaProcessor.SetWatermarkImage(lpbi: PBITMAPINFO;
  lpBits: PBYTE; const pRect: PRECT; nOpacity: Integer);
begin
end;

procedure TVideoMediaProcessor.SetWatermarkImageAlign(lpbi: PBITMAPINFO;
  lpBits: PBYTE; nPos, nOpacity: Integer);
begin

end;

procedure TVideoMediaProcessor.SetWatermarkSrcID(hModule: Cardinal;
  const pID: PWideChar; const pRect: PRECT; nOpacity: Integer);
begin

end;

procedure TVideoMediaProcessor.SetWatermarkSrcIDAlign(hModule: HMODULE;
  pID: PWideChar; nPos, nOpacity: Integer);
begin

end;

{ TAudioMediaProcessor }

constructor TAudioMediaProcessor.Create(const hSrcAudio: IAudioDecoder;
  nSampleRate, nChannels, nBitsPerSample: Integer; bEvenFlag: BOOL);
begin
end;

 constructor TAudioMediaProcessor.Create(AAudieoDecoder: HAUDIODECODER);
begin
  FHandle := AAudieoDecoder;
  FAudioEffectStream := TAudioEffectStream.Create;
end;

destructor TAudioMediaProcessor.Destroy;
begin
  if Assigned(FAudioEffectStream) then
    FAudioEffectStream.Free;
  inherited;
end;

function TAudioMediaProcessor.GetAudioDecoder: HAUDIODECODER;
var
  pAudioDecoder: HAUDIODECODER;
begin
  FAudioEffectStream.GetStream(FHandle, Result);
end;

function TAudioMediaProcessor.GetAudioInfo: PWSAudioInfo;
begin

end;

procedure TAudioMediaProcessor.SetChannel(nChannel: Integer);
begin
  if Assigned(FAudioEffectStream) then
    FAudioEffectStream.SetChannels(nChannel);
end;

procedure TAudioMediaProcessor.SetSamplerate(nSamplerate: Integer);
begin
  if Assigned(FAudioEffectStream) then
    FAudioEffectStream.SetSampleRate(nSamplerate);
end;

procedure TAudioMediaProcessor.SetVolum(nValue: Integer);
begin
  if Assigned(FAudioEffectStream) then
    FAudioEffectStream.SetVolume(nValue);
end;

end.
