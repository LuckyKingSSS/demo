{*******************************************************}
{       Wondershare独立进程批量转换任务管理             }
{       创建人:　 凌曙光　　 　　　　　　　   　　　　  }
{       创建日期: 2009/07/07                            }
{       修订人:                                         }
{       修订日期:                                       }
{*******************************************************}
{$Z4}  // 4字节枚举

unit MediaTaskParaConv;

interface

uses Windows, Messages, SysUtils, Classes, Contnrs, MediaCommon, MediaEncode,
  MediaEncodeMgr, MediaBatchConv, MediaTaskPro, MediaDibImage, MediaProcess,
  WSSubPic;

type
  TMediaParallelEncodeTaskList = class;

  TMediaParallelEncodeTask = class(TMediaEncodeTask)
  private
    FEncoder: TMediaBatchEncoder;
    FEncodedLength: Double;
    function GetOwner: TMediaParallelEncodeTaskList;
    function InitEncoder: Boolean;
    function DoEncodeProgress(Sender: TMediaBatchEncoder; EncodingState: TMediaEncodingState;
      AProgress, AProgressMax: Integer; AProgressInfo: PMediaProgressInfo): Boolean;
  public
    constructor Create(AOwner: TMediaParallelEncodeTaskList; ATask: TMediaTask);
    destructor Destroy; override;

    function Start(hMsgWnd: HWnd): Boolean; override;
    function Pause: Boolean; override;
    function Resume: Boolean; override;
    function Stop: Boolean; override;

    property Owner: TMediaParallelEncodeTaskList read GetOwner;
    property Encoder: TMediaBatchEncoder read FEncoder;
  end;

  // 多任务并行转换（同一时间可以有多个转换任务并行）
  TMediaParallelEncodeTaskList = class(TMediaEncodeTaskList)
  private
    FParallelNum: Integer;
    FEncodingTaskOrd: Integer;     // 所有任务已转换长度
    FOnEncodeProgress: TMediaTaskEncodeProgressEvent;
    FActiveTaskCount: Integer;
    function GetEncodedLength: Double;
    procedure TasksToEncodeList;
    function DoEncodeProgress(Sender: TMediaParallelEncodeTask; EncodingState: TMediaEncodingState;
      AProgress, AProgressMax: Integer; AProgressInfo: PMediaProgressInfo): Boolean;
  public
    constructor Create(AOwner: TMediaTaskMgr);
    destructor Destroy; override;

    function EncodeNext(hMsgWnd: HWnd=0): Boolean;
    function Pause: Boolean;
    function Resume: Boolean;
    function Stop: Boolean;

    property EncodingTaskOrd: Integer read FEncodingTaskOrd;              // 正在转换的任务序号
    property ParallelNum: Integer read FParallelNum write FParallelNum;   // 允许并行任务数
    property ActiveTaskCount: Integer read FActiveTaskCount;              // 正在转换的任务数
    property EncodedLength: Double read GetEncodedLength;                 // 当前任务已经转换的长度
    property OnEncodeProgress: TMediaTaskEncodeProgressEvent read FOnEncodeProgress write FOnEncodeProgress; // 转换进度
  end;

implementation

uses WideStrUtils;

{ TMediaParallelEncodeTask }

constructor TMediaParallelEncodeTask.Create(
  AOwner: TMediaParallelEncodeTaskList; ATask: TMediaTask);
begin
  inherited Create(AOwner, ATask);
end;

destructor TMediaParallelEncodeTask.Destroy;
begin
  FEncoder.Free;
  inherited;
end;

function TMediaParallelEncodeTask.DoEncodeProgress(Sender: TMediaBatchEncoder;
  EncodingState: TMediaEncodingState; AProgress, AProgressMax: Integer;
  AProgressInfo: PMediaProgressInfo): Boolean;
begin
  case EncodingState of
    mesEncoding:
      if AProgressMax>0 then
        FEncodedLength:=(Task.OutputLength*AProgress/AProgressMax);
    mesFinished, mesUserAbort,
      mesCallBackAbort, mesError:
      begin
        FEncodedLength:=Task.OutputLength;
      end;
  end;
  Result:=Owner.DoEncodeProgress(Self, EncodingState, AProgress, AProgressMax, AProgressInfo);
end;

function TMediaParallelEncodeTask.GetOwner: TMediaParallelEncodeTaskList;
begin
  Result:=TMediaParallelEncodeTaskList(inherited Owner);
end;

function TMediaParallelEncodeTask.InitEncoder: Boolean;

  // 添加用户水印
  procedure InitTaskUserWatermarkParams(pEditParams: TMediaTaskEditParams; var pTaskItem: TTaskItem);
  begin
    if pEditParams.WaterMark then // 用户添加的水印
      pTaskItem.UserWatermark:=pEditParams.GetUserWaterMarkParam();
  end;

  // 添加版权水印
  procedure InitCopyrightWatermark(pEncodeParams: TMediaTaskEncodeParams; var pTaskItem: TTaskItem);
  begin
    if not Owner.Owner.Registered then// 如果没有注册, 添加版权水印
    begin
      if pEncodeParams.IsOutputAudio then // 纯音频设置时长限制（一般３分钟，180秒）
      begin
        FEncoder.SetAudioLimitLength(Round(Owner.Owner.MaxAudioLen));
      end;
      // 版权水印
      pTaskItem.Watermark:=Owner.Owner.Watermark^;
    end;
  end;

  // 添加外挂字幕
  procedure InitTaskSubPicParams(pEditParams: TMediaTaskEditParams;var pTaskItem: TTaskItem);
  var
    pUserSubPic: TUserSubPicRec;
  begin
    if pEditParams.SubTitle then
    begin
      pUserSubPic:=pEditParams.GetUserSubTitleParams();
      pTaskItem.UserSubPic.nDummy1:=USEData_Buffer_SUBSTYLE;
      pTaskItem.UserSubPic.uSubPicID:=pUserSubPic.uSubPicID;
      pTaskItem.UserSubPic.Style:=pUserSubPic.Style;
      pTaskItem.UserSubPic.nOpacity:=pUserSubPic.nOpacity;
      WStrPLCopy(pTaskItem.UserSubPic.pSubPicPath, pEditParams.SubTitleFile, SizeOf(pTaskItem.UserSubPic.pSubPicPath)-1);
    end;
  end;

  procedure InitTaskItem(ATask: TMediaTask; var pTaskItem: TTaskItem);
  var
    pEditParams: TMediaTaskEditParams;
  begin
    FillChar(pTaskItem, SizeOf(pTaskItem), 0);
    with ATask.MediaSource do
    begin
      WStrPLCopy(pTaskItem.szInputFile, FileName, Length(pTaskItem.szInputFile)-1);
      pTaskItem.VidDecParam := VideoDecParam^;
      pTaskItem.AudDecParam := AudioDecParam^;
      pTaskItem.uDecMode := DecModel;
    end;
    // 裁剪属性
    pTaskItem.dTrimStart := ATask.OutputStart;
    pTaskItem.dTrimEnd := ATask.OutputStart+ATask.OutputLength;
    pTaskItem.dStart := pTaskItem.dTrimStart;
    pTaskItem.dEnd := pTaskItem.dTrimEnd;

    // 编辑属性
    pEditParams:=ATask.EditParams;
    if Assigned(pEditParams) then
    with pEditParams do
    begin
      pTaskItem.Effect.nVolume := Volume;
      pTaskItem.Effect.nBrightness := Brightness;
      pTaskItem.Effect.nContrast := Contrast;
      pTaskItem.Effect.nSaturation := Saturation;
      pTaskItem.Effect.uExFilterID := FilterID;
      pTaskItem.Effect.uFlipID := FlipFlag;
      pTaskItem.Effect.uDeintelace := DeInterlacing;
      pTaskItem.crCrop := CropRect;
      pTaskItem.nAngle := Round(pEditParams.Rotation); // 旋转
      pTaskItem.bDrag := False;    // 不拉伸
      if Mute then // 静音,把音量调到最小
        pTaskItem.Effect.nVolume:=0;
    end;
    // 添加水印参数
    InitTaskUserWatermarkParams(pEditParams, pTaskItem);

    // 添加外挂字幕参数
    InitTaskSubPicParams(pEditParams, pTaskItem);
  end;

  procedure InitMainTaskItem(ATask: TMediaTask; var pTaskItem: TTaskItem);
  var
    pEncodeParams: TMediaTaskEncodeParams;
    pEditParams: TMediaTaskEditParams;
  begin
    // 输出文件名, 以第一个项为准
    WStrPLCopy(pTaskItem.szOutputFile, ATask.FullOutFileName, Length(pTaskItem.szOutputFile)-1);
    // 编码属性
    pEncodeParams:=ATask.EncodeParams;
    if Assigned(pEncodeParams) then
    with pEncodeParams do
    begin
      pTaskItem.EncParam:=EncParam^;
      // 添加版权水印
      InitCopyrightWatermark(pEncodeParams, pTaskItem);
    end;
    pEditParams:=ATask.EditParams;
    if Assigned(pEditParams) then
      pTaskItem.EncParam.Video.nResizeStyle:=pEditParams.ResizeStyle;
  end;

  procedure AddMergeSource(ATask: TMediaTask; ASource: TMediaTaskMergeSource);
  var
    pChildTask: TMediaTask;
    pTaskItem: TTaskItem;
  begin
    for pChildTask in ASource.TaskList do
    begin
      InitTaskItem(pChildTask, pTaskItem);
      InitMainTaskItem(ATask, pTaskItem); // 输出和编码参数以主任务为准
      // 下面这一行必须放在最后, 使用子任务自己的缩放方式
      pTaskItem.EncParam.Video.nResizeStyle:=pChildTask.EditParams.ResizeStyle;// 每个子任务使用自己的缩放方式
      FEncoder.AddTaskItem(pTaskItem);
    end;
  end;

  procedure AddNormalTaskItem(ATask: TMediaTask);
  var
    pTaskItem: TTaskItem;
    pSource: TMediaTaskMediaSource;
  begin
    pSource:=ATask.MediaSource;
    if pSource is TMediaTaskMergeSource then
      AddMergeSource(ATask, TMediaTaskMergeSource(pSource))
    else
    begin
      InitTaskItem(ATask, pTaskItem);
      InitMainTaskItem(ATask, pTaskItem); // 输出和编码参数以主任务为准
      FEncoder.AddTaskItem(pTaskItem);
    end;
  end;

begin
  FreeAndNil(FEncoder);
  FEncoder:=TMediaBatchEncoder.Create;
  FEncoder.OnEncodeProgress:=DoEncodeProgress;
  AddNormalTaskItem(Task);
  Result:=True;
end;

function TMediaParallelEncodeTask.Pause: Boolean;
begin
  if Assigned(FEncoder) then
  begin
    FEncoder.Pause;
    Result:=True;
  end else
    Result:=False;
end;

function TMediaParallelEncodeTask.Resume: Boolean;
begin
  if Assigned(FEncoder) then
  begin
    FEncoder.Resume;
    Result:=True;
  end else
    Result:=False;
end;

function TMediaParallelEncodeTask.Start(hMsgWnd: HWnd): Boolean;
begin
  Result:=False;
  FEncodedLength:=0;
  if InitEncoder then
  begin
    Result:=FEncoder.Run(hMsgWnd, FEncoder.GetTaskCount>1);
  end;
end;

function TMediaParallelEncodeTask.Stop: Boolean;
begin
  if Assigned(FEncoder) then
  begin
    FEncoder.Abort();
    Result:=True;
  end else
    Result:=False;
end;

{ TMediaParallelEncodeTaskList }

constructor TMediaParallelEncodeTaskList.Create(AOwner: TMediaTaskMgr);
begin
  inherited Create(AOwner);
  TasksToEncodeList;
end;

destructor TMediaParallelEncodeTaskList.Destroy;
begin
  Stop;
  inherited;
end;

function TMediaParallelEncodeTaskList.DoEncodeProgress(Sender: TMediaParallelEncodeTask;
  EncodingState: TMediaEncodingState;
  AProgress, AProgressMax: Integer; AProgressInfo: PMediaProgressInfo): Boolean;
begin
  case EncodingState of
    mesFinished, mesUserAbort,
    mesCallBackAbort, mesError: Dec(FActiveTaskCount);  // 减少活动任务计算
  end;
  if Assigned(FOnEncodeProgress) then
    Result:=FOnEncodeProgress(Sender, EncodingState, AProgress, AProgressMax,
      AProgressInfo)
  else
    Result:=False;
end;

function TMediaParallelEncodeTaskList.EncodeNext(hMsgWnd: HWnd): Boolean;
var
  i: Integer;
  pTask: TMediaParallelEncodeTask;
begin
  Result:=False;
  if FActiveTaskCount<FParallelNum then
  for i:=FEncodingTaskOrd to Count-1 do
  begin
    pTask:=TMediaParallelEncodeTask(Items[i]);
    Result:=pTask.Start(hMsgWnd);
    if Result then
    begin
      Inc(FEncodingTaskOrd);
      Inc(FActiveTaskCount);  // 增加活动任务计数
      if FActiveTaskCount>=FParallelNum then
        Exit;
    end;
  end;
end;

function TMediaParallelEncodeTaskList.GetEncodedLength: Double;
var
  pTask: TMediaEncodeTask;
begin
  Result:=0;
  for pTask in Self do
  begin
    Result:=Result+TMediaParallelEncodeTask(pTask).FEncodedLength;
  end;
end;

function TMediaParallelEncodeTaskList.Pause: Boolean;
var
  pTask: TMediaEncodeTask;
begin
  for pTask in Self do
  begin
    pTask.Pause;
  end;
  Result:=True;
end;

function TMediaParallelEncodeTaskList.Resume: Boolean;
var
  pTask: TMediaEncodeTask;
begin
  for pTask in Self do
  begin
    pTask.Resume;
  end;
  Result:=True;
end;

function TMediaParallelEncodeTaskList.Stop: Boolean;
var
  pTask: TMediaEncodeTask;
begin
  for pTask in Self do
  begin
    pTask.Stop;
  end;
  Result:=True;
  FEncodingTaskOrd:=0;
  FActiveTaskCount:=0;
end;

procedure TMediaParallelEncodeTaskList.TasksToEncodeList;

  procedure AddTaskToEncodeList(ATask: TMediaTask);
  var
    pChildTask: TMediaTask;
    pEncodeTask: TMediaEncodeTask;
  begin
    if not ATask.SkipEncode then
    begin
      pEncodeTask:=TMediaParallelEncodeTask.Create(Self, ATask);
      Self.Add(pEncodeTask);
    end;
    for pChildTask in ATask.GetChildTasks do
    begin
      AddTaskToEncodeList(pChildTask);
    end;
  end;

var
  pTask: TMediaTask;
begin
  for pTask in Owner.Tasks do
  begin
    AddTaskToEncodeList(pTask);
  end;
end;

end.
