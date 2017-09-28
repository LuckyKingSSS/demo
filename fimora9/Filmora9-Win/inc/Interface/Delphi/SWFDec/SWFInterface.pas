unit SWFInterface;

{$MINENUMSIZE 4}

interface

uses
  Windows, VidInfo, AudInfo;

const
  WM_ACREATEPROGRESS = 9000;  //音频解码器创建进度消息

type
  HVIDEODEC = Pointer;
  HAUDIODEC = Pointer;

  //截图回调
  //pUserObj：对象指针 lpbi：设备无关位图指针，nOffset，帧号，lpbi和nOffset同时为0结束
  CAPTURECALLBACK = procedure (pUserObj: Pointer; lpbi: PBITMAPINFO; nOffset: Integer); stdcall;

  //创建视频解码器
  //pPathFiel:flash文件名，返回解码器句柄
  FUNCreateVideoDec = function (pPathFile: PWideChar): HVIDEODEC; stdcall;

  //销毁视频解码器
  FUNDestroyVideoDec = procedure (hVideo: HVIDEODEC); stdcall;

  //取视频信息
  FUNGetVideoInfo = function (hVideo: HVIDEODEC): PVideoInfo; stdcall;

  //设置回调函数
  FUNSetCaptureCallback = procedure (hVideo: HVIDEODEC; pUserObj: Pointer; fnCallback: CAPTURECALLBACK); stdcall;

  //启动视频解码器, hParent——父窗口句柄，rc——区域 , bVisable——是否可见
  FUNVideoRun = procedure (hVideo: HVIDEODEC; hParent: HWND; rc: TRect; bVisable: BOOL); stdcall;

  //视频seek操作，nFrame－ 帧号， 返回seek后帧号
  FUNVideoSeek = function (hVideo: HVIDEODEC; nFrame: Integer): Integer; stdcall;

  //创建音频解码，hMsgWnd——进度消息接收窗口
  //       进度消息 Msg: WM_ACREATEPROCESS
  //                          wParam: 0 进度消息  lParam: 0 - 100
  //                                  1 完成     （暂未使用）
  //                                  2 遭遇错误 （暂未使用）
  FUNCreateAudioDec = function (pPathFile: PWideChar; hMsgWnd: HWND ): HAUDIODEC; stdcall;

  //销毁音频解码
  FUNDestroyAudioDec = procedure (hAudio: HAUDIODEC); stdcall;

  //取音频信息
  FUNGetAudioInfo = function (hAudio: HAUDIODEC): PAudioInfo; stdcall;

  //取音频数据, pBuffer——数据缓冲区, nSample——缓冲区大小，返回取得Sample大小
  FUNGetNextSamples = function (hAudio: HAUDIODEC; pBuffer: PSmallInt; nSample: Integer): Integer; stdcall;

  //音频seek操作,dTime——时间，成功返回seek时间
  FUNAudioSeek = function (hAudio: HAUDIODEC; dTime: Double): Double; stdcall;

  //函数指针结构题
  SWFFunc = record
    fnCreateVideoDec:       FUNCreateVideoDec;
    fnDestroyVideoDec:      FUNDestroyVideoDec;
    fnGetVideoInfo:         FUNGetVideoInfo;
    fnSetCaptureCallback:   FUNSetCaptureCallback;
    fnVideoRun:             FUNVideoRun;
    fnVideoSeek:            FUNVideoSeek;
    fnCreateAudioDec:       FUNCreateAudioDec;
    fnDestroyAudioDec:      FUNDestroyAudioDec;
    fnGetAudioInfo:         FUNGetAudioInfo;
    fnGetNextSamples:       FUNGetNextSamples;
    fnAudioSeek:            FUNAudioSeek;
  end;
  PSWFFunc = ^SWFFunc;



  //取函数地址
  function  SWFGetFuncAddress(pFuncs: PSWFFunc):BOOL; stdcall;
  //取swf视频信息，pPathFile——文件名， pVInfo——视频信息，返回swf文件版本，返回0失败
  function  SWFGetVideoInfo(pPathFile: PWideChar; pVInfo: PVideoInfo): WORD; stdcall;



implementation

const
  DLLNAME = 'WS_FlashDecoder.dll';

function SWFGetFuncAddress; external DLLNAME Name 'SWFGetFuncAddress';
function  SWFGetVideoInfo; external DLLNAME Name 'SWFGetVideoInfo';

end.
