unit SWFRecoder;

interface

uses
  Windows, Messages, SysUtils, MediaCommon, MediaEncodeMgr;

const
  SFlashRecord_API = 'WS_FlashRecoder.dll';
const
  CM_RECORDEND = WM_USER + 777;

type
  PSWFRecThreadData = ^TSWFRecThreadData;
  TSWFRecThreadData = record
    FilePath      : WideString; //源文件路径
    TotalFrame    : Integer;    //源文件总帧率
    hMainHand     : HWND;       //主窗口句柄
    hParent       : HWND;       //播放器父窗口句柄
    hViewer       : HWND;
    Rect          : TRect;      //播放窗口RECT
    ViewerRect    : TRect;
    FlashScript   : WideString; //播入脚本路径
    IsAutoRecord  : Boolean;    //是否自动录制
  end;

  TSWFRecData = TSWFRecThreadData;
  PSWFRecData = PSWFRecThreadData;

  TFSCommandEvent = procedure (const cmd: WideString; const args: WideString) of object;
  
  //初始化数据
  TFRCreate = procedure (const SWFRecData : TSWFRecData); stdcall;
  //销毁数据
  TFRDestroy = procedure (); stdcall;
  //导入视频文件输出路径
  TFRSetVideoFileName = procedure (const pVFilePath: PWideChar); stdcall;
  //导入音频文件输出路径
  TFRSetAudioFileName = procedure (const pAFilePath: PWideChar); stdcall;
  //设置文件输出路径
  TFRSetOutputFileName = procedure (const pOutputFilePath: PWideChar); stdcall;
  //设置编码参数
  TFRSetEncParam = procedure (const pEncParam: PENCODEPARAM); stdcall;
  //设置解码参数
  TFRSetDecParam = procedure (VideoInfo: PWSVideoInfo; AudioInfo: PWSAudioInfo); stdcall;
  //设置是否为自动录制
  TFRSetAutoRecord = procedure (IsAutoRecord : Boolean); stdcall;
  TFRSetFSCommand = procedure (value: TFSCommandEvent); stdcall;
  TFRSetIsDVD = procedure (Value: Boolean); stdcall;

type
  TFlashRecorderAPI = record
  private
    FHandle: THandle;
    FFRCreate: TFRCreate;
    FFRDestroy: TFRDestroy;
    FFRSetVideoFileName: TFRSetVideoFileName;
    FFRSetAudioFileName: TFRSetAudioFileName;
    FFRSetOutputFileName: TFRSetOutputFileName;
    FFRSetEncParam: TFRSetEncParam;
    FFRSetDecParam: TFRSetDecParam;
    FFRSetAutoRecord: TFRSetAutoRecord;
    FFRSetFSCommand: TFRSetFSCommand;
    FFRSetIsDVD: TFRSetIsDVD;
  public
    function Init(): Boolean;
    procedure Free;

    procedure FRCreate(const SWFRecData : TSWFRecData);
    procedure FRDestroy();
    procedure FRSetVideoFileName(const pVFilePath: PWideChar);
    procedure FRSetAudioFileName(const pAFilePath: PWideChar);
    procedure FRSetOutputFileName(const pOutputFilePath: PWideChar);
    procedure FRSetEncParam(const pEncParam: PENCODEPARAM);
    procedure FRSetDecParam(VideoInfo: PWSVideoInfo; AudioInfo: PWSAudioInfo);
    procedure FRSetAutoRecord(IsAutoRecord : Boolean);
    procedure FRSetFSCommand(Value: TFSCommandEvent);
    procedure FRSetIsDVD(Value: Boolean);

    property Handle: THandle read FHandle;
  end;

var
  FlashRecorderAPI: TFlashRecorderAPI;

implementation

const
  sFRCreate = 'FRCreate';
  sFRDestroy = 'FRDestroy';
  sFRSetVideoFileName = 'FRSetVideoFileName';
  sFRSetAudioFileName = 'FRSetAudioFileName';
  sFRSetOutputFileName = 'FRSetOutputFileName';
  sFRSetEncParam = 'FRSetEncParam';
  sFRSetDecParam = 'FRSetDecParam';
  sFRSetAutoRecord = 'FRSetAutoRecord';
  sFRSetFSCommand = 'FRSetFSCommand';
  sFRSetIsDVD = 'FRSetIsDVD';

procedure CleanupProc;
begin
  FlashRecorderAPI.Free;
end;

{ TFlashRecorderAPI }

procedure TFlashRecorderAPI.FRCreate(const SWFRecData: TSWFRecData);
begin
  Init;
  if Assigned(FFRCreate) then
    FFRCreate(SWFRecData);
end;

procedure TFlashRecorderAPI.FRDestroy;
begin
  if Assigned(FFRDestroy) then
    FFRDestroy();
end;

procedure TFlashRecorderAPI.Free;
begin
  if FHandle <> 0 then
  begin
    FreeLibrary(FHandle);
    FHandle:=0;
    FillChar(Self, SizeOf(Self), 0);
  end;
end;

procedure TFlashRecorderAPI.FRSetAudioFileName(const pAFilePath: PWideChar);
begin
  if Assigned(FFRSetAudioFileName) then
    FRSetAudioFileName(pAFilePath);
end;

procedure TFlashRecorderAPI.FRSetAutoRecord(IsAutoRecord: Boolean);
begin
  if Assigned(FFRSetAutoRecord) then
    FFRSetAutoRecord(IsAutoRecord);
end;

procedure TFlashRecorderAPI.FRSetDecParam(VideoInfo: PWSVideoInfo;
  AudioInfo: PWSAudioInfo);
begin
  if Assigned(FFRSetDecParam) then
    FFRSetDecParam(VideoInfo, AudioInfo);
end;

procedure TFlashRecorderAPI.FRSetEncParam(const pEncParam: PENCODEPARAM);
begin
  if Assigned(FFRSetEncParam) then
    FFRSetEncParam(pEncParam);
end;

procedure TFlashRecorderAPI.FRSetFSCommand(Value: TFSCommandEvent);
begin
  if Assigned(FFRSetFSCommand) then
    FFRSetFSCommand(Value);
end;

procedure TFlashRecorderAPI.FRSetIsDVD(Value: Boolean);
begin
  if Assigned(FFRSetIsDVD) then
    FFRSetIsDVD(Value);
end;

procedure TFlashRecorderAPI.FRSetOutputFileName(
  const pOutputFilePath: PWideChar);
begin
  if Assigned(FFRSetOutputFileName) then
    FFRSetOutputFileName(pOutputFilePath);
end;

procedure TFlashRecorderAPI.FRSetVideoFileName(const pVFilePath: PWideChar);
begin
  if Assigned(FFRSetVideoFileName) then
    FFRSetVideoFileName(pVFilePath);
end;

function TFlashRecorderAPI.Init: Boolean;
begin
  if FHandle = 0 then
  begin
    FHandle := LoadLibrary(SFlashRecord_API);
    if FHandle <> 0 then
    begin
      AddExitProc(CleanupProc);
      FFRCreate := GetProcAddress(FHandle, (sFRCreate));
      FFRDestroy := GetProcAddress(FHandle, (sFRDestroy));
      FFRSetVideoFileName := GetProcAddress(FHandle, (sFRSetVideoFileName));
      FFRSetAudioFileName := GetProcAddress(FHandle, (sFRSetAudioFileName));
      FFRSetOutputFileName := GetProcAddress(FHandle, (sFRSetOutputFileName));
      FFRSetEncParam := GetProcAddress(FHandle, (sFRSetEncParam));
      FFRSetDecParam := GetProcAddress(FHandle, (sFRSetDecParam));
      FFRSetAutoRecord := GetProcAddress(FHandle, (sFRSetAutoRecord));
      FFRSetFSCommand := GetProcAddress(FHandle, (sFRSetFSCommand));
      FFRSetIsDVD := GetProcAddress(FHandle, (sFRSetIsDVD));
    end else
      RaiseLoadLibError(SFlashRecord_API);
  end;
  Result := FHandle <> 0;
end;

end.
