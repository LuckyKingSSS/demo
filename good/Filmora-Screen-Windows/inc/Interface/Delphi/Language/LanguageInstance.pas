unit
	LanguageInstance;

// 为了方便多语言的支持，  程序界面中使用文字的地方一律使
// 用 MLIGetText(const wchar_t* lpTextID)函数获取
    
interface

uses
    Windows;

type
   HLANGUAGE = pointer;


//HLANGUAGE _stdcall MLIInit(LPCTSTR lpPrefix); // lpPrefix - 语言文件名前缀 扩展名为dat 返回可用语言数目
function MLIInit(lpPrefix : PWideChar) : HLANGUAGE; stdcall;

//void _stdcall MLIClose(HLANGUAGE hLang);
procedure MLIClose(hLang : HLANGUAGE); stdcall;

//int _stdcall MLIGetLanguageCount(HLANGUAGE hLang);
function MLIGetLanguageCount(hLang : HLANGUAGE) : Integer; stdcall;

//const wchar_t* _stdcall MLIGetLanguageName(HLANGUAGE hLang, int nLanguageIndex);
function MLIGetLanguageName(hLang : HLANGUAGE; nLanguageIndex : Integer) : PWideChar; stdcall;

//BOOL _stdcall MLILoadLanguageRes(HLANGUAGE hLang, int nLanguageIndex);
function MLILoadLanguageRes(hLang : HLANGUAGE; nLanguageIndex : Integer) : BOOL; stdcall;

//BOOL __stdcall MLILoadLanguageResByName(HLANGUAGE hLang, LPCTSTR lpName);
function MLILoadLanguageResByName(hLang : HLANGUAGE; lpName : PWideChar) : BOOL; stdcall;

//const wchar_t* __stdcall MLIGetText(HLANGUAGE hLang, LPCTSTR lpTextID);
function MLIGetText(hLang : HLANGUAGE; lpTextID: PWideChar) : PWideChar; stdcall;

//const wchar_t* __stdcall MLIGetImage(HLANGUAGE hLang, LPCTSTR lpImageID);
function MLIGetImage(hLang : HLANGUAGE; lpImageID: PWideChar) : PWideChar; stdcall;

//const wchar_t* __stdcall MLIGetTextEx(HLANGUAGE hLang, LPCTSTR lpDir, LPCTSTR lpTextID); // 指定Text目录下的相对路径 如果lpTextID或lpDir不存在， 返回0
function MLIGetTextEx(hLang : HLANGUAGE; lpDir: PWideChar; lpTextID: PWideChar) : PWideChar; stdcall;

//const wchar_t* __stdcall MLGetAttribute(LPCTSTR lpTextID, LPCTSTR lpAttribName);
function MLIGetAttribute(hLang : HLANGUAGE; lpTextID: PWideChar; lpAttribName: PWideChar) : PWideChar; stdcall;

//const wchar_t* __stdcall MLGetAttributeEx(LPCTSTR lpDir, LPCTSTR lpTextID, LPCTSTR lpAttribName); // 指定Text目录下的相对路径 如果lpTextID或lpDir不存在， 返回0
function MLIGetAttributeEx(hLang : HLANGUAGE; lpDir: PWideChar; lpTextID: PWideChar; lpAttribName: PWideChar) : PWideChar; stdcall;


implementation

const
    DLLNAME = 'WS_Language.dll';

function MLIInit							; external DLLNAME Name 'MLIInit';
procedure MLIClose							; external DLLNAME Name 'MLIClose';
function MLIGetLanguageCount				; external DLLNAME Name 'MLIGetLanguageCount';
function MLIGetLanguageName					; external DLLNAME Name 'MLIGetLanguageName';
function MLILoadLanguageRes					; external DLLNAME Name 'MLILoadLanguageRes';
function MLILoadLanguageResByName			; external DLLNAME Name 'MLILoadLanguageResByName';
function MLIGetText							; external DLLNAME Name 'MLIGetText';
function MLIGetImage						; external DLLNAME Name 'MLIGetImage';
function MLIGetTextEx						; external DLLNAME Name 'MLIGetTextEx';

function MLIGetAttribute						; external DLLNAME Name 'MLIGetAttribute';
function MLIGetAttributeEx					; external DLLNAME Name 'MLIGetAttributeEx';


end.