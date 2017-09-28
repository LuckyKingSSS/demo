unit TransPrev;

interface

uses
  Windows;

type
  HTRANSPREV = Pointer;
  HSUBVIEW   = HWND;
  HVIEW      = HWND;

//创建窗口，hParent——父窗口，rcPos——窗口区域
//HTRANSPREV __stdcall TPCreate(HVIEW hParentWnd, LPRECT prcPos);
function TPCreate(hParentWnd : HVIEW; prcPos : PRect) : HTRANSPREV; stdcall;

//销毁窗口
//void __stdcall TPDestroy(HTRANSPREV hPrev);
procedure TPDestroy(hPrev : HTRANSPREV); stdcall;

// 设置转场ID
//void __stdcall TPSetTransID(HTRANSPREV hPrev, const char*  pTrID, double dTransLen = 3);
procedure TPSetTransID(hPrev : HTRANSPREV; const pTrID: PChar; dTransLen : Double = 3); stdcall;

//设置图像
//BOOL __stdcall TPSetImage(HTRANSPREV hPrev, const LPBITMAPINFO lpbiFrom, const LPBYTE lpBitsFrom, const LPBITMAPINFO lpbiTo, const LPBYTE lpBitsTo);
function TPSetImage(hPrev : HTRANSPREV; const lpbiFrom : PBITMAPINFO; const pBitsFrom : PByte; const lpbiTo : PBITMAPINFO; const pBitsTo : PByte) : BOOL; stdcall;

//BOOL __stdcall TPPlay(HTRANSPREV hPrev);
function TPPlay(hPrev : HTRANSPREV) : BOOL; stdcall;

//void __stdcall TPStop(HTRANSPREV hPrev);
procedure TPStop(hPrev : HTRANSPREV); stdcall;

// 取得预览窗口HWND句柄
//HSUBVIEW __stdcall TPGetHwnd(HTRANSPREV hPrev);
function TPGetHwnd(hPrev : HTRANSPREV) : HSUBVIEW; stdcall;

implementation

const
    DLLNAME = 'TransPreview.dll';

function  TPCreate        ; external DLLNAME Name 'TPCreate';
procedure TPDestroy       ; external DLLNAME Name 'TPDestroy';
procedure TPSetTransID    ; external DLLNAME Name 'TPSetTransID';
function  TPSetImage      ; external DLLNAME Name 'TPSetImage';
function  TPPlay          ; external DLLNAME Name 'TPPlay';
procedure TPStop          ; external DLLNAME Name 'TPStop';
function  TPGetHwnd       ; external DLLNAME Name 'TPGetHwnd';

end.


