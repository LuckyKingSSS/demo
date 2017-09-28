{*******************************************************}
{       Wondershare多媒体任务管理单元                   }
{       创建人:　 凌曙光　　 　　　　　　　   　　　　  }
{       创建日期: 2008/03/20                            }
{       修订人:   凌曙光                                }
{       修订日期: 2008/07/10                            }
{*******************************************************}

unit MediaTask;

interface

uses Windows, SysUtils, Classes, Contnrs, FSClasses, MediaCommon, MediaDecode,
  MediaEncodeMgr, MediaEncode, MediaProcess, DVDInfo, WideStrings, MediaDibImage,
  WSTextArt, Graphics, WSSubPic;

type
  TMediaTask = class;
  TMediaTaskMgr = class;
  TMediaTaskList = class;

  TMediaTaskType = (mttVideo, mttAudio, mttDVDTitle, mttDVDChapter);
  TWatermarkType = (wtImage, wtText);

  TMediaTask = class
  private
    FOwner: TMediaTaskList;
    FId: Integer;
    FOrderId: Integer;
    FTitle: string;
    FFileName    : UnicodeString;
    FOutFileName : UnicodeString;
    FFormatExt: string;
    FMediaStart  : Double;
    FMediaLength : Double;
    FTitleIndex: Integer;
    FChapterIndex: Integer;
    FVideoInfo   : TWSVideoInfo;
    FAudioInfo   : TWSAudioInfo;
    FLoaded: Boolean;
    // 下面是编码参数
    FDecModel   : DEC_MODEL;
    FFormatId   : Integer;
    FResizeStyle: IMAGE_RESIZE_METHOD;
    FCropRect   : TRect;             //视频剪切区域
    FCutStart   : Double;            //剪切后的开始时间
    FCutEnd     : Double;            //剪切后的结束时间
    FEncParam    : TEncodeParam;     //编码参数

    FVDecParam  : TDecParam;         //视频解码参数
    FADecParam  : TDecParam;         //音频解码参数
    FHasAudioInfo: Boolean;
    FHasVideoInfo: Boolean;
    FAudioBitrate: Integer;
    FVideoBitrate: Integer;
    FBitrate: Integer;
    FContrast: Integer;
    FFilterId: UINT;
    FSaturation: Integer;
    FBrightness: Integer;
    FHorzFlip: Boolean;
    FVertFlip: Boolean;
    FVolume: Integer;
    FMute: Boolean;
    FDeInterlacing: DEINTERLACE_STYLE;
    FMediaEncoder: TMediaEncoder;
    FSkipEncode: Boolean;
    FMediaType: TMediaTaskType;
    FEncodingState: TMediaEncodingState;
    FProgress, FProgressMax: Integer;
    FProgressInfo: PMediaProgressInfo;
    FEncodeProgressResult: BOOL;
    FChapterCount: Integer;
    FSubTitles: TWideStrings;
    FAudioTracks: TWideStrings;
    FResizeStyleModified: Boolean;
    FWaterMarkImage: TDibImage;
    FWaterMark: Boolean;
    FWaterMarkPos: TPoint;
    FWaterMarkType: TWatermarkType;
    FWaterMarkSize: TSize;
    FWaterMarkTrans: Integer;
    FWaterMarkText: UnicodeString;
    FWaterMarkFile: UnicodeString;
    FWaterMarkFont: TFont;
    FSetVPUserWaterMarkProc: function (AVideoProcessor: TVideoMediaProcessor; AToEncode: Boolean=False): Boolean of object;
    FSubTitlePos: TPoint;
    FSubTitleTrans: Integer;
    FSubTitleUserStyle: Boolean;
    FSubTitle: Boolean;
    FSubTitleAngle: Integer;
    FSubTitleFont: TFont;
    FSubTitleFile: UnicodeString;
    FHasSubTitle: Boolean;
    FVideoSlideProcess: TVideoMediaProcessor;
    FAudioSlideProcess: TAudioMediaProcessor;
    FRotation: Double;
    FMediaDecoder: TMediaDecoder;
    FVideoClipCreated: Boolean;
    FAudioClipCreated: Boolean;
    procedure DoSyncEncodeProgress;
    procedure DoSyncEncodeFinish;
    procedure DoSyncFreeEncoder;
    function DoEncodeProgress(Sender: TMediaEncoder; EncodingState: TMediaEncodingState;
      AProgress, AProgressMax: Integer; AProgressInfo: PMediaProgressInfo): Boolean;
    function VideoSlideClipEvent(Sender: TSlideDecoderEx; nOptFlag: UINT): HVIDEODECODER;
    function AudioSlideClipEvent(Sender: TSlideDecoderEx; nOptFlag: UINT): HAUDIODECODER;
    function InitMediaEncoder(ASlide: Boolean): Boolean;
    function GetFullOutFileName: UnicodeString;
    function GetCutLength: Double;
    function GetFlipFlag: UINT;
    procedure SetFlipFlag(Value: UINT);
    procedure SetResizeStyle(Value: IMAGE_RESIZE_METHOD);
    function GetIsOutputAudio: Boolean;
    function LoadExtSubTitleList(pMediaInfo: IMediaInfo): Integer;
    function LoadFromMediaInfo(pMediaInfo: IMediaInfo; ANeedRelease: Boolean=True): Boolean;
    function GetADecParam: PDecParam;
    function GetVDecParam: PDecParam;
    procedure SetADecParam(Value: PDecParam);
    procedure SetVDecParam(Value: PDecParam);
    function GetAudioTrackIdx: Integer;
    function GetSubTitleIdx: Integer;
    procedure SetAudioTrackIdx(Value: Integer);
    procedure SetSubTitleIdx(Value: Integer);
    function InnerGetWaterMarkImage(AScale: Double): TDibImage;
    function GetWaterMarkImage: TDibImage;
    procedure SetWaterMarkImage(Value: TDibImage);
    procedure SetWaterMarkFont(const Value: TFont);
    procedure SetWaterMark(Value: Boolean);
    function GetIsLosslessCopy: Boolean;
    function GetSubTitleId: Integer;
    procedure SetSubTitleId(Value: Integer);
    function GetSubTitleIds(Index: Integer): Integer;
    procedure SetSubTitleFont(const Value: TFont);
    function GetMediaCantSeek: Boolean;
    function GetMediaUnknownLength: Boolean;
    function GetOutputVideoSize: TSize;
    function GetPreviewVideoSize: TSize;
    function GetOutputResizeParam: RESIZE_PARAM;
    function GetPreviewResizeParam: RESIZE_PARAM;
  public
    constructor Create(AOwner: TMediaTaskList; AId: Integer;
      const ATitle: string; const AFileName: UnicodeString; ADecModel: DEC_MODEL=AUTO_MODEL);
    destructor Destroy; override;

    function Load: Boolean;
    function LoadAV: Boolean;
    function LoadDVD(ADVDInfo: TDVDIFOInfo=nil): Boolean;
    procedure Remove;
    function CalcOutputFileSize(const AEncParam: TEncodeParam): UInt64; overload;
    function CalcOutputFileSize(): UInt64; overload;
    function InitDefaultEncoders: Boolean;
    function StartEncode(hMsgWnd: HWnd=0): Integer;
    function StopEncode: Boolean;
    function SetVPUserWaterMark(AVideoProcessor: TVideoMediaProcessor; AToEncode: Boolean=False): Boolean;
    function SetUserSubTitle(AVideoProcessor: TVideoMediaProcessor; AToEncode: Boolean=False): Boolean;
    function SetVideoProcessor(AVideoProcessor: TVideoMediaProcessor; AToEncode: Boolean=False): Boolean;
    function SetAudioProcessor(AAudioProcessor: TAudioMediaProcessor): Boolean;
    function SetVideoPreviewProcessor(AVideoProcessor: TVideoMediaProcessor): Boolean;

    function AddToSlideEncoder(ASlideEncoder: TMediaSlideEncoderEx;
      AStartTime: Double): Boolean; overload;
    // 任务属性
    property Owner: TMediaTaskList read FOwner;
    property Id: Integer read FId;
    property OrderId: Integer read FOrderId write FOrderId;  // 顺序ID，由用户使用
    property Title: string read FTitle write FTitle;
    property OutFileName: UnicodeString read FOutFileName write FOutFileName;
    property FullOutFileName: UnicodeString read GetFullOutFileName;
    property SkipEncode: Boolean read FSkipEncode write FSkipEncode;
    property MediaType: TMediaTaskType read FMediaType write FMediaType;
    // 媒体信息
    property FileName: UnicodeString read FFileName;
    property MediaStart: Double read FMediaStart;
    property MediaLength: Double read FMediaLength;
    property Bitrate: Integer read FBitrate;
    property VideoBitrate: Integer read FVideoBitrate;
    property AudioBitrate: Integer read FAudioBitrate;
    property VideoInfo: TWSVideoInfo read FVideoInfo;
    property AudioInfo: TWSAudioInfo read FAudioInfo;
    property HasVideoInfo: Boolean read FHasVideoInfo;
    property HasAudioInfo: Boolean read FHasAudioInfo;
    property MediaCantSeek: Boolean read GetMediaCantSeek;
    property MediaUnknownLength: Boolean read GetMediaUnknownLength;
    // DVD 媒体信息
    property TitleIndex: Integer read FTitleIndex write FTitleIndex;
    property ChapterIndex: Integer read FChapterIndex write FChapterIndex;
    property ChapterCount: Integer read FChapterCount;
    property AudioTracks: TWideStrings read FAudioTracks;
    property SubTitles: TWideStrings read FSubTitles;                  //  外挂字幕
    property SubTitleIds[Index: Integer]: Integer read GetSubTitleIds; // 从索引取得字幕Id
    // 解码参数
    property DecModel: DEC_MODEL read FDecModel write FDecModel;
    property VDecParam: PDecParam read GetVDecParam write SetVDecParam;
    property ADecParam: PDecParam read GetADecParam write SetADecParam;
    property AudioTrackIdx: Integer read GetAudioTrackIdx write SetAudioTrackIdx;
    property SubTitleIdx: Integer read GetSubTitleIdx write SetSubTitleIdx;
    property SubTitleId: Integer read GetSubTitleId write SetSubTitleId;

    // 编码参数
    property FormatId: Integer read FFormatId write FFormatId;
    property FormatExt: string read FFormatExt write FFormatExt;
    property EncParam: TEncodeParam read FEncParam write FEncParam;
    property IsOutputAudio: Boolean read GetIsOutputAudio;
    property IsLosslessCopy: Boolean read GetIsLosslessCopy;  // 是否DVD无损拷贝
    property OutputVideoSize: TSize read GetOutputVideoSize;          // 输出视频大小（从EncParam中取得）
    property PreviewVideoSize: TSize read GetPreviewVideoSize;        // 输出预览视频大小（保证预览效果,不缩小）
    property OutputResizeParam: RESIZE_PARAM read GetOutputResizeParam;           // 快速获取输出缩放参数
    property PreviewResizeParam: RESIZE_PARAM read GetPreviewResizeParam;           // 快速获取预览缩放参数

    // 编辑效果
    property ResizeStyleModified: Boolean read FResizeStyleModified write FResizeStyleModified;
    property ResizeStyle: IMAGE_RESIZE_METHOD read FResizeStyle write SetResizeStyle;
    property CropRect: TRect read FCropRect write FCropRect;
    property CutStart: Double read FCutStart write FCutStart;
    property CutEnd: Double read FCutEnd write FCutEnd;
    property CutLength: Double read GetCutLength;
    property Volume: Integer read FVolume write FVolume;
    property Mute: Boolean read FMute write FMute;
    property Brightness: Integer read FBrightness write FBrightness;
    property Contrast: Integer read FContrast write FContrast;
    property Saturation: Integer read FSaturation write FSaturation;
    property FilterID: UINT read FFilterId write FFilterID; //滤镜ID
    property FlipFlag: UINT read GetFlipFlag write SetFlipFlag;
    property HorzFlip: Boolean read FHorzFlip write FHorzFlip;
    property VertFlip: Boolean read FVertFlip write FVertFlip;
    property DeInterlacing: DEINTERLACE_STYLE read FDeInterlacing write FDeInterlacing;
    property Rotation: Double read FRotation write FRotation;
    // 水印
    property WaterMark: Boolean read FWaterMark write SetWaterMark; // 是否允许用户水印
    property WaterMarkImage: TDibImage read GetWaterMarkImage write SetWaterMarkImage;      // 用户水印图像，文字也转为图像
    property WaterMarkType: TWatermarkType read FWaterMarkType write FWaterMarkType; // 水印类型
    property WaterMarkFile: UnicodeString read FWaterMarkFile write FWaterMarkFile; // 水印图片文件
    property WaterMarkText: UnicodeString read FWaterMarkText write FWaterMarkText; // 水印文字
    property WaterMarkFont: TFont read FWaterMarkFont write SetWaterMarkFont;      // 水印字体
    property WaterMarkTrans: Integer read FWaterMarkTrans write FWaterMarkTrans; // 水印透明度
    property WaterMarkPos: TPoint read FWaterMarkPos write FWaterMarkPos;        // 水印位置
    property WaterMarkSize: TSize read FWaterMarkSize write FWaterMarkSize;       // 水印大小
    // 外挂字幕
    property HasSubTitle: Boolean read FHasSubTitle;                            //是否含有外挂字幕（一般是MKV文件）
    property SubTitle: Boolean read FSubTitle write FSubTitle;                  //是否允许用户外挂字幕
    property SubTitleUserStyle: Boolean read FSubTitleUserStyle write FSubTitleUserStyle;   //是否允许用户自定义外挂字幕样式
    property SubTitleFont: TFont read FSubTitleFont write SetSubTitleFont;      //字幕字体
    property SubTitleTrans: Integer read FSubTitleTrans write FSubTitleTrans;   //字幕透明度
    property SubTitleAngle: Integer read FSubTitleAngle write FSubTitleAngle;   //字幕旋转角度，未使用
    property SubTitlePos: TPoint read FSubTitlePos write FSubTitlePos;          //字幕位置
    property SubTitleFile: UnicodeString read FSubTitleFile write FSubTitleFile;// 外挂字幕文件 srt等

    property MediaEncoder: TMediaEncoder read FMediaEncoder;
  end;

  TMediaTaskListEnumerator = class
  private
    FIndex: Integer;
    FList: TMediaTaskList;
  public
    constructor Create(AList: TMediaTaskList);
    function GetCurrent: TMediaTask;
    function MoveNext: Boolean;
    property Current: TMediaTask read GetCurrent;
  end;

  TMediaListSortCompare = function(Item1, Item2: Pointer): Integer;

  TMediaTaskList = class
  private
    FOwner: TMediaTaskMgr;
    FList: TObjectList;
    function GetCount: Integer;
    function GetItems(Index: Integer): TMediaTask;
  public
    constructor Create(AOwner: TMediaTaskMgr);
    destructor Destroy; override;

    function Add(AId: Integer; const ATitle: string; const AFileName: UnicodeString; ADecModel: DEC_MODEL=AUTO_MODEL): TMediaTask; overload;
    function Add(const AFileName: UnicodeString; ADecModel: DEC_MODEL=AUTO_MODEL): TMediaTask; overload;
    function Add(const ATitle: string; const AFileName: UnicodeString; ADecModel: DEC_MODEL=AUTO_MODEL): TMediaTask; overload;
    function Add(Item: TMediaTask): Integer; overload;
    procedure Insert(Index: Integer; Item: TMediaTask);
    function Extract(Item: TMediaTask): TMediaTask;
    function IndexOf(Id: Integer): Integer;
    function Find(Id: Integer): TMediaTask; overload;
    function Find(const AFileName: UnicodeString): TMediaTask; overload;
    function Find(ASkipItem: TMediaTask; const AFileName: UnicodeString): TMediaTask; overload;
    function FindRepeat(ASkipItem: TMediaTask; const AFileName: UnicodeString): Integer; overload;
    function FindOutNoExt(ASkipItem: TMediaTask; const AFileName: UnicodeString): Integer; overload;
    function CheckRepeatAndRenameOutFile(ASkipItem: TMediaTask; const AFileName, AExt: UnicodeString): UnicodeString;
    procedure Delete(Index: Integer);
    procedure Clear;
    procedure Move(CurIndex, NewIndex: Integer);
    function Remove(Item: TMediaTask): Integer;
    function GetEnumerator: TMediaTaskListEnumerator;
    function CheckExists(const AFileName: UnicodeString): Boolean;

    procedure SortByOrderId;

    property Owner: TMediaTaskMgr read FOwner;
    property Count: Integer read GetCount;
    property Items[Index: Integer]: TMediaTask read GetItems; default;
  end;

  TCustomProfileSettings = class
  private
    FFormatId: Integer;
    FVideoWidth: Integer;
    FVideoFrameRate: Double;
    FAudioEncoder: DWORD;
    FVideoEncoder: DWORD;
    FAudioBitrate: Integer;
    FVideoBitrate: Integer;
    FVideoHeight: Integer;
    FAudioSampleRate: Integer;
    FAudioChannels: Integer;
  public
    procedure AssignTo(ATask: TMediaTask);
    procedure AssignFrom(ATask: TMediaTask); overload;
    procedure AssignFrom(Source: TCustomProfileSettings); overload;
    procedure LoadConfig;
    procedure SaveConfig;

    property FormatId: Integer read FFormatId write FFormatId;
    property VideoEncoder: DWORD read FVideoEncoder write FVideoEncoder;
    property VideoWidth: Integer read FVideoWidth write FVideoWidth;
    property VideoHeight: Integer read FVideoHeight write FVideoHeight;
    property VideoFrameRate: Double read FVideoFrameRate write FVideoFrameRate;
    property VideoBitrate: Integer read FVideoBitrate write FVideoBitrate;

    property AudioEncoder: DWORD read FAudioEncoder write FAudioEncoder;
    property AudioSampleRate: Integer read FAudioSampleRate write FAudioSampleRate;
    property AudioChannels: Integer read FAudioChannels write FAudioChannels;
    property AudioBitrate: Integer read FAudioBitrate write FAudioBitrate;
  end;

  TMediaTaskEncodeProgressEvent = function (Sender: TMediaTask; EncodingState: TMediaEncodingState;
    AProgress, AProgressMax: Integer; AProgressInfo: PMediaProgressInfo): Boolean of object;

  TMediaTaskMgr = class
  private
    FList: TMediaTaskList;
    FAutoId: Integer;
    FCustomProfile: TCustomProfileSettings;
    FOnEncodeProgress: TMediaTaskEncodeProgressEvent;
    FActiveTask: TMediaTask;
    FOutputDir: UnicodeString;
    FTotalLength: Double;
    FEncodedLength: Double;
    FEncodedTaskLength: Double;
    FFirstStartEncode: Boolean;
    FMergeAll: Boolean;
    FActiveTaskCount: Integer;
    FWatermark: TWatermarkParam;
    FRegistered: Boolean;
    FMaxAudioLen: Double;
    FEncodingTaskOrd: Integer;
    function GetActiveTaskCount: Integer;
    function GetAutoId: Integer;
    function GetTotalCutLength: Double;
    function GetTotalOutputFileSize: UInt64;
    function GetEncodedLength: Double;
    function GetIsAllOutputAudio: Boolean;
    function GetWatermark: PWatermarkParam;
    procedure SetWatermark(Value: PWatermarkParam);
    function DoEncodeProgress(Sender: TMediaTask; EncodingState: TMediaEncodingState;
      AProgress, AProgressMax: Integer; AProgressInfo: PMediaProgressInfo): Boolean;
  public
    constructor Create();
    destructor Destroy; override;
    // 启动转换，返回本次启动任务索引
    function StartEncode(var AStartIdx: Integer; hMsgWnd: HWnd=0): Integer;
    function StopEncode: Boolean;
    function EncodingOrPaused: Boolean;
    function Encoding: Boolean;
    procedure ResetAllSkip;

    property Tasks: TMediaTaskList read FList;
    property ActiveTask: TMediaTask read FActiveTask;
    property EncodingTaskOrd: Integer read FEncodingTaskOrd;
    property ActiveTaskCount: Integer read FActiveTaskCount;
    property TotalCutLength: Double read GetTotalCutLength;
    property TotalOutputFileSize: UInt64 read GetTotalOutputFileSize;
    property EncodedLength: Double read GetEncodedLength;
    property IsAllOutputAudio: Boolean read GetIsAllOutputAudio;
    property CustomProfile: TCustomProfileSettings read FCustomProfile;
    property Registered: Boolean read FRegistered write FRegistered;
    property Watermark: PWatermarkParam read GetWatermark write SetWatermark;
    property MaxAudioLen: Double read FMaxAudioLen write FMaxAudioLen;
    // 转换进度回调(已切换回主线程，线程安全)
    property OnEncodeProgress: TMediaTaskEncodeProgressEvent read FOnEncodeProgress write FOnEncodeProgress;
    property OutputDir: UnicodeString read FOutputDir write FOutputDir;
    property MergeAll: Boolean read FMergeAll write FMergeAll;
  end;

implementation

uses WideStrUtils, WideFileUtils, SysConfigs;

{ TMediaTask }

function TMediaTask.AddToSlideEncoder(ASlideEncoder: TMediaSlideEncoderEx;
  AStartTime: Double): Boolean;
var
  b1, b2: Boolean;
  ClipTime: CLIP_TIME;
begin
  ClipTime.dStart:=AStartTime;
  ClipTime.dEnd:=AStartTime+CutLength;
  ClipTime.dTrimFrom:=FMediaStart+CutStart;
  ClipTime.dTrimTo:=FMediaStart+CutStart+CutLength;
  if {HasVideoInfo and }Assigned(ASlideEncoder.VideoSlideDecoder) then
  begin  // 不要检查是否存在视频都往Slide上加
    b1:=Assigned(ASlideEncoder.VideoSlideDecoder.AddVideoClip(VideoInfo, ClipTime, Self));
  end else
    b1:=True;
  if {HasAudioInfo and }Assigned(ASlideEncoder.AudioSlideDecoder) then
  begin  // 不要检查是否存在音频都往Slide上加
    b2:=Assigned(ASlideEncoder.AudioSlideDecoder.AddAudioClip(AudioInfo, ClipTime, Self));
  end else
    b2:=b1;
  Result:=b1 or b2;
end;

function TMediaTask.VideoSlideClipEvent(Sender: TSlideDecoderEx; 
  nOptFlag: UINT): HVIDEODECODER;
var
  ResizeParam: RESIZE_PARAM;
begin
  Result:=nil;
  try
    case nOptFlag of
      SD_OPTFLAG_CLIP_START: // 准备视频剪辑解码器
        begin
          if FMediaDecoder = nil then
          begin
            FMediaDecoder := TMediaDecoder.Create(FileName, FVDecParam, FADecParam, DecModel);
          end;  
          if FVideoSlideProcess=nil then
          begin
            with Sender.VideoSlideParam do
            begin
              ResizeParam.width:=nWidth;
              ResizeParam.height:=nHeight;
              ResizeParam.uInterpolation:=Self.EncParam.Video.nInterpolation;
              ResizeParam.uStyle:=Self.ResizeStyle;
            end;
            FVideoSlideProcess:=TVideoMediaProcessor.Create(FMediaDecoder.GetVideoDecoder);
            with FVideoSlideProcess.VideoEffectStream do
            begin
              EnableVideoCopy;
              EnableVideoSubPicCompositor;
              EnableVideoCrop;
              EnableVideoRotation;
              EnableVideoEffectDeintelace;
              EnableVideoResizer;
              EnableVideoEffect;
              EnableVideoCompositor;
              EnableVideoSlider;
              EnablePCText;
            end;  
            FVideoSlideProcess.SetResizeParam(@ResizeParam);
          end;
          SetVideoProcessor(FVideoSlideProcess, True);
          Result := FVideoSlideProcess.Handle;
          FVideoClipCreated := True;
        end;
      SD_OPTFLAG_CLIP_END:  // 释放视频剪辑解码器
        begin
          FVideoClipCreated := False;
          if (not FVideoClipCreated) and (not FAudioClipCreated) then
          begin
            FMediaDecoder := nil;
          end;  
          FreeAndNil(FVideoSlideProcess);
        end;
    end;
  except
    if Assigned(ApplicationHandleException) then
      Classes.ApplicationHandleException(Self);
  end;
end;

function TMediaTask.AudioSlideClipEvent(Sender: TSlideDecoderEx; 
  nOptFlag: UINT): HAUDIODECODER;
begin
  Result:=nil;
  try
    case nOptFlag of
      SD_OPTFLAG_CLIP_START: // 准备音频剪辑解码器
        begin
          if FMediaDecoder = nil then
          begin
            FMediaDecoder := TMediaDecoder.Create(FileName, FVDecParam, FADecParam, DecModel);
          end;  
          if FAudioSlideProcess=nil then
          begin
            with Sender.AudioSlideParam do
            begin
              FAudioSlideProcess:=TAudioMediaProcessor.Create(FMediaDecoder.AudioDecoder);
              with FAudioSlideProcess.AudioEffectStream do
              begin
                EnableAudioProcessor;
                EnableAudioSlider;
                EnableAudioFade;
                EnableVolumeSetting;
              end;
              FAudioSlideProcess.SetSamplerate(nSampleRate);
              FAudioSlideProcess.SetChannel(nChannels);
            end;
            
          end;
          SetAudioProcessor(FAudioSlideProcess);
          FAudioClipCreated := True;
          Result:=FAudioSlideProcess.Handle;
        end;
      SD_OPTFLAG_CLIP_END:  // 释放音频剪辑解码器
        begin
          FAudioClipCreated := False;
          if (not FVideoClipCreated) and (not FAudioClipCreated) then
          begin
            FMediaDecoder := nil;
          end;
          FreeAndNil(FAudioSlideProcess);
        end;
    end;
  except
    if Assigned(ApplicationHandleException) then
      Classes.ApplicationHandleException(Self);
  end;
end;

function TMediaTask.CalcOutputFileSize: UInt64;
begin
  Result:=CalcOutputFileSize(EncParam);
end;

function TMediaTask.CalcOutputFileSize(const AEncParam: TEncodeParam): UInt64;
var
  pEncParam: TEncodeParam;
begin
  pEncParam:=AEncParam;
  pEncParam.bVideoDisable:=not HasVideoInfo;
  pEncParam.bAudioDisable:=not HasAudioInfo;
end;

constructor TMediaTask.Create(AOwner: TMediaTaskList; AId: Integer;
  const ATitle: string;const AFileName: UnicodeString; ADecModel: DEC_MODEL);
begin
  inherited Create;
  FOwner:=AOwner;
  FId:=AId;
  FTitle:=ATitle;
  FFileName:=AFileName;
  FDecModel:=ADecModel;
  if FTitle='' then
    FTitle:=WideExtractFileName(AFileName);
  FVolume:=KEEP_ORIGINAL_VOLUME;        // 保持原音量
  FResizeStyle:=IRM_ORIGINAL_SCALE;     // 保持原尺寸
  FAudioTracks:=TWideStringList.Create;
  FSubTitles:=TWideStringList.Create;
  FWaterMarkFont:=TFont.Create;
  FWaterMarkFont.Size:= 36;
  FWaterMarkFont.Color := clWhite;
  FSubTitleFont:=TFont.Create;
  FSubTitleFont.Size := 36;
  FSubTitleFont.Color := clWhite;
end;

destructor TMediaTask.Destroy;
begin
  FreeAndNil(FVideoSlideProcess);
  FreeAndNil(FAudioSlideProcess);
  FSubTitleFont.Free;
  FWaterMarkFont.Free;
  FAudioTracks.Free;
  FSubTitles.Free;
  FMediaEncoder.Free;
  FWaterMarkImage.Free;
  inherited;
end;

function TMediaTask.DoEncodeProgress(Sender: TMediaEncoder;
  EncodingState: TMediaEncodingState; AProgress, AProgressMax: Integer;
  AProgressInfo: PMediaProgressInfo): Boolean;
begin
  FEncodingState:=EncodingState;
  FProgress:=AProgress;
  FProgressMax:=AProgressMax;
  case EncodingState of
    mesFinished, mesUserAbort, mesCallBackAbort, mesError:
      begin
        FProgressInfo:=nil; // 非同步调用，不能保存指针
        TThread.Queue(nil, DoSyncEncodeFinish); // 加入到队列，返回, 等候处理
      end;
  else
    FProgressInfo:=AProgressInfo;
    TThread.Synchronize(nil, DoSyncEncodeProgress);
  end;
  Result:=FEncodeProgressResult;
end;

procedure TMediaTask.DoSyncEncodeFinish;
begin
  DoSyncFreeEncoder;
  DoSyncEncodeProgress;
end;

procedure TMediaTask.DoSyncEncodeProgress;
begin
  if Assigned(FOwner.Owner) then
    FEncodeProgressResult:=FOwner.Owner.DoEncodeProgress(Self, FEncodingState, FProgress, FProgressMax,
        FProgressInfo)
  else
    FEncodeProgressResult:=True;
end;

procedure TMediaTask.DoSyncFreeEncoder;
begin
  if GetCurrentThreadId=MainThreadId then
    CheckSynchronize(0);
  FreeAndNil(FMediaEncoder);
  FreeAndNil(FVideoSlideProcess);
  FreeAndNil(FAudioSlideProcess);
end;

function TMediaTask.GetADecParam: PDecParam;
begin
  Result:=@FADecParam;
end;

function TMediaTask.GetAudioTrackIdx: Integer;
begin
  Result:=FAudioTracks.IndexOfObject(TObject(FADecParam.nTrack));
end;

function TMediaTask.GetCutLength: Double;
begin
  if (FCutEnd<>0) and (FCutEnd<>FCutStart) then
    Result:=FCutEnd-FCutStart
  else
    Result:=MediaLength-FCutStart;
end;

function TMediaTask.GetIsLosslessCopy: Boolean;
begin
  Result:=FEncParam.dwFourCC=MakeFourCC('COPY');
end;

function TMediaTask.GetFlipFlag: UINT;
begin
  Result:=0;
  if HorzFlip then
    Result:=Result or FLIP_HORIZONTAL;
  if VertFlip then
    Result:=Result or FLIP_VERTICAL;
end;

function TMediaTask.GetFullOutFileName: UnicodeString;
begin
  Result:=WideIncludeTrailingBackslash(FOwner.Owner.OutputDir)+FOutFileName;
end;

function TMediaTask.GetIsOutputAudio: Boolean;
begin
  with EncParam do
  begin
    Result:=not bAudioDisable and not IsLosslessCopy and
      (bVideoDisable or (Video.nWidth=0) or
      (Video.nHeight=0) or (Video.nBitrate=0));
  end;
end;

function TMediaTask.GetMediaCantSeek: Boolean;
begin
  Result:=(HasVideoInfo and (VideoInfo.dwCapsFlag and MCAPS_CANT_SEEK<>0))
            or (HasAudioInfo and (AudioInfo.dwCapsFlag and MCAPS_CANT_SEEK<>0));
end;

function TMediaTask.GetMediaUnknownLength: Boolean;
begin
  Result:=(HasVideoInfo and (VideoInfo.dwCapsFlag and MCAPS_UNKNOWN_LENGTH<>0))
            or (HasAudioInfo and (AudioInfo.dwCapsFlag and MCAPS_UNKNOWN_LENGTH<>0));
end;

function TMediaTask.GetOutputVideoSize: TSize;
begin
  with FEncParam.Video do
  begin
    Result.cx:=nWidth;
    Result.cy:=nHeight;
  end;
  if (Result.cx=0) or (Result.cy=0) then // 保证输出视频大小有效（=源视频大小）
  begin
    Result.cx:=FVideoInfo.Width;
    Result.cy:=FVideoInfo.Height;
  end;
end;

function TMediaTask.GetPreviewResizeParam: RESIZE_PARAM;
begin
  with PreviewVideoSize do
  begin
    Result.width:=cx;
    Result.height:=cy;
    Result.uInterpolation:=FEncParam.Video.nInterpolation;
    Result.uStyle:=ResizeStyle;  //保持原始比例
  end;
end;

function TMediaTask.GetPreviewVideoSize: TSize;
begin
  if (FEncParam.Video.nWidth=0) and (FEncParam.Video.nHeight=0) then// 保证输出视频大小有效（=源视频大小）
  with FEncParam.Video do
  begin
    Result.cx:=FVideoInfo.Width;
    Result.cy:=FVideoInfo.Height;
  end else if (FEncParam.Video.nWidth>=FVideoInfo.Width) and (FEncParam.Video.nHeight>=FVideoInfo.Height) then
  with FEncParam.Video do
  begin
    Result.cx:=nWidth;
    Result.cy:=nHeight;
  end else
  begin
    if (FVideoInfo.Width>=FVideoInfo.Height) then
    begin
      Result.cx:=FVideoInfo.Width;
      Result.cy:=Round(FVideoInfo.Width*(FEncParam.Video.nHeight/FEncParam.Video.nWidth));
    end else
    begin
      Result.cy:=FVideoInfo.Height;
      Result.cx:=Round(FVideoInfo.Height*(FEncParam.Video.nWidth/FEncParam.Video.nHeight));
    end;
  end;
end;

function TMediaTask.GetOutputResizeParam: RESIZE_PARAM;
begin
  with OutputVideoSize do
  begin
    Result.width:=cx;
    Result.height:=cy;
    Result.uInterpolation:=FEncParam.Video.nInterpolation;
    Result.uStyle:=ResizeStyle;  //保持原始比例
  end;
end;

function TMediaTask.GetSubTitleId: Integer;
begin
  Result:=FVDecParam.nSubTrack;
end;

function TMediaTask.GetSubTitleIds(Index: Integer): Integer;
begin
  Result:=Integer(FSubTitles.Objects[Index]);
end;

function TMediaTask.GetSubTitleIdx: Integer;
begin
  Result:=FSubTitles.IndexOfObject(TObject(FVDecParam.nSubTrack));
end;

function TMediaTask.InitDefaultEncoders: Boolean;
var
  I, J: Integer;
  pFileFormatInfos: IFileFormatInfos;
  pFileFormatInfo: IFileFormatInfo;
  pVidEncoderInfos: IEncoderInfos;
  pAudEncoderInfos: IEncoderInfos;
  pVidEncoderInfo: IEncoderInfo;
  pAudEncoderInfo: IEncoderInfo;
begin
  pFileFormatInfos := nil;
  WSCoCreateInstance(CLSID_CFileFormatInfos,
    nil, 0, IID_IFileFormatInfo, pFileFormatInfos);
  if (FEncParam.Video.dwFourCC=0) or (FEncParam.Audio.dwFourCC=0) then
  begin
    if Assigned(pFileFormatInfos) then
    begin
      for I := 0 to pFileFormatInfos.GetCount - 1 do
      begin
        pFileFormatInfo := nil;
        pFileFormatInfos.GetItem(I, pFileFormatInfo);
        if Assigned(pFileFormatInfo) then
        begin
          if pFileFormatInfo.GetFourCC = EncParam.dwFourCC then
          begin
            pVidEncoderInfos := nil;
            pFileFormatInfo.GetVideoEncoderInfos(pVidEncoderInfos);
            if Assigned(pVidEncoderInfos) then
            begin
              for J := 0 to pVidEncoderInfos.GetCount - 1 do
              begin
                pVidEncoderInfo := nil;
                pVidEncoderInfos.GetItem(J, pVidEncoderInfo);
                if Assigned(pVidEncoderInfo) then
                begin
                  if pVidEncoderInfo.GetFourCC <> 0 then
                  begin
                    FEncParam.Video.dwFourCC := pVidEncoderInfo.GetFourCC;
                    pVidEncoderInfo._Release;
                    pVidEncoderInfo := nil;
                    Break;
                  end;  
                  pVidEncoderInfo._Release;
                  pVidEncoderInfo := nil;
                end;  
              end;  
              pVidEncoderInfos._Release;
              pVidEncoderInfos := nil;
            end;

            pAudEncoderInfos := nil;
            pFileFormatInfo.GetAudioEncoderInfos(pAudEncoderInfos);
            if Assigned(pAudEncoderInfos) then
            begin
              for J := 0 to pAudEncoderInfos.GetCount - 1 do
              begin
                pAudEncoderInfo := nil;
                pAudEncoderInfos.GetItem(J, pAudEncoderInfo);
                if Assigned(pAudEncoderInfo) then
                begin
                  if pAudEncoderInfo.GetFourCC <> 0 then
                  begin
                    FEncParam.Audio.dwFourCC := pAudEncoderInfo.GetFourCC;
                    pAudEncoderInfo._Release;
                    Break;
                  end;  
                  pAudEncoderInfo._Release;
                  pAudEncoderInfo := nil;
                end;  
              end;  
              pAudEncoderInfos._Release;
              pAudEncoderInfos := nil;
            end;
            Result := True;
            pFileFormatInfo._Release;
            Break;  
          end;  
          pFileFormatInfo._Release;
        end;
      end;
      pFileFormatInfos._Release;
    end;
  end;
  Result:=False;
end;

function TMediaTask.InitMediaEncoder(ASlide: Boolean): Boolean;
var
  dNewFrameRate: Double;
  pVDecParam, pADecParam: TDecParam;
  VidSlideParam: TVideoSlideParam;
  AudSlideParam: TAudioSlideParam;
  pVidParam: PVideoSlideParam;
  pAudParam: PAudioSlideParam;
  ResizeParam: RESIZE_PARAM;
  bLosslessCopy, bEvenFlag: Boolean;
begin
  FreeAndNil(FMediaEncoder);
  InitDefaultEncoders;
  if ASlide then
  begin
    FillChar(VidSlideParam, SizeOf(VidSlideParam), 0);
    FillChar(AudSlideParam, SizeOf(AudSlideParam), 0);
    pVidParam:=nil;
    pAudParam:=nil;
    if EncParam.Video.dwFourCC<>0 then
    begin
      with OutputVideoSize do
      begin
        VidSlideParam.nWidth:=cx;
        VidSlideParam.nHeight:=cy;
      end;
      with EncParam.Video do
      begin
        VidSlideParam.dFrameRate:=dFrameRate;
        if dFrameRate=0 then
          VidSlideParam.dFrameRate:=Self.VideoInfo.frame_rate;
        VidSlideParam.dwCompression:=dwCompression;
        VidSlideParam.nBitsDepth:=nBitsDepth;
        if not Owner.Owner.Registered then // 如果没有注册
          VidSlideParam.Watermark:=Owner.Owner.FWatermark; // 视频添加水印
        pVidParam:=@VidSlideParam;
      end;
    end;
    if EncParam.Audio.dwFourCC<>0 then
    with EncParam.Audio do
    begin
      AudSlideParam.nSampleRate:=nSampleRate;
      AudSlideParam.nChannels:=nChannels;
      AudSlideParam.nBitsPerSample:=nBitsPerSample;
      AudSlideParam.nInterpolation:=SIMPLE_LINEAR;
      pAudParam:=@AudSlideParam;
    end;
    Result:=Assigned(pVidParam) or Assigned(pAudParam);
    if Result then
    begin
      FMediaEncoder:=TMediaSlideEncoderEx.Create(pVidParam, pAudParam,
        VideoSlideClipEvent, AudioSlideClipEvent);
      FEncParam.Video.nResizeStyle:=ResizeStyle;
      FMediaEncoder.EncodeParam:=@EncParam;
    end;
  end else
  begin
    if (FEncParam.dwFourCC=MakeFourCC('flv ')) or (FEncParam.dwFourCC=MakeFourCC('avm2')) then
    begin
      with EncParam.Video do
      begin
        if dFrameRate=0 then
          dNewFrameRate:=VideoInfo.frame_rate
        else
          dNewFrameRate:=dFrameRate;
      end;
      bEvenFlag:=True;      // 只有FLV文件才使用使用帧率转换和时间戳校正功能
    end else
    begin
      dNewFrameRate:=-1;    // 一般不使用时间戳校正功能
      bEvenFlag:=False;     //
    end;
    bLosslessCopy:=IsLosslessCopy;
    if bLosslessCopy then // 无损拷贝不能加效果，不能视频连接
    begin
      pVDecParam:=FVDecParam;
      pADecParam:=FADecParam;
      pVDecParam.bDirectCopy:=bLosslessCopy;
      pADecParam.bDirectCopy:=bLosslessCopy;
      FMediaEncoder:=TMediaEncoder.Create(FFileName, pVDecParam,
         pADecParam, FDecModel);
    end else
    begin
      FillChar(ResizeParam, SizeOf(ResizeParam), 0);
      with OutputVideoSize do
      begin
        ResizeParam.width:=cx;
        ResizeParam.height:=cy;
      end;
      with EncParam.Video do
      begin
        ResizeParam.uInterpolation:=nInterpolation;
        ResizeParam.uStyle:=Self.ResizeStyle;
      end;
      FMediaEncoder:=TMediaProcessEncoder.Create(FFileName, FVDecParam,
         FADecParam, FDecModel,
         @ResizeParam, dNewFrameRate, EncParam.Audio.nSampleRate,
          EncParam.Audio.nChannels, EncParam.Audio.nBitsPerSample, bEvenFlag);
    end;
    Result:=True;
  end;
  if Assigned(FMediaEncoder) then
  begin
    FMediaEncoder.SetOutputFile(FullOutFileName);
    if Assigned(FOwner) and Assigned(FOwner.Owner) then
      FMediaEncoder.OnEncodeProgress:=DoEncodeProgress;
    Result:=FMediaEncoder.Status in [eshaveFile, esNone];
  end;
end;

function TMediaTask.InnerGetWaterMarkImage(AScale: Double): TDibImage;
var
  hImage: HDIBIMAGE;
  TextItem: TTextItem;
begin
  Result:=nil;
  if not WaterMark then Exit;
  Result:=FWaterMarkImage;
  if (Result=nil) then
  begin
    case WaterMarkType of
      wtImage:
        if WideFileExists(WaterMarkFile) then
          Result:=TDibImage.FromFile(WaterMarkFile); // 图片水印
      wtText:
        if WaterMarkText<>'' then
        begin
          FillChar(TextItem, SizeOf(TextItem), 0);
          WStrPLCopy(TextItem.strText, WaterMarkText, Length(TextItem.strText));
          with WaterMarkFont do
          begin
            WStrPLCopy(TextItem.strFontName, Name, Length(TextItem.strFontName));
            TextItem.crFont:=ColorToRGB(Color);
            TextItem.nFontSize:=Size;
            TextItem.uStyle:=VCLFontStyleToTextStyle(Style);
          end;
          hImage:=WSTextArtAPI.GenerateNormalTextEx(TextItem, 0, 0, nil);
          if Assigned(hImage) then
            Result:=TDibImage.Create(hImage);
        end;
    end;
    FWatermarkImage:=Result;
  end;
end;

function TMediaTask.Load: Boolean;
begin
  case FMediaType of
    mttDVDTitle,
    mttDVDChapter:
      begin
        Result:=LoadDVD;
        Exit;
      end;
  else
    Result:=LoadAV;
  end;
end;

function TMediaTask.LoadAV: Boolean;
var
  pMediaInfo: IMediaInfo;
begin
  Result:=False;
  pMediaInfo := nil;
  if not VADecoderAPI.DecMgrInit then Exit;
  try
    gDecMgr.GetMediaInfo(FileName, MEDIA_FILE, FDecModel, pMediaInfo);
  except
    if Assigned(pMediaInfo) then
      pMediaInfo._Release;
    pMediaInfo := nil;
    if Assigned(ApplicationHandleException) then
      Classes.ApplicationHandleException(Self);
  end;
  LoadExtSubTitleList(pMediaInfo);
  Result:=LoadFromMediaInfo(pMediaInfo, True);
end;

function TMediaTask.LoadDVD(ADVDInfo: TDVDIFOInfo): Boolean;
var
  pDVDInfo: TDVDIFOInfo;
  pMediaInfo: IMediaInfo;
begin
  Result:=False;
  if not VADecoderAPI.DecMgrInit then Exit;
  Result:=True;
  if not FLoaded then
  begin
    if ADVDInfo=nil then
      pDVDInfo:=TDVDIFOInfo.Create(FFileName)
    else
      pDVDInfo:=ADVDInfo;
    try
      pMediaInfo:=pDVDInfo.GetFileInfo();
      Result:=LoadFromMediaInfo(pMediaInfo, False);
      case MediaType of
        mttDVDTitle:
          begin
            FMediaStart:=pDVDInfo.GetTitleBeginTime(FTitleIndex);
            FMediaLength:=pDVDInfo.GetTitleLength(FTitleIndex);
            FChapterCount:=pDVDInfo.GetChapterCount(FTitleIndex);
            pDVDInfo.GetTitleAudioTrackList(FTitleIndex, FAudioTracks);
            pDVDInfo.GetSubTitleList(FTitleIndex, FSubTitles);

            FVDecParam.uProgramID:=pDVDInfo.GetTitleProgramId(pMediaInfo, FTitleIndex);
            FVDecParam.nSubTrack:=pDVDInfo.GetSubTitleTrackID(FTitleIndex, 0);

            FADecParam.uProgramID:=FVDecParam.uProgramID;
            FADecParam.nSubTrack:=FVDecParam.nSubTrack;
            FADecParam.nTrack:=pDVDInfo.GetAudioTrackID(FTitleIndex, 0);
          end;
        mttDVDChapter:
          begin
            FMediaStart:=pDVDInfo.GetChapterBeginTime(FTitleIndex, FChapterIndex);
            FMediaLength:=pDVDInfo.GetChapterLength(FTitleIndex, FChapterIndex);
            FChapterCount:=0;
            pDVDInfo.GetTitleAudioTrackList(FTitleIndex, FAudioTracks);
            pDVDInfo.GetSubTitleList(FTitleIndex, FSubTitles);

            FVDecParam.uProgramID:=pDVDInfo.GetTitleProgramId(pMediaInfo, FTitleIndex);
            FVDecParam.nSubTrack:=pDVDInfo.GetSubTitleTrackID(FTitleIndex, 0);

            FADecParam.uProgramID:=FVDecParam.uProgramID;
            FADecParam.nSubTrack:=FVDecParam.nSubTrack;
            FADecParam.nTrack:=pDVDInfo.GetAudioTrackID(FTitleIndex, 0);
          end;
      end;
    finally
      if ADVDInfo=nil then
        pDVDInfo.Free;
    end;
  end;
end;

function TMediaTask.LoadFromMediaInfo(pMediaInfo: IMediaInfo; ANeedRelease: Boolean=True): Boolean;
var
  pProgInfo: IProgramInfo;
  pProgramInfos: IProgramInfos;
  pVidStreamInfos: IStreamInfos;
  pAudStreamInfos: IStreamInfos;
  pVidStreamInfo: IStreamInfo;
  pAudStreamInfo: IStreamInfo;
begin
  Result:=False;
  if Assigned(pMediaInfo) then
  begin
    FMediaLength := pMediaInfo.GetMediaLength;
    pMediaInfo.GetIPrograms(pProgramInfos);
    pProgramInfos := nil;
    pProgInfo := nil;
    if Assigned(pProgramInfos) then
    begin
      if (FMediaType = mttDVDTitle) or (FMediaType = mttDVDChapter) then
      begin
        pProgramInfos.GetItem(FTitleIndex, pProgInfo);
      end
      else
      begin
        pProgramInfos.GetItem(0, pProgInfo);
      end;
      if Assigned(pProgInfo) then
      begin
        Result := True;
        FBitrate := pProgInfo.GetBitrate;
        pVidStreamInfos := nil;
        pProgInfo.GetVideoStreamInfos(pVidStreamInfos);
        if Assigned(pVidStreamInfos) then
        begin
          FHasVideoInfo := pVidStreamInfos.GetCount > 0;
          pVidStreamInfo := nil;
          pVidStreamInfos.GetItem(0, pVidStreamInfo);
          if Assigned(pVidStreamInfo) then
          begin
            FVideoBitrate := pVidStreamInfo.GetBitRate;
            if Assigned(pVidStreamInfo.GetVideoInfo) then
              FVideoInfo := pVidStreamInfo.GetVideoInfo^;
            pVidStreamInfo._Release;
          end;
          pVidStreamInfos._Release;
        end;

        pProgInfo.GetAudioStreamInfos(pAudStreamInfos);
        if Assigned(pAudStreamInfos) then
        begin
          FHasAudioInfo := pAudStreamInfos.GetCount > 0;
          pAudStreamInfo := nil;
          pAudStreamInfos.GetItem(0, pAudStreamInfo);
          if Assigned(pAudStreamInfo) then
          begin
            FAudioBitrate := pAudStreamInfo.GetBitRate;
            if Assigned(pAudStreamInfo.GetAudioInfo) then
              FAudioInfo := pAudStreamInfo.GetAudioInfo^;
            pAudStreamInfo._Release;
          end;
          pAudStreamInfos._Release;
        end;  
        pProgInfo._Release;
      end;  
      pProgramInfos._Release;
    end;
  end;
end;

function TMediaTask.LoadExtSubTitleList(pMediaInfo: IMediaInfo): Integer;
var
  i, j: Integer;
  WS: UnicodeString;
  pProgramInfos: IProgramInfos;
  pProgramInfo: IProgramInfo;
  pSubPicStreamInfos: ISubPicStreamInfos;
  pSubPicStreamInfo: ISubPicStreamInfo;
begin
  Result:=0;
  FSubTitles.Clear;
  if Assigned(pMediaInfo)then
  begin
    pProgramInfos := nil;
    pMediaInfo.GetIPrograms(pProgramInfos);
    if Assigned(pProgramInfos) then
    begin
      for I := 0 to pProgramInfos.GetCount- 1 do
      begin
        pProgramInfo := nil;
        pProgramInfos.GetItem(I, pProgramInfo);
        if Assigned(pProgramInfo) then
        begin
          pSubPicStreamInfos := nil;
          pProgramInfo.GetSubPicStreamInfos(pSubPicStreamInfos);
          if Assigned(pSubPicStreamInfos) then
          begin
            for J := 0 to pSubPicStreamInfos.GetCount - 1 do
            begin
              pSubPicStreamInfo := nil;
              pSubPicStreamInfos.GetItem(J, pSubPicStreamInfo);
              if Assigned(pSubPicStreamInfo) then
              begin
                if pSubPicStreamInfo.GetID <> 0 then
                begin
                  pSubPicStreamInfo.GetDescription(WS);
                  FSubTitles.AddObject(WS, TObject(pSubPicStreamInfo.GetID));
                end;
                pSubPicStreamInfo._Release;
              end;  
            end;
            pSubPicStreamInfos._Release;
          end;  
          pProgramInfo._Release;
        end;

      end;
      pProgramInfos._Release;  
    end;  
  end;
  FHasSubTitle:=Result>0;
end;

procedure TMediaTask.Remove;
begin
  Owner.Remove(Self);
end;

procedure TMediaTask.SetADecParam(Value: PDecParam);
begin
  if Assigned(Value) then
    FADecParam:=Value^;
end;

function TMediaTask.SetAudioProcessor(
  AAudioProcessor: TAudioMediaProcessor): Boolean;
begin
  Result:=Assigned(AAudioProcessor);
  if Result then
  begin
    if FMute then
      AAudioProcessor.SetVolum(0)
    else
      AAudioProcessor.SetVolum(FVolume);
  end;
end;

procedure TMediaTask.SetAudioTrackIdx(Value: Integer);
begin
  if (Value>=0) and (Value<FAudioTracks.Count) then
  begin
    FADecParam.nTrack:=Integer(FAudioTracks.Objects[Value]);
  end else
    FADecParam.nTrack:=0;
end;

procedure TMediaTask.SetFlipFlag(Value: UINT);
begin
  FHorzFlip:=Value and FLIP_HORIZONTAL<>0;
  FVertFlip:=Value and FLIP_VERTICAL<>0;
end;

procedure TMediaTask.SetResizeStyle(Value: IMAGE_RESIZE_METHOD);
begin
  FResizeStyle := Value;
  FEncParam.Video.nResizeStyle:=Value;
end;

procedure TMediaTask.SetSubTitleFont(const Value: TFont);
begin
  if Assigned(Value) then
    FSubTitleFont.Assign(Value);
end;

procedure TMediaTask.SetSubTitleId(Value: Integer);
begin
  FVDecParam.nSubTrack:=Value;
end;

procedure TMediaTask.SetSubTitleIdx(Value: Integer);
begin
  if (Value>=0) and (Value<FSubTitles.Count) then
  begin
    FVDecParam.nSubTrack:=Integer(FSubTitles.Objects[Value]);
  end else
    FVDecParam.nSubTrack:=0;
end;

function TMediaTask.SetUserSubTitle(AVideoProcessor: TVideoMediaProcessor; AToEncode: Boolean=False): Boolean;
var
  pUserSubPic: TUserSubPicRec;
  pUserSubPicEx: TUserSubPicEx;  
begin
  if SubTitle then
  begin
    FillChar(pUserSubPic, SizeOf(pUserSubPic), 0);
    pUserSubPic.uSubPicID := SubTitleId;
    pUserSubPic.Style.bUseDefaultStyle := not SubTitleUserStyle;
    if SubTitleUserStyle then
    begin
      pUserSubPic.Style.nAngle := SubTitleAngle;
      pUserSubPic.Style.nLeftTopX := SubTitlePos.X;
      pUserSubPic.Style.nLeftTopY := SubTitlePos.Y;
      with SubTitleFont do
      begin
        WStrPLCopy(pUserSubPic.Style.szFontName, Name, Length(pUserSubPic.Style.szFontName)-1);
        pUserSubPic.Style.Color := ColorToRGB(Color);
        pUserSubPic.Style.nFontSize := Size;
        pUserSubPic.Style.uStyle := VCLFontStyleToTextStyle(Style);
      end;
    end;
    if (FSubTitleFile<>'') then
    begin
      pUserSubPicEx.pSubPicPath:=PWideChar(FSubTitleFile);
      pUserSubPicEx.Style:=pUserSubPic.Style;
      AVideoProcessor.SetSubPicEx(pUserSubPicEx, pUserSubPic.nOpacity);
    end else
      AVideoProcessor.SetUserSubPicture(@pUserSubPic, 100-SubTitleTrans);
    Result:=True;
  end else
  begin
    AVideoProcessor.SetUserSubPicture(nil);
    Result:=True;
  end;
end;

procedure TMediaTask.SetVDecParam(Value: PDecParam);
begin
  if Assigned(Value) then
    FVDecParam:=Value^;
end;

function TMediaTask.SetVideoPreviewProcessor(
  AVideoProcessor: TVideoMediaProcessor): Boolean;
begin
  Result:=Assigned(AVideoProcessor);
  if Result then
  with AVideoProcessor do
  begin
    SetCropRectEx(Self.CropRect, ResizeStyle);
    SetBrightness(Self.Brightness);
    SetContrast(Self.Contrast);
    SetSaturation(Self.Saturation);
    SetExFilterFlag(Self.FilterID);
    SetFlipFlag(Self.FlipFlag);
    SetDeintelace(Self.FDeInterlacing);
    SetRotation(Round(Self.Rotation));
    // 外挂字幕（一般是MKV文件）
    SetUserSubTitle(AVideoProcessor);
//    // 用户水印
//    FreeAndNil(FWaterMarkImage);
//    pMarkImage:=InnerGetWatermarkImage(PreviewVideoSize.cx/OutputVideoSize.cx);
//    if Assigned(pMarkImage) then
//    begin
//      fRect:=CalcWatermarkPos;
//      SetUserWatermarkImage(pMarkImage.BitmapInfo, pMarkImage.Bits, @fRect, 100-WaterMarkTrans);
//    end else
//      SetUserWatermarkImage(nil, nil);
  end;
end;

function TMediaTask.SetVideoProcessor(
  AVideoProcessor: TVideoMediaProcessor; AToEncode: Boolean): Boolean;
begin
  Result:=Assigned(AVideoProcessor);
  if Result then
  with AVideoProcessor do
  begin
    SetCropRectEx(Self.CropRect, ResizeStyle);
    SetBrightness(Self.Brightness);
    SetContrast(Self.Contrast);
    SetSaturation(Self.Saturation);
    SetExFilterFlag(Self.FilterID);
    SetFlipFlag(Self.FlipFlag);
    SetDeintelace(Self.FDeInterlacing);
    SetRotation(Round(Self.Rotation));
    // 外挂字幕（一般是MKV文件）
    SetUserSubTitle(AVideoProcessor);
    // 用户水印
    if Assigned(FSetVPUserWaterMarkProc) then
      FSetVPUserWaterMarkProc(AVideoProcessor, AToEncode);
    // 版权水印
    if AToEncode then  // 编码输出文件时才添加水印
    with Owner.Owner do
    begin
      if not Registered then
      begin
        with FWatermark do
          SetWatermarkSrcIDAlign(hModule, @szID, nAlign, nOpacity);
        DisableWatermark(False);
      end;
    end;
  end;
end;

function TMediaTask.SetVPUserWaterMark(AVideoProcessor: TVideoMediaProcessor;
  AToEncode: Boolean): Boolean;

  function CalcWatermarkPos(): TFRect;
  begin
    with GetOutputVideoSize do
      Result:=CalcRelativePos(WaterMarkPos.X, WaterMarkPos.Y, WatermarkSize.cx, WatermarkSize.cy,
        cx, cy);
  end;

var
  fRect: TFRECT;
  pMarkImage: TDibImage;
begin
  Result:=Assigned(AVideoProcessor);
  if Result then
  with AVideoProcessor do
  begin
    pMarkImage:=GetWatermarkImage;
    if Assigned(pMarkImage) and (VideoInfo.Width<>0) and (VideoInfo.Height<>0) then
    begin
      fRect:=CalcWatermarkPos;
      SetUserWatermarkImage(pMarkImage.BitmapInfo, pMarkImage.Bits, @fRect, 100-WaterMarkTrans);
    end else
      SetUserWatermarkImage(nil, nil);
  end;
end;

procedure TMediaTask.SetWaterMark(Value: Boolean);
begin
  if Value<>FWaterMark then
  begin
    FWaterMark := Value;
    if Value then
      FSetVPUserWaterMarkProc:=SetVPUserWaterMark;
  end;
end;

procedure TMediaTask.SetWaterMarkFont(const Value: TFont);
begin
  if Assigned(Value) then
    FWaterMarkFont.Assign(Value);
end;

procedure TMediaTask.SetWaterMarkImage(Value: TDibImage);
begin
  FreeAndNil(FWaterMarkImage);
  if Assigned(Value) then
    FWaterMarkImage:=Value.Clone;
end;

function TMediaTask.StartEncode(hMsgWnd: HWnd=0): Integer;
var
  dLength: Double;
  pEncParam: TEncodeParam;
begin
  Result:=-1;
  if InitMediaEncoder(False) then
  begin
    MediaEncodeMgrAPI.FCInitPlugins;
    dLength:=CutLength;
    if not Owner.Owner.Registered then// 如果没有注册
    begin
      if IsOutputAudio then // 纯音频设置时长限制（一般３分钟，180秒）
      begin
        if dLength>Owner.Owner.MaxAudioLen then
          dLength:=Owner.Owner.MaxAudioLen;
      end;
    end;
    if FMediaEncoder is TMediaProcessEncoder then
    begin
      SetVideoProcessor(TMediaProcessEncoder(FMediaEncoder).VideoProcessor, True);
      SetAudioProcessor(TMediaProcessEncoder(FMediaEncoder).AudioProcessor);
    end;
    FEncParam.Video.nResizeStyle:=ResizeStyle;
    pEncParam:=FEncParam;
    with pEncParam.Video do
    begin
      if dFrameRate=0 then
        dFrameRate:=VideoInfo.frame_rate;
      if (nWidth=0) or (nHeight=0) then // 支持保持源文件分辨率功能
      begin
        nWidth:=VideoInfo.Width;
        nHeight:=VideoInfo.Height;
      end;
    end;
    FMediaEncoder.EncodeParam:=@pEncParam;
    Result:=FMediaEncoder.Start(FMediaStart+FCutStart, dLength, hMsgWnd);
  end; 
end;

function TMediaTask.StopEncode: Boolean;
begin
  Result:=False;
  if Assigned(FMediaEncoder) then
  begin
    CheckSynchronize(0);
    FMediaEncoder.Stop();
    FreeAndNil(FMediaEncoder);
    Result:=True;
  end;
end;

function TMediaTask.GetVDecParam: PDecParam;
begin
  Result:=@FVDecParam;
end;

function TMediaTask.GetWaterMarkImage: TDibImage;
begin
  Result:=InnerGetWaterMarkImage(1);
end;

{ TMediaTaskListEnumerator }

constructor TMediaTaskListEnumerator.Create(AList: TMediaTaskList);
begin
  inherited Create;
  FIndex:=-1;
  FList:=AList;
end;

function TMediaTaskListEnumerator.GetCurrent: TMediaTask;
begin
  Result:=FList[FIndex];
end;

function TMediaTaskListEnumerator.MoveNext: Boolean;
begin
  Result:=FIndex<FList.Count-1;
  if Result then
    Inc(FIndex);
end;

{ TMediaTaskList }

function TMediaTaskList.Add(Item: TMediaTask): Integer;
begin
  Result:=FList.Add(Item);
end;

function TMediaTaskList.Add(AId: Integer; const ATitle: string; const AFileName: UnicodeString; ADecModel: DEC_MODEL): TMediaTask;
begin
  Result:=TMediaTask.Create(Self, AId, ATitle, AFileName, ADecModel);
  Add(Result);
end;

function TMediaTaskList.Add(const AFileName: UnicodeString;
  ADecModel: DEC_MODEL): TMediaTask;
begin
  Result:=Add(FOwner.GetAutoId, '', AFileName, ADecModel);
end;

function TMediaTaskList.Add(const ATitle: string; const AFileName: UnicodeString;
  ADecModel: DEC_MODEL): TMediaTask;
begin
  Result:=Add(FOwner.GetAutoId, ATitle, AFileName, ADecModel);
  Result.FTitle:=ATitle;
end;

function TMediaTaskList.CheckExists(const AFileName: UnicodeString): Boolean;
begin
  Result:=Assigned(Find(AFileName));
end;

function TMediaTaskList.CheckRepeatAndRenameOutFile(ASkipItem: TMediaTask;
  const AFileName, AExt: UnicodeString): UnicodeString;
var
  LastRepeatCount: Integer;
  nRepeatCount: Integer;
  szFile: UnicodeString;
begin
  szFile:=AFileName;
  LastRepeatCount:=0;
  repeat
    nRepeatCount:=FindRepeat(ASkipItem, szFile+AExt);
    if nRepeatCount>0 then
    begin
      if LastRepeatCount=0 then
        LastRepeatCount:=nRepeatCount
      else
        Inc(LastRepeatCount);
      szFile:=WideFormat('%s(%d)', [AFileName, LastRepeatCount]);
    end;
  until nRepeatCount=0;
  Result:=szFile+AExt;
end;

procedure TMediaTaskList.Clear;
begin
  FList.Clear;
end;

constructor TMediaTaskList.Create(AOwner: TMediaTaskMgr);
begin
  inherited Create();
  FOwner:=AOwner;
  FList:=TObjectList.Create;
end;

procedure TMediaTaskList.Delete(Index: Integer);
begin
  FList.Delete(Index);
end;

destructor TMediaTaskList.Destroy;
begin
  FList.Free;
  inherited;
end;

function TMediaTaskList.Extract(Item: TMediaTask): TMediaTask;
begin
  Result:=TMediaTask(FList.Extract(Item));
end;

function TMediaTaskList.Find(const AFileName: UnicodeString): TMediaTask;
var
  pItem: TMediaTask;
begin
  for pItem in Self do
  begin
    if WideSameText(pItem.FileName, AFileName) then
    begin
      Result:=pItem;
      Exit;
    end;
  end;
  Result:=nil;
end;

function TMediaTaskList.Find(ASkipItem: TMediaTask;
  const AFileName: UnicodeString): TMediaTask;
var
  pItem: TMediaTask;
begin
  for pItem in Self do
  begin
    if (pItem<>ASkipItem) and WideSameText(pItem.FileName, AFileName) then
    begin
      Result:=pItem;
      Exit;
    end;
  end;
  Result:=nil;
end;

function TMediaTaskList.FindOutNoExt(ASkipItem: TMediaTask; const AFileName: UnicodeString): Integer;
var
  szFile: UnicodeString;
  pItem: TMediaTask;
begin
  Result:=0;
  szFile:=WideChangeFileExt(WideExtractFileName(AFileName), '');
  for pItem in Self do
  begin
    if (pItem<>ASkipItem) and
      WideSameText(WideChangeFileExt(WideExtractFileName(pItem.OutFileName), ''), szFile) then
    begin
      Inc(Result);
    end;
  end;
end;

function TMediaTaskList.FindRepeat(ASkipItem: TMediaTask;
  const AFileName: UnicodeString): Integer;
var
  pItem: TMediaTask;
begin
  Result:=0;
  for pItem in Self do
  begin
    if (pItem<>ASkipItem) and
      WideSameText(WideExtractFileName(pItem.OutFileName), AFileName) then
    begin
      Inc(Result);
    end;
  end;
end;

function TMediaTaskList.Find(Id: Integer): TMediaTask;
var
  i: Integer;
  pItem: TMediaTask;
begin
  for i:=0 to FList.Count-1 do
  begin
    pItem:=TMediaTask(FList[i]);
    if pItem.Id=Id then
    begin
      Result:=pItem;
      Exit;
    end;
  end;
  Result:=nil;
end;

function TMediaTaskList.GetCount: Integer;
begin
  Result:=FList.Count;
end;

function TMediaTaskList.GetEnumerator: TMediaTaskListEnumerator;
begin
  Result:=TMediaTaskListEnumerator.Create(Self);
end;

function TMediaTaskList.GetItems(Index: Integer): TMediaTask;
begin
  Result:=TMediaTask(FList[Index]);
end;

function TMediaTaskList.IndexOf(Id: Integer): Integer;
var
  i: Integer;
  pItem: TMediaTask;
begin
  for i:=0 to FList.Count-1 do
  begin
    pItem:=TMediaTask(FList[i]);
    if pItem.Id=Id then
    begin
      Result:=i;
      Exit;
    end;
  end;
  Result:=-1;
end;

procedure TMediaTaskList.Insert(Index: Integer; Item: TMediaTask);
begin
  FList.Insert(Index, Item);
end;

procedure TMediaTaskList.Move(CurIndex, NewIndex: Integer);
begin
  FList.Move(CurIndex, NewIndex);
end;

function TMediaTaskList.Remove(Item: TMediaTask): Integer;
begin
  Result:=FList.Remove(Item);
end;

function MediaListSortByOrderIdCompare(Item1, Item2: Pointer): Integer;
begin
  Result:=TMediaTask(Item1).OrderId-TMediaTask(Item2).OrderId;
end;

procedure TMediaTaskList.SortByOrderId;
begin
  FList.Sort(MediaListSortByOrderIdCompare);
end;

{ TCustomProfileSettings }

procedure TCustomProfileSettings.AssignFrom(ATask: TMediaTask);
begin
  FFormatId:=ATask.FormatId;
  FVideoEncoder:=ATask.EncParam.Video.dwFourCC;
  FVideoBitrate:=ATask.EncParam.Video.nBitrate;
  FVideoFrameRate:=ATask.EncParam.Video.dFrameRate;
  FVideoWidth:=ATask.EncParam.Video.nWidth;
  FVideoHeight:=ATask.EncParam.Video.nHeight;

  FAudioEncoder:=ATask.EncParam.Audio.dwFourCC;
  FAudioBitrate:=ATask.EncParam.Audio.nBitrate;
  FAudioSampleRate:=ATask.EncParam.Audio.nSampleRate;
  FAudioChannels:=ATask.EncParam.Audio.nChannels;
end;

procedure TCustomProfileSettings.AssignFrom(Source: TCustomProfileSettings);
begin
  FFormatId:=Source.FormatId;
  FVideoEncoder:=Source.FVideoEncoder;
  FVideoBitrate:=Source.FVideoBitrate;
  FVideoFrameRate:=Source.FVideoFrameRate;
  FVideoWidth:=Source.FVideoWidth;
  FVideoHeight:=Source.FVideoHeight;

  FAudioEncoder:=Source.FAudioEncoder;
  FAudioBitrate:=Source.FAudioBitrate;
  FAudioSampleRate:=Source.FAudioSampleRate;
  FAudioChannels:=Source.FAudioChannels;
end;

procedure TCustomProfileSettings.AssignTo(ATask: TMediaTask);
var
  pEncParam: TEncodeParam;
begin
  if FFormatId=0 then Exit;

  ATask.FormatId:=FFormatId;
  pEncParam:=ATask.EncParam;
  if FVideoEncoder<>0 then
    pEncParam.Video.dwFourCC:=FVideoEncoder;
  if FVideoBitrate<>0 then
    pEncParam.Video.nBitrate:=FVideoBitrate;
//  if FVideoFrameRate<>0 then  // =0时保持原来的帧率
    pEncParam.Video.dFrameRate:=FVideoFrameRate;
//  if FVideoWidth<>0 then     // =0时保持原来的视频大小
    pEncParam.Video.nWidth:=FVideoWidth;
//  if FVideoHeight<>0 then    // =0时保持原来的视频大小
    pEncParam.Video.nHeight:=FVideoHeight;
  if FAudioEncoder<>0 then
    pEncParam.Audio.dwFourCC:=FAudioEncoder;
  if FAudioBitrate<>0 then
    pEncParam.Audio.nBitrate:=FAudioBitrate;
  if FAudioSampleRate<>0 then
    pEncParam.Audio.nSampleRate:=FAudioSampleRate;
  if FAudioChannels<>0 then
    pEncParam.Audio.nChannels:=FAudioChannels;
  ATask.EncParam:=pEncParam;
end;

procedure TCustomProfileSettings.LoadConfig;
begin
  gConfig.Section:='Output';
  FFormatId:=gConfig.ReadInteger('FormatId', 0);
  FVideoEncoder:=gConfig.ReadInteger('VideoFourCC', FVideoEncoder);
  FVideoBitrate:=gConfig.ReadInteger('VideoBitrate', FVideoBitrate);
  FVideoFrameRate:=gConfig.ReadFloat('VideoFrameRate', FVideoFrameRate);
  FVideoWidth:=gConfig.ReadInteger('VideoWidth', FVideoWidth);
  FVideoHeight:=gConfig.ReadInteger('VideoHeight', FVideoHeight);

  FAudioEncoder:=gConfig.ReadInteger('AudioFourCC', FAudioEncoder);
  FAudioBitrate:=gConfig.ReadInteger('AudioBitrate', FAudioBitrate);
  FAudioSampleRate:=gConfig.ReadInteger('AudioSampleRate', FAudioSampleRate);
  FAudioChannels:=gConfig.ReadInteger('Channels', FAudioChannels);
end;

procedure TCustomProfileSettings.SaveConfig;
begin
  gConfig.Section:='Output';
  gConfig.WriteInteger('FormatId', FFormatId);

  gConfig.WriteInteger('VideoFourCC', FVideoEncoder);
  gConfig.WriteInteger('VideoBitrate', FVideoBitrate);
  gConfig.WriteFloat('VideoFrameRate', FVideoFrameRate);
  gConfig.WriteInteger('VideoWidth', FVideoWidth);
  gConfig.WriteInteger('VideoHeight', FVideoHeight);

  gConfig.WriteInteger('AudioFourCC', FAudioEncoder);
  gConfig.WriteInteger('AudioBitrate', FAudioBitrate);
  gConfig.WriteInteger('AudioSampleRate', FAudioSampleRate);
  gConfig.WriteInteger('Channels', FAudioChannels);
end;

{ TMediaTaskMgr }

constructor TMediaTaskMgr.Create;
begin
  inherited Create;
  FList:=TMediaTaskList.Create(Self);
  FCustomProfile:=TCustomProfileSettings.Create;
  FFirstStartEncode:=True;
end;

destructor TMediaTaskMgr.Destroy;
begin
  FCustomProfile.Free;
  FList.Free;
  inherited;
end;

function TMediaTaskMgr.DoEncodeProgress(Sender: TMediaTask;
  EncodingState: TMediaEncodingState; AProgress, AProgressMax: Integer;
  AProgressInfo: PMediaProgressInfo): Boolean;
begin
  case EncodingState of
    mesEncoding:
      begin
        FEncodedTaskLength:=AProgressInfo.dCurrTime;
      end;
    mesFinished:
      begin
        FEncodedTaskLength:=0;
        FEncodedLength:=FEncodedLength+Sender.CutLength
      end;
  end;
  if Assigned(FOnEncodeProgress) then
    Result:=FOnEncodeProgress(Sender, EncodingState, AProgress, AProgressMax,
      AProgressInfo)
  else
    Result:=True;
end;

function TMediaTaskMgr.Encoding: Boolean;
begin
  Result:=False;
  if Assigned(FActiveTask) and Assigned(FActiveTask.MediaEncoder) then
  begin
    Result:=FActiveTask.MediaEncoder.Status=esEncoding;
  end;
end;

function TMediaTaskMgr.EncodingOrPaused: Boolean;
begin
  Result:=False;
  if Assigned(FActiveTask) and Assigned(FActiveTask.MediaEncoder) then
  begin
    Result:=FActiveTask.MediaEncoder.Status in [esEncoding, esPause];
  end;
end;

function TMediaTaskMgr.GetActiveTaskCount: Integer;
var
  pTask: TMediaTask;
begin
  Result:=0;
  for pTask in FList do
  begin
    if not pTask.SkipEncode then
    begin
      Inc(Result);
    end;
  end;
end;

function TMediaTaskMgr.GetAutoId: Integer;
begin
  if FAutoId<MaxInt then
    Inc(FAutoId)
  else
    FAutoId:=0;
  Result:=FAutoId;
end;

function TMediaTaskMgr.GetEncodedLength: Double;
begin
  Result:=FEncodedTaskLength+FEncodedLength;
end;

function TMediaTaskMgr.GetIsAllOutputAudio: Boolean;
var
  pTask: TMediaTask;
begin
  for pTask in FList do
  begin
    if not pTask.SkipEncode and not pTask.IsOutputAudio then
    begin
      Result:=False;
      Exit;
    end;
  end;
  Result:=True;
end;

function TMediaTaskMgr.GetTotalCutLength: Double;
var
  dLen: Double;
  pTask: TMediaTask;
begin
  if FTotalLength=0 then
  begin
    dLen:=0;
    for pTask in FList do
    begin
      if not pTask.SkipEncode then
        dLen:=dLen+pTask.CutLength;
    end;
    FTotalLength:=dLen;
  end;
  Result:=FTotalLength;
end;

function TMediaTaskMgr.GetTotalOutputFileSize: UInt64;
var
  c: Integer;
  pEncParam: TEncodeParam;
  pTask: TMediaTask;
begin
  Result:=0;
  if MergeAll then
  begin
    c:=0;
    for pTask in FList do
    begin
      if not pTask.SkipEncode then
      begin
        if c=0 then
          pEncParam:=pTask.EncParam;
        Inc(Result, pTask.CalcOutputFileSize(pEncParam));
        Inc(c);
      end;
    end;
  end else
  begin
    for pTask in FList do
    begin
      if not pTask.SkipEncode then
      begin
        Inc(Result, pTask.CalcOutputFileSize);
      end;
    end;
  end;
end;

function TMediaTaskMgr.GetWatermark: PWatermarkParam;
begin
  Result:=@FWatermark;
end;

procedure TMediaTaskMgr.ResetAllSkip;
var
  pTask: TMediaTask;
begin
  for pTask in FList do
  begin
    pTask.SkipEncode:=False;
  end;
end;

procedure TMediaTaskMgr.SetWatermark(Value: PWatermarkParam);
begin
  FWatermark:=Value^;
end;

function TMediaTaskMgr.StartEncode(var AStartIdx: Integer; hMsgWnd: HWnd): Integer;
var
  i: Integer;
  dStartTime, dMaxLength: Double;
  pTask: TMediaTask;
  pEncoder: TMediaSlideEncoderEx;
begin
  if MergeAll and (FList.Count>1) then
  begin
    if AStartIdx>0 then
    begin
      Result:=-1;
      FActiveTask:=nil;
      Exit;
    end;
    FTotalLength:=0;
    FEncodedLength:=0;
    FEncodedTaskLength:=0;
    FActiveTaskCount:=0;
    FEncodingTaskOrd:=0;
    dMaxLength:=0;
    if AStartIdx=0 then
    begin
      pEncoder:=nil;
      dStartTime:=0;
      if FActiveTask=nil then
      begin
        for i := 0 to FList.Count-1 do
        begin
          pTask:=FList[i];
          pTask.StopEncode;
          pTask.DoSyncFreeEncoder;
        end;
      end;
      for i := AStartIdx to FList.Count-1 do
      begin
        pTask:=FList[i];
        if pTask.SkipEncode then Continue;

        if FActiveTask=nil then
        begin
          AStartIdx:=i;
          if pTask.InitMediaEncoder(True) then
          begin
            FActiveTask:=pTask;
            pEncoder:=pTask.FMediaEncoder as TMediaSlideEncoderEx;
            if not Registered and pTask.IsOutputAudio then
            begin
              dMaxLength:=MaxAudioLen;
            end;
          end;
        end;
        if Assigned(pEncoder) then
        begin
          if pTask.AddToSlideEncoder(pEncoder, dStartTime) then
          begin
            dStartTime:=dStartTime+pTask.CutLength;
          end;
          if (dMaxLength>0) and (dStartTime>dMaxLength) then // 超出限制不再添加
          begin
            Break;
          end;
        end
        else
          Break;
      end;
      if dMaxLength>dStartTime then dMaxLength:=dStartTime;
            
      if Assigned(pEncoder) then
      begin
        FActiveTaskCount:=1;
        Inc(FEncodingTaskOrd);
        MediaEncodeMgrAPI.FCInitPlugins;
        Result:=pEncoder.Start(0, dMaxLength, hMsgWnd);
        FActiveTaskCount:=1;
        Exit;
      end;
    end;
  end else
  begin
    if FFirstStartEncode then // 第一次启动任务
    begin
      FTotalLength:=0;
      GetTotalCutLength;      // 更新需转换总长度
      FEncodedTaskLength:=0;
      FActiveTaskCount:=GetActiveTaskCount;
      FEncodedLength:=0;
      FEncodingTaskOrd:=0;
      FFirstStartEncode:=False;
    end;
    for i:=AStartIdx to FList.Count-1 do
    begin
      pTask:=FList[i];
      if not pTask.SkipEncode then
      begin
        Inc(FEncodingTaskOrd);
        Result:=pTask.StartEncode(hMsgWnd);
        if Result=0 then
        begin
          FActiveTask:=pTask;
          AStartIdx:=i;
          FActiveTaskCount:=GetActiveTaskCount;
          Exit;
        end;
      end;
    end;
  end;
  Result:=-1;
  FFirstStartEncode:=True;
  FActiveTask:=nil;
end;

function TMediaTaskMgr.StopEncode: Boolean;
var
  pTask: TMediaTask;
begin
  Result:=False;
  for pTask in FList do
  begin
    Result:=pTask.StopEncode;
  end;
  for pTask in FList do
  begin
    pTask.DoSyncFreeEncoder;
  end;
  FFirstStartEncode:=True;
  FActiveTask:=nil;
end;

end.
