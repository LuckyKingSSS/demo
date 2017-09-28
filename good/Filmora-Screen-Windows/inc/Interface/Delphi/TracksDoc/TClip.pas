unit
	TClip;

{$MINENUMSIZE 4}
    
interface

uses
    Windows;

type
	HCLIP	   = Pointer; 	// BaseClip对象句柄定义。
	HIMAGECLIP = Pointer; 	// ImageClip对象句柄定义。
	HTITLECLIP = Pointer; 	// TitleClip对象句柄定义。
	HVIDEOCLIP = Pointer; 	// VideoClip对象句柄定义。
	HAUDIOCLIP = Pointer; 	// AudioClip对象句柄定义。
	HTRANSCLIP = Pointer; 	// TransClip对象句柄定义。
	
	
{$IFNDEF CLIP_PARAM_DEF}
{$DEFINE CLIP_PARAM_DEF}

// Clip类型定义
type
ClipType = 
(
	CT_NONE,
	CT_VIDEO,		// 视频 
	CT_IMAGE,		// 图像 
	CT_TITLE,		// 字幕 
	CT_AUDIO,		// 音频 
	CT_TRANSITION	// 转场 
);

// 操作状态
type
ClipWorkState =
(
	CWS_NORMAL		= $0001,		// 正常
	CWS_SELECTED	= $0002,		// 选中状态
	CWS_FOCUS		= $0004		// 拥有焦点
);

// 属性状态
type
ClipAttributeState =
(
	CAS_NORMAL		= $0001,		// 正常
	CAS_LOCKED		= $0002,		// 锁定状态
	CAS_FROZEN		= $0004		// 冻结状态
);

{$ENDIF}  //  CLIP_PARAM_DEF

{$IFNDEF IMAGE_RESIZE_METHOD_DEFINE}
{$DEFINE IMAGE_RESIZE_METHOD_DEFINE}
  IMAGE_RESIZE_METHOD =
  (
    IRM_FULLSCREEN,		        // stretch to full screen
    IRM_ORIGINAL_SCALE,			// keep scale, fit to size of output
    IRM_16_9,					// resize to 16:9, fit to size of output
    IRM_4_3,					// resize to 4:3, fit to size of output
 	IRM_LETTERBOX,
	IRM_PAN_SCAN
 );
{$ENDIF}  //  IMAGE_RESIZE_METHOD_DEFINE


//HCLIP __stdcall TCClone(HCLIP hClip);		// 复制Clip
function TCClone(hClip: HCLIP) : HCLIP; stdcall;

//HCLIP __stdcall TCCreate(ClipType nType);	// 创建Clip
function TCCreate(nType: ClipType) : HCLIP; stdcall;

//int __stdcall TCRelease(HCLIP hClip);		// 释放Clip
function TCRelease(hClip: HCLIP) : Integer; stdcall;

//int __stdcall TCAddRef(HCLIP hClip);		// 添加引用记数
function TCAddRef(hClip: HCLIP) : Integer; stdcall;


//ClipType __stdcall TCGetType(HCLIP hClip);
function TCGetType(hClip: HCLIP) : ClipType; stdcall;

//void __stdcall TCSetBeginTime(HCLIP hClip, double dBeginTime, double dLength = -1); 
procedure TCSetBeginTime(hClip: HCLIP; dBeginTime : double; dLength : double = -1); stdcall;

//double __stdcall TCGetBeginTime(HCLIP hClip);
function TCGetBeginTime(hClip: HCLIP) : double; stdcall;

//double __stdcall TCGetEndTime(HCLIP hClip);
function TCGetEndTime(hClip: HCLIP) : double; stdcall;

//double __stdcall TCGetLength(HCLIP hClip);// 获取当前长度
function TCGetLength(hClip: HCLIP) : double; stdcall;

//void __stdcall TCSetLength(HCLIP hClip, double dLength);
procedure TCSetLength(hClip: HCLIP; dLength : double); stdcall;

//DWORD __stdcall TCGetUserData(HCLIP hClip);
function TCGetUserData(hClip: HCLIP) : DWORD; stdcall;

//DWORD __stdcall TCSetUserData(HCLIP hClip, DWORD dwUserData); // 返回先前的UserData
function TCSetUserData(hClip: HCLIP; dwUserData : DWORD) : DWORD; stdcall;


//double __stdcall TCGetPhysicalLength(HCLIP hClip); // 返回Clip物理长度，仅对视频、音频Clip有效
function TCGetPhysicalLength(hClip: HCLIP) : double; stdcall;

//double __stdcall TCGetPhysicalBegin(HCLIP hClip); // 返回Clip的物理起始位置
function TCGetPhysicalBegin(hClip: HCLIP) : double; stdcall;

//void __stdcall TCSetPhysicalBegin(HCLIP hClip, double dBegin); // 设置Clip的物理起始位置
procedure TCSetPhysicalBegin(hClip: HCLIP; dBegin : double); stdcall;

//void __stdcall TCSetPhysicalLength(HCLIP hClip, double dLength); // 设置Clip的物理长度
procedure TCSetPhysicalLength(hClip: HCLIP; dLength : double); stdcall;


//extern "C" void __stdcall TCSetLabel(HCLIP hClip, const wchar_t* lpLabel); // 默认情况下：Video、Audio、Image Clip将以文件名Title作为Label，转场则用ID
procedure TCSetLabel(hClip: HCLIP; const lpLabel : PWideChar); stdcall;

//extern "C" const wchar_t* __stdcall TCGetLabel(HCLIP hClip);
function TCGetLabel(hClip: HCLIP) : PWideChar; stdcall;

//extern "C" void __stdcall TCSetFileName(HCLIP hClip, const wchar_t* lpFileName); // 仅对Video、Audio、Image Clip有效
procedure TCSetFileName(hClip: HCLIP; const lpFileName : PWideChar); stdcall;

//extern "C" const wchar_t* __stdcall TCGetFileName(HCLIP hClip);
function TCGetFileName(hClip: HCLIP) : PWideChar; stdcall;


//BOOL __stdcall TCSetBitmap(HCLIP hClip, UINT nID, HINSTANCE hInstance);  // 缩略图
function TCSetBitmap(hClip: HCLIP; nID : UINT; hModule : HMODULE) : BOOL; stdcall;

//BOOL __stdcall TCSetDib(HCLIP hClip, LPBITMAPINFO lpbi, void* pBits); // 缩略图 Clip会拷贝hBitmap数据，应用程序应该自动释放位图数据
function TCSetDib(hClip: HCLIP; lpbi : PBITMAPINFO; pBits : pointer) : BOOL; stdcall;

//BOOL __stdcall TCSetHBitmap(HCLIP hClip, HBITMAP hBitmap); // 缩略图 Clip会拷贝hBitmap数据，应用程序应该自动释放hBitmap
function TCSetHBitmap(hClip: HCLIP; hBitmap : HBITMAP) : BOOL; stdcall;

//BOOL __stdcall TCAutoLoadThumbnail(HCLIP hClip); // 自动加载缩略图 调用TIMELINE_QUERY_API回调函数 回调函数应该调用以上三个函数之一来设置，如果回调失败 Timeline将自行加载缩略图 适用于CT_IMAGE的clip
function TCAutoLoadThumbnail(hClip: HCLIP) : BOOL; stdcall;

//DWORD __stdcall TCGetAttribState(HCLIP hClip);
function TCGetAttribState(hClip: HCLIP) : DWORD; stdcall;

//DWORD __stdcall TCGetWorkState(HCLIP hClip);
function TCGetWorkState(hClip: HCLIP) : DWORD; stdcall;

//int	__stdcall TCGetIndex(HCLIP hClip); // 返回在Track中的索引值
function TCGetIndex(hClip: HCLIP) : Integer; stdcall;

//HCLIP __stdcall TCGetNext(HCLIP hClip);
function TCGetNext(hClip: HCLIP) : HCLIP; stdcall;

//HCLIP __stdcall TCGetPrev(HCLIP hClip);
function TCGetPrev(hClip: HCLIP) : HCLIP; stdcall;


// Title Clip
//extern "C" void __stdcall TTSetText(HTITLECLIP hClip, const wchar_t* lpText);
procedure TTSetText(hClip: HTITLECLIP; const lpText : PWideChar); stdcall;

//extern "C" const wchar_t* __stdcall TTGetText(HTITLECLIP hClip);
function TTGetText(hClip: HTITLECLIP) : PWideChar; stdcall;

// Transition Clip
//extern "C" void __stdcall TRSetTransID(HTRANSCLIP hClip, const wchar_t* lpTransID);
procedure TRSetTransID(hClip: HTRANSCLIP; const lpTransID : PWideChar); stdcall;

//extern "C" const wchar_t* __stdcall TRGetTransID(HTRANSCLIP hClip);
function TRGetTransID(hClip: HTRANSCLIP) : PWideChar; stdcall;



//HTRANSCLIP __stdcall TCGetClipTailTrans(HCLIP hClip); // 取得尾部转场
function TCGetClipTailTrans(hClip: HCLIP) : HTRANSCLIP; stdcall;

//HCLIP __stdcall TCGetTransBeginClip(HTRANSCLIP hTrClip); // 取得转场hTrClip前面的clip
function TCGetTransBeginClip(hTrClip: HTRANSCLIP) : HCLIP; stdcall;

//HTRANSCLIP __stdcall TCGetClipHeadTrans(HCLIP hClip); // 取得头部转场
function TCGetClipHeadTrans(hClip: HCLIP) : HTRANSCLIP; stdcall;

//HCLIP __stdcall TCGetTransEndClip(HTRANSCLIP hTrClip); // 取得转场hTrClip后面的clip
function TCGetTransEndClip(hTrClip: HTRANSCLIP) : HCLIP; stdcall;


//BOOL __stdcall TCSetDubbingFlag(HCLIP hClip, BOOL bHasDubbing);
function TCSetDubbingFlag(hClip: HCLIP; bHasDubbing : BOOL) : BOOL; stdcall;

//BOOL __stdcall TCHasDubbing(HCLIP hClip);
function TCHasDubbing(hClip: HCLIP) : BOOL; stdcall;

//void __stdcall TCSetMinLength(HCLIP hClip, double dMinLen);
procedure TCSetMinLength(hClip: HCLIP; dMinLen : double); stdcall;

//double __stdcall TCGetMinLength(HCLIP hClip);
function TCGetMinLength(hClip: HCLIP) : double; stdcall;

//void __stdcall TCSetAudioFadeIn(HCLIP hClip, double dFadeIn);
procedure TCSetAudioFadeIn(hClip: HCLIP; dFadeIn : double); stdcall;

//double __stdcall TCGetAudioFadeIn(HCLIP hClip);
function TCGetAudioFadeIn(hClip: HCLIP) : double; stdcall;

//void __stdcall TCSetAudioFadeOut(HCLIP hClip, double dFadeOut);
procedure TCSetAudioFadeOut(hClip: HCLIP; dFadeOut : double); stdcall;

//double __stdcall TCGetAudioFadeOut(HCLIP hClip);
function TCGetAudioFadeOut(hClip: HCLIP) : double; stdcall;

//void __stdcall TCSetVolume(HCLIP hClip, int nVolume);
procedure TCSetVolume(hClip: HCLIP; nVolume : Integer); stdcall;

//int __stdcall TCGetVolume(HCLIP hClip);
function TCGetVolume(hClip: HCLIP) : Integer; stdcall;

//void __stdcall TCSetVideoOnly(HCLIP hClip, BOOL bVideoOnly = TRUE);
procedure TCSetVideoOnly(hClip: HCLIP; bVideoOnly : BOOL = TRUE); stdcall;

//BOOL __stdcall TCIsVideoOnly(HCLIP hClip)
function TCIsVideoOnly(hClip: HCLIP) : BOOL; stdcall;

//void __stdcall TCSetAudioMute(HCLIP hClip, BOOL bAudioMute = TRUE);
procedure TCSetAudioMute(hClip: HCLIP; bAudioMute : BOOL = TRUE); stdcall;

//BOOL __stdcall TCIsAudioMute(HCLIP hClip)
function TCIsAudioMute(hClip: HCLIP) : BOOL; stdcall;

//void __stdcall TCSetResizeStyle(HCLIP hClip, IMAGE_RESIZE_METHOD uResizeStyle);
procedure TCSetResizeStyle(hClip: HCLIP; uResizeStyle : IMAGE_RESIZE_METHOD); stdcall;

//设置剪裁区域, pCropRect——剪裁区域, 0 为取消剪裁操作
//void __stdcall TCSetCropRect(HCLIP hClip, RECT* pCropRect);
procedure TCSetCropRect(hClip: HCLIP; pCropRect : PRECT); stdcall;

//设置亮度，-100～100，0保持原亮度
//void __stdcall TCSetBrightness(HCLIP hClip, int nValue);
procedure TCSetBrightness(hClip: HCLIP; nValue : Integer); stdcall;

//设置对比度，-100～100，0保持原对比度
//void __stdcall TCSetContrast(HCLIP hClip, int nValue);
procedure TCSetContrast(hClip: HCLIP; nValue : Integer); stdcall;

//设置饱和度,-100～100，0保持原饱和度
//void __stdcall TCSetSaturation(HCLIP hClip, int nValue); 
procedure TCSetSaturation(hClip: HCLIP; nValue : Integer); stdcall;


//IMAGE_RESIZE_METHOD __stdcall TCGetResizeStyle(HCLIP hClip);
function TCGetResizeStyle(hClip: HCLIP) : IMAGE_RESIZE_METHOD; stdcall;

//返回剪裁区域, 0 为不使用剪裁
//const RECT* __stdcall TCGetCropRect(HCLIP hClip);
function TCGetCropRect(hClip: HCLIP) : PRECT; stdcall;

//int __stdcall TCGetBrightness(HCLIP hClip);
function TCGetBrightness(hClip: HCLIP) : Integer; stdcall;

//int __stdcall TCGetContrast(HCLIP hClip);
function TCGetContrast(hClip: HCLIP) : Integer; stdcall;

//int __stdcall TCGetSaturation(HCLIP hClip); 
function TCGetSaturation(hClip: HCLIP) : Integer; stdcall;


implementation


const
    DLLNAME = 'WS_Timeline.dll';

function TCClone; external DLLNAME Name 'TCClone';
function TCCreate; external DLLNAME Name 'TCCreate';
function TCRelease; external DLLNAME Name 'TCRelease';
function TCAddRef; external DLLNAME Name 'TCAddRef';
function TCGetType; external DLLNAME Name 'TCGetType';
procedure TCSetBeginTime; external DLLNAME Name 'TCSetBeginTime';
function TCGetBeginTime; external DLLNAME Name 'TCGetBeginTime';
function TCGetEndTime; external DLLNAME Name 'TCGetEndTime';
function TCGetLength; external DLLNAME Name 'TCGetLength';
procedure TCSetLength; external DLLNAME Name 'TCSetLength';
function TCGetUserData; external DLLNAME Name 'TCGetUserData';
function TCSetUserData; external DLLNAME Name 'TCSetUserData';
function TCGetPhysicalLength; external DLLNAME Name 'TCGetPhysicalLength';
function TCGetPhysicalBegin; external DLLNAME Name 'TCGetPhysicalBegin';
procedure TCSetPhysicalBegin; external DLLNAME Name 'TCSetPhysicalBegin';
procedure TCSetPhysicalLength; external DLLNAME Name 'TCSetPhysicalLength';
procedure TCSetLabel; external DLLNAME Name 'TCSetLabel';
function TCGetLabel; external DLLNAME Name 'TCGetLabel';
procedure TCSetFileName; external DLLNAME Name 'TCSetFileName';
function TCGetFileName; external DLLNAME Name 'TCGetFileName';
function TCSetBitmap; external DLLNAME Name 'TCSetBitmap';
function TCSetDib; external DLLNAME Name 'TCSetDib';
function TCSetHBitmap; external DLLNAME Name 'TCSetHBitmap';
function TCAutoLoadThumbnail; external DLLNAME Name 'TCAutoLoadThumbnail';
function TCGetAttribState; external DLLNAME Name 'TCGetAttribState';
function TCGetWorkState; external DLLNAME Name 'TCGetWorkState';
function TCGetIndex; external DLLNAME Name 'TCGetIndex';
function TCGetNext; external DLLNAME Name 'TCGetNext';
function TCGetPrev; external DLLNAME Name 'TCGetPrev';
procedure TTSetText; external DLLNAME Name 'TTSetText';
function TTGetText; external DLLNAME Name 'TTGetText';
procedure TRSetTransID; external DLLNAME Name 'TRSetTransID';
function TRGetTransID; external DLLNAME Name 'TRGetTransID';
function TCGetClipTailTrans; external DLLNAME Name 'TCGetClipTailTrans';
function TCGetTransBeginClip; external DLLNAME Name 'TCGetTransBeginClip';
function TCGetClipHeadTrans; external DLLNAME Name 'TCGetClipHeadTrans';
function TCGetTransEndClip; external DLLNAME Name 'TCGetTransEndClip';
function TCSetDubbingFlag; external DLLNAME Name 'TCSetDubbingFlag';
function TCHasDubbing; external DLLNAME Name 'TCHasDubbing';
procedure TCSetMinLength; external DLLNAME Name 'TCSetMinLength';
function TCGetMinLength; external DLLNAME Name 'TCGetMinLength';

procedure TCSetAudioFadeIn; external DLLNAME Name 'TCSetAudioFadeIn';
function TCGetAudioFadeIn; external DLLNAME Name 'TCGetAudioFadeIn';
procedure TCSetAudioFadeOut; external DLLNAME Name 'TCSetAudioFadeOut';
function TCGetAudioFadeOut; external DLLNAME Name 'TCGetAudioFadeOut';
procedure TCSetVolume; external DLLNAME Name 'TCSetVolume';
function TCGetVolume; external DLLNAME Name 'TCGetVolume';
procedure TCSetVideoOnly; external DLLNAME Name 'TCSetVideoOnly';
function TCIsVideoOnly; external DLLNAME Name 'TCIsVideoOnly';
procedure TCSetAudioMute; external DLLNAME Name 'TCSetAudioMute';
function TCIsAudioMute; external DLLNAME Name 'TCIsAudioMute';

function TCGetResizeStyle; external DLLNAME Name 'TCGetResizeStyle';
function TCGetCropRect; external DLLNAME Name 'TCGetCropRect';
function TCGetBrightness; external DLLNAME Name 'TCGetBrightness';
function TCGetContrast; external DLLNAME Name 'TCGetContrast';
function TCGetSaturation; external DLLNAME Name 'TCGetSaturation';

procedure TCSetResizeStyle; external DLLNAME Name 'TCSetResizeStyle';
procedure TCSetCropRect; external DLLNAME Name 'TCSetCropRect';
procedure TCSetBrightness; external DLLNAME Name 'TCSetBrightness';
procedure TCSetContrast; external DLLNAME Name 'TCSetContrast';
procedure TCSetSaturation; external DLLNAME Name 'TCSetSaturation';


end.

