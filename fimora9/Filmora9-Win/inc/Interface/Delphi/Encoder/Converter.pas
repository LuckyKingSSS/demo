unit
	Converter;
    
interface

uses
    Windows, Messages, EncParam;


//#ifndef EVENT_WAIT_STATE
{$ifndef  EVENT_WAIT_STATE}
//#define EVENT_WAIT_STATE
  {$define EVENT_WAIT_STATE}

//#define EWS_OK			(0)
//#define EWS_TIMEOUT		(0x0102L)
//#define EWS_ERROR		(-1)
  {$define EWS_OK			(0)}
  {$define EWS_TIMEOUT		($0102)}
  {$define EWS_ERROR		(-1)}


//#ifndef WAIT_OBJECT_0
//#define WAIT_OBJECT_0	0
//#endif
  {$ifndef WAIT_OBJECT_0}
    {$define WAIT_OBJECT_0	0}
  {$endif}


//#endif // EVENT_WAIT_STATE
{$endif}


//#ifndef WIN32
//	typedef HIViewRef HSUBVIEW;
//	typedef WindowRef HVIEW;
//#else
//	typedef HWND HSUBVIEW;
//	typedef HWND HVIEW;

//#endif


type
	HCONVERTER    = pointer;	// 转换器句柄定义
	HVIDEODECODER = pointer;	// VideoDecoder句柄定义
	HAUDIODECODER = pointer;	// AudioDecoder句柄定义
	

type
  TimeData = record
    hour      : Integer;
	  minute    : Integer;
	  second    : Integer;
	  millisec  : Integer;
  end;

type
  PMEDIA_FRAME = pointer;
  ProgressInfo = record
  	  dTotalLength  : double;		  // 总帧数量
	  dCurrTime     : double;		  // 当前帧
	  nPercent      : Integer;			// 百分比（0 -100）
	  nPercentPercent : Integer;		// 万分比（0 - 10000）
	  pImageFrame    : PMEDIA_FRAME;	// 图像数据

	  tmPrepaid     : TimeData;		  // 当前任务已用时间 (系统时间)
	  tmRemain      : TimeData;		  // 当前任务剩余时间 (系统时间)
	  dTransedTime  : Double;		    //当前任务已转换片断时间
	  dTransRate    : Double;			  //转换速率，单位：每秒转换的片断长度
  end;
 
	PProgressInfo = ^ProgressInfo;	// AudioDecoder句柄定义

// 进度通知使用Windows消息
const
  WM_ENCODE_PROGRESS = (WM_USER + 3000);    // 进度消息通知 wParam: 0   正在处理  lParam: 0 - 100
                                                //                      100 编码完成  lParam : 0 正常完成
                                                //                                             1 用户终止
                                                //                                             2 外部回掉终止
                                                //                                             3 遭遇错误
  WM_ENCODE_ERROR = (WM_USER + 3001);       // 错误消息通知 wParam: 错误码（暂未使用）

  WM_COPY_PROGRESS = (WM_USER + 3002);    // 文件拷贝进度消息通知 wParam: 0   正在处理  lParam: 0 - 100
                                                //                      100 编码完成  lParam : 0 正常完成
                                                //                                             1 用户终止 
                                                //                                             2 外部回掉终止 
                                                //                                             3 遭遇错误      
      


// 编码时的预览使用回调函数
//typedef BOOL (__stdcall * ENCODE_CALLBACK)(void* pUserObj, UINT uMsg, WPARAM wParam, LPARAM lParam);
// uMsg -    0 进度消息  wParam - 进度(0-100)  lParam - ProgressInfo*
//           1 转换完成  wParam - 0 正常完成  1 用户终止 2 外部回掉终止 3 遭遇错误
//         100 第二次进度开始 
//           101 进度消息  wParam - 进度(0-100)  lParam - 0
//         110 第二次进度结束 在第二次进度期间 不能调用WEPause和WEStop函数
// 返回FALSE 则停止转换
//type
//  ENCODE_CALLBACK = function(pUserObj: Pointer; uMsg: UINT; wParam: WPARAM; lParam: LPARAM): BOOL; stdcall; 


//HCONVERTER __stdcall WECreate(HVIDEODECODER hVideoDecoder, HAUDIODECODER hAudioDecoder); // 创建编码器
function WECreate(hVideoDecoder: HVIDEODECODER; hAudioDecoder: HAUDIODECODER) : HCONVERTER; stdcall;

//void __stdcall WEDestroy(HCONVERTER hEncoder); // 销毁编码器
procedure WEDestroy(hEncoder : HCONVERTER); stdcall;


//BOOL __stdcall WESetOutputFile(HCONVERTER hEncoder, const wchar_t* pOutFileName);
function WESetOutputFile(hEncoder : HCONVERTER; const pOutFileName : PWideChar) : BOOL; stdcall;

//BOOL __stdcall WESetEncodeParam(HCONVERTER hEncoder, const EncodeParam* pParam);
function WESetEncodeParam(hEncoder : HCONVERTER; const pParam : PENCODEPARAM) : BOOL; stdcall;

//BOOL __stdcall WEGetEncodeParam(HCONVERTER hEncoder, EncodeParam* pParam);
function WEGetEncodeParam(hEncoder : HCONVERTER; pParam : PENCODEPARAM) : BOOL; stdcall;

//
//__int64 __stdcall WEGetEstimateDiskSpace(HCONVERTER hEncoder);
function WEGetEstimateDiskSpace(hEncoder : HCONVERTER) : INT64; stdcall;

//__int64 __stdcall WEGetDiskFreeSpace(const wchar_t* pFileName);
function WEGetDiskFreeSpace(const pFileName : PWideChar) : INT64; stdcall;


//int __stdcall WEStart(HCONVERTER hEncoder, HWND hMsgReceiverWnd, ENCODE_CALLBACK fnCallback, void* pUserObj);
// 0 - 成功， 非0失败 1 - 找不到符合音频格式的编码器。 2 - 找不到FourCC对应的视频编码器, 3 - 打开视频文件失败
//4- 音频解码失败，无法生成音频文件
function WEStart(hEncoder : HCONVERTER; dBeginTime: Double; dLength: Double;
                 hMsgReceiverWnd : HWND; fnCallback : ENCODE_CALLBACK; pUserObj : Pointer) : Integer; stdcall;

//int __stdcall WEPause(HCONVERTER hEncoder);
function WEPause(hEncoder : HCONVERTER) : Integer; stdcall;

//int __stdcall WEResume(HCONVERTER hEncoder);
function WEResume(hEncoder : HCONVERTER) : Integer; stdcall;

//int __stdcall WEStop(HCONVERTER hEncoder, WPARAM wParam = 0, LPARAM lParam = 0); // 发送终止消息 并等待转换结束
function WEStop(hEncoder : HCONVERTER; wParam : WPARAM = 0; lParam : LPARAM = 0) : Integer; stdcall;

//int __stdcall WEAbort(HCONVERTER hEncoder, WPARAM wParam = 0, LPARAM lParam = 0); // 发送终止消息，应用程序应该调用WEWait函数等待转换结束
function WEAbort(hEncoder : HCONVERTER; wParam : WPARAM = 0; lParam : LPARAM = 0) : Integer; stdcall;

//DWORD __stdcall WEWait(HCONVERTER hEncoder, UINT uMilliSecondsTimeOut = 0xFFFFFFFF); // 等待转换完成
function WEWait(hEncoder : HCONVERTER; uMilliSecondsTimeOut : UINT = $FFFFFFFF) : DWORD; stdcall;


//BOOL __stdcall WEEnablePreview(HCONVERTER hEncoder, BOOL bEnable);
function WEEnablePreview(hEncoder : HCONVERTER; bEnable : BOOL) : BOOL; stdcall;


//__int64 __stdcall WEGetEstimateDiskSpaceEx(const EncodeParam* pParam, double dLength); // 取得估计所需磁盘空间
function WEGetEstimateDiskSpaceEx(pParam : PENCODEPARAM; dLength : double) : INT64; stdcall;

const
  SPLIT_BASE_ON_TIME	=	1;
  SPLIT_BASE_ON_SIZE	=	2;
// 设置文件分割的参数
// nStyle - SPLIT_BASE_ON_TIME 按时间分割，dwLimit单位为毫秒(ms)；
//        - SPLIT_BASE_ON_SIZE 按文件大小分割，dwLimit单位为字节(Byte)； （暂不支持此种方式）
//BOOL __stdcall WESetSplitParam(HCONVERTER hConverter, int nStyle, DWORD dwLimit);
function WESetSplitParam(hEncoder : HCONVERTER; nStyle : Integer; dwLimit : DWORD) : BOOL ; stdcall;
// 获取最终生成文件数
//int  __stdcall WEGetTargetCount(HCONVERTER hConverter);
function WEGetTargetCount(hEncoder : HCONVERTER) : Integer; stdcall;
// 按序号取文件名
//BOOL __stdcall WEGetTargetFileName(HCONVERTER hConverter, wchar_t pFileName[MAX_PATH], int nIndex);
function WEGetTargetFileName(hEncoder : HCONVERTER; pFileName : PWideChar; nIndex : integer): BOOL; stdcall;

implementation

const
    DLLNAME = 'WS_Converter.dll';

function WECreate						      ; external DLLNAME index 1;
procedure WEDestroy						    ; external DLLNAME index 2;

function WESetOutputFile				  ; external DLLNAME Name 'WESetOutputFile';
function WESetEncodeParam				  ; external DLLNAME Name 'WESetEncodeParam';
function WEGetEncodeParam				  ; external DLLNAME Name 'WEGetEncodeParam';

function WEGetEstimateDiskSpace	  ; external DLLNAME Name 'WEGetEstimateDiskSpace';
function WEGetDiskFreeSpace			  ; external DLLNAME Name 'WEGetDiskFreeSpace';

function WEStart						      ; external DLLNAME Name 'WEStart';
function WEPause						      ; external DLLNAME Name 'WEPause';
function WEResume						      ; external DLLNAME Name 'WEResume';
function WEStop							      ; external DLLNAME Name 'WEStop';
function WEAbort                  ; external DLLNAME Name 'WEAbort';
function WEWait							      ; external DLLNAME Name 'WEWait';
function WEEnablePreview				  ; external DLLNAME Name 'WEEnablePreview';


function WEGetEstimateDiskSpaceEx	; external DLLNAME Name 'WEGetEstimateDiskSpaceEx';

function WESetSplitParam; external DLLNAME Name  'WESetSplitParam';
function WEGetTargetCount; external DLLNAME Name 'WEGetTargetCount';
function WEGetTargetFileName; external DLLNAME Name 'WEGetTargetFileName';


end.


