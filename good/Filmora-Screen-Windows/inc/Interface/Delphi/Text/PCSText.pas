{
定义文字编辑和艺术字的接口
}

unit PCSText;

interface

uses
  Windows, TextDef, Image;

//#include "TextDef.h"
//#include "Image.h"

//typedef struct tagARTTEXTINFO
//{
//	HANDLE  hInst;
//	wchar_t  szArtID[64];
//	UINT      uBmpID;
//	DWORD     dwReserve;
//}ARTTEXTINFO, * LPARTTEXTINFO;

//typedef struct tagTEXTTRANSFORM
//{
//	POINT ptLeftTop;
//	POINT ptLeftBottom;
//	POINT ptRightTop;
//	POINT ptRightBottom;
//}TEXTTRANSFORM, * LPTEXTTRANSFORM;
type
  ARTTEXTINFO = record
  hInst : THandle;
  szArtID : array[0..63] of WCHAR;
  uBmpID: UINT;
  dwReserve: DWORD;
  end;
  LPARTTEXTINFO = ^ARTTEXTINFO;
  
  TEXTTRANSFORM = record
  ptLeftTop : TPoint;
  ptLeftBottom : TPoint;
  ptRightTop : TPoint ;
  ptRightBottom : TPoint ;
  end;
  LPTEXTTRANSFORM = ^TEXTTRANSFORM ;

//  void __stdcall TextInit();
function TextInit() : void; stdcall;

// 关闭GDI+
// void __stdcall TextUninit();
function TextUninit() : void; stdcall;

//int  __stdcall GetSupportFontCount();
function GetSupportFontCount() : Integer; stdcall;

//bool __stdcall GetSupportFontName(int nIndex, wchar_t * pFontName);
function GetSupportFontName(nIndex: Integer; pFontName: PWideChar): Boolean; stdcall;

//bool __stdcall SetNormalTransform(LPTEXTTRANSFORM lp);
//function SetNormalTransform(lp : LPTEXTTRANSFORM) : Boolean; stdcall;

//bool __stdcall GenerateNormalText(HDIBIMAGE hImage, const LPTEXTITEM pItem, HDIBIMAGE hImageTexture = NULL);
//function GenerateNormalText(hImage: HDIBIMAGE; pItem: LPTEXTITEM; hImageTexture : HDIBIMAGE = nil): Boolean; stdcall;

//HDIBIMAGE __stdcall GenerateNormalText(const LPTEXTITEM pItem, LPTEXTTRANSFORM lpTextTransform, HDIBIMAGE hImageTexture = NULL);
function GenerateNormalText(const pItem: LPTEXTITEM; lpTextTransform: LPTEXTTRANSFORM; hImageTexture: HDIBIMAGE = NIL): HDIBIMAGE; stdcall;

//HDIBIMAGE __stdcall GenerateNormalTextEx(const LPTEXTITEM pItem, int width = 0, int height = 0, HDIBIMAGE hImageTexture = NULL);
function GenerateNormalTextEx(const pItem: LPTEXTITEM; width: Integer = 0; height: Integer = 0; hImageTexture: HDIBIMAGE = NIL): HDIBIMAGE; stdcall;

//int __stdcall GetArtTextCount();
function GetArtTextCount: Integer; stdcall;

//bool __stdcall GetArtTextInfo(int nIndex, LPARTTEXTINFO pInfo);
function GetArtTextInfo(nIndex: Integer; pInfo: LPARTTEXTINFO) : Boolean; stdcall;

//int __stdcall  GetIndexFromID(wchar_t * pArtID);
function GetIndexFromID(pArtID : PWideChar): Integer; stdcall;

//bool __stdcall SetArtTransform(LPTEXTTRANSFORM lp);
//function SetArtTransform(lp: LPTEXTTRANSFORM) : Boolean; stdcall;

//bool __stdcall GenerateArtText(HDIBIMAGE hImage, LPARTTEXTITEM pArtItem);
//function GenerateArtText(hImage: HDIBIMAGE; pArtItem: LPARTTEXTITEM): Boolean; stdcall;

//HDIBIMAGE __stdcall GenerateArtText(LPARTTEXTITEM pArtItem, LPTEXTTRANSFORM lpTextTransform);
function GenerateArtText(pArtItem: LPARTTEXTITEM; lpTextTransform: LPTEXTTRANSFORM) : HDIBIMAGE; stdcall;

//HDIBIMAGE __stdcall GenerateArtTextEx(LPARTTEXTITEM pArtItem, int width, int height);
function GenerateArtText(pArtItem: LPARTTEXTITEM; width: Integer; height: Integer) : HDIBIMAGE; stdcall;

implementation

const 
  DLL_NAME = 'WS_Text.dll' ;
//function SetNormalTransform       ; external DLL_NAME name  'SetNormalTransform' ;
//function SetArtTransform          ; external DLL_NAME name  'SetArtTransform'    ;
function TextInit                 ; external DLL_NAME name  'TextInit';
function TextUninit               ; external DLL_NAME name  'TextUninit';

function GenerateNormalText       ; external DLL_NAME name  'GenerateNormalText' ;
function GenerateNormalTextEx     ; external DLL_NAME name  'GenerateNormalTextEx' ;
function GetArtTextCount          ; external DLL_NAME name  'GetArtTextCount'    ;
function GetArtTextInfo           ; external DLL_NAME name  'GetArtTextInfo'     ;
function GenerateArtText          ; external DLL_NAME name  'GenerateArtText'    ;
function GenerateArtTextEx        ; external DLL_NAME name  'GenerateArtTextEx'    ;
function GetSupportFontCount      ; external DLL_NAME name  'GetSupportFontCount';
function GetSupportFontName       ; external DLL_NAME name  'GetSupportFontName' ;
function GetIndexFromID           ; external DLL_NAME name  'GetIndexFromID'     ;

end.