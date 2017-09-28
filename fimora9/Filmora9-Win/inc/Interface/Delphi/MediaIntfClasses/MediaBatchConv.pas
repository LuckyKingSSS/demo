{*******************************************************}
{       Wondershare独立进程批量转换接口                 }
{       创建人:　 凌曙光　　 　　　　　　　   　　　　  }
{       创建日期: 2009/07/07                            }
{       修订人:                                         }
{       修订日期:                                       }
{*******************************************************}
{$Z4}  // 4字节枚举

unit MediaBatchConv;

interface

uses Windows, Messages, SysUtils, Classes, MediaCommon, MediaDecode, MediaProcess,
  MediaEncodeMgr, MediaPlay, MediaEncode, WSSubPic;

const
  WSBatchConvDLL = 'WS_BatchConvProc.dll';

//分割模式定义
  SPLIT_BASE_ON_NONE = 0;
  SPLIT_BASE_ON_TIME = 1;
  SPLIT_BASE_ON_SIZE = 2;

  USEData_Buffer_SUBSTYLE = $A1;

//------------------------------------------------------------------------------
  UM_CONVERT_PROGRESS_WRAP = (WM_APP + 930);
// HIWORD(wParam) 
// LOWROD(wParam) -  0 进度  lParam - 进度(LOWORD:总进度 HIWORD:当前任务进度 0-100)  详细信息请使用 BCGetProgressInfo	
//           1 转换完成  wParam - 0 正常完成  1 用户终止 2 外部回掉终止 3 遭遇错误
//         100 第二次进度开始 
//           101 进度消息  lParam - 进度(0-100)
//         110 第二次进度结束 在第二次进度期间 不能调用WEPause和WEStop函数
//   1000 开始新任务

  UM_CONVERT_END_WRAP    = (WM_APP + 931);
  UM_CONVERT_ERROR_WRAP  = (WM_APP + 932);

type
// Effect设置
  TEffectData = record
    nVolume: Longint;               // 0 - 200
    nBrightness: Longint;           // -100～100
    nContrast: Longint;             // -100～100
    nSaturation: Longint;           // -100～100
    uExFilterID: DWORD;             // 滤镜ID
    uFlipID: DWORD;                 // 翻转ID
    uDeintelace: DEINTERLACE_STYLE; // 去除隔行扫描方式
  end;

  TUserWatermarkParam = MediaProcess.TUserWatermarkParam;

  BATCHCONVERTER = record end;
  HBATCHCONVERTER = ^BATCHCONVERTER;

  TConvertState = ( // 转换状态
    CS_NONE,      //
    CS_RUNNING,   // 正在转换
    CS_COMPLETE,  // 成功转换
    CS_ERROR      // 遭遇错误
  );

  // 批量转换专用外挂字幕样式结构
  TUserSubPicForBC = packed record
    nDummy1: Byte;                 // 为了兼容保留的一个字节 nDummy1 = A1
    uSubPicID: UINT;               // 字幕ID
    Style: TSubPicStyle;           // 字幕样式
    nOpacity: UINT;                // 透明度 0--100, 0=完全透明,100表示完全不透明
    pSubPicPath: array[0..MAX_PATH-1] of WideChar; // 字幕路径,当ID=0时,指定字幕路径
    nDummy2: array[0..2] of Byte;  // 为了对齐保留的三个字节, SizeOf(nDummy1)+SizeOf(nDummy2)=SizeOf(DWORD)
  end;
  PUserSubPicForBC = ^TUserSubPicForBC;

  PTaskItem = ^TTaskItem;
  TTaskItem = record
    szInputFile: array [0..511] of WideChar;   //输入文件名
    VidDecParam : TVideoDecParam;    // 视频解码参数
    AudDecParam : TAudioDecParam;    // 音频解码参数
    Effect : TEffectData;       // 特效参数
    crCrop : TRECT;             // 视频减切区域
    Watermark : TWatermarkParam;// 水印
    UserWatermark: TUserWatermarkParam; // 用户自定义水印
    // Slide Time 片断在时间线上的时间
    dStart : double;            // 相对输出（音）视频的起止时间，单位：秒，若 剪切长度 != 输出长度，则会产生拉伸效果
    dEnd : double;              // 此值为零则无拉伸效果
    // Slide Time 为时间线上的时间
    dTrimStart : double;        // 减切开始时间（文件绝对时间）
    dTrimEnd : double;          // 减切结束时间（文件绝对时间）视频连接时必须填写正确的时间

    uDecMode : DEC_MODEL;
    EncParam : TEncodeParam;
    szOutputFile : array [0..511] of WideChar;  // 输出文件名, 路径全名
    nConvState : TConvertState;
    dwUserData : DWORD;          // 用户数据
    dwSplitMode: DWORD;          // 分割模式 见分割模式定义
    dwSplitSize: DWORD;          // 分割模式下的大小，SPLIT_BASE_ON_TIME时单位：毫秒，SPLIT_BASE_ON_SIZE时单位：kb
    UserSubPic: TUserSubPicForBC; // 外挂字幕
    nAngle     : UINT;           // 旋转角度 0-270, 逆时针方向
    bDrag      : BOOL;           // 旋转后是否拉伸 默认False
    pUseData: array[0..1023-SizeOf(UINT)-SizeOf(BOOL)-SizeOf(TUserSubPicForBC)] of Byte;       // 备用数据，为应用扩展需要，使用该字段时需要定义第一个字节类型
  end;

  TConvertProgressInfo = packed record
    dTotalLength        : double;   // 影片总时间长度
    dTotalTransedLength : double;   // 总任务已转换影片时间
    dCurrTime           : double;   // 当前时间
    nPercent            : Longint;  // 总任务百分比（0 - 100）
    nCurrentTaskPercent : Longint;  // 当前任务进度百分比
    nPercentPercent     : Longint;  // 总任务万分比（0 - 10000）
    nCurrentTaskPercentPercent : Longint; // 当前任务进度万分比
    nCurrentTaskIndex   : Longint;  // 当前任务索引号, 从０开始
    nTaskCount          : Longint;  // 总任务数?
    tmPrepaid           : DWORD;    // 已用时间 单位：秒
    tmRemain            : DWORD;    // 剩余时间 单位：秒
    dCurrTransedTime    : double;   // 当前任务已转换影片时间
    dCurrLength         : double;   // 当前任务影片长度
    dTransRate          : double;   // 转换速率，单位：每秒转换的片断长度
    nFramesPerSecond    : Longint;  // 转换速率，单位：每秒处理的帧数
    nEnocdeFramesPerSecond : Longint; // 每秒钟编码的视频帧数量
    dwConvState         : TConvertState; // 转换状态
  end;

  PConvertProgressInfo = ^TConvertProgressInfo;

  TInitTaskItem = procedure (const pTask: TTaskItem); stdcall;
  TInitEncParam = procedure (const pEncParam: TEncodeParam); stdcall;

  TBCCreate = function (): HBATCHCONVERTER; stdcall;
  TBCDestroy = procedure (hBatchConv: HBATCHCONVERTER); stdcall;
  TBCAddTaskItem = procedure (hBatchConv: HBATCHCONVERTER; const pTask: TTaskItem); stdcall;
  TBCGetTaskCount = function (hBatchConv: HBATCHCONVERTER): Integer; stdcall;
  TBCGetTaskItem = function (hBatchConv: HBATCHCONVERTER; nIndex: Integer; var pTask: TTaskItem): BOOL; stdcall;
  TBCSetAudioLimitLength = procedure (hBatchConv: HBATCHCONVERTER; nLimitLength: Integer); stdcall;
  TBCSetWatermarkImage = procedure (hBatchConv: HBATCHCONVERTER; lpbi: PBITMAPINFO;
    lpBits: PBYTE; pRect: PRECT = nil; nOpacity: Integer = 100); stdcall;
  // 开始转换, 返回进程ID
  TBCRun = function (hBatchConv: HBATCHCONVERTER; hWnd: HWND = 0; bMerge: BOOL = False): Integer; stdcall;
  TBCPause = function (hBatchConv: HBATCHCONVERTER): Integer; stdcall;
  TBCResume = function (hBatchConv: HBATCHCONVERTER): Integer; stdcall;
  TBCWait = function (hBatchConv: HBATCHCONVERTER; uMilliSecondsTimeOut: UINT = $FFFFFFFF): DWORD; stdcall;
  TBCAbort = function (hBatchConv: HBATCHCONVERTER; wParam: WPARAM = 0; lParam: LPARAM = 0): Integer; stdcall;
  // 初始化预览，返回的句柄由 HBATCHCONVERTER私有,不需要手工释放
  TBCInitPreview = function (hBatchConv: HBATCHCONVERTER; hParentWnd: HWND; const pRcPos: TRECT): HEXPORTVIEWER; stdcall;
  TBCShowPreview = procedure (hBatchConv: HBATCHCONVERTER; bShow: BOOL = True); stdcall;
  TBCEnablePreview = procedure (hBatchConv: HBATCHCONVERTER; bEnable: BOOL = True); stdcall;
  TBCGetProgressInfo = procedure (hBatchConv: HBATCHCONVERTER; const AProgressInfo: TConvertProgressInfo); stdcall;

  TBatchConvAPI = record
  private
    FHandle: THandle;
    FInitTaskItem: TInitTaskItem;
    FInitEncParam: TInitEncParam;
    FBCCreate: TBCCreate;
    FBCDestroy: TBCDestroy;
    FBCAddTaskItem: TBCAddTaskItem;
    FBCGetTaskCount: TBCGetTaskCount;
    FBCGetTaskItem: TBCGetTaskItem;
    FBCSetAudioLimitLength: TBCSetAudioLimitLength;
//    FBCSetWatermarkImage: TBCSetWatermarkImage;
    FBCRun: TBCRun;
    FBCPause: TBCPause;
    FBCResume: TBCResume;
    FBCWait: TBCWait;
    FBCAbort: TBCAbort;
    FBCInitPreview: TBCInitPreview;
    FBCShowPreview: TBCShowPreview;
//    FBCEnablePreview: TBCEnablePreview;
    FBCGetProgressInfo: TBCGetProgressInfo;
  public
    function Init: Boolean;
    procedure Free;

    procedure InitTaskItem(var pTask: TTaskItem);
    procedure InitEncParam(var pEncParam: TEncodeParam);

    function BCCreate(): HBATCHCONVERTER;
    procedure BCDestroy(hBatchConv: HBATCHCONVERTER);
    // 任务管理
    procedure BCAddTaskItem(hBatchConv: HBATCHCONVERTER; const pTask: TTaskItem);
    function BCGetTaskItem(hBatchConv: HBATCHCONVERTER; nIndex: Integer; var pTask: TTaskItem): BOOL;
    function BCGetTaskCount(hBatchConv: HBATCHCONVERTER): Integer;
    procedure BCSetAudioLimitLength(hBatchConv: HBATCHCONVERTER; nLimitLength: Integer);
//    procedure BCSetWatermarkImage(hBatchConv: HBATCHCONVERTER; lpbi: PBITMAPINFO;
//      lpBits: PBYTE; pRect: PRECT = nil; nOpacity: Integer = 100);      不能用
    // 转换状态控制
    function BCRun(hBatchConv: HBATCHCONVERTER; hWnd: HWND = 0; bMerge: BOOL = False): Integer;
    function BCPause(hBatchConv: HBATCHCONVERTER): Integer;
    function BCResume(hBatchConv: HBATCHCONVERTER): Integer;
    function BCWait(hBatchConv: HBATCHCONVERTER; uMilliSecondsTimeOut: UINT = $FFFFFFFF): DWORD;
    function BCAbort(hBatchConv: HBATCHCONVERTER; wParam: WPARAM = 0; lParam: LPARAM = 0): Integer;
    procedure BCGetProgressInfo(hBatchConv: HBATCHCONVERTER; var ProgressInfo: TConvertProgressInfo);
    // 预览控制
    function BCInitPreview(hBatchConv: HBATCHCONVERTER; hParentWnd: HWND; const pRcPos: TRECT): HEXPORTVIEWER;
    procedure BCShowPreview(hBatchConv: HBATCHCONVERTER; bShow: BOOL = TRUE);
//    procedure BCEnablePreview(hBatchConv: HBATCHCONVERTER; bEnable: BOOL = True);   不能用

    property Handle: THandle read FHandle;
  end;

  TMediaBatchEncoder = class;

  TBatchEncodeProgressEvent = function (Sender: TMediaBatchEncoder; EncodingState: TMediaEncodingState;
    AProgress, AProgressMax: Integer; AProgressInfo: PMediaProgressInfo): Boolean of object;

  TMediaBatchEncoder = class
  private
    FHandle: HBATCHCONVERTER;
    FMsgWnd: HWnd;
    FProcessId: Integer;
    FOnEncodeProgress: TBatchEncodeProgressEvent;
    FCallBackWnd: HWND;
    procedure InitCallBackWnd;
    procedure ReleaseCallBackWnd;
    procedure CallBackWndProc(var Message: TMessage);
  public
    constructor Create;
    destructor Destroy; override;

    class procedure InitTaskItem(var pTask: TTaskItem);
    class procedure InitEncParam(var pEncParam: TEncodeParam);
    // 任务管理
    procedure AddTaskItem(const pTask: TTaskItem);
    function GetTaskItem(nIndex: Integer; var pTask: TTaskItem): BOOL;
    function GetTaskCount(): Integer;
    procedure SetAudioLimitLength(nLimitLength: Integer);
//    procedure SetWatermarkImage(lpbi: PBITMAPINFO;
//      lpBits: PBYTE; pRect: PRECT = nil; nOpacity: Integer = 100);   不能用
    // 转换状态控制
    function Run(hMsgWnd: HWND = 0; bMerge: BOOL = False): BOOL;
    function Pause(): Integer;
    function Resume(): Integer;
    function Wait(uMilliSecondsTimeOut: UINT = $FFFFFFFF): DWORD;
    function Abort(wParam: WPARAM = 0; lParam: LPARAM = 0): Integer;
    procedure GetProgressInfo(var ProgressInfo: TConvertProgressInfo);
    // 预览控制
    function InitPreview(hParentWnd: HWND; const pRcPos: TRECT): HEXPORTVIEWER;
    procedure ShowPreview(bShow: BOOL = True);
//    procedure EnablePreview(bEnable: BOOL = True);    不能用

    property Handle: HBATCHCONVERTER read FHandle;
    property TaskCount: Integer read GetTaskCount;
    property OnEncodeProgress: TBatchEncodeProgressEvent read FOnEncodeProgress write FOnEncodeProgress;
  end;

var
  BatchConvAPI: TBatchConvAPI;

implementation

const
  sInitTaskItem = 'InitTaskItem';
  sInitEncParam = 'InitEncParam';
  sBCCreate = 'BCCreate';
  sBCDestroy = 'BCDestroy';
  sBCInitPreview = 'BCInitPreview';
  sBCAddTaskItem = 'BCAddTaskItem';
  sBCGetTaskCount = 'BCGetTaskCount';
  sBCGetTaskItem = 'BCGetTaskItem';
  sBCSetAudioLimitLength = 'BCSetAudioLimitLength';
  sBCSetWatermarkImage = 'BCSetWatermarkImage';
  sBCRun = 'BCRun';
  sBCPause = 'BCPause';
  sBCResume = 'BCResume';
  sBCWait = 'BCWait';
  sBCAbort = 'BCAbort';
  sBCShowPreview = 'BCShowPreview';
  sBCEnablePreview = 'BCEnablePreview';
  sBCGetProgressInfo = 'BCGetProgressInfo';

{ TBatchConvAPI }

function TBatchConvAPI.BCAbort(hBatchConv: HBATCHCONVERTER; wParam: WPARAM;
  lParam: LPARAM): Integer;
begin
  if Assigned(FBCAbort) then
    Result:=FBCAbort(hBatchConv, wParam, lParam)
  else
    Result:=0;
end;

procedure TBatchConvAPI.BCAddTaskItem(hBatchConv: HBATCHCONVERTER;
  const pTask: TTaskItem);
begin
  if Assigned(FBCAddTaskItem) then
    FBCAddTaskItem(hBatchConv, pTask);
end;

function TBatchConvAPI.BCCreate: HBATCHCONVERTER;
begin
  if Assigned(FBCCreate) then
    Result:=FBCCreate()
  else
    Result:=nil;
end;

procedure TBatchConvAPI.BCDestroy(hBatchConv: HBATCHCONVERTER);
begin
  if Assigned(FBCDestroy) then
    FBCDestroy(hBatchConv);
end;

//procedure TBatchConvAPI.BCEnablePreview(hBatchConv: HBATCHCONVERTER;
//  bEnable: BOOL);
//begin
//  if Assigned(FBCEnablePreview) then
//    BCEnablePreview(hBatchConv, bEnable);
//end;

procedure TBatchConvAPI.BCGetProgressInfo(hBatchConv: HBATCHCONVERTER;
  var ProgressInfo: TConvertProgressInfo);
begin
  if Assigned(FBCGetProgressInfo) then
    FBCGetProgressInfo(hBatchConv, ProgressInfo);
end;

function TBatchConvAPI.BCGetTaskCount(hBatchConv: HBATCHCONVERTER): Integer;
begin
  if Assigned(FBCGetTaskCount) then
    Result:=FBCGetTaskCount(hBatchConv)
  else
    Result:=0;
end;

function TBatchConvAPI.BCGetTaskItem(hBatchConv: HBATCHCONVERTER;
  nIndex: Integer; var pTask: TTaskItem): BOOL;
begin
  if Assigned(FBCGetTaskItem) then
    Result:=FBCGetTaskItem(hBatchConv, nIndex, pTask)
  else
    Result:=False;
end;

function TBatchConvAPI.BCInitPreview(hBatchConv: HBATCHCONVERTER;
  hParentWnd: HWND; const pRcPos: TRECT): HEXPORTVIEWER;
begin
  if Assigned(FBCInitPreview) then
    Result:=FBCInitPreview(hBatchConv, hParentWnd, pRcPos)
  else
    Result:=nil;
end;

function TBatchConvAPI.BCPause(hBatchConv: HBATCHCONVERTER): Integer;
begin
  if Assigned(FBCPause) then
    Result:=FBCPause(hBatchConv)
  else
    Result:=0;
end;

function TBatchConvAPI.BCResume(hBatchConv: HBATCHCONVERTER): Integer;
begin
  if Assigned(FBCResume) then
    Result:=FBCResume(hBatchConv)
  else
    Result:=0;
end;

function TBatchConvAPI.BCRun(hBatchConv: HBATCHCONVERTER; hWnd: HWND;
  bMerge: BOOL): Integer;
begin
  if Assigned(FBCRun) then
    Result:=FBCRun(hBatchConv, hWnd, bMerge)
  else
    Result:=0;
end;

procedure TBatchConvAPI.BCSetAudioLimitLength(hBatchConv: HBATCHCONVERTER;
  nLimitLength: Integer);
begin
  if Assigned(FBCSetAudioLimitLength) then
    FBCSetAudioLimitLength(hBatchConv, nLimitLength);
end;

//procedure TBatchConvAPI.BCSetWatermarkImage(hBatchConv: HBATCHCONVERTER;
//  lpbi: PBITMAPINFO; lpBits: PBYTE; pRect: PRECT; nOpacity: Integer);
//begin
//  if Assigned(FBCSetWatermarkImage) then
//    FBCSetWatermarkImage(hBatchConv, lpbi, lpBits, pRect, nOpacity);
//end;

procedure TBatchConvAPI.BCShowPreview(hBatchConv: HBATCHCONVERTER; bShow: BOOL);
begin
  if Assigned(FBCShowPreview) then
    FBCShowPreview(hBatchConv, bShow);
end;

function TBatchConvAPI.BCWait(hBatchConv: HBATCHCONVERTER;
  uMilliSecondsTimeOut: UINT): DWORD;
begin
  if Assigned(FBCWait) then
    Result:=FBCWait(hBatchConv, uMilliSecondsTimeOut)
  else
    Result:=0;
end;

procedure TBatchConvAPI.Free;
begin
  if FHandle <> 0 then
  begin
    FreeLibrary(FHandle);
    FHandle := 0;
    FillChar(Self, SizeOf(Self), 0);
  end;
end;

function TBatchConvAPI.Init: Boolean;
begin
  if FHandle=0 then
  begin
    FHandle := LoadLibrary(WSBatchConvDLL);
    if FHandle <> 0 then
    begin
      FInitTaskItem := GetProcAddress(FHandle, (sInitTaskItem));
      FInitEncParam := GetProcAddress(FHandle, (sInitEncParam));
      FBCCreate := GetProcAddress(FHandle, (sBCCreate));
      FBCDestroy := GetProcAddress(FHandle, (sBCDestroy));
      FBCInitPreview := GetProcAddress(FHandle, (sBCInitPreview));
      FBCAddTaskItem := GetProcAddress(FHandle, (sBCAddTaskItem));
      FBCGetTaskCount := GetProcAddress(FHandle, (sBCGetTaskCount));
      FBCGetTaskItem := GetProcAddress(FHandle, (sBCGetTaskItem));
      FBCSetAudioLimitLength := GetProcAddress(FHandle, (sBCSetAudioLimitLength));
//      FBCSetWatermarkImage := GetProcAddress(FHandle, (sBCSetWatermarkImage));
      FBCRun := GetProcAddress(FHandle, (sBCRun));
      FBCPause := GetProcAddress(FHandle, (sBCPause));
      FBCResume := GetProcAddress(FHandle, (sBCResume));
      FBCWait := GetProcAddress(FHandle, (sBCWait));
      FBCAbort := GetProcAddress(FHandle, (sBCAbort));
      FBCShowPreview := GetProcAddress(FHandle, (sBCShowPreview));
//      FBCEnablePreview := GetProcAddress(FHandle, (sBCEnablePreview));
      FBCGetProgressInfo := GetProcAddress(FHandle, (sBCGetProgressInfo));
    end else
      RaiseLoadLibError(SEncMgrAPI);
  end;
  Result:=FHandle<>0;
end;

procedure TBatchConvAPI.InitEncParam(var pEncParam: TEncodeParam);
begin
  if Assigned(FInitEncParam) then
    FInitEncParam(pEncParam);
end;

procedure TBatchConvAPI.InitTaskItem(var pTask: TTaskItem);
begin
  if Assigned(FInitTaskItem) then
    FInitTaskItem(pTask);
end;

{ TBatchConv }

function TMediaBatchEncoder.Abort(wParam: WPARAM; lParam: LPARAM): Integer;
begin
  Result:=BatchConvAPI.BCAbort(FHandle, wParam, lParam);
  FProcessId:=0;
end;

procedure TMediaBatchEncoder.AddTaskItem(const pTask: TTaskItem);
begin
  BatchConvAPI.BCAddTaskItem(FHandle, pTask);
end;

procedure TMediaBatchEncoder.CallBackWndProc(var Message: TMessage);
// wParam -  0 进度  lParam - 进度(LOWORD:总进度 HIWORD:当前任务进度 0-100)  详细信息请使用 BCGetProgressInfo
//           1 转换完成  wParam - 0 正常完成  1 用户终止 2 外部回掉终止 3 遭遇错误
//         100 第二次进度开始
//           101 进度消息  lParam - 进度(0-100)
//         110 第二次进度结束 在第二次进度期间 不能调用WEPause和WEStop函数
//    1000 开始新任务
var
  nProg, nProgMax: Integer;
  EncodingState: TMediaEncodingState;

  procedure SendCallBack;
//  var
//    ProgressInfo: TConvertProgressInfo;
  begin
    if Assigned(FOnEncodeProgress) then
    begin
//      FillChar(ProgressInfo, SizeOf(ProgressInfo), 0);
//      GetProgressInfo(ProgressInfo); // 现在很慢，不要调它
      FOnEncodeProgress(Self, EncodingState, nProg, nProgMax, nil);
    end;
    if FMsgWnd<>0 then
    with Message do
      SendMessage(FMsgWnd, Msg, WParam, LParam);
  end;

begin
  with Message do
  begin
    nProg:=0;
    nProgMax:=0;
    case Msg of
      UM_CONVERT_PROGRESS_WRAP:
        begin
          EncodingState := mesEncoding;
          nProg:=LParamLo;
          nProgMax:=10000;
          SendCallBack;
        end;
      UM_CONVERT_END_WRAP:  // 转换结束
        begin
          EncodingState := mesFinished;
          SendCallBack;
        end;
      UM_CONVERT_ERROR_WRAP:  // 转换错误
        begin
          EncodingState := mesError;
          SendCallBack;
        end
    else
      Result := DefWindowProc(FCallBackWnd, Msg, wParam, lParam);
    end;
  end;
end;

constructor TMediaBatchEncoder.Create;
begin
  inherited Create;
  BatchConvAPI.Init;
  FHandle:=BatchConvAPI.BCCreate();
end;

destructor TMediaBatchEncoder.Destroy;
begin
  BatchConvAPI.BCDestroy(FHandle);
  ReleaseCallBackWnd;
  inherited;
end;

//procedure TMediaBatchEncoder.EnablePreview(bEnable: BOOL);
//begin
//  BatchConvAPI.BCEnablePreview(FHandle, bEnable);
//end;

procedure TMediaBatchEncoder.GetProgressInfo(var ProgressInfo: TConvertProgressInfo);
begin
  BatchConvAPI.BCGetProgressInfo(FHandle, ProgressInfo);
end;

function TMediaBatchEncoder.GetTaskCount: Integer;
begin
  Result:=BatchConvAPI.BCGetTaskCount(FHandle);
end;

function TMediaBatchEncoder.GetTaskItem(nIndex: Integer; var pTask: TTaskItem): BOOL;
begin
  Result:=BatchConvAPI.BCGetTaskItem(FHandle, nIndex, pTask);
end;

procedure TMediaBatchEncoder.InitCallBackWnd;
begin
  if FCallBackWnd=0 then
  begin
    FCallBackWnd:=Classes.AllocateHWnd(CallBackWndProc);
  end;
end;

class procedure TMediaBatchEncoder.InitEncParam(var pEncParam: TEncodeParam);
begin
  BatchConvAPI.Init;
  BatchConvAPI.InitEncParam(pEncParam);
end;

function TMediaBatchEncoder.InitPreview(hParentWnd: HWND;
  const pRcPos: TRECT): HEXPORTVIEWER;
begin
  Result:=BatchConvAPI.BCInitPreview(FHandle, hParentWnd, pRcPos);
end;

class procedure TMediaBatchEncoder.InitTaskItem(var pTask: TTaskItem);
begin
  BatchConvAPI.Init;
  BatchConvAPI.InitTaskItem(pTask);
end;

function TMediaBatchEncoder.Pause: Integer;
begin
  Result:=BatchConvAPI.BCPause(FHandle);
end;

procedure TMediaBatchEncoder.ReleaseCallBackWnd;
begin
  if FCallBackWnd<>0 then
  begin
    Classes.DeallocateHWnd(FCallBackWnd);
    FCallBackWnd:=0;
  end;
end;

function TMediaBatchEncoder.Resume: Integer;
begin
  Result:=BatchConvAPI.BCResume(FHandle);
end;

function TMediaBatchEncoder.Run(hMsgWnd: HWnd; bMerge: BOOL): BOOL;
begin
  InitCallBackWnd;
  FProcessId:=BatchConvAPI.BCRun(FHandle, FCallBackWnd, bMerge);
  Result:=FProcessId<>0;
end;

procedure TMediaBatchEncoder.SetAudioLimitLength(nLimitLength: Integer);
begin
  BatchConvAPI.BCSetAudioLimitLength(FHandle, nLimitLength);
end;

//procedure TMediaBatchEncoder.SetWatermarkImage(lpbi: PBITMAPINFO; lpBits: PBYTE;
//  pRect: PRECT; nOpacity: Integer);
//begin
//  BatchConvAPI.BCSetWatermarkImage(FHandle, lpbi, lpBits, pRect, nOpacity);
//end;

procedure TMediaBatchEncoder.ShowPreview(bShow: BOOL);
begin
  BatchConvAPI.BCShowPreview(FHandle, bShow);
end;

function TMediaBatchEncoder.Wait(uMilliSecondsTimeOut: UINT): DWORD;
begin
  Result:=BatchConvAPI.BCWait(FHandle, uMilliSecondsTimeOut);
end;

initialization
finalization
  BatchConvAPI.Free;

end.
