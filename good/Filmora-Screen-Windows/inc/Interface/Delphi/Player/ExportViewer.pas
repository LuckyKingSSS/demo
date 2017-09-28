unit ExportViewer;

interface

uses
  Windows,YUVImage;

type
  HEXPORTVIEWER = Pointer;

//创建窗口，hParentWnd——父窗口，rcPos——窗口区域
//HEXPORTVIEWER __stdcall EVCreate(HWND hParentWnd, LPRECT prcPos);
  function EVCreate(hParentWnd: HWND; prcPos: PRect):HEXPORTVIEWER; stdcall;

//销毁窗口
//void __stdcall EVDestroy(HEXPORTVIEWER hViewer);
  procedure EVDestroy(hViewer: HEXPORTVIEWER); stdcall;

// 取得预览窗口HWND句柄
//HSUBVIEW __stdcall EVGetHwnd(HEXPORTVIEWER hViewer);
  function EVGetHwnd(hViewer: HEXPORTVIEWER): HWND; stdcall;


//显示设备无关位图
//BOOL __stdcall EVShowDib(HEXPORTVIEWER hViewer, const LPBITMAPINFO lpbi, const LPBYTE pBits = 0);
  function EVShowDib(hViewer: HEXPORTVIEWER; const lpbi: PBITMAPINFO; const pBits : PBYTE = 0): BOOL; stdcall;

//显示YUV图像
//BOOL __stdcall EVShowImage(HEXPORTVIEWER hViewer, const LPBITMAPINFO lpbi, const LPBYTE pBits = 0);
  function EVShowImage(hViewer: HEXPORTVIEWER; const lpbi: PBITMAPINFO; const pBits : PBYTE = 0): BOOL; stdcall;

//显示YUVImage图像
//BOOL __stdcall EVShowYUVImage(HEXPORTVIEWER hViewer, const YUVImage * pImage);
  function EVShowYUVImage(hViewer: HEXPORTVIEWER;const pImage:PYUVImage):boolean;stdcall;

implementation

const
  DLLNAME = 'GLPlayer.dll';

  function EVCreate 						; external DLLNAME Name 'EVCreate';
  procedure EVDestroy						; external DLLNAME Name 'EVDestroy';
  function EVShowDib						; external DLLNAME Name 'EVShowDib';
  function EVShowImage					; external DLLNAME Name 'EVShowImage';
  function EVGetHwnd					  ; external DLLNAME Name 'EVGetHwnd';
  function EVShowYUVImage       ; external DLLNAME Name 'EVShowYUVImage';
end.
 