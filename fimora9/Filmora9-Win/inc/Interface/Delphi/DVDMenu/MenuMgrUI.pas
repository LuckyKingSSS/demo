unit
	MenuMgrUI;

interface

uses
    Windows, DVDMenuItem, MenuDesigner;

type
  HMENUMGR    = Pointer;
  HMENUDATA   = Pointer;
  HTITLELIST  = Pointer;
//  HMENUPAGE   = Pointer;

// HMENUMGR __stdcall MGCreateMenuMgr();
function MGCreateMenuMgr(): HMENUMGR; stdcall;

// void __stdcall MGDestroyMenuMgr(HMENUMGR hMenuMgr)
procedure MGDestroyMenuMgr(hMenuMgr : HMENUMGR); stdcall;

//void __stdcall MGRegisterMsgHwnd(HMENUMGR hMenuMgr, HWND hWnd);
procedure MGRegisterMsgHwnd(hMenuMgr : HMENUMGR; hWnd : HWND); stdcall;

// BOOL __stdcall MGCreateMDWindow(HMENUMGR hMenuMgr, HWND hParent, RECT rcPos);
function MGCreateMDWindow(hMenuMgr : HMENUMGR; hParent : HWND; rcPos : TRect) : BOOL; stdcall;

// void __stdcall MGUpdateMDWindow(HMENUMGR hMenuMgr);
procedure MGUpdateMDWindow(hMenuMgr : HMENUMGR); stdcall;

//void __stdcall MGShowMDWindow(HMENUMGR hMenuMgr, BOOL bShow = TRUE);
procedure MGShowMDWindow(hMenuMgr : HMENUMGR; bShow : BOOL = TRUE); stdcall;

//HWND __stdcall MGGetMDHwnd(HMENUMGR hMemuMgr);
function MGGetMDHwnd(hMenuMgr : HMENUMGR) : HWND; stdcall;

//uItem为PAGE_FRAME或PAGE_TEXT时，uParam为Index编号，编号不合法时按0处理，其余情况无意义
//void __stdcall MGSetMDFocus(HMENUMGR hMenuMgr, MENUDESIGN_PAGEITEM uItem, UINT uParam);
procedure MGSetMDFocus(hMenuMgr : HMENUMGR; uItem : MENUDESIGN_PAGEITEM; uParam : UINT); stdcall;

// 返回值为当前Frame在全部Frame中的nIndex值，若菜单设计器焦点不在Frame上，则返回-1失败
//int __stdcall MGGetCurFrameTotalIndex(HMENUMGR hMenuMgr);
function MGGetCurFrameTotalIndex(hMenuMgr : HMENUMGR) : Integer; stdcall;

//void __stdcall MGSetTitleList(HMENUMGR hMenuMgr, HTITLELIST hTitleList);
procedure MGSetTitleList(hMenuMgr : HMENUMGR; hTitleList : HTITLELIST); stdcall;

//void __stdcall MGSetMenuData(HMENUMGR hMenuMgr, HMENUDATA hMenuData);
procedure MGSetMenuData(hMenuMgr : HMENUMGR; hMenuData : HMENUDATA); stdcall;

//int __stdcall MGGetButtonState(HMENUMGR hMenuMgr, BUTTON_STATE* pBtnState);
function MGGetButtonState(hMenuMgr : HMENUMGR; pBtnState : PBUTTON_STATE) : Integer; stdcall;

//int __stdcall MGGetMenuPageCount(HMENUMGR hMenuMgr);
//function MGGetMenuPageCount(hMenuMgr : HMENUMGR) : Integer; stdcall;

//HMENUPAGE __stdcall MGGetCurrentPage(HMENUMGR hMenuMgr);
function MGGetCurrentPage(hMenuMgr : HMENUMGR) : HMENUPAGE; stdcall;

//BOOL __stdcall MGNextMenuPage(HMENUMGR hMenuMgr);
function MGNextMenuPage(hMenuMgr : HMENUMGR) : BOOL; stdcall;

//BOOL __stdcall MGSetPrevMenuPage(HMENUMGR hMenuMgr);
function MGPrevMenuPage(hMenuMgr : HMENUMGR) : BOOL; stdcall;

{
//int __stdcall MGSetBackgndAudio(HMENUMGR hMenuMgr, const wchar_t* szFilePath);
function MGSetBackgndAudio(hMenuMgr : HMENUMGR; const szFilePath : PWideChar) : Integer; stdcall;

//int __stdcall MGGetBackgndAudio(HMENUMGR hMenuMgr, wchar_t* pBuf, int cbSize);
function MGGetBackgndAudio(hMenuMgr : HMENUMGR; pBuf : PWideChar; cbSize : Integer) : Integer; stdcall;
}

//BOOL __stdcall MGSetXmlText(HMENUMGR hMenuMgr, const wchar_t* szText);
function MGSetXmlText(hMenuMgr : HMENUMGR; const szText : PWideChar) : BOOL; stdcall;

//int __stdcall MGGetXmlText(HMENUMGR hMenuMgr, wchar_t* pBuf, int cbSize);
function MGGetXmlText(hMenuMgr : HMENUMGR; pBuf : PWideChar; cbSize : Integer) : Integer; stdcall;


const
    DLLNAME = 'WS_MenuMgr.dll';

implementation

function  MGCreateMenuMgr           ; external DLLNAME Name 'MGCreateMenuMgr';
procedure MGDestroyMenuMgr          ; external DLLNAME Name 'MGDestroyMenuMgr';
procedure MGRegisterMsgHwnd         ; external DLLNAME Name 'MGRegisterMsgHwnd';
function  MGCreateMDWindow			; external DLLNAME Name 'MGCreateMDWindow';
procedure MGUpdateMDWindow          ; external DLLNAME Name 'MGUpdateMDWindow';
procedure MGShowMDWindow            ; external DLLNAME Name 'MGShowMDWindow';
function  MGGetMDHwnd		        ; external DLLNAME Name 'MGGetMDHwnd';
procedure MGSetMDFocus              ; external DLLNAME Name 'MGSetMDFocus';
function  MGGetCurFrameTotalIndex   ; external DLLNAME Name 'MGGetCurFrameTotalIndex';
procedure MGSetTitleList            ; external DLLNAME Name 'MGSetTitleList';
procedure MGSetMenuData             ; external DLLNAME Name 'MGSetMenuData';
function  MGGetButtonState          ; external DLLNAME Name 'MGGetButtonState';
function  MGGetCurrentPage          ; external DLLNAME Name 'MGGetCurrentPage';
function  MGNextMenuPage            ; external DLLNAME Name 'MGNextMenuPage';
function  MGPrevMenuPage            ; external DLLNAME Name 'MGPrevMenuPage';
function  MGSetXmlText              ; external DLLNAME Name 'MGSetXmlText';
function  MGGetXmlText              ; external DLLNAME Name 'MGGetXmlText';

end.
