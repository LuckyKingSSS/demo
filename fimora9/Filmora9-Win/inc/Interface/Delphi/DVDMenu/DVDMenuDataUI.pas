unit
	DVDMenuDataUI;

interface

uses
    Windows, DVDMenuItem;

type
  HMENUDATA   = Pointer;
  HPAGEDATA       = Pointer;

//HMENUDATA __stdcall MTCreateMenuData();
function MTCreateMenuData() : HMENUDATA; stdcall;

//HMENUDATA __stdcall MTGetSafeHandle(HMENUDATA hData);
function MTGetSafeHandle(hData : HMENUDATA) : HMENUDATA; stdcall;


//void __stdcall MTReleaseMenuData(HMENUDATA hData);
procedure MTReleaseMenuData(hData : HMENUDATA); stdcall;

//void __stdcall MTSetCallBack(HMENUDATA hData, void* pUsrObject, MENUMGR_CALLBACK fnCallBack);
procedure MTSetCallBack(hData : HMENUDATA; pUsrObject : Pointer; fnCallBack : MENUMGR_CALLBACK); stdcall;


//int __stdcall MTGetMenuPageCount(HMENUDATA hData);
function MTGetMenuPageCount(hData : HMENUDATA) : Integer; stdcall;

//int __stdcall MTInsertMenuPage(HMENUDATA hData, HPAGEDATA hPage, int nIndex);
function MTInsertMenuPage(hData : HMENUDATA) : Integer; stdcall;

//int __stdcall MTRemoveMenuPage(HMENUDATA hData, int nIndex);
function MTRemoveMenuPage(hData : HMENUDATA) : Integer; stdcall;

//HPAGEDATA __stdcall MTGetMenuPage(HMENUDATA hData, int nIndex);
function MTGetMenuPage(hData : HMENUDATA; nIndex : Integer) : HPAGEDATA; stdcall;


//HPAGEDATA __stdcall MTCreateMenuPage(HMENUDATA hData);
function MTCreateMenuPage(hData : HMENUDATA) : HPAGEDATA; stdcall;

//void __stdcall MTReleaseMenuPage(HPAGEDATA hPage);
procedure MTReleaseMenuPage(hPage : HPAGEDATA); stdcall;

//int __stdcall MTGetFrameCount(HPAGEDATA hPage);
function MTGetFrameCount(hPage : HPAGEDATA) : Integer; stdcall;

//int __stdcall MTInsertFrame(HPAGEDATA hPage, int nIndex, DOUBLE_RECT pos);
function MTInsertFrame(hPage : HPAGEDATA; nIndex : Integer; pos : DOUBLE_RECT) : Integer; stdcall;

//int __stdcall MTRemoveFrame(HPAGEDATA hPage, int nIndex);
function MTRemoveFrame(hPage : HPAGEDATA; nIndex : Integer) : Integer; stdcall;

//int __stdcall MTGetFrameItem(HPAGEDATA hPage, int nIndex, FRAME_ITEM* pItem);
function MTGetFrameItem(hPage : HPAGEDATA; nIndex : Integer; pItem : FRAME_ITEM) : Integer; stdcall;

//int __stdcall MTInsertButton(HPAGEDATA hPage, BUTTON_TYPE type, DOUBLE_RECT pos);
function MTInsertButton(hPage : HPAGEDATA; tType : BUTTON_TYPE; pos : DOUBLE_RECT) : Integer; stdcall;

//int __stdcall MTRemoveButton(HPAGEDATA hPage, BUTTON_TYPE type);
function MTRemoveButton(hPage : HPAGEDATA; tType : BUTTON_TYPE) : Integer; stdcall;

//int __stdcall MTGetButtonState(HPAGEDATA hPage, BUTTON_STATE* pBtnState);
function MTGetButtonState(hPage : HPAGEDATA; pBtnState : BUTTON_STATE) : Integer; stdcall;

//int __stdcall MTGetButtonItem(HPAGEDATA hPage, BUTTON_TYPE type, BUTTON_ITEM* pItem);
function MTGetButtonItem(hPage : HPAGEDATA; tType : BUTTON_TYPE; pItem : BUTTON_ITEM) : Integer; stdcall;


//int __stdcall MTSetMenuPagePix(HPAGEDATA hPage, const wchar_t* pFilePath);
function MTSetMenuPagePix(hPage : HPAGEDATA; const pFilePath : PWideChar) : Integer; stdcall;

//int __stdcall MTGetMenuPagePix(HPAGEDATA hPage, wchar_t* pBuf, int cbSize);
function MTGetMenuPagePix(hPage : HPAGEDATA;  pBuf : PWideChar; cbSize : Integer) : Integer; stdcall;

//int __stdcall MTSetMenuPageAudio(HPAGEDATA hPage, const wchar_t* pFilePath);
function MTSetMenuPageAudio(hPage : HPAGEDATA; const pFilePath : PWideChar) : Integer; stdcall;

//int __stdcall MTGetMenuPageAudio(HPAGEDATA hPage, wchar_t* pFilePath, int cbSize);
function MTGetMenuPageAudio(hPage : HPAGEDATA; pFilePath : PWideChar; nLen : Integer) : Integer; stdcall;


const

    DLLNAME = 'WS_DVDMenuData.dll';

implementation

function  MTCreateMenuData             ; external DLLNAME Name 'MTCreateMenuData';
function  MTGetSafeHandle              ; external DLLNAME Name 'MTGetSafeHandle';
procedure MTReleaseMenuData            ; external DLLNAME Name 'MTReleaseMenuData';
procedure MTSetCallBack                ; external DLLNAME Name 'MTSetCallBack';
function  MTGetMenuPageCount           ; external DLLNAME Name 'MTGetMenuPageCount';
function  MTInsertMenuPage             ; external DLLNAME Name 'MTInsertMenuPage';
function  MTRemoveMenuPage             ; external DLLNAME Name 'MTRemoveMenuPage';
function  MTGetMenuPage                ; external DLLNAME Name 'MTGetMenuPage';
function  MTCreateMenuPage             ; external DLLNAME Name 'MTCreateMenuPage';
procedure MTReleaseMenuPage            ; external DLLNAME Name 'MTReleaseMenuPage';
function  MTGetFrameCount              ; external DLLNAME Name 'MTGetFrameCount';
function  MTInsertFrame                ; external DLLNAME Name 'MTInsertFrame';
function  MTRemoveFrame                ; external DLLNAME Name 'MTRemoveFrame';
function  MTGetFrameItem               ; external DLLNAME Name 'MTGetFrameItem';
function  MTInsertButton               ; external DLLNAME Name 'MTInsertButton';
function  MTRemoveButton               ; external DLLNAME Name 'MTRemoveButton';
function  MTGetButtonState             ; external DLLNAME Name 'MTGetButtonState';
function  MTGetButtonItem              ; external DLLNAME Name 'MTGetButtonItem';
function  MTSetMenuPagePix             ; external DLLNAME Name 'MTSetMenuPagePix';
function  MTGetMenuPagePix             ; external DLLNAME Name 'MTGetMenuPagePix';
function  MTSetMenuPageAudio           ; external DLLNAME Name 'MTSetMenuPageAudio';
function  MTGetMenuPageAudio           ; external DLLNAME Name 'MTGetMenuPageAudio'; 


end.
