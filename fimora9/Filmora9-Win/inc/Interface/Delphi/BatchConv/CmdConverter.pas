
unit
  CmdConverter;

{$MINENUMSIZE 4}

interface

uses
    Windows, Messages;

//------------------------------------------------------------------------------
// wParam不为0时表示暂停
//#define UM_CONV_PAUSE (WM_APP + 900)
const UM_CONV_PAUSE = (WM_APP + 900);

//------------------------------------------------------------------------------
// wParam 为1表示显示，为0表示不显示
//#define UM_SHOW_PREVIEW (WM_APP + 903)
const UM_SHOW_PREVIEW = (WM_APP + 903);

//------------------------------------------------------------------------------
// 开始转换
//#define UM_RUN_CONV (WM_APP + 904)
const UM_RUN_CONV = (WM_APP + 904);

//------------------------------------------------------------------------------
// 强行终止
//#define UM_FORCE_TERM (WM_APP + 906)
const UM_FORCE_TERM = (WM_APP + 906);

//------------------------------------------------------------------------------
// 请求GetProgressInfo()
//#define UM_GET_PROGRESS_INFO (WM_APP + 907)
const UM_GET_PROGRESS_INFO = (WM_APP + 907);

//------------------------------------------------------------------------------
//#define UM_CONVERT_PROGRESS_WRAP   (WM_APP + 930)
const UM_CONVERT_PROGRESS_WRAP  = (WM_APP + 930);
// HIWORD(wParam)
// LOWROD(wParam) -  0 进度  lParam - 进度(LOWORD:总进度 HIWORD:当前任务进度 0-100)  详细信息请使用 BCGetProgressInfo
//           1 转换完成  wParam - 0 正常完成  1 用户终止 2 外部回掉终止 3 遭遇错误
//         100 第二次进度开始
//           101 进度消息  lParam - 进度(0-100)
//         110 第二次进度结束 在第二次进度期间 不能调用WEPause和WEStop函数
// 	  1000 开始新任务


//#define UM_CONVERT_END_WRAP        (WM_APP + 931)
const UM_CONVERT_END_WRAP    =    (WM_APP + 931);

//#define UM_CONVERT_ERROR_WRAP      (WM_APP + 932)
const UM_CONVERT_ERROR_WRAP   =   (WM_APP + 932);

implementation


end.