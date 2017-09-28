unit Player;

{$WARNINGS OFF}

interface

uses
  Windows, Messages, CodecDefine;

{typedef void *HPLAYER;
typedef void *HVIDEODECODER;
typedef void *HAUDIODECODER;}



type
  HPLAYER = Pointer;
  HVIDEODECODER = Pointer;
  HAUDIODECODER = Pointer;


const
  WM_PLAYERMSG = WM_USER + 2001; //播放器消息
  //消息参数wParam
  PM_Progress = 1; //播放中，可用VPGetPlayedTime取得播放时间以计算进度
  PM_Finish = 2;   //播放结束（自动结束才会Post该消息）
  
  UM_CROP_CHANGE = (WM_USER + 4326); // Crop区域更改消息
  
 
// void __stdcall VPInit();
procedure VPInit(); stdcall;

// 关闭GDI+
// void __stdcall VPUninit();
procedure VPUninit(); stdcall;

//创建播放器窗口rcPos－窗口位置
//HPLAYER __stdcall VPCreate(HWND hWndParent, LPRECT pRect);
function  VPCreate(hWndParent: HWND; pRect: PRECT): HPLAYER; stdcall;

//销毁播放器窗口
//void __stdcall VPDestroy(HPLAYER hPlayer);
procedure VPDestroy(hPlay: HPLAYER);stdcall;

//设定编码器
//void __stdcall VPSetDecoder(HPLAYER hPlayer, HVIDEODECODER hVideo, HAUDIODECODER hAudio);
procedure VPSetDecoder(hPlay: HPLAYER; hVideo: HVIDEODECODER; hAudio: HAUDIODECODER);stdcall;

//根据VPSetDecoder所设定decoder，得到视频段长度
//double __stdcall VPGetMovieLength(HPLAYER hPlayer);
function  VPGetMovieLength(hPlayer:HPLAYER):double;stdcall;


//设置视频播放时间， dBegin——开始时间，dLength——持续时间，为0播放到结尾
//void __stdcall VPSetPlayTime(HPLAYER hPlayer, double dBegin, double dLength);
procedure VPSetPlayTime(hPlayer: HPLAYER; dBegin: Double; dLength: Double);stdcall;

//播放，dBegin——开始时间，dLength——持续时间
//void __stdcall VPPlay(HPLAYER hPlayer, double dBegin, double dLength);
procedure  VPPlay(hPlay: HPLAYER);stdcall;

//停止播放
//void __stdcall VPStop(HPLAYER hPlayer);
procedure VPStop(hPlay: HPLAYER);stdcall;

//暂停
//void __stdcall VPPause(HPLAYER hPlayer);
procedure VPPause(hPlay: HPLAYER);stdcall;

//恢复
//void __stdcall VPResume(HPLAYER hPlayer);
procedure VPResume(hPlay: HPLAYER);stdcall;

//定位到dTime,dTime为相对时间 以VPSetPlayTime设置的dBegin为基准
//void __stdcall VPSeek(HPLAYER hPlayer, double dTime, BOOL accurate = TRUE);
procedure VPSeek(hPlay:HPLAYER; dTime: Double; accurate : BOOL = TRUE);stdcall;

//定义回调窗口,消息为WM_PLAYERMSG,wParam参数见PM_Progress,PM_Finish定义
//void __stdcall VPSetCallBack(HPLAYER hPlayer, HWND hCBWnd);
procedure VPSetCallBack(hPlay: HPLAYER; hCBWnd: HWND);stdcall;

//取得播放器窗口句柄
//HWND __stdcall VPGetPlayerHwnd(HPLAYER hPlayer);
function VPGetPlayerHwnd(hPlay: HPLAYER): HWND; stdcall;


//取得播放时间,相对于VPSetPlayTime中dBegin,0～dLength
//double __stdcall VPGetPlayedTime(HPLAYER hPlayer);
function VPGetPlayedTime(hPlay: HPLAYER): Double; stdcall;

//取显示信息
//IMAGE_RESIZE_METHOD __stdcall VPGetDisplayResizeMode(HPLAYER hPlayer);
function VPGetDisplayResizeMode(hPlay: HPLAYER): IMAGE_RESIZE_METHOD; stdcall;

//设置播放比例，其中一个为0则按原视频比例，默认为0
//BOOL __stdcall VPSetDisplayResizeMode(HPLAYER hPlayer, IMAGE_RESIZE_METHOD irm);
function VPSetDisplayResizeMode(hPlay: HPLAYER; irm: IMAGE_RESIZE_METHOD): BOOL; stdcall;


// Crop区域更改消息
//#define UM_CROP_CHANGE		(WM_USER + 4326)

// 设置播放器窗口的编辑模式 
//void __stdcall VPSetCropEditStyle(HPLAYER hPlayer, UINT uCropStyle = 1);
procedure VPSetCropEditStyle(hPlay: HPLAYER; uCropStyle: UINT = 1);stdcall;

// 设置Crop区域  图像坐标 非窗口坐标
//void __stdcall VPSetCropRect(HPLAYER hPlayer, LPRECT pCropRect); 
procedure VPSetCropRect(hPlay: HPLAYER; pCropRect: PRECT);stdcall;

// 获取Crop区域  图像坐标 非窗口坐标
//void __stdcall VPGetCropRect(HPLAYER hPlayer, LPRECT pCropRect); 
procedure VPGetCropRect(hPlay: HPLAYER; pCropRect: PRECT);stdcall;

//刷新显示
//void __stdcall VPUpdate(HPLAYER hPlayer);
procedure VPUpdate(hPlay: HPLAYER); stdcall;

// // 播放器是否处于活动状态，包括正在播放和Pause状态
// 0 - 不活动状态， 1- 正在播放， 2 - 暂停状态
//BOOL __stdcall VPIsActivate(HPLAYER hPlayer);
function VPIsActivate(hPlayer:HPLAYER):integer;stdcall;
//int __stdcall VPIsActivate(HPLAYER hPlayer);
// 0 - 不活动状态， 1- 正在播放， 2 - 暂停状态
//function VPIsActivate(hPlayer:HPLAYER): Integer;stdcall;


const
// Message Code:
//#define VM_SAVE_IMAGE			0x0100 // wParam - LPBITMAPINFO  lParam - file name (char*)
  VM_SAVE_IMAGE		    =	$0100;
//#define VM_SAVE_YUV_2_BMP		0x0101 // wParam - const YUVImage *  lParam - file name (char*)
  VM_SAVE_YUV_2_BMP		= $0101;

//#define VM_FULL_SPEED			0x0800
  VM_FULL_SPEED			  = $0800;
//#define VM_NORMAL_SPEED			0x0801
  VM_NORMAL_SPEED			= $0801;
//#define VM_GET_SPEED			0x0802 // 取得当前播放器每秒播放的帧数
  VM_GET_SPEED			  = $0802;

//DWORD __stdcall VPSendCommand(HPLAYER hPlayer, UINT uMsg, WPARAM wParam, LPARAM lParam);
  function  VPSendCommand(hPlayer:HPLAYER; uMsg:UINT; wParam:WPARAM; lParam:LPARAM):DWORD;stdcall;

implementation

const
  DLLNAME = 'GLPlayer.dll';

procedure VPInit     ;external DLLNAME Name 'VPInit';
procedure VPUninit   ;external DLLNAME Name 'VPUninit';

function  VPCreate; external DLLNAME Name 'VPCreate';
procedure VPDestroy; external DLLNAME Name 'VPDestroy';
procedure VPSetDecoder; external DLLNAME index 3;
function  VPGetMovieLength; external DLLNAME Name 'VPGetMovieLength';
procedure VPSetPlayTime; external DLLNAME Name 'VPSetPlayTime';
procedure VPPlay; external DLLNAME Name 'VPPlay';
procedure VPStop; external DLLNAME Name 'VPStop';
procedure VPPause; external DLLNAME Name 'VPPause';
procedure VPResume; external DLLNAME Name 'VPResume';
procedure VPSeek; external DLLNAME Name 'VPSeek';
procedure VPSetCallBack; external DLLNAME Name 'VPSetCallBack';
function  VPGetPlayerHwnd; external DLLNAME Name 'VPGetPlayerHwnd';
function  VPGetPlayedTime; external DLLNAME Name 'VPGetPlayedTime';
function  VPGetDisplayResizeMode; external DLLNAME Name 'VPGetDisplayResizeMode';
function  VPSetDisplayResizeMode; external DLLNAME Name 'VPSetDisplayResizeMode';

procedure VPSetCropEditStyle; external DLLNAME Name 'VPSetCropEditStyle';
procedure VPSetCropRect; external DLLNAME Name 'VPSetCropRect';
procedure VPGetCropRect; external DLLNAME Name 'VPGetCropRect';

procedure VPUpdate; external DLLNAME Name 'VPUpdate';

function  VPIsActivate;external DLLNAME Name 'VPIsActivate';

function  VPSendCommand;external DLLNAME Name 'VPSendCommand';

end.
 