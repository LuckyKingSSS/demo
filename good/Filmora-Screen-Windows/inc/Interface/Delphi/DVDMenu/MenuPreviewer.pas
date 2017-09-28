unit
	MenuPreviewer;

interface

uses
    Windows, DVDMenuItem;

type
  HMENUPREVIEWER  = Pointer;
  HMENUDATA       = Pointer;

//HMENUPREVIEWER __stdcall MPCreate(HWND hParent, RECT rcPos);
function MPCreate(hParent : HWND; rcPos : TRect) : HMENUPREVIEWER; stdcall;

//void __stdcall MPDestroy(HMENUPREVIEWER hPreviwer);
procedure MPDestroy(hPreviwer : HMENUPREVIEWER); stdcall;

//void __stdcall MPRegisterMsgHwnd(HMENUPREVIEWER hPreviwer, HWND hWnd);
procedure MPRegisterMsgHwnd(hPreviwer : HMENUPREVIEWER; hWnd : HWND); stdcall;

//void __stdcall MPSetMenuData(HMENUPREVIEWER hPreviwer, HMENUDATA hMenuData);
procedure MPSetMenuData(hPreviwer : HMENUPREVIEWER; hMenuData : HMENUDATA); stdcall;

//void __stdcall MPSetCurrentPage(HMENUPREVIEWER hPreviwer, int nIndex = 0);
procedure MPSetCurrentPage(hPreviwer : HMENUPREVIEWER; nIndex : Integer = 0); stdcall;

//HWND __stdcall MPGetPreviwerHwnd(HMENUPREVIEWER hPreviwer);
function MPGetPreviwerHwnd(hPreviwer : HMENUPREVIEWER) : HWND; stdcall;

const
    DLLNAME = 'WS_MenuPreviewer.dll';

implementation

function  MPCreate                  ; external DLLNAME Name 'MPCreate';
procedure MPDestroy                 ; external DLLNAME Name 'MPDestroy';
procedure MPRegisterMsgHwnd         ; external DLLNAME Name 'MPRegisterMsgHwnd';
procedure MPSetMenuData             ; external DLLNAME Name 'MPSetMenuData';
procedure MPSetCurrentPage          ; external DLLNAME Name 'MPSetCurrentPage';
function  MPGetPreviwerHwnd         ; external DLLNAME Name 'MPGetPreviwerHwnd';

end.
