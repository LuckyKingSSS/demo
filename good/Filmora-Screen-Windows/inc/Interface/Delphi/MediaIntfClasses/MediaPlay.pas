unit MediaPlay;

interface
uses
  Windows, Messages, Classes, SysUtils, MediaCommon, MediaDecode, MediaProcess, WideFileUtils;
const
  CLSID_CMediaPlayer: TGUID = '{6487969c-8280-4371-b037-6fdd91321a3e}';
  IID_IMediaPlayer: TGUID = '{05A20FDC-2032-4c70-9134-AE340D8FC36F}';
  SOURCE_VIEW_MODE_ID = 0;            // 源视图
  OUTPUT_VIEW_MODE_ID = 1;            // 目标视图
  WM_PLAYERMSG = WM_USER + 2001;      //播放器消息 (不建议再使用，用事件代替)
  UM_CROP_CHANGE = (WM_USER + 4326);  // Crop区域更改消息 (不建议再使用，用事件代替)
  //消息参数wParam
  PM_Progress = 1; //播放中，可用VPGetPlayedTime取得播放时间以计算进度
  PM_Finish = 2;  //播放结束
  WM_PLAYERDESTROY = WM_USER + 2003;
  sDVDVIDEOTS = 'VIDEO_TS';
type
  IDVPVIEW = UINT;
  HEXPORTVIEWER = type Pointer;
  IPlayerView = interface(IUnknown)
    ['{8E7CE0D9-BF91-4e4f-AF9A-E25C6D9A2C12}']
    function SetVideoInput(bSource: Integer; MediaStream: IMediaStream): HRESULT; stdcall;
    function SetParentWND(parent: HVIEW): HRESULT; stdcall;
    function GetParentWND(): HVIEW; stdcall;
    function SetRect(): HRESULT; stdcall;
    function GetRect(): TRect; stdcall;
    function GetIndex(): Integer; stdcall;
    function GetWND(): HWND; stdcall;
  end;

  IPlayerViewMgr = interface(IUnknown)
    ['{08E9EED2-63E7-4895-9C46-8577AD7745CC}']
    function CreatePlayView(parentWND: HVIEW; const rect: TRECT): Pointer; stdcall;
    function GetCount(): Integer; stdcall;
    function Remove(nIndex: Integer): HRESULT; stdcall;
    function GetItem(nIndex: Integer; var pPlayerView: IPlayerView): HRESULT; stdcall;
  end;
  
  IMediaPlay = interface(IUnknown)
    ['{05A20FDC-2032-4c70-9134-AE340D8FC36F}']
    function GetMovieLength(): Double; stdcall;
    function SetPlayTime(dBegin: Double; dLength: Double): HRESULT; stdcall;
    function SetCallBack(pCallBack: TMULTI_CALLBACK; pUserObject: Pointer): HRESULT; stdcall;
    function GetPlayerViewMgr(var pIViewMgr: IPlayerViewMgr): HRESULT; stdcall;
    function IsActivate(): Integer; stdcall;
    function Stop(): HRESULT; stdcall;
    function Play(): HRESULT; stdcall;
    function Pause(): HRESULT; stdcall;
    function Resume(): HRESULT; stdcall;
    function Seek(dTime: Double; accurate: Integer): HRESULT; stdcall;
    function SyncSeek(dTime: Double; accurate: Integer): HRESULT; stdcall;
    function Forward(): HRESULT; stdcall;
    function SyncForward(): HRESULT; stdcall;
    function Update(bForce: BOOL): HRESULT; stdcall;
    function CaptureCurrentImage(const pImageInfo: PCaptureImageInfo; const pszFile: WideString): HRESULT; stdcall;
  end;

  TMediaPlayer = class;
  TPlayerStatus = (psNone, psHaveFile, psPlaying, psStop, psPause);
  TPlayerStatusChangeEvent = procedure (Sender: TObject; Status: TPlayerStatus) of object;
  TPlayerProgressEvent = procedure (Sender: TMediaPlayer; APlayedTime, ATotalTime: Double) of object;

  TMediaPlayerViewList = class
  private
    FPlayer: TMediaPlayer;
    FList: TList;
    function GetCount: Integer;
    function GetViewIds(Index: Integer): IDVPVIEW;
    procedure NotifyDestroy;
  public
    constructor Create(APlayer: TMediaPlayer);
    destructor Destroy; override;
    function Add(ID: IDVPVIEW): Integer;
    function Remove(ID: IDVPVIEW): Integer;

    property Count: Integer read GetCount;
    property ViewIds[Index: Integer]: IDVPVIEW read GetViewIds;
  end;

  PVideoProcessRecord = ^TVideoProcessRecord;
  TVideoProcessRecord = record
    nViewID: Integer;
    nViewMode: Integer;
    pVideoMediaProcessor: TVideoMediaProcessor;
    ResizeParam: PRESIZE_PARAM;
  end;
  
  HPLAYER = IMediaPlay;
  TMediaPlayer = class
  private
    FPlayer: HPLAYER;
    FWndCallBack: HWnd;
    FVideoDecoder: IVideoDecoder;
    FAudioDecoder: IAudioDecoder;
    FOnStatusChange: TPlayerStatusChangeEvent;
    FOnPlayerProgress: TPlayerProgressEvent;
    FOnCropAreaChange: TNotifyEvent;
    FPlayerStatus: TPlayerStatus;
    FDefViewId: IDVPVIEW;
    FPlayingFileName: UnicodeString;
    FPlayBegin, FPlayLength: Double;
    FObjectInst: Pointer;
    FPrevWndProc: Pointer;
    FSaveTopWnd: HWnd;
    FHookedCallbackWnd: HWnd;
    FSaveTopWndStyle: Cardinal;
    FDecoderFromFile: Boolean;
    FAudioInfo: TWSAudioInfo;
    FVideoInfo: TWSVideoInfo;
    FCurMsg: UINT;
    FCurWParam: WPARAM;
    FCurLParam: LPARAM;
    FResizeParam: RESIZE_PARAM;
    FVideoProcessList: TList;
    FAudioProcess: TAudioMediaProcessor;
    FSeekWaitting: Boolean;
    function CreateViewBase(hWndParent: HVIEW; const ARect: TRECT; nViewMode : Integer = SOURCE_VIEW_MODE_ID): IDVPVIEW;
    procedure InitVideoInfo;
    procedure InitAudioInfo;
    procedure StopNoStatus();
    procedure DoStatusChange;
    procedure DoCropAreaChange;
    function GetPlayerWnd(id: IDVPVIEW): HWND;
    procedure SetPlayerStatus(Value: TPlayerStatus);
    function GetPlayingSegment: Boolean;
    procedure SynPlayCallback;
    function DoPlayCallback(uMsg: UINT; wParam: WPARAM; lParam: LPARAM): HRESULT;
    function GetPlayedTime(): Double;
    function GetDefVideoProcessor: TVideoMediaProcessor;
    function GetAudioProcessor: TAudioMediaProcessor;
  public
    constructor Create(); overload;
    constructor Create(hWndParent: HWND; rcPos: TRect); overload;
    constructor Create(hWndParent, hWndCallBack: HWND; rcPos: TRect); overload;
    destructor Destroy; override;

    function OpenFile(const AFileName: UnicodeString; ADecModel: DEC_MODEL=AUTO_MODEL;
      AVidParam: PVideoDecParam=nil; AAudParam: PAudioDecParam=nil): Boolean;

    // 新增的播放DVD文件的方法
    function OpenDVDISO(const AISOFileName: UnicodeString; AVTSId, ATitleId,
       ASubTitleId, AAudioTrackId: Integer;
       ADecModel: DEC_MODEL=AUTO_MODEL): Boolean; // 播放VTS标题集（IFO）文件
    function OpenDVDFolder(const ADVDFolder: UnicodeString; AVTSId, ATitleId,
       ASubTitleId, AAudioTrackId: Integer;
       ADecModel: DEC_MODEL=AUTO_MODEL; AUseBup: Boolean=False): Boolean; // 插放DVD文件夹
    function OpenDVDVTS(const AVTSFileName: UnicodeString; ATitleId,
       ASubTitleId, AAudioTrackId: Integer;
       ADecModel: DEC_MODEL=AUTO_MODEL): Boolean; // 播放VTS标题集（IFO）文件

    procedure CloseFile();
    function SetDecoder(const AVideo: IVideoDecoder; const AAudio: IAudioDecoder): Boolean; virtual;
    procedure SetPlayTime(dBegin: Double; dLength: Double);
    procedure SetCallBack(hCBWnd: HWND);
    procedure Play();
    procedure Stop();
    procedure Pause();
    procedure Resume();
    procedure Forward();
    procedure SyncForward();
    procedure Seek(dTime: Double; accurate: BOOL = TRUE);
    procedure SeekWait(dwMilliseconds: DWORD);  // 等待Seek完成
    procedure SyncSeek(dTime: double; accurate: BOOL = TRUE);
    procedure Update();
    function GetVideoProcessHandle(id: IDVPVIEW): HVIDEODECODER;
    function GetVideoProcessor(id: IDVPVIEW): TVideoMediaProcessor;

    function GetMovieLength(): double;
    function CreateView(hWndParent: HVIEW; const ARect: TRECT; nViewMode : Integer = SOURCE_VIEW_MODE_ID): IDVPVIEW;
    function CreateViewEx(hWndParent: HVIEW; const pRect: TRECT;
      pResizeParam: PRESIZE_PARAM = nil; nViewMode : Integer = SOURCE_VIEW_MODE_ID): IDVPVIEW;
    procedure DestroyView(id: IDVPVIEW);
    function SetVolume(nValue: Integer): Integer;
    function GetDisplayResizeMode(id: IDVPVIEW): IMAGE_RESIZE_METHOD;
    function SetDisplayResizeMode(id: IDVPVIEW; irm: IMAGE_RESIZE_METHOD): BOOL;
    procedure SetCropEditStyle(id: IDVPVIEW; uCropStyle: UINT = 1);
    procedure SetCropMaskMode(id: IDVPVIEW; bMask: BOOL = True);
    procedure SetCropRect(id: IDVPVIEW; const pCropRect: TRECT);
    procedure GetCropRect(id: IDVPVIEW; var pCropRect: TRECT); overload;
    function GetCropRect(id: IDVPVIEW): TRect; overload;
    function IsActivate(): Integer;
    function SendCommand(uMsg: UINT; wParam:WPARAM; lParam:LPARAM): DWORD;
    function SetResizeParam(const pResizeParam: RESIZE_PARAM): Integer;  // 2009.09.09新增 V3.3

    procedure SetFullScreen(id: IDVPVIEW; AFullScreen: Boolean);
    procedure SetDefViewFullScreen(AFullScreen: Boolean);

    property Handle: HPLAYER read FPlayer;
    property PlayerWnd[id: IDVPVIEW]: HWnd read GetPlayerWnd;
    property DefViewId: IDVPVIEW read FDefViewId;
    property CallBackWnd: HWnd read FWndCallBack write SetCallBack;
    property MovieLength: Double read GetMovieLength;
    property PlayedTime: Double read GetPlayedTime;
    property PlayingLength: Double read FPlayLength;
    property PlayerStatus: TPlayerStatus read FPlayerStatus write SetPlayerStatus;
    property OnStatusChange: TPlayerStatusChangeEvent read FOnStatusChange write FOnStatusChange;
    property OnPlayerProgress: TPlayerProgressEvent read FOnPlayerProgress write FOnPlayerProgress;
    property OnCropAreaChange: TNotifyEvent read FOnCropAreaChange write FOnCropAreaChange;
    property VideoDecoder: IVideoDecoder read FVideoDecoder;
    property AudioDecoder: IAudioDecoder read FAudioDecoder;
    property PlayingFileName: UnicodeString read FPlayingFileName;
    property PlayingSegment: Boolean read GetPlayingSegment;
    property VideoInfo: TWSVideoInfo read FVideoInfo;
    property AudioInfo: TWSAudioInfo read FAudioInfo;
    property VideoProcessor: TVideoMediaProcessor read GetDefVideoProcessor;
    property AudioProcessor: TAudioMediaProcessor read GetAudioProcessor;
  end;

  TMediaPlayerAPI = record
  end;

  TMediaPreview = class
  private
    FParentWnd: HWND;
    FRect: TRect;
  public
    constructor Create(var APreviewAPI: TMediaPlayerAPI;
      const ADllName: string; hWndParent: HWnd;const rcPos: TRect); overload;
    constructor Create(hWndParent: HWnd;const rcPos: TRect); overload;
    destructor Destroy; override;

    function ShowDib(dib_header: PBITMAPINFO; const data: Pointer): Boolean;
    function ShowImage(image: PBITMAPINFO): Boolean;
    function ShowYUVImage(const pImage: TYUVImage): Boolean;
    function GetHwnd(): HWnd;
    function Clear: Boolean;
  end;

  TMediaProcessPlayer = TMediaPlayer;
  procedure ShowWinNoAnimate(Handle: THandle; CmdShow: Integer);

implementation

function GetAnimation: Boolean;
var
  Info: TAnimationInfo;
begin
  Info.cbSize := SizeOf(Info);
  if SystemParametersInfo(SPI_GETANIMATION, Info.cbSize, {$IFNDEF CLR}@{$ENDIF}Info, 0) then
    Result := Info.iMinAnimate <> 0
  else
    Result := False;
end;

procedure SetAnimation(Value: Boolean);
var
  Info: TAnimationInfo;
begin
  Info.cbSize := SizeOf(Info);
  Info.iMinAnimate := Integer(Value);
  SystemParametersInfo(SPI_SETANIMATION, Info.cbSize, {$IFNDEF CLR}@{$ENDIF}Info, 0);
end;

procedure ShowWinNoAnimate(Handle: THandle; CmdShow: Integer);
var
  Animation: Boolean;
begin
  Animation := GetAnimation;
  if Animation then
    SetAnimation(False);
  ShowWindow(Handle, CmdShow);
  if Animation then
    SetAnimation(True);
end;

function MediaPlayerCallback(pUserObj: Pointer; uMsg: UINT;
  wParam: WPARAM; lParam: LPARAM): HRESULT; stdcall;
var
  pPlayer: TMediaPlayer;
begin
  Result := S_OK;
  if Assigned(pUserObj) then
  begin
    pPlayer := TMediaPlayer(pUserObj);
    Result := pPlayer.DoPlayCallback(uMsg, wParam, lParam);
  end;
end;
{ TMediaPlayer }

procedure TMediaPlayer.CloseFile;
begin

end;

constructor TMediaPlayer.Create;
begin
  inherited Create;
  WSCoCreateInstance(CLSID_CMediaPlayer, nil, 0, IID_IMediaPlayer, FPlayer);
  FSeekWaitting := False;
  FVideoProcessList := TList.Create;
end;

constructor TMediaPlayer.Create(hWndParent: HWND; rcPos: TRect);
begin
  Create;
  if Assigned(FPlayer) then
    FDefViewId := CreateView(hWndParent, rcPos);
end;

constructor TMediaPlayer.Create(hWndParent, hWndCallBack: HWND; rcPos: TRect);
begin
  Create(hWndParent, rcPos);
end;

function TMediaPlayer.CreateView(hWndParent: HVIEW; const ARect: TRECT;
  nViewMode: Integer): IDVPVIEW;
var
  pVideoProcessRec: PVideoProcessRecord;
  nPlayerViewID: Integer;
begin
  nPlayerViewID := CreateViewBase(hWndParent, ARect, nViewMode);
  if FDefViewId = 0 then
    FDefViewId := nPlayerViewID;
  pVideoProcessRec := GetMemory(SizeOf(TVideoProcessRecord));
  pVideoProcessRec.nViewID := nPlayerViewID;
  pVideoProcessRec.pVideoMediaProcessor := nil;
  pVideoProcessRec.ResizeParam := nil;
  pVideoProcessRec.nViewMode := nViewMode;
  FVideoProcessList.Add(pVideoProcessRec);
end;

function TMediaPlayer.CreateViewBase(hWndParent: HVIEW; const ARect: TRECT;
  nViewMode: Integer): IDVPVIEW;
var
  pPlayerViewMgr: IPlayerViewMgr;
  pPlayerView: IPlayerView;
  nPlayerViewID: Integer;
  pVideoProcessRec: PVideoProcessRecord;
begin
  if Assigned(FPlayer) then
  begin
    FPlayer.GetPlayerViewMgr(pPlayerViewMgr);
    if Assigned(pPlayerViewMgr) then
    begin
      pPlayerView := IPlayerView(pPlayerViewMgr.CreatePlayView(hWndParent, ARect));
      if Assigned(pPlayerView) then
      begin
        nPlayerViewID := pPlayerView.GetIndex;
        pPlayerView._Release;
      end;
      pPlayerViewMgr._Release;
    end;
  end;
  if FDefViewId = 0 then
    FDefViewId := nPlayerViewID;
end;

function TMediaPlayer.CreateViewEx(hWndParent: HVIEW; const pRect: TRECT;
  pResizeParam: PRESIZE_PARAM; nViewMode: Integer): IDVPVIEW;
var
  pVideoProcessRec: PVideoProcessRecord;
  nPlayerViewID: Integer;
begin
  nPlayerViewID := CreateViewBase(hWndParent, pRect, nViewMode);
  if FDefViewId = 0 then
    FDefViewId := nPlayerViewID;
  pVideoProcessRec := GetMemory(SizeOf(TVideoProcessRecord));
  pVideoProcessRec.nViewID := nPlayerViewID;
  pVideoProcessRec.nViewMode := nViewMode;
  pVideoProcessRec.pVideoMediaProcessor := nil;
  pVideoProcessRec.ResizeParam := GetMemory(SizeOf(TRESIZE_PARAM));
  CopyMemory(pVideoProcessRec.ResizeParam, pResizeParam, SizeOf(TRESIZE_PARAM));
  FVideoProcessList.Add(pVideoProcessRec);
end;

destructor TMediaPlayer.Destroy;
begin
  if Assigned(FPlayer) then
  begin
    FPlayer._Release;
  end;
  inherited;
end;

procedure TMediaPlayer.DestroyView(id: IDVPVIEW);
begin

end;

procedure TMediaPlayer.DoCropAreaChange;
begin

end;

function TMediaPlayer.DoPlayCallback(uMsg: UINT; wParam: WPARAM;
  lParam: LPARAM): HRESULT;
begin
  Result := S_OK;
  if FSeekWaitting then
    Exit;
  FCurMsg := uMsg;
  FCurWParam := wParam;
  FCurLParam := lParam;
  TThread.Synchronize(nil, SynPlayCallback);
end;

procedure TMediaPlayer.DoStatusChange;
begin
  if Assigned(FOnStatusChange) then FOnStatusChange(Self, FPlayerStatus);
end;

procedure TMediaPlayer.Forward;
begin
  if Assigned(FPlayer) then
    FPlayer.Forward;
end;

procedure TMediaPlayer.GetCropRect(id: IDVPVIEW; var pCropRect: TRECT);
begin

end;

function TMediaPlayer.GetAudioProcessor: TAudioMediaProcessor;
begin
  Result := FAudioProcess;
end;

function TMediaPlayer.GetCropRect(id: IDVPVIEW): TRect;
begin

end;

function TMediaPlayer.GetDefVideoProcessor: TVideoMediaProcessor;
begin
  Result := nil;
  if FVideoProcessList.Count > 0 then
    Result := TVideoMediaProcessor(FVideoProcessList[0]);
end;

function TMediaPlayer.GetDisplayResizeMode(id: IDVPVIEW): IMAGE_RESIZE_METHOD;
begin

end;

function TMediaPlayer.GetMovieLength: double;
begin
  Result := 0;
  if Assigned(FPlayer) then
    Result := FPlayer.GetMovieLength;
end;


function TMediaPlayer.GetPlayedTime: Double;
begin

end;

function TMediaPlayer.GetPlayerWnd(id: IDVPVIEW): HWND;
var
  I: Integer;
  pPlayerViewMgr: IPlayerViewMgr;
  pPlayerView: IPlayerView;
begin
  Result := HWND(-1);
  if Assigned(FPlayer) then
  begin
    FPlayer.GetPlayerViewMgr(pPlayerViewMgr);
    if Assigned(pPlayerViewMgr) then
    begin
      pPlayerViewMgr.GetItem(id, pPlayerView);
      if Assigned(pPlayerView) then
        Result := pPlayerView.GetWND;
    end;  
  end;
end;

function TMediaPlayer.GetPlayingSegment: Boolean;
begin

end;

function TMediaPlayer.GetVideoProcessHandle(id: IDVPVIEW): HVIDEODECODER;
begin

end;

function TMediaPlayer.GetVideoProcessor(id: IDVPVIEW): TVideoMediaProcessor;
begin

end;

procedure TMediaPlayer.InitAudioInfo;
begin
  FillChar(FAudioInfo, SizeOf(FAudioInfo), 0);
end;

procedure TMediaPlayer.InitVideoInfo;
begin
  FillChar(FVideoInfo, SizeOf(FVideoInfo), 0);
end;

function TMediaPlayer.IsActivate: Integer;
begin

end;

function TMediaPlayer.OpenDVDFolder(const ADVDFolder: UnicodeString; AVTSId,
  ATitleId, ASubTitleId, AAudioTrackId: Integer; ADecModel: DEC_MODEL;
  AUseBup: Boolean): Boolean;
var
  szDir, szVTSFileName, szExt: UnicodeString;
begin
  if WideExtractFileName(ADVDFolder) <> sDVDVIDEOTS then
  begin
    szDir:=WideIncludeTrailingBackslash(ADVDFolder) + sDVDVIDEOTS;
    if not WideDirectoryExists(szDir) then
      szDir:=ADVDFolder;
  end else
    szDir:=ADVDFolder;

  if AUseBup then szExt:='BUP' else szExt:='IFO';
  if AVTSId=0 then
    szVTSFileName:=WideIncludeTrailingBackslash(szDir)+format('VIDEO_TS.%s', [szExt])
  else
    szVTSFileName:=WideIncludeTrailingBackslash(szDir)+format('VTS_%.2d_0.%s', [AVTSId, szExt]);
  Result:=OpenDVDVTS(szVTSFileName, ATitleId, ASubTitleId, AAudioTrackId, ADecModel);
end;

function TMediaPlayer.OpenDVDISO(const AISOFileName: UnicodeString; AVTSId,
  ATitleId, ASubTitleId, AAudioTrackId: Integer; ADecModel: DEC_MODEL): Boolean;
begin
  Result:=OpenDVDVTS(AISOFileName+format(':%d', [AVTSId]), ATitleId,
    ASubTitleId, AAudioTrackId, ADecModel);
end;

function TMediaPlayer.OpenDVDVTS(const AVTSFileName: UnicodeString; ATitleId,
  ASubTitleId, AAudioTrackId: Integer; ADecModel: DEC_MODEL): Boolean;
var
  VDecParam: TVideoDecParam;
  ADecParam: TAudioDecParam;
begin
  FillChar(VDecParam, SizeOf(VDecParam), 0);
  FillChar(ADecParam, SizeOf(ADecParam), 0);

  VDecParam.uProgramID:=ATitleId;
  VDecParam.nSubTrack:=ASubTitleId;

  ADecParam.uProgramID:=ATitleId;
  ADecParam.nSubTrack:=VDecParam.nSubTrack;
  ADecParam.nTrack:=AAudioTrackId;

  Result:=OpenFile(AVTSFileName, ADecModel, @VDecParam, @ADecParam);
end;

function TMediaPlayer.OpenFile(const AFileName: UnicodeString;
  ADecModel: DEC_MODEL; AVidParam: PVideoDecParam;
  AAudParam: PAudioDecParam): Boolean;
var
  pMediaDecoder: TMediaDecoder;
begin
  try
    pMediaDecoder := TMediaDecoder.Create(AFileName, AVidParam^, AAudParam^, ADecModel);
  except
    pMediaDecoder := nil;
    if Assigned(Classes.ApplicationHandleException) then
      Classes.ApplicationHandleException(Self);
  end;
  if Assigned(pMediaDecoder) and (pMediaDecoder.Handle=nil) then
    FreeAndNil(pMediaDecoder);
  if Assigned(pMediaDecoder) then
  begin
    FPlayingFileName:=AFileName;
    FDecoderFromFile:=True;
    Result := SetDecoder(pMediaDecoder, pMediaDecoder);
  end;
end;

procedure TMediaPlayer.Pause;
begin
  if Assigned(FPlayer) then
  begin
    FPlayer.Pause;
  end;
end;

procedure TMediaPlayer.Play;
begin
  if Assigned(FPlayer) then
  begin
    FPlayer.Play;
  end;
end;

procedure TMediaPlayer.Resume;
begin
  if Assigned(FPlayer) then
  begin
    FPlayer.Resume;
  end;
end;

procedure TMediaPlayer.Seek(dTime: Double; accurate: BOOL);
begin
  if Assigned(FPlayer) then
  begin
    FPlayer.Seek(dTime, Integer(accurate));
  end;
end;

procedure TMediaPlayer.SeekWait(dwMilliseconds: DWORD);
var
  st: Cardinal;
begin
  st:=GetTickCount;
  repeat
    FSeekWaitting := True;
  until GetTickCount-st>dwMilliseconds;
  FSeekWaitting := False;
end;

function TMediaPlayer.SendCommand(uMsg: UINT; wParam: WPARAM;
  lParam: LPARAM): DWORD;
begin

end;

procedure TMediaPlayer.SetCallBack(hCBWnd: HWND);
begin

end;

procedure TMediaPlayer.SetCropEditStyle(id: IDVPVIEW; uCropStyle: UINT);
begin

end;

procedure TMediaPlayer.SetCropMaskMode(id: IDVPVIEW; bMask: BOOL);
begin

end;

procedure TMediaPlayer.SetCropRect(id: IDVPVIEW; const pCropRect: TRECT);
begin

end;

function TMediaPlayer.SetDecoder(const AVideo: IVideoDecoder;
  const AAudio: IAudioDecoder): Boolean;
var
  hVideo: HVIDEODECODER;
  hAudio: HAUDIODECODER;
  hVideoProcess: HVIDEODECODER;
  bSavePlaying: Boolean;
  pAudioInfo: PWSAudioInfo;
  pVideoInfo: PWSVideoInfo;
  I, J: Integer;
  pPlayerViewMgr: IPlayerViewMgr;
  pPlayerView: IPlayerView;
  pVidProcessRec: PVideoProcessRecord;
begin
  Result:=False;
  if FPlayer=nil then Exit;
  bSavePlaying:=PlayerStatus=psPlaying;
  FPlayBegin:=0;
  FPlayLength:=0;
  hVideo := nil;
  hAudio := nil;  
  if Assigned(AVideo) then
    hVideo := AVideo.VideoDecoder;
  if Assigned(AAudio) then
    hAudio := AAudio.AudioDecoder;
  if Assigned(FPlayer) then
  begin
    if PlayerStatus in [psPlaying, psPause] then
    begin
      StopNoStatus;
    end;
    pPlayerViewMgr := nil;
    FPlayer.GetPlayerViewMgr(pPlayerViewMgr);
    if Assigned(pPlayerViewMgr) then
    begin
      for I := 0 to pPlayerViewMgr.GetCount - 1 do
      begin
        pPlayerViewMgr.GetItem(I, pPlayerView);
        pVidProcessRec := nil;
        for J := 0 to FVideoProcessList.Count - 1 do
        begin
          pVidProcessRec := PVideoProcessRecord(FVideoProcessList[J]);
          if pVidProcessRec.nViewID = pPlayerView.GetIndex then
          begin
            if Assigned(pVidProcessRec.pVideoMediaProcessor) then
              FreeAndNil(pVidProcessRec.pVideoMediaProcessor);
            pVidProcessRec.pVideoMediaProcessor := TVideoMediaProcessor.Create(hVideo);
            with pVidProcessRec.pVideoMediaProcessor.VideoEffectStream do
            begin
              EnableVideoCopy;
              if (pVidProcessRec.nViewMode <> OUTPUT_VIEW_MODE_ID) or (FVideoProcessList.Count = 1) then
              begin
                EnableVideoSubPicCompositor;
                EnableVideoCrop;
                EnableVideoRotation;
                EnableVideoResizer;
                EnableVideoEffect;
                EnableVideoCompositor;
                EnablePCText;
              end;
            end;
            Break;          
          end
          else
            pVidProcessRec := nil;
        end;
        if Assigned(pVidProcessRec) then
        begin
          hVideoProcess := pVidProcessRec.pVideoMediaProcessor.Handle;
          if pVidProcessRec.nViewMode = OUTPUT_VIEW_MODE_ID then
            pPlayerView.SetVideoInput(0, hVideoProcess)
          else
            pPlayerView.SetVideoInput(1, hVideoProcess);
          if Assigned(hVideoProcess) then
            hVideoProcess._Release;
        end;
      end;
    end;
  end;
  if Assigned(hVideo) then
    hVideo._Release;
  if Assigned(hAudio) then
    hAudio._Release;
end;

procedure TMediaPlayer.SetDefViewFullScreen(AFullScreen: Boolean);
begin

end;

function TMediaPlayer.SetDisplayResizeMode(id: IDVPVIEW;
  irm: IMAGE_RESIZE_METHOD): BOOL;
begin

end;

procedure TMediaPlayer.SetFullScreen(id: IDVPVIEW; AFullScreen: Boolean);
begin

end;

procedure TMediaPlayer.SetPlayerStatus(Value: TPlayerStatus);
begin
  if Value <> FPlayerStatus then
  begin
    FPlayerStatus := Value;
    DoStatusChange;
  end;
end;

procedure TMediaPlayer.SetPlayTime(dBegin, dLength: Double);
begin
  if Assigned(FPlayer) then
  begin
    if dLength > FPlayer.GetMovieLength then
      dLength := FPlayer.GetMovieLength;
    FPlayer.SetPlayTime(dBegin, dLength);
    FPlayLength := dLength;
  end;
end;

function TMediaPlayer.SetResizeParam(const pResizeParam: RESIZE_PARAM): Integer;
begin

end;

function TMediaPlayer.SetVolume(nValue: Integer): Integer;
begin

end;

procedure TMediaPlayer.Stop;
begin
  if Assigned(FPlayer) then
  begin
    FPlayer.Stop;
  end;
  PlayerStatus := psStop;
end;

procedure TMediaPlayer.StopNoStatus;
begin
  if Assigned(FPlayer) then
  begin
    FPlayer.Stop;
  end;
end;

procedure TMediaPlayer.SyncForward;
begin
  if Assigned(FPlayer) then
    FPlayer.SyncForward;
end;

procedure TMediaPlayer.SyncSeek(dTime: double; accurate: BOOL);
begin
  if Assigned(FPlayer) then
    FPlayer.SyncSeek(dTime, Integer(accurate));
end;

procedure TMediaPlayer.SynPlayCallback;
begin
  case FCurMsg of
    WM_PLAYERMSG:
    begin
      case FCurWParam of
        PM_Progress:
          if (FPlayerStatus=psPlaying) and Assigned(FOnPlayerProgress) then
          begin
            FOnPlayerProgress(Self, FCurLParam, PlayingLength);
          end;
        PM_Finish:
          begin
            Stop;
          end;
      end;
    end;
  end;
end;

procedure TMediaPlayer.Update;
begin
  if Assigned(FPlayer) then
    FPlayer.Update(TRUE);
end;

{ TMediaPlayerViewList }

function TMediaPlayerViewList.Add(ID: IDVPVIEW): Integer;
begin
  Result:=FList.Add(Pointer(ID));
end;

constructor TMediaPlayerViewList.Create(APlayer: TMediaPlayer);
begin
  inherited Create;
  FPlayer:=APlayer;
  FList:=TList.Create;
end;

destructor TMediaPlayerViewList.Destroy;
begin
  NotifyDestroy;
  FList.Free;
  inherited;
end;

function TMediaPlayerViewList.GetCount: Integer;
begin
  Result:=FList.Count;
end;

function TMediaPlayerViewList.GetViewIds(Index: Integer): IDVPVIEW;
begin
  Result:=IDVPVIEW(FList[Index]);
end;

procedure TMediaPlayerViewList.NotifyDestroy;
var
  i: Integer;
  id: IDVPVIEW;
begin
  for i:=Count - 1 downto 0 do
  begin
    id:=ViewIds[i];
    SendMessage(FPlayer.PlayerWnd[id], WM_PLAYERDESTROY, 0, 0);
  end;
end;

function TMediaPlayerViewList.Remove(ID: IDVPVIEW): Integer;
begin
  Result:=FList.Remove(Pointer(ID));
end;

{ TMediaPreview }

function TMediaPreview.Clear: Boolean;
begin

end;

constructor TMediaPreview.Create(var APreviewAPI: TMediaPlayerAPI;
  const ADllName: string; hWndParent: HWnd; const rcPos: TRect);
begin
  FRect := rcPos;
  FParentWnd := hWndParent;
end;

constructor TMediaPreview.Create(hWndParent: HWnd; const rcPos: TRect);
begin
  FRect := rcPos;
  FParentWnd := hWndParent;
end;

destructor TMediaPreview.Destroy;
begin

  inherited;
end;

function TMediaPreview.GetHwnd: HWnd;
begin

end;

function TMediaPreview.ShowDib(dib_header: PBITMAPINFO;
  const data: Pointer): Boolean;
begin

end;

function TMediaPreview.ShowImage(image: PBITMAPINFO): Boolean;
begin

end;

function TMediaPreview.ShowYUVImage(const pImage: TYUVImage): Boolean;
begin

end;

end.
