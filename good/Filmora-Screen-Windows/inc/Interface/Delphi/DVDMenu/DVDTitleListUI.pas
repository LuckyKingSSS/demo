unit
	DVDTitleListUI;

interface

uses
    Windows, DVDMenuItem;

type
	HTITLELIST  = Pointer;

//HTITLELIST __stdcall TLCreate();
function TLCreate() : HTITLELIST; stdcall;

//HTITLELIST __stdcall TLGetSafeHandle(HTITLELIST hTitle);
function TLGetSafeHandle(hTitle : HTITLELIST) : HTITLELIST; stdcall;

//void __stdcall TLRlease(HTITLELIST hTitle);
procedure TLRlease(hTitle : HTITLELIST); stdcall;

//void __stdcall TLSetCallBack(HTITLELIST hTitle, void* pUsrObject, MENUMGR_CALLBACK fnCallBack);
procedure TLSetCallBack(hTitle : HTITLELIST; pUsrObject : Pointer; fnCallBack : MENUMGR_CALLBACK); stdcall;


//int __stdcall TLGetTitleCount(HTITLELIST hTitle);
function TLGetTitleCount(hTitle : HTITLELIST) : Integer; stdcall;

//int __stdcall TLInsertTitle(HTITLELIST hTitle, int nIndex);
function TLInsertTitle(hTitle : HTITLELIST; nIndex : Integer) : Integer; stdcall;

//int __stdcall TLRemoveTitle(HTITLELIST hTitle, int nIndex);
function TLRemoveTitle(hTitle : HTITLELIST; nIndex : Integer) : Integer; stdcall;

//int __stdcall TLSetTitleParam(HTITLELIST hTitle, int nIndex, DWORD dwNorm, DWORD dwAspect);
function TLSetTitleParam(hTitle : HTITLELIST; nIndex : Integer; dwNorm, dwAspect : DWORD) : Integer; stdcall;

//int __stdcall TLSetTitleName(HTITLELIST hTitle, int nIndex, const wchar_t* pTitleName);
//function TLSetTitleName(hTitle : HTITLELIST; nIndex : Integer; const pTitleName : PWideChar) : Integer; stdcall;

//int __stdcall TLGetTitleName(HTITLELIST hTitle, int nIndex, wchar_t* pBuf, int cbSize);
//function TLGetTitleName(hTitle : HTITLELIST; nIndex : Integer; pBuf : PWideChar; cbSize : Integer) : Integer; stdcall;

//int __stdcall TLSetTitleThumbnail(HTITLELIST hTitle, int nIndex, const LPBITMAPINFOHEADER lpbiHeader, const LPBYTE lpBits);
function TLSetTitleThumbnail(hTitle : HTITLELIST; nIndex : Integer; const lpbiHeader : PBitmapInfoHeader; const lpBits : PByte) : Integer; stdcall;

//LPBITMAPINFO __stdcall TLGetTitleThumbnail(HTITLELIST hTitle, int nIndex);
function TLGetTitleThumbnail(hTitle : HTITLELIST; nIndex : Integer) : PBITMAPINFO; stdcall;


const
    DLLNAME = 'WS_DVDTitleList.dll';

implementation

function  TLCreate                   ; external DLLNAME Name 'TLCreate'             ;
function  TLGetSafeHandle            ; external DLLNAME Name 'TLGetSafeHandle'      ; 
procedure TLRlease                   ; external DLLNAME Name 'TLRlease'             ;
procedure TLSetCallBack              ; external DLLNAME Name 'TLSetCallBack'        ;
function  TLGetTitleCount            ; external DLLNAME Name 'TLGetTitleCount'      ;
function  TLInsertTitle              ; external DLLNAME Name 'TLInsertTitle'        ;
function  TLRemoveTitle              ; external DLLNAME Name 'TLRemoveTitle'        ; 
function  TLSetTitleParam            ; external DLLNAME Name 'TLSetTitleParam'      ; 
//function  TLSetTitleName             ; external DLLNAME Name 'TLSetTitleName'       ;
//function  TLGetTitleName             ; external DLLNAME Name 'TLGetTitleName'       ; 
function  TLSetTitleThumbnail        ; external DLLNAME Name 'TLSetTitleThumbnail'  ; 
function  TLGetTitleThumbnail        ; external DLLNAME Name 'TLGetTitleThumbnail'  ; 


end.
