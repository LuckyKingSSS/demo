;FilmoraScrn之预定义及公用代码文件

#define AppName "Wondershare Filmora Scrn"
#define OldAppName "Wondershare Filmora Scrn"
#define AppPath "Wondershare\Filmora Scrn"
;#define JpAppName "Wondershare Filmora 動画編集"
;#define JpAppPath "Wondershare\Filmora 動画編集"
#define NewExeName "FSRecorder.exe"
;#define ResourceInstallExeName "EffectStore.exe"
;#define ExeName "FilmoraScreen.exe"
#define ProductID "3235"
#define ProductName "Filmora Scrn"
#define ProductShortName "Filmora Scrn"
#define ConfigName "FilmoraScrn.ini"
#define UpdateRegisterConfigName "UpdateRegister.ini"
#define CrashReportConfigName "CrashReport.ini"
;#define RegisterPath "SOFTWARE\Wondershare\Filmora Scrn"

;网址相关
#define UpdatePage "http://cbs.wondershare.com/go.php?m=upgrade_info&pid=%s&version=%s&email=%s"
#define EmailServerURL "http://api.wondershare.com/interface.php?m=smtpinfo"
;Affiliate定制版本之购买链接
#define AgentPage "https://www.regnow.com/softsell/nph-softsell.cgi?item=15969-115&affiliate=%s"

;注册相关...62088607-14102045-12206945
#define SGUID "{{79B718BE-67C9-4B30-A9AD-6BB96ED6AFF7}"
#define RegKeyN "924839719"
#define RegKeyD "19332601"

;Size&Version等
#ifdef CPUX64
#define AppSize FileSize("..\..\bin\x64\Release\FSRecorder.exe");
#define AppVersion GetFileVersion(AddBackslash(SourcePath) + "..\..\bin\x64\Release\FSRecorder.exe")
#else
#define AppSize FileSize("..\..\bin\x86\Release\FSRecorder.exe");
#define AppVersion GetFileVersion(AddBackslash(SourcePath) + "..\..\bin\x86\Release\FSRecorder.exe")
#endif
;互斥信号
#define AppMutex "Wondershare Filmora Scrn"
#define OutputName "Filmora Scrn"
;安装图片
#define WizardImage "WizardImage.bmp"

;注册表工程节点
#define ProjectExt ".wfsproj"
#define ProjNodeName "WFSFile"
;#define ResNodeName "fmrespack"

;防盗版注册键名
#define Publisher "Wondershare"
;************ CBS 平台客户端程序名称
#define CBSHelperAppName "Wondershare Helper Compact"
#define CBSHelperExeName CBSHelperAppName + ".exe"

;以下一项针对实现广告部提醒消息需求的产品
#define CBSHelperShortExeName "WSHelper.exe"

;xp须安装WinBurn Engine
#define IMApiXPSrv "IMAPI_XP_SRV2003_x86.exe"

;代理商定制相关...
;定制配置文件
#define MyAppCustomizedFileName "Customization.xml"
;定制辅助动态库
#define ProductInfoManagerDLL "WSUtilities_Setup.dll"

[code]

const
  CSIDL_PERSONAL     = $0005;
  WM_CLOSE           = $0010;

var
  //是否重新安装
  g_overInstall: Boolean;
  //若是自动升级，取之前注册码
  g_regMail, g_regCode: string;

type
//  TGUID = record
//    D1: LongWord;
//    D2: Word;
//    D3: Word;
//    D4: array[0..7] of Byte;
//  end;
  pchar = pansichar;
  //安装, 卸载, 注册, 在线升级
  TCBSProcessType = (cptInstall, cptUnInstall, cptRegister, cptOnlineUpdate);
  
//代理商定制相关
function InstallCustomized(const ALangName, AExeFileName, AExtractedConfigFile, AInstalledConfigFile,APublisherName,
  AProductName: PAnsichar):BOOL;  external 'InnoInstallProcessAllA@files:{#ProductInfoManagerDLL} stdcall delayload';

function UnInstallCustomized(const ALangName, AInstalledConfigFile, APublisherName,
  AProductName: PAnsichar):BOOL; external 'InnoUnInstallProcessAllA@{app}\{#ProductInfoManagerDLL} stdcall uninstallonly';

function CoCreateGuid(var Guid: TGUID):integer; external 'CoCreateGuid@ole32.dll stdcall';
procedure SHChangeNotify(wEventId: Longint; uFlags: UINT; dwItem1, dwItem2: Longint);
external 'SHChangeNotify@shell32.dll stdcall';

function ShellExecute(hWnd: Integer; Operation, FileName, Parameters,
  Directory: pchar; ShowCmd: Integer): Integer;
external 'ShellExecuteA@shell32.dll stdcall';


//隐藏打开链接之导出函数
function WSOpenURLI(pszURL: PChar; nOpenMode: Integer): Boolean;
external 'WSOpenURLA@files:WSUtilities_Setup.dll stdcall setuponly';
function WSOpenURLU(pszURL: PChar; nOpenMode: Integer): Boolean;
external 'WSOpenURLA@{app}\WSUtilities_Setup.dll stdcall uninstallonly';

function GetFileMD5(const pFileName: PChar; szBuf : array of Char): BOOL;
external 'GetFileMD5@files:WS_Log_Setup.dll stdcall';

function GetFileMD5Ex(const pFileName: PChar): PChar;
external 'GetFileMD5Ex@files:WS_Log_Setup.dll stdcall';

//function PopFileFromReserved(sPatchFile, sOverlayFile: PChar): Boolean;
//external 'PopFileFromReserved@files:WSOverlay.dll stdcall';

//代理商定制工具
function PopFileFromReservedA(const APatchedFileName, AOverlayFileName: String):BOOL; external 'PopFileFromReservedW@files:{#ProductInfoManagerDLL} stdcall delayload';
function LoadCustomizeXMLFileA(const AConfigFileName: PAnsiChar):BOOL; external 'LoadCustomizeXMLFileA@files:{#ProductInfoManagerDLL} stdcall delayload';
function ReadCustomizeBoolInfoA(const AInfoName, ALangName: PAnsiChar):BOOL; external 'ReadCustomizeBoolInfoA@files:{#ProductInfoManagerDLL} stdcall delayload';
function ReadCustomizeInfoA(const AInfoName, ALangName: PAnsichar; AValue: PAnsiChar; AValueLen: integer; AType: PAnsiChar; ATypeLen: integer):BOOL; external 'ReadCustomizeInfoA@files:{#ProductInfoManagerDLL} stdcall delayload';
function ProcessDRCustomizeA(const AConfigFileName, ADRRegSubPath, ADRBuyUrlRegKey: PAnsiChar):BOOL; external 'ProcessDRCustomizeA@files:{#ProductInfoManagerDLL} stdcall delayload';
function CustomizeUpdateProcessA(const ASrcConfigFile, ADstConfigFile: PAnsiChar):BOOL; external 'CustomizeUpdateProcessA@files:{#ProductInfoManagerDLL} stdcall delayload';
function WSOpenURLA(const AURL: PAnsichar; nOpenMode: integer):BOOL; external 'WSOpenURLA@files:{#ProductInfoManagerDLL} stdcall delayload';
function UninstallLoadCustomizeXMLFileA(const AConfigFileName: PAnsiChar):BOOL; external 'LoadCustomizeXMLFileA@{app}\{#ProductInfoManagerDLL} stdcall uninstallonly';
function UninstallReadCustomizeInfoA(const AInfoName, ALangName: PAnsichar; AValue: PAnsiChar; AValueLen: integer; AType: PAnsiChar; ATypeLen: integer):BOOL; external 'ReadCustomizeInfoA@{app}\{#ProductInfoManagerDLL} stdcall uninstallonly';
function UninstallWSOpenURLA(const AURL: PAnsichar; nOpenMode: integer):BOOL; external 'WSOpenURLA@{app}\{#ProductInfoManagerDLL} stdcall uninstallonly';

function ReadCustomizedStringInfo(const AInfoName: ansistring): ansistring; forward;
function GetProductID(const APID: string): string; forward;

//自定义函数...

function KillProcess(szFileName: string): Boolean;
var
  ErrorCode: Integer;
begin
  ShellExec('open', ExpandConstant('{sys}') + '\TASKKILL.exe', '/F /IM ' + szFileName,'', SW_HIDE, ewWaitUntilTerminated, ErrorCode);
  Result := ErrorCode = 0;
end;

function GetLanguageCode: string;
begin
  Result := ExpandConstant('{language}');
  if Result = 'ARA' then
    Result := 'ARG';
end;

procedure UninstallCustomizedProcess; // 卸载跳转
var
  configFile, LanguageCode, publishName, ProductName, sType: ansistring;
  sUrl: string;
  ErrorCode : Integer;
  sJumpPage: ansistring;
  sRes: string;
begin
  LanguageCode:= GetLanguageCode;
  configFile:= AddBackslash(ExpandConstant('{app}')) + '{#MyAppCustomizedFileName}';
  publishName:= '{#Publisher}';
  ProductName:= '{#AppName}';

  if not UninstallLoadCustomizeXMLFileA(PAnsichar(configFile)) then Exit;
  
  sRes := '0';
  RegQueryStringValue(HKEY_LOCAL_MACHINE, 'Software\Wondershare\{#ProductID}', 'Register', sRes);  // 读入注册标记

  SetLength(sJumpPage, 1024);
  if sRes = '0' then UninstallReadCustomizeInfoA('UninstallTrialJumpPage', PAnsichar(LanguageCode), PAnsichar(sJumpPage), 1024, PAnsichar(sType), 0)
  else UninstallReadCustomizeInfoA('UninstallJumpPage', PAnsichar(LanguageCode), PAnsichar(sJumpPage), 1024, PAnsichar(sType), 0);
  UninstallWSOpenURLA(PAnsichar(sJumpPage), 0);

  //publishName:= '{#Publisher}';
  //ProductName:= '{#AppName}';
  // 调用失败，原因不明，可能跟动态库有关。改从注册表中读入UninstallJumpPage，使用ShellExec执行跳转。
  //MsgBox('卸载跳转1', mbConfirmation, MB_YESNO);   // test
  //MsgBox(GetProductID('{#ProductID}'), mbConfirmation, MB_YESNO);   // test
  //MsgBox(PAnsichar(LanguageCode), mbConfirmation, MB_YESNO);   // test
  //MsgBox(PAnsichar(configFile), mbConfirmation, MB_YESNO);   // test
  //MsgBox(PAnsichar(publishName), mbConfirmation, MB_YESNO);   // test
  //MsgBox(PAnsichar(ProductName), mbConfirmation, MB_YESNO);   // test
  //UnInstallCustomized(PAnsichar(LanguageCode),PAnsichar(configFile),PAnsichar(publishName), PAnsichar(ProductName));
  //MsgBox('卸载跳转2', mbConfirmation, MB_YESNO);   // test
  //configFile := AddBackslash(ExpandConstant('{sd}')) + ExpandConstant('ProgramData/Wondershare Filmora Scrn/') + ExpandConstant('{#ConfigName}');
  //MsgBox(configFile, mbConfirmation, MB_YESNO);
  //sPID := GetIniString('UpdateRegister', 'ProdID', '{#ProductID}', configFile);
  //MsgBox(sPID, mbConfirmation, MB_YESNO);
  //RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\Wondershare\' + sPID, 'UninstallJumpPage', sUrl);
  //MsgBox('卸载跳转', mbConfirmation, MB_YESNO);   // test
  //MsgBox(sUrl, mbConfirmation, MB_YESNO);   // test
  //sUrl := ReadCustomizedStringInfo('UninstallJumpPage'); 
  //ShellExec('open', sUrl, '', '', SW_SHOW, ewNoWait, ErrorCode);
  //sUrl := ReadCustomizedStringInfo('UninstallJumpPage');
  //sPID := ReadCustomizedStringInfo('PID');
  //MsgBox(g_PID, mbConfirmation, MB_YESNO);   // test
  //RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\Wondershare\' + g_PID, 'UninstallJumpPage', sUrl);
  //ShellExec('open', g_sJumpPage, '', '', SW_SHOW, ewNoWait, ErrorCode);
  //var ErrorCode : Integer;
  //if RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\Wondershare\3235', 'UninstallJumpPage', sUrl) then
  //begin
  //MsgBox(sUrl, mbConfirmation, MB_YESNO);
  //ShellExec('open', sUrl, '', '', SW_SHOW, ewNoWait, ErrorCode);
  //end;
end;

function GetPersonalFolder(Param: string): string;
begin
  Result := GetShellFolderByCSIDL(CSIDL_PERSONAL, True);
  //其实也可用此代码：ExpandConstant('{userdocs}')
end;

function GetAgentValue(const AKey: string): string;
begin
  //从Customization.xml中读取链接地址
  Result := ReadCustomizedStringInfo(AKey);
end;

function GetAppPublisher(const APublisher: string): string;
begin
  Result := ReadCustomizedStringInfo('Publisher');
  if Result = '' then Result := APublisher;
end;

function GetAppShortVersion(const AVersion: string): string;
var
  i, n: Integer;
begin
  //RPos在此代码中不能用，它用在ISPP中，所以此处用比较方法来求
  n := 0;
  for i := 1 to Length(AVersion) do
    if AVersion[i] = '.' then
    begin
      n := n + 1;
      if n = 3 then
      begin
        Result := Copy(AVersion, 1, i - 1);
        Break;
      end;
    end;
end;

//是否升级后安装
function IsUpdateSetup: Boolean;
var
  i: Integer;
begin
  Result := False;
  for i := 0 to ParamCount  do
    if (ParamStr(i) = '/SILENT') or (ParamStr(i) = '/NOTSHOWCBS') then
    begin
      Result := True;
      Break;
    end;
end;
            
//是否下载器安装
function IsDownloaderSetup: Boolean;
var
  i: Integer;
begin
  Result := False;
  for i := 0 to ParamCount  do
    if (ParamStr(i) = '/VERYSILENT') then
    begin
      Result := True;
      Break;
    end;
end;

//是否覆盖卸载
function IsOverUnInstall: Boolean;
var
  i: Integer;
begin
  Result := False;
  //为覆盖安装卸载完全而设计
  if not Result then
    for i := 0 to ParamCount  do
      if ParamStr(i) = '/REINSTALL' then
      begin
        Result := True;
        Break;
      end;
end;

//打开链接
function OpenUrl(const AUrl: string): Boolean;
var
  sUrl: string;
begin
  sUrl := AUrl;
  if Pos(':', sUrl) = 0 then sUrl := 'http://' + sUrl;
  Result := ShellExecute(0, 'open', PChar(sUrl), '', '', 1) > 32;
  if not Result then Result := ShellExecute(0, '', 'IEXPLORE.EXE', PChar(sUrl), '', 1) > 32;
end;


//解析启动参数是否有不跳网页
function NoJumpPage: Boolean;
var
  i: Integer;
begin
  Result := False;
  for i := 0 to ParamCount  do
    if (ParamStr(i) = '/NOPAGE') then
    begin
      Result := True;
      Break;
    end;
end;


//是否/VERYSILENT安装
function IsVERYSILENTSetup: Boolean;
var
  i: Integer;
begin
  Result := False;
  for i := 0 to ParamCount  do
    if (ParamStr(i) = '/VERYSILENT') then
    begin
      Result := True;
      Break;
    end;
end;

//是否绑定产品安装
function IsBindProduct: Boolean;
var
  i: Integer;
begin
  Result := False;
  for i := 0 to ParamCount  do
    if (ParamStr(i) = '/ISBind') then
    begin
      Result := True;
      Break;
    end;
end;


//是否静默安装
function IsSilentInstall: Boolean;
var
  i: Integer;
begin
  Result := False;
  for i := 0 to ParamCount  do
    if (ParamStr(i) = '/VERYSILENT') then
    begin
      Result := True;
      Break;
    end;
end;

function GetParamByName(paramName: string): string;
var
  i: Integer;
begin
  Result := '';
  for i := 0 to ParamCount  do
  begin
    Log('>>>>ParamStr(i):'+ ParamStr(i));
    if (Uppercase(ParamStr(i)) = Uppercase(paramName)) then
    begin
      Result := ParamStr(i+1);
      Break;
    end;
  end;
end;

//获取跳转类型及跳转链接
procedure OpenJumpPage(const AProcessType: TCBSProcessType);       // 安装跳转
var
  sJumpType, sJumpPage: string;
begin
  sJumpType := ReadCustomizedStringInfo('CBSJumpType');
  //MsgBox('sJumpType', mbConfirmation, MB_YESNO);     // test
  //MsgBox(sJumpType, mbConfirmation, MB_YESNO);       // test
  //无跳转
  if compareText(sJumpType, 'Disable') = 0 then Exit;

  //跳转链接
  case AProcessType of
    cptInstall:      
    begin
      if IsBindProduct then
        sJumpPage := ReadCustomizedStringInfo('C10')
      else
        sJumpPage := ReadCustomizedStringInfo('InstallJumpPage');
    end;
    //MsgBox('sJumpPage1', mbConfirmation, MB_YESNO);     // test
    //MsgBox(sJumpPage, mbConfirmation, MB_YESNO);       // test 
    cptOnlineUpdate: sJumpPage := ReadCustomizedStringInfo('UpdateJumpPage');
    cptUnInstall:    sJumpPage := ReadCustomizedStringInfo('UninstallJumpPage');
  end;
  if sJumpPage = '' then Exit;
  //MsgBox('安装跳转', mbConfirmation, MB_YESNO);     // test
  //MsgBox(sJumpPage, mbConfirmation, MB_YESNO);       // test 
  //正常打开
  if CompareText(sJumpType, 'Jump') = 0 then
  begin
    if NoJumpPage then
      WSOpenURLA(sJumpPage, 1)
    else
      OpenURL(sJumpPage);
  end
  //隐藏打开...'Hide'
  else
  begin
    if AProcessType = cptInstall then WSOpenURLI(sJumpPage, 1);
    if AProcessType = cptUnInstall then WSOpenURLU(sJumpPage, 1);
  end;
end;

// 以下一项针对代理商不支持广告设置
procedure SetRemindSwitch(AEnable: Boolean);
var
  RegPath: string;
begin
  RegPath:= 'SOFTWARE{#x6432Node}\{#Publisher}\{#OldAppName}';
  RegWriteDWordValue(HKEY_LOCAL_MACHINE, RegPath, 'RemindSwitch', Integer(AEnable));
end;

// 以下一项针对安装时设置是否参加用户数据收集
procedure SetEnableUserDataCollect(AEnable: Boolean);
var
  RegPath, sPID, sVersion: string;
begin
  sPID := ReadCustomizedStringInfo('PID');
  sVersion := ReadCustomizedStringInfo('ProductVersion');
  RegPath:= 'SOFTWARE\{#Publisher}\{#CBSHelperAppName}\' + sPID + '\' + sVersion;
  RegWriteDWordValue(HKEY_LOCAL_MACHINE, RegPath, 'JoinUserExperience', integer(AEnable));
end;

function ReadCustomizedStringInfo(const AInfoName: ansistring): ansistring;
var
  Value, sType: ansistring;
begin
  result:= '';
  SetLength(Value, 1024);
  if ReadCustomizeInfoA(PAnsichar(AInfoName), PAnsichar(GetLanguageCode), PAnsichar(value), 1024, PAnsichar(sType), 0) then
    result:= string(PAnsichar(value));
end;

//字串加密函数开始...
function ExChange(const S: string): String;
var
  i: Integer;
begin
  Result := S;
  for i := 1 to Length(S) do
    Result[i] := S[Length(S) - i + 1];
end;

function ByteToHex(b: Byte): string;
begin
  SetLength(Result, 3);
  if (b div 16) > 9 then
    Result[1] := Chr((b div 16) - 10 + Ord('A'))
  else Result[1] := Chr(b div 16 + Ord('0'));
  if (b mod 16) > 9 then
    Result[2] := Chr((b mod 16) - 10 + Ord('A'))
  else Result[2] := Chr(b mod 16 + Ord('0'));

  Result[3] := #0;
end;

function EncryptStr(const Src, Key: string): string;
var
  i, j, Keyidx, step, SrcLen, KeyLen, DstLen: Integer;
  pch, str: string;
  ntmp: Integer;
begin
  SrcLen := Length(Src);
  KeyLen := Length(Key);
  if SrcLen > KeyLen then
    DstLen := 2 * (SrcLen + 1)
  else DstLen := 2 * (KeyLen + 1);

  SetLength(Result, DstLen);
  ntmp := DstLen div 2;
  SetLength(pch, ntmp);
  SetLength(str, ntmp);
  pch[ntmp] := chr(SrcLen - KeyLen);
  for i := 1 to KeyLen do
    pch[ntmp] := Chr(Ord(pch[ntmp]) xor Ord(Key[i]));
  for i := 0 to ntmp - 2 do
    pch[i + 1] := Chr(Ord(Src[i mod SrcLen + 1]) xor Ord(Key[i mod KeyLen + 1]) xor Ord(pch[ntmp]));

  i := 1;
  Keyidx := 0;
  while i < ntmp do
  begin
    step := Ord(Key[(Keyidx mod KeyLen) + 1]) mod 10;
    if i + step > ntmp then
    Step :=  ntmp - i;
    str := Copy(pch, i, step);
    str := ExChange(str);
    for j := 0 to step - 1 do
      pch[i + j] := str[j + 1];
    i := i + Step;
    Keyidx := Keyidx + 1;
  end;

  for i := 1 to ntmp do
  begin
    str := ByteToHex(Ord(pch[i]));
    Result[2 * i - 1] := str[1];
    Result[2 * i] := str[2];
  end;
end;

//程序是否正在运行中
function CheckProgramRunning(): Boolean;
begin
  Result := (FindWindowByClassName('Qt5QWindowIcon') <> 0) or (FindWindowByClassName('Qt5QWindowToolSaveBits') <> 0) or (FindWindowByClassName('SysListView32') <> 0);
end;

//关闭正在运行的程序
procedure TerminateProgram();
begin
  if FindWindowByClassName('Qt5QWindowIcon') <> 0 then SendMessage(FindWindowByClassName('Qt5QWindowIcon'), WM_CLOSE, 0, 0);
end;

//获取卸载文件路径
function UninstallExeFile(): string;
var
  AppPath: string;
  i: Integer;
begin
  Result := '';
  //为避免安装文件不为unins000而做处理
  if not RegQueryStringValue(HKEY_CLASSES_ROOT, ExpandConstant('{#ProjNodeName}'), 'path', AppPath) then
    //兼顾2.0及以前版本
    if RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\Wondershare\3235', 'EXEPath', AppPath) then
      AppPath := ExtractFileDir(AppPath);

  if DirExists(AppPath) then
    for i := 0 to 9 do
      if FileExists(AppPath + '\unins00' + IntToStr(i) + '.exe') then
      begin
        Result := AppPath + '\unins00' + IntToStr(i) + '.exe';
        Exit;
      end;
end;

//xp系统安装刻录引擎
procedure InstallWinBurnEngine;
var
  Version: TWindowsVersion;
  sExePath: string;
  ErrorCode: Integer;
begin
  GetWindowsVersionEx(Version);
  if Version.NTPlatform and (Version.Major = 5) and (Version.Minor = 1) then
  begin
    sExePath := ExpandConstant('{tmp}\{#IMApiXPSrv}');
    ExtractTemporaryFile(ExtractFileName(sExePath));
    ShellExec('open', sExePath, '/quiet', '', SW_HIDE, ewNoWait, ErrorCode);
  end;
end;

//处理代理商文件
function ProcessAgentData(): Boolean;
var
  configFile, DstConfigFile: ansistring;
  exeFile: String;
  CBSJumpType, PID, PIDRegPath, UninstallJumpPage: ansistring;
  EnableAD: Boolean;
begin
  Result := True;
  //只在处理文件时才执行此函数
  if not FileExists(AddBackslash(ExpandConstant('{app}')) + '{#NewExeName}') then Exit;
  SetRemindSwitch(True);
  // 从exe中提取定制配置文件
  configFile:= AddBackslash(ExpandConstant('{tmp}')) + '{#MyAppCustomizedFileName}';
  exeFile:= ExpandConstant('{srcexe}');
  if not PopFileFromReservedA(exeFile, configFile) then Exit;
  // 加载配置文件...2012-05-19验证，在win 7系统下Load失败
  if not LoadCustomizeXMLFileA(PAnsichar(configFile)) then Exit;
  // 升级处理, 将[Files]中的配置文件替换为exe中提取的配置文件,处理定制升级后保留的链接地址
  // 若升级，则将当前配置文件之信息，更新至安装包解压后的，合并后再置于程序目录下
  DstConfigFile:= AddBackslash(ExpandConstant('{app}')) + '{#MyAppCustomizedFileName}';
  if not CustomizeUpdateProcessA(PAnsichar(configFile), PAnsichar(dstConfigFile)) then Exit;
  // DR定制处理，从DR代理商注册表中读取购买地址，需要先进行升级处理...置于此处，使其能写购买链接入配置文件中
  ProcessDRCustomizeA(PAnsichar(dstConfigFile), 'SoftWare\Digital River\SoftwarePassport\', 'BuyURL');
  // 加载配置文件
  if not LoadCustomizeXMLFileA(PAnsichar(dstConfigFile)) then Exit;
  Result := False;
  // 读取PID, CBSJumpType, InstallJumpPage等定制信息
  PID:= ReadCustomizedStringInfo('PID');
  CBSJumpType:= ReadCustomizedStringInfo('CBSJumpType');
  if (PID = '') or (CBSJumpType = '') then Exit;
  // 设置广告弹出使能
  EnableAD:= ReadCustomizeBoolInfoA('EnableAD', PAnsichar(GetLanguageCode));
  SetRemindSwitch(EnableAD);
  
  // 设置CBS跳转, 沿用旧版本方式写入注册表
  PIDRegPath:= 'SOFTWARE\{#Publisher}\' + PID;
  if CompareText(CBSJumpType, 'Hide') = 0 then
    RegWriteStringValue(HKEY_LOCAL_MACHINE, PIDRegPath, 'JumpType', '1')
  else if CompareText(CBSJumpType, 'Jump') = 0 then
    RegWriteStringValue(HKEY_LOCAL_MACHINE, PIDRegPath, 'JumpType', '0')
  else RegWriteStringValue(HKEY_LOCAL_MACHINE, PIDRegPath, 'JumpType', '2');
  UninstallJumpPage:= ReadCustomizedStringInfo('UninstallTrialJumpPage');
  //MsgBox('PID: ' + PID, mbConfirmation, MB_YESNO);     // test
  RegWriteStringValue(HKEY_LOCAL_MACHINE, PIDRegPath, 'UninstallJumpPage', UninstallJumpPage);
end;

function GetProductID(const APID: string): string;
begin
  Result := ReadCustomizedStringInfo('PID');
  if Result = '' then Result := APID;
end;

//计算字符串中.的个数
function CountPoint(const sStr: string): integer;
var i, j: integer;
begin
  j:=0;
  for i:=1 to length(sStr) do
  if sStr[i] = '.' then j := j + 1;
  Result:= j;
end;

//写配置文件
procedure WriteConfigFile();
var
  sConfigIni, sKey: string;
  count: integer;
  sVersion: string;
begin
  //对FilmoraScrn.ini写入
  //sConfigIni := AddBackslash(ExpandConstant('{app}')) + ExpandConstant('{#ConfigName}');
  sConfigIni := AddBackslash(ExpandConstant('{sd}')) + ExpandConstant('ProgramData/Wondershare Filmora Scrn/') + ExpandConstant('{#ConfigName}');
  //if FileExists(sConfigIni) = False then 
  SetIniString('System', 'DefInstallLang', GetLanguageCode, sConfigIni);
  SetIniString('System', 'ProdID', GetProductID('{#ProductID}'), sConfigIni);

  //对UpdateRegister.ini写入
  sConfigIni := AddBackslash(ExpandConstant('{app}')) + ExpandConstant('{#UpdateRegisterConfigName}');
  //以EXE文件的MD5值做为加密Key
  sKey := GetFileMD5Ex(PChar(ExpandConstant('{app}') + '\' + ExpandConstant('{#NewExeName}')));
  //System Section
  SetIniString('System', 'CodeKey', sKey, sConfigIni);
  SetIniString('System', 'GUID', EncryptStr(ExpandConstant('{#SGUID}'), sKey), sConfigIni);
  SetIniString('System', 'Value1', EncryptStr(ExpandConstant('{#RegKeyN}'), sKey), sConfigIni);
  SetIniString('System', 'Value2', EncryptStr(ExpandConstant('{#RegKeyD}'), sKey), sConfigIni);
  //SetIniInt('System', 'UpdatePeriod', 7, sConfigIni);
  //SetIniString('System', 'DefInstallLang', GetLanguageCode, sConfigIni);
  //SetIniString('System', 'DefLanguage', GetLanguageCode, sConfigIni);
  //SetIniString('System', 'ProductID', ExpandConstant('{#ProductID}'), sConfigIni);
  //注册信息
  if g_regMail <> '' then SetIniString('UpdateRegister', 'Email', g_regMail, sConfigIni);
  if g_regCode <> '' then SetIniString('UpdateRegister', 'RegkeyCode', g_regCode, sConfigIni);

  sVersion := ExpandConstant('{#AppVersion}');
  if CountPoint(sVersion) = 3 then sVersion := copy(sVersion, 1, length(sVersion) - 2);

  //UpdateRegister Section
  SetIniString('UpdateRegister', 'ProdID', GetProductID('{#ProductID}'), sConfigIni);
  SetIniString('UpdateRegister', 'ProductName', ExpandConstant('{cm:AppName}'), sConfigIni);
  SetIniString('UpdateRegister', 'Version', sVersion, sConfigIni);

  //CrashReport.ini
  sConfigIni := AddBackslash(ExpandConstant('{app}')) + ExpandConstant('{#CrashReportConfigName}');
  SetIniString('bug_splat', 'product_version', sVersion, sConfigIni);

  //Http Section
  //SetIniString('HTTP', 'UpdateUrl', ExpandConstant('{#UpdatePage}'), sConfigIni);
  //SetIniString('HTTP', 'EmailServerURL', ExpandConstant('{#EmailServerURL}'), sConfigIni);
end;

function IntToHex(Value: Integer; Digits: Integer): String;
var
  s: string;
begin
  s := Format('%x', [Value]);
  if Length(s) < Digits then
    Result := StringOfChar('0', Digits - Length(s)) + s
  else Result := s;
end;

function GetGuid(dummy: string): string;
var
  Guid: TGuid;
begin
  if CoCreateGuid(Guid) = 0 then
  begin
    Result := '{' + IntToHex(Guid.D1, 8) + '-' +
      IntToHex(Guid.D2, 4) + '-' +
      IntToHex(Guid.D3, 4) + '-' +
      IntToHex(Guid.D4[0], 2) + IntToHex(Guid.D4[1], 2) + '-' +
      IntToHex(Guid.D4[2], 2) + IntToHex(Guid.D4[3], 2) +
      IntToHex(Guid.D4[4], 2) + IntToHex(Guid.D4[5], 2) +
      IntToHex(Guid.D4[6], 2) + IntToHex(Guid.D4[7], 2) +
      '}';
  end
  else Result := '{00000000-0000-0000-0000-000000000000}';
end;

//定制安装步骤相关函数组...

procedure LicenseCheckBoxOnClick(Sender: TObject);
begin
  WizardForm.LicenseAcceptedRadio.Checked := TNewCheckBox(Sender).Checked;
  WizardForm.LicenseNotAcceptedRadio.Checked := not TNewCheckBox(Sender).Checked;
end;

procedure CreateLicenseCheckBox(ParentPage: TNewNotebookPage; const ALeft, ATop: Integer);
begin
  with TNewCheckBox.Create(ParentPage) do
  begin
    Parent := ParentPage;
    Caption := ExpandConstant('{cm:AcceptLicense}');
    Left := ALeft;
    Top := ATop;
    Width := ParentPage.Width;
    OnClick := @LicenseCheckBoxOnClick;
  end;
end;

procedure LaunchCheckBoxOnClick(Sender: TObject);
//var
//  i: Integer;
begin
  //此处与Run之任务列表互动起来，启动程序交由安装程序执行
  //2012-06-13与产品确认，此CheckBox只为免责所用，不执行具体操作
  //with WizardForm.RunList do
  //  for i := 0 to Items.Count - 1 do
  //    if ItemCaption[i] = TNewCheckBox(Sender).Caption then
  //    begin
  //      Checked[i] := TNewCheckBox(Sender).Checked;
  //      Break;
  //    end;
end;

procedure CreateFinishedPage;
var
  Page: TWizardPage;
  h: Integer;
begin
  Page := CreateCustomPage(wpInfoAfter, '{#AppName}', '');

  //标签
  with TNewStaticText.Create(Page) do
  begin
    Left := 0;
    Top := 0;
    Width := Page.SurfaceWidth;
    //Height := Page.SurfaceHeight - 48;
    AutoSize := True;
    Parent := Page.Surface;

    Caption := ExpandConstant('{cm:UninstMsg3}') + #13#10 + #13#10
      + ExpandConstant('{cm:UninstMsg4}') + #13#10
      + ExpandConstant('{cm:UninstMsg5}') + #13#10 + #13#10
      + ExpandConstant('{cm:UninstMsg6}') + #13#10
      + ExpandConstant('{cm:UninstMsg7}') + #13#10
      + ExpandConstant('{cm:UninstMsg8}');
    h := Height;
  end;

  with TNewCheckBox.Create(Page) do
  begin
    Top := h + 12;//Page.SurfaceHeight - 48;
    Width := Page.SurfaceWidth;
    Parent := Page.Surface;
    Caption := ExpandConstant('{cm:TakePlan}');
    Checked := True;
    OnClick := @LaunchCheckBoxOnClick;
  end;
end;





















