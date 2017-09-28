unit
	DVDMakerUI;

interface

uses
    Windows;

type
  HDVDMAKER       = Pointer;
  HMENUDATA       = Pointer;
  HTITLELIST      = Pointer;

//HDVDMAKER __stdcall MKCreateDvdMaker();
function MKCreateDvdMaker(): HDVDMAKER; stdcall;

//void __stdcall MKDestroyDvdMaker(HDVDMAKER hMaker);
procedure MKDestroyDvdMaker(hMaker : HDVDMAKER); stdcall;

//void __stdcall MKRegisterMsgHwnd(HDVDMAKER hMaker, HWND hWnd);
procedure MKRegisterMsgHwnd(hMaker : HDVDMAKER; hWnd : HWND); stdcall;

//void __stdcall MKSetMenuData(HDVDMAKER hMaker, HMENUDATA hData);
procedure MKSetMenuData(hMaker : HDVDMAKER; hMenuData : HMENUDATA); stdcall;

//void __stdcall MKSetTitleList(HDVDMAKER hMaker, HTITLELIST hTitle);
procedure MKSetTitleList(hMaker : HDVDMAKER; hTitleList : HTITLELIST); stdcall;

// …Ë÷√¡Ÿ ±¬∑æ∂£¨
//void __stdcall MKSetTempPath(HDVDMAKER hMaker, const wchar_t* szTempPath);
procedure MKSetTempPath(hMaker : HDVDMAKER; const szTempPath : PWideChar); stdcall;

//void __stdcall MKSetDVDFolder(HDVDMAKER hMaker, const wchar_t *szFolderPath);
procedure MKSetDVDFolder(hMaker : HDVDMAKER; const szFolderPath : PWideChar); stdcall;

//void __stdcall MKSetTVNorm(HDVDMAKER hMaker, DWORD dwNorm);
procedure MKSetTVNorm(hMaker : HDVDMAKER; dwNorm : DWORD); stdcall;

//void __stdcall MKSetAspect(HDVDMAKER hMaker, DWORD dwAspect);
procedure MKSetAspect(hMaker : HDVDMAKER; dwAspect : DWORD); stdcall;

//void __stdcall MKSetResolution(HDVDMAKER hMaker, DWORD dwWidth, DWORD dwHeight);
procedure MKSetResolution(hMaker : HDVDMAKER; dwWidth : DWORD; dwHeight : DWORD); stdcall;

//BOOL __stdcall MKMakeDVD(HDVDMAKER hMaker);
function MKMakeDVD(hMaker : HDVDMAKER) : BOOL; stdcall;

const
    DLLNAME = 'WS_DVDMaker.dll';

implementation

function  MKCreateDvdMaker             ; external DLLNAME Name 'MKCreateDvdMaker';
procedure MKDestroyDvdMaker            ; external DLLNAME Name 'MKDestroyDvdMaker';
procedure MKRegisterMsgHwnd            ; external DLLNAME Name 'MKRegisterMsgHwnd';
procedure MKSetMenuData                ; external DLLNAME Name 'MKSetMenuData';
procedure MKSetTitleList               ; external DLLNAME Name 'MKSetTitleList';
procedure MKSetTempPath                ; external DLLNAME Name 'MKSetTempPath';
procedure MKSetDVDFolder               ; external DLLNAME Name 'MKSetDVDFolder';
procedure MKSetTVNorm                  ; external DLLNAME Name 'MKSetTVNorm';
procedure MKSetAspect                  ; external DLLNAME Name 'MKSetAspect';
procedure MKSetResolution              ; external DLLNAME Name 'MKSetResolution';
function  MKMakeDVD                    ; external DLLNAME Name 'MKMakeDVD';

end.
