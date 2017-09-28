unit
	MenuDesigner;

interface

uses
    Windows, DVDMenuItem;

type
  HMENUDESIGNER    = Pointer;
  HMENUPAGE        = Pointer;

//HMENUDESIGNER __stdcall MDCreate();
function MDCreate() : HMENUDESIGNER; stdcall;

//void __stdcall MDDestroy(HMENUDESIGNER hMenu);
procedure MDDestroy(hMenu : HMENUDESIGNER); stdcall;

//void __stdcall MDSetCallBack(HMENUDESIGNER hMenu, void* pUsrObject, MENUMGR_CALLBACK fnCallBack);
procedure MDSetCallBack(hMenu : HMENUDESIGNER; pUsrObject : Pointer; fnCallBack : MENUMGR_CALLBACK); stdcall;

//BOOL __stdcall MDShowWindow(HMENUDESIGNER hMenu, HWND hParent, RECT rcPos);
function MDShowWindow(hMenu : HMENUDESIGNER; hParent : HWND; rcPos : TRect) : BOOL; stdcall;

//void __stdcall MDUpdateWindow(HMENUDESIGNER hMenu);
procedure MDUpdateWindow(hMenu : HMENUDESIGNER); stdcall;

//HWND __stdcall MDGetHwnd(HMENUDESIGNER hMenu);
function MDGetHwnd(hMenu : HMENUDESIGNER) : HWND; stdcall;

//HMENUPAGE __stdcall MDGetCurrentPage(HMENUDESIGNER hMenu);
function MDGetCurrentPage(hMenu : HMENUDESIGNER) : HMENUPAGE; stdcall;

//int __stdcall MDSetCurrentPage(HMENUDESIGNER hMenu, HMENUPAGE hPage);
function MDSetCurrentPage(hMenu : HMENUDESIGNER; hPage : HMENUPAGE) : Integer; stdcall;

//uItem为PAGE_FRAME或PAGE_TEXT时，uParam为Index编号，编号不合法时按0处理，其余情况无意义
//int __stdcall MDSetFocus(HMENUDESIGNER hMenu, MENUDESIGN_PAGEITEM uItem, UINT uParam);
function MDSetFocus(hMenu : HMENUDESIGNER; uItem : MENUDESIGN_PAGEITEM; uParam : UINT) : Integer; stdcall;


//菜单页操作
//HMENUPAGE __stdcall MDCreatePage();
function MDCreatePage() : HMENUPAGE; stdcall;

//HMENUPAGE __stdcall MDGetSafePageHandle(HMENUPAGE hPage);
function MDGetSafePageHandle(hPage : HMENUPAGE) : HMENUPAGE; stdcall;

//void __stdcall MDReleasePage(HMENUPAGE hPage);
procedure MDReleasePage(hPage : HMENUPAGE); stdcall;

//菜单操作
//int __stdcall MDSetTemplatePath(HMENUPAGE hPage, const wchar_t* pPath);
function MDSetTemplatePath(hPage : HMENUPAGE; const pPath : PWideChar) : Integer; stdcall;

//int __stdcall MDGetTemplatePath(HMENUPAGE hPage, wchar_t* pBuf, int cbSize);
function MDGetTemplatePath(hPage : HMENUPAGE; pBuf : PWideChar; cbSize : Integer) : Integer; stdcall;

//void __stdcall MDSetBackgndPicture(HMENUPAGE hPage, const wchar_t* pFilePath);
function MDSetBackgndPicture(hPage : HMENUPAGE; const pFilePath : PWideChar) : Integer; stdcall;

//int __stdcall MDGetBackgndPicture(HMENUPAGE hPage, wchar_t* pBuf, int cbSize);
function MDGetBackgndPicture(hPage : HMENUPAGE;  pBuf : PWideChar; cbSize : Integer) : Integer; stdcall;


//int __stdcall MDGetMaxFrameCount(HMENUPAGE hPage);
function MDGetMaxFrameCount(hPage : HMENUPAGE) : Integer; stdcall;

//int __stdcall MDGetFrameCount(HMENUPAGE hPage);
function MDGetFrameCount(hPage : HMENUPAGE) : Integer; stdcall;

//int __stdcall MDAddFrame(HMENUPAGE hPage);
function MDAddFrame(hPage : HMENUPAGE) : Integer; stdcall;

//int __stdcall MDRemoveFrame(HMENUPAGE hPage);
function MDRemoveFrame(hPage : HMENUPAGE) : Integer; stdcall;

//int __stdcall MDSetFrameImage(HMENUPAGE hPage, int nIndex, const wchar_t* pFilePath);
function MDSetFrameImage(hPage : HMENUPAGE; nIndex : Integer; const pFilePath : PWideChar) : Integer; stdcall;

//int __stdcall MDGetFrameImage(HMENUPAGE hPage, int nIndex, wchar_t* pBuf, int cbSize);
function MDGetFrameImage(hPage : HMENUPAGE;  pBuf : PWideChar; cbSize : Integer) : Integer; stdcall;

//int __stdcall MDGetFramePos(HMENUPAGE hPage, int nIndex, DOUBLE_RECT* pRect);
function MDGetFramePos(hPage : HMENUPAGE; nIndex : Integer; pRect : DOUBLE_RECT) : Integer; stdcall;

//int __stdcall MDSetThumbnailImage(HMENUPAGE hPage, int nIndex, const LPBITMAPINFO lpbi);
function MDSetThumbnailImage(hPage : HMENUPAGE; const lpbi : PBITMAPINFO) : Integer; stdcall;

//int __stdcall MDGetThumbnail(HMENUPAGE hPage, int nIndex, wchar_t* pBuf, int cbSize);
function MDGetThumbnail(hPage : HMENUPAGE;  pBuf : PWideChar; cbSize : Integer) : Integer; stdcall;

//int __stdcall MDAddButton(HMENUPAGE hPage, BUTTON_TYPE type);
function MDAddButton(hPage : HMENUPAGE; tType : BUTTON_TYPE) : Integer; stdcall;

//int __stdcall MDRemoveButton(HMENUPAGE hPage, BUTTON_TYPE type);
function MDRemoveButton(hPage : HMENUPAGE; tType : BUTTON_TYPE) : Integer; stdcall;

//int __stdcall MDSetButtonImage(HMENUPAGE hPage, BUTTON_TYPE type, const wchar_t* pFilePath);
function MDSetButtonImage(hPage : HMENUPAGE; tType : BUTTON_TYPE; const pFilePath : PWideChar) : Integer; stdcall;

//int __stdcall MDGetButtonImage(HMENUPAGE hPage, BUTTON_TYPE type, wchar_t* pBuf, int cbSize);
function MDGetButtonImage(hPage : HMENUPAGE;  tType : BUTTON_TYPE; pBuf : PWideChar; cbSize : Integer) : Integer; stdcall;

//int __stdcall MDGetButtonPos(HMENUPAGE hPage, BUTTON_TYPE type, DOUBLE_RECT* pRect);
function MDGetButtonPos(hPage : HMENUPAGE; tType : BUTTON_TYPE; pRect : DOUBLE_RECT) : Integer; stdcall;

//int __stdcall MDGetButtonState(HMENUPAGE hPage, BUTTON_STATE* pState);
function MDGetButtonState(hPage : HMENUPAGE; pState : PBUTTON_STATE) : Integer; stdcall;


//int __stdcall MDSetBackgndAudio(HMENUPAGE hPage, const wchar_t* pFilePath);
function MDSetBackgndAudio(hPage : HMENUPAGE; const pFilePath : PWideChar) : Integer; stdcall;

//int __stdcall MDGetBackgndAudio(HMENUPAGE hPage, wchar_t* pBuf, int cbSize);
function MDGetBackgndAudio(hPage : HMENUPAGE; pBuf : PWideChar; cbSize : Integer) : Integer; stdcall;

//const LPBITMAPINFO __stdcall MDGetPagePix(HMENUPAGE hPage);
function MDGetPagePix(hPage : HMENUPAGE) : PBITMAPINFO; stdcall;

//int __stdcall MDGetSupportFontCount();									//取得可用的字体个数
function MDGetSupportFontCount() : Integer; stdcall;

//int	__stdcall MDGetSupportFontName(int nIndex, wchar_t * pFontName);	//按序号取得可用的字体名称
function MDGetSupportFontName(nIndex : Integer; pFontName : PWideChar) : Integer; stdcall;

//int __stdcall MDGetTextItemCount(HMENUPAGE hPage);
function MDGetTextItemCount(hPage : HMENUPAGE) : Integer; stdcall;

//int __stdcall MDAddTextItem(HMENUPAGE hPage);
function MDAddTextItem(hPage : HMENUPAGE) : Integer; stdcall;

//int __stdcall MDRemoveCurrentTextItem(HMENUPAGE hPage);
function MDRemoveCurrentTextItem(hPage : HMENUPAGE) : Integer; stdcall;

//BOOL __stdcall MDGetCurrentTextItem(HMENUPAGE hPage, TEXT_ITEM* pTextItem);
function MDGetCurrentTextItem(hPage : HMENUPAGE; pTextItem : PTEXT_ITEM) : BOOL; stdcall;

//void __stdcall MDSetTextContent(HMENUPAGE hPage, const wchar_t* szContent);
procedure MDSetTextContent(hPage : HMENUPAGE; const szContent : PWideChar); stdcall;

//void __stdcall MDSetTextFont(HMENUPAGE hPage, const wchar_t* szFont);
procedure MDSetTextFont(hPage : HMENUPAGE; const szFont : PWideChar); stdcall;

//void __stdcall MDSetTextFontSize(HMENUPAGE hPage, int nFontSize);
procedure MDSetTextFontSize(hPage : HMENUPAGE; nFontSize : Integer); stdcall;

//void __stdcall MDSetTextColor(HMENUPAGE hPage, COLORREF uFontColor);
procedure MDSetTextColor(hPage : HMENUPAGE; uFontColor : COLORREF); stdcall;

//void __stdcall MDSetTextStyle(HMENUPAGE hPage, TEXT_STYLE style, BOOL bUse);
procedure MDSetTextStyle(hPage : HMENUPAGE; style : TEXT_STYLE; bUse : BOOL); stdcall;

//int __stdcall MDSetXmlText(HMENUPAGE hPage, const wchar_t* pText);
function MDSetXmlText(hPage : HMENUPAGE; const pText : PWideChar) : Integer; stdcall;

//int __stdcall MDGetXmlText(HMENUPAGE hPage, wchar_t* pBuf, int cbSize);
function MDGetXmlText(hPage : HMENUPAGE;  pBuf : PWideChar; cbSize : Integer) : Integer; stdcall;


const
    DLLNAME = 'WS_MenuDesigner.dll';

implementation

function  MDCreate                      ; external DLLNAME Name 'MDCreate';
procedure MDDestroy                     ; external DLLNAME Name 'MDDestroy';
procedure MDSetCallBack                 ; external DLLNAME Name 'MDSetCallBack';
function  MDShowWindow                  ; external DLLNAME Name 'MDShowWindow';
procedure MDUpdateWindow                ; external DLLNAME Name 'MDUpdateWindow';
function  MDGetHwnd                     ; external DLLNAME Name 'MDGetHwnd';
function  MDSetCurrentPage              ; external DLLNAME Name 'MDSetCurrentPage';
function  MDGetCurrentPage              ; external DLLNAME Name 'MDGetCurrentPage';
function MDSetFocus                     ; external DLLNAME Name 'MDSetFocus';

function  MDCreatePage                  ; external DLLNAME Name 'MDCreatePage';
function  MDGetSafePageHandle           ; external DLLNAME Name 'MDGetSafePageHandle';
procedure MDReleasePage                 ; external DLLNAME Name 'MDReleasePage';
function  MDSetTemplatePath             ; external DLLNAME Name 'MDSetTemplatePath';
function  MDGetTemplatePath             ; external DLLNAME Name 'MDGetTemplatePath';
function  MDSetBackgndPicture           ; external DLLNAME Name 'MDSetBackgndPicture';
function  MDGetBackgndPicture           ; external DLLNAME Name 'MDGetBackgndPicture';
function  MDGetMaxFrameCount            ; external DLLNAME Name 'MDGetMaxFrameCount';
function  MDGetFrameCount               ; external DLLNAME Name 'MDGetFrameCount';
function  MDAddFrame                    ; external DLLNAME Name 'MDAddFrame';
function  MDRemoveFrame                 ; external DLLNAME Name 'MDRemoveFrame';
function  MDSetFrameImage               ; external DLLNAME Name 'MDSetFrameImage';
function  MDGetFrameImage               ; external DLLNAME Name 'MDGetFrameImage';
function  MDSetThumbnailImage           ; external DLLNAME Name 'MDSetThumbnailImage';
function  MDGetThumbnail                ; external DLLNAME Name 'MDGetThumbnail';
function  MDAddButton                   ; external DLLNAME Name 'MDAddButton';
function  MDRemoveButton                ; external DLLNAME Name 'MDRemoveButton';
function  MDSetButtonImage              ; external DLLNAME Name 'MDSetButtonImage';
function  MDGetButtonImage              ; external DLLNAME Name 'MDGetButtonImage';
function  MDGetFramePos                 ; external DLLNAME Name 'MDGetFramePos';
function  MDGetButtonPos                ; external DLLNAME Name 'MDGetButtonPos';
function  MDGetButtonState              ; external DLLNAME Name 'MDGetButtonState';

function  MDSetBackgndAudio             ; external DLLNAME Name 'MDSetBackgndAudio';
function  MDGetBackgndAudio             ; external DLLNAME Name 'MDGetBackgndAudio';
function  MDGetPagePix                  ; external DLLNAME Name 'MDGetPagePix';

function MDGetSupportFontCount          ; external DLLNAME Name 'MDGetSupportFontCount';
function MDGetSupportFontName           ; external DLLNAME Name 'MDGetSupportFontName';
function MDGetTextItemCount             ; external DLLNAME Name 'MDGetTextItemCount';
function MDAddTextItem                  ; external DLLNAME Name 'MDAddTextItem';
function MDRemoveCurrentTextItem        ; external DLLNAME Name 'MDRemoveCurrentTextItem';
function MDGetCurrentTextItem           ; external DLLNAME Name 'MDGetCurrentTextItem';
procedure MDSetTextContent              ; external DLLNAME Name 'MDSetTextContent';
procedure MDSetTextFont                 ; external DLLNAME Name 'MDSetTextFont';
procedure MDSetTextFontSize             ; external DLLNAME Name 'MDSetTextFontSize';
procedure MDSetTextColor                ; external DLLNAME Name 'MDSetTextColor';
procedure MDSetTextStyle                ; external DLLNAME Name 'MDSetTextStyle';

function  MDSetXmlText                  ; external DLLNAME Name 'MDSetXmlText';
function  MDGetXmlText                  ; external DLLNAME Name 'MDGetXmlText';

end.

