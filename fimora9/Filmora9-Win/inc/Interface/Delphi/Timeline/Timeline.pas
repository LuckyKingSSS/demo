unit TimeLine;

{$MINENUMSIZE 4}

interface

uses
    Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
    Dialogs, TDoc, TMsg, TClip;

type
    HTIMELINE = Pointer;
    LPVOID = Pointer;

    TRACKCONFIG = record			// 轨道配置参数结构
        nVideoTrackCount: UINT;		// 视频轨道数量
        nAudioTrackCount: UINT;		// 音频轨道数量
        nOverlapTrackCount: UINT;	// 叠加轨道数量
 		bUseSubAudio: BOOL;			// 是否使用附属音频轨道
		bUseSubTrans: BOOL;			// 是否使用附属转场轨道
    end;
    LPTRACKCONFIG = ^TRACKCONFIG;

	TRANSIMAGE = record							// 转场图标参数结构
		szTransID : Array[0..63] of WideChar;	// 转场ID
		uBmpID : UINT;							// 资源ID
		hInst : HMODULE;						// 资源模块句柄
    end;
    LPTRANSIMAGE = ^TRANSIMAGE;


//DWORD __stdcall InitTimeline(LPVOID pvReserved); // 初始化Timeline 应用程序启动时调用
function InitTimeline(pvReserved: LPVOID) : DWORD; stdcall;

//void __stdcall UninitTimeline(); // 卸载Timeline 应用程序结束时调用
procedure UninitTimeline(); stdcall;

procedure DestroyTimeline(hTimeline: HTIMELINE); stdcall; // 销毁Timeline窗口

function CreateTimeline(hParentWnd: HWND; // 父窗口
    lpRect: PRECT; // 矩形位置指针
    lpTrackConfig: LPTRACKCONFIG; // 轨道配置数据指针
    lpReserved: LPVOID // 保留，供以后扩展用
    ): HTIMELINE; stdcall;
    

// 取得Timeline HWND句柄
//HWND __stdcall GetTimelineHwnd(HTIMELINE hTimeline);
function GetTimelineHwnd(hTimeline: HTIMELINE) : HWND; stdcall;


// 设置Timeline界面颜色配置参数  
// nID     - 颜色ID
// dwColor - 颜色值从高字节到低字节分别表示A、R、G、B的取值，其中A代表颜色的不透明度，0xFF表示全不透明，0x00表示全透明。
//BOOL __stdcall SetColors (UINT nID, DWORD dwColor);
function SetColors(nID : UINT; dwColor: DWORD) : BOOL; stdcall;


// 设置Timeline界面语言文本
// nID    - 文本ID 见TIMELINE_TEXT_ID
// lpText - 指向语言文本（UNICODE）字符串指针。
// 如果ID为0，且lpText为NULL，则还原全部系统默认语言文本。
//extern "C" BOOL __stdcall SetLanguageText (UINT nID, const wchar_t* lpText);
function SetLanguageText(nID : UINT; lpText: pWideChar) : BOOL; stdcall;

// TIMELINE_TEXT_ID
const
TTI_CLOSE					= 1;	// Close
TTI_HIDE					= 2;	// Hide
TTI_SHOW					= 3;	// Show
TTI_VIDEO					= 4;	// Video
TTI_AUDIO					= 5;	// Audio
TTI_SUBAUDIO				= 6;	// SubAudio
TTI_SUBTRANS				= 7;	// SubTrans
TTI_OVERLAP					= 8;	// Title Overlap
TTI_STORYBOARD_LOGO			= 100;	// Story Board



// 刷新Timeline界面（强制重画）
//void __stdcall UpdateTimeline(HTIMELINE hTimeline);
procedure UpdateTimeline(hTimeline: HTIMELINE); stdcall;

//DWORD __stdcall SendTimelineCommand(HTIMELINE hTimeline, UINT uMsgCode, WPARAM wParam, LPARAM lParam); // uMsgCode 见 "TimelineMsg.h" 中定义
function SendTimelineCommand(hTimeline: HTIMELINE; uMsgCode : UINT; wParam: WPARAM; lParam: LPARAM) : DWORD; stdcall; // uMsgCode 见 "TimelineMsg.pas" 中定义

//typedef DWORD(__stdcall * TIMELINE_EVENT_CALLBACK)(void* pUserObj, UINT uMsg, WPARAM wParam, LPARAM lParam); // nMsgID 见"TimelineMsg"中Callback消息定义
type
  TIMELINE_EVENT_CALLBACK = function(pUserObj: Pointer; uMsg: UINT; wParam: WPARAM; lParam: LPARAM): DWORD; stdcall; 

// 设置事件回调接口函数
//void __stdcall SetTimelineEventCallback(HTIMELINE hTimeline, TIMELINE_EVENT_CALLBACK fnEventCallback, void* pUserObj);
procedure SetTimelineEventCallback(hTimeline : HTIMELINE; fnEventCallback: TIMELINE_EVENT_CALLBACK; pUserObj: Pointer); stdcall; 

//typedef DWORD(__stdcall * TIMELINE_QUERY_API)(void* pUserObj, UINT uMsg, WPARAM wParam, LPARAM lParam); // 暂时用于查询转场小图标和文件信息 
type
  TIMELINE_QUERY_API = function(pUserObj: Pointer; uMsg: UINT; wParam: WPARAM; lParam: LPARAM): DWORD; stdcall; // 暂时用于查询转场小图标和文件信息
// uMsg: 0 - 查询缩略图  返回0为失败 应用程序应该调用TCSetDib、TCSetBitmap或TCSetHBitmap函数设置缩略图 另外若是转场 请调用TCSetLabel函数设置Label
//              lParam - HCLIP句柄
//       1 - 查询随机特效ID  返回0为失败       
//              lParam - pointer of wchar_t[64]
//		 2 - 查询媒体格式（并根据格式创建HCLIP） 返回HCLIP句柄 0为失败 创建的HCLIP句柄不要使用TCRelease释放 如果是视频请使用TCSetPhysicalLength设置视频长度
//              lParam - 指向文件名的wchar_t指针 以'\0'结束
// 设置查询回调接口函数
//void __stdcall SetTimelineQueryCallback(TIMELINE_QUERY_API fnQueryCallback, void* pUserObj); // 设置查询接口
procedure SetTimelineQueryCallback(fnQueryCallback: TIMELINE_QUERY_API; pUserObj: Pointer); stdcall; // 设置查询接口

//typedef DWORD(__stdcall * TIMELINE_LOG_API)(void* pUserObj, const wchar_t* pLogText, DWORD dwReserved); // 日志回调函数
type
  TIMELINE_LOG_API = function(pUserObj: Pointer; const pLogText: PWideChar; dwReserved: DWORD): DWORD; stdcall; 
// 设置日志回调接口函数
//void __stdcall SetTimelineLogCallback(TIMELINE_LOG_API fnLogCallback, void* pUserObj); 
procedure SetTimelineLogCallback(fnLogCallback: TIMELINE_LOG_API; pUserObj: Pointer); stdcall; 
// 日志和查询回调属于全局回调

type
	DRAW_CLIP_ITEM = record
		uViewMode : UINT;		// 视图 0 - 故事板
		nItem : Integer;		// Item 索引
		nViewIndex : Integer;		// 在视图上的可见Item索引
		hClip : HCLIP;
		hDC : HDC;
		rcItem : TRECT;
		bTransArea : BOOL;	// 是否转场区域
	end;
    LPDRAW_CLIP_ITEM = ^DRAW_CLIP_ITEM;

//typedef DWORD(__stdcall * TIMELINE_DRAWITEM_API)(void* pUserObj, WPARAM wParam, LPARAM lParam); // Clip绘制回调函数 lParam - pointer of DRAW_CLIP_ITEM
type
  TIMELINE_DRAWITEM_API = function(pUserObj: Pointer; wParam: WPARAM; lParam: LPARAM): DWORD; stdcall;
// Clip绘制回调接口函数
//void __stdcall SetTimelineDrawCallback(HTIMELINE hTimeline, TIMELINE_DRAWITEM_API fnDrawCallback, void* pUserObj); 
procedure SetTimelineDrawCallback(hTimeline : HTIMELINE; fnDrawCallback: TIMELINE_DRAWITEM_API; pUserObj: Pointer); stdcall; 


// 设置是否自动重画（当通过接口添加、删除、修改元素时是否自动重画） 默认为自动重画
//void __stdcall SetAutoRedraw(HTIMELINE hTimeline, BOOL bFlag); 
procedure SetAutoRedraw(hTimeline : HTIMELINE; bFlag : BOOL); stdcall;

// 打开、禁用事件回调
//void __stdcall EnableEventCallback(HTIMELINE hTimeline, BOOL bEnable);
procedure EnableEventCallback(hTimeline : HTIMELINE; bEnable: BOOL); stdcall; 

// 取得Timeline文档对象句柄
//HTLDOC __stdcall GetTimelineDoc(HTIMELINE hTimeline);
function GetTimelineDoc(hTimeline: HTIMELINE) : HTLDOC; stdcall; 


//==========================================================
// Timeline OleDragDrop 媒体拖入接口
// CLIPFORMAT Name定义如下：
const
 TIMELINE_MEDIA_FORAMT_NAME	= ('Timeline Media File');
 TIMELINE_IMAGE_FORAMT_NAME	= ('Timeline Image File');
 TIMELINE_VIDEO_FORAMT_NAME	= ('Timeline Video File');
 TIMELINE_AUDIO_FORAMT_NAME	= ('Timeline Audio File');
 TIMELINE_TRANS_FORAMT_NAME	= ('Timeline Transition');
 TIMELINE_TITLE_FORAMT_NAME	= ('Timeline Title');

// 其Ole数据结构同DROPFILES接口完全一致 头部内容为一DROPFILES结构体数据 后面紧跟若干文件名，文件名之间以'\0'分隔开
// tymed 均为 TYMED_HGLOBAL
// cfFormat 为以上CLIPFORMAT Name注册而得

// 示例例如：
//HGLOBAL MakeMediaClips()
//{
//	HGLOBAL hBuffer = GlobalAlloc(GPTR, sizeof(DROPFILES) + 1024 * 2 + 4 + 2);
//	LPDROPFILES pDropFiles = (LPDROPFILES)GlobalLock(hBuffer);
//
//	pDropFiles->pFiles = sizeof(DROPFILES);
//	pDropFiles->fWide=TRUE;
//
//	wchar_t* pName = (wchar_t*)((BYTE*)pDropFiles + sizeof(DROPFILES));
//	wcscpy(pName, L"G:\\beautifull.jpg");
//	pName += lstrlen(pName) + 1;
//	wcscpy(pName, L"G:\\Demo.jpg");
//
//	GlobalUnlock(hBuffer);
//
//	return hBuffer;
//}

//	DWORD cfTimelineImageFile = RegisterClipboardFormat(TIMELINE_IMAGE_FORAMT_NAME);
//	FORMATETC cImageFileFmt = {(CLIPFORMAT) cfTimelineImageFile, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
//	
//	IDataObjectImpl	DragData;
//	IDropSourceImpl DropSource;
//
//	STGMEDIUM medium = { 0 }; 
//	medium.tymed = TYMED_HGLOBAL;
//	medium.hGlobal = MakeMediaClips();
//	DragData.SetData(&cImageFileFmt, &medium, TRUE);
//
//	DWORD dwEffect = (DWORD)-1;
//	HRESULT hr = ::DoDragDrop(&DragData, &DropSource, DROPEFFECT_COPY, &dwEffect);
//

//==========================================================================================================
// PhotoViewer拖入接口
// TPhoto结构体 dwReserved 使用以下值：
const
 DRAG_TRANS_ID		= 1000;  // strReserved - 转场ID（Unicode字符串）
 DRAG_VIDEO_FILE	= 1001;  // FilePath - 文件名（Unicode字符串）
 DRAG_AUDIO_FILE	= 1002;  // FilePath - 文件名（Unicode字符串）
 DRAG_IMAGE_FILE	= 0;     // FilePath - 文件名（Unicode字符串）


implementation

const
    DLLNAME = 'WS_Timeline.dll';

function InitTimeline; external DLLNAME Name 'InitTimeline';
procedure UninitTimeline; external DLLNAME Name 'UninitTimeline';
procedure DestroyTimeline; external DLLNAME Name 'DestroyTimeline';
function CreateTimeline; external DLLNAME Name 'CreateTimeline';
function GetTimelineHwnd; external DLLNAME Name 'GetTimelineHwnd';
function SetColors; external DLLNAME Name 'SetColors';
function SetLanguageText; external DLLNAME Name 'SetLanguageText';
procedure UpdateTimeline; external DLLNAME Name 'UpdateTimeline';
function SendTimelineCommand; external DLLNAME Name 'SendTimelineCommand';
procedure SetTimelineEventCallback; external DLLNAME Name 'SetTimelineEventCallback';
procedure SetTimelineQueryCallback; external DLLNAME Name 'SetTimelineQueryCallback';
procedure SetTimelineLogCallback; external DLLNAME Name 'SetTimelineLogCallback';
procedure SetTimelineDrawCallback; external DLLNAME Name 'SetTimelineDrawCallback';
procedure SetAutoRedraw; external DLLNAME Name 'SetAutoRedraw';
procedure EnableEventCallback; external DLLNAME Name 'EnableEventCallback';
function GetTimelineDoc; external DLLNAME Name 'GetTimelineDoc';

end.

 