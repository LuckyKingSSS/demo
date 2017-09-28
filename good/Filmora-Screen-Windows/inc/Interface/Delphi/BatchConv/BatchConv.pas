unit
	BatchConv; 

{$MINENUMSIZE 4}
    
interface

uses
    Windows, Messages, EncParam, ConvTaskInfo;



type
	HSUBVIEW = HWND;
	HVIEW = HWND;

	
{$IFNDEF EVENT_WAIT_STATE}
{$DEFINE EVENT_WAIT_STATE}

const
 EWS_OK			= (0);
 EWS_TIMEOUT	= ($0102);
 EWS_ERROR		= (-1);


{$IFNDEF WAIT_OBJECT_0}
{$DEFINE WAIT_OBJECT_0}
const
 WAIT_OBJECT_0	= 0;
{$ENDIF}  //  WAIT_OBJECT_0

{$ENDIF}  //  EVENT_WAIT_STATE



const
 UM_CONVERT_PROGRESS   = (WM_USER + 3000);
	// wParam -  0 进度  lParam - 进度(LOWORD:总进度 HIWORD:当前任务进度 0-100)  详细信息请使用 BCGetProgressInfo	
	//           1 转换完成  wParam - 0 正常完成  1 用户终止 2 外部回掉终止 3 遭遇错误
	//         100 第二次进度开始 
	//           101 进度消息  lParam - 进度(0-100)
	//         110 第二次进度结束 在第二次进度期间 不能调用WEPause和WEStop函数
	// 	  1000 开始新任务
			

const
 UM_CONVERT_END        = (WM_USER + 3001);
 UM_CONVERT_ERROR      = (WM_USER + 3002);


type
 ConvertProgressInfo = record

	 dTotalLength     		: double; // 影片总时间长度
	 dTotalTransedLength	: double; // 总任务已转换影片时间

	 dCurrTime         		: double; // 当前时间
  	 nPercent            	: Integer; // 总任务百分比（0 - 100）
  	 nCurrentTaskPercent 	: Integer; // 当前任务进度百分比
  	 nPercentPercent            	: Integer; // 总任务万分比（0 - 10000）
  	 nCurrentTaskPercentPercent 	: Integer; // 当前任务进度万分比
  	 nCurrentTaskIndex   	: Integer; // 当前任务索引号, 从０开始
	 nTaskCount				: Integer; // 总任务数?		


	 tmPrepaid				: DWORD; // 已用时间 单位：秒
	 tmRemain				: DWORD; // 剩余时间 单位：秒

	 dCurrTransedTime		: double; // 当前任务已转换影片时间
	 dCurrLength			: double; // 当前任务影片长度
	 dTransRate				: double; // 转换速率，单位：每秒转换的片断长度
	 nFramesPerSecond		: Integer; // 转换速率，单位：每秒处理的帧数
  	 nEnocdeFramesPerSecond	: Integer; // 每秒钟编码的视频帧数量

	 dwConvState			: ConvertState; // 转换状态		

end; 
  PConvertProgressInfo = ^ConvertProgressInfo;


//void __stdcall InitTaskItem(TaskItem* pTask);
procedure InitTaskItem(pTask : PTaskItem); stdcall;

//void __stdcall InitEncParam(EncodeParam* pEncParam);
procedure InitEncParam(pEncParam : PEncodeParam); stdcall;


type HBATCHCONVERTER = pointer;
type HEXPORTVIEWER = pointer;


//HBATCHCONVERTER __stdcall BCCreate();
function BCCreate() : HBATCHCONVERTER; stdcall;

//void __stdcall BCDestroy(HBATCHCONVERTER hBatchConv);
procedure BCDestroy(hBatchConv : HBATCHCONVERTER); stdcall;



//HEXPORTVIEWER __stdcall BCInitPreview(HBATCHCONVERTER hBatchConv, HVIEW hParentWnd, LPRECT pRcPos);
function BCInitPreview(hBatchConv : HBATCHCONVERTER; hParentWnd : HVIEW; pRcPos : PRECT) : HEXPORTVIEWER; stdcall;
 

//void __stdcall BCAddTaskItem(HBATCHCONVERTER hBatchConv, const TaskItem* pTask);
procedure BCAddTaskItem(hBatchConv : HBATCHCONVERTER; const pTask : PTaskItem); stdcall;

//int __stdcall BCGetTaskCount(HBATCHCONVERTER hBatchConv);
function BCGetTaskCount(hBatchConv : HBATCHCONVERTER) : Integer; stdcall;

//BOOL __stdcall BCGetTaskItem(HBATCHCONVERTER hBatchConv, int nIndex, TaskItem* pTask);
function BCGetTaskItem(hBatchConv : HBATCHCONVERTER; nIndex : Integer; const pTask : PTaskItem) : BOOL; stdcall;

//void __stdcall BCSetAudioLimitLength(HBATCHCONVERTER hBatchConv, int nLimitLength);
procedure BCSetAudioLimitLength(hBatchConv : HBATCHCONVERTER; nLimitLength : Integer); stdcall;

//void __stdcall BCSetWatermarkImage(HVIDEODECODER hVideo, LPBITMAPINFO lpbi, LPBYTE lpBits, const LPRECT pRect = 0, int nOpacity = 100); // 支持32位透明图像 pRect - 相对于剪裁后的图像分辨率中的位置
//procedure BCSetWatermarkImage(hBatchConv : HBATCHCONVERTER; lpbi : PBITMAPINFO; lpBits : PBYTE; const pRect : PRECT = nil; nOpacity : Integer = 100); stdcall;



// 开始转换
//BOOL __stdcall BCRun(HBATCHCONVERTER hBatchConv, HVIEW hWnd = 0, BOOL bMerge = FALSE);
function BCRun(hBatchConv : HBATCHCONVERTER; hWnd : HVIEW = 0; bMerge : BOOL = FALSE) : BOOL; stdcall;

//int __stdcall BCPause(HBATCHCONVERTER hBatchConv);
function BCPause(hBatchConv : HBATCHCONVERTER) : Integer; stdcall;

//int __stdcall BCResume(HBATCHCONVERTER hBatchConv);
function BCResume(hBatchConv : HBATCHCONVERTER) : Integer; stdcall;

//DWORD __stdcall BCWait(HBATCHCONVERTER hBatchConv, UINT uMilliSecondsTimeOut = 0xFFFFFFFF);
function BCWait(hBatchConv : HBATCHCONVERTER; uMilliSecondsTimeOut : UINT = $FFFFFFFF) : DWORD; stdcall;

//int __stdcall BCAbort(HBATCHCONVERTER hBatchConv, WPARAM wParam = 0, LPARAM lParam = 0);
function BCAbort(hBatchConv : HBATCHCONVERTER; wParam : WPARAM = 0; lParam : LPARAM = 0) : Integer; stdcall;

// 显示Preview窗口
//void __stdcall BCShowPreview(HBATCHCONVERTER hBatchConv, BOOL bShow = TRUE);
procedure BCShowPreview(hBatchConv : HBATCHCONVERTER; bShow : BOOL = TRUE); stdcall;

// 打开预览回调
//void __stdcall BCEnablePreview(HBATCHCONVERTER hBatchConv, BOOL bEnable = TRUE);
procedure BCEnablePreview(hBatchConv : HBATCHCONVERTER; bEnable : BOOL = TRUE); stdcall;


//void __stdcall BCGetProgressInfo(HBATCHCONVERTER hBatchConv, ConvertProgressInfo* pProgressInfo);
procedure BCGetProgressInfo(hBatchConv : HBATCHCONVERTER; pProgressInfo : PConvertProgressInfo); stdcall;



implementation

const
    DLLNAME = 'WS_BatchConv.dll';

procedure InitTaskItem				; external DLLNAME Name 'InitTaskItem';
procedure InitEncParam				; external DLLNAME Name 'InitEncParam';

function BCCreate					; external DLLNAME Name 'BCCreate';
procedure BCDestroy					; external DLLNAME Name 'BCDestroy';
function BCInitPreview				; external DLLNAME Name 'BCInitPreview';
procedure BCAddTaskItem				; external DLLNAME Name 'BCAddTaskItem';

function BCGetTaskCount				; external DLLNAME Name 'BCGetTaskCount';
function BCGetTaskItem				; external DLLNAME Name 'BCGetTaskItem';

procedure BCSetAudioLimitLength				; external DLLNAME Name 'BCSetAudioLimitLength';
//procedure BCSetWatermarkImage				; external DLLNAME Name 'BCSetWatermarkImage';

function BCRun						; external DLLNAME Name 'BCRun';
function BCPause					; external DLLNAME Name 'BCPause';
function BCResume					; external DLLNAME Name 'BCResume';
function BCWait						; external DLLNAME Name 'BCWait';
function BCAbort					; external DLLNAME Name 'BCAbort';

procedure BCShowPreview				; external DLLNAME Name 'BCShowPreview';
procedure BCEnablePreview			; external DLLNAME Name 'BCEnablePreview';
procedure BCGetProgressInfo			; external DLLNAME Name 'BCGetProgressInfo';



end.




