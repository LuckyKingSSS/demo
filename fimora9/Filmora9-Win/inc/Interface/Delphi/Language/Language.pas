unit
	Language;

// 为了方便多语言的支持，  程序界面中使用文字的地方一律使
// 用 MLGetText(const wchar_t* lpTextID)函数获取
    
interface

uses
    Windows;


//int _stdcall MLInit(LPCTSTR lpPrefix); // lpPrefix - 语言文件名前缀 扩展名为dat 返回可用语言数目
function MLInit(lpPrefix : PWideChar) : Integer; stdcall;

//void _stdcall MLClose();
procedure MLClose(); stdcall;

//int _stdcall MLGetLanguageCount();
function MLGetLanguageCount() : Integer; stdcall;

//const wchar_t* _stdcall MLGetLanguageName(int nLanguageIndex);
function MLGetLanguageName(nLanguageIndex : Integer) : PWideChar; stdcall;

//BOOL _stdcall MLLoadLanguageRes(int nLanguageIndex);
function MLLoadLanguageRes(nLanguageIndex : Integer) : BOOL; stdcall;

//BOOL __stdcall MLLoadLanguageResByName(LPCTSTR lpName);
function MLLoadLanguageResByName(lpName : PWideChar) : BOOL; stdcall;

//const wchar_t* __stdcall MLGetText(LPCTSTR lpTextID);
function MLGetText(lpTextID: PWideChar) : PWideChar; stdcall;

//const wchar_t* __stdcall MLGetImage(LPCTSTR lpImageID);
function MLGetImage(lpImageID: PWideChar) : PWideChar; stdcall;

//const wchar_t* __stdcall MLGetTextEx(LPCTSTR lpDir, LPCTSTR lpTextID); // 指定Text目录下的相对路径 如果lpTextID或lpDir不存在， 返回0
function MLGetTextEx(lpDir: PWideChar; lpTextID: PWideChar) : PWideChar; stdcall;

//const wchar_t* __stdcall MLGetAttribute(LPCTSTR lpTextID, LPCTSTR lpAttribName);
function MLGetAttribute(lpTextID: PWideChar; lpAttribName: PWideChar) : PWideChar; stdcall;

//const wchar_t* __stdcall MLGetAttributeEx(LPCTSTR lpDir, LPCTSTR lpTextID, LPCTSTR lpAttribName); // 指定Text目录下的相对路径 如果lpTextID或lpDir不存在， 返回0
function MLGetAttributeEx(lpDir: PWideChar; lpTextID: PWideChar; lpAttribName: PWideChar) : PWideChar; stdcall;


implementation

const
    DLLNAME = 'WS_Language.dll';

function MLInit								; external DLLNAME Name 'MLInit';
procedure MLClose							; external DLLNAME Name 'MLClose';
function MLGetLanguageCount					; external DLLNAME Name 'MLGetLanguageCount';
function MLGetLanguageName					; external DLLNAME Name 'MLGetLanguageName';
function MLLoadLanguageRes					; external DLLNAME Name 'MLLoadLanguageRes';
function MLLoadLanguageResByName			; external DLLNAME Name 'MLLoadLanguageResByName';
function MLGetText							; external DLLNAME Name 'MLGetText';
function MLGetImage							; external DLLNAME Name 'MLGetImage';
function MLGetTextEx						; external DLLNAME Name 'MLGetTextEx';

function MLGetAttribute						; external DLLNAME Name 'MLGetAttribute';
function MLGetAttributeEx					; external DLLNAME Name 'MLGetAttributeEx';


end.