unit MediaEncodeMgr;

interface
uses
  Windows, MediaCommon;

const
  CLSID_CFileFormatInfos: TGUID = '{f1eb87e5-6940-42b6-b4ce-208aa7695819}';
  IID_IFileFormatInfos: TGUID = '{5C9908D7-FDCD-4827-BC0B-94CB3ECCF8BD}';
  IID_IFileFormatInfo: TGUID = '{72BE4AD5-A6B3-4c75-92E2-15BEACE01F72}';
  IID_IEncoderInfos: TGUID = '{A6F06C44-8115-47c5-A98A-F2E44744B378}';
  IID_IEncoderInfo: TGUID = '{D5B71CE2-3957-413c-9DF4-2587CBD20BE4}';
  SEncMgrAPI = 'WS_EncMgr.dll';
type

  IEncoderInfo = interface(IUnknown)
    ['{D5B71CE2-3957-413c-9DF4-2587CBD20BE4}']
    function GetFourCC(): UINT; stdcall;
    function GetName(var pName: WideString): HRESULT; stdcall;
    function GetDescription(var pDescription: WideString): HRESULT; stdcall;
  end;
  
  IEncoderInfos = interface(IUnknown)
    ['{A6F06C44-8115-47c5-A98A-F2E44744B378}']
    function GetCount(): Integer; stdcall;
    function GetItem(nIndex: Integer; var pEncoderInfo: IEncoderInfo): HRESULT; stdcall;
  end;

  IFileFormatInfo = interface(IUnknown)
    ['{72BE4AD5-A6B3-4c75-92E2-15BEACE01F72}']
    function GetFourCC(): UINT; stdcall;
    function GetExName(var pName: UnicodeString): HRESULT; stdcall;
    function GetFormatName(var pFormatName: UnicodeString): HRESULT; stdcall;
    function GetDescription(var pDescription: UnicodeString): HRESULT; stdcall;
    function GetAudioEncoderInfos(var pEncoderInfos: IEncoderInfos): HRESULT; stdcall;
    function GetVideoEncoderInfos(var pEncoderInfos: IEncoderInfos): HRESULT; stdcall;
  end;
  
  IFileFormatInfos = interface(IUnknown)
    ['{5C9908D7-FDCD-4827-BC0B-94CB3ECCF8BD}']
    function GetCount(): Integer; stdcall;
    function GetItem(nIndex: Integer; var pFileFormatInfo: IFileFormatInfo): HRESULT; stdcall;
  end;
const
  RESERVEDSIZE = 1024;

type
  //WMV格式的扩展参数
  WMV_VideoParamEx = record
    Quality: Longint;            // 质量等级   0-4 4为最好 默认为0
    VBRType: Longint;            // 可变长编码类型 m_VBRType 0 1 默认为0 动态码率VBR 0 平均码率ABR 1(暂时不支持)
    MaxBuffer: Longint;          // 缓冲大小   0~10秒 默认为0
    IFrameInterval: Longint;     // 默认为25
  end;

  VC1_VideoParamEx = record
    dwProfile: Cardinal;
	  dwComplexityLevel: Cardinal;
	  dwQP: Cardinal;
	  dwMaxKeyFrameDistance: Cardinal;
	  dwNumOfBFrames: Cardinal;
	  dwRateControlMode: Cardinal;
	  dwVBVBufferInBytes: Cardinal;
    dPeakBitRate: Double;
  end;

  Xvid_EncodeParamEx = record
    Quality: Integer;
    VHQMODE: Integer;
  end;

  TVideoParamEx = record
    dwFourCC: DWORD; // 视频编码器 FourCC ： '1VMW'/'2VMW'/'3VMW' /'DIVX'/'462H'
    case Integer of
      0: (wmvParamEx: WMV_VideoParamEx);
      1: (vc1ParamEx: VC1_VideoParamEx);
      2: (xvidParamEx: Xvid_EncodeParamEx);
      3: (reserved : array[0..RESERVEDSIZE-1] of Longint); //保留字段 为了分配固定大小结构
  end;

  TAudioParamEx = record
    dwFourCC: DWORD; // 音频编码器 FourCC ： '1AMW'/'2AMW'/'3AMW' /' 3PM'
    case Integer of
      0: (reserved : array[0..RESERVEDSIZE div 2-1] of Longint); //保留字段 为了分配固定大小结构
  end;

  PEncodeParamEx = ^TEncodeParamEx;
  TEncodeParamEx = record
    dwFourCC: DWORD;             // 文件格式FourCC
    useflg: Longint;             // 有效标志  0  未使用 1 视频扩展参数  2音频扩展参数 3 音视频扩展参数
    m_TwoPass: Longint;          // 二次编码标志  0 一次编码; 1 二次编码只送一次数据; 2 二次编码送二次数据      默认为0
    videoEx: TVideoParamEx;       // 视频扩展参数
    audioEx: TAudioParamEx;       // 音频扩展参数
  end;

 // 视频参数
  PVideoParam = ^TVideoParam;
  TVideoParam = record
    dwFourCC : DWORD;    // 视频编码器 FourCC ： 'V2PM'
    nWidth : Longint;    // 分辨率宽度    720
    nHeight : Longint;    // 分辨率高度   480或576（国内）
    dFrameRate : double;  // 帧率 0表示自动选择  30000.0 / 1001 或25（国内）
    nBitrate : Longint;    // 码率 bps （恒定码率、可变最小码率） 9800，000或4000，000
    bIsVBR : BOOL;      // 是否使用变码率
    nMaxBitrate : Longint;  // 最大码率
    nResizeStyle : IMAGE_RESIZE_METHOD; // 图像缩放方式
    nInterpolation : IMAGE_INTERPOLATION;  // 图像插值算法
    dwCompression : DWORD;        // 图像格式  ‘21VY’
    nBitsDepth : Longint;          // 图像位深度  12
    
    //=============== MPEG 编码器参数 ===========================//
    // 使用Mpeg编码请参考     mpeg_param.pas                     // 
    dwFormat   : DWORD;   // 编码格式                            //
    dwNorm     : DWORD;     // 电视制式                          //
    dwAspect   : DWORD;   // 比例                                //
    bFieldEnc  : BOOL;  // 是否使用交错模式                      //
    //=============== MPEG 编码器参数 ===========================//

    VideoParamEx: TEncodeParamEx;
  end;

// 音频参数
  PAudioParam = ^TAudioParam;
  TAudioParam = record
    dwFourCC : DWORD;    // 音频编码器 FourCC ： ' 3CA'
    nSampleRate : Longint;  // 采样率 Hz  DVD默认48000
    nChannels : Longint;  // 声道数量    2
    nBitrate : Longint;    // 码率 bps （恒定码率、可变最小码率）128，000
    bIsVBR : BOOL;      // 是否使用变码率
    nMaxBitrate : Longint;  // 最大码率
    nBitsPerSample : Longint;  // Number of bits per sample of mono data 16
  end;  

// 编码参数
  PEncodeParam  = ^TEncodeParam;
  TEncodeParam = record
    Video : TVideoParam;      // 视频参数
    Audio : TAudioParam;      // 音频参数
    bVideoDisable : BOOL;    // 禁用视频
    bAudioDisable : BOOL;    // 禁用音频
//    case Longint of
//      0: (format : EncoderFormat);    // 文件格式
//      1: (dwFourCC: DWORD);           //视频编码器
    dwFourCC: DWORD;        // 文件格式FourCC：' 4PM' ’2gpm'
  end;

  PEncoderInfo = ^TEncoderInfo;
  TEncoderInfo = record
    dwFourCC      : DWORD;
    szEncName     : array [0..63] of WideChar;
    szDescription : array [0..127] of WideChar;
  end;

  PPEncoderInfo = ^PEncoderInfo;

  PFileFormatInfo = ^TFileFormatInfo;
  TFileFormatInfo = record
    dwFourCC      : DWORD;
    szExtName     : array [0..9] of WideChar;
    szFormatName  : array [0..63] of WideChar;
    szDescription : array [0..127] of WideChar;
    nAudioEncs    : Longint;
    nVideoEncs    : Longint;
    ppAudioEncInfos  : PPEncoderInfo;
    ppVideoEncInfos  : PPEncoderInfo;
  end;


// 应用程序启动时调用
  TFCInitPlugins = function () : BOOL; stdcall;
// 应用程序退出时调用
  TFCUninitPlugins = procedure (); stdcall;
// 获取已加载支持的所有文件格式，需在FCInitPlugins()调用成功后才可调用
  TFCGetFileFormatInfos = function () : PFILEFORMATINFO; stdcall;
  
  TMediaEncodeMgrAPI = record
  private
    FAPIHandle: THandle;
    FFCInitPlugins: TFCInitPlugins;
    FFCUninitPlugins: TFCUninitPlugins;
    FPluginsInited: BOOL;
    FFileFormatInfoHead: PFileFormatInfo;
  public
    function Init: Boolean;
    procedure Free;

    function FCInitPlugins() : BOOL;
    procedure FCUninitPlugins();
    function FCGetFileFormatInfos() : PFILEFORMATINFO;
  end;
  
var
  MediaEncodeMgrAPI: TMediaEncodeMgrAPI;
  
implementation

uses
  MediaEncode;
  
const
  SFCInitPlugins =  PAnsiChar(5);
  SFCUninitPlugins =  PAnsiChar(6);

{ TMediaEncodeMgrAPI }

function TMediaEncodeMgrAPI.FCGetFileFormatInfos: PFILEFORMATINFO;
begin

end;

function TMediaEncodeMgrAPI.FCInitPlugins: BOOL;
var
  pFileFormatInfos: IFileFormatInfos;
  pFileFormatInfo: IFileFormatInfo;
  pVidEncoderInfos: IEncoderInfos;
  pVidEncoderInfo: IEncoderInfo;
  pAudEncoderInfos: IEncoderInfos;
  pAudEncoderInfo: IEncoderInfo;
  pEncInfo: PEncoderInfo;
  pEnc: PPEncoderInfo;
  I, J: Integer;
  pStr: UnicodeString;
begin
  Result:=FPluginsInited;
  if not FPluginsInited then
  begin
    if Init then
    begin
      if Assigned(FFCInitPlugins) then
      begin
        InterlockedExchange(Longint(FPluginsInited), Ord(FFCInitPlugins()));
        Result := FPluginsInited;
        if Result then
        begin
          pFileFormatInfos := nil;
          WSCoCreateInstance(CLSID_CFileFormatInfos,
            nil, 0, IID_IFileFormatInfos, pFileFormatInfos);
          if Assigned(pFileFormatInfos) then
          begin
            FFileFormatInfoHead := GetMemory((pFileFormatInfos.GetCount + 1) * SizeOf(TFileFormatInfo));
            for I := 0 to pFileFormatInfos.GetCount - 1 do
            begin
              pFileFormatInfo := nil;
              pFileFormatInfos.GetItem(I, pFileFormatInfo);
              if Assigned(pFileFormatInfo) then
              begin
                with FFileFormatInfoHead^ do
                begin
                  dwFourCC := pFileFormatInfo.GetFourCC;
                  pFileFormatInfo.GetExName(pStr);
                  StringToWideChar(pStr, szExtName, length(pStr));
                  pFileFormatInfo.GetFormatName(pStr);
                  StringToWideChar(pStr, szFormatName, Length(pStr));
                  pFileFormatInfo.GetDescription(pStr);
                  StringToWideChar(pStr, szDescription, Length(pStr));
                  pVidEncoderInfos := nil;
                  pFileFormatInfo.GetVideoEncoderInfos(pVidEncoderInfos);
                  if Assigned(pVidEncoderInfos) then
                  begin
                    nVideoEncs := pVidEncoderInfos.GetCount;
                    if nVideoEncs > 0 then
                    begin

                      ppVideoEncInfos := GetMemory(nVideoEncs * SizeOf(PEncoderInfo));
                      pEnc := ppVideoEncInfos;
                      for J := 0 to nVideoEncs - 1 do
                      begin
                        pVidEncoderInfo := nil;
                        pVidEncoderInfos.GetItem(J, pVidEncoderInfo);
                        if Assigned(pVidEncoderInfo) then
                        begin
                          pEncInfo := pEnc^;
                          pEncInfo := GetMemory(SizeOf(TEncoderInfo));
                          with pEncInfo^ do
                          begin
                            dwFourCC := pVidEncoderInfo.GetFourCC;
                            pVidEncoderInfo.GetName(pStr);
                            StringToWideChar(pStr, szEncName, Length(pStr));
                            pVidEncoderInfo.GetDescription(pStr);
                            StringToWideChar(pStr, szDescription, Length(pStr));
                          end;  
                          Inc(pEnc);
                          pVidEncoderInfo._Release;
                        end;  
                      end;

                      ppAudioEncInfos := GetMemory(nAudioEncs * SizeOf(PEncoderInfo));
                      pEnc := ppAudioEncInfos;
                      for J := 0 to nAudioEncs - 1 do
                      begin
                        pAudEncoderInfo := nil;
                        pAudEncoderInfos.GetItem(J, pAudEncoderInfo);
                        if Assigned(pAudEncoderInfo) then
                        begin
                          pEncInfo := pEnc^;
                          pEncInfo := GetMemory(SizeOf(TEncoderInfo));
                          with pEncInfo^ do
                          begin
                            dwFourCC := pAudEncoderInfo.GetFourCC;
                            pAudEncoderInfo.GetName(pStr);
                            StringToWideChar(pStr, szEncName, Length(pStr));
                            pAudEncoderInfo.GetDescription(pStr);
                            StringToWideChar(pStr, szDescription, Length(pStr));
                          end;
                          Inc(pEnc);
                          pAudEncoderInfo._Release;
                        end;  
                      end;  
                    end;
                    pVidEncoderInfos._Release;  
                  end;  
                end;  
                Inc(FFileFormatInfoHead);
              end;  
            end;  
          end;
        end;
      end;
    end;
  end;
end;

procedure TMediaEncodeMgrAPI.FCUninitPlugins;
begin
  if FPluginsInited then
  begin
    if Assigned(FFCUninitPlugins) then
      FFCUninitPlugins;
    InterlockedExchange(Longint(FPluginsInited), 0);
  end;
end;

procedure TMediaEncodeMgrAPI.Free;
begin
  if FAPIHandle <> 0 then
  begin
    FCUninitPlugins;      // 卸载插件
    FreeLibrary(FAPIHandle);
    FAPIHandle := 0;
    FillChar(Self, SizeOf(Self), 0);
  end;
end;

function TMediaEncodeMgrAPI.Init: Boolean;
begin
  if FAPIHandle=0 then
  begin
    FAPIHandle := LoadLibrary(SEncMgrAPI);
    if FAPIHandle <> 0 then
    begin
      FFCInitPlugins := GetProcAddress(FAPIHandle, (SFCInitPlugins));
      FFCUninitPlugins := GetProcAddress(FAPIHandle, (SFCUninitPlugins));
    end else
      RaiseLoadLibError(SEncMgrAPI);
  end;
  Result := FAPIHandle<>0;
end;

end.
